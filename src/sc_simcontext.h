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

    sc_simcontext.h -- definition of the simulation context class.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_SIMCONTEXT_H
#define SC_SIMCONTEXT_H

#include "sc_list.h"
#include "sc_vector.h"
#include "sc_process.h"

#ifndef WIN32
struct qt_t;
extern "C" {
    void* sc_simcontext_yieldhelp( qt_t* sp, void* simc, void* );
}
#else
#include "sc_cmnhdr.h"
#include <Windows.h>
struct qt_t;
#endif

/* FORWARD DECLARATIONS OF CLASS NAMES */
class sc_port_manager;
class sc_module;
class sc_trace_file;
class sc_array_concat_manager;
class sc_object_manager;
class sc_object;
template<class T> class sc_ppq;
template<class K, class C> class sc_phash;
class sc_lambda_ptr;
class sc_signal_edgy_deval;
struct sc_polarity;
class sc_module_name;

enum sc_curr_proc_kind {
    SC_CURR_PROC_NONE,
    SC_CURR_PROC_ASYNC,
    SC_CURR_PROC_APROC,
    SC_CURR_PROC_SYNC
};

struct sc_curr_proc_info {
    sc_curr_proc_info()
        : process_handle( 0 ) {} // to get rid of UMRs in purify

    sc_process_b* process_handle;
    sc_curr_proc_kind kind;
};

struct sc_async_aproc_process_table;

class sc_simcontext {
    friend class sc_clock;
    friend class sc_clock_edge;
    friend class sc_array_base;
    friend class sc_signal_base;
    friend class sc_signal_edgy;
    friend class sc_async;
    friend class sc_aproc;
    friend class sc_module;

public:
    typedef int (*callback_fn)( const sc_clock_edge& edge, void* arg );

public:
    sc_simcontext();
    ~sc_simcontext();

    void hierarchy_push(sc_module* mdl);
    sc_module* hierarchy_pop();
    sc_module* hierarchy_curr() const;
    sc_object* first_object();
    sc_object* next_object();
    sc_object* find_object(const char* name);

    bool simulate( double duration, callback_fn callback = 0, void* arg = 0 );
    void stop();
    void reset();

    double time_stamp() const;

    void add_trace_file(sc_trace_file* tf);
    void trace_cycle(bool delta_cycle);

    /* prepare for simulation - should be called after all objects have been
       instantiated */
    void initialize();

    void submit_update( sc_signal_base* sig )
    {
        signals_to_update[++lastof_signals_to_update] = sig;
    }

    void add_signal( sc_signal_base* sig );

    void initial_crunch();
    void crunch();
    void sc_cycle( double t )
    {
        crunch();
	trace_cycle( /* is not delta cycle */ false );
        curr_time += t;
    }

    /* THIS PART OF THE INTERFACE SHOULD BE VISIBLE ONLY TO THE
       IMPLEMENTATION.  */
    const sc_curr_proc_info* get_curr_proc_info()
    {
        return &curr_proc_info;
    }
    void reset_curr_proc();
    void set_curr_proc( sc_sync_process_handle handle );
    void set_curr_proc( sc_async_process_handle handle );
    void set_curr_proc( sc_aproc_process_handle handle );

    bool is_running() const { return ready_to_simulate; }

    /* Functions related to aprocs */
#ifdef WIN32
    PVOID next_aproc_fiber();
#else
    qt_t* next_aproc_qt();
#endif

    void submit_clock_edge( sc_clock_edge* edge );

    sc_array_concat_manager* get_concat_manager() { return concat_manager; }
    sc_object_manager*       get_object_manager() { return object_manager; }
    sc_port_manager*         get_port_manager()   { return port_manager; }

private:
    sc_plist<sc_clock_edge*>::handle_t add_clock_edge(sc_clock_edge* ce);

