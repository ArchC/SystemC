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

    sc_fxdefs.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXDEFS_H
#define SC_FXDEFS_H


#include "../sc_nbutils.h"
#include "../../sc_string.h"
#include "sc_report.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  ENUM : sc_enc
//
//  Enumeration of sign encodings.
// ----------------------------------------------------------------------------

enum sc_enc
{
    SC_TC,	// two's complement
    SC_US	// unsigned
};


const sc_string to_string( sc_enc );


inline
ostream&
operator << ( ostream& os, sc_enc enc )
{
    return os << to_string( enc );
}


// ----------------------------------------------------------------------------
//  ENUM : sc_q_mode
//
//  Enumeration of quantization modes.
// ----------------------------------------------------------------------------

enum sc_q_mode
{
    SC_RND,		// rounding to plus infinity
    SC_RND_ZERO,	// rounding to zero
    SC_RND_MIN_INF,	// rounding to minus infinity
    SC_RND_INF,		// rounding to infinity
    SC_RND_CONV,	// convergent rounding
    SC_TRN,		// truncation
    SC_TRN_ZERO		// truncation to zero
};


const sc_string to_string( sc_q_mode );


inline
ostream&
operator << ( ostream& os, sc_q_mode q_mode )
{
    return os << to_string( q_mode );
}


// ----------------------------------------------------------------------------
//  ENUM : sc_o_mode
//
//  Enumeration of overflow modes.
// ----------------------------------------------------------------------------

enum sc_o_mode
{
    SC_SAT,		// saturation
    SC_SAT_ZERO,	// saturation to zero
    SC_SAT_SYM,		// symmetrical saturation
    SC_WRAP,		// wrap-around (*)
    SC_WRAP_SM		// sign magnitude wrap-around (*)
};

// (*) uses the number of saturated bits argument, see the documentation.


const sc_string to_string( sc_o_mode );


inline
ostream&
operator << ( ostream& os, sc_o_mode o_mode )
{
    return os << to_string( o_mode );
}


// ----------------------------------------------------------------------------
//  ENUM : sc_switch
//
//  Enumeration of switch states.
// ----------------------------------------------------------------------------

enum sc_switch
{
    SC_OFF,
    SC_ON
};


const sc_string to_string( sc_switch );


inline
ostream&
operator << ( ostream& os, sc_switch sw )
{
    return os << to_string( sw );
}


// ----------------------------------------------------------------------------
//  ENUM : sc_fmt
//
//  Enumeration of formats for character string conversion.
// ----------------------------------------------------------------------------

enum sc_fmt
{
    SC_F,	// fixed
    SC_E	// scientific
};


const sc_string to_string( sc_fmt );


inline
ostream&
operator << ( ostream& os, sc_fmt fmt )
{
    return os << to_string( fmt );
}


// ----------------------------------------------------------------------------
//  Built-in & default fixed-point type parameter values.
// ----------------------------------------------------------------------------

const int       _SC_BUILTIN_WL     = 32;
const int       _SC_BUILTIN_IWL    = 32;
const sc_q_mode _SC_BUILTIN_Q_MODE = SC_TRN;
const sc_o_mode _SC_BUILTIN_O_MODE = SC_WRAP;
const int       _SC_BUILTIN_N_BITS = 0;


const int       _SC_DEFAULT_WL     = _SC_BUILTIN_WL;
const int       _SC_DEFAULT_IWL    = _SC_BUILTIN_IWL;
const sc_q_mode _SC_DEFAULT_Q_MODE = _SC_BUILTIN_Q_MODE;
const sc_o_mode _SC_DEFAULT_O_MODE = _SC_BUILTIN_O_MODE;
const int       _SC_DEFAULT_N_BITS = _SC_BUILTIN_N_BITS;


// ----------------------------------------------------------------------------
//  Built-in & default fixed-point cast switch parameter values.
// ----------------------------------------------------------------------------

const sc_switch _SC_BUILTIN_CAST_SWITCH = SC_ON;


const sc_switch _SC_DEFAULT_CAST_SWITCH = _SC_BUILTIN_CAST_SWITCH;


// ----------------------------------------------------------------------------
//  Built-in & default fixed-point value type parameter values.
// ----------------------------------------------------------------------------

