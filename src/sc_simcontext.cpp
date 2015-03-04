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

    sc_simcontext.cpp -- provides a simulation context for use with multiple
                         simulations.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: Added sc_stop() detection into initial_crunch 
                                 and crunch. This makes it possible to exit out of 
                                 a combinational loop using sc_stop().

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#ifdef WIN32
#include "sc_cmnhdr.h"
#include <Windows.h>
//#include <winnt.h>
//PVOID __stdcall GetCurrentFiber( void );
#else
#include "qt/qt.h"
#endif

#include "sc_iostream.h"
#include "sc_pq.h"
#include "sc_hash.h"
#include "sc_simcontext.h"

#include "sc_clock.h"
#include "sc_dump.h"
#include "sc_signal.h"

#include "sc_ver.h"
#include "sc_trace.h"

#include "sc_context_switch.h"

#include "sc_array_concat.h"
#include "sc_object_manager.h"

#include "sc_lambda.h"
#include "sc_module.h"
#include "sc_port.h"
#include "sc_port_manager.h"

#include "sc_polarity.h"
#include "sc_port_clk.h"

#include "sc_process_int.h"
#include "sc_simcontext_int.h"
#include "sc_macros_int.h"

#include "sc_mempool.h"
#include "sc_clock_int.h"
#include "sc_signal_optimize.h"

struct sc_async_aproc_process_table {
    sc_async_process_handle async_head;
    sc_async_process_handle async_tail;
    sc_aproc_process_handle aproc_head;
    sc_aproc_process_handle aproc_tail;

    sc_async_aproc_process_table();
    ~sc_async_aproc_process_table();
};

sc_async_aproc_process_table::sc_async_aproc_process_table()
{
    async_head = async_tail = 0;
    aproc_head = aproc_tail = 0;
}

sc_async_aproc_process_table::~sc_async_aproc_process_table()
{
    sc_async_process_handle curr_async_handle, next_async_handle;
    for (curr_async_handle = async_head;
         NIL(sc_async_process_handle) != curr_async_handle;
         curr_async_handle = next_async_handle) {
        next_async_handle = curr_async_handle->next_handle;
        delete curr_async_handle;
    }

    sc_aproc_process_handle curr_aproc_handle, next_aproc_handle;
    for (curr_aproc_handle = aproc_head;
         NIL(sc_aproc_process_handle) != curr_aproc_handle;
         curr_aproc_handle = next_aproc_handle) {
        next_aproc_handle = curr_aproc_handle->next_handle;
        delete curr_aproc_handle;
    }
}

/* Not MT-safe! -- MT implementation not yet finished. */
static sc_simcontext* sc_curr_simcontext = 0;


void
pln()
{
    static bool lnp = false;
    if (! lnp) {
        cerr << endl;
	cerr << sc_version() << endl;
	cerr << sc_copyright() << endl;

	/**************************************************/
	/* regressions check point                        */
        if (getenv("SCENIC_REGRESSION") != NULL) {
            cerr << "Scenic Simulation" << endl;
        }
	/**************************************************/
        lnp = true;
    }
}
#if 0
static struct print_pln {
    print_pln()
    {
        pln();
    }
} print_pln_object;
#endif

static int
next_edge_compare( const void* ve1, const void* ve2 )
{
    const sc_clock_edge* e1 = (const sc_clock_edge*) ve1;
    const sc_clock_edge* e2 = (const sc_clock_edge*) ve2;

    double diff = e2->next_edge() - e1->next_edge();
    if (diff < 0) {
        return -1;
    } else if (diff > 0) {
        return 1;
    } else {
        return (e2->id() - e1->id());
    }
}

#ifndef WIN32
void*
sc_simcontext_yieldhelp( qt_t* sp, void* simc, void* )
{
    sc_simcontext* simcontext = (sc_simcontext*) simc;

    /* make sure we know how to get back */
    /* next_aproc_index was set to one beyond the last qt,
       i.e. lastof_aprocs_to_execute */

    simcontext->sp = sp;

    /* This is important because each aproc uses this value to
       figure out the next aproc to switch into, by calling
       next_aproc_qt() */

    return sp;
}
#endif 

