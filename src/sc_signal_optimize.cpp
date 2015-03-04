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

    sc_signal_optimize.cpp - Optimize the signal->async/aproc propagation
                             functions.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

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
#include "sc_signal_sc_bit.h"
#include "sc_logic.h"
#include "sc_signal_array.h"
#include "sc_clock.h"
#include "sc_clock_int.h"
#include "sc_simcontext.h"

#include "sc_signal_optimize.h"

/*
 *  Provide this function just in case we use
 *  a non-2's-complement machine (unlikely).
 */
static unsigned
sanitize(unsigned bits)
{
    return bits;
}

void
sc_signal_base::prop_asyncs_only()
{
    sc_simcontext* l_simc = this->simcontext();
    int j;

    j = this->sensitive_asyncs.size() - 1;
    
    /* "Register-allocate" these member variables of the
       simulation context. */
    sc_async_process_handle* const l_asyncs_to_execute = l_simc->asyncs_to_execute;
    int l_lastof_asyncs_to_execute = l_simc->lastof_asyncs_to_execute;
    sc_async_process_handle* const l_sensitive_asyncs = this->sensitive_asyncs.raw_data();
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

void
sc_signal_base::prop_aprocs_only()
{
    sc_simcontext* l_simc = this->simcontext();
    int j;

    j = this->sensitive_aprocs.size() - 1;

    sc_aproc_process_handle* const l_aprocs_to_execute = l_simc->aprocs_to_execute;
    int l_lastof_aprocs_to_execute = l_simc->lastof_aprocs_to_execute;
    sc_aproc_process_handle* const l_sensitive_aprocs = this->sensitive_aprocs.raw_data();
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

void
sc_signal_base::prop_asyncs_aprocs()
{
    sc_simcontext* l_simc = this->simcontext();
    int j;

    j = this->sensitive_asyncs.size() - 1;
    /* "Register-allocate" these member variables of the
       simulation context. */
    sc_async_process_handle* const l_asyncs_to_execute = l_simc->asyncs_to_execute;
    int l_lastof_asyncs_to_execute = l_simc->lastof_asyncs_to_execute;
    sc_async_process_handle* const l_sensitive_asyncs = this->sensitive_asyncs.raw_data();
    do {
        sc_async_process_handle async = l_sensitive_asyncs[j];
        if (! async->is_in_updateq()) {
            async->set_in_updateq( true );
            l_asyncs_to_execute[++l_lastof_asyncs_to_execute] = async;
        }
    } while (--j >= 0);
    /* Save register-allocated variables */
    l_simc->lastof_asyncs_to_execute = l_lastof_asyncs_to_execute;

    j = this->sensitive_aprocs.size() - 1;
    sc_aproc_process_handle* const l_aprocs_to_execute = l_simc->aprocs_to_execute;
    int l_lastof_aprocs_to_execute = l_simc->lastof_aprocs_to_execute;
    sc_aproc_process_handle* const l_sensitive_aprocs = this->sensitive_aprocs.raw_data();
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

void
sc_signal_optimize::add_asyncs_too(sc_signal_base* p_signal)
{
    sc_signal_base::UPDATE_FUNC* update_funcs = p_signal->update_funcs_array();
    p_signal->prop_which |= SC_SIGNAL_PROP_ASYNC;
    p_signal->update_func = update_funcs[sanitize(p_signal->prop_which)];
}

void
sc_signal_optimize::add_aprocs_too(sc_signal_base* p_signal)
{
    sc_signal_base::UPDATE_FUNC* update_funcs = p_signal->update_funcs_array();
    p_signal->prop_which |= SC_SIGNAL_PROP_APROC;
    p_signal->update_func = update_funcs[sanitize(p_signal->prop_which)];
}

void
sc_signal_optimize::rem_aprocs_too(sc_signal_base* p_signal)
{
    sc_signal_base::UPDATE_FUNC* update_funcs = p_signal->update_funcs_array();
    p_signal->prop_which &= ~SC_SIGNAL_PROP_APROC;
    p_signal->update_func = update_funcs[sanitize(p_signal->prop_which)];
}



void
sc_signal_edgy::prop_asyncs_only_neg()
{
    sc_simcontext* l_simc = this->simcontext();
    int j;

    j = this->sensitive_asyncs_neg.size() - 1;

    /* "Register-allocate" these member variables of the
       simulation context. */
    sc_async_process_handle* const l_asyncs_to_execute = l_simc->asyncs_to_execute;
    int l_lastof_asyncs_to_execute = l_simc->lastof_asyncs_to_execute;
    sc_async_process_handle* const l_sensitive_asyncs = this->sensitive_asyncs_neg.raw_data();
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

void
sc_signal_edgy::prop_aprocs_only_neg()
{
    sc_simcontext* l_simc = this->simcontext();
    int j;

    j = this->sensitive_aprocs_neg.size() - 1;

    sc_aproc_process_handle* const l_aprocs_to_execute = l_simc->aprocs_to_execute;
    int l_lastof_aprocs_to_execute = l_simc->lastof_aprocs_to_execute;
    sc_aproc_process_handle* const l_sensitive_aprocs = this->sensitive_aprocs_neg.raw_data();
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

void
sc_signal_edgy::prop_asyncs_aprocs_neg()
{
    sc_simcontext* l_simc = this->simcontext();
    int j;

    j = this->sensitive_asyncs_neg.size() - 1;
    /* "Register-allocate" these member variables of the
       simulation context. */
    sc_async_process_handle* const l_asyncs_to_execute = l_simc->asyncs_to_execute;
    int l_lastof_asyncs_to_execute = l_simc->lastof_asyncs_to_execute;
    sc_async_process_handle* const l_sensitive_asyncs = this->sensitive_asyncs_neg.raw_data();
    do {
        sc_async_process_handle async = l_sensitive_asyncs[j];
        if (! async->is_in_updateq()) {
            async->set_in_updateq( true );
            l_asyncs_to_execute[++l_lastof_asyncs_to_execute] = async;
        }
    } while (--j >= 0);
    /* Save register-allocated variables */
    l_simc->lastof_asyncs_to_execute = l_lastof_asyncs_to_execute;

    j = this->sensitive_aprocs_neg.size() - 1;
    sc_aproc_process_handle* const l_aprocs_to_execute = l_simc->aprocs_to_execute;
    int l_lastof_aprocs_to_execute = l_simc->lastof_aprocs_to_execute;
    sc_aproc_process_handle* const l_sensitive_aprocs = this->sensitive_aprocs_neg.raw_data();
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

void
sc_signal_optimize::add_asyncs_too_neg(sc_signal_edgy* p_edgy)
{
    sc_signal_base::UPDATE_FUNC* update_funcs = p_edgy->update_funcs_array();
    p_edgy->prop_which |= SC_SIGNAL_PROP_ASYNC_NEG;
    p_edgy->update_func = update_funcs[sanitize(p_edgy->prop_which)];
}

void
sc_signal_optimize::add_aprocs_too_neg(sc_signal_edgy* p_edgy)
{
    sc_signal_base::UPDATE_FUNC* update_funcs = p_edgy->update_funcs_array();
    p_edgy->prop_which |= SC_SIGNAL_PROP_APROC_NEG;
    p_edgy->update_func = update_funcs[sanitize(p_edgy->prop_which)];
}

void
sc_signal_optimize::rem_aprocs_too_neg(sc_signal_edgy* p_edgy)
{
    sc_signal_base::UPDATE_FUNC* update_funcs = p_edgy->update_funcs_array();
    p_edgy->prop_which &= ~SC_SIGNAL_PROP_APROC_NEG;
    p_edgy->update_func = update_funcs[sanitize(p_edgy->prop_which)];
}


// +: no prop
// -: no prop
void
sc_signal_optimize::bool_update_prop_none(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    Tme->cur_value = nv;
    /* No need to propagate */
}


// +: asyncs
// -: no prop
void
sc_signal_optimize::bool_update_prop_async(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_only();
    } /* no need to prop on negedge */
}

// +: aprocs
// -: no prop
void
sc_signal_optimize::bool_update_prop_aproc(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_aprocs_only();
    } /* no need to prop on negedge */
}

// +: both
// -: no prop
void
sc_signal_optimize::bool_update_prop_both(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_aprocs();
    } /* no need to prop on negedge */
}

// +: no prop
// -: asyncs
void
sc_signal_optimize::bool_update_prop_async_neg(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (!nv) 
        Tme->prop_asyncs_only_neg();
    } /* no need to prop on posedge */
}

// +: asyncs
// -: asyncs
void
sc_signal_optimize::bool_update_prop_async_neg_async(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_only();
      else 
        Tme->prop_asyncs_only_neg();
    }
}

// +: aprocs
// -: asyncs
void
sc_signal_optimize::bool_update_prop_async_neg_aproc(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_aprocs_only();
      else
        Tme->prop_asyncs_only_neg();
    }
}

