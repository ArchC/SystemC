/******************************************************************************
    Copyright (c) 1996-2001 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    sc_macros.h -- miscellaneous definitions that are needed by the headers

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_MACROS_H
#define SC_MACROS_H

#if defined(__HP_aCC)
#include <algorithm>
#else
#if !defined(_MSC_VER)
// there are problems with implementation of the standard library in VC6
#include <algorithm>
using std::min;
using std::max;
#else
#include <Windows.h>
#endif
#endif

#if defined(__GNUC__) && defined(USE_RTTI)
#define HAVE_CAST_OPERATORS
#endif

#if defined(__GNUC__) || defined(WIN32) || defined(__SUNPRO_CC) || defined(__HP_aCC)
#define USE_SC_SIGNAL_ARRAY_GNU_HACK
#endif


#if defined(__GNUC__)
// 10.3.5 - Some compilers (e.g. K&A C++) do not support the
// covariant return types
#define ANSI_VIRTUAL_RETURN_INHERITED_TYPE
#endif

/* Note that sc_get_curr_simcontext() may also be a member
   of sc_module.  The idea is that if we are inside an sc_module,
   then its associated simcontext should always be the current
   simcontext. */
#define W_BEGIN \
do { \
    sc_watch __aux_watch(sc_get_curr_simcontext());

#define W_DO try { \
      __watching_first(__aux_watch.sync_handle);

#define W_ESCAPE \
    } \
    catch (int sc_level) { \
        __sanitycheck_watchlists(__aux_watch.sync_handle); \
        if (sc_level < __watch_level(__aux_watch.sync_handle)) { \
            throw sc_level; \
        } \

#define W_END \
    } \
} while (false);

/* These help debugging -- user can find out where each process is stopped at. */

#define WAIT()             sc_set_location(__FILE__,__LINE__); wait()
#define WAITN(n)           sc_set_location(__FILE__,__LINE__); wait(n)
#define NEXT()             sc_set_location(__FILE__,__LINE__); wait()
#define WAIT_UNTIL(lambda) sc_set_location(__FILE__,__LINE__); wait_until(lambda)

#endif
