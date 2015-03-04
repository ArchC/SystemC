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

    sc_clock.cpp -- Implementation of the clock process

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <stdio.h>
#include <assert.h>

#ifndef WIN32
#include "qt/qt.h"
#else
#include <Windows.h>
#include <winnt.h>
#endif

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

#include "sc_iostream.h"
#include "sc_vector.h"
#include "sc_clock.h"
#include "sc_signal.h"
#include "sc_sync.h"
#include "sc_signal_array.h"
#include "sc_signal_delay.h"
#include "sc_simcontext.h"
#include "sc_dump.h"

#include "sc_context_switch.h"
#include "sc_measure.h"

#include "sc_process_int.h"
#include "sc_simcontext_int.h"
#include "sc_macros_int.h"

#include "sc_polarity.h"
#include "sc_clock_int.h"

#include "sc_signal_optimize.h"

int sc_clock_edge::num_clock_edges = 0;

const char* sc_clock::kind_string = "sc_clock";

/******************************************************************************
    sc_clock_edge( sc_clock& ) is used ONLY as an implcit type converter.  It
    is provided only for compatibility, so that when the user supplies an
    argument of type sc_clock to the constructor of sc_sync, the positive
    edge will be used.  Note that the constructor of sc_sync will check if
    the implicit type conversion took place, and use the positive edge that was
    created with the sc_clock accordingly.
******************************************************************************/
sc_clock_edge::sc_clock_edge( sc_clock& ck )
    : clk(ck), faze(PHASE_POS), init_time(-1.0)
{
    eid = -1;                   // implicitly converted, no real eid

#ifndef WIN32
    sp = 0;                     // Necessary only for QT
#endif

    per = clk.period();
    next_time = -1.0;
    signals_to_update = 0;
    lastof_signals_to_update = -1;
    sync_runnable = 0;
    sync_defunct  = 0;
    total_number_of_syncs = 0;
}

sc_clock_edge::~sc_clock_edge()
{
    delete[] signals_to_update;

    sc_sync_process_handle curr_handle, next_handle;
    for (curr_handle = sync_runnable;
         NIL(sc_sync_process_handle) != curr_handle;
         curr_handle = next_handle) {
        next_handle = curr_handle->next_handle;
        delete curr_handle;
    }
    for (curr_handle = sync_defunct;
         NIL(sc_sync_process_handle) != curr_handle;
         curr_handle = next_handle) {
        next_handle = curr_handle->next_handle;
        delete curr_handle;
    }
}

#ifndef WIN32
/******************************************************************************
    yieldhelp() is the helper function for switching out of a
    coroutine.  The first argument `sp' is the stack pointer to be saved
    for the old coroutine, and the second argument `clk' is the
    pointer to the clock process which just switched out.
******************************************************************************/
void*
sc_clock_edge_yieldhelp( qt_t* sp, void* clk_edge, void* )
{
    return ((sc_clock_edge*)clk_edge)->sp = sp;
}
#endif

#if 0
void
sc_clock_edge::submit_update_d( sc_signal_delay_base* sd, int delay )
{
    sc_plist<da_elem*>::iterator dit( delayed_assignments );
    while (!dit.empty()) {
        da_elem* de = *dit;
        if (de->delay > delay)
            break;
        else if (de->delay == delay) {
            sd->set_next(de->first);
            de->first = sd;
            return;
        }
        dit++;
    }
    da_elem* new_da = new da_elem;
    new_da->delay = delay;
    new_da->first = sd;
    delayed_assignments.insert_before( dit.get_handle(), new_da );
    return;
}
#endif

/******************************************************************************
    advance() finds the first process in the list that's ready to wake up, i.e.
    when its waiting time is up, its lambda evals to true, or some lambda on
    its watchlist evals to true.Since we trace both positive and negative clock
    edges, we have to keep the edge alive even if no proc is depending on it
    (i.e. when the runnable queue is empty ).
******************************************************************************/
void
sc_clock_edge::advance()
{
    sc_simcontext* simc = clk.simcontext();
    sc_sync_process_handle handle_ = sync_runnable;

    while (true) {
        if (NIL(sc_sync_process_handle) == handle_) return;
        if (handle_->ready_to_wakeup()) {
            (void) simc->set_curr_proc( handle_ );

#ifndef WIN32
            context_switch( sc_clock_edge_yieldhelp, this, 0, handle_->sp );
#else
            /* Save the current fiber and then context switch */
	    pFiber = GetCurrentFiber();
            if(handle_->pFiber)
              context_switch( handle_->pFiber );
            else
	      REPORT_ERROR(1004,"");
#endif
            break;
        }
        else {
            handle_ = handle_->next_handle;
        }
    }
    return;
}

