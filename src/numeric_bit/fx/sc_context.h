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

    sc_context.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_CONTEXT_H
#define SC_CONTEXT_H


#include "sc_fxdefs.h"
#include "../../sc_simcontext.h"
#include "../../sc_hash.h"


class sc_process_b;


// ----------------------------------------------------------------------------
//  CLASS : sc_without_context
//
//  Empty class that is used for its type only.
// ----------------------------------------------------------------------------

class sc_without_context {};


// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_global
//
//  Template global variable class; singleton; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
class sc_global
{

    sc_global();

    void update();

public:

    static sc_global<T>* instance();

    const T*& value_ptr();

private:

    static sc_global<T>* _instance;

    sc_phash<const sc_process_b*,const T*> _map;
    const sc_process_b*                    _proc;
    const T*                               _value_ptr;

};


// ----------------------------------------------------------------------------
//  ENUM : sc_context_begin
//
//  Enumeration of context begin options.
// ----------------------------------------------------------------------------

enum sc_context_begin
{
    SC_NOW,
    SC_LATER
};


// ----------------------------------------------------------------------------
//  CLASS : sc_context
//
//  Template context class; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
class sc_context
{

    sc_context( const sc_context<T>& );
    void* operator new( size_t );

public:

    explicit sc_context( const T&, sc_context_begin = SC_NOW );
    ~sc_context();

    void begin();
    void end();

    static const T& default_value();
    const T& value() const;

private:

    const T   _value;
    const T*& _def_value_ptr;
    const T*  _old_value_ptr;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_global
//
//  Template global variable class; singleton; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
sc_global<T>* sc_global<T>::_instance = 0;


template <class T>
inline
sc_global<T>::sc_global()
: _proc( reinterpret_cast<const sc_process_b*>( -1 ) ), _value_ptr( 0 )
{}


template <class T>
inline
void
sc_global<T>::update()
{
    const sc_process_b* p = sc_get_curr_process_handle();
    if( p != _proc )
    {
        const T* vp = _map[p];
        if( vp == 0 )
        {
            vp = new T( sc_without_context() );
            _map.insert( p, vp );
        }
        _proc = p;
        _value_ptr = vp;
    }
}


template <class T>
inline
sc_global<T>*
sc_global<T>::instance()
{
    if( _instance == 0 )
    {
        _instance = new sc_global<T>;
    }
    return _instance;
}


template <class T>
inline
const T*&
sc_global<T>::value_ptr()
{
    update();
    return _value_ptr;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_context
//
//  Template context class; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
inline
sc_context<T>::sc_context( const sc_context<T>& )
: _def_value_ptr( sc_global<T>::instance()->value_ptr() )
{
    // this constructor should never be called
    _SC_NO_DEFN;
}

template <class T>
inline
void*
sc_context<T>::operator new( size_t )
{
    // this method should never be called
    _SC_NO_DEFN;
    throw;
#if defined(__HP_aCC)
    return 0;
#endif
}


template <class T>
inline
sc_context<T>::sc_context( const T& value_, sc_context_begin begin )
: _value( value_ ),
  _def_value_ptr( sc_global<T>::instance()->value_ptr() ),
  _old_value_ptr( 0 )
{
    if( begin == SC_NOW )
    {
	_old_value_ptr = _def_value_ptr;
	_def_value_ptr = &_value;
    }
}

template <class T>
inline
sc_context<T>::~sc_context()
{
    if( _old_value_ptr != 0 )
    {
        _def_value_ptr = _old_value_ptr;
	_old_value_ptr = 0;
    }
}


template <class T>
inline
void
sc_context<T>::begin()
{
    if( _old_value_ptr == 0 )
    {
	_old_value_ptr = _def_value_ptr;
	_def_value_ptr = &_value;
    }
    else
    {
        sc_report::error( _SC_ID_CONTEXT_BEGIN_FAILED );
    }
}

template <class T>
inline
void
sc_context<T>::end()
{
    if( _old_value_ptr != 0 )
    {
        _def_value_ptr = _old_value_ptr;
	_old_value_ptr = 0;
    }
    else
    {
        sc_report::error( _SC_ID_CONTEXT_END_FAILED );
    }
}


template <class T>
inline
const T&
sc_context<T>::default_value()
{
    return *sc_global<T>::instance()->value_ptr();
}

template <class T>
inline
const T&
sc_context<T>::value() const
{
    return _value;
}


#endif

// Taf!
