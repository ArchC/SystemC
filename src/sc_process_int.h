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

    sc_process_int.h -- Process internals. THIS INCLUDE FILE IS FOR
    SYSTEMC LIBRARY ONLY.  DO NOT EXPORT FOR USER'S USE.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_PROCESS_INT_H
#define SC_PROCESS_INT_H




#ifndef WIN32
/***************************************************************************
    These functions have "C" linkage because QT's assembly code is such.
***************************************************************************/
#include "qt/qt.h"
extern "C" {
    void  sc_sync_process_only(void*, void*, qt_userf_t*);
    void* sc_sync_process_yieldhelp(qt_t*, void*, void*);
    void* sc_sync_process_aborthelp(qt_t*, void*, void*);

    void  sc_aproc_process_only(void*, void*, qt_userf_t*);
    void* sc_aproc_process_yieldhelp(qt_t*, void*, void*);
    void* sc_aproc_process_aborthelp(qt_t*, void*, void*);
};
#else
#include <Windows.h>
#pragma hdrstop
#include "sc_cmnhdr.h"
typedef unsigned long qt_word_t;
#endif
#include "sc_polarity.h"
#include "sc_object.h"
#include "sc_vector.h"
#include "sc_list.h"
#include "sc_lambda.h"
#include "sc_module.h"

class sc_signal_array_base;
class sc_module;

inline void*
stack_align( void* sp, int alignment, size_t* stack_size )
{
    *stack_size = (*stack_size + alignment - 1) & ~(alignment - 1);
    return ((void*)((((qt_word_t)sp) + alignment - 1) & ~(alignment - 1)));
}

/* Common stuff for all kinds of processes */
class sc_process_b : public sc_object {
public:
    SC_ENTRY_FUNC entry_fn;
    sc_module*   module;
    int          proc_id;
    const char*  file;
    int          lineno;
    sc_process_b(const char* nm,
                 SC_ENTRY_FUNC fn,
                 sc_module* mod)
        : sc_object(nm),
          entry_fn(fn),
          module(mod)
    {
        /*EMPTY*/
    }
    ~sc_process_b()
    {
        /*EMPTY*/
    }
    void execute()
    {
#ifndef SC_USE_MEMBER_FUNC_PTR
        (*entry_fn)(module);
#else
        (module->*entry_fn)(); 
#endif
    }
};



/***************************************************************************
    Definition of the "sc_sync_process" class.
***************************************************************************/
class sc_sync_process : public sc_process_b {
    friend class sc_clock_edge;
    friend class sc_module;
    friend class sc_sync;
    friend class sc_simcontext;
    friend class sc_clock_helpers;
    friend class sc_port_manager;

private:
    enum sc_wait_state_t { WAIT_CLOCK, WAIT_LAMBDA, WAIT_UNKNOWN };
    typedef sc_pvector< sc_plist<sc_lambda_ptr*>* > dwatchlists_t;

    sc_polarity      polarity;   /* used when process not yet bound to real clock edge */
#ifdef WIN32
    public:
#endif
    sc_clock_edge*   clock_edge;
    size_t           stack_size;
#ifndef WIN32
    void*            stack;
    qt_t*            sp;
#else
    PVOID            pFiber;
#endif

    sc_wait_state_t  wait_state;
    int              wait_cycles;
    sc_lambda_ptr    wait_lambda;
    int              exception_level;
#ifdef WIN32
    public:
#endif
    int              watch_level;
    private:
    dwatchlists_t    dwatchlists;

    sc_sync_process* next_handle;
    sc_sync_process* prev_handle;

    sc_sync_process(const char* nm,
                    SC_ENTRY_FUNC fn,
                    sc_module* mod,
                    sc_clock_edge* edge);
    ~sc_sync_process();

    void set_stack_size(size_t size);
    void prepare_for_simulation();
    
#ifndef WIN32
    qt_t* next_qt();
#else
    public:
    PVOID next_fiber();
    typedef void *(qt_userf_t)(void *pu);
    private:
#endif
    bool eval_watchlist();
    bool ready_to_wakeup();
    bool eval_watchlist_curr_level();

    friend void* sc_sync_process_yieldhelp(qt_t*, void*, void*);
    friend void* sc_sync_process_aborthelp(qt_t*, void*, void*);
    friend void  sc_sync_process_only(void*, void*, qt_userf_t*);

    void __reset_watching()    { watch_level = 0; }
    void __open_watching()     { watch_level++; }
    void __close_watching()    { watch_level--; assert(watch_level >= 0); }
    int  __watch_level() const { return watch_level; }