void
sc_clock_helpers::remove_from_runnable( sc_clock_edge* p_edge,
                                        sc_sync_process_handle sync_handle )
{
    if (NIL(sc_sync_process_handle) == sync_handle->prev_handle) {
        /* ASSERT that it is the first one. */
        assert( sync_handle == p_edge->sync_runnable );
        p_edge->sync_runnable = sync_handle->next_handle;
        if (NIL(sc_sync_process_handle) != p_edge->sync_runnable) {
            p_edge->sync_runnable->prev_handle = 0;
        }
    } else {
        sync_handle->prev_handle->next_handle = sync_handle->next_handle;
        if (NIL(sc_sync_process_handle) != sync_handle->next_handle) {
            sync_handle->next_handle->prev_handle = sync_handle->prev_handle;
        }
    }

    /* mark this as defunct */
    sync_handle->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;
    if (NIL(sc_sync_process_handle) == p_edge->sync_defunct) {
        p_edge->sync_defunct = sync_handle;
        /* Use the head's `prev_handle' to point to last one */
        p_edge->sync_defunct->prev_handle = sync_handle;
        p_edge->sync_defunct->next_handle = 0;
    } else {
        sc_sync_process_handle& last_defunct = p_edge->sync_defunct->prev_handle;
        last_defunct->next_handle = sync_handle;
        sync_handle->prev_handle = last_defunct;
        sync_handle->next_handle = 0;
        last_defunct = sync_handle;
    }
}

sc_clock_edge::sc_clock_edge( sc_clock& ck, phase_t ph, double p, double s )
    : clk(ck), faze(ph), init_time(s)
{
    eid = num_clock_edges++;

#ifndef WIN32
    sp = 0;
#else
    pFiber = NULL;
#endif

    per = p;
    next_time = s;

    handle = clk.simcontext()->add_clock_edge(this);
    sync_runnable = 0;
    sync_defunct  = 0;
    total_number_of_syncs = 0;

    signals_to_update = 0;
    lastof_signals_to_update = -1;
}

void
sc_clock_edge::reset()
{
    next_time = init_time;
}

/*****************************************************************************/


void
sc_clock::stop()
{
    sc_stop();
}

typedef sc_clock::callback_fn callback_fn;

/******************************************************************************
    sc_clock::start( double duration, callback_fn
    callback, void *arg ) is a class-static function invoked by the
    user to start the simulation.  The function `callback' is supplied
    by the user to monitor events (i.e. clock edges) during the
    simulation.  At any time a clock goes high, after the signals are
    updated, the function `callback' is called with a reference to
    that clock and the user-supplied argument `arg'.  If `callback' is
    zero, then no function callback will occur.
******************************************************************************/
void
sc_clock::start( double duration, callback_fn callback, void* arg )
{
    sc_start(duration, callback, arg);
}

double
sc_clock::time_stamp()
{
    return sc_get_curr_simcontext()->time_stamp();
}

/******************************************************************************
    Class-specific static variables.
******************************************************************************/

int sc_clock::num_clocks = 0;

static const char*
new_clock_name()
{
    static int counter = 0;
    static char namebuf[16];
    sprintf(namebuf, "C%d", counter++);
    return namebuf;
}


/******************************************************************************
    sc_clock::sc_clock() - constructor.  The user may give the clock a
    name (nm), specify its period (p), and specify the time of its first
    edge (s).  Default values are specified in "sc_clock.h"
******************************************************************************/
sc_clock::sc_clock( const char* nm, double p, double d, double s, bool posedge_first )
    : sc_signal<bool>( nm ? nm : new_clock_name() )
{
    init( p, d, s, posedge_first );
}

void
sc_clock::init( double p, double d, double s, bool posedge_first )
{
    cid = num_clocks++;

    /* Nonpositive period means nonautomatic clock */
    per = p;

    if (d <= 0 || d >= 1)
        dut = 0.5;
    else
        dut = d;

    // sig = new sc_signal<bool>( (sc_object*) this );   /* make signal anonymous */

    if (posedge_first) {
        edges[1] = new sc_clock_edge( *this, PHASE_POS, p, s );
        edges[0] = new sc_clock_edge( *this, PHASE_NEG, p, s + d * p );
        set_old_value(false);
        set_cur_value(false);
        set_new_value(false);
    } else {
        edges[0] = new sc_clock_edge( *this, PHASE_NEG, p, s );
        edges[1] = new sc_clock_edge( *this, PHASE_POS, p, s + d * p );
        set_old_value(true);
        set_cur_value(true);
        set_new_value(true);
    }

    set_edge_seen_in_delta_cycle( false );

    update_func = &sc_signal_optimize::sc_clock_update;
    prop_which = SC_SIGNAL_PROP_NONE;
    optimized = false;
}

sc_clock::~sc_clock()
{
    delete edges[0];
    delete edges[1];
    // delete sig;
}


