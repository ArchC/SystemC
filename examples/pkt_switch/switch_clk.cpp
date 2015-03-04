/***********************************************************************
   Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC(TM) Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at 
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

  *************************************************************************/

/******************************************************************************

    switch_clk.cpp - This is the implementation file for the synchronous process
                    "switch_clk"

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "systemc.h"
#include "switch_clk.h"

void switch_clk::entry()
 {
  static bool var_switch_cntrl = false;
    {
       switch_cntrl = var_switch_cntrl;
       var_switch_cntrl = !var_switch_cntrl;
     }
 }