// +: both
// -: asyncs
void
sc_signal_optimize::bool_update_prop_async_neg_both(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_aprocs();
      else
        Tme->prop_asyncs_only_neg();
    }
}

// +: none
// -: aprocs
void
sc_signal_optimize::bool_update_prop_aproc_neg(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (!nv) 
        Tme->prop_aprocs_only_neg();
    } /* no need to prop on posedge */
}

// +: asyncs
// -: aprocs
void
sc_signal_optimize::bool_update_prop_aproc_neg_async(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_only();
      else 
        Tme->prop_aprocs_only_neg();
    }
}

// +: aprocs
// -: aprocs
void
sc_signal_optimize::bool_update_prop_aproc_neg_aproc(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_aprocs_only();
      else 
        Tme->prop_aprocs_only_neg();
    }
}

// +: both
// -: aprocs
void
sc_signal_optimize::bool_update_prop_aproc_neg_both(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_aprocs();
      else 
        Tme->prop_aprocs_only_neg();
    }
}

// +: none
// -: both
void
sc_signal_optimize::bool_update_prop_both_neg(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (!nv) 
        Tme->prop_asyncs_aprocs_neg();
    } /* no need to prop on posedge */
}

// +: asyncs
// -: both
void
sc_signal_optimize::bool_update_prop_both_neg_async(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_only();
      else 
        Tme->prop_asyncs_aprocs_neg();
    }
}

