/******************************************************************************
    Copyright (c) 1996-2001 Synopsys, Inc.    ALL RIGHTS RESERVED
 
  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC(TM) Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
 
******************************************************************************/
/******************************************************************************
 
    fir_fsm.h -- 
 
    Original Author: Rocco Jonack. Synopsys, Inc. (rocco@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
    
******************************************************************************/

SC_MODULE(fir_fsm) {

  sc_in<bool>      clock;
  sc_in<bool>      reset;
  sc_in<bool>      in_valid;
  sc_out<unsigned> state_out;

  // defining the states of the ste machine
  enum {reset_s, first_s, second_s, third_s, output_s} state;

  SC_CTOR(fir_fsm) 
    { 
      SC_METHOD(entry);
      sensitive_pos(clock); 
    };
  void entry();
};

