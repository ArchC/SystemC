/***********************************************************************
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

  *************************************************************************/

/******************************************************************************

    sink.cpp - This is the implementation file for the synchronous process
              "sink"

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "systemc.h"
#include "sink.h"

void sink::entry()
{
 FILE *fp_real, *fp_imag;
 sc_int<16> tmp;
 int tmp_out;
 fp_real = fopen("out_real","w");
 fp_imag = fopen("out_imag","w");

 data_ack.write(false);

 while(true)
 { 
   wait_until(data_ready.delayed() == true);
   tmp = in_real.read();
   tmp_out = tmp;
   fprintf(fp_real,"%d  \n",tmp_out);
   tmp = in_imag.read();
   tmp_out = tmp;
   fprintf(fp_imag,"%d  \n",tmp_out);
   data_ack.write(true);
   wait_until(data_ready.delayed() == false);
   data_ack.write(false); 
 }
}
