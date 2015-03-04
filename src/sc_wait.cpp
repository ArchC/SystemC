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

    sc_wait.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: Added error messages to clarify the
    following: wait() has no effect for SC_METHODs. wait_until(),
    wait(n), and watching work only for SC_CTHREADS.

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_lambda.h"
#include "sc_wait.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_process_int.h"
#include "sc_context_switch.h"
#include "sc_except.h"
#include "sc_port.h"
#include "sc_port_manager.h"
#include "sc_process_int.h"
#include "sc_simcontext_int.h"

void
halt( sc_simcontext* simc )
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    assert(SC_CURR_PROC_SYNC == cpi->kind);
    sc_sync_process_handle handle = (sc_sync_process_handle) cpi->process_handle;
    handle->wait_cycles = 1;
    handle->wait_state = sc_sync_process::WAIT_CLOCK;
#ifndef WIN32
    context_switch( sc_sync_process_yieldhelp, handle, 0, handle->next_qt() );
#else
    context_switch( handle->next_fiber() );
#endif
    throw sc_halt();
}

void
wait( sc_simcontext* simc )
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    switch (cpi->kind) {
    case SC_CURR_PROC_SYNC: {
        sc_sync_process_handle handle = (sc_sync_process_handle) cpi->process_handle;
        handle->wait_cycles = 1;
        handle->wait_state = sc_sync_process::WAIT_CLOCK;
#ifndef WIN32
        context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
        context_switch(handle->next_fiber());
#endif
        handle->wait_state = sc_sync_process::WAIT_UNKNOWN;
        int exception_level = handle->exception_level;
        if (0 == exception_level) {
            throw sc_user();
        } else if (exception_level > 0) {
            throw exception_level;
        }
        break;
    }

    case SC_CURR_PROC_APROC: {
        sc_aproc_process_handle handle = (sc_aproc_process_handle) cpi->process_handle;
#ifndef WIN32
        qt_t* nqt = simc->next_aproc_qt();
        handle->set_in_updateq( false );
        /* No need to do context_switch because this is a tail call;
           registers won't be needed even if they're messed up. */
        QT_BLOCK( sc_aproc_process_yieldhelp, handle, 0, nqt );
#else
        PVOID next_fiber = simc->next_aproc_fiber();
        handle->set_in_updateq( false );
        SwitchToFiber( next_fiber );
#endif
        break;
    }

    default:
        REPORT_WARNING(3003,"ignored");
        break;
    }
}

void
wait(int n, sc_simcontext* simc)
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    switch (cpi->kind) {
    case SC_CURR_PROC_SYNC: {
        sc_sync_process_handle handle = (sc_sync_process_handle) cpi->process_handle;
        handle->wait_cycles = n;
        handle->wait_state = sc_sync_process::WAIT_CLOCK;
#ifndef WIN32
        context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
        context_switch(handle->next_fiber());
#endif
        handle->wait_state = sc_sync_process::WAIT_UNKNOWN;
        int exception_level = handle->exception_level;
        if (0 == exception_level) {
            throw sc_user();
        } else if (exception_level > 0) {
            throw exception_level;
        }
        break;
    }

    default:
        cerr << "SystemC warning: wait(n) works only for SC_CTHREADs. For other process types, it has no effect." << endl;
        break;
    }
}

void
wait_until(const sc_lambda_ptr& lambda, sc_simcontext* simc)
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    switch (cpi->kind) {
    case SC_CURR_PROC_SYNC: {
        sc_sync_process_handle handle = (sc_sync_process_handle) cpi->process_handle;
        handle->wait_lambda = lambda;
        handle->wait_state = sc_sync_process::WAIT_LAMBDA;

#ifndef WIN32
        context_switch(sc_sync_process_yieldhelp, handle, 0, handle->next_qt());
#else
        context_switch(handle->next_fiber());
#endif

        handle->wait_state = sc_sync_process::WAIT_UNKNOWN;
        int exception_level = handle->exception_level;
        if (0 == exception_level) {
            throw sc_user();
        } else if (exception_level > 0) {
            throw exception_level;
        }
        break;
    }

    default:
        cerr << "SystemC warning: wait_until() works only for SC_CTHREADs. For other process types, it has no effect; For them, you can use a do-while loop with wait() inside to get a similar effect." << endl;
        break;
    }
}