sc_simcontext::sc_simcontext()
{
/*
#ifdef __GNUC__
  // gcc 2.92.2 bug in initialization of global variables
  pln(); // print legal notice
#else
#endif
*/
    watching_fn = watching_before_simulation;

    concat_manager = new sc_array_concat_manager;
    object_manager = new sc_object_manager;
    port_manager = new sc_port_manager( this );

    prioq = 0;
    curr_time = 0.0;
    curr_edge = 0;
    forced_stop = false;

    update_array0_alloc = 64;
    signals_to_update_arrays[0] = new sc_signal_base*[update_array0_alloc];
    signals_to_update_arrays[1] = 0;
    curr_array = 0;
    signals_to_update = signals_to_update_arrays[0];
    lastof_signals_to_update = -1;
    asyncs_to_execute = 0;
    ready_to_simulate = false;

    clock_edges_async_arrays[0] = 0;
    clock_edges_async_arrays[1] = 0;
    curr_clock_edges_array = 0;
    lastof_clock_edges_async = -1;

    clock_edges_to_advance = 0;
    lastof_clock_edges_to_advance = -1;

    total_number_of_signals = 0;
    total_number_of_asyncs = 0;
    total_number_of_aprocs = 0;

    aa_process_table = new sc_async_aproc_process_table;
    error_occurred = false;
}

sc_simcontext::~sc_simcontext()
{
    delete aa_process_table;

    delete prioq;
    for (int i = 0; i < trace_files.size(); ++i)
        delete trace_files[i];

    delete[] signals_to_update_arrays[0];
    delete[] signals_to_update_arrays[1];

    delete[] clock_edges_async_arrays[0];
    delete[] clock_edges_async_arrays[1];

    delete[] clock_edges_to_advance;

    delete[] asyncs_to_execute;
    delete[] aprocs_to_execute;

    delete port_manager;
    delete concat_manager;
    delete object_manager;
}

void
sc_simcontext::initialize()
{
    if (! ready_to_simulate) {

        port_manager->resolve_all_ports();
        watching_fn = watching_during_simulation;

        /* signals_to_update_arrays[0] already initialized */
        signals_to_update_arrays[1] = new sc_signal_base*[total_number_of_signals];
        curr_array = 0;

        assert( signals_to_update == signals_to_update_arrays[0] );

        int clock_edge_count = 0;
        {  /* This block will destroy 'it' when it exits. */
          sc_plist<sc_clock_edge*>::iterator it(clock_edge_list);
          while (! it.empty()) {
            sc_clock_edge* edge_tmp = *it;
            sc_clock_helpers::prepare_for_simulation(edge_tmp,
                                                     total_number_of_signals);
            // There's something weird about the sparc.  Even though
            // the following function call is also made in
            // prepare_for_simulation
            // (above), if you remove either this one or the one in
            // prepare_for_simulation, the execution time is somewhat longer.
            // But then, how do we predict which configuration is optimal?
            // This is due to instruction cache misses, I believe.  Not much
            // we can do about it.
            sc_signal_optimize::optimize_clock(&(edge_tmp->clock()));
            it++;
            clock_edge_count++;
          }
        }

        clock_edges_async_arrays[0] = new sc_clock_edge*[clock_edge_count];
        clock_edges_async_arrays[1] = new sc_clock_edge*[clock_edge_count];
        curr_clock_edges_array = 0;
        lastof_clock_edges_async = -1;

        clock_edges_to_advance = new sc_clock_edge*[clock_edge_count];
        lastof_clock_edges_to_advance = -1;

        asyncs_to_execute = new sc_async_process_handle[total_number_of_asyncs];

        /* one extra for the sentinel */
        aprocs_to_execute = new sc_aproc_process_handle[total_number_of_aprocs + 1];
        sc_aproc_process_handle aproc_h;
        for (aproc_h = aa_process_table->aproc_head;
             NIL(sc_aproc_process_handle) != aproc_h;
             aproc_h = aproc_h->next_handle) {
            aproc_h->prepare_for_simulation();
        }

        next_aproc_index = 0;
        ready_to_simulate = true;

        lastof_signals_to_update = total_number_of_signals - 1;

        lastof_asyncs_to_execute = -1;
        lastof_aprocs_to_execute = -1;

        initial_crunch();
        /* The first call the trace_cycle simply dumps the current value.
           We need the initial cycle too! */
	trace_cycle( /* is not delta cycle */ false );
        lastof_signals_to_update = -1;
    
#ifdef WIN32
        if (NULL == ConvertThreadToFiber(NULL)) {
            cerr << "SystemC error: Unable to convert thread to fiber (Windows NT)." << endl;
            error_occurred = true;
        }
#endif
    }  /* if not ready_to_simulate */

    if (error_occurred) {
        cerr << "SystemC Info: Simulation cannot begin due to previous errors." << endl;
        exit(1);        
    }
}

