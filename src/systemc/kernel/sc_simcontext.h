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

  sc_simcontext.h -- Definition of the simulation context class.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_SIMCONTEXT_H
#define SC_SIMCONTEXT_H


#include "systemc/kernel/sc_cmnhdr.h"
#include "systemc/kernel/sc_runnable.h"
#include "systemc/kernel/sc_time.h"
#include "systemc/utils/sc_hash.h"
#include "systemc/utils/sc_vector.h"
#include "systemc/utils/sc_pq.h"


#ifndef WIN32

struct qt_t;
extern "C" {
    void* sc_simcontext_yieldhelp( qt_t* sp, void* simc, void* );
}

#endif


// forward declarations
class sc_event;
class sc_event_timed;
class sc_lambda_ptr;
class sc_module;
class sc_module_name;
class sc_module_registry;
class sc_name_gen;
class sc_object;
class sc_object_manager;
class sc_port_registry;
class sc_prim_channel_registry;
class sc_process_table;
class sc_signal_bool_deval;
class sc_trace_file;


enum sc_curr_proc_kind
{
    SC_NO_PROC_,
    SC_METHOD_PROC_,
    SC_THREAD_PROC_,
    SC_CTHREAD_PROC_
};


struct sc_curr_proc_info
{
    sc_process_b* process_handle;
    sc_curr_proc_kind kind;
    sc_curr_proc_info() : process_handle( 0 ), kind( SC_NO_PROC_ ) {}
};

typedef const sc_curr_proc_info* sc_curr_proc_handle;


// simulation status codes

const int SC_SIM_OK        = 0;
const int SC_SIM_ERROR     = 1;
const int SC_SIM_USER_STOP = 2;


// ----------------------------------------------------------------------------
//  CLASS : sc_simcontext
//
//  The simulation context.
// ----------------------------------------------------------------------------

class sc_simcontext
{
    friend class sc_event;
    friend class sc_module;
    friend class sc_object;
    friend class sc_time;

    void init();
    void clean();

public:

    sc_simcontext();
    ~sc_simcontext();

    void initialize( bool = false );
    void cycle( const sc_time& );
    void simulate( const sc_time& duration );
    void stop();
    void reset();

    int sim_status() const;

    sc_object_manager* get_object_manager();

    void hierarchy_push( sc_module* );
    sc_module* hierarchy_pop();
    sc_module* hierarchy_curr() const;
    sc_object* first_object();
    sc_object* next_object();
    sc_object* find_object( const char* name );

    sc_module_registry* get_module_registry();
    sc_port_registry* get_port_registry();
    sc_prim_channel_registry* get_prim_channel_registry();

    // to generate unique names for objects in an MT-Safe way
    const char* gen_unique_name( const char* basename_ );

    sc_method_handle register_method_process( const char* name,
					      SC_ENTRY_FUNC fn,
					      sc_module* );
    sc_thread_handle register_thread_process( const char* name,
					      SC_ENTRY_FUNC fn,
					      sc_module* );
    sc_cthread_handle register_cthread_process( const char* name,
						SC_ENTRY_FUNC fn,
						sc_module* );

    sc_curr_proc_handle get_curr_proc_info();
    void set_curr_proc( sc_method_handle );
    void set_curr_proc( sc_thread_handle );
    void reset_curr_proc();

    int next_proc_id();

    void add_trace_file( sc_trace_file* );

    friend void    sc_set_time_resolution( double, sc_time_unit );
    friend sc_time sc_get_time_resolution();
    friend void    sc_set_default_time_unit( double, sc_time_unit );
    friend sc_time sc_get_default_time_unit();

    const sc_time& time_stamp() const;

    uint64 delta_count() const;
    bool is_running() const;
    bool update_phase() const;
    bool get_error();
    void set_error();

#ifndef WIN32
    qt_t* next_thread_qt();
    friend void* sc_simcontext_yieldhelp( qt_t* sp, void* simc, void* );
#else
    PVOID next_thread_fiber();
#endif

    const sc_pvector<sc_object*>& get_child_objects() const;

private:

    void add_child_object( sc_object* );
    void remove_child_object( sc_object* );

    void crunch();

    const sc_time next_time();

    int add_delta_event( sc_event* );
    void remove_delta_event( sc_event* );
    void add_timed_event( sc_event_timed* );

    void trace_cycle( bool delta_cycle );

    void push_runnable_method( sc_method_handle );
    void push_runnable_thread( sc_thread_handle );

    sc_method_handle pop_runnable_method();
    sc_thread_handle pop_runnable_thread();

    friend void watching( const sc_lambda_ptr&, sc_simcontext* );
    friend void watching( const sc_signal_bool_deval&, sc_simcontext* );

private:

    sc_object_manager*         m_object_manager;

    sc_module_registry*        m_module_registry;
    sc_port_registry*          m_port_registry;
    sc_prim_channel_registry*  m_prim_channel_registry;

    sc_name_gen*               m_name_gen;

