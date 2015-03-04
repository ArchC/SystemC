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

    sc_signal_edgy.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Stan Liao. Synopsys, Inc. (stanliao@synopsy.com)
    Description of Modification: Added faster update functions. (991203)

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "sc_iostream.h"
#include "sc_macros_int.h"
#include "sc_bvrep.h"
#include "sc_bit.h"
#include "sc_signal.h"
#include "sc_signal_sc_bit.h"

#ifndef WIN32
#include "qt/qt.h"
#endif

#include "sc_process.h"
#include "sc_lambda.h"
#include "sc_process_int.h"

#include "sc_signal_optimize.h"

sc_signal_edgy::~sc_signal_edgy()
{
    /* EMPTY */
}


void
sc_signal_edgy::decl_sensitive_async( sc_async_process_handle async ) const
{
    decl_sensitive_async_pos(async);
    decl_sensitive_async_neg(async);
}

void
sc_signal_edgy::decl_sensitive_async_pos( sc_async_process_handle async ) const
{
    sc_signal_base::decl_sensitive_async( async );
}

void
sc_signal_edgy::decl_sensitive_async_neg( sc_async_process_handle async ) const 
{
    for (int i = 0; i < sensitive_asyncs_neg.size(); ++i) {
        /* Already in the sensitive list? */
        if (async == sensitive_asyncs_neg[i])
            return;
    }
    /* Not yet, force it in */
    sensitive_asyncs_neg.push_back(async);
    sc_signal_optimize::add_asyncs_too_neg(const_cast<sc_signal_edgy*>(this));
}

void
sc_signal_edgy::decl_sensitive_aproc( sc_aproc_process_handle aproc ) const
{
    decl_sensitive_aproc_pos(aproc);
    decl_sensitive_aproc_neg(aproc);
}

void
sc_signal_edgy::decl_sensitive_aproc_pos( sc_aproc_process_handle aproc ) const
{
    sc_signal_base::decl_sensitive_aproc( aproc );
}

void
sc_signal_edgy::decl_sensitive_aproc_neg( sc_aproc_process_handle aproc ) const 
{
    for (int i = 0; i < sensitive_aprocs_neg.size(); ++i) {
        /* Already in the sensitive list? */
        if (aproc == sensitive_aprocs_neg[i])
            return;
    }
    /* Not yet, force it in */
    sensitive_aprocs_neg.push_back(aproc);
    sc_signal_optimize::add_aprocs_too_neg(const_cast<sc_signal_edgy*>(this));
}

void
sc_signal_edgy::add_dependent_procs_to_queue_neg()
{
    sc_simcontext* l_simc = simcontext();
    int j;

    j = sensitive_asyncs_neg.size() - 1;
    if (j >= 0) {
        /* "Register-allocate" these member variables of the
           simulation context. */
        sc_async_process_handle* const l_asyncs_to_execute = l_simc->asyncs_to_execute;
        int l_lastof_asyncs_to_execute = l_simc->lastof_asyncs_to_execute;
        sc_async_process_handle* const l_sensitive_asyncs_neg = sensitive_asyncs_neg.raw_data();
        do {
            sc_async_process_handle async = l_sensitive_asyncs_neg[j];
            if (! async->is_in_updateq()) {
                async->set_in_updateq( true );
                l_asyncs_to_execute[++l_lastof_asyncs_to_execute] = async;
            }
        } while (--j >= 0);
        /* Save register-allocated variables */
        l_simc->lastof_asyncs_to_execute = l_lastof_asyncs_to_execute;
    }

    j = sensitive_aprocs_neg.size() - 1;
    if (j >= 0) {
        sc_aproc_process_handle* const l_aprocs_to_execute = l_simc->aprocs_to_execute;
        int l_lastof_aprocs_to_execute = l_simc->lastof_aprocs_to_execute;
        sc_aproc_process_handle* const l_sensitive_aprocs_neg = sensitive_aprocs_neg.raw_data();
        do {
            sc_aproc_process_handle aproc = l_sensitive_aprocs_neg[j];
            if (! aproc->is_in_updateq()) {
                aproc->set_in_updateq( true );
                l_aprocs_to_execute[++l_lastof_aprocs_to_execute] = aproc;
            }
        } while (--j >= 0);
        /* Save register-allocated variables */
        l_simc->lastof_aprocs_to_execute = l_lastof_aprocs_to_execute;
    }
}

