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

    sc_sync_process.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <cstring>
#include <assert.h>
#include <stdlib.h>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_lambda.h"
#include "sc_process_int.h"
#include "sc_simcontext_int.h"
#include "sc_clock.h"
#include "sc_clock_int.h"
#include "sc_except.h"
#include "sc_module.h"

sc_sync_process::sc_sync_process(const char* nm,
                                 SC_ENTRY_FUNC fn,
                                 sc_module* mod,
                                 sc_clock_edge* edge)
    : sc_process_b(nm, fn, mod), dwatchlists(SC_MAX_WATCH_LEVEL)
{
    static int next_proc_id = 0;

    proc_id         = ++next_proc_id;

    /* Initialize member variables */
    clock_edge    = edge;

    /* The stack pointer/fiber pointer will be initialized
       before simulation, because the user may still wish
       to change the stack size. */
    stack_size      = SC_DEFAULT_STACK_SIZE;
#ifndef WIN32
    stack           = 0;
    sp              = 0;
#else
    pFiber          = (PVOID) 0;
#endif

    wait_state      = WAIT_UNKNOWN;
    wait_cycles     = 0;
    exception_level = -1;
    __reset_watching();
    for (int i = 0; i < SC_MAX_WATCH_LEVEL; ++i)
        dwatchlists[i] = new sc_plist<sc_lambda_ptr*>;

    prev_handle = next_handle = 0;
}


sc_sync_process::~sc_sync_process()
{
#ifndef WIN32
    delete[] (char*)stack;
#else
    if ((PVOID) 0 != pFiber) 
        DeleteFiber( pFiber );
#endif

    for (int i = 0; i < SC_MAX_WATCH_LEVEL; ++i) {
        sc_plist<sc_lambda_ptr*>::iterator it(dwatchlists[i]);
        while (! it.empty()) {
            delete *it;  /* this was created with `new sc_lambda_ptr' */
            it.remove(); /* and go on to next */
        }
        delete dwatchlists[i];
    }
}

void
sc_sync_process::set_stack_size(size_t size)
{
    stack_size = size; /* TBD: ROUND UP! */
}

void
sc_sync_process::prepare_for_simulation()
{
#ifndef WIN32
    /* Create a coroutine for the sc_sync_process */
    stack = new char[stack_size];
    assert( 0 != stack );
    void* sto = stack_align( stack, QT_STKALIGN, &stack_size );
    sp = QT_SP( sto, stack_size - QT_STKALIGN );
    sp = QT_ARGS( sp, this, this, 0, sc_sync_process_only );
#else
    /* Create a fiber for the sc_sync_process */
    void WINAPI sc_sync_process_fiber_func(LPVOID p);
    pFiber = CreateFiber(stack_size, sc_sync_process_fiber_func, this);
#endif
}


#ifndef WIN32

void
sc_sync_process_only( void*, void* p, qt_userf_t* )
{
    sc_sync_process_handle handle = (sc_sync_process_handle) p;
    bool restart;

    while (true) {
        restart = false;
        try {
            handle->execute();
        }
        catch (sc_user) {
            assert( 0 == handle->watch_level );
            restart = true;
        }
        catch (sc_halt) {
            cerr << "Terminating process " << handle->name() << endl;
            break;
        }
        if (!restart) break;
    }
    qt_t* nqt = handle->next_qt();
    sc_clock_helpers::remove_from_runnable(handle->clock_edge,
                                           handle);
    QT_ABORT( sc_sync_process_aborthelp, handle, 0, nqt );
}

void*
sc_sync_process_yieldhelp( qt_t* sp, void* old, void* )
{
    return ((sc_sync_process_handle) old)->sp = sp;
}

void*
sc_sync_process_aborthelp( qt_t*, void* old, void* )
{
    delete[] (char*) (((sc_sync_process_handle) old)->stack);
    ((sc_sync_process_handle) old)->stack = 0;
    return 0;
}

qt_t*
sc_sync_process::next_qt()
{
    sc_simcontext* simc_ = module->simcontext();
    sc_sync_process_handle next_h;
    for (next_h = next_handle; 0 != next_h; next_h = next_h->next_handle) {
        if (next_h->ready_to_wakeup()) {
            (void) simc_->set_curr_proc(next_h);
            return next_h->sp;
        }
    }
    (void) simc_->reset_curr_proc();
    return clock_edge->qt();
}

#else

void WINAPI
sc_sync_process_fiber_func(LPVOID p)
{
    sc_sync_process_handle handle = (sc_sync_process_handle) p;
    bool restart;

    while (true) {
        restart = false;
        try {
            handle->execute();
        }
        catch (sc_user) {
            assert( 0 == handle->watch_level );
            restart = true;
        }
        catch (sc_halt) {
            cerr << "Terminating process " << handle->name() << endl;
            break;
        }
        if (!restart) break;
    }

    PVOID next_fiber = handle->next_fiber();
    sc_clock_helpers::remove_from_runnable(handle->clock_edge,
                                           handle);
    SwitchToFiber(next_fiber);
}

PVOID
sc_sync_process::next_fiber()
{
    sc_simcontext* simc = module->simcontext();
    sc_sync_process_handle next_h;
    for (next_h = next_handle; 0 != next_h; next_h = next_h->next_handle) {
        if (next_h->ready_to_wakeup()) {
            (void) simc->set_curr_proc(next_h);
            return next_h->pFiber;
        }
    }
    (void) simc->reset_curr_proc();
    return clock_edge->fiber();
}

#endif


bool
sc_sync_process::eval_watchlist_curr_level()
{
    int wlevel = watch_level;
    assert(wlevel > 0);
    
    sc_plist<sc_lambda_ptr*>& li = *dwatchlists[wlevel];
    sc_plist<sc_lambda_ptr*>::iterator wit(li);
    while (! wit.empty()) {
        if ((**wit)->eval()) {
            exception_level = wlevel;
            wit.reset(li);
            while (! wit.empty()) {
                delete *wit;
                wit.remove();
            }
            return true;
        }
        wit++;
    }
    exception_level = -1;
    return false;
}

bool
sc_sync_process::ready_to_wakeup()
{
    bool exception = eval_watchlist();
    if (exception) return true;

    bool ready;
    switch (wait_state) {
    case WAIT_CLOCK:
        ready = (--wait_cycles == 0);
        break;
    case WAIT_LAMBDA:
        ready = wait_lambda->eval();
        break;
    default:
        ready = true;
        break;
    }
    return ready;
}

bool
sc_sync_process::eval_watchlist()
{
    int wlevel = __watch_level();
    for (int i = 0; i <= wlevel; ++i) {
        sc_plist<sc_lambda_ptr*>& li = *(dwatchlists[i]);
        sc_plist<sc_lambda_ptr*>::iterator wit(li);
        while (! wit.empty()) {
            if ((**wit)->eval()) {
                exception_level = i;
                if (i == 0) /* never remove global watching */
                    i = 1;
                for (; i <= wlevel; ++i) {
                    sc_plist<sc_lambda_ptr*>& l = *(dwatchlists[i]);
                    sc_plist<sc_lambda_ptr*>::iterator it(l);
                    while (! it.empty()) {
                        delete *it;
                        it.remove();
                    }
                }
                return true;
            }
            wit++;
        }
    }
    exception_level = -1;
    return false;
}

void
sc_set_stack_size(sc_sync_process_handle sync_h,
                  size_t size)
{
    sync_h->set_stack_size(size);
}
