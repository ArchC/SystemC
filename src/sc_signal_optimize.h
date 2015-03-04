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

    sc_signal_optimize.h - Optimize the signal->async/aproc propagation
                           functions.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_SIGNAL_OPTIMIZE_H
#define SC_SIGNAL_OPTIMIZE_H

/* This header is internal to SystemC library -- do not export (unless you
   want to lengthen your compilation time). */

class sc_signal_base;
class sc_signal_edgy;

class sc_signal_optimize {
public:
    static void add_asyncs_too(sc_signal_base*);
    static void add_aprocs_too(sc_signal_base*);
    static void rem_aprocs_too(sc_signal_base*);

    static void add_asyncs_too_neg(sc_signal_edgy*);
    static void add_aprocs_too_neg(sc_signal_edgy*);
    static void rem_aprocs_too_neg( sc_signal_edgy*);

    static void bool_update_prop_none(sc_signal_base*);
    static void bool_update_prop_async(sc_signal_base*);
    static void bool_update_prop_aproc(sc_signal_base*);
    static void bool_update_prop_both(sc_signal_base*);
    static void bool_update_prop_async_neg(sc_signal_base*);
    static void bool_update_prop_async_neg_async(sc_signal_base*);
    static void bool_update_prop_async_neg_aproc(sc_signal_base*);
    static void bool_update_prop_async_neg_both(sc_signal_base*);
    static void bool_update_prop_aproc_neg(sc_signal_base*);
    static void bool_update_prop_aproc_neg_async(sc_signal_base*);
    static void bool_update_prop_aproc_neg_aproc(sc_signal_base*);
    static void bool_update_prop_aproc_neg_both(sc_signal_base*);
    static void bool_update_prop_both_neg(sc_signal_base*);
    static void bool_update_prop_both_neg_async(sc_signal_base*);
    static void bool_update_prop_both_neg_aproc(sc_signal_base*);
    static void bool_update_prop_both_neg_both(sc_signal_base*);

    static void sc_bit_update_prop_none(sc_signal_base*);
    static void sc_bit_update_prop_async(sc_signal_base*);
    static void sc_bit_update_prop_aproc(sc_signal_base*);
    static void sc_bit_update_prop_both(sc_signal_base*);
    static void sc_bit_update_prop_async_neg(sc_signal_base*);
    static void sc_bit_update_prop_async_neg_async(sc_signal_base*);
    static void sc_bit_update_prop_async_neg_aproc(sc_signal_base*);
    static void sc_bit_update_prop_async_neg_both(sc_signal_base*);
    static void sc_bit_update_prop_aproc_neg(sc_signal_base*);
    static void sc_bit_update_prop_aproc_neg_async(sc_signal_base*);
    static void sc_bit_update_prop_aproc_neg_aproc(sc_signal_base*);
    static void sc_bit_update_prop_aproc_neg_both(sc_signal_base*);
    static void sc_bit_update_prop_both_neg(sc_signal_base*);
    static void sc_bit_update_prop_both_neg_async(sc_signal_base*);
    static void sc_bit_update_prop_both_neg_aproc(sc_signal_base*);
    static void sc_bit_update_prop_both_neg_both(sc_signal_base*);

    static void unsigned_update_prop_none(sc_signal_base*);
    static void unsigned_update_prop_async(sc_signal_base*);
    static void unsigned_update_prop_aproc(sc_signal_base*);
    static void unsigned_update_prop_both(sc_signal_base*);

    static void sc_logic_update_prop_none(sc_signal_base*);
    static void sc_logic_update_prop_async(sc_signal_base*);
    static void sc_logic_update_prop_aproc(sc_signal_base*);
    static void sc_logic_update_prop_both(sc_signal_base*);

    static void sc_clock_update(sc_signal_base*);
    static void optimize_clock(sc_clock* clk);
};

#endif
