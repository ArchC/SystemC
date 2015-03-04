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

  sc_communication_ids.cpp -- Report ids for the communication code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/communication/sc_communication_ids.h"


// ----------------------------------------------------------------------------
//  Report ids (communication)
//
//  Report ids in the range of 100-199.
// ----------------------------------------------------------------------------

const int SC_ID_PORT_OUTSIDE_MODULE_         = 100;
const int SC_ID_CLOCK_PERIOD_ZERO_           = 101;
const int SC_ID_CLOCK_HIGH_TIME_ZERO_        = 102;
const int SC_ID_CLOCK_LOW_TIME_ZERO_         = 103;
const int SC_ID_MORE_THAN_ONE_FIFO_READER_   = 104;
const int SC_ID_MORE_THAN_ONE_FIFO_WRITER_   = 105;
const int SC_ID_INVALID_FIFO_SIZE_           = 106;
const int SC_ID_BIND_IF_TO_PORT_             = 107;
const int SC_ID_BIND_PORT_TO_PORT_           = 108;
const int SC_ID_COMPLETE_BINDING_            = 109;
const int SC_ID_INSERT_PORT_                 = 110;
const int SC_ID_REMOVE_PORT_                 = 111;
const int SC_ID_GET_IF_                      = 112;
const int SC_ID_INSERT_PRIM_CHANNEL_         = 113;
const int SC_ID_REMOVE_PRIM_CHANNEL_         = 114;
const int SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_ = 115;
const int SC_ID_NO_DEFAULT_EVENT_            = 116;
const int SC_ID_RESOLVED_PORT_NOT_BOUND_     = 117;
const int SC_ID_FIND_EVENT_                  = 118;
const int SC_ID_INVALID_SEMAPHORE_VALUE_     = 119;


// ----------------------------------------------------------------------------
//  Register report ids and messages.
// ----------------------------------------------------------------------------

static
int
initialize()
{
    sc_report::register_id( SC_ID_PORT_OUTSIDE_MODULE_,
			    "port specified outside of module" );
    sc_report::register_id( SC_ID_CLOCK_PERIOD_ZERO_,
			    "sc_clock period is zero" );
    sc_report::register_id( SC_ID_CLOCK_HIGH_TIME_ZERO_,
			    "sc_clock high time is zero" );
    sc_report::register_id( SC_ID_CLOCK_LOW_TIME_ZERO_,
			    "sc_clock low time is zero" );
    sc_report::register_id( SC_ID_MORE_THAN_ONE_FIFO_READER_,
			    "sc_fifo<T> cannot have more than one reader" );
    sc_report::register_id( SC_ID_MORE_THAN_ONE_FIFO_WRITER_,
			    "sc_fifo<T> cannot have more than one writer" );
    sc_report::register_id( SC_ID_INVALID_FIFO_SIZE_,
			    "sc_fifo<T> must have a size of at least 1" );
    sc_report::register_id( SC_ID_BIND_IF_TO_PORT_,
			    "bind interface to port failed" );
    sc_report::register_id( SC_ID_BIND_PORT_TO_PORT_,
			    "bind parent port to port failed" );
    sc_report::register_id( SC_ID_COMPLETE_BINDING_,
			    "complete binding failed" );
    sc_report::register_id( SC_ID_INSERT_PORT_,
			    "insert port failed" );
    sc_report::register_id( SC_ID_REMOVE_PORT_,
			    "remove port failed" );
    sc_report::register_id( SC_ID_GET_IF_,
			    "get interface failed" );
    sc_report::register_id( SC_ID_INSERT_PRIM_CHANNEL_,
			    "insert primitive channel failed" );
    sc_report::register_id( SC_ID_REMOVE_PRIM_CHANNEL_,
			    "remove primitive channel failed" );
    sc_report::register_id( SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_,
			    "sc_signal<T> cannot have more than one driver" );
    sc_report::register_id( SC_ID_NO_DEFAULT_EVENT_,
			    "channel doesn't have a default event" );
    sc_report::register_id( SC_ID_RESOLVED_PORT_NOT_BOUND_,
			    "resolved port not bound to resolved signal" );
    sc_report::register_id( SC_ID_FIND_EVENT_,
			    "find event failed" );
    sc_report::register_id( SC_ID_INVALID_SEMAPHORE_VALUE_,
			    "sc_semaphore requires an initial value >= 0" );

    return 42;
}


static int forty_two = initialize();


// Taf!
