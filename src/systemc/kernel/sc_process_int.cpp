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

  sc_process_int.cpp -- Process internals.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <assert.h>
#include <stdlib.h>
#include <cstddef>
#ifndef WIN32
#include <unistd.h>
#include <sys/mman.h>
#endif

#include "systemc/kernel/sc_process_int.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/kernel/sc_simcontext_int.h"


// ----------------------------------------------------------------------------

#ifndef WIN32

inline
void*
stack_align( void* sp, int alignment, size_t* stack_size )
{
    *stack_size = ( *stack_size + alignment - 1 ) & ~( alignment - 1 );
    return ((void*)(( ((qt_word_t)sp) + alignment - 1 ) & ~( alignment - 1 )));
}

#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_process_b
//
//  Process base class.
// ----------------------------------------------------------------------------

const char* const sc_process_b::kind_string = "sc_process_b";


sc_process_b::sc_process_b( const char*   nm,
			    SC_ENTRY_FUNC fn,
			    sc_module*    mod )
: sc_object( nm ),
  entry_fn( fn ),
  module( mod ),
  proc_id( simcontext()->next_proc_id() ),
  m_do_initialize( true ),
  m_is_runnable( false ),
  m_trigger_type( STATIC ),
  m_event( 0 ),
  m_event_list( 0 ),
  m_event_count( 0 ),
  m_timed_out( false )
{}

sc_process_b::~sc_process_b()
{}


void
sc_process_b::add_static_event( const sc_event& e )
{
    // check if already in static events set
    for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
	if( &e == m_static_events[i] ) {
	    return;
	}
    }
    m_static_events.push_back( &e );
    // not very nice, but it works
    sc_method_handle method_h = DCAST<sc_method_handle>( this );
    if( method_h != 0 ) {
	e.add_static( method_h );
	return;
    }
    sc_thread_handle thread_h = DCAST<sc_thread_handle>( this );
    if( thread_h != 0 ) {
	e.add_static( thread_h );
	return;
    }
    assert( false );
}

void
sc_process_b::remove_static_events()
{
    // not very nice, but it works
    sc_method_handle method_h = DCAST<sc_method_handle>( this );
    if( method_h != 0 ) {
	for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
	    m_static_events[i]->remove_static( method_h );
	}
	m_static_events.erase_all();
	return;
    }
    sc_thread_handle thread_h = DCAST<sc_thread_handle>( this );
    if( thread_h != 0 ) {
	for( int i = m_static_events.size() - 1; i >= 0; -- i ) {
	    m_static_events[i]->remove_static( thread_h );
	}
	m_static_events.erase_all();
	return;
    }
    assert( false );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_method_process
//
//  Process class for SC_METHODs.
// ----------------------------------------------------------------------------

const char* const sc_method_process::kind_string = "sc_method_process";


sc_method_process::sc_method_process( const char*   nm,
				      SC_ENTRY_FUNC fn,
				      sc_module*    mod )
: sc_process_b( nm, fn, mod )
{}

sc_method_process::~sc_method_process()
{}


void
sc_method_process::clear_trigger()
{
    switch( m_trigger_type ) {
    case STATIC: {
	return;
    }
    case EVENT: {
	m_event->remove_dynamic( this );
	m_event = 0;
	break;
    }
    case OR_LIST: {
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	break;
    }
    case AND_LIST: {
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_event_count = 0;
	break;
    }
    case TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	break;
    }
    case EVENT_TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	m_event->remove_dynamic( this );
	m_event = 0;
	break;
    }
    case OR_LIST_TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	break;
    }
    case AND_LIST_TIMEOUT: {
	m_timeout_event.cancel();
	m_timeout_event.reset();
	m_event_list->remove_dynamic( this, 0 );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_event_count = 0;
	break;
    }
    }
    m_trigger_type = STATIC;
}


