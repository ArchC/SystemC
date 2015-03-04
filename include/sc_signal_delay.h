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

    sc_signal_delay.h - Allows for delayed assignments to signals (in
    sequential processes only).

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_SIGNAL_DELAY_H
#define SC_SIGNAL_DELAY_H

//
// Base class for delayed assignment of signals
//
// sc_signal_delay_base serves as the base class for delayed assignment of
// signals.  This is so that polymorphism can be achieved---the sc_clock_edge
// does not have to know exactly what the signal is: the subclasses define
// that.
//
class sc_signal_delay_base {
    friend class sc_clock_edge;

protected:
    sc_signal_delay_base( const sc_module* p, sc_signal_base* dst )
        : proc(p), dest(dst), nxt(0)
    {
    }
    virtual ~sc_signal_delay_base() { }
    const sc_module* process() const { return proc; }
    sc_signal_base* destination() const { return dest; }
    virtual bool do_xwrite() = 0;

private:    
    sc_signal_delay_base* get_next() const { return nxt; }
    void set_next( sc_signal_delay_base* n ) { nxt = n; }

        // The process that caused the delayed assignment.
    const sc_module* proc;
    sc_signal_base* dest;
        // Next assignment with the same delay
    sc_signal_delay_base* nxt;
};

template< class T > class sc_signal_delay : public sc_signal_delay_base
{
    friend class sc_signal<T>;

private:
    sc_signal_delay( const sc_module* p, sc_signal<T>* dst, const T& nv );
    ~sc_signal_delay();
    bool do_xwrite();

    T new_value;
};


#ifdef __GNUC__

template< class A, class T >
class sc_signal_array_delay_gnu : public sc_signal_delay_base
{
    friend class sc_signal_array_gnu<A,T>;

private:
    sc_signal_array_delay_gnu( const sc_module* p,
                               sc_signal_array_gnu<A,T>* dst,
                               const T* nv );
    sc_signal_array_delay_gnu( const sc_module* p,
                               sc_signal_array_gnu<A,T>* dst,
                               const sc_array<T>& nv );
    sc_signal_array_delay_gnu( const sc_module* p,
                               sc_signal_array_gnu<A,T>* dst );
    ~sc_signal_array_delay_gnu();

    bool do_xwrite();
    T* new_value_array;
};

#else

template< class A >
class sc_signal_array_delay : public sc_signal_delay_base
{
    friend class sc_signal_array<A>;
    typedef typename A::elemtype T;

private:
    sc_signal_array_delay( const sc_module* p,
                           sc_signal_array<A>* dst,
                           const T* nv );
    sc_signal_array_delay( const sc_module* p,
                           sc_signal_array<A>* dst,
                           const sc_array<T>& nv );
    sc_signal_array_delay( const sc_module* p, sc_signal_array<A>* dst );
    ~sc_signal_array_delay();
    bool do_xwrite();

    T* new_value_array;
};

#endif

#include "sc_signal_delay_defs.h_"

#endif