// +: aprocs
// -: both
void
sc_signal_optimize::bool_update_prop_both_neg_aproc(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_aprocs_only();
      else 
        Tme->prop_asyncs_aprocs_neg();
    }
}

// +: both
// -: both
void
sc_signal_optimize::bool_update_prop_both_neg_both(sc_signal_base* me)
{
    sc_signal<bool>* Tme = static_cast<sc_signal<bool>*>(me);
    bool nv = Tme->new_value;
    if(nv != Tme->cur_value)
    {
      Tme->cur_value = nv;
      if (nv) 
        Tme->prop_asyncs_aprocs();
      else 
        Tme->prop_asyncs_aprocs_neg();
    }
}


#define UPDATE_SC_SIGNAL_SC_BIT \
    bool nv;					\
    if (Tme->cur_value.is_bitref()) {		\
        nv = sc_bvrep_test_wb((const sc_bvrep*) Tme->new_value.rep, \
                              0, Tme->cur_value.val_or_index); \
        sc_bvrep_set_wb((sc_bvrep*) Tme->cur_value.rep, \
                        0, Tme->cur_value.val_or_index, nv); \
    } else {					\
        Tme->cur_value.val_or_index = nv = Tme->new_value.value; \
    }

// +: no prop
// -: no prop
void
sc_signal_optimize::sc_bit_update_prop_none(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    /* No need to propagate */
}


