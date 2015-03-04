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

/*****************************************************************************
 * Filename : display.h                                                      
 *   
 * Description : The is the interface file for the display module. 
 *
 * Original Author : Amit Rao (arao@synopsys.com)
 *
 *****************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H 

struct display : sc_module {
    sc_in<double> in;        // input port 1
    sc_in<bool>   clk;       // clock

    void print();            // method to display input port values
    
    //Constructor
    SC_CTOR( display ) {
	SC_METHOD( print );    // declare print as SC_METHOD and 
	sensitive_pos << clk;  // make it sensitive to positive clock edge
    }
	
};

#endif