const int _SC_BUILTIN_DIV_WL = 64;
const int _SC_BUILTIN_CTE_WL = 64;
const int _SC_BUILTIN_MAX_WL = 1024;


#if defined( SC_FXDIV_WL ) && ( SC_FXDIV_WL > 0 )
const int _SC_DEFAULT_DIV_WL = SC_FXDIV_WL;
#else
const int _SC_DEFAULT_DIV_WL = _SC_BUILTIN_DIV_WL;
#endif

#if defined( SC_FXCTE_WL ) && ( SC_FXCTE_WL > 0 )
const int _SC_DEFAULT_CTE_WL = SC_FXCTE_WL;
#else
const int _SC_DEFAULT_CTE_WL = _SC_BUILTIN_CTE_WL;
#endif

#if defined( SC_FXMAX_WL ) && ( SC_FXMAX_WL > 0 || SC_FXMAX_WL == -1 )
const int _SC_DEFAULT_MAX_WL = SC_FXMAX_WL;
#else
const int _SC_DEFAULT_MAX_WL = _SC_BUILTIN_MAX_WL;
#endif


// ----------------------------------------------------------------------------
//  Predefined report ids.
// ----------------------------------------------------------------------------

extern const char* _SC_ID_NO_DEFN;
extern const char* _SC_ID_INTERNAL_ERROR;
extern const char* _SC_ID_INVALID_WL;
extern const char* _SC_ID_INVALID_N_BITS;
extern const char* _SC_ID_INVALID_DIV_WL;
extern const char* _SC_ID_INVALID_CTE_WL;
extern const char* _SC_ID_INVALID_MAX_WL;
extern const char* _SC_ID_INVALID_VALUE;
extern const char* _SC_ID_INVALID_O_MODE;
extern const char* _SC_ID_OUT_OF_RANGE;
extern const char* _SC_ID_CONTEXT_BEGIN_FAILED;
extern const char* _SC_ID_CONTEXT_END_FAILED;


// ----------------------------------------------------------------------------
//  Predefined error reporting and checking.
// ----------------------------------------------------------------------------

#ifdef DEBUG_SYSTEMC
#define _SC_ASSERT(cnd,msg)                                                   \
{                                                                             \
    if( ! (cnd) )                                                             \
	sc_report::error( _SC_ID_INTERNAL_ERROR, msg );                       \
}
#else
#define _SC_ASSERT(cnd,msg)
#endif

#define _SC_ERROR_IF(cnd,id)                                                  \
{                                                                             \
    if( cnd )                                                                 \
	sc_report::error( id );                                               \
}


#define _SC_NO_DEFN                                                           \
    sc_report::fatal( _SC_ID_NO_DEFN );

#define _SC_CHECK_WL(wl)                                                      \
    _SC_ERROR_IF( (wl) <= 0, _SC_ID_INVALID_WL )

#define _SC_CHECK_N_BITS(n_bits)                                              \
    _SC_ERROR_IF( (n_bits) < 0, _SC_ID_INVALID_N_BITS )

#define _SC_CHECK_DIV_WL(div_wl)                                              \
    _SC_ERROR_IF( (div_wl) <= 0, _SC_ID_INVALID_DIV_WL )

#define _SC_CHECK_CTE_WL(cte_wl)                                              \
    _SC_ERROR_IF( (cte_wl) <= 0, _SC_ID_INVALID_CTE_WL )

#define _SC_CHECK_MAX_WL(max_wl)                                              \
    _SC_ERROR_IF( (max_wl) <= 0 && (max_wl) != -1, _SC_ID_INVALID_MAX_WL )


// ----------------------------------------------------------------------------
//  Generic observer macros.
// ----------------------------------------------------------------------------

#define _SC_OBSERVER(object,observer_type,event)                              \
{                                                                             \
    if( (object).observer() != 0 )                                            \
    {                                                                         \
	observer_type observer = (object).lock_observer();                    \
	observer->event( (object) );                                          \
	(object).unlock_observer( observer );                                 \
    }                                                                         \
}

#define _SC_OBSERVER_DEFAULT(observer_type)                                   \
{                                                                             \
    if( _observer == 0 && observer_type ## ::default_observer != 0 )          \
        _observer = (* ## observer_type ## ::default_observer)();             \
}


#endif

// Taf!
