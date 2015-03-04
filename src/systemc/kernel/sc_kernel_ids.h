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

  sc_kernel_ids.h -- Report ids for the kernel code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_KERNEL_IDS_H
#define SC_KERNEL_IDS_H


#include "systemc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (kernel)
//
//  Report ids in the range of 500-599.
// ----------------------------------------------------------------------------

extern const int SC_ID_NO_BOOL_RETURNED_;
extern const int SC_ID_NO_INT_RETURNED_;
extern const int SC_ID_NO_SC_LOGIC_RETURNED_;
extern const int SC_ID_OPERAND_NOT_SC_LOGIC_;
extern const int SC_ID_OPERAND_NOT_BOOL_;
extern const int SC_ID_OBJECT_EXISTS_;
extern const int SC_ID_ILLEGAL_CHARACTERS_;
extern const int SC_ID_VC6_PROCESS_HELPER_;
extern const int SC_ID_VC6_MAX_PROCESSES_EXCEEDED_;
extern const int SC_ID_END_MODULE_NOT_CALLED_;
extern const int SC_ID_HIER_NAME_INCORRECT_;
extern const int SC_ID_SET_STACK_SIZE_;
extern const int SC_ID_SC_MODULE_NAME_USE_;
extern const int SC_ID_SC_MODULE_NAME_REQUIRED_;
extern const int SC_ID_SET_TIME_RESOLUTION_;
extern const int SC_ID_SET_DEFAULT_TIME_UNIT_;
extern const int SC_ID_DEFAULT_TIME_UNIT_CHANGED_;
extern const int SC_ID_WAIT_N_NOT_ALLOWED_;
extern const int SC_ID_WAIT_UNTIL_NOT_ALLOWED_;
extern const int SC_ID_WAIT_NOT_ALLOWED_;
extern const int SC_ID_NEXT_TRIGGER_NOT_ALLOWED_;
extern const int SC_ID_IMMEDIATE_NOTIFICATION_;
extern const int SC_ID_HALT_NOT_ALLOWED_;
extern const int SC_ID_WATCHING_NOT_ALLOWED_;
extern const int SC_ID_DONT_INITIALIZE_;
extern const int SC_ID_WAIT_N_INVALID_;
extern const int SC_ID_MAKE_SENSITIVE_;
extern const int SC_ID_MAKE_SENSITIVE_POS_;
extern const int SC_ID_MAKE_SENSITIVE_NEG_;
extern const int SC_ID_INSERT_MODULE_;
extern const int SC_ID_REMOVE_MODULE_;
extern const int SC_ID_NOTIFY_DELAYED_;
extern const int SC_ID_GEN_UNIQUE_NAME_;
extern const int SC_ID_MODULE_NAME_STACK_EMPTY_;


#endif

// Taf!
