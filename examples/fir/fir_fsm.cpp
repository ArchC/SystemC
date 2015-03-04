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
 
    fir_fsm.cpp -- 
 
    Original Author: Rocco Jonack. Synopsys, Inc. (rocco@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
    
******************************************************************************/

#include <systemc.h>
#include "fir_fsm.h"

void fir_fsm::entry() {

  sc_uint<3> state_tmp;

  // reset behavior
  if(reset.read()==true) {
    state = reset_s;
  } else {
#ifdef DEBUG
    cout << "debug: " << state << " " << state_tmp << " " << reset.read() << endl;
#endif
    // main state machine 
    switch(state) {
    case reset_s:
      state = first_s;
      state_tmp = 0;
      state_out.write((unsigned)state_tmp);
      break;
    case first_s:
      if(in_valid.read()==true) {
	state = second_s;
      };
      state_tmp = 1;
      state_out.write((unsigned)state_tmp);
      break;
    case second_s:
      state = third_s;
      state_tmp = 2;
      state_out.write((unsigned)state_tmp);
      break;
    case third_s:
      state = output_s;
      state_tmp = 3;
      state_out.write((unsigned)state_tmp);
      break;
    default:
      state = first_s;
      state_tmp = 4;
      state_out.write((unsigned)state_tmp);
      break;
    }
  }
}
