/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2001 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  systemc.h - Top-level include file for the SystemC library.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SYSTEMC_H
#define SYSTEMC_H


// all windows 32-bit compilers should define WIN32
#if defined( _MSC_VER )
#if !defined( WIN32 )
#define WIN32
#endif
#endif

// WIN64 is reserved for the future 64-bit OS
#if defined( WIN64 )
#error WIN64 keyword is reserved for 64-bit Windows
#endif

#if defined( __HP_aCC )
#define _INCLUDE_LONGLONG
#endif

#include <stdlib.h>
#include <assert.h>

#include "systemc/kernel/sc_externs.h"
#include "systemc/kernel/sc_module.h"
#include "systemc/kernel/sc_process_b.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/kernel/sc_ver.h"

#include "systemc/communication/sc_buffer.h"
#include "systemc/communication/sc_clock.h"
#include "systemc/communication/sc_clock_ports.h"
#include "systemc/communication/sc_fifo.h"
#include "systemc/communication/sc_fifo_ports.h"
#include "systemc/communication/sc_mutex.h"
#include "systemc/communication/sc_semaphore.h"
#include "systemc/communication/sc_signal.h"
#include "systemc/communication/sc_signal_ports.h"
#include "systemc/communication/sc_signal_resolved.h"
#include "systemc/communication/sc_signal_resolved_ports.h"
#include "systemc/communication/sc_signal_rv.h"
#include "systemc/communication/sc_signal_rv_ports.h"

#include "systemc/datatypes/bit/sc_bit.h"
#include "systemc/datatypes/bit/sc_logic.h"
#include "systemc/datatypes/bit/sc_bv.h"
#include "systemc/datatypes/bit/sc_lv.h"

using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv;
using sc_bv_ns::sc_lv_base;

#include "systemc/datatypes/int/sc_bigint.h"
#include "systemc/datatypes/int/sc_biguint.h"
#include "systemc/datatypes/int/sc_int.h"
#include "systemc/datatypes/int/sc_uint.h"

#ifdef SC_INCLUDE_FX
#include "systemc/datatypes/fx/fx.h"
#endif

#include "systemc/tracing/sc_vcd_trace.h"
#include "systemc/tracing/sc_wif_trace.h"
#include "systemc/tracing/sc_isdb_trace.h"

#include "systemc/utils/sc_iostream.h"


// MSVC6.0 for() scope bug
#if defined( _MSC_VER )
#define for if(false);else for
#endif

#ifdef __EDG__
#pragma hdrstop
#endif


#endif
