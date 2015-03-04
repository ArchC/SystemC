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

    sc_fxcast_switch.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXCAST_SWITCH_H
#define SC_FXCAST_SWITCH_H


#include "sc_context.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_fxcast_switch
//
//  Fixed-point cast switch class.
// ----------------------------------------------------------------------------

class sc_fxcast_switch
{

public:

             sc_fxcast_switch();
             sc_fxcast_switch( sc_switch );
             sc_fxcast_switch( const sc_fxcast_switch& );
    explicit sc_fxcast_switch( sc_without_context );

    sc_fxcast_switch& operator = ( const sc_fxcast_switch& );

    friend bool operator == ( const sc_fxcast_switch&,
			      const sc_fxcast_switch& );
    friend bool operator != ( const sc_fxcast_switch&,
			      const sc_fxcast_switch& );

    const sc_string to_string() const;

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_switch _sw;

};


// ----------------------------------------------------------------------------
//  TYPEDEF : sc_fxcast_context
//
//  Context type for the fixed-point cast switch parameter.
// ----------------------------------------------------------------------------

typedef sc_context<sc_fxcast_switch> sc_fxcast_context;


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_fxcast_switch::sc_fxcast_switch()
{
    *this = sc_fxcast_context::default_value();
}

inline
sc_fxcast_switch::sc_fxcast_switch( sc_switch sw_ )
: _sw( sw_ )
{}

inline
sc_fxcast_switch::sc_fxcast_switch( const sc_fxcast_switch& a )
: _sw( a._sw )
{}

inline
sc_fxcast_switch::sc_fxcast_switch( sc_without_context )
: _sw( _SC_DEFAULT_CAST_SWITCH )
{}


inline
sc_fxcast_switch&
sc_fxcast_switch::operator = ( const sc_fxcast_switch& a )
{
    if( &a != this )
    {
        _sw = a._sw;
    }
    return *this;
}


inline
bool
operator == ( const sc_fxcast_switch& a, const sc_fxcast_switch& b )
{
    return ( a._sw == b._sw );
}


inline
bool
operator != ( const sc_fxcast_switch& a, const sc_fxcast_switch& b )
{
    return ( a._sw != b._sw );
}


inline
ostream&
operator << ( ostream& os, const sc_fxcast_switch& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
