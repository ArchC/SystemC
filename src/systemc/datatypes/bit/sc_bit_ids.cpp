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

  sc_bit_ids.cpp -- Report ids for the datatypes/bit code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/datatypes/bit/sc_bit_ids.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/bit)
//
//  Report ids in the range of 200-299.
// ----------------------------------------------------------------------------

const int SC_ID_LENGTH_MISMATCH_              = 200;
const int SC_ID_INCOMPATIBLE_TYPES_           = 201;
const int SC_ID_CANNOT_CONVERT_               = 202;
const int SC_ID_INCOMPATIBLE_VECTORS_         = 203;
const int SC_ID_VALUE_NOT_VALID_              = 204;
const int SC_ID_ZERO_LENGTH_                  = 205;
const int SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_  = 206;
const int SC_ID_SC_BV_CANNOT_CONTAIN_X_AND_Z_ = 207;
const int SC_ID_VECTOR_TOO_LONG_              = 208;
const int SC_ID_VECTOR_TOO_SHORT_             = 209;
const int SC_ID_WRONG_VALUE_                  = 210;
const int SC_ID_LOGIC_Z_TO_BOOL_              = 211;
const int SC_ID_LOGIC_X_TO_BOOL_              = 212;


// ----------------------------------------------------------------------------
//  Register report ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    sc_report::register_id( SC_ID_LENGTH_MISMATCH_,
			    "length mismatch in bit/logic vector assignment" );
    sc_report::register_id( SC_ID_INCOMPATIBLE_TYPES_,
			    "incompatible types" );
    sc_report::register_id( SC_ID_CANNOT_CONVERT_,
			    "cannot perform conversion" );
    sc_report::register_id( SC_ID_INCOMPATIBLE_VECTORS_,
			    "incompatible vectors" );
    sc_report::register_id( SC_ID_VALUE_NOT_VALID_,
			    "value is not valid" );
    sc_report::register_id( SC_ID_ZERO_LENGTH_,
			    "zero length" );
    sc_report::register_id( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_,
			    "vector contains 4-value logic" );
    sc_report::register_id( SC_ID_SC_BV_CANNOT_CONTAIN_X_AND_Z_,
			    "sc_bv cannot contain values X and Z" );
    sc_report::register_id( SC_ID_VECTOR_TOO_LONG_,
			    "vector is too long: truncated" );
    sc_report::register_id( SC_ID_VECTOR_TOO_SHORT_,
			    "vector is too short: 0-padded" );
    sc_report::register_id( SC_ID_WRONG_VALUE_,
			    "wrong value" );
    sc_report::register_id( SC_ID_LOGIC_Z_TO_BOOL_,
			    "sc_logic value 'Z' cannot be converted to bool" );
    sc_report::register_id( SC_ID_LOGIC_X_TO_BOOL_,
			    "sc_logic value 'X' cannot be converted to bool" );

    return 42;
}


static int forty_two = initialize();


// Taf!
