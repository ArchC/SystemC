/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_object.cpp -- Abstract base class of all SystemC objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "systemc/kernel/sc_externs.h"
#include "systemc/kernel/sc_kernel_ids.h"
#include "systemc/kernel/sc_module.h"
#include "systemc/kernel/sc_object.h"
#include "systemc/kernel/sc_object_manager.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/utils/sc_hash.h"
#include "systemc/utils/sc_iostream.h"
#include "systemc/utils/sc_list.h"
#include "systemc/utils/sc_mempool.h"
#include "systemc/utils/sc_vector.h"


typedef int (*STRCMP)(const void*, const void*);

const char SC_HIERARCHY_CHAR = '.';

/* This will be gotten rid after multiple-processes
   are implemented.  This is to fix some regression
   problems. */
bool sc_enable_name_checking = true;


// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

const char*
sc_object::basename() const
{
    const char* p = strrchr( m_name, SC_HIERARCHY_CHAR );
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
    m_simc = sc_get_curr_simcontext();
    sc_object_manager* object_manager = m_simc->get_object_manager();
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
	m_ptr = new char[pn_len + strlen( newname ) + 3];
	m_name = m_ptr + 1;
        strcpy(m_name, parent->name());
        m_name[pn_len] = SC_HIERARCHY_CHAR;
        m_name[pn_len + 1] = '\0';
    } else {
	m_ptr = new char[strlen( newname ) + 3];
	m_name = m_ptr + 1;
        m_name[0] = '\0';
    }
    m_name[-1] = put_in_table;
    strcat(m_name, newname);

    if (put_in_table) {
        if (object_manager->find_object(m_name)) {
	    SC_REPORT_WARNING( SC_ID_OBJECT_EXISTS_,
                sc_string::to_string("%s. Latter"
	        " declaration will be ignored", m_name).c_str());
	} else {
            /* should check for the uniqueness of the name */
            object_manager->insert_object(m_name, this);
        }

	sc_module* curr_module = m_simc->hierarchy_curr();
	if( curr_module != 0 ) {
	    curr_module->add_child_object( this );
	} else {
	    m_simc->add_child_object( this );
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
    return (ch == SC_HIERARCHY_CHAR) || isspace(ch);
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
            } else {
                *q = *r;
            }
            r++;
            q++;
        }
        *q = '\0';
        p = namebuf;
        if (has_illegal_char)
          SC_REPORT_WARNING( SC_ID_ILLEGAL_CHARACTERS_,
			     sc_string::to_string("%s substituted by %s",
						  nm,namebuf).c_str());
    }
    sc_object_init(p);
    sc_mempool::release( namebuf, namebuf_alloc );
}

sc_object::~sc_object()
{
    if (m_name[-1] && m_simc) {
        sc_object_manager* object_manager = m_simc->get_object_manager();
        object_manager->remove_object(m_name);

	sc_module* curr_module = m_simc->hierarchy_curr();
	if( curr_module != 0 ) {
	    curr_module->remove_child_object( this );
	} else {
	    m_simc->remove_child_object( this );
	}
    }
    delete [] m_ptr;
}

void
sc_object::trace( sc_trace_file * /* unused */) const
{
    /* This space is intentionally left blank */
}


// add attribute

bool
sc_object::add_attribute( sc_attr_base& attribute_ )
{
    return ( m_attr_cltn.push_back( &attribute_ ) );
}


// get attribute by name

sc_attr_base*
sc_object::get_attribute( const sc_string& name_ )
{
    return ( m_attr_cltn[name_] );
}

const sc_attr_base*
sc_object::get_attribute( const sc_string& name_ ) const
{
    return ( m_attr_cltn[name_] );
}


// remove attribute by name

sc_attr_base*
sc_object::remove_attribute( const sc_string& name_ )
{
    return ( m_attr_cltn.remove( name_ ) );
}


// remove all attributes

void
sc_object::remove_all_attributes()
{
    m_attr_cltn.remove_all();
}


// get the number of attributes

int
sc_object::num_attributes() const
{
    return ( m_attr_cltn.size() );
}


// get the attribute collection

sc_attr_cltn&
sc_object::attr_cltn()
{
    return m_attr_cltn;
}

const sc_attr_cltn&
sc_object::attr_cltn() const
{
    return m_attr_cltn;
}
