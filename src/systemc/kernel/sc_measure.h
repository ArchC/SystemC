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

  sc_measure.h -- Measuring context switching time.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_MEASURE_H
#define SC_MEASURE_H

#ifdef MEASURE_CONTEXT_SWITCH

#define begin_context_switch()        begin_context_switch_real()
#define end_context_switch()          end_context_switch_real()
#define report_context_switch_time()  report_context_switch_time_real()

extern void begin_context_switch_real();
extern void end_context_switch_real();
extern void report_context_switch_time_real();

#else

#define begin_context_switch() /* empty */
#define end_context_switch() /* empty */
#define report_context_switch_time() /* empty */

#endif

#endif
