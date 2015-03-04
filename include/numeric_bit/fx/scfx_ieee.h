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

    scfx_ieee.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_IEEE_H
#define SCFX_IEEE_H


#include "sc_fxdefs.h"


// ----------------------------------------------------------------------------
//  Little or big endian machine?
// ----------------------------------------------------------------------------

#if defined( i386 ) || defined(WIN32)
# define SCFX_LITTLE_ENDIAN
#elif defined( sparc ) || defined( __hppa )
# define SCFX_BIG_ENDIAN
#endif


// ----------------------------------------------------------------------------
//  UNION : ieee_double
//
//  IEEE 754 double-precision format.
// ----------------------------------------------------------------------------
    
union ieee_double
{

    double d;

    struct
    {
#if defined( SCFX_BIG_ENDIAN )
        unsigned negative:1;
        unsigned exponent:11;
        unsigned mantissa0:20;
        unsigned mantissa1:32;
#elif defined( SCFX_LITTLE_ENDIAN )
        unsigned mantissa1:32;
        unsigned mantissa0:20;
        unsigned exponent:11;
        unsigned negative:1;
#endif
    } s;

};


const unsigned int SCFX_IEEE_DOUBLE_BIAS   =  1023U;

const int          SCFX_IEEE_DOUBLE_E_MAX  =  1023;
const int          SCFX_IEEE_DOUBLE_E_MIN  = -1022;

const unsigned int SCFX_IEEE_DOUBLE_M_SIZE =    52;


// ----------------------------------------------------------------------------
//  CLASS : scfx_ieee_double
//
//  Convenient interface to union ieee_double.
// ----------------------------------------------------------------------------

class scfx_ieee_double
{

    ieee_double _id;

public:
    
    scfx_ieee_double();
    scfx_ieee_double( double );
    scfx_ieee_double( const scfx_ieee_double& );
    
    scfx_ieee_double& operator = ( double );
    scfx_ieee_double& operator = ( const scfx_ieee_double& );
    
    operator double() const;

    unsigned int negative() const;
    void negative( unsigned int );
    int exponent() const;
    void exponent( int );
    unsigned int mantissa0() const;
    void mantissa0( unsigned int );
    unsigned int mantissa1() const;
    void mantissa1( unsigned int );

    bool is_zero() const;
    bool is_subnormal() const;
    bool is_normal() const;
    bool is_inf() const;
    bool is_nan() const;

    void set_inf();
    void set_nan();

    int msb() const;            // most significant non-zero bit
    int lsb() const;            // least significant non-zero bit

