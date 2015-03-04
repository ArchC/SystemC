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

  sc_fxdefs.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/datatypes/fx/sc_fxdefs.h"


namespace sc_dt
{

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
        case SC_TC_:
            return sc_string( "SC_TC_" );
        case SC_US_:
            return sc_string( "SC_US_" );
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

} // namespace sc_dt


// Taf!
