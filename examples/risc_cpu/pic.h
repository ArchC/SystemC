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
 
    pic.h -- Programmable Interrupt Unit 
 
    Original Author: Martin Wang. Synopsys, Inc. (mwang@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
 
 
******************************************************************************/

 

struct pic : sc_module { 
  	sc_in<bool>  		ireq0; 		// interrupt request 0   	
  	sc_in<bool>  		ireq1;    	// interrupt request 1
  	sc_in<bool>  		ireq2;    	// interrupt request 2
  	sc_in<bool>  		ireq3;    	// interrupt request 3
  	sc_in<bool>  		cs;    		// chip select
  	sc_in<bool>  		rd_wr;    	// read or write
  	sc_in<bool>  		intack_cpu;    	// interrupt acknowledge from CPU
  	sc_out<bool>  		intreq;         // interrupt request to CPU		
  	sc_out<bool>  		intack;         // interrupt acknowledge to devices		
  	sc_out<unsigned>        vectno;         // vector number		


  //Constructor 
  SC_CTOR(pic) {
      SC_METHOD(entry);
      sensitive << ireq0 << ireq1 << ireq2 << ireq3 ;
  }


  // Process functionality in member function below
  void entry();
};


