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

    switch.h - This is the interface file for the asynchronous process "switch"

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


struct mcast_pkt_switch : sc_module {

    sc_in<bool>  switch_cntrl;
    sc_in<pkt>  in0;
    sc_in<pkt>  in1;
    sc_in<pkt>  in2;
    sc_in<pkt>  in3;

    sc_out<pkt>  out0;
    sc_out<pkt>  out1;
    sc_out<pkt>  out2;
    sc_out<pkt>  out3;   

    SC_CTOR(mcast_pkt_switch) 
     {
      SC_THREAD(entry);
      sensitive << in0;
      sensitive << in1;
      sensitive << in2;
      sensitive << in3;
      sensitive_pos << switch_cntrl;
    }  

  void entry();  
 
};