    friend void halt(sc_simcontext*);
    friend void wait(sc_simcontext*);
    friend void wait(int, sc_simcontext*);
    friend void wait_until(const sc_lambda_ptr&, sc_simcontext*);
    friend void watching_before_simulation(const sc_lambda_ptr&, sc_simcontext*);
    friend void watching_during_simulation(const sc_lambda_ptr&, sc_simcontext*);
    friend void __reset_watching( sc_sync_process_handle );
    friend void __open_watching( sc_sync_process_handle );
    friend void __close_watching( sc_sync_process_handle );
    friend int  __watch_level( sc_sync_process_handle );
    friend void __watching_first( sc_sync_process_handle );
    friend void __sanitycheck_watchlists( sc_sync_process_handle );
    friend void sc_set_stack_size( sc_sync_process_handle, size_t );
};



/***************************************************************************
    Definition of the "sc_async_process" class.
***************************************************************************/
class sc_async_process : public sc_process_b {
    friend class sc_module;
    friend class sc_async;
    friend class sc_simcontext;
    friend class sc_port_manager;
    friend struct sc_port_info;
    friend struct sc_async_aproc_process_table;
    friend class sc_signal_base;
    friend class sc_signal_edgy;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;

private:
    bool              m_is_in_updateq;

    sc_async_process* next_handle;

    sc_async_process(const char* nm,
                     SC_ENTRY_FUNC fn,
                     sc_module* mod);
    ~sc_async_process();

    void set_in_updateq(bool v) { m_is_in_updateq = v; }
    bool is_in_updateq() const { return m_is_in_updateq; }

    void sensitive( const sc_signal_base& s );
    void sensitive( const sc_signal_array_base& sa );
    void sensitive( const sc_signal_bool_vector& sbv );

    void sensitive( const sc_signal_edgy& s );
    void sensitive_pos( const sc_signal_edgy& s );
    void sensitive_neg( const sc_signal_edgy& s );
};



/***************************************************************************
    Definition of the "sc_aproc_process" class.
***************************************************************************/
class sc_aproc_process : public sc_process_b {
    friend class sc_module;
    friend class sc_aproc;
    friend class sc_simcontext;
    friend class sc_port_manager;
    friend struct sc_port_info;
    friend struct sc_async_aproc_process_table;
    friend class sc_signal_base;
    friend class sc_signal_edgy;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;

private:
    bool              m_is_in_updateq;

    size_t            stack_size;
#ifndef WIN32
    void*             stack;
    qt_t*             sp;
#else
    PVOID             pFiber;
#endif 

    sc_pvector<const sc_signal_base*> trigger_signals;
    sc_pvector<const sc_signal_edgy*> trigger_signals_edgy_neg;

    sc_aproc_process* next_handle;

    sc_aproc_process(const char* nm,
                     SC_ENTRY_FUNC fn,
                     sc_module* mod);
    ~sc_aproc_process();

    void set_stack_size(size_t size);
    void prepare_for_simulation();
#ifdef WIN32
public:
#endif
    void add_trigger_signal(const sc_signal_base&);
    void add_trigger_signal(const sc_signal_array_base&);
    void add_trigger_signal(const sc_signal_bool_vector&);
    void add_trigger_signal_pos(const sc_signal_edgy& s);
    void add_trigger_signal_neg(const sc_signal_edgy& s);
    void remove_from_trigger_signals();
    void remove_from_trigger_signals_neg();

#ifndef WIN32
    qt_t* next_aproc_qt();
    friend void sc_aproc_process_only(void*, void*, qt_userf_t*);
    friend void* sc_aproc_process_yieldhelp(qt_t*, void*, void*);
    friend void* sc_aproc_process_aborthelp(qt_t*, void*, void*);
#else
    PVOID next_aproc_fiber();
    static void WINAPI fiber_func(PVOID);
#endif
    
    void set_in_updateq(bool v) { m_is_in_updateq = v; }
    bool is_in_updateq() const { return m_is_in_updateq; }

    void sensitive( const sc_signal_base& s );
    void sensitive( const sc_signal_array_base& s );
    void sensitive( const sc_signal_bool_vector& s );
    void sensitive_pos( const sc_signal_edgy& s );
    void sensitive_neg( const sc_signal_edgy& s );
    void sensitive( const sc_signal_edgy& s );

    friend void wait( sc_simcontext* );
    friend void sc_set_stack_size( sc_aproc_process_handle aproc_h,
                                   size_t size );
};


#endif
