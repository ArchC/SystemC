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

    sc_simcontext_int.h -- For inline definitions of some utility
    functions.DO NOT EXPORT THIS INCLUDE FILE. Include this file after
    "sc_process_int.h" so that we can get the base class right.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_SIMCONTEXT_INT_H
#define SC_SIMCONTEXT_INT_H

inline void
sc_simcontext::reset_curr_proc()
{
    curr_proc_info.process_handle = 0;
    curr_proc_info.kind           = SC_CURR_PROC_NONE;
}

inline void
sc_simcontext::set_curr_proc(sc_async_process_handle h)
{
    curr_proc_info.process_handle = h;
    curr_proc_info.kind           = SC_CURR_PROC_ASYNC;
}

inline void
sc_simcontext::set_curr_proc(sc_aproc_process_handle h)
{
    curr_proc_info.process_handle = h;
    curr_proc_info.kind           = SC_CURR_PROC_APROC;
}

inline void
sc_simcontext::set_curr_proc(sc_sync_process_handle h)
{
    curr_proc_info.process_handle = h;
    curr_proc_info.kind           = SC_CURR_PROC_SYNC;
}

extern void sc_defunct_process_function(sc_module*);

extern void watching_before_simulation(const sc_lambda_ptr&, sc_simcontext*);
extern void watching_during_simulation(const sc_lambda_ptr&, sc_simcontext*);

#endif
