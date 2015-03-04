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

    sc_signal.cpp - Basic definitions for sc_signal_base

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Stan Liao, Synopsys, Inc. (stanliao@synopsys.com)
    Description of Modification: Added faster update functions (991203)
    
    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/


#include <stdlib.h>
#include <assert.h>

#include "sc_iostream.h"
#include "sc_macros_int.h"
#include "sc_bvrep.h"
#include "sc_bit.h"
#include "sc_list.h"
#include "sc_vector.h"
#ifndef WIN32
#include "qt/qt.h"
#endif
#include "sc_process.h"
#include "sc_lambda.h"
#include "sc_process_int.h"
#include "sc_signal.h"
#include "sc_logic.h"
#include "sc_signal_array.h"
#include "sc_simcontext.h"

#include "sc_signal_optimize.h"

const char* sc_signal_base::kind_string = "sc_signal";

static void
update_func_abort(sc_signal_base*)
{
    /* If this function is ever invoked, that means something's
       wrong with your specialization of some signal.  Specifically,
       the update_func is not set correctly. */
    assert(false);
}

sc_signal_base::sc_signal_base( sc_object* from, bool add_to_simcontext )
    : sc_object(0)
{
    submitted = false;
    created_by = from;
    if (add_to_simcontext)
        simcontext()->add_signal(this);
    update_func = update_func_abort;
    prop_which  = SC_SIGNAL_PROP_NONE;
}

sc_signal_base::sc_signal_base( const char* nm, sc_object* from,
                                bool add_to_simcontext )
    : sc_object(nm)
{
    submitted = false;
    created_by = from;
    if (add_to_simcontext)
        simcontext()->add_signal(this);
    update_func = update_func_abort;
    prop_which  = SC_SIGNAL_PROP_NONE;
}

sc_signal_base::~sc_signal_base()
{
    /* Intentionally blank */
}

void
sc_signal_base::decl_sensitive_async( sc_async_process_handle async ) const
{
    for (int i = 0; i < sensitive_asyncs.size(); ++i) {
        /* Already in the sensitive list? */
        if (async == sensitive_asyncs[i])
            return;
    }
    /* Not yet, force it in (no need to cast away constness since
       the member is mutable.  Same below.  */
    sensitive_asyncs.push_back(async);
    sc_signal_optimize::add_asyncs_too(const_cast<sc_signal_base*>(this));
}

void
sc_signal_base::decl_sensitive_aproc( sc_aproc_process_handle aproc ) const
{
    for (int i = 0; i < sensitive_aprocs.size(); ++i) {
        /* Already in the sensitive list? */
        if (aproc == sensitive_aprocs[i])
            return;
    }
    /* Not yet, force it in */
    sensitive_aprocs.push_back(aproc);
    sc_signal_optimize::add_aprocs_too(const_cast<sc_signal_base*>(this));
}

/* Note that this function is virtual.  In sc_clock, for example, the
   real sensitive aprocs are kept inside its signal member, not in the
   clock itself.  Hence the virtualness of this function. */
void
sc_signal_base::remove_sensitive_aproc( sc_aproc_process_handle aproc ) const
{
    int j;
    for (j = sensitive_aprocs.size() - 1; j >= 0; --j) {
        if (sensitive_aprocs.fetch(j) == aproc)
            break;
    }
    assert( j >= 0 );

    /* Move the last item to the hole, and decrement sensitive_aprocs.count() */
    sc_aproc_process_handle last_item = 
        sensitive_aprocs.fetch( sensitive_aprocs.size() - 1 );
    sensitive_aprocs.put( last_item, j );
    sensitive_aprocs.decr_count();
    if (0 == sensitive_aprocs.size()) {
        sc_signal_optimize::rem_aprocs_too(const_cast<sc_signal_base*>(this));
    }
}

const char*
sc_signal_base::kind() const
{
    return kind_string;
}

void
sc_signal_base::dump(ostream& os) const
{
    sc_object::dump(os);
    os << "created by = " << created_by->name() << endl;
}

