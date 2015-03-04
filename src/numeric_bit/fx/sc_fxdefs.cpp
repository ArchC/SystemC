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

    sc_fxdefs.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fxdefs.h"


// ----------------------------------------------------------------------------
//  ENUM : sc_enc
//
//  Enumeration of sign encodings.
// ----------------------------------------------------------------------------

const sc_string
to_string( sc_enc enc )
{
    switch( enc )
    {
        case SC_TC:
            return sc_string( "SC_TC" );
        case SC_US:
            return sc_string( "SC_US" );
	default:
	    return sc_string( "unknown" );
    }
}


// ----------------------------------------------------------------------------
//  ENUM : sc_q_mode
//
//  Enumeration of quantization modes.
// ----------------------------------------------------------------------------

const sc_string
to_string( sc_q_mode q_mode )
{
    switch( q_mode )
    {
        case SC_RND:
            return sc_string( "SC_RND" );
        case SC_RND_ZERO:
            return sc_string( "SC_RND_ZERO" );
        case SC_RND_MIN_INF:
            return sc_string( "SC_RND_MIN_INF" );
        case SC_RND_INF:
            return sc_string( "SC_RND_INF" );
        case SC_RND_CONV:
            return sc_string( "SC_RND_CONV" );
        case SC_TRN:
            return sc_string( "SC_TRN" );
        case SC_TRN_ZERO:
            return sc_string( "SC_TRN_ZERO" );
	default:
	    return sc_string( "unknown" );
    }
}


// ----------------------------------------------------------------------------
//  ENUM : sc_o_mode
//
//  Enumeration of overflow modes.
// ----------------------------------------------------------------------------

const sc_string
to_string( sc_o_mode o_mode )
{
    switch( o_mode )
    {
        case SC_SAT:
            return sc_string( "SC_SAT" );
        case SC_SAT_ZERO:
            return sc_string( "SC_SAT_ZERO" );
        case SC_SAT_SYM:
            return sc_string( "SC_SAT_SYM" );
        case SC_WRAP:
            return sc_string( "SC_WRAP" );
        case SC_WRAP_SM:
            return sc_string( "SC_WRAP_SM" );
	default:
	    return sc_string( "unknown" );
    }
}


// ----------------------------------------------------------------------------
//  ENUM : sc_switch
//
//  Enumeration of switch states.
// ----------------------------------------------------------------------------

const sc_string
to_string( sc_switch sw )
{
    switch( sw ) {
        case SC_OFF:
            return sc_string( "SC_OFF" );
        case SC_ON:
            return sc_string( "SC_ON" );
	default:
	    return sc_string( "unknown" );
    }
}


// ----------------------------------------------------------------------------
//  ENUM : sc_fmt
//
//  Enumeration of formats for character string conversion.
// ----------------------------------------------------------------------------

const sc_string
to_string( sc_fmt fmt )
{
    switch( fmt ) {
        case SC_F:
            return sc_string( "SC_F" );
        case SC_E:
            return sc_string( "SC_E" );
	default:
	    return sc_string( "unknown" );
    }
}


// ----------------------------------------------------------------------------
//  Predefined report ids.
// ----------------------------------------------------------------------------

const char* _SC_ID_NO_DEFN =
    "no_defn";

const char* _SC_ID_INTERNAL_ERROR =
    "internal_error";

const char* _SC_ID_INVALID_WL =
    "invalid_wl";

const char* _SC_ID_INVALID_N_BITS =
    "invalid_n_bits";

const char* _SC_ID_INVALID_DIV_WL =
    "invalid_div_wl";

const char* _SC_ID_INVALID_CTE_WL =
    "invalid_cte_wl";

const char* _SC_ID_INVALID_MAX_WL =
    "invalid_max_wl";

const char* _SC_ID_INVALID_VALUE =
    "invalid_value";

const char* _SC_ID_INVALID_O_MODE =
    "invalid_o_mode";

const char* _SC_ID_OUT_OF_RANGE =
    "out_of_range";

const char* _SC_ID_CONTEXT_BEGIN_FAILED =
    "context_begin_failed";

const char* _SC_ID_CONTEXT_END_FAILED =
    "context_end_failed";


// ----------------------------------------------------------------------------
//  Register error ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    sc_report::register_id( _SC_ID_NO_DEFN,
                            "no definition" );
    sc_report::register_id( _SC_ID_INTERNAL_ERROR,
			    "internal error" );
    sc_report::register_id( _SC_ID_INVALID_WL,
                            "total wordlength <= 0 is not valid" );
    sc_report::register_id( _SC_ID_INVALID_N_BITS,
                            "number of bits < 0 is not valid" );
    sc_report::register_id( _SC_ID_INVALID_DIV_WL,
                            "division wordlength <= 0 is not valid" );
    sc_report::register_id( _SC_ID_INVALID_CTE_WL,
                            "constant wordlength <= 0 is not valid" );
    sc_report::register_id( _SC_ID_INVALID_MAX_WL,
                            "maximum wordlength <= 0 and != -1 is not valid" );
    sc_report::register_id( _SC_ID_INVALID_VALUE,
			    "invalid fixed-point value" );
    sc_report::register_id( _SC_ID_INVALID_O_MODE,
			    "invalid overflow mode" );
    sc_report::register_id( _SC_ID_OUT_OF_RANGE,
			    "index out of range" );
    sc_report::register_id( _SC_ID_CONTEXT_BEGIN_FAILED,
			    "context begin failed" );
    sc_report::register_id( _SC_ID_CONTEXT_END_FAILED,
			    "context end failed" );

    return 42;
}


static int forty_two = initialize();


// Taf!
