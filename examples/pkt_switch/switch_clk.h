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

    switch_clk.h - This is the interface file for the synchronous process 
                   "switch_clk"

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/
struct switch_clk: sc_module {
      sc_out<bool>   switch_cntrl;
      sc_in_clk CLK;

      SC_CTOR(switch_clk)
        {
           SC_METHOD(entry);
           sensitive_pos(CLK); 
         }
     void entry();
};
