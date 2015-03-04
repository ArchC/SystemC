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

  sc_kernel_ids.cpp -- Report ids for the kernel code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/kernel/sc_kernel_ids.h"


// ----------------------------------------------------------------------------
//  Report ids (kernel)
//
//  Report ids in the range of 500-599.
// ----------------------------------------------------------------------------

const int SC_ID_NO_BOOL_RETURNED_            = 500;
const int SC_ID_NO_INT_RETURNED_             = 501;
const int SC_ID_NO_SC_LOGIC_RETURNED_        = 502;
const int SC_ID_OPERAND_NOT_SC_LOGIC_        = 503;
const int SC_ID_OPERAND_NOT_BOOL_            = 504;
const int SC_ID_OBJECT_EXISTS_               = 505;
const int SC_ID_ILLEGAL_CHARACTERS_          = 506;
const int SC_ID_VC6_PROCESS_HELPER_          = 507;
const int SC_ID_VC6_MAX_PROCESSES_EXCEEDED_  = 508;
const int SC_ID_END_MODULE_NOT_CALLED_       = 509;
const int SC_ID_HIER_NAME_INCORRECT_         = 510;
const int SC_ID_SET_STACK_SIZE_              = 511;
const int SC_ID_SC_MODULE_NAME_USE_          = 512;
const int SC_ID_SC_MODULE_NAME_REQUIRED_     = 513;
const int SC_ID_SET_TIME_RESOLUTION_         = 514;
const int SC_ID_SET_DEFAULT_TIME_UNIT_       = 515;
const int SC_ID_DEFAULT_TIME_UNIT_CHANGED_   = 516;
const int SC_ID_WAIT_N_NOT_ALLOWED_          = 517;
const int SC_ID_WAIT_UNTIL_NOT_ALLOWED_      = 518;
const int SC_ID_WAIT_NOT_ALLOWED_            = 519;
const int SC_ID_NEXT_TRIGGER_NOT_ALLOWED_    = 520;
const int SC_ID_IMMEDIATE_NOTIFICATION_      = 521;
const int SC_ID_HALT_NOT_ALLOWED_            = 522;
const int SC_ID_WATCHING_NOT_ALLOWED_        = 523;
const int SC_ID_DONT_INITIALIZE_             = 524;
const int SC_ID_WAIT_N_INVALID_              = 525;
const int SC_ID_MAKE_SENSITIVE_              = 526;
const int SC_ID_MAKE_SENSITIVE_POS_          = 527;
const int SC_ID_MAKE_SENSITIVE_NEG_          = 528;
const int SC_ID_INSERT_MODULE_               = 529;
const int SC_ID_REMOVE_MODULE_               = 530;
const int SC_ID_NOTIFY_DELAYED_              = 531;
const int SC_ID_GEN_UNIQUE_NAME_             = 532;
const int SC_ID_MODULE_NAME_STACK_EMPTY_     = 533;