void
sc_simcontext::reset()
{
    delete prioq;
    prioq = 0;
    curr_time = 0.0;
    sc_plist<sc_clock_edge*>::iterator it(clock_edge_list);
    while (! it.empty()) {
        (*it)->reset();
        it++;
    }
}

void
sc_simcontext::initial_crunch()
{
    int i;
#ifdef DEBUG_SYSTEMC
    int ndelta = 0;  // Number of delta cycles.
#endif

    while (true) {
        lastof_asyncs_to_execute = -1;
        lastof_aprocs_to_execute = -1;
        for (i = lastof_signals_to_update; i >= 0; --i) {
            sc_signal_base* sig = signals_to_update[i];
            sig->update();
            /* Make sure we don't generate spurious events
               to trigger edge-sensitive processes, by
               prematurely updating the old value too. */
            sig->update_old();
            sig->submitted = false;
        }

        lastof_signals_to_update = -1;
        lastof_clock_edges_to_advance = -1;
        for (i = lastof_asyncs_to_execute; i >= 0; --i) {
            sc_async_process_handle async = asyncs_to_execute[i];
            (void) set_curr_proc( async );
            async->set_in_updateq( false );
            async->execute();
        }
        /* Don't trigger aprocs */
        for (i = lastof_aprocs_to_execute; i >= 0; --i) {
            sc_aproc_process_handle aproc = aprocs_to_execute[i];
            aproc->set_in_updateq( false );
        }

        (void) reset_curr_proc();

        /* No more signals to update, quit */
        if (lastof_signals_to_update == -1)
            break;
        else if (forced_stop) { /* Check for calls to sc_stop(). */
            cerr << "SystemC error: Detected sc_stop() during the initialization phase.\n" 
                 << "During this phase, your method processes are run until the signals\n" 
                 << "stabilize. This error means that there is a cycle of processes that\n"
                 << "prevents the signals from stabilizing, a possible sign of an infinite loop." 
                 << endl;
            abort();
        }
#ifdef DEBUG_SYSTEMC
        else if (++ndelta > SC_MAX_NUM_DELTA_CYCLES) { /* Check for possible infinite loops. */
            cerr << "SystemC error: Cannot finish the initialization phase in " 
                 << SC_MAX_NUM_DELTA_CYCLES << " delta cycles.\n"
                 << "During this phase, your method processes are run until the signals\n" 
                 << "stabilize. This error means that there is a cycle of processes that\n"
                 << "prevents the signals from stabilizing, a possible sign of an infinite loop.\n" 
                 << "You can delay this detection by setting SC_MAX_NUM_DELTA_CYCLES\n"
                 << "in sc_constants.h to a larger value."
                 << endl;
            abort();
        }
#endif
    }  // while
    lastof_clock_edges_to_advance = -1;
}  /* sc_simcontext::initial_crunch */

