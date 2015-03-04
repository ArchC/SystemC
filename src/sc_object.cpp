/******************************************************************************
    Copyright (c) 1996-2001 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    sc_object.cpp -- Abstract base class of all SystemC objects.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


/****************************************************************************

    The class hierarchy is as follows.  Note that sc_object is not the
    superclass of everything, notably variables (i.e. sc_array) and
    sc_async_node.  Perhaps we should make sc_object a virtual base class,
    but I didn't like the complexities of dealing with such evil stuff.

        sc_async_node   sc_object
            |       \      /  \
            |        \    /    \
            |         \  /      \
            |          \/        \
            |          /\         \
            |         /  ------    \
            |        /         \    \
            |  sc_module        \    \
            |    /    \         sc_signal_base
            |   /      \              |
            |  /        \             |
        sc_async      sc_sync   sc_signal_array_base

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <cstring>

#include "sc_iostream.h"
#include "sc_object.h"
#include "sc_hash.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_mempool.h"
#include "sc_simcontext.h"
#include "sc_object_manager.h"
#include "sc_externs.h"
#include "sc_exception.h"

typedef int (*STRCMP)(const void*, const void*);

/* This will be gotten rid after multiple-processes
   are implemented.  This is to fix some regression
   problems. */
bool sc_enable_name_checking = true;

const char*
sc_object::basename() const
{
    const char* p = strrchr( m_name, HIERARCHY_CHAR );
    return p ? (p + 1) : m_name;
}

void
sc_object::print() const
{
    print(cout);
}

void
sc_object::print(ostream& os) const
{
    os << name();
}

void
sc_object::dump() const
{
    dump(cout);
}

void
sc_object::dump(ostream& os) const
{
    os << "name = " << name() << "\n";
    os << "kind = " << kind() << "\n";
}

const char* sc_object::kind_string = "sc_object";

const char*
sc_object::kind() const
{
    return kind_string;
}

static int sc_object_num = 0;

static char*
sc_object_newname(char* name)
{
    sprintf(name, "{%d}", sc_object_num);
    sc_object_num++;
    return name;
}

void
sc_object::sc_object_init(const char* nm)
{
    /* Make the current simcontext the simcontext for this object */
    simc = sc_get_curr_simcontext();
    sc_object_manager* object_manager = simc->get_object_manager();
    sc_object* parent = object_manager->hierarchy_curr();
    char namebuf[16];
    const char* newname;
    bool put_in_table;

    if (nm && nm[0]) {
        newname = nm;
        put_in_table = true;
    }
    else {
        newname = sc_object_newname(namebuf);
        put_in_table = false;
    }

    /* Note here we pull a little trick -- use the
       first byte to store information about whether
       to put the object in the object table in the
       object manager */
    if (parent) {
        int pn_len = strlen(parent->name());
        m_name = new char[pn_len + strlen(newname) + 3];
        m_name++;
        strcpy(m_name, parent->name());
        m_name[pn_len] = HIERARCHY_CHAR;
        m_name[pn_len + 1] = '\0';
    }
    else {
        m_name = new char[strlen(newname) + 3];
        m_name++;
        m_name[0] = '\0';
    }
    m_name[-1] = put_in_table;
    strcat(m_name, newname);

    if (put_in_table) {
        if (object_manager->find_object(m_name))
          REPORT_WARNING(1021,sc_string::to_string("%s. Latter"
           " declaration will be ignored", m_name).c_str());
        else {
            /* should check for the uniqueness of the name */
            object_manager->insert_object(m_name, this);
        }
    }
}

sc_object::sc_object()
{
    sc_object_init(0);
}

static bool
object_name_illegal_char(char ch)
{
    return (ch == HIERARCHY_CHAR) || isspace(ch);
}

sc_object::sc_object(const char* nm)
{
    int namebuf_alloc = 0;
    char* namebuf = 0;

    const char* p = nm;
    if (nm && sc_enable_name_checking) {
        namebuf_alloc = 1 + strlen(nm);
        namebuf = (char*) sc_mempool::allocate(namebuf_alloc);
        char* q = namebuf;
        const char* r = nm;
        bool has_illegal_char = false;
        while (*r) {
            if (object_name_illegal_char(*r)) {
                has_illegal_char = true;
                *q = '_';
            }
            else {
                *q = *r;
            }
            r++;
            q++;
        }
        *q = '\0';
        p = namebuf;
        if (has_illegal_char)
          REPORT_WARNING(1022,sc_string::to_string("%s substituted by %s",
                         nm,namebuf).c_str());
    }
    sc_object_init(p);
    sc_mempool::release( namebuf, namebuf_alloc );
}

sc_object::~sc_object()
{
    if (m_name[-1] && simc) {
        sc_object_manager* object_manager = simc->get_object_manager();
        object_manager->remove_object(m_name);
    }
    --m_name;
    delete[] m_name;
}

void
sc_object::trace( sc_trace_file * /* unused */) const
{
    /* This space is intentionally left blank */
}