bool
sc_method_process::trigger_dynamic( sc_event* e )
{
    if( m_is_runnable ) {
	return false;
    }
    m_timed_out = false;
    switch( m_trigger_type ) {
    case EVENT: {
	m_event = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST: {
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST: {
	if( -- m_event_count == 0 ) {
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case TIMEOUT: {
	m_trigger_type = STATIC;
	return true;
    }
    case EVENT_TIMEOUT: {
	if( e == m_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	    m_event->remove_dynamic( this );
	}
	m_event = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST_TIMEOUT: {
	if( e != &m_timeout_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	}
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST_TIMEOUT: {
	if( e == &m_timeout_event ) {
	    m_timed_out = true;
	    m_event_list->remove_dynamic( this, e );
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	} else if( -- m_event_count == 0 ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case STATIC: {
	// we should never get here
	assert( false );
    }
    }
    return false;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_thread_process
//
//  Process class for SC_THREADs.
// ----------------------------------------------------------------------------

const char* const sc_thread_process::kind_string = "sc_thread_process";


sc_thread_process::sc_thread_process( const char*   nm,
                                      SC_ENTRY_FUNC fn,
                                      sc_module*    mod,
				      bool          is_cthread_ )
: sc_process_b( nm, fn, mod ),
  m_is_cthread( is_cthread_ ),
  m_stack_size( SC_DEFAULT_STACK_SIZE ),
#ifndef WIN32
  m_stack( 0 ),
  m_sp( 0 )
#else
  m_fiber( (PVOID) 0 )
#endif
{
    if( m_is_cthread ) {
	do_initialize( false );
    }
}

sc_thread_process::~sc_thread_process()
{
    stack_protect( false );
#ifndef WIN32
    if( m_stack != 0 ) {
        delete[] (char*) m_stack;
    }
#else
    if( m_fiber != (PVOID) 0 ) {
	DeleteFiber( m_fiber );
    }
#endif
}


void
sc_thread_process::set_stack_size( size_t size )
{
    m_stack_size = size;
}


void
sc_thread_process::stack_protect( bool enable )
{
    // Code needs to be tested on HP-UX and disabled if it doesn't work there
    // Code still needs to be ported to WIN32

#ifndef WIN32

    static size_t pagesize;
    
    if( pagesize == 0 ) {
	pagesize = sysconf( _SC_PAGESIZE );
    }

    assert( pagesize != 0 );
    assert( m_stack_size > ( 2 * pagesize ) );

#ifdef QT_GROW_DOWN
    // Stacks grow from high address down to low address
    caddr_t redzone = caddr_t( ( ( size_t( m_stack ) + pagesize - 1 ) /
				 pagesize ) * pagesize );
#else
    // Stacks grow from low address up to high address
    caddr_t redzone = caddr_t( ( ( size_t( m_stack ) +
				   m_stack_size - pagesize ) /
				 pagesize ) * pagesize );
#endif

    int ret;

    if( enable ) {
	ret = mprotect( redzone, pagesize - 1, PROT_NONE );
    } else {
	ret = mprotect( redzone, pagesize - 1, PROT_READ | PROT_WRITE );
    }

    assert( ret == 0 );

#endif
}


void
sc_thread_process::prepare_for_simulation()
{
#ifndef WIN32
    m_stack = new char[m_stack_size];
    assert( m_stack != 0 );
    void* sto = stack_align( m_stack, QT_STKALIGN, &m_stack_size );
    m_sp = QT_SP( sto, m_stack_size - QT_STKALIGN );
    m_sp = QT_ARGS( m_sp, this, this, 0, sc_thread_only );
#else
    // forward declare fiber function
    void WINAPI sc_thread_fiber_func( PVOID );
    m_fiber = CreateFiber( m_stack_size, sc_thread_fiber_func, this );
#endif
    stack_protect( true );
}


#ifndef WIN32

void
sc_thread_only( void*, void* p, qt_userf_t* )
{
    sc_thread_handle thread_h = RCAST<sc_thread_handle>( p );

    try {
	thread_h->execute();
    }
    catch( const sc_exception& ex ) {
	cout << "\n" << ex.str() << "\n";
	thread_h->simcontext()->set_error();
    }

    // if control reaches this point, then the process has gone to heaven
    qt_t* next_qt = thread_h->simcontext()->next_thread_qt();
    QT_ABORT( sc_thread_aborthelp, thread_h, 0, next_qt );
}

void*
sc_thread_aborthelp( qt_t*, void* old, void* )
{
    sc_thread_handle thread_h = RCAST<sc_thread_handle>( old );
    thread_h->remove_static_events();
    thread_h->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;  // mark defunct
    return 0;
}

void*
sc_thread_yieldhelp( qt_t* sp, void* old, void* )
{
    RCAST<sc_thread_handle>( old )->m_sp = sp;
    return sp;
}

#else

void
WINAPI
sc_thread_fiber_func( PVOID p )
{
    sc_thread_handle thread_h = RCAST<sc_thread_handle>( p );

    try {
        thread_h->execute();
    }
    catch( const sc_exception& ex ) {
	cout << "\n" << ex.str() << "\n";
	thread_h->simcontext()->set_error();
    }

    // if control reaches this point, then the process has gone to heaven
    thread_h->remove_static_events();
    thread_h->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;  // mark defunct
    PVOID next_fiber = thread_h->simcontext()->next_thread_fiber();
    SwitchToFiber( next_fiber );
}

#endif


bool
sc_thread_process::trigger_dynamic( sc_event* e )
{
    if( m_is_runnable ) {
	return false;
    }
    m_timed_out = false;
    switch( m_trigger_type ) {
    case EVENT: {
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST: {
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST: {
	if( -- m_event_count == 0 ) {
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case TIMEOUT: {
	m_trigger_type = STATIC;
	return true;
    }
    case EVENT_TIMEOUT: {
	if( e == m_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	    m_event->remove_dynamic( this );
	}
	m_event = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case OR_LIST_TIMEOUT: {
	if( e != &m_timeout_event ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	} else {
	    m_timed_out = true;
	}
	m_event_list->remove_dynamic( this, e );
	m_event_list->auto_delete();
	m_event_list = 0;
	m_trigger_type = STATIC;
	return true;
    }
    case AND_LIST_TIMEOUT: {
	if( e == &m_timeout_event ) {
	    m_timed_out = true;
	    m_event_list->remove_dynamic( this, e );
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	} else if( -- m_event_count == 0 ) {
	    m_timeout_event.cancel();
	    m_timeout_event.reset();
	    // no need to remove_dynamic
	    m_event_list->auto_delete();
	    m_event_list = 0;
	    m_trigger_type = STATIC;
	    return true;
	}
	return false;
    }
    case STATIC: {
	// we should never get here
	assert( false );
    }
    }
    return false;
}


void
sc_set_stack_size( sc_thread_handle thread_h, size_t size )
{
    thread_h->set_stack_size( size );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_cthread_process
//
//  Process class for SC_CTHREADs.
// ----------------------------------------------------------------------------

const char* const sc_cthread_process::kind_string = "sc_cthread_process";


sc_cthread_process::sc_cthread_process( const char*   nm,
					SC_ENTRY_FUNC fn,
					sc_module*    mod )
: sc_thread_process( nm, fn, mod, true ),
  m_wait_state( UNKNOWN ),
  m_wait_cycles( 0 ),
  m_exception_level( -1 ),
  m_watchlists( SC_MAX_WATCH_LEVEL )
{
    __reset_watching();
    for( int i = 0; i < SC_MAX_WATCH_LEVEL; ++ i ) {
        m_watchlists[i] = new sc_plist<sc_lambda_ptr*>;
    }
}

sc_cthread_process::~sc_cthread_process()
{
    for( int i = 0; i < SC_MAX_WATCH_LEVEL; ++ i ) {
        sc_plist<sc_lambda_ptr*>::iterator it( m_watchlists[i] );
        while( ! it.empty() ) {
            delete *it;  // this was created with `new sc_lambda_ptr'
            it.remove(); // and go on to next
        }
        delete m_watchlists[i];
    }
}


void
sc_cthread_process::prepare_for_simulation()
{
#ifndef WIN32
    m_stack = new char[m_stack_size];
    assert( m_stack != 0 );
    void* sto = stack_align( m_stack, QT_STKALIGN, &m_stack_size );
    m_sp = QT_SP( sto, m_stack_size - QT_STKALIGN );
    m_sp = QT_ARGS( m_sp, this, this, 0, sc_cthread_only );
#else
    void WINAPI sc_cthread_fiber_func( LPVOID );
    m_fiber = CreateFiber( m_stack_size, sc_cthread_fiber_func, this );
#endif
    stack_protect( true );
}


bool
sc_cthread_process::ready_to_run()
{
    bool exception = eval_watchlist();
    if( exception ) {
	return true;
    }

    bool ready;
    switch( m_wait_state ) {
    case CLOCK:
        ready = ( -- m_wait_cycles == 0 );
        break;
    case LAMBDA:
        ready = m_wait_lambda->eval();
        break;
    default:
        ready = true;
        break;
    }
    return ready;
}


#ifndef WIN32

void
sc_cthread_only( void*, void* p, qt_userf_t* )
{
    sc_cthread_handle cthread_h = RCAST<sc_cthread_handle>( p );

    while( true ) {
        try {
            cthread_h->execute();
        }
        catch( sc_user ) {
	    assert( cthread_h->m_watch_level == 0 );
	    continue;
        }
        catch( sc_halt ) {
            cout << "Terminating process " << cthread_h->name() << "\n";
        }
	catch( const sc_exception& ex ) {
	    cout << "\n" << ex.str() << "\n";
	    cthread_h->simcontext()->set_error();
	}
	break;
    }

    // if control reaches this point, then the process has gone to heaven
    qt_t* next_qt = cthread_h->simcontext()->next_thread_qt();
    QT_ABORT( sc_cthread_aborthelp, cthread_h, 0, next_qt );
}

void*
sc_cthread_aborthelp( qt_t*, void* old, void* )
{
    sc_cthread_handle cthread_h = RCAST<sc_cthread_handle>( old );
    cthread_h->remove_static_events();
    cthread_h->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;  // mark defunct
    return 0;
}

#else

void WINAPI
sc_cthread_fiber_func( LPVOID p )
{
    sc_cthread_handle cthread_h = RCAST<sc_cthread_handle>( p );

    while( true ) {
        try {
            cthread_h->execute();
        }
        catch( sc_user ) {
            assert( cthread_h->m_watch_level == 0 );
	    continue;
        }
        catch( sc_halt ) {
            cout << "Terminating process " << cthread_h->name() << "\n";
        }
	catch( const sc_exception& ex ) {
	    cout << "\n" << ex.str() << "\n";
	    cthread_h->simcontext()->set_error();
	}
	break;
    }

    // if control reaches this point, then the process has gone to heaven
    cthread_h->remove_static_events();
    cthread_h->entry_fn = SC_DEFUNCT_PROCESS_FUNCTION;  // mark defunct
    PVOID next_fiber = cthread_h->simcontext()->next_thread_fiber();
    SwitchToFiber( next_fiber );
}

#endif


bool
sc_cthread_process::eval_watchlist()
{
    int wlevel = __watch_level();
    for( int i = 0; i <= wlevel; ++ i ) {
        sc_plist<sc_lambda_ptr*>& li = *(m_watchlists[i]);
        sc_plist<sc_lambda_ptr*>::iterator wit( li );
        while( ! wit.empty() ) {
            if( (**wit)->eval() ) {
                m_exception_level = i;
                if( i == 0 ) /* never remove global watching */
                    i = 1;
                for( ; i <= wlevel; ++ i ) {
                    sc_plist<sc_lambda_ptr*>& l = *(m_watchlists[i]);
                    sc_plist<sc_lambda_ptr*>::iterator it( l );
                    while( ! it.empty() ) {
                        delete *it;
                        it.remove();
                    }
                }
                return true;
            }
            wit ++;
        }
    }
    m_exception_level = -1;
    return false;
}

bool
sc_cthread_process::eval_watchlist_curr_level()
{
    int wlevel = m_watch_level;
    assert( wlevel > 0 );
    
    sc_plist<sc_lambda_ptr*>& li = *m_watchlists[wlevel];
    sc_plist<sc_lambda_ptr*>::iterator wit( li );
    while( ! wit.empty() ) {
        if( (**wit)->eval() ) {
            m_exception_level = wlevel;
            wit.reset( li );
            while( ! wit.empty() ) {
                delete *wit;
                wit.remove();
            }
            return true;
        }
        wit ++;
    }
    m_exception_level = -1;
    return false;
}


// Taf!