// +: asyncs
// -: no prop
void
sc_signal_optimize::sc_bit_update_prop_async(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_only();
    } /* no need to prop on negedge */
}

// +: aprocs
// -: no prop
void
sc_signal_optimize::sc_bit_update_prop_aproc(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_aprocs_only();
    } /* no need to prop on negedge */
}

// +: both
// -: no prop
void
sc_signal_optimize::sc_bit_update_prop_both(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_aprocs();
    } /* no need to prop on negedge */
}

// +: no prop
// -: asyncs
void
sc_signal_optimize::sc_bit_update_prop_async_neg(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (!nv) {
        Tme->prop_asyncs_only_neg();
    } /* no need to prop on posedge */
}

// +: asyncs
// -: asyncs
void
sc_signal_optimize::sc_bit_update_prop_async_neg_async(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_only();
    } else {
        Tme->prop_asyncs_only_neg();
    }
}

// +: aprocs
// -: asyncs
void
sc_signal_optimize::sc_bit_update_prop_async_neg_aproc(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_aprocs_only();
    } else {
        Tme->prop_asyncs_only_neg();
    }
}

// +: both
// -: asyncs
void
sc_signal_optimize::sc_bit_update_prop_async_neg_both(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_aprocs();
    } else {
        Tme->prop_asyncs_only_neg();
    }
}

// +: none
// -: aprocs
void
sc_signal_optimize::sc_bit_update_prop_aproc_neg(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (!nv) {
        Tme->prop_aprocs_only_neg();
    } /* no need to prop on posedge */
}

// +: asyncs
// -: aprocs
void
sc_signal_optimize::sc_bit_update_prop_aproc_neg_async(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_only();
    } else {
        Tme->prop_aprocs_only_neg();
    }
}

// +: aprocs
// -: aprocs
void
sc_signal_optimize::sc_bit_update_prop_aproc_neg_aproc(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_aprocs_only();
    } else {
        Tme->prop_aprocs_only_neg();
    }
}

// +: both
// -: aprocs
void
sc_signal_optimize::sc_bit_update_prop_aproc_neg_both(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_aprocs();
    } else {
        Tme->prop_aprocs_only_neg();
    }
}

// +: none
// -: both
void
sc_signal_optimize::sc_bit_update_prop_both_neg(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (!nv) {
        Tme->prop_asyncs_aprocs_neg();
    } /* no need to prop on posedge */
}

// +: asyncs
// -: both
void
sc_signal_optimize::sc_bit_update_prop_both_neg_async(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_only();
    } else {
        Tme->prop_asyncs_aprocs_neg();
    }
}

// +: aprocs
// -: both
void
sc_signal_optimize::sc_bit_update_prop_both_neg_aproc(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_aprocs_only();
    } else {
        Tme->prop_asyncs_aprocs_neg();
    }
}

// +: both
// -: both
void
sc_signal_optimize::sc_bit_update_prop_both_neg_both(sc_signal_base* me)
{
    sc_signal<sc_bit>* Tme = static_cast<sc_signal<sc_bit>*>(me);
    UPDATE_SC_SIGNAL_SC_BIT;
    if (nv) {
        Tme->prop_asyncs_aprocs();
    } else {
        Tme->prop_asyncs_aprocs_neg();
    }
}


void
sc_signal_optimize::unsigned_update_prop_none(sc_signal_base* me)
{
    sc_signal<unsigned>* Tme = static_cast<sc_signal<unsigned>*>(me);
    Tme->cur_value = Tme->new_value;
    /* no need to propagate */
}

void
sc_signal_optimize::unsigned_update_prop_async(sc_signal_base* me)
{
    sc_signal<unsigned>* Tme = static_cast<sc_signal<unsigned>*>(me);
    if(Tme->new_value != Tme->cur_value)
    {
      Tme->cur_value = Tme->new_value;
      Tme->prop_asyncs_only();
    }
}

