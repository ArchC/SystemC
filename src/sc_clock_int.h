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

    sc_clock_int.h -- Internal include file for sc_clock.
    
    *** DO NOT EXPORT THIS INCLUDE FILE ***

    Requires sc_process.h (for the definition or
    sc_sync_process_handle).  This include file effectively hides some
    internal functionalities of sc_clock.h from the outside world.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_CLOCK_INT_H
#define SC_CLOCK_INT_H

class sc_clock_helpers {
public:
    /* Put the handle at the back */
    static void bind_sync_to_clock_edge_back(sc_clock_edge*,
                                             sc_sync_process_handle handle);

    /* Put the handle at the front */
    static void bind_sync_to_clock_edge_front(sc_clock_edge*,
                                              sc_sync_process_handle handle);

    static int number_of_syncs(const sc_clock_edge*);

    /* Toggles the clock that owns the given edge */
    static void toggle_clock_signal(sc_clock_edge* p_edge)
    {
        p_edge->next_time += p_edge->per;
        p_edge->clk.write(PHASE_POS == p_edge->phase());
    }

    static void prepare_for_simulation(sc_clock_edge*, int num_signals);
    static void remove_from_runnable(sc_clock_edge*, sc_sync_process_handle);

    /* This function is used in sc_simcontext to do automatic
       clock generation when there's only one clock. */
    static void clock_write(sc_clock* p_clk, bool nv)
    {
        p_clk->new_value = nv;
        p_clk->submit_update();
    }
};

#endif