void
sc_signal_edgy::remove_sensitive_aproc_neg( sc_aproc_process_handle aproc ) const
{
    /* Find out where this aproc lives in the signals's aproc list */
    int j;
    for (j = sensitive_aprocs_neg.size() - 1; j >= 0; --j) {
        if (sensitive_aprocs_neg.fetch(j) == aproc)
            break;
    }
    /* This process should be on the sensitivity list of the signal! */
    assert( j >= 0 );
    
    /* Move the last item to the hole, and decrement sensitive_aprocs.count() */
    sc_aproc_process_handle last_item = 
        sensitive_aprocs_neg.fetch( sensitive_aprocs_neg.size() - 1 );
    sensitive_aprocs_neg.put( last_item, j );
    sensitive_aprocs_neg.decr_count();
    if (0 == sensitive_aprocs_neg.size()) {
        sc_signal_optimize::rem_aprocs_too_neg(const_cast<sc_signal_edgy*>(this));
    }
}



sc_signal<bool>::sc_signal()
    : sc_signal_edgy(this)
{
#ifndef WIN32
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
#else
    cur_value &= true;
    new_value &= true;
    old_value &= true;
#endif
    update_func = &sc_signal_optimize::bool_update_prop_none;
}


sc_signal<bool>::sc_signal( const char* nm )
    : sc_signal_edgy(nm, this)
{
#ifndef WIN32
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
#else
    cur_value &= true;
    new_value &= true;
    old_value &= true;
#endif
    update_func = &sc_signal_optimize::bool_update_prop_none;
}


sc_signal<bool>::sc_signal( sc_object* created_by )
    : sc_signal_edgy( created_by )
{
#ifndef WIN32
    sc_signal_init(&cur_value);
    sc_signal_init(&new_value);
    sc_signal_init(&old_value);
#else
    cur_value &= true;
    new_value &= true;
    old_value &= true;
#endif
    update_func = &sc_signal_optimize::bool_update_prop_none;
}



bool
sc_signal<bool>::edgy_read() const
{
    return cur_value;
}


sc_signal_base::UPDATE_FUNC*
sc_signal<bool>::update_funcs_array() const
{
    static sc_signal_base::UPDATE_FUNC funcs[] = {
        &sc_signal_optimize::bool_update_prop_none,
        &sc_signal_optimize::bool_update_prop_async,
        &sc_signal_optimize::bool_update_prop_aproc,
        &sc_signal_optimize::bool_update_prop_both,
        &sc_signal_optimize::bool_update_prop_async_neg,
        &sc_signal_optimize::bool_update_prop_async_neg_async,
        &sc_signal_optimize::bool_update_prop_async_neg_aproc,
        &sc_signal_optimize::bool_update_prop_async_neg_both,
        &sc_signal_optimize::bool_update_prop_aproc_neg,
        &sc_signal_optimize::bool_update_prop_aproc_neg_async,
        &sc_signal_optimize::bool_update_prop_aproc_neg_aproc,
        &sc_signal_optimize::bool_update_prop_aproc_neg_both,
        &sc_signal_optimize::bool_update_prop_both_neg,
        &sc_signal_optimize::bool_update_prop_both_neg_async,
        &sc_signal_optimize::bool_update_prop_both_neg_aproc,
        &sc_signal_optimize::bool_update_prop_both_neg_both
    };
    return funcs;
}




bool
sc_signal<sc_bit>::edgy_read() const
{
    return cur_value.operator bool();
}


sc_signal<sc_bit>&
sc_signal<sc_bit>::write( bool d )
{
    if (cur_value.is_bitref()) {
        sc_bvrep_set_wb((sc_bvrep*) new_value.rep, 0, cur_value.val_or_index);
        if (d != sc_bvrep_test_wb((const sc_bvrep*) cur_value.rep,
                                  0, cur_value.val_or_index))
            submit_update();
    } else {
        new_value.value = d;
        if (d != (bool)cur_value.val_or_index)
            submit_update();
    }
    return *this;
}            


bool
sc_signal<sc_bit>::event() const
{
    bool cv, ov;
    if (cur_value.is_bitref()) {
        cv = sc_bvrep_test_wb((const sc_bvrep*) cur_value.rep,
                              0, cur_value.val_or_index);
        ov = sc_bvrep_test_wb((const sc_bvrep*) old_value.rep,
                              0, cur_value.val_or_index);
        return (cv != ov);
    } else {
        return (cur_value.val_or_index != old_value.value);
    }
}


bool
sc_signal<sc_bit>::posedge() const
{
    bool cv, ov;
    if (cur_value.is_bitref()) {
        cv = sc_bvrep_test_wb((const sc_bvrep*) cur_value.rep,
                              0, cur_value.val_or_index);
        ov = sc_bvrep_test_wb((const sc_bvrep*) old_value.rep,
                              0, cur_value.val_or_index);
        return (cv && !ov);
    } else {
        return (cur_value.val_or_index && !old_value.value);
    }
}


