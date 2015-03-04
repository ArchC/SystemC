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

  sc_communication_ids.h -- Report ids for the communication code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_COMMUNICATION_IDS_H
#define SC_COMMUNICATION_IDS_H


#include "systemc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (communication)
//
//  Report ids in the range of 100-199.
// ----------------------------------------------------------------------------

extern const int SC_ID_PORT_OUTSIDE_MODULE_;
extern const int SC_ID_CLOCK_PERIOD_ZERO_;
extern const int SC_ID_CLOCK_HIGH_TIME_ZERO_;
extern const int SC_ID_CLOCK_LOW_TIME_ZERO_;
extern const int SC_ID_MORE_THAN_ONE_FIFO_READER_;
extern const int SC_ID_MORE_THAN_ONE_FIFO_WRITER_;
extern const int SC_ID_INVALID_FIFO_SIZE_;
extern const int SC_ID_BIND_IF_TO_PORT_;
extern const int SC_ID_BIND_PORT_TO_PORT_;
extern const int SC_ID_COMPLETE_BINDING_;
extern const int SC_ID_INSERT_PORT_;
extern const int SC_ID_REMOVE_PORT_;
extern const int SC_ID_GET_IF_;
extern const int SC_ID_INSERT_PRIM_CHANNEL_;
extern const int SC_ID_REMOVE_PRIM_CHANNEL_;
extern const int SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_;
extern const int SC_ID_NO_DEFAULT_EVENT_;
extern const int SC_ID_RESOLVED_PORT_NOT_BOUND_;
extern const int SC_ID_FIND_EVENT_;
extern const int SC_ID_INVALID_SEMAPHORE_VALUE_;


#endif

// Taf!
