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
 * Filename: main.cpp                                                  
 *   
 * Description : The is the top level file instantiating the modules and
 *               binding ports to signals 
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

#include "systemc.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "display.h"
#include "numgen.h"

#define NS * 1e-9

int sc_main(int ac, char *av[])
{
  //Signals
  sc_signal<double> in1;
  sc_signal<double> in2;
  sc_signal<double> sum;
  sc_signal<double> diff;
  sc_signal<double> prod;
  sc_signal<double> quot;
  sc_signal<double> powr;  
  //Clock
  sc_signal<bool>   clk;
  
  numgen N("numgen");               //instance of `numgen' module
  N(in1, in2, clk );                //Positional port binding

  
  stage1 S1("stage1");              //instance of `stage1' module
  //Named port binding
  S1.in1(in1);
  S1.in2(in2);
  S1.sum(sum);
  S1.diff(diff);
  S1.clk(clk);
  
  stage2 S2("stage2");              //instance of `stage2' module
  S2(sum, diff, prod, quot, clk );  //Positional port binding


  stage3 S3("stage3");              //instance of `stage3' module
  S3( prod, quot, powr, clk);       //Positional port binding

  
  display D("display");             //instance of `display' module
  D(powr, clk);                     //Positional port binding 

  sc_initialize();                  //Initialize simulation
  for(int i = 0; i < 50; i++){
      clk.write(1);
      sc_cycle( 10 NS );
      clk.write(0);
      sc_cycle( 10 NS );
  }

  return 0;
}