void
sc_simcontext::crunch()
{
    int i, j;
#ifdef DEBUG_SYSTEMC
    int ndelta = 0;  // Number of delta cycles.
#endif

    /* Some premature regalloc optimizations - root of all evil! */
    int my_curr_array = curr_array;   // This register-optimizes access to curr_array

    while (true) {
        lastof_asyncs_to_execute = -1;
        lastof_aprocs_to_execute = -1;

        /* Save for updating old values -- also use as register allocation */
        sc_signal_base** old_signals_to_update = signals_to_update;
        int old_lastof_signals_to_update = lastof_signals_to_update;

        int my_curr_clock_edges_array = curr_clock_edges_array;
        sc_clock_edge* const* old_clock_edges_async = 
            clock_edges_async_arrays[my_curr_clock_edges_array];
        int old_lastof_clock_edges_async = lastof_clock_edges_async;

        /* Switch to the other clock_edges_array */
        curr_clock_edges_array = 1 - my_curr_clock_edges_array;
        lastof_clock_edges_async = -1;

        /* This updates all the signals in the queue.  Note that if
           any signal is a clock, then the corresponding edge will be
           inserted into the clock_edges_async_arrays[curr_clock_edges_array]
           as well as clock_edges_to_advance arrays */
        if (old_lastof_signals_to_update >= 0) {
            i = old_lastof_signals_to_update;
            do {
                sc_signal_base* sig = old_signals_to_update[i];
                (*sig->update_func)(sig);
                sig->submitted = false;
            } while (--i >= 0);
        }

        if (old_lastof_clock_edges_async >= 0) {
            i = old_lastof_clock_edges_async;
            do {
                sc_clock_edge* const edge = old_clock_edges_async[i];
                j = edge->lastof_signals_to_update;
                if (j >= 0) {
                    sc_signal_base* const* l_signals_to_update = edge->signals_to_update;
                    do {
                        sc_signal_base* const sig = l_signals_to_update[j];
                        (*sig->update_func)(sig);
                        sig->submitted = false;
                    } while (--j >= 0);
                }
            } while (--i >= 0);
        }

        /* Switch to the other array; this should be somewhat faster
           than using ?: */
        my_curr_array = 1 - my_curr_array;
        signals_to_update = signals_to_update_arrays[my_curr_array];
        lastof_signals_to_update = -1;

        /* Execute asynchronous BLOCKS (sc_async) (those without wait()) */
        i = lastof_asyncs_to_execute;
        if (i >= 0) {
            sc_async_process_handle* const l_asyncs_to_execute = asyncs_to_execute;
            do {
                sc_async_process_handle async = l_asyncs_to_execute[i];
                (void) set_curr_proc( async );
                async->set_in_updateq( false );
                async->execute();
            } while (--i >= 0);
        }

        /* Execute asynchronous PROCESSES (sc_aproc) (those with wait()) */
        int l_lastof_aprocs_to_execute = lastof_aprocs_to_execute;
        if (l_lastof_aprocs_to_execute >= 0) {
            sc_aproc_process_handle* const l_aprocs_to_execute = aprocs_to_execute;
            sc_aproc_process_handle aproc = l_aprocs_to_execute[0];

            /* this is so that the helper knows how to set the sp of
               the simulation context.  see yieldhelp() */
            next_aproc_index = 1;
            l_aprocs_to_execute[l_lastof_aprocs_to_execute + 1] = (sc_aproc_process_handle) 0;
            (void) set_curr_proc( aproc );

#ifndef WIN32 
            context_switch( sc_simcontext_yieldhelp, this, 0, aproc->sp );
#else
	    /* On WinNT, save the current fiber and then schedule 
	     * the aproc's fiber 
	     */
	    pFiber = GetCurrentFiber();
            context_switch( aproc->pFiber );
#endif
            /* The necessary set_in_updateq(false) is in sc_aproc::wait() */
        }

	/* Update the old values now that the delta cycle is finished. */
	for (i = old_lastof_signals_to_update; i >= 0; --i) {
            sc_signal_base* const sig = old_signals_to_update[i];
            sig->update_old();
        }

        /* Update old values of signals tied to the output of synchronous
           processes. */

        for (i = old_lastof_clock_edges_async; i >= 0; --i) {
            sc_clock_edge* const edge = old_clock_edges_async[i];
            j = edge->lastof_signals_to_update;
            if (j >= 0) {
                sc_signal_base* const* l_signals_to_update = edge->signals_to_update;
                do {
                    sc_signal_base* const sig = l_signals_to_update[j];
                    sig->update_old();
                } while (--j >= 0);
            }
        }

        if (lastof_signals_to_update + lastof_clock_edges_async == -2) {
            break;
        } else {
            trace_cycle( /* delta cycle? */ true );

            if (forced_stop)  /* Check for calls to sc_stop(). */
                break;
#ifdef DEBUG_SYSTEMC
            else if (++ndelta > SC_MAX_NUM_DELTA_CYCLES) {  /* Check for possible infinite loops. */
                cerr << "SystemC warning: The number of delta cycles is larger than " 
                     << SC_MAX_NUM_DELTA_CYCLES << ", defined in sc_constants.h. \n"
                     << "This is a possible sign of an infinite loop. \n"
                     << "Increase this limit if this warning is invalid."
                     << endl;
                break;
            }
#endif
        }
    }  // while

    i = lastof_clock_edges_to_advance;
    if (i >= 0) {
        sc_clock_edge* const* const l_clock_edges_to_advance = clock_edges_to_advance;
        do {
            sc_clock_edge* const edge = l_clock_edges_to_advance[i];
            lastof_signals_to_update = -1;
            signals_to_update = edge->signals_to_update;
            (void) edge->advance();
            edge->lastof_signals_to_update = lastof_signals_to_update;
            edge->clock().set_edge_seen_in_delta_cycle( false );
        } while (--i >= 0);
        lastof_clock_edges_to_advance = -1;
    }

    curr_array = my_curr_array;
    signals_to_update = signals_to_update_arrays[my_curr_array];
    lastof_signals_to_update = -1;
} /* sc_simcontext::crunch() */


