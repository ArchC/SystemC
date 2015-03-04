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

     sc_trace.cpp - Functions for tracing signals and variables

     Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

/****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, the basics are identical to what was
   originally contributed by Infineon.  The contribution of Infineon
   in the development of this tracing technology is hereby
   acknowledged.

*****************************************************************************/

#include <stdarg.h>
#include <stdio.h>

#include "sc_iostream.h"
#include "sc_trace.h"
#include "sc_signal.h"

// Trace file common functions.

sc_trace_file::sc_trace_file()
{
  /* Intentionally blank */
}

void tprintf(sc_trace_file* tf,  const char* format, ...)
{
    static char buffer[4096];
    va_list ap;
    va_start(ap, format);
    (void) vsprintf(buffer, format, ap);
    va_end(ap);
    if (tf) tf->write_comment(buffer);
}

void sc_trace_file::space(int)
{
  /* Intentionally blank */
}

void sc_trace_file::delta_cycles(bool)
{
  /* Intentionally blank */
}

void sc_trace(sc_trace_file *tf, const sc_signal<char>& object, const sc_string& name, int width) 
{
    if (tf) tf->trace(*(object.get_data_ptr()), name, width);
}

void sc_trace(sc_trace_file *tf, const sc_signal<short>& object, const sc_string& name, int width) 
{
    if (tf) tf->trace(*(object.get_data_ptr()), name, width);
}

void sc_trace(sc_trace_file *tf, const sc_signal<int>& object, const sc_string& name, int width) 
{
    if (tf) tf->trace(*(object.get_data_ptr()), name, width);
}

void sc_trace(sc_trace_file *tf, const sc_signal<long>& object, const sc_string& name, int width) 
{
    if (tf) tf->trace(*(object.get_data_ptr()), name, width);
}

void sc_trace(sc_trace_file* /* not used */, const void* /* not used */, const sc_string& name)
{
    cerr << "Object " << name << " will not be traced\n";
}



void double_to_special_int64(double in, unsigned* high, unsigned* low)
{
    double invar = in;
    if(invar > 5e17) invar = 5e17; // Saturation limit
    if(invar < 0.0) invar = 0.0;
    invar += .5;
    *high = (unsigned)(invar / 1e9);
    double rest = invar - 1e9 * (*high);
    if(rest < 0) *low = 0;
    else *low = (unsigned)rest;
}

