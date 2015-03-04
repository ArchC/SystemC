/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2001 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_simcontext.cpp -- Provides a simulation context for use with multiple
                       simulations.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Added sc_stop() detection into initial_crunch
                                 and crunch. This makes it possible to exit out
                                 of a combinational loop using sc_stop().

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/kernel/sc_context_switch.h"
#include "systemc/kernel/sc_event.h"
#include "systemc/kernel/sc_lambda.h"
#include "systemc/kernel/sc_macros_int.h"
#include "systemc/kernel/sc_module.h"
#include "systemc/kernel/sc_module_registry.h"
#include "systemc/kernel/sc_name_gen.h"
#include "systemc/kernel/sc_object_manager.h"
#include "systemc/kernel/sc_process_int.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/kernel/sc_simcontext_int.h"
#include "systemc/kernel/sc_ver.h"
#include "systemc/communication/sc_port.h"
#include "systemc/communication/sc_prim_channel.h"
#include "systemc/tracing/sc_trace.h"
#include "systemc/utils/sc_mempool.h"
#ifndef WIN32
#include "systemc/qt/qt.h"
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_process_table
//
//  Container class that keeps track of all method processes,
//  thread processes, and cthread processes.
// ----------------------------------------------------------------------------

typedef sc_pvector<sc_method_handle>  sc_method_vec;
typedef sc_pvector<sc_thread_handle>  sc_thread_vec;
typedef sc_pvector<sc_cthread_handle> sc_cthread_vec;


class sc_process_table
{
public:

    sc_process_table();
    ~sc_process_table();

    void push_back( sc_method_handle );
    void push_back( sc_thread_handle );
    void push_back( sc_cthread_handle );

    sc_method_handle remove( sc_method_handle );
    sc_thread_handle remove( sc_thread_handle );
    sc_cthread_handle remove( sc_cthread_handle );

    int num_methods() const;
    int num_threads() const;
    int num_cthreads() const;

    const sc_method_vec& method_vec() const;
    const sc_thread_vec& thread_vec() const;
    const sc_cthread_vec& cthread_vec() const;

private:

