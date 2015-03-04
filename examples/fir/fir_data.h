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
 
    fir_data.h -- 
 
    Original Author: Rocco Jonack. Synopsys, Inc. (rocco@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
    
 
******************************************************************************/

SC_MODULE(fir_data) {
   
  sc_in<bool>      reset;
  sc_in<unsigned>  state_out;
  sc_in<int>       sample;
  sc_out<int>      result;
  sc_out<bool>     output_data_ready;
  
  sc_int<19> acc;
  sc_int<8> shift[16];
  sc_int<9> coefs[16];

  SC_CTOR(fir_data)
    { 
      SC_METHOD(entry);
      sensitive(reset);
      sensitive(state_out);
      sensitive(sample);
#include "fir_const.h"
    };
  void entry();
};