void
watching_before_simulation(const sc_lambda_ptr& lambda, sc_simcontext* simc)
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    simc->get_port_manager()->add_lambda_for_resolution(lambda);
    switch (cpi->kind) {
    case SC_CURR_PROC_SYNC: {
        sc_sync_process_handle handle = (sc_sync_process_handle) cpi->process_handle;
        int wlevel = handle->watch_level;
        assert(wlevel < SC_MAX_WATCH_LEVEL);
        handle->dwatchlists[wlevel]->push_back(new sc_lambda_ptr(lambda));
        break;
    }
    default:
        cerr << "SystemC warning: watching works only for SC_CTHREADs. For other process types, watching has no effect." << endl;
        break;
    }
}

void
watching_during_simulation(const sc_lambda_ptr& lambda, sc_simcontext* simc)
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    switch (cpi->kind) {
    case SC_CURR_PROC_SYNC: {
        sc_sync_process_handle handle = (sc_sync_process_handle) cpi->process_handle;
        int wlevel = handle->watch_level;
        assert(wlevel < SC_MAX_WATCH_LEVEL);
        handle->dwatchlists[wlevel]->push_back(new sc_lambda_ptr(lambda));
        break;
    }
    default:
        cerr << "SystemC warning: watching works only for SC_CTHREADs. For other process types, watching has no effect." << endl;
        break;
    }
}
 
void
__reset_watching( sc_sync_process_handle handle )
{
    handle->__reset_watching();
}

void
__open_watching( sc_sync_process_handle handle )
{
    handle->__open_watching();
}

void
__close_watching( sc_sync_process_handle handle )
{
    handle->__close_watching();
    assert(handle->__watch_level() >= 0);
}

int
__watch_level( sc_sync_process_handle handle )
{
    return handle->__watch_level();
}

void
__watching_first( sc_sync_process_handle handle )
{
    if (handle->eval_watchlist_curr_level()) {
        throw handle->exception_level;
    }
}

void
__sanitycheck_watchlists( sc_sync_process_handle handle )
{
    assert(handle->dwatchlists[handle->__watch_level()]->empty());
}

void
sc_set_location( const char* file, int lineno,
                 sc_simcontext* simc )
{
    const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
    sc_process_b* handle = cpi->process_handle;
    handle->file = file;
    handle->lineno = lineno;
}


void
at_posedge( const sc_signal<sc_logic>& s, sc_simcontext* simc )
{
    if (s.read() == '1') {
        wait_until( s.delayed() == '0', simc );
        wait_until( s.delayed() == '1', simc );
    } else {
        wait_until( s.delayed() == '1', simc );
    }
}

void at_posedge( const sc_signal_edgy& s, sc_simcontext* simc )
{
    if (s.edgy_read()) {
        wait_until(s.delayed() == 0);
        wait_until(s.delayed());
    } else {
        wait_until(s.delayed());
    }
}

void at_negedge( const sc_signal<sc_logic>& s, sc_simcontext* simc )
{
    if (s.read() == '0') {
        wait_until( s.delayed() == '1', simc );
        wait_until( s.delayed() == '0', simc );
    } else {
        wait_until( s.delayed() == '0', simc );
    }
}

void at_negedge( const sc_signal_edgy& s, sc_simcontext* simc )
{
    if (! s.edgy_read()) {
        wait_until(s.delayed());
        wait_until(s.delayed() == 0);
    } else {
        wait_until(s.delayed() == 0);
    }
}