    double simulate_forever( callback_fn callback, void* arg );
    double simulate_forever();
    double simulate_until( double until, callback_fn callback, void* arg );
    double simulate_until( double until );

#ifndef WIN32
    friend void* sc_simcontext_yieldhelp( qt_t* sp, void* simc, void* );
#endif

public:
    sc_async_process_handle register_sc_async_process(const char* name,
                                                      SC_ENTRY_FUNC fn,
                                                      sc_module*);
    sc_aproc_process_handle register_sc_aproc_process(const char* name,
                                                      SC_ENTRY_FUNC fn,
                                                      sc_module*);
    sc_sync_process_handle  register_sc_sync_process( const char* name,
                                                      SC_ENTRY_FUNC fn,
                                                      sc_module*,
                                                      sc_clock_edge& clk_edge );
    /* This version is used to late-bind clock edges */
    sc_sync_process_handle  register_sc_sync_process( const char* name,
                                                      SC_ENTRY_FUNC fn,
                                                      sc_module*,
                                                      const sc_polarity& polarity );

private:
    sc_array_concat_manager* concat_manager;
    sc_object_manager*       object_manager;
    sc_port_manager*         port_manager;

    sc_ppq<sc_clock_edge*>* prioq;     // priority queue for ordering clocks
    sc_plist<sc_clock_edge*> clock_edge_list;

    sc_curr_proc_info  curr_proc_info;

    sc_clock_edge** clock_edges_async_arrays[2];
    int curr_clock_edges_array;

    sc_clock_edge** clock_edges_async;
    int lastof_clock_edges_async;

    sc_clock_edge** clock_edges_to_advance;
    int lastof_clock_edges_to_advance;

    sc_pvector<sc_trace_file*> trace_files;

    sc_signal_base** signals_to_update_arrays[2];
    int curr_array;
    int update_array0_alloc; /* number of elements temporarily allocated
                                for signals_to_update_array[0] */

    /* The pointer `signals_to_update' can point to various places:
       signals_to_update_arrays[0] or signals_to_update_arrays[1]
       signals_to_update member of any sc_clock_edge.
       
       By switching this pointer to point to sc_clock_edge's
       signals_to_update member, we have an efficient way to put
       signals on the update queue.  See submit_update(), above. */
    sc_signal_base** signals_to_update;
    int lastof_signals_to_update;

    sc_async_process_handle* asyncs_to_execute;
    int                      lastof_asyncs_to_execute;
    sc_aproc_process_handle* aprocs_to_execute;
    int                      lastof_aprocs_to_execute;

    sc_async_aproc_process_table* aa_process_table;

#ifndef WIN32
    qt_t*      sp;  /* the simulation context's own stack ptr */
#else
    PVOID pFiber;   /* simulation context's own fiber */
#endif

    int total_number_of_signals;
    int total_number_of_asyncs;
    int total_number_of_aprocs;

    int next_aproc_index;

    double curr_time;
    sc_clock_edge* curr_edge;
    bool forced_stop;

    bool error_occurred;
    bool ready_to_simulate;

    void (*watching_fn)(const sc_lambda_ptr&, sc_simcontext*);

    friend void watching(const sc_lambda_ptr&, sc_simcontext*);
    friend void watching(const sc_signal_edgy_deval&, sc_simcontext*);
    friend void sc_start(sc_clock& clk, double duration);
};

extern sc_simcontext* sc_get_curr_simcontext();

inline sc_process_b* sc_get_curr_process_handle()
{
    return sc_get_curr_simcontext()->get_curr_proc_info()->process_handle;
}

extern double sc_simulation_time();
extern double sc_time_stamp();

inline void sc_cycle( double t )
{
    sc_get_curr_simcontext()->sc_cycle(t);
}

extern void sc_initialize();

typedef int (*sc_callback_fn)(const sc_clock_edge&, void*);
extern void sc_start(double duration=-1.0, sc_callback_fn cb = 0, void* ua = 0);
extern void sc_start(sc_clock& clk, double duration);
extern void sc_stop();

#endif
