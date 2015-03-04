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

    scfx_other_defs.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_OTHER_DEFS_H
#define SCFX_OTHER_DEFS_H


#include "../sc_bigint.h"
#include "../sc_biguint.h"
#include "../sc_int.h"
#include "../sc_uint.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_signed
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_signed&
sc_signed::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
sc_signed&
sc_signed::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
sc_signed&
sc_signed::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
sc_signed&
sc_signed::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}


// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_bigint
// ----------------------------------------------------------------------------

// assignment operators

template <int W>
inline
sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_fxval& v )
{
    sc_signed::operator = ( v );
    return *this;
}

template <int W>
inline
sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_fxval_fast& v )
{
    sc_signed::operator = ( v );
    return *this;
}

template <int W>
inline
sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_fxnum& v )
{
    sc_signed::operator = ( v );
    return *this;
}

template <int W>
inline
sc_bigint<W>&
sc_bigint<W>::operator = ( const sc_fxnum_fast& v )
{
    sc_signed::operator = ( v );
    return *this;
}


// constructors

template <int W>
inline
sc_bigint<W>::sc_bigint( const sc_fxval& v )
: sc_signed( W )
{
    *this = v;
}

template <int W>
inline
sc_bigint<W>::sc_bigint( const sc_fxval_fast& v )
: sc_signed( W )
{
    *this = v;
}

template <int W>
inline
sc_bigint<W>::sc_bigint( const sc_fxnum& v )
: sc_signed( W )
{
    *this = v;
}

template <int W>
inline
sc_bigint<W>::sc_bigint( const sc_fxnum_fast& v )
: sc_signed( W )
{
    *this = v;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_unsigned&
sc_unsigned::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
sc_unsigned&
sc_unsigned::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
sc_unsigned&
sc_unsigned::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}

inline
sc_unsigned&
sc_unsigned::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < length(); ++ i )
	(*this)[i] = v.get_bit( i );

    return *this;
}


// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_biguint
// ----------------------------------------------------------------------------

// assignment operators

template <int W>
inline
sc_biguint<W>&
sc_biguint<W>::operator = ( const sc_fxval& v )
{
    sc_unsigned::operator = ( v );
    return *this;
}

template <int W>
inline
sc_biguint<W>&
sc_biguint<W>::operator = ( const sc_fxval_fast& v )
{
    sc_unsigned::operator = ( v );
    return *this;
}

template <int W>
inline
sc_biguint<W>&
sc_biguint<W>::operator = ( const sc_fxnum& v )
{
    sc_unsigned::operator = ( v );
    return *this;
}

template <int W>
inline
sc_biguint<W>&
sc_biguint<W>::operator = ( const sc_fxnum_fast& v )
{
    sc_unsigned::operator = ( v );
    return *this;
}


// constructors

template <int W>
inline
sc_biguint<W>::sc_biguint( const sc_fxval& v )
: sc_unsigned( W )
{
    *this = v;
}

template <int W>
inline
sc_biguint<W>::sc_biguint( const sc_fxval_fast& v )
: sc_unsigned( W )
{
    *this = v;
}

template <int W>
inline
sc_biguint<W>::sc_biguint( const sc_fxnum& v )
: sc_unsigned( W )
{
    *this = v;
}

template <int W>
inline
sc_biguint<W>::sc_biguint( const sc_fxnum_fast& v )
: sc_unsigned( W )
{
    *this = v;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
// ----------------------------------------------------------------------------

#ifndef _32BIT_
#define NUM_WIDTH LLWIDTH
#else
#define NUM_WIDTH INTWIDTH
#endif


// assignment operators

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    int S = NUM_WIDTH - width;
    num = num << S >> S;

    return *this;
}

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    int S = NUM_WIDTH - width;
    num = num << S >> S;

    return *this;
}

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    int S = NUM_WIDTH - width;
    num = num << S >> S;

    return *this;
}

inline
sc_int_base&
sc_int_base::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    int S = NUM_WIDTH - width;
    num = num << S >> S;

    return *this;
}

#undef NUM_WIDTH


// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_int
// ----------------------------------------------------------------------------

// assignment operators

