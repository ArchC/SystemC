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

  sc_fx_ids.cpp -- Report ids for the datatypes/fx code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/datatypes/fx/sc_fx_ids.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/fx)
//
//  Report ids in the range of 300-399.
// ----------------------------------------------------------------------------

const int SC_ID_INVALID_WL_           = 300;
const int SC_ID_INVALID_N_BITS_       = 301;
const int SC_ID_INVALID_DIV_WL_       = 302;
const int SC_ID_INVALID_CTE_WL_       = 303;
const int SC_ID_INVALID_MAX_WL_       = 304;
const int SC_ID_INVALID_FX_VALUE_     = 305;
const int SC_ID_INVALID_O_MODE_       = 306;
const int SC_ID_OUT_OF_RANGE_         = 307;
const int SC_ID_CONTEXT_BEGIN_FAILED_ = 308;
const int SC_ID_CONTEXT_END_FAILED_   = 309;
const int SC_ID_WRAP_SM_NOT_DEFINED_  = 310;


// ----------------------------------------------------------------------------
//  Register report ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    sc_report::register_id( SC_ID_INVALID_WL_,
                            "total wordlength <= 0 is not valid" );
    sc_report::register_id( SC_ID_INVALID_N_BITS_,
                            "number of bits < 0 is not valid" );
    sc_report::register_id( SC_ID_INVALID_DIV_WL_,
                            "division wordlength <= 0 is not valid" );
    sc_report::register_id( SC_ID_INVALID_CTE_WL_,
                            "constant wordlength <= 0 is not valid" );
    sc_report::register_id( SC_ID_INVALID_MAX_WL_,
                            "maximum wordlength <= 0 and != -1 is not valid" );
    sc_report::register_id( SC_ID_INVALID_FX_VALUE_,
			    "invalid fixed-point value" );
    sc_report::register_id( SC_ID_INVALID_O_MODE_,
			    "invalid overflow mode" );
    sc_report::register_id( SC_ID_OUT_OF_RANGE_,
			    "index out of range" );
    sc_report::register_id( SC_ID_CONTEXT_BEGIN_FAILED_,
			    "context begin failed" );
    sc_report::register_id( SC_ID_CONTEXT_END_FAILED_,
			    "context end failed" );
    sc_report::register_id( SC_ID_WRAP_SM_NOT_DEFINED_,
			    "SC_WRAP_SM not defined for unsigned numbers" );

    return 42;
}


static int forty_two = initialize();


// Taf!