// ----------------------------------------------------------------------------
//  Register report ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    sc_report::register_id( SC_ID_NO_BOOL_RETURNED_,
			    "operator does not return boolean" );
    sc_report::register_id( SC_ID_NO_INT_RETURNED_,
			    "operator does not return int" );
    sc_report::register_id( SC_ID_NO_SC_LOGIC_RETURNED_,
			    "operator does not return sc_logic" );
    sc_report::register_id( SC_ID_OPERAND_NOT_SC_LOGIC_,
			    "operand is not sc_logic" );
    sc_report::register_id( SC_ID_OPERAND_NOT_BOOL_,
			    "operand is not bool" );
    sc_report::register_id( SC_ID_OBJECT_EXISTS_,
			    "object already exists" );
    sc_report::register_id( SC_ID_ILLEGAL_CHARACTERS_,
			    "illegal characters" );
    sc_report::register_id( SC_ID_VC6_PROCESS_HELPER_,
			    "internal error: sc_vc6_process_helper" );
    sc_report::register_id( SC_ID_VC6_MAX_PROCESSES_EXCEEDED_,
			    "maximum number of processes per module exceeded "
			    "(VC6)" );
    sc_report::register_id( SC_ID_END_MODULE_NOT_CALLED_,
			    "module construction not properly completed: did "
			    "you forget to add a sc_module_name parameter to "
			    "your module constructor?" );
    sc_report::register_id( SC_ID_HIER_NAME_INCORRECT_,
			    "hierarchical name as shown may be incorrect due "
			    "to previous errors" );
    sc_report::register_id( SC_ID_SET_STACK_SIZE_,
			    "set_stack_size() is only allowed for SC_THREADs "
			    "and SC_CTHREADs" );
    sc_report::register_id( SC_ID_SC_MODULE_NAME_USE_,
			    "incorrect use of sc_module_name" );
    sc_report::register_id( SC_ID_SC_MODULE_NAME_REQUIRED_,
			    "an sc_module_name parameter for your constructor "
			    "is required" );
    sc_report::register_id( SC_ID_SET_TIME_RESOLUTION_,
			    "set time resolution failed" );
    sc_report::register_id( SC_ID_SET_DEFAULT_TIME_UNIT_,
			    "set default time unit failed" );
    sc_report::register_id( SC_ID_DEFAULT_TIME_UNIT_CHANGED_,
			    "default time unit changed to time resolution" );
    sc_report::register_id( SC_ID_WAIT_N_NOT_ALLOWED_,
			    "wait(n) is only allowed in SC_CTHREADs" );
    sc_report::register_id( SC_ID_WAIT_UNTIL_NOT_ALLOWED_,
			    "wait_until() is only allowed in SC_CTHREADs" );
    sc_report::register_id( SC_ID_WAIT_NOT_ALLOWED_,
			    "wait() is only allowed in SC_THREADs and "
			    "SC_CTHREADs" );
    sc_report::register_id( SC_ID_NEXT_TRIGGER_NOT_ALLOWED_,
			    "next_trigger() is only allowed in SC_METHODs" );
    sc_report::register_id( SC_ID_IMMEDIATE_NOTIFICATION_,
			    "immediate notification is not allowed during the "
			    "update phase" );
    sc_report::register_id( SC_ID_HALT_NOT_ALLOWED_,
			    "halt() is only allowed in SC_CTHREADs" );
    sc_report::register_id( SC_ID_WATCHING_NOT_ALLOWED_,
			    "watching() is only allowed in SC_CTHREADs" );
    sc_report::register_id( SC_ID_DONT_INITIALIZE_,
			    "dont_initialize() has no effect for "
			    "SC_CTHREADs" );
    sc_report::register_id( SC_ID_WAIT_N_INVALID_,
			    "wait(n) is only valid for n > 0" );
    sc_report::register_id( SC_ID_MAKE_SENSITIVE_,
			    "make sensitive failed" );
    sc_report::register_id( SC_ID_MAKE_SENSITIVE_POS_,
			    "make sensitive pos failed" );
    sc_report::register_id( SC_ID_MAKE_SENSITIVE_NEG_,
			    "make sensitive neg failed" );
    sc_report::register_id( SC_ID_INSERT_MODULE_,
			    "insert module failed" );
    sc_report::register_id( SC_ID_REMOVE_MODULE_,
			    "remove module failed" );
    sc_report::register_id( SC_ID_NOTIFY_DELAYED_,
			    "notify_delayed() cannot be called on events "
			    "that have pending notifications" );
    sc_report::register_id( SC_ID_GEN_UNIQUE_NAME_,
			    "cannot generate unique name from null string" );
    sc_report::register_id( SC_ID_MODULE_NAME_STACK_EMPTY_,
			    "module name stack is empty: did you forget to "
			    "add a sc_module_name parameter to your module "
			    "constructor?" );

    return 42;
}


static int forty_two = initialize();


// Taf!
