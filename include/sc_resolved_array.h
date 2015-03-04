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

    sc_resolved_array.h -- one-dimensional arrays of signals of type T

    Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_RESOLVED_ARRAY_H
#define SC_RESOLVED_ARRAY_H

#include <assert.h>
#include "sc_resolved.h"
#include "sc_signal_array.h"
#include "sc_write_macros.h"


// sc_signal_resolved_array_gnu is a template class where A is a sc_signal_array<T> type,
// T is the base type of the signal array and R is the resolution class. For
// example, if A is sc_signal_logic_vector, then T is sc_logic and R could be
// sc_logic_resolve. 
// sc_signal_resolved_array_gnu is designed to create resolved signal arrays for any
// type. 

// sc_signal_resolved_array_gnu is a type of signal array, therefore it inherits from
// the signal array.

template< class A, class T, class R >
class sc_signal_resolved_array_gnu
    : public A   // inherit the properties of a signal array, where A is sc_signal_array<T>
{
public:
    typedef sc_signal_resolved_array_gnu<A,T,R> this_type;
    typedef sc_signal_resolved_t<T,R> resolved_sig_type; // Type of the resolved signal

protected:
    // The various constructors seen by derived classes
    sc_signal_resolved_array_gnu( int len, sc_signal<T>* (*create_sig)(void *) );
    sc_signal_resolved_array_gnu( const char* nm, int len, sc_signal<T>* (*create_sig)(void *) );
    sc_signal_resolved_array_gnu( int len, const this_type& ref );

    virtual ~sc_signal_resolved_array_gnu();

protected:
    // Prevent the use of the copy constructor
    sc_signal_resolved_array_gnu( const this_type& ref );

public:
    // No one is using this currently, but we will keep it here
    virtual bool is_resolved() const { return true; }

    DECLARE_SUBSCRIPT_OP(this_type, resolved_sig_type)

private:
    virtual sc_signal<T>& subscript(int i);

protected:
    DECLARE_PARTSEL(this_type)

protected:
    // Should never be called. Instead, update on indiv signals will be called
    virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const { assert(false); return 0; }
    virtual void update() { assert(false); } 
    this_type& write(const sc_array<T>&);
    this_type& write(const T*);
    this_type& operator=(const this_type& nv)   { return write(nv.read()); }
    this_type& operator=(const sc_array<T>& nv) { return write(nv); }
    this_type& operator=(T* nv)                 { return write(nv); }

protected:
    const this_type* parent() const
    {
        return (this_type*) A::parent();
    }

protected:
    // Virtual constructor that will be called by the sub method in sc_array_base
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;
    // The actual constructor that will be called by the virtual constructor
    sc_signal_resolved_array_gnu( const sc_array_base* p, int l, int r );
    

protected:
    // Constructors for concatenation
    sc_signal_resolved_array_gnu( sc_signal_resolved_array_gnu<A,T,R>* lhs, sc_signal_resolved_array_gnu<A,T,R>* rhs );
    sc_signal_resolved_array_gnu( sc_signal_resolved_array_gnu<A,T,R>* lhs, sc_signal_resolved_t<T,R>* rhs );
    sc_signal_resolved_array_gnu( sc_signal_resolved_t<T,R>* lhs, sc_signal_resolved_array_gnu<A,T,R>* rhs );
    sc_signal_resolved_array_gnu( sc_signal_resolved_t<T,R>* lhs, sc_signal_resolved_t<T,R>* rhs );
};


// We now define sc_signal_resolved_array, which inherits from sc_signal_resolved_array_gnu.
// A is a sc_signal_array<T>. Note that A::elemtype (i.e. T) is defined in A. Note that R is still the 
// resolution class.


