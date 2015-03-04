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
 
    display.cpp -- 
 
    Original Author: Rocco Jonack. Synopsys, Inc. (rocco@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
    
 
******************************************************************************/

#include <systemc.h>
#include "display.h"

void display::entry(){

  //  Reading Data when valid if high
  tmp1 = result.read();
  cout << "Display : " << tmp1 << " " 
       << " at time " << sc_time_stamp() << endl;
  i++;
  if(i == 24) {
    cout << "Simulation of " << i << " items finished" 
	 << " at time " << sc_time_stamp() << endl;
    sc_stop();
  };
}
// EOF
