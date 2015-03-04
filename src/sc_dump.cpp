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

    sc_dump.cpp - Provides dumping in VCD format.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <assert.h>
#include <sys/types.h>
#include <time.h>

#include "sc_iostream.h"

#if defined(SC_VC6_SCL) && defined(_MSC_VER) || defined(__BCPLUSPLUS__) || defined(__GNUC__)
#include <fstream>
#else
#include <fstream.h>
#endif

#include "sc_object.h"
#include "sc_simcontext.h"
#include "sc_dump.h"
#include "sc_hash.h"
#include "sc_vector.h"
#include "sc_ver.h"
#include "sc_macros.h"
#include "sc_clock.h"
#include "sc_mempool.h"
#include "sc_trace.h"
#include "sc_vcd_trace.h"

sc_trace_file* the_dumpfile = 0;
static char* dumpfile_name = 0;

const char CODE_LOW  = 33;
const char CODE_HIGH = 126;

static char curr_code[16] = {
    CODE_LOW, CODE_LOW, CODE_LOW, CODE_LOW,
    CODE_LOW, CODE_LOW, CODE_LOW, CODE_LOW,
    CODE_LOW, CODE_LOW, CODE_LOW, CODE_LOW,
    CODE_LOW, CODE_LOW, CODE_LOW, CODE_LOW
}; /* this should be more than enough */
static int curr_len = 1;   /* length of the code */

// This function is kept here for backward compatibility.

void
sc_dumpfile(const char* fname)
{
    if (the_dumpfile)
        REPORT_WARNING(1011,sc_string::to_string("%s request ignored",
          dumpfile_name).c_str());
    else {
        if (fname && fname[0]) {
            dumpfile_name = new char[strlen(fname)+1];
            strcpy( dumpfile_name, fname );
        }
        else {
            dumpfile_name = new char[L_tmpnam + 6];
            // default name
            strcpy( dumpfile_name, "unnamed_dumpfile" );
            REPORT_WARNING(1012,sc_string::to_string("using %s",dumpfile_name).c_str());
        }
        the_dumpfile = new vcd_trace_file(dumpfile_name);

        for (int i = 0; i < 16; ++i)
            curr_code[i] = CODE_LOW;
        curr_len = 1;

        sc_get_curr_simcontext()->add_trace_file(the_dumpfile);
    }
}

// This function is here for backward compatibility

void
sc_dumpclose()
{
    // delete dumpfile;   -- the dumpfile is deleted by the simulation context
    delete[] dumpfile_name;
    the_dumpfile = 0;
    dumpfile_name = 0;
}


/******************************************************************************
      These functions need to be there
*******************************************************************************/

void
sc_dumpall()
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    sc_object* obj = simc->first_object();
    while (obj) {
        sc_dumpvar(*obj);
        obj = simc->next_object();
    }
}

void
sc_dumpall(sc_trace_file *f)
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    sc_object* obj = simc->first_object();
    while (obj) {
        sc_dumpvar(*obj, f);
        obj = simc->next_object();
    }
}

void
sc_dumpvar(sc_object& obj)
{
    obj.trace( the_dumpfile );
}

void
sc_dumpvar(sc_object& obj, sc_trace_file *f)
{
    obj.trace( f );
}

void
sc_dumpvars(sc_object& obj)
{
    sc_dumpvar(obj);
}

void
sc_dumpvars(sc_object& obj, sc_trace_file *f)
{
    sc_dumpvar(obj, f);
}

void
sc_dumpvars(const char* objs_string)
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    const char* p = objs_string;
    const char* q = p;
    while (*q) {
        /* skip white spaces */
        while (*q && isspace(*q))
            q++;
        if (! *q) break;
        /* reset position */
        p = q;
        while (*q && !isspace(*q))
            q++;
        int r_alloc = q - p + 1;
        char* r = (char*) sc_mempool::allocate(r_alloc);
        strncpy(r, p, q - p);
        r[q - p] = '\0';
        sc_object* obj = simc->find_object(r);
        if (obj)
          sc_dumpvar(*obj);
        else
          REPORT_WARNING(1013,r);
        sc_mempool::release(r, r_alloc);
    }
}

void
sc_dumpvars(const char* objs_string, sc_trace_file *f)
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    const char* p = objs_string;
    const char* q = p;
    while (*q) {
        /* skip white spaces */
        while (*q && isspace(*q))
            q++;
        if (! *q) break;
        /* reset position */
        p = q;
        while (*q && !isspace(*q))
            q++;
        int r_alloc = q - p + 1;
        char* r = (char*) sc_mempool::allocate(r_alloc);
        strncpy(r, p, q - p);
        r[q - p] = '\0';
        sc_object* obj = simc->find_object(r);
        if (obj)
          sc_dumpvar(*obj, f);
        else
          REPORT_WARNING(1013,r);
        sc_mempool::release(r, r_alloc);
    }
}