#ifndef WIN32
qt_t*
sc_simcontext::next_aproc_qt()
{
    sc_aproc_process_handle next_aproc = aprocs_to_execute[next_aproc_index++];
    if (next_aproc != 0) {
        set_curr_proc( next_aproc );
	return next_aproc->sp;
    }
    else {
        reset_curr_proc();
	return sp;
    }
}

#else

PVOID
sc_simcontext::next_aproc_fiber()
{
    sc_aproc_process_handle next_aproc = aprocs_to_execute[next_aproc_index++];
    if (next_aproc != 0) {
        set_curr_proc( next_aproc );
        return next_aproc->pFiber;
    }
    else {
        reset_curr_proc();
        return pFiber;
    }
}

#endif

void
sc_simcontext::submit_clock_edge( sc_clock_edge* edge )
{
    clock_edges_async_arrays[curr_clock_edges_array][++lastof_clock_edges_async] = edge;
    clock_edges_to_advance[++lastof_clock_edges_to_advance] = edge;
}

double
sc_simcontext::simulate_forever( sc_simcontext::callback_fn callback, void* arg )
{
    sc_clock_edge** edge_tmp_array;
    edge_tmp_array = new sc_clock_edge* [prioq->size()];

    while (true) {

        sc_clock_edge* edge_tmp;   /* This is used to register allocate `edge'. */

        // Check if we received a signal to stop.
        if (forced_stop) {
            cerr << "SystemC: simulation stopped by user.\n";
	    delete[] edge_tmp_array;
            return prioq->top()->next_edge();
        }

	double time_of_upcoming_edge = prioq->top()->next_edge();
	int i = 0;

	do {
	    edge_tmp_array[i++] = edge_tmp = prioq->extract_top();

	    /* Toggle the signal associated with the clock. */
	    (void) sc_clock_helpers::toggle_clock_signal(edge_tmp);

	} while (prioq->top()->next_edge() == time_of_upcoming_edge);

	// Set the current edge of the simulation context.
	curr_edge = edge_tmp;
      
        (void) (*callback)( *edge_tmp, arg );
        crunch();
      
	while (--i >= 0) {
	    prioq->insert(edge_tmp_array[i]);
	}
      
        trace_cycle( /* is not delta cycle */ false );
    }
}

double
sc_simcontext::simulate_forever()
{
    sc_clock_edge** edge_tmp_array;
    edge_tmp_array = new sc_clock_edge* [prioq->size()];

    while (true) {

        sc_clock_edge* edge_tmp;   /* This is used to register allocate `edge'. */
        
	// Check if we received a signal to stop.
        if (forced_stop) {
            cerr << "SystemC: simulation stopped by user.\n";
	    delete [] edge_tmp_array;
            return prioq->top()->next_edge();
        }

	double time_of_upcoming_edge = prioq->top()->next_edge();
	int i = 0;

	do {
	    edge_tmp_array[i++] = edge_tmp = prioq->extract_top();

	    /* Toggle the signal associated with the clock. */
	    (void) sc_clock_helpers::toggle_clock_signal(edge_tmp);

	} while (prioq->top()->next_edge() == time_of_upcoming_edge);

	// Set the current edge of the simulation context.
	curr_edge = edge_tmp;

        crunch();
	
	while (--i >= 0) {
	    prioq->insert(edge_tmp_array[i]);
	}
      
        trace_cycle( /* is not delta cycle */ false );
    }
}

