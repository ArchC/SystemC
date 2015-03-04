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

    sc_aproc_process.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <assert.h>
#include <cstring>
#include <stdlib.h>
#include <cstddef>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0400
#include <Windows.h>
#endif

#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_lambda.h"
#include "sc_process_int.h"
#include "sc_simcontext_int.h"
#include "sc_signal.h"
#include "sc_signal_array.h"
#include "sc_module.h"

#include "sc_bool_vector.h"

sc_aproc_process::sc_aproc_process(const char* nm,
                                   SC_ENTRY_FUNC fn,
                                   sc_module* mod)
    : sc_process_b(nm, fn, mod),
      trigger_signals(16),
      trigger_signals_edgy_neg(16)
{
    static int next_proc_id = 0;
    /* Initialize data members */
    proc_id         = ++next_proc_id;
    m_is_in_updateq = false;

    stack_size      = SC_DEFAULT_STACK_SIZE;
#ifndef WIN32
    stack           = 0;
    sp              = 0;
#else
    pFiber          = (PVOID)0;
#endif

    next_handle = 0;
}

sc_aproc_process::~sc_aproc_process()
{
#ifndef WIN32
    delete[] (char*)stack;
#else
    if ((PVOID) 0 != pFiber)
        DeleteFiber( pFiber );
#endif
}

void
sc_aproc_process::set_stack_size(size_t size)
{
    stack_size = size;
}

void
sc_aproc_process::prepare_for_simulation()
{
#ifndef WIN32
    stack = new char[stack_size];
    assert( stack != 0 );
    void* sto = stack_align( stack, QT_STKALIGN, &stack_size );
    sp = QT_SP( sto, stack_size - QT_STKALIGN );
    sp = QT_ARGS( sp, this, this, 0, sc_aproc_process_only );
#else
    // forward declare fiber function
    void WINAPI sc_aproc_process_fiber_func(PVOID p);
    pFiber = CreateFiber(stack_size, sc_aproc_process_fiber_func, this);
#endif
}

void
sc_aproc_process::add_trigger_signal(const sc_signal_base& s)
{
    for (int i = 0; i < trigger_signals.size(); ++i) {
        const sc_signal_base* p = trigger_signals.fetch(i);
        if (p == &s)
            return;
    }
    trigger_signals.push_back( &s );
}

void
sc_aproc_process::add_trigger_signal(const sc_signal_array_base& s)
{
    for (int i = 0; i < s.length(); ++i) {
        const sc_signal_base* sig = s.indiv_signals[i];
        add_trigger_signal( *sig );
    }
}

void
sc_aproc_process::add_trigger_signal(const sc_signal_bool_vector& sbv)
{
    for (int i = 0; i < sbv.length(); ++i) {
        const sc_signal_base* sb = sbv.indiv_signals[i];
        const sc_signal_edgy* sig = static_cast<const sc_signal_edgy*>(sb);
        add_trigger_signal_pos( *sig );
        add_trigger_signal_neg( *sig );
    }
}

void
sc_aproc_process::add_trigger_signal_pos(const sc_signal_edgy& s)
{
    add_trigger_signal( s );
}

void
sc_aproc_process::add_trigger_signal_neg(const sc_signal_edgy& s)
{
    for (int i = 0; i < trigger_signals_edgy_neg.size(); ++i) {
        const sc_signal_edgy* p = trigger_signals_edgy_neg.fetch(i);
        if (p == &s)
            return;
    }
    trigger_signals_edgy_neg.push_back( &s );
}

void
sc_aproc_process::remove_from_trigger_signals()
{
    /* Remove this aproc_handle from the signals to
       which it is sensitive. */
    for (int i = trigger_signals.size() - 1; i >= 0; --i) {
        const sc_signal_base* sig = trigger_signals.fetch(i);
 
        /* Note that this function call is virtual */
        sig->remove_sensitive_aproc(this);
    }
}

void
sc_aproc_process::remove_from_trigger_signals_neg()
{
    /* Remove this aproc_handle from the signals to
       which it is sensitive. */
    for (int i = trigger_signals_edgy_neg.size() - 1; i >= 0; --i) {
        const sc_signal_edgy* sig = trigger_signals_edgy_neg.fetch(i);

        /* Note that this function call is virtual */
        sig->remove_sensitive_aproc_neg(this);
    }
}

#ifndef WIN32
void
sc_aproc_process_only( void*, void* p, qt_userf_t* )
{
    sc_aproc_process_handle aproc_handle;
    aproc_handle = (sc_aproc_process_handle) p;

    aproc_handle->execute();

    /* If control reaches this point, then the process has
       gone to heaven. */

    qt_t* next_qt = aproc_handle->module->simcontext()->next_aproc_qt();

    aproc_handle->remove_from_trigger_signals();
    aproc_handle->remove_from_trigger_signals_neg();

    QT_ABORT( sc_aproc_process_aborthelp, aproc_handle, 0, next_qt );
}

void*
sc_aproc_process_aborthelp( qt_t*, void* old, void* )
{
    /* Simply mark this aproc as defunct */
    ((sc_aproc_process_handle) old)->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;
    return 0;
}

void*
sc_aproc_process_yieldhelp( qt_t* sp, void* old, void* )
{
    return ((sc_aproc_process_handle) old)->sp = sp;
}

#else

void WINAPI
sc_aproc_process_fiber_func(PVOID p)
{
    sc_aproc_process_handle aproc_handle;
    aproc_handle = (sc_aproc_process_handle) p;
    
    aproc_handle->execute();

    /* If control reaches this point, then the process has
       gone to heaven. */

    PVOID next_fiber = aproc_handle->module->simcontext()->next_aproc_fiber();

    aproc_handle->remove_from_trigger_signals();
    aproc_handle->remove_from_trigger_signals_neg();
    aproc_handle->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;

    SwitchToFiber( next_fiber );
}

#endif

void 
sc_aproc_process::sensitive( const sc_signal_base& s )
{
    s.decl_sensitive_aproc(this);
    add_trigger_signal(s);
}

void
sc_aproc_process::sensitive( const sc_signal_array_base& sa )
{
    sa.decl_sensitive_aproc(this);
    add_trigger_signal(sa);
}

void
sc_aproc_process::sensitive( const sc_signal_bool_vector& sbv )
{
    sbv.decl_sensitive_aproc(this);
    add_trigger_signal(sbv);
}

void
sc_aproc_process::sensitive_pos( const sc_signal_edgy& s )
{
    s.decl_sensitive_aproc_pos(this);
    add_trigger_signal_pos(s);
}

void
sc_aproc_process::sensitive_neg( const sc_signal_edgy& s )
{
    s.decl_sensitive_aproc_neg(this);
    add_trigger_signal_neg(s);
}

void
sc_aproc_process::sensitive( const sc_signal_edgy& s )
{
    sensitive_neg(s);
    sensitive_pos(s);
}


void
sc_set_stack_size( sc_aproc_process_handle aproc_h,
                   size_t size )
{
    aproc_h->set_stack_size(size);
}