    static const scfx_ieee_double nan();
    static const scfx_ieee_double inf( int );

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
    
inline
scfx_ieee_double::scfx_ieee_double()
{
    _id.d = 0.0;
}

inline
scfx_ieee_double::scfx_ieee_double( double d )
{
    _id.d = d;
}

inline
scfx_ieee_double::scfx_ieee_double( const scfx_ieee_double& a )
{
    _id.d = a._id.d;
}
    

inline
scfx_ieee_double&
scfx_ieee_double::operator = ( double d )
{
    _id.d = d;
    return *this;
}

inline
scfx_ieee_double&
scfx_ieee_double::operator = ( const scfx_ieee_double& a )
{
    _id.d = a._id.d;
    return *this;
}
    

inline
scfx_ieee_double::operator double() const
{
    return _id.d;
}


inline
unsigned int
scfx_ieee_double::negative() const
{
    return _id.s.negative;
}

inline
void
scfx_ieee_double::negative( unsigned int a )
{
    _id.s.negative = a;
}

inline
int
scfx_ieee_double::exponent() const
{
    return _id.s.exponent - SCFX_IEEE_DOUBLE_BIAS;
}

inline
void
scfx_ieee_double::exponent( int a )
{
    _id.s.exponent = SCFX_IEEE_DOUBLE_BIAS + a;
}

inline
unsigned int
scfx_ieee_double::mantissa0() const
{
    return _id.s.mantissa0;
}

inline
void
scfx_ieee_double::mantissa0( unsigned int a )
{
    _id.s.mantissa0 = a;
}

inline
unsigned int
scfx_ieee_double::mantissa1() const
{
    return _id.s.mantissa1;
}

inline
void
scfx_ieee_double::mantissa1( unsigned int a )
{
    _id.s.mantissa1 = a;
}


inline
bool
scfx_ieee_double::is_zero() const
{
    return ( exponent() == SCFX_IEEE_DOUBLE_E_MIN - 1 &&
             mantissa0() == 0U && mantissa1() == 0U );
}

inline
bool
scfx_ieee_double::is_subnormal() const
{
    return ( exponent() == SCFX_IEEE_DOUBLE_E_MIN - 1 &&
             ( mantissa0() != 0U || mantissa1() != 0U ) );
}

inline
bool
scfx_ieee_double::is_normal() const
{
    return ( exponent() >= SCFX_IEEE_DOUBLE_E_MIN &&
             exponent() <= SCFX_IEEE_DOUBLE_E_MAX );
}

inline
bool
scfx_ieee_double::is_inf() const
{
    return ( exponent() == SCFX_IEEE_DOUBLE_E_MAX + 1 &&
             mantissa0() == 0U && mantissa1() == 0U );
}

inline
bool
scfx_ieee_double::is_nan() const
{
    return ( exponent() == SCFX_IEEE_DOUBLE_E_MAX + 1 &&
             ( mantissa0() != 0U || mantissa1() != 0U ) );
}


inline
void
scfx_ieee_double::set_inf()
{
    exponent( SCFX_IEEE_DOUBLE_E_MAX + 1 );
    mantissa0( 0U );
    mantissa1( 0U );
}

inline
void
scfx_ieee_double::set_nan()
{
    exponent( SCFX_IEEE_DOUBLE_E_MAX + 1 );
    mantissa0( (unsigned int) -1 );
    mantissa1( (unsigned int) -1 );
}


#define MSB_STATEMENT(x,n) if( x >> n ) { x >>= n; i += n; }

inline
int
scfx_ieee_double::msb() const
{
    unsigned int m0 = mantissa0();
    unsigned int m1 = mantissa1();
    if( m0 != 0 )
    {
        int i = 0;
        MSB_STATEMENT(m0,16);
        MSB_STATEMENT(m0,8);
        MSB_STATEMENT(m0,4);
        MSB_STATEMENT(m0,2);
        MSB_STATEMENT(m0,1);
        return ( i - 20 );
    }
    else if( m1 != 0 )
    {
        int i = 0;
        MSB_STATEMENT(m1,16);
        MSB_STATEMENT(m1,8);
        MSB_STATEMENT(m1,4);
        MSB_STATEMENT(m1,2);
        MSB_STATEMENT(m1,1);
        return ( i - 52 );
    }
    else
    {
        return 0;
    }
}

#undef MSB_STATEMENT

#define LSB_STATEMENT(x,n) if( x << n ) { x <<= n; i -= n; }

inline
int
scfx_ieee_double::lsb() const
{
    unsigned int m0 = mantissa0();
    unsigned int m1 = mantissa1();
    if( m1 != 0 )
    {
        int i = 31;
        LSB_STATEMENT(m1,16);
        LSB_STATEMENT(m1,8);
        LSB_STATEMENT(m1,4);
        LSB_STATEMENT(m1,2);
        LSB_STATEMENT(m1,1);
        return ( i - 52 );
    }
    else if( m0 != 0 )
    {
        int i = 31;
        LSB_STATEMENT(m0,16);
        LSB_STATEMENT(m0,8);
        LSB_STATEMENT(m0,4);
        LSB_STATEMENT(m0,2);
        LSB_STATEMENT(m0,1);
        return ( i - 20 );
    }
    else
    {
        return 0;
    }
}

#undef LSB_STATEMENT


inline
const scfx_ieee_double
scfx_ieee_double::nan()
{
    scfx_ieee_double id;
    id.set_nan();
    return id;
}

inline
const scfx_ieee_double
scfx_ieee_double::inf( int sign )
{
    scfx_ieee_double id( sign );
    id.set_inf();
    return id;
}


// ----------------------------------------------------------------------------
//  UNION : ieee_float
//
//  IEEE 754 single-precision format.
// ----------------------------------------------------------------------------
    
union ieee_float
{

    float f;

    struct
    {
#if defined( SCFX_BIG_ENDIAN )
        unsigned negative:1;
        unsigned exponent:8;
        unsigned mantissa:23;
#elif defined( SCFX_LITTLE_ENDIAN )
        unsigned mantissa:23;
        unsigned exponent:8;
        unsigned negative:1;
#endif
    } s;

};


const unsigned int SCFX_IEEE_FLOAT_BIAS   =  127U;

const int          SCFX_IEEE_FLOAT_E_MAX  =  127;
const int          SCFX_IEEE_FLOAT_E_MIN  = -126;

const unsigned int SCFX_IEEE_FLOAT_M_SIZE =   23;


// ----------------------------------------------------------------------------
//  CLASS : scfx_ieee_float
//
// Convenient wrapper to union ieee_float.
// ----------------------------------------------------------------------------

class scfx_ieee_float
{