double
sc_simcontext::simulate_until( double until, sc_simcontext::callback_fn callback, void* arg )
{
    sc_clock_edge** edge_tmp_array;
    edge_tmp_array = new sc_clock_edge* [prioq->size()];

    double time_of_upcoming_edge;

    while ((time_of_upcoming_edge = prioq->top()->next_edge()) < until) {

        sc_clock_edge* edge_tmp;   /* This is used to register allocate `edge'. */

        // Check if we received a signal to stop.
        if (forced_stop) {
            cerr << "SystemC: simulation stopped by user.\n";
	    delete [] edge_tmp_array;
            return time_of_upcoming_edge;
        }

	int i = 0;
	do {
	    edge_tmp_array[i++] = edge_tmp = prioq->extract_top();

	    /* Toggle the signal associated with the clock. */
	    (void) sc_clock_helpers::toggle_clock_signal(edge_tmp);
	
	} while (prioq->top()->next_edge() == time_of_upcoming_edge);

	// Set the current edge of the simulation context.
	curr_edge = edge_tmp;

        (void) (*callback)( *edge_tmp, arg );
        crunch();
      
	while (--i >= 0) {
            prioq->insert(edge_tmp_array[i]);
	}
      
        trace_cycle( /* is not delta cycle */ false );
    }

    delete [] edge_tmp_array;
    
    return until;
}

double
sc_simcontext::simulate_until( double until )
{
    sc_clock_edge** edge_tmp_array;
    edge_tmp_array = new sc_clock_edge* [prioq->size()];

    double time_of_upcoming_edge;

    while ((time_of_upcoming_edge = prioq->top()->next_edge()) < until) {

        sc_clock_edge* edge_tmp;   /* This is used to register allocate `edge'. */

        // Check if we received a signal to stop.
        if (forced_stop) {
            cerr << "SystemC: simulation stopped by user.\n";
	    delete [] edge_tmp_array;
            return time_of_upcoming_edge;
        }

	int i = 0;
	do {
	    edge_tmp_array[i++] = edge_tmp = prioq->extract_top();

	    /* Toggle the signal associated with the clock. */
	    (void) sc_clock_helpers::toggle_clock_signal(edge_tmp);
	
	} while (prioq->top()->next_edge() == time_of_upcoming_edge);

	// Set the current edge of the simulation context.
	curr_edge = edge_tmp;

        crunch();
      
	while (--i >= 0) {
            prioq->insert(edge_tmp_array[i]);
	}
      
        trace_cycle( /* is not delta cycle */ false );
    }

    delete [] edge_tmp_array;
    
    return until;
}

bool
sc_simcontext::simulate( double duration, sc_clock::callback_fn callback, void* arg )
{
    if (
#ifndef WIN32
        curr_time < -0.1
#else
        curr_time + 0.1 < 0 
#endif
        ) {
      cerr << "SystemC: no more processes in current context, simulation stopped.\n";
      return false;
    }
    
    sc_curr_simcontext = this;

    if (! ready_to_simulate)
        initialize();

    if (!prioq) {
        prioq = new sc_ppq<sc_clock_edge*>(clock_edge_list.size(), next_edge_compare);
        sc_plist<sc_clock_edge*>::iterator it( clock_edge_list );
        while (! it.empty()) {
            // Only do automatic clock generation for those
            // clocks whose period is positive
            if ((*it)->period() > 0.0) {
                prioq->insert( *it );
            }
            it++;
        }
    }
    if (0 == prioq->size()) {
        return false;
    }    

    forced_stop = false;
    if (duration < 0) {
        if (callback != 0)
            curr_time = simulate_forever( callback, arg );
        else
            curr_time = simulate_forever();
    }
    else {
        if (callback != 0)
            curr_time = simulate_until( curr_time + duration, callback, arg );
        else
            curr_time = simulate_until( curr_time + duration );
    }
    
    curr_edge = 0;
    return (
#ifndef WIN32
        curr_time >= -0.1
#else
        curr_time + 0.1 >= 0
#endif
        );
}

void
sc_simcontext::stop()
{
    forced_stop = true;
}

