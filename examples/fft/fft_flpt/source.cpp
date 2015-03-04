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

    source.cpp - This is the implementation file for the synchronous process
                 "source" 

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "systemc.h"
#include "source.h"
void source::entry()
{ FILE *fp_real, *fp_imag;

  float tmp_val;

  fp_real = fopen("in_real", "r");
  fp_imag = fopen("in_imag", "r");

  data_valid.write(false);

  while(true)
  { 
    wait_until(data_req.delayed() == true);
    if (fscanf(fp_real,"%f \n",&tmp_val) == EOF)
      { cout << "End of Input Stream: Simulation Stops" << endl;
        sc_stop();
        break;
	};
    out_real.write(tmp_val);
    if (fscanf(fp_imag,"%f \n",&tmp_val) == EOF)
      { cout << "End of Input Stream: Simulation Stops" << endl;
        sc_stop();
	break;
	};
    out_imag.write(tmp_val);
    data_valid.write(true);
    wait_until(data_req.delayed() == false);
    data_valid.write(false);
    wait();
  }
}