void
sc_clock::print(ostream& os) const
{
    sc_object::print(os);
}

void
sc_clock::dump(ostream& os) const
{
    sc_object::dump(os);
    os << "period = " << period() << endl;
    os << "duty cycle = " << duty_cycle() << endl;
}

void
sc_clock::trace( sc_trace_file* tf ) const
{
    ::sc_trace( tf, *(this->get_data_ptr()), name() );
}

void
sc_clock::update()
{
    sc_signal<bool>* const l_sig = this;
    sc_simcontext* const l_simc = simcontext();

    l_sig->update_nonv(); /* call the nonvirtual version -- faster */
    submitted = false;
    if (l_sig->event()) {
        if (edge_seen_in_delta_cycle())
            REPORT_WARNING(1010,"");
        sc_clock_edge* const edge = edges[(int) l_sig->read()];
        l_simc->submit_clock_edge( edge );
        set_edge_seen_in_delta_cycle( true );
    }
}

sc_signal_base::UPDATE_FUNC*
sc_clock::update_funcs_array() const
{
    static sc_signal_base::UPDATE_FUNC funcs[] = {
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update,
        &sc_signal_optimize::sc_clock_update
    };
    if (optimized) {
        /* If there are no syncs, the clock has been
           optimized, then we return the update
           functions of sc_signal<bool>.  Note that
           this also works when aprocs are removed
           from the sensitive list of this clock,
           because sc_signal_optimize::rem_aproc_too*()
           calls this function to determine how to
           change the update_func pointer */
        return sc_signal<bool>::update_funcs_array();
    } else {
        return funcs;
    }
}

sc_sync_process_handle
sc_clock_edge::register_sc_sync_process(const char* name,
                                        SC_ENTRY_FUNC entry_fn,
                                        sc_module* module)
{
    sc_sync_process_handle handle_;
    handle_ = new sc_sync_process(name, entry_fn, module, this);
    sc_clock_helpers::bind_sync_to_clock_edge_back(this, handle_);
    clock().simcontext()->set_curr_proc( handle_ );
    return handle_;
}

/* Put the synchronous process at the back of the list */
void
sc_clock_helpers::bind_sync_to_clock_edge_back(sc_clock_edge* clk_edge,
                                               sc_sync_process_handle handle)
{
    ++(clk_edge->total_number_of_syncs);
    if (NIL(sc_sync_process_handle) == clk_edge->sync_runnable) {
        clk_edge->sync_runnable = handle;
        /* Use the head's `prev_handle' to point to last */
        clk_edge->sync_runnable->prev_handle = handle;
    } else {
        sc_sync_process_handle& last_handle = clk_edge->sync_runnable->prev_handle;
        last_handle->next_handle = handle;
        handle->prev_handle = last_handle;
        last_handle = handle;
    }
    handle->next_handle = 0;
    handle->polarity.clk_port = 0;
}

/* Put the synchronous process at the front of the list */
void
sc_clock_helpers::bind_sync_to_clock_edge_front(sc_clock_edge* clk_edge,
                                                sc_sync_process_handle handle)
{
    ++(clk_edge->total_number_of_syncs);
    if (NIL(sc_sync_process_handle) == clk_edge->sync_runnable) {
        clk_edge->sync_runnable = handle;
        clk_edge->sync_runnable->prev_handle = handle;
        handle->next_handle = 0;
    } else {
        handle->next_handle = clk_edge->sync_runnable;
        handle->prev_handle = clk_edge->sync_runnable->prev_handle;
        clk_edge->sync_runnable->prev_handle = handle;
        clk_edge->sync_runnable = handle;
    }
    handle->polarity.clk_port = 0;
}

/* Returns the number of syncs driven by the given clock edge */
int
sc_clock_helpers::number_of_syncs(const sc_clock_edge* p_edge)
{
    return p_edge->total_number_of_syncs;
}
 
void
sc_clock_helpers::prepare_for_simulation(sc_clock_edge* p_edge,
                                         int num_signals)
{
    p_edge->signals_to_update = new sc_signal_base*[num_signals];
    p_edge->lastof_signals_to_update = -1;

    /* prev_handle of the first handle was used to point to
       the last handle.  We set it to zero to remove the
       confusion. */
    if (NIL(sc_sync_process_handle) != p_edge->sync_runnable) {
        p_edge->sync_runnable->prev_handle = NIL(sc_sync_process_handle);
    }

    /* Go through each sc_sync_process_handle, and call
       prepare_for_simulation on each. */

    sc_sync_process_handle handle;
    for (handle = p_edge->sync_runnable;
         NIL(sc_sync_process_handle) != handle;
         handle = handle->next_handle) {
        handle->prepare_for_simulation();
    }
    sc_signal_optimize::optimize_clock(&(p_edge->clock()));
}