sc_plist<sc_clock_edge*>::handle_t
sc_simcontext::add_clock_edge( sc_clock_edge* ce )
{
    assert(! ready_to_simulate);
    return clock_edge_list.push_back(ce);
}

void
sc_simcontext::add_signal( sc_signal_base* s )
{
    if (ready_to_simulate) {
        cerr << "Signals may not be created after simulation has started." << endl;
        assert( false );
    }

    /* resize the array if necessary; make sure we have enough
       space for signal writes that appear in constructors */
    if (total_number_of_signals == update_array0_alloc) {
        update_array0_alloc += 64;
        sc_signal_base** new_array = new sc_signal_base*[update_array0_alloc];

        /* Copy old array into new array */
        for (int i = 0; i < total_number_of_signals; ++i)
            new_array[i] = signals_to_update[i];

        /* Free the old array */
        delete[] signals_to_update;

        /* Make sure we update both */
        signals_to_update = new_array;
        signals_to_update_arrays[0] = new_array;
    }

    /* Stick the signal to the update queue for the initial update */
    s->submitted = true;
    signals_to_update[total_number_of_signals++] = s;
}

double
sc_simcontext::time_stamp() const
{
    return curr_edge ? curr_edge->time_stamp() : curr_time;
}

void
sc_simcontext::add_trace_file( sc_trace_file* tf )
{
    trace_files.push_back(tf);
}

void
sc_simcontext::trace_cycle( bool delta_cycle )
{
    sc_trace_file* const* const l_trace_files = trace_files.raw_data();
    for (int i = trace_files.size() - 1; i >=0; --i) {
        l_trace_files[i]->cycle( delta_cycle );
    }
}

void
sc_simcontext::hierarchy_push(sc_module* mdl)
{
    object_manager->hierarchy_push(mdl);
}

sc_module*
sc_simcontext::hierarchy_pop()
{
    sc_module* m = (sc_module*) object_manager->hierarchy_pop();
    return m;
}

sc_module*
sc_simcontext::hierarchy_curr() const
{
    sc_module* m;
    m = (sc_module*) object_manager->hierarchy_curr();
    return m;
}
    
sc_object*
sc_simcontext::first_object()
{
    return object_manager->first_object();
}

sc_object*
sc_simcontext::next_object()
{
    return object_manager->next_object();
}

sc_object*
sc_simcontext::find_object(const char* name)
{
    return object_manager->find_object(name);
}


sc_simcontext*
sc_get_curr_simcontext()
{
    if (NIL(sc_simcontext*) == sc_curr_simcontext) {
#ifdef PURIFY
        static sc_simcontext sc_default_global_context;
        sc_curr_simcontext = &sc_default_global_context;
#else
        static sc_simcontext* sc_default_global_context = new sc_simcontext;
        sc_curr_simcontext = sc_default_global_context;
#endif
    }
    return sc_curr_simcontext;
}

double
sc_simulation_time()
{
    return sc_get_curr_simcontext()->time_stamp();
}

double
sc_time_stamp()
{ 
    return sc_get_curr_simcontext()->time_stamp();
}

void
sc_initialize()
{
    sc_module::validate_all();
    sc_get_curr_simcontext()->initialize();
}

void
sc_start(double duration, sc_callback_fn callback, void* arg)
{
    sc_module::validate_all();
    (void) sc_get_curr_simcontext()->simulate(duration, callback, arg);
}