    sc_process_table*          m_process_table;
    sc_curr_proc_info          m_curr_proc_info;
    int                        m_next_proc_id;

    sc_pvector<sc_object*>     m_child_objects;

    sc_pvector<sc_event*>      m_delta_events;
    sc_ppq<sc_event_timed*>*   m_timed_events;

    sc_pvector<sc_trace_file*> m_trace_files;
    bool                       m_something_to_trace;

    sc_runnable*               m_runnable;

    sc_time_params*            m_time_params;
    sc_time                    m_curr_time;

    uint64                     m_delta_count;
    bool                       m_forced_stop;
    bool                       m_ready_to_simulate;
    bool                       m_update_phase;
    bool                       m_error;

#ifndef WIN32
    qt_t*                      m_sp;     // the simcontext's stack pointer
#else
    PVOID                      m_fiber;  // the simcontext's fiber
#endif

    void (*m_watching_fn)( const sc_lambda_ptr&, sc_simcontext* );

private:

    // disabled
    sc_simcontext( const sc_simcontext& );
    sc_simcontext& operator = ( const sc_simcontext& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_simcontext::cycle( const sc_time& t )
{
    m_runnable->toggle();
    crunch();
    trace_cycle( /* delta cycle? */ false );
    m_curr_time += t;
}


inline
int
sc_simcontext::sim_status() const
{
    if( m_error ) {
        return SC_SIM_ERROR;
    }
    if( m_forced_stop ) {
        return SC_SIM_USER_STOP;
    }
    return SC_SIM_OK;
}


inline
sc_object_manager*
sc_simcontext::get_object_manager()
{
    return m_object_manager;
}

inline
sc_module_registry*
sc_simcontext::get_module_registry()
{
    return m_module_registry;
}

inline
sc_port_registry*
sc_simcontext::get_port_registry()
{
    return m_port_registry;
}

inline
sc_prim_channel_registry*
sc_simcontext::get_prim_channel_registry()
{
    return m_prim_channel_registry;
}


inline
sc_curr_proc_handle
sc_simcontext::get_curr_proc_info()
{
    return &m_curr_proc_info;
}


inline
int
sc_simcontext::next_proc_id()
{
    return ( ++ m_next_proc_id );
}


inline
const sc_time&
sc_simcontext::time_stamp() const
{
    return m_curr_time;
}


inline
uint64
sc_simcontext::delta_count() const
{
    return m_delta_count;
}

inline
bool
sc_simcontext::is_running() const
{
    return m_ready_to_simulate;
}

inline
bool
sc_simcontext::update_phase() const
{
    return m_update_phase;
}

inline
bool
sc_simcontext::get_error()
{
    return m_error;
}

inline
void
sc_simcontext::set_error()
{
    m_error = true;
}


inline
int
sc_simcontext::add_delta_event( sc_event* e )
{
    m_delta_events.push_back( e );
    return ( m_delta_events.size() - 1 );
}

inline
void
sc_simcontext::add_timed_event( sc_event_timed* et )
{
    m_timed_events->insert( et );
}


// ----------------------------------------------------------------------------

extern sc_simcontext* sc_get_curr_simcontext();

inline
sc_process_b*
sc_get_curr_process_handle()
{
    return sc_get_curr_simcontext()->get_curr_proc_info()->process_handle;
}


// Generates unique names within each module.
extern
const char*
sc_gen_unique_name( const char* basename_ );


// Set the random seed for controlled randomization -- not yet implemented
extern
void
sc_set_random_seed( unsigned int seed_ );


extern void sc_start( const sc_time& duration );
extern void sc_stop();

extern void sc_initialize();
extern void sc_cycle( const sc_time& duration );

extern const sc_time& sc_time_stamp();
extern double sc_simulation_time();  // in default time units


inline
void
sc_start( double duration, sc_time_unit time_unit )
{
    sc_start( sc_time( duration, time_unit ) );
}

// for backward compatibility with 1.0
inline
void
sc_start( double duration = -1 )  // in default time units
{
    if( duration == -1 ) {
	// simulate forever
	sc_start( SC_ZERO_TIME );
    } else {
	sc_start( sc_time( duration, true ) );
    }
}


inline
void
sc_cycle( double duration, sc_time_unit time_unit )
{
    sc_cycle( sc_time( duration, time_unit ) );
}

// for backward compatibility with 1.0
inline
void
sc_cycle( double duration )  // in default time units
{
    sc_cycle( sc_time( duration, true ) );
}


// ----------------------------------------------------------------------------
//  MACRO : sc_assert(expr)
//
//  Like assert(), but additionally prints the current process name
//  and simulation time, if the simulation is running.
// ----------------------------------------------------------------------------

extern void sc_assert_fail( const char*, const char*, int );

#ifdef NDEBUG

#define sc_assert(expr)                                                       \
    ((void) 0)

#else

#define sc_assert(expr)                                                       \
    ((void) ((expr) ? 0 : (sc_assert_fail( #expr, __FILE__, __LINE__ ), 0)))

#endif


#endif
