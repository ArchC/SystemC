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

  sc_int_ids.cpp -- Report ids for the datatypes/int code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/datatypes/int/sc_int_ids.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/int)
//
//  Report ids in the range of 400-499.
// ----------------------------------------------------------------------------

const int SC_ID_INIT_FAILED_       = 400;
const int SC_ID_ASSIGNMENT_FAILED_ = 401;
const int SC_ID_OPERATION_FAILED_  = 402;
const int SC_ID_CONVERSION_FAILED_ = 403;


// ----------------------------------------------------------------------------
//  Register report ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    sc_report::register_id( SC_ID_INIT_FAILED_,
			    "initialization failed" );
    sc_report::register_id( SC_ID_ASSIGNMENT_FAILED_,
			    "assignment failed" );
    sc_report::register_id( SC_ID_OPERATION_FAILED_,
			    "operation failed" );
    sc_report::register_id( SC_ID_CONVERSION_FAILED_,
			    "conversion failed" );

    return 42;
}


static int forty_two = initialize();


// Taf!