void
sc_signal_optimize::unsigned_update_prop_aproc(sc_signal_base* me)
{
    sc_signal<unsigned>* Tme = static_cast<sc_signal<unsigned>*>(me);
    if(Tme->new_value != Tme->cur_value)
    {
      Tme->cur_value = Tme->new_value;
      Tme->prop_aprocs_only();
    }
}

void
sc_signal_optimize::unsigned_update_prop_both(sc_signal_base* me)
{
    sc_signal<unsigned>* Tme = static_cast<sc_signal<unsigned>*>(me);
    if(Tme->new_value != Tme->cur_value)
    {
      Tme->cur_value = Tme->new_value;
      Tme->prop_asyncs_aprocs();
    }
}


void
sc_signal_optimize::sc_logic_update_prop_none(sc_signal_base* me)
{
    sc_signal<sc_logic>* Tme = static_cast<sc_signal<sc_logic>*>(me);
    Tme->cur_value = Tme->new_value;
    /* no need to propagate */
}

void
sc_signal_optimize::sc_logic_update_prop_async(sc_signal_base* me)
{
    sc_signal<sc_logic>* Tme = static_cast<sc_signal<sc_logic>*>(me);
    if(Tme->new_value != Tme->cur_value)
    {
      Tme->cur_value = Tme->new_value;
      Tme->prop_asyncs_only();
    }
}

void
sc_signal_optimize::sc_logic_update_prop_aproc(sc_signal_base* me)
{
    sc_signal<sc_logic>* Tme = static_cast<sc_signal<sc_logic>*>(me);
    if(Tme->new_value != Tme->cur_value)
    {
      Tme->cur_value = Tme->new_value;
      Tme->prop_aprocs_only();
    }
}

void
sc_signal_optimize::sc_logic_update_prop_both(sc_signal_base* me)
{
    sc_signal<sc_logic>* Tme = static_cast<sc_signal<sc_logic>*>(me);
    if(Tme->new_value != Tme->cur_value)
    {
      Tme->cur_value = Tme->new_value;
      Tme->prop_asyncs_aprocs();
    }
}


void
sc_signal_optimize::sc_clock_update(sc_signal_base* me)
{
    sc_clock* Tme = static_cast<sc_clock*>(me);
    sc_signal<bool>* const l_sig = Tme;
    sc_simcontext* const l_simc = Tme->simcontext();

    l_sig->update_nonv(); /* call the nonvirtual version -- faster */
    Tme->submitted =false;
    if (l_sig->event()) {
        if (Tme->edge_seen_in_delta_cycle()) {
            cerr << "WARNING: Clock signal has multiple "
                 << "transitions within a delta cycle" << endl;
        }
        sc_clock_edge* const edge = Tme->edges[(int) l_sig->read()];
        l_simc->submit_clock_edge( edge );
        Tme->set_edge_seen_in_delta_cycle( true );
    }
}

void
sc_signal_optimize::optimize_clock(sc_clock* clk)
{
    if (clk->optimized)
        return;

    /* If the clock is not driving any SC_CTHREAD,
       then we don't need to go through the trouble
       of bookkeeping async_clock_edges, etc.  Thus,
       we can use sc_signal<bool>'s update mechanisms */
    sc_clock_edge* pos_edge = &(clk->pos());
    sc_clock_edge* neg_edge = &(clk->neg());

    /* Be careful -- after optimization, no processes
       may be added. */
    if ((0 == sc_clock_helpers::number_of_syncs(pos_edge)) &&
        (0 == sc_clock_helpers::number_of_syncs(neg_edge))) {
        sc_signal_base::UPDATE_FUNC* update_funcs;
        clk->optimized = true;
        /* With optimized set to true, we should get
           the update function array for sc_signal<bool>. */
        update_funcs = clk->update_funcs_array();
        clk->update_func = update_funcs[sanitize(clk->prop_which)];
    }
}
