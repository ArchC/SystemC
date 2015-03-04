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

    sc_measure.cpp -- measuring context switching time.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#if !defined(WIN32) && !defined(i386)

#include <stdio.h>

#include <sys/times.h>
#include <limits.h>
#include "sc_measure.h"

static struct tms context_switch_tms;
static double last_time_stamp = 0.0;
static double total_time = 0.0;
static int num_context_switches = 0;

void
begin_context_switch_real()
{
    times(&context_switch_tms);
    last_time_stamp = (double) context_switch_tms.tms_utime / CLK_TCK;
    num_context_switches++;
}

void
end_context_switch_real()
{
    times(&context_switch_tms);
    total_time += ((double) context_switch_tms.tms_utime / CLK_TCK) - last_time_stamp;
}

void
report_context_switch_time_real()
{
    printf( "Total number of context switches   = %d\n", num_context_switches );
    printf( "Total time spent in context switch = %3.2f\n", total_time );
}

#else
/* Have to incorporate timing code for WINNT */
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "sc_measure.h"

static double last_time_stamp = 0.0;
static double total_time = 0.0;
static int num_context_switches = 0;

void
begin_context_switch_real()
{ 
  last_time_stamp = (double)clock()/CLOCKS_PER_SEC;
  num_context_switches++;
}

void
end_context_switch_real()
{ 
  total_time += ( (double)clock()/CLOCKS_PER_SEC ) - last_time_stamp;
}

void
report_context_switch_time_real()
{
   printf( "Total number of context switches   = %d\n", num_context_switches );
   printf( "Total time spent in context switch = %3.2f\n", total_time );
}

#endif