void
sc_signal_base::add_dependent_procs_to_queue_default()
{
    sc_simcontext* l_simc = simcontext();
    int j;

    j = sensitive_asyncs.size() - 1;
    if (j >= 0) {
        /* "Register-allocate" these member variables of the
           simulation context. */
        sc_async_process_handle* const l_asyncs_to_execute = l_simc->asyncs_to_execute;
        int l_lastof_asyncs_to_execute = l_simc->lastof_asyncs_to_execute;
        sc_async_process_handle* const l_sensitive_asyncs = sensitive_asyncs.raw_data();
        do {
            sc_async_process_handle async = l_sensitive_asyncs[j];
            if (! async->is_in_updateq()) {
                async->set_in_updateq( true );
                l_asyncs_to_execute[++l_lastof_asyncs_to_execute] = async;
            }
        } while (--j >= 0);
        /* Save register-allocated variables */
        l_simc->lastof_asyncs_to_execute = l_lastof_asyncs_to_execute;
    }

    j = sensitive_aprocs.size() - 1;
    if (j >= 0) {
        sc_aproc_process_handle* const l_aprocs_to_execute = l_simc->aprocs_to_execute;
        int l_lastof_aprocs_to_execute = l_simc->lastof_aprocs_to_execute;
        sc_aproc_process_handle* const l_sensitive_aprocs = sensitive_aprocs.raw_data();
        do {
            sc_aproc_process_handle aproc = l_sensitive_aprocs[j];
            if (! aproc->is_in_updateq()) {
                aproc->set_in_updateq( true );
                l_aprocs_to_execute[++l_lastof_aprocs_to_execute] = aproc;
            }
        } while (--j >= 0);
        /* Save register-allocated variables */
        l_simc->lastof_aprocs_to_execute = l_lastof_aprocs_to_execute;
    }
}



/* Some Specializations */
sc_signal<unsigned>::sc_signal()
    : sc_signal_base(this)
{
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
    update_func = &sc_signal_optimize::unsigned_update_prop_none;
}

sc_signal<unsigned>::sc_signal( const char* nm )
    : sc_signal_base(nm, this)
{
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
    update_func = &sc_signal_optimize::unsigned_update_prop_none;
}

sc_signal<unsigned>::sc_signal( sc_object* created_by )
    : sc_signal_base( created_by )
{
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
    update_func = &sc_signal_optimize::unsigned_update_prop_none;
}

sc_signal_base::UPDATE_FUNC*
sc_signal<unsigned>::update_funcs_array() const
{
    static UPDATE_FUNC funcs[] = {
        &sc_signal_optimize::unsigned_update_prop_none,
        &sc_signal_optimize::unsigned_update_prop_async,
        &sc_signal_optimize::unsigned_update_prop_aproc,
        &sc_signal_optimize::unsigned_update_prop_both,
    };
    return funcs;
}



sc_signal<sc_logic>::sc_signal()
    : sc_signal_base(this)
{
    sc_signal_init(&old_value);
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    update_func = &sc_signal_optimize::sc_logic_update_prop_none;
}

sc_signal<sc_logic>::sc_signal( const char* nm )
    : sc_signal_base(nm, this)
{
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
    update_func = &sc_signal_optimize::sc_logic_update_prop_none;
}

sc_signal<sc_logic>::sc_signal( sc_object* created_by )
    : sc_signal_base( created_by )
{
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
    update_func = &sc_signal_optimize::sc_logic_update_prop_none;
}

sc_signal_base::UPDATE_FUNC*
sc_signal<sc_logic>::update_funcs_array() const
{
    static UPDATE_FUNC funcs[] = {
        &sc_signal_optimize::sc_logic_update_prop_none,
        &sc_signal_optimize::sc_logic_update_prop_async,
        &sc_signal_optimize::sc_logic_update_prop_aproc,
        &sc_signal_optimize::sc_logic_update_prop_both
    };
    return funcs;
}