    ieee_float _if;

public:

    scfx_ieee_float();
    scfx_ieee_float( float );
    scfx_ieee_float( const scfx_ieee_float& );

    scfx_ieee_float& operator = ( float );
    scfx_ieee_float& operator = ( const scfx_ieee_float& );

    operator float() const;

    unsigned int negative() const;
    void negative( unsigned int );
    int exponent() const;
    void exponent( int );
    unsigned int mantissa() const;
    void mantissa( unsigned int );

    bool is_zero() const;
    bool is_subnormal() const;
    bool is_normal() const;
    bool is_inf() const;
    bool is_nan() const;

    void set_inf();
    void set_nan();

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
    
inline
scfx_ieee_float::scfx_ieee_float()
{
    _if.f = 0.0;
}

inline
scfx_ieee_float::scfx_ieee_float( float f )
{
    _if.f = f;
}

inline
scfx_ieee_float::scfx_ieee_float( const scfx_ieee_float& a )
{
    _if.f = a._if.f;
}


inline
scfx_ieee_float&
scfx_ieee_float::operator = ( float f )
{
    _if.f = f;
    return *this;
}

inline
scfx_ieee_float&
scfx_ieee_float::operator = ( const scfx_ieee_float& a )
{
    _if.f = a._if.f;
    return *this;
}
    

inline
scfx_ieee_float::operator float() const
{
    return _if.f;
}


inline
unsigned int
scfx_ieee_float::negative() const
{
    return _if.s.negative;
}

inline
void
scfx_ieee_float::negative( unsigned int a )
{
    _if.s.negative = a;
}

inline
int
scfx_ieee_float::exponent() const
{
    return _if.s.exponent - SCFX_IEEE_FLOAT_BIAS;
}

inline
void
scfx_ieee_float::exponent( int a )
{
    _if.s.exponent = SCFX_IEEE_FLOAT_BIAS + a;
}

inline
unsigned int
scfx_ieee_float::mantissa() const
{
    return _if.s.mantissa;
}

inline
void
scfx_ieee_float::mantissa( unsigned int a )
{
    _if.s.mantissa = a;
}


inline
bool
scfx_ieee_float::is_zero() const
{
    return ( exponent() == SCFX_IEEE_FLOAT_E_MIN - 1 && mantissa() == 0U );
}

inline
bool
scfx_ieee_float::is_subnormal() const
{
    return ( exponent() == SCFX_IEEE_FLOAT_E_MIN - 1 && mantissa() != 0U );
}

inline
bool
scfx_ieee_float::is_normal() const
{
    return ( exponent() >= SCFX_IEEE_FLOAT_E_MIN &&
             exponent() <= SCFX_IEEE_FLOAT_E_MAX );
}

inline
bool
scfx_ieee_float::is_inf() const
{
    return ( exponent() == SCFX_IEEE_FLOAT_E_MAX + 1 && mantissa() == 0U );
}

inline
bool
scfx_ieee_float::is_nan() const
{
    return ( exponent() == SCFX_IEEE_FLOAT_E_MAX + 1 && mantissa() != 0U );
}


inline
void
scfx_ieee_float::set_inf()
{
    exponent( SCFX_IEEE_FLOAT_E_MAX + 1 );
    mantissa( 0U );
}

inline
void
scfx_ieee_float::set_nan()
{
    exponent( SCFX_IEEE_FLOAT_E_MAX + 1 );
    mantissa( (unsigned int) -1 );
}


// ----------------------------------------------------------------------------
//  FUNCTION : scfx_pow2
//
//  Computes 2.0**exp in double-precision.
// ----------------------------------------------------------------------------

inline
double scfx_pow2( int exp )
{
    scfx_ieee_double r;
    if( exp < SCFX_IEEE_DOUBLE_E_MIN )
    {
        r = 0.0;
        // handle subnormal case
        exp -= SCFX_IEEE_DOUBLE_E_MIN;
        if( ( exp += 20 ) >= 0 )
	{
            r.mantissa0( 1U << exp );
        }
	else if( ( exp += 32 ) >= 0 )
	{
            r.mantissa1( 1U << exp );
        }
    }
    else if( exp > SCFX_IEEE_DOUBLE_E_MAX )
    {
        r.set_inf();
    }
    else
    {
        r = 1.0;
        r.exponent( exp );
    }
    return r;
}


#endif

// Taf!
