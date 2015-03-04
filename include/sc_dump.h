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

    sc_dump.h - Provides dumping in VCD format.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_DUMP_H
#define SC_DUMP_H

class sc_object;
class sc_clock_edge;
class sc_trace_file;
class vcd_trace_file;
class wif_trace_file;

// OLD FUNCTIONS - needed for backward compatibility. Need to go in the long run.
extern void sc_dumpfile(const char* fname);
extern void sc_dumpclose();

// These need to stay in the long run
extern void sc_dumpall();
extern void sc_dumpall(sc_trace_file *f);
extern void sc_dumpvar(sc_object& obj);
extern void sc_dumpvar(sc_object& obj, sc_trace_file *f);
extern void sc_dumpvars(sc_object& obj);
extern void sc_dumpvars(sc_object& obj, sc_trace_file *f);
extern void sc_dumpvars(const char* objs_string);
extern void sc_dumpvars(const char *objs_string, sc_trace_file *f);
extern sc_trace_file* the_dumpfile;

#endif