void
sc_start(sc_clock& clock, double duration)
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    double period = clock.period();
    double duty_cycle = clock.duty_cycle();
    double first_half  = period * duty_cycle;
    double second_half = period - first_half;
    double cycle_count_d = duration / period;
    unsigned cycle_count = ((cycle_count_d > (double)UINT_MAX) ?
                            UINT_MAX : (unsigned) cycle_count_d);

    sc_initialize();
    if (false == clock.read()) {
        /* First edge positive? */
        if (duration < 0) {
            for (;;) {
                if (simc->forced_stop)
                    break;
                // Here we use sc_clock_helpers::clock_write()
                // to get around the problem of making a
                // virtual function call (because operator=()
                // is virtual.
                sc_clock_helpers::clock_write(&clock, 1);
                simc->sc_cycle(first_half);
                sc_clock_helpers::clock_write(&clock, 0);
                simc->sc_cycle(second_half);
            }
        } else {
            double until = simc->curr_time + duration;
            for (unsigned i = 0; i < cycle_count; ++i) {
                if (simc->forced_stop)
                    break;
                sc_clock_helpers::clock_write(&clock, 1);
                simc->sc_cycle(first_half);
                sc_clock_helpers::clock_write(&clock, 0);
                simc->sc_cycle(second_half);
            }
            while (simc->time_stamp() < until) {
                if (simc->forced_stop)
                    break;
                sc_clock_helpers::clock_write(&clock, 1);
                simc->sc_cycle(first_half);
                sc_clock_helpers::clock_write(&clock, 0);
                simc->sc_cycle(second_half);
            }
        }
    } else {
        /* First edge negative? */
        if (duration < 0) {
            for (;;) {
                if (simc->forced_stop)
                    break;
                sc_clock_helpers::clock_write(&clock, 0);
                simc->sc_cycle(first_half);
                sc_clock_helpers::clock_write(&clock, 1);
                simc->sc_cycle(second_half);
            }
        } else {
            double until = simc->curr_time + duration;
            for (unsigned i = 0; i < cycle_count; ++i) {
                if (simc->forced_stop)
                    break;
                sc_clock_helpers::clock_write(&clock, 0);
                simc->sc_cycle(first_half);
                sc_clock_helpers::clock_write(&clock, 1);
                simc->sc_cycle(second_half);
            }
            // I have noticed a few times (not deterministically,
            // unfortunately) that adding *two* lines of
            // sc_signal_optimize::optimize_clock(&clock)
            // here would significantly degrade the performance
            // of the a2901 example (with one process).  But I
            // cannot always reproduce the result ...
            while (simc->time_stamp() < until) {
                if (simc->forced_stop)
                    break;
                sc_clock_helpers::clock_write(&clock, 0);
                simc->sc_cycle(second_half);
                sc_clock_helpers::clock_write(&clock, 1);
                simc->sc_cycle(first_half);
            }
        }
    }
}

void
sc_stop()
{
    sc_get_curr_simcontext()->stop();
}

sc_async_process_handle
sc_simcontext::register_sc_async_process(const char* name,
                                         SC_ENTRY_FUNC entry_fn,
                                         sc_module* module)
{
    total_number_of_asyncs++;
    sc_async_process_handle handle = new sc_async_process(name, entry_fn, module);
    if (NIL(sc_async_process_handle) == aa_process_table->async_head) {
        aa_process_table->async_head = handle;
    } else {
        aa_process_table->async_tail->next_handle = handle;
    }
    aa_process_table->async_tail = handle;
    set_curr_proc( handle );
    return handle;
}

sc_aproc_process_handle
sc_simcontext::register_sc_aproc_process(const char* name,
                                         SC_ENTRY_FUNC entry_fn,
                                         sc_module* module)
{
    total_number_of_aprocs++;
    sc_aproc_process_handle handle = new sc_aproc_process(name, entry_fn, module);
    if (NIL(sc_aproc_process_handle) == aa_process_table->aproc_head) {
        aa_process_table->aproc_head = handle;
    } else {
        aa_process_table->aproc_tail->next_handle = handle;
    }
    aa_process_table->aproc_tail = handle;
    set_curr_proc( handle );
    return handle;
}

sc_sync_process_handle
sc_simcontext::register_sc_sync_process(const char* name,
                                        SC_ENTRY_FUNC entry_fn,
                                        sc_module* module, 
                                        sc_clock_edge& clk_edge)
{
    return clk_edge.register_sc_sync_process(name, entry_fn, module);
}

sc_sync_process_handle
sc_simcontext::register_sc_sync_process(const char* name,
                                        SC_ENTRY_FUNC entry_fn,
                                        sc_module* module, 
                                        const sc_polarity& polarity)
{
    sc_sync_process_handle handle;
    handle = new sc_sync_process(name, entry_fn, module, (sc_clock_edge*)0);
    handle->polarity = polarity;
    port_manager->add_unbound_sync_handle(handle);
    set_curr_proc( handle );
    return handle;
}

void
sc_defunct_process_function(sc_module* p)
{
    /* This function is pointed to by defunct
       sc_aproc_process'es and sc_sync_process'es.
       In a correctly constructed world, this
       function should never be called; hence the
       assert. */
    assert(false);
}
