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

  sc_utils_ids.cpp -- Report ids for the utils code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/utils/sc_utils_ids.h"


// ----------------------------------------------------------------------------
//  Report ids (utils)
//
//  Report ids in the range of 800-899.
// ----------------------------------------------------------------------------

const int SC_ID_REGISTER_ID_FAILED_  = 800;
const int SC_ID_STRING_TOO_LONG_     = 801;
const int SC_ID_FRONT_ON_EMPTY_LIST_ = 802;
const int SC_ID_BACK_ON_EMPTY_LIST_  = 803;


// ----------------------------------------------------------------------------
//  Register report ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    // SC_ID_REGISTER_ID_FAILED_ is already registered (special case)

    sc_report::register_id( SC_ID_STRING_TOO_LONG_,
			    "string is too long" );
    sc_report::register_id( SC_ID_FRONT_ON_EMPTY_LIST_,
			    "attempt to take front() on an empty list" );
    sc_report::register_id( SC_ID_BACK_ON_EMPTY_LIST_,
			    "attempt to take back() on an empty list" );

    return 42;
}


static int forty_two = initialize();


// Taf!
