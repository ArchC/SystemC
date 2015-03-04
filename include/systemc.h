/******************************************************************************
    Copyright (c) 1996-2001 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file excep
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    systemc.h - Top-level include file for the SystemC library

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: 
    - Added the macros to create processes.

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SYSTEMC_H
#define SYSTEMC_H

#ifndef __SCENIC_COMPILER__  

// all windows 32-bit compilers should define WIN32
#if defined(__BCPLUSPLUS__)
#define WIN32
#endif
// WIN64 is reserved for the future 64-bit OS
#if defined(WIN64)
#error WIN64 keyword is reserved for 64-bit Windows
#endif

#include <stdlib.h>
#include <assert.h>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_bvrep.h"
#include "sc_bit.h"
#include "sc_ver.h"
#include "sc_macros.h"
#include "sc_signal.h"
#include "sc_signal_sc_bit.h"
#include "sc_lambda.h"
#include "sc_module_name.h"
#include "sc_sync.h"
#include "sc_async.h"
#include "sc_aproc.h"
#include "sc_clock.h"
#include "sc_simcontext.h"
#include "sc_polarity.h"
#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
#include "sc_2d.h"
#include "sc_except.h"
#include "sc_old.h"
#include "sc_dump.h"
#include "sc_resolved.h"
#include "sc_reslv_vector.h"
#include "sc_channel_array.h"
#include "sc_trace.h"
#include "sc_vcd_trace.h"
#include "sc_wif_trace.h"
#include "sc_isdb_trace.h"
#include "numeric_bit/numeric_bit.h"
#include "sc_externs.h"
#include "sc_port.h"
#include "sc_port_sc_bit.h"
#include "sc_port_clk.h"
#include "sc_bv.h"
#include "sc_lv.h"
#include "sc_signal_rv.h"
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv;
using sc_bv_ns::sc_lv_base;

#else

#include <stdio.h>
typedef sc_logic                  std_ulogic;
typedef sc_logic_vector           std_ulogic_vector;
typedef sc_signal_logic_vector    signal_std_ulogic_vector;
typedef sc_bool_vector            bool_vector;
typedef sc_signal_bool_vector     signal_bool_vector;

/* These macros get around the problem of undefined symbol (the handle)
   in the case sc_sync_tprocess (&c.) were treated as a function
   call.  Here the real function call is sc_sync_tprocess_SC, a
   SC-builtin symbol.  NEVER CHANGE THESE MACROS!! */

#define sc_sync_tprocess(handle, string_name, class_name, func, clock_edge) \
        sc_proc_handle handle; \
        handle = sc_sync_tprocess_SC(string_name, (class_name*) 0, func, clock_edge)

#define sc_async_tprocess(handle, string_name, class_name, func) \
        sc_proc_handle handle; \
        handle = sc_async_tprocess_SC(string_name, (class_name*) 0, func)

#define sc_async_fprocess(handle, string_name, class_name, func) \
        sc_proc_handle handle; \
        handle = sc_async_fprocess_SC(string_name, (class_name*) 0, func)

#define sc_set_stack_size(x, y) /* ignore sc_set_stack_size (for now at least) */

#define SC_USE_MEMBER_FUNC_PTR  /* yikes! */
#endif  // __SCENIC_COMPILER__


// To further simplify the syntax, consider the following.

#define SC_MODULE(user_module_name) \
    struct user_module_name : sc_module

#ifdef SC_USE_MEMBER_FUNC_PTR
#define SC_CTOR(user_module_name) \
    typedef user_module_name SC_CURRENT_USER_MODULE; \
    user_module_name(sc_module_name)
#else
#define SC_CTOR(user_module_name) \
    typedef user_module_name SC_CURRENT_USER_MODULE; \
    sc_vc6_process_helper_class<user_module_name> sc_vc6_process_helper; \
    user_module_name(sc_module_name)
#endif

// Equivalence:
//   SC_CTHREAD = SC_SYNC  = sc_sync_tprocess 
//   SC_THREAD  = SC_APROC = sc_async_tprocess
//   SC_METHOD  = SC_ASYNC = sc_async_fprocess 

#define SC_SYNC(func, clock_edge) \
    sc_sync_tprocess(func ## _handle, #func, SC_CURRENT_USER_MODULE, func, clock_edge)

#define SC_APROC(func) \
    sc_async_tprocess(func ## _handle, #func, SC_CURRENT_USER_MODULE, func)

#define SC_ASYNC(func) \
    sc_async_fprocess(func ## _handle, #func, SC_CURRENT_USER_MODULE, func)

#define SC_CTHREAD(func, clock_edge)   SC_SYNC(func, clock_edge)
#define SC_THREAD(func)                SC_APROC(func)
#define SC_METHOD(func)                SC_ASYNC(func)

// MSVC6.0 for() scope bug
#if defined(_MSC_VER) || defined(__HP_aCC)
#define for if(false);else for
#endif

// HPUX 10.20 aCC namespace bug: Koenig lookup isn't implemented
// even for operators
#if defined(__HP_aCC)
using namespace sc_bv_ns;
#endif

#endif
