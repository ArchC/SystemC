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
 
    mmxu.h -- MMX-Like Execution Unit 
 
    Original Author: Martin Wang. Synopsys, Inc. (mwang@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
 
 
******************************************************************************/



struct mmxu : sc_module {
  sc_in<bool>   	       	mmx_valid;	// MMX unit enable
  sc_in<int>  	      		opcode;		// opcode
  sc_in<signed int>        	mmxa;		// operand A
  sc_in<signed int>        	mmxb;		// operand B
  sc_in<unsigned>        	dest;		// Destination register number
  sc_out<signed int>        	mmxdout;	// MMX output
  sc_out<bool>   	       	mmxout_valid;	// MMX output valid
  sc_out<unsigned>        	mmxdestout;	// destination number
  sc_in_clk 			CLK;


  // can make it asynchronous process to speed up simulation
  SC_CTOR(mmxu) {
      SC_CTHREAD(entry, CLK.pos());
  }
  
  void entry();
};

// EOF