template <int W>
inline
sc_int<W>&
sc_int<W>::operator = ( const sc_fxval& v )
{
    sc_int_base::operator = ( v );
    return *this;
}

template <int W>
inline
sc_int<W>&
sc_int<W>::operator = ( const sc_fxval_fast& v )
{
    sc_int_base::operator = ( v );
    return *this;
}

template <int W>
inline
sc_int<W>&
sc_int<W>::operator = ( const sc_fxnum& v )
{
    sc_int_base::operator = ( v );
    return *this;
}

template <int W>
inline
sc_int<W>&
sc_int<W>::operator = ( const sc_fxnum_fast& v )
{
    sc_int_base::operator = ( v );
    return *this;
}


// constructors

template <int W>
inline
sc_int<W>::sc_int( const sc_fxval& v )
: sc_int_base( W )
{
    *this = v;
}

template  <int W>
inline
sc_int<W>::sc_int( const sc_fxval_fast& v )
: sc_int_base( W )
{
    *this = v;
}

template <int W>
inline
sc_int<W>::sc_int( const sc_fxnum& v )
: sc_int_base( W )
{
    *this = v;
}

template <int W>
inline
sc_int<W>::sc_int( const sc_fxnum_fast& v )
: sc_int_base( W )
{
    *this = v;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_base
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxval& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    num &= MASK( width );

    return *this;
}

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxval_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point value argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    num &= MASK( width );

    return *this;
}

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxnum& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    num &= MASK( width );

    return *this;
}

inline
sc_uint_base&
sc_uint_base::operator = ( const sc_fxnum_fast& v )
{
    if( ! v.is_normal() ) /* also triggers OBSERVER_READ call */
    {
	fprintf( stderr,
		 "SystemC fatal: "
		 "fixed-point argument is invalid - NaN or Inf.\n" );
	abort();
    }

    for( int i = 0; i < width; ++ i )
	set( i, v.get_bit( i ) );

    num &= MASK( width );

    return *this;
}


// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_uint
// ----------------------------------------------------------------------------

// assignment operators

template <int W>
inline
sc_uint<W>&
sc_uint<W>::operator = ( const sc_fxval& v )
{
    sc_uint_base::operator = ( v );
    return *this;
}

template <int W>
inline
sc_uint<W>&
sc_uint<W>::operator = ( const sc_fxval_fast& v )
{
    sc_uint_base::operator = ( v );
    return *this;
}

template <int W>
inline
sc_uint<W>&
sc_uint<W>::operator = ( const sc_fxnum& v )
{
    sc_uint_base::operator = ( v );
    return *this;
}

template <int W>
inline
sc_uint<W>&
sc_uint<W>::operator = ( const sc_fxnum_fast& v )
{
    sc_uint_base::operator = ( v );
    return *this;
}


// constructors

template <int W>
inline
sc_uint<W>::sc_uint( const sc_fxval& v )
: sc_uint_base( W )
{
    *this = v;
}

template <int W>
inline
sc_uint<W>::sc_uint( const sc_fxval_fast& v )
: sc_uint_base( W )
{
    *this = v;
}

template <int W>
inline
sc_uint<W>::sc_uint( const sc_fxnum& v )
: sc_uint_base( W )
{
    *this = v;
}

template <int W>
inline
sc_uint<W>::sc_uint( const sc_fxnum_fast& v )
: sc_uint_base( W )
{
    *this = v;
}


// ----------------------------------------------------------------------------
//  FUNCTION : sc_trace
// ----------------------------------------------------------------------------

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxval& object, const sc_string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxval* object, const sc_string& name )
{
    if( tf )
	tf->trace( *object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxval_fast& object, const sc_string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxval_fast* object, const sc_string& name )
{
    if( tf )
	tf->trace( *object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxnum& object, const sc_string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxnum* object, const sc_string& name )
{
    if( tf )
	tf->trace( *object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxnum_fast& object, const sc_string& name )
{
    if( tf )
	tf->trace( object, name );
}

inline
void
sc_trace( sc_trace_file* tf,
	  const sc_fxnum_fast* object, const sc_string& name )
{
    if( tf )
	tf->trace( *object, name );
}


#endif

// Taf!