    sc_method_vec  m_method_vec;
    sc_thread_vec  m_thread_vec;
    sc_cthread_vec m_cthread_vec;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

sc_process_table::sc_process_table()
{}

sc_process_table::~sc_process_table()
{
    // delete all method processes
    for( int i = m_method_vec.size() - 1; i >= 0; -- i ) {
	delete m_method_vec[i];
    }

    // delete all thread processes
    for( int i = m_thread_vec.size() - 1; i >= 0; -- i ) {
	delete m_thread_vec[i];
    }

    // delete all cthread processes
    for( int i = m_cthread_vec.size() - 1; i >= 0; -- i ) {
	delete m_cthread_vec[i];
    }
}


inline
void
sc_process_table::push_back( sc_method_handle handle_ )
{
    m_method_vec.push_back( handle_ );
}

inline
void
sc_process_table::push_back( sc_thread_handle handle_ )
{
    m_thread_vec.push_back( handle_ );
}

inline
void
sc_process_table::push_back( sc_cthread_handle handle_ )
{
    m_cthread_vec.push_back( handle_ );
}


sc_method_handle
sc_process_table::remove( sc_method_handle handle_ )
{
    int size = m_method_vec.size();
    for( int i = 0; i < size; ++ i ) {
	if( m_method_vec[i] == handle_ ) {
	    m_method_vec[i] = m_method_vec[size - 1];
	    m_method_vec.decr_count();
	    return handle_;
	}
    }
    return 0;
}

sc_thread_handle
sc_process_table::remove( sc_thread_handle handle_ )
{
    int size = m_thread_vec.size();
    for( int i = 0; i < size; ++ i ) {
	if( m_thread_vec[i] == handle_ ) {
	    m_thread_vec[i] = m_thread_vec[size - 1];
	    m_thread_vec.decr_count();
	    return handle_;
	}
    }
    return 0;
}

sc_cthread_handle
sc_process_table::remove( sc_cthread_handle handle_ )
{
    int size = m_cthread_vec.size();
    for( int i = 0; i < size; ++ i ) {
	if( m_cthread_vec[i] == handle_ ) {
	    m_cthread_vec[i] = m_cthread_vec[size - 1];
	    m_cthread_vec.decr_count();
	    return handle_;
	}
    }
    return 0;
}


inline
int
sc_process_table::num_methods() const
{
    return m_method_vec.size();
}

inline
int
sc_process_table::num_threads() const
{
    return m_thread_vec.size();
}

inline
int
sc_process_table::num_cthreads() const
{
    return m_cthread_vec.size();
}


inline
const sc_method_vec&
sc_process_table::method_vec() const
{
    return m_method_vec;
}

inline
const sc_thread_vec&
sc_process_table::thread_vec() const
{
    return m_thread_vec;
}

inline
const sc_cthread_vec&
sc_process_table::cthread_vec() const
{
    return m_cthread_vec;
}


// ----------------------------------------------------------------------------

void
pln()
{
    static bool lnp = false;
    if( ! lnp ) {
        cerr << endl;
	cerr << sc_version() << endl;
	cerr << sc_copyright() << endl;

	//  regressions check point
        if( getenv( "SYSTEMC_REGRESSION" ) != 0 ) {
            cerr << "SystemC Simulation" << endl;
        }

        lnp = true;
    }
}


int
sc_notify_time_compare( const void* p1, const void* p2 )
{
    const sc_event_timed* et1 = static_cast<const sc_event_timed*>( p1 );
    const sc_event_timed* et2 = static_cast<const sc_event_timed*>( p2 );

    const sc_time& t1 = et1->notify_time();
    const sc_time& t2 = et2->notify_time();
    
    if( t1 < t2 ) {
	return 1;
    } else if( t1 > t2 ) {
	return -1;
    } else {
	return 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_simcontext
//
//  The simulation context.
// ----------------------------------------------------------------------------

void
sc_simcontext::init()
{
    m_object_manager = new sc_object_manager;
    m_module_registry = new sc_module_registry( *this );
    m_port_registry = new sc_port_registry( *this );
    m_prim_channel_registry = new sc_prim_channel_registry( *this );
    m_name_gen = new sc_name_gen;
    m_process_table = new sc_process_table;
    reset_curr_proc();
    m_next_proc_id = -1;
    m_timed_events = new sc_ppq<sc_event_timed*>( 128,
						  sc_notify_time_compare );
    m_something_to_trace = false;
    m_runnable = new sc_runnable;
    m_time_params = new sc_time_params;
    m_curr_time = SC_ZERO_TIME;
    m_delta_count = 0;
    m_forced_stop = false;
    m_ready_to_simulate = false;
    m_update_phase = false;
    m_error = false;
// #ifndef WIN32
//     m_sp = ?
// #else
//     m_fiber = ?
// #endif
    m_watching_fn = watching_before_simulation;
}

void
sc_simcontext::clean()
{
    delete m_object_manager;
    delete m_module_registry;
    delete m_port_registry;
    delete m_prim_channel_registry;
    delete m_name_gen;
    delete m_process_table;
    m_child_objects.erase_all();
    m_delta_events.erase_all();
    delete m_timed_events;
    for( int i = m_trace_files.size() - 1; i >= 0; -- i ) {
	delete m_trace_files[i];
    }
    m_trace_files.erase_all();
    delete m_runnable;
    delete m_time_params;
}


sc_simcontext::sc_simcontext()
{
    init();
}

sc_simcontext::~sc_simcontext()
{
    clean();
}


void
sc_simcontext::initialize( bool no_crunch )
{
    if( m_ready_to_simulate || sim_status() != SC_SIM_OK ) {
        return;
    }

    m_port_registry->elaboration_done();
    m_prim_channel_registry->elaboration_done();
    m_module_registry->elaboration_done();

    // check for call(s) to sc_stop
    if( m_forced_stop ) {
        cout << "SystemC: simulation stopped by user.\n";
        return;
    }

    m_watching_fn = watching_during_simulation;

    // allocate space in the runnable queues
    m_runnable->alloc( m_process_table->num_methods(),
		       m_process_table->num_threads() +
		       m_process_table->num_cthreads() );

    // prepare all thread processes for simulation
    const sc_thread_vec& thread_vec = m_process_table->thread_vec();
    for( int i = thread_vec.size() - 1; i >= 0; -- i ) {
	thread_vec[i]->prepare_for_simulation();
    }

    // prepare all cthread processes for simulation
    const sc_cthread_vec& cthread_vec = m_process_table->cthread_vec();
    for( int i = cthread_vec.size() - 1; i >= 0; -- i ) {
	cthread_vec[i]->prepare_for_simulation();
    }

    m_ready_to_simulate = true;

    m_runnable->init();

    // update phase

    m_update_phase = true;
    m_prim_channel_registry->perform_update();
    m_update_phase = false;

    int size;

    // make all method processes runnable

    const sc_method_vec& method_vec = m_process_table->method_vec();
    size = method_vec.size();
    for( int i = 0; i < size; ++ i ) {
	sc_method_handle method_h = method_vec[i];
	if( method_h->do_initialize() ) {
	    push_runnable_method( method_h );
	}
    }

    // make all thread processes runnable

    size = thread_vec.size();
    for( int i = 0; i < size; ++ i ) {
	sc_thread_handle thread_h = thread_vec[i];
	if( thread_h->do_initialize() ) {
	    push_runnable_thread( thread_h );
	}
    }

    // process delta notifications
    
    if( ( size = m_delta_events.size() ) != 0 ) {
	sc_event** l_delta_events = m_delta_events.raw_data();
	int i = size - 1;
	do {
	    l_delta_events[i]->trigger();
	} while( -- i >= 0 );
	m_delta_events.erase_all();
    }

    if( no_crunch || m_runnable->push_empty() ) {
        return;
    }
    m_runnable->toggle();

    // run the delta cycle loop

    crunch();
    if( m_error ) {
        return;
    }
    if( m_something_to_trace ) {
        trace_cycle( /* delta cycle? */ false );
    }
    // check for call(s) to sc_stop
    if( m_forced_stop ) {
        cout << "SystemC: simulation stopped by user.\n";
    }
}

void
sc_simcontext::simulate( const sc_time& duration )
{
    initialize( true );

    if( sim_status() != SC_SIM_OK ) {
        return;
    }

    sc_time until_t = ( duration == SC_ZERO_TIME ) 
	              ? sc_time( ~const_zero_ull, false ) // max time
		      : m_curr_time + duration;

    sc_event until_e;
    until_e.notify( until_t - m_curr_time );

    sc_time t;
    
    do {
	m_runnable->toggle();

	crunch();
	if( m_error ) {
	    return;
	}
	if( m_something_to_trace ) {
	    trace_cycle( /* delta cycle? */ false );
	}
	// check for call(s) to sc_stop
	if( m_forced_stop ) {
	    cout << "SystemC: simulation stopped by user.\n";
	    return;
	}
	
	do {
	    t = next_time();

	    // PROCESS TIMED NOTIFICATIONS

	    do {
		sc_event_timed* et = m_timed_events->extract_top();
		sc_event* e = et->event();
		delete et;
		if( e != 0 ) {
		    e->trigger();
		}
	    } while( m_timed_events->size() &&
		     m_timed_events->top()->notify_time() == t );

	} while( m_runnable->push_empty() && t != until_t );

	m_curr_time = t;

    } while( t != until_t );
}

void
sc_simcontext::stop()
{
    m_forced_stop = true;
}

void
sc_simcontext::reset()
{
    clean();
    init();
}


void
sc_simcontext::hierarchy_push( sc_module* mod )
{
    m_object_manager->hierarchy_push( mod );
}

sc_module*
sc_simcontext::hierarchy_pop()
{
    return DCAST<sc_module*>( m_object_manager->hierarchy_pop() );
}

sc_module*
sc_simcontext::hierarchy_curr() const
{
    return DCAST<sc_module*>( m_object_manager->hierarchy_curr() );
}
    
sc_object*
sc_simcontext::first_object()
{
    return m_object_manager->first_object();
}

sc_object*
sc_simcontext::next_object()
{
    return m_object_manager->next_object();
}

sc_object*
sc_simcontext::find_object( const char* name )
{
    return m_object_manager->find_object( name );
}


// to generate unique names for objects in an MT-Safe way

const char*
sc_simcontext::gen_unique_name( const char* basename_ )
{
    return m_name_gen->gen_unique_name( basename_ );
}


sc_method_handle
sc_simcontext::register_method_process( const char* name,
				        SC_ENTRY_FUNC entry_fn,
				        sc_module* module )
{
    sc_method_handle handle = new sc_method_process( name,
						     entry_fn,
						     module );
    m_process_table->push_back( handle );
    set_curr_proc( handle );
    return handle;
}

sc_thread_handle
sc_simcontext::register_thread_process( const char* name,
					SC_ENTRY_FUNC entry_fn,
					sc_module* module )
{
    sc_thread_handle handle = new sc_thread_process( name,
						     entry_fn,
						     module );
    m_process_table->push_back( handle );
    set_curr_proc( handle );
    return handle;
}

sc_cthread_handle
sc_simcontext::register_cthread_process( const char* name,
					 SC_ENTRY_FUNC entry_fn,
					 sc_module* module )
{
    sc_cthread_handle handle = new sc_cthread_process( name,
						       entry_fn,
						       module );
    m_process_table->push_back( handle );
    set_curr_proc( handle );
    return handle;
}


void
sc_simcontext::add_trace_file( sc_trace_file* tf )
{
    m_trace_files.push_back( tf );
    m_something_to_trace = true;
}


#ifndef WIN32

qt_t*
sc_simcontext::next_thread_qt()
{
    if( m_error ) {
	return m_sp;
    }

    sc_thread_handle thread_h = pop_runnable_thread();
    while( thread_h != 0 && ! thread_h->ready_to_run() ) {
	thread_h = pop_runnable_thread();
    }
    
    if( thread_h != 0 ) {
	return thread_h->m_sp;
    } else {
	return m_sp;
    }
}

void*
sc_simcontext_yieldhelp( qt_t* sp, void* simc, void* )
{
    SCAST<sc_simcontext*>( simc )->m_sp = sp;
    return sp;
}

#else

PVOID
sc_simcontext::next_thread_fiber()
{
    if( m_error ) {
	return m_fiber;
    }
    
    sc_thread_handle thread_h = pop_runnable_thread();
    while( thread_h != 0 && ! thread_h->ready_to_run() ) {
	thread_h = pop_runnable_thread();
    }
    
    if( thread_h != 0 ) {
	return thread_h->m_fiber;
    } else {
	return m_fiber;
    }
}

#endif


const sc_pvector<sc_object*>&
sc_simcontext::get_child_objects() const
{
    return m_child_objects;
}

void
sc_simcontext::add_child_object( sc_object* object_ )
{
    // no check if object_ is already in the set
    m_child_objects.push_back( object_ );
}

void
sc_simcontext::remove_child_object( sc_object* object_ )
{
    int size = m_child_objects.size();
    for( int i = 0; i < size; ++ i ) {
	if( object_ == m_child_objects[i] ) {
	    m_child_objects[i] = m_child_objects[size - 1];
	    m_child_objects.decr_count();
	    return;
	}
    }
    // no check if object_ is really in the set
}


void
sc_simcontext::crunch()
{
#ifdef DEBUG_SYSTEMC
    int num_deltas = 0;  // number of delta cycles
#endif

    while( true ) {

	// EVALUATE PHASE
	
	while( true ) {

	    // execute method processes

	    sc_method_handle method_h = pop_runnable_method();
	    while( method_h != 0 ) {
		try {
		    method_h->execute();
		}
		catch( const sc_exception& ex ) {
		    cout << "\n" << ex.str() << "\n";
		    m_error = true;
		    return;
		}
		method_h = pop_runnable_method();
	    }

	    // execute (c)thread processes

	    sc_thread_handle thread_h = pop_runnable_thread();
	    while( thread_h != 0 && ! thread_h->ready_to_run() ) {
		thread_h = pop_runnable_thread();
	    }
	    if( thread_h != 0 ) {
#ifndef WIN32
		context_switch( sc_simcontext_yieldhelp,
				this,
				0,
				thread_h->m_sp );
#else
		m_fiber = GetCurrentFiber();
		context_switch( thread_h->m_fiber );
#endif
	    }
	    if( m_error ) {
		return;
	    }

	    // check for call(s) to sc_stop
	    if( m_forced_stop ) {
		break;
	    }

	    if( m_runnable->push_empty() ) {
		// no more runnable processes
		break;
	    }
	    m_runnable->toggle();
	}

	// UPDATE PHASE

	m_update_phase = true;
	m_prim_channel_registry->perform_update();
	m_update_phase = false;
	
	if( m_something_to_trace ) {
	    trace_cycle( /* delta cycle? */ true );
	}

	m_delta_count ++;

        // check for call(s) to sc_stop
        if( m_forced_stop ) {
            break;
        }

#ifdef DEBUG_SYSTEMC
        // check for possible infinite loops
        if( ++ num_deltas > SC_MAX_NUM_DELTA_CYCLES ) {
	    cerr << "SystemC warning: "
		 << "the number of delta cycles exceeds the limit of "
		 << SC_MAX_NUM_DELTA_CYCLES
		 << ", defined in sc_constants.h.\n"
		 << "This is a possible sign of an infinite loop.\n"
		 << "Increase the limit if this warning is invalid.\n";
	    break;
	}
#endif

	// PROCESS DELTA NOTIFICATIONS

        int size;
	if( ( size = m_delta_events.size() ) == 0 ) {
	    break;
	}

        sc_event** l_events = m_delta_events.raw_data();
        int i = size - 1;
        do {
	    l_events[i]->trigger();
        } while( -- i >= 0 );
	m_delta_events.erase_all();
	
	if( m_runnable->push_empty() ) {
	    // no more runnable processes
	    break;
	}
	m_runnable->toggle();
    }
}


const sc_time
sc_simcontext::next_time()
{
    while( m_timed_events->size() ) {
	sc_event_timed* et = m_timed_events->top();
	if( et->event() != 0 ) {
	    return et->notify_time();
	}
	delete m_timed_events->extract_top();
    }
    return SC_ZERO_TIME;
}


void
sc_simcontext::remove_delta_event( sc_event* e )
{
    int i = e->m_delta;
    int j = m_delta_events.size() - 1;
    assert( i >= 0 && i <= j );
    if( i != j ) {
	sc_event** l_delta_events = m_delta_events.raw_data();
	l_delta_events[i] = l_delta_events[j];
	l_delta_events[i]->m_delta = i;
    }
    m_delta_events.decr_count();
    e->m_delta = -1;
}


void
sc_simcontext::trace_cycle( bool delta_cycle )
{
    int size;
    if( ( size = m_trace_files.size() ) != 0 ) {
	sc_trace_file** l_trace_files = m_trace_files.raw_data();
	int i = size - 1;
	do {
	    l_trace_files[i]->cycle( delta_cycle );
	} while( -- i >= 0 );
    }
}


// ----------------------------------------------------------------------------

// Not MT-safe!
static sc_simcontext* sc_curr_simcontext = 0;


sc_simcontext*
sc_get_curr_simcontext()
{
    if( sc_curr_simcontext == 0 ) {
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


// Generates unique names within each module.

const char*
sc_gen_unique_name( const char* basename_ )
{
    sc_simcontext* simc = sc_get_curr_simcontext();
    sc_module* curr_module = simc->hierarchy_curr();
    if( curr_module != 0 ) {
	return curr_module->gen_unique_name( basename_ );
    } else {
	return simc->gen_unique_name( basename_ );
    }
}


// Set the random seed for controlled randomization -- not yet implemented

void
sc_set_random_seed( unsigned int )
{
    REPORT_WARNING( 2, "void sc_set_random_seed( unsigned int )" );
}


void
sc_start( const sc_time& duration )
{
    sc_get_curr_simcontext()->simulate( duration );
}

void
sc_stop()
{
    sc_get_curr_simcontext()->stop();
}


void
sc_initialize()
{
    sc_get_curr_simcontext()->initialize();
}

void
sc_cycle( const sc_time& duration )
{
    sc_get_curr_simcontext()->cycle( duration );
}


const sc_time&
sc_time_stamp()
{
    return sc_get_curr_simcontext()->time_stamp();
}

double
sc_simulation_time()
{
    return sc_get_curr_simcontext()->time_stamp().to_default_time_units();
}


void
sc_defunct_process_function( sc_module* )
{
    // This function is pointed to by defunct sc_thread_process'es and
    // sc_cthread_process'es. In a correctly constructed world, this
    // function should never be called; hence the assert.
    assert( false );
}


// ----------------------------------------------------------------------------

void
sc_assert_fail( const char* assertion_,
                const char* file_,
                int line_ )
{
    cerr << "SystemC: ";
    cerr << file_ << ":" << line_ << ": ";
    sc_simcontext* simc = sc_get_curr_simcontext();
    if( simc->is_running() ) {
        sc_process_b* p = simc->get_curr_proc_info()->process_handle;
        if( p != 0 ) {
            cerr << p->name() << ":";
        }
        cerr << sc_simulation_time() << ": ";
    }
    cerr << "Assertion `" << assertion_ << "' failed.\n";
    abort();
}


// Taf!