bool
sc_signal<sc_bit>::negedge() const
{
    bool cv, ov;
    if (cur_value.is_bitref()) {
        cv = sc_bvrep_test_wb((const sc_bvrep*) cur_value.rep,
                              0, cur_value.val_or_index);
        ov = sc_bvrep_test_wb((const sc_bvrep*) old_value.rep,
                              0, cur_value.val_or_index);
        return (!cv && ov);
    } else {
        return (!cur_value.val_or_index && old_value.value);
    }
}


void
sc_signal<sc_bit>::print(ostream& os) const
{
    os << cur_value;
}


void
sc_signal<sc_bit>::dump(ostream& os) const
{
    bool cv, ov, nv;
    if (cur_value.is_bitref()) {
        cv = sc_bvrep_test_wb((const sc_bvrep*) cur_value.rep,
                              0, cur_value.val_or_index);
        ov = sc_bvrep_test_wb((const sc_bvrep*) old_value.rep,
                              0, cur_value.val_or_index);
        nv = sc_bvrep_test_wb((const sc_bvrep*) new_value.rep,
                              0, cur_value.val_or_index);
    } else {
        cv = cur_value.val_or_index;
        ov = old_value.value;
        nv = new_value.value;
    }
    sc_signal_base::dump(os);
    os << "value = " << cv << endl;
    os << "old value = " << ov << endl;
    os << "new value = " << nv << endl;
}


void
sc_signal<sc_bit>::trace( sc_trace_file* tf ) const
{
    /* TBD: FILL ME */
}


void
sc_signal<sc_bit>::update()
{
  if(cur_value!=(int)new_value.value)
  {
    bool nv;
    if (cur_value.is_bitref())
    {
        nv = sc_bvrep_test_wb((const sc_bvrep*) new_value.rep,
                              0, cur_value.val_or_index);
        sc_bvrep_set_wb((sc_bvrep*) cur_value.rep,
                        0, cur_value.val_or_index, nv);
    } 
    else
        cur_value.val_or_index = nv = new_value.value;
    
    if (nv)  /* posedge */
        add_dependent_procs_to_queue_pos();
    else  /* negedge */
        add_dependent_procs_to_queue_neg();
   
  }
}


void
sc_signal<sc_bit>::update_old()
{
    bool cv;
    if (cur_value.is_bitref()) {
        cv = sc_bvrep_test_wb((const sc_bvrep*) cur_value.rep,
                              0, cur_value.val_or_index);
        sc_bvrep_set_wb((sc_bvrep*) old_value.rep,
                        0, cur_value.val_or_index, cv);
    } else {
        old_value.value = cur_value.val_or_index;
    }
}


sc_signal<sc_bit>::sc_signal()
    : sc_signal_edgy( this )
{
    new_value.value &= 1;
    old_value.value &= 1;
    update_func = &sc_signal_optimize::sc_bit_update_prop_none;
    prop_which = SC_SIGNAL_PROP_NONE;
}


sc_signal<sc_bit>::sc_signal( const char* nm )
    : sc_signal_edgy( nm, this )
{
    new_value.value &= 1;
    old_value.value &= 1;
    update_func = &sc_signal_optimize::sc_bit_update_prop_none;
    prop_which = SC_SIGNAL_PROP_NONE;
}

#if 0

sc_signal<sc_bit>::sc_signal( sc_signal_bv_b* sig_bv, unsigned index )
    : sc_signal_edgy( /*created_by*/ sig_bv )
{
    /* TBD: FILL ME */
}
#endif


sc_signal_base::UPDATE_FUNC*
sc_signal<sc_bit>::update_funcs_array() const
{
    static sc_signal_base::UPDATE_FUNC funcs[] = {
        &sc_signal_optimize::sc_bit_update_prop_none,
        &sc_signal_optimize::sc_bit_update_prop_async,
        &sc_signal_optimize::sc_bit_update_prop_aproc,
        &sc_signal_optimize::sc_bit_update_prop_both,
        &sc_signal_optimize::sc_bit_update_prop_async_neg,
        &sc_signal_optimize::sc_bit_update_prop_async_neg_async,
        &sc_signal_optimize::sc_bit_update_prop_async_neg_aproc,
        &sc_signal_optimize::sc_bit_update_prop_async_neg_both,
        &sc_signal_optimize::sc_bit_update_prop_aproc_neg,
        &sc_signal_optimize::sc_bit_update_prop_aproc_neg_async,
        &sc_signal_optimize::sc_bit_update_prop_aproc_neg_aproc,
        &sc_signal_optimize::sc_bit_update_prop_aproc_neg_both,
        &sc_signal_optimize::sc_bit_update_prop_both_neg,
        &sc_signal_optimize::sc_bit_update_prop_both_neg_async,
        &sc_signal_optimize::sc_bit_update_prop_both_neg_aproc,
        &sc_signal_optimize::sc_bit_update_prop_both_neg_both
    };
    return funcs;
}