template< class A, class R >
class sc_signal_resolved_array
  : public sc_signal_resolved_array_gnu< A, typename A::elemtype, R >
{
    friend class sc_clock;

public:
    typedef typename A::elemtype T;
    typedef sc_signal_resolved_array<A,R> this_type;

public:
    sc_signal_resolved_array( int len, sc_signal<T>* (*create_sig)(void *) )
        : sc_signal_resolved_array_gnu<A,T,R>( len, create_sig )
    {
        /* Intentionally Blank */
    }
    sc_signal_resolved_array( const char* nm, int len, sc_signal<T>* (*create_sig)(void *) )
        : sc_signal_resolved_array_gnu<A,T,R>( nm, len, create_sig )
    {
        /* Intentionally Blank */
    }
    sc_signal_resolved_array( int len, const sc_signal_resolved_array<A,R>& ref )
        : sc_signal_resolved_array_gnu<A,T,R>( len, (const sc_signal_resolved_array_gnu<A,T,R>&) ref )
    {
        /* Intentionally Blank */
    }

protected:
    sc_signal_resolved_array( const this_type& ref )
        : sc_signal_resolved_array_gnu<A,T,R>( ref )
    {
        assert(false); // Disallow copy constructor
    }

public:
    DECLARE_PARTSEL(this_type)

    virtual ~sc_signal_resolved_array()
    {
        /* Intentionally Blank */
    }

    // The virtual constructor that will be called by sc_array_base::sub
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;

    // The actual constructor that will be called by the virtual constructor
    sc_signal_resolved_array( const sc_array_base* p, int l, int r )
        : sc_signal_resolved_array_gnu<A,T,R>( p, l, r )
    {
        /* Intentionally Blank */
    }

    // Constructors for concatenation
    sc_signal_resolved_array( sc_signal_resolved_array<A,R>* lhs, sc_signal_resolved_array<A,R>* rhs )
        : sc_signal_resolved_array_gnu<A,T,R>( lhs, rhs )
    {
        /* Intentionally Blank */
    }
    sc_signal_resolved_array( sc_signal_resolved_array<A,R>* lhs, sc_signal_resolved_t<T,R>* rhs )
        : sc_signal_resolved_array_gnu<A,T,R>( lhs, rhs )
    {
        /* Intentionally Blank */
    }
    sc_signal_resolved_array( sc_signal_resolved_t<T,R>* lhs, sc_signal_resolved_array<A,R>* rhs )
        : sc_signal_resolved_array_gnu<A,T,R>( lhs, rhs )
    {
        /* Intentionally Blank */
    }
    sc_signal_resolved_array( sc_signal_resolved_t<T,R>* lhs, sc_signal_resolved_t<T,R>* rhs )
        : sc_signal_resolved_array_gnu<A,T,R>( lhs, rhs )
    {
        /* Intentionally Blank */
    }

    this_type& write(const sc_array<T>& nv)
    {
        return (sc_signal_resolved_array<A,R>&) sc_signal_resolved_array_gnu<A,T,R>::write(nv);
    }
    this_type& write(const T* nv)
    {
        return (sc_signal_resolved_array<A,R>&) sc_signal_resolved_array_gnu<A,T,R>::write(nv);
    }
    this_type& operator=(const this_type& nv)   { return write(nv.read()); }
    this_type& operator=(const sc_array<T>& nv) { return write(nv);        }
    this_type& operator=(const T* nv)           { return write(nv);        }

protected:
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );

public:
    static this_type* get_concat_array( this_type* x, this_type* y )
    {
        return (this_type*) sc_array_base::get_concat_array( x, y, new_array_vv );
    }
    static this_type* get_concat_array( this_type* x, sc_signal_resolved_t<T,R>* y )
    {
        return (this_type*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
    }
    static this_type* get_concat_array( sc_signal_resolved_t<T,R>* x, this_type* y )
    {
        return (this_type*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
    }
    static this_type* get_concat_array( sc_signal_resolved_t<T,R>* x, sc_signal_resolved_t<T,R>* y )
    {
        return (this_type*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
    }
};

/*******************************************************************************************************/

#include "sc_resolved_array_defs.h_"

#endif


