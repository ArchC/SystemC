/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_runnable.h --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification: 

 *****************************************************************************/

#ifndef SC_RUNNABLE_H
#define SC_RUNNABLE_H


#include "systemc/kernel/sc_process.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_q<T>
//
//  Rudimentary queue. T must be a pointer type.
//  Should be used only for ready-to-run process queues.
//  BEWARE: *there are no out-of-bounds checks* (performance reasons)
// ----------------------------------------------------------------------------

template <class T>
class sc_q
{
public:

    sc_q()
	: m_vec( 0 ), m_end( 0 ), m_wp( 0 ), m_rp( 0 )
	{}

    ~sc_q()
	{ if( m_vec ) { delete[] m_vec; } }

    void reset()
	{ m_wp = m_rp = m_vec; }

    void alloc( int sz )
	{ m_vec = new T[sz]; m_end = m_vec + sz; reset(); }

    int size() const
	{ return m_wp - m_rp; }

    void push( T t )
	{ *m_wp ++ = t; }

    T pop()
	{ return *m_rp ++; }

private:

    T* m_vec;  // data
    T* m_end;  // end of allocated space (one beyond last)
    T* m_wp;   // write pointer
    T* m_rp;   // read pointer

private:

    // disabled
    sc_q( const sc_q<T>& );
    sc_q<T>& operator = ( const sc_q<T>& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_runnable
//
//  Class that manages the ready-to-run queues.
// ----------------------------------------------------------------------------

class sc_runnable
{
    void push_done();
    void set();

public:

    sc_runnable();
    ~sc_runnable();

    void alloc( int num_methods, int num_threads );

    void init();
    void toggle();

    void push_method( sc_method_handle );
    void push_thread( sc_thread_handle );

    bool push_empty() const;

    sc_method_handle pop_method();
    sc_thread_handle pop_thread();

private:

    sc_q<sc_method_handle>  m_methods[2];
    sc_q<sc_thread_handle>  m_threads[2];

    int                     m_push;

    sc_q<sc_method_handle>* m_methods_push;
    sc_q<sc_method_handle>* m_methods_pop;
    sc_q<sc_thread_handle>* m_threads_push;
    sc_q<sc_thread_handle>* m_threads_pop;

private:

    // disabled
    sc_runnable( const sc_runnable& );
    sc_runnable& operator = ( const sc_runnable& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_runnable::push_done()
{
    m_methods_push->push( 0 );
    m_threads_push->push( 0 );
}

inline
void
sc_runnable::set()
{
    int push_index = m_push;
    int pop_index  = 1 - push_index;

    m_methods_push = &m_methods[push_index];
    m_methods_pop  = &m_methods[pop_index];
    m_threads_push = &m_threads[push_index];
    m_threads_pop  = &m_threads[pop_index];

    m_methods_push->reset();
    m_threads_push->reset();
}


inline
void
sc_runnable::init()
{
    m_push = 0;
    set();
}

inline
void
sc_runnable::toggle()
{
    push_done();
    m_push = 1 - m_push;
    set();
}


inline
void
sc_runnable::push_method( sc_method_handle method_h )
{
    m_methods_push->push( method_h );
}

inline
void
sc_runnable::push_thread( sc_thread_handle thread_h )
{
    m_threads_push->push( thread_h );
}


inline
bool
sc_runnable::push_empty() const
{
    return ( m_methods_push->size() == 0 && m_threads_push->size() == 0 );
}


inline
sc_method_handle
sc_runnable::pop_method()
{
    return m_methods_pop->pop();
}

inline
sc_thread_handle
sc_runnable::pop_thread()
{
    return m_threads_pop->pop();
}


#endif

// Taf!
