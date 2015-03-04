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
 
    paging.cpp -- Instruction Paging Unit 
 
    Original Author: Martin Wang. Synopsys, Inc. (mwang@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
    Description of Modification:
 
 
******************************************************************************/


 

#include <iostream.h>
#include "systemc.h"
#include "paging.h"
#include "directive.h"

void paging::entry()
{
 int		address=0;
 int		address_conversion_factor = 0;
 int	 	dataout_tmp =0;

 while (true) {
    	wait_until(paging_csin.delayed() == true); 
	address = logical_address.read();
        address_conversion_factor = paging_din.read(); 

	if (address >= 5) {
    		if (paging_wein.read() == true) { 	// Write operation
			paging_dout.write(paging_din.read());
			paging_csout.write(true);
			paging_weout.write(true);
			physical_address.write(logical_address.read());
      			wait();
			paging_csout.write(false);
			paging_weout.write(false);
			
    		}
    		else { 					// Read operation
			paging_csout.write(true);
			paging_weout.write(false);
			physical_address.write(logical_address.read());
			wait();
			wait_until(icache_validin.delayed() == true);
			dataout_tmp = icache_din.read();
			
			if (PRINT_PU){
   				cout << "-----------------------" << endl;
				printf( "PAGE : mem=%x\n",dataout_tmp);
   				cout << "PAGE : " ;
   				cout << " at CSIM " << sc_time_stamp() << endl;
   				cout << "-----------------------" << endl;
			}

			dataout.write(icache_din.read());
			data_valid.write(true);
			paging_csout.write(false);
			wait(); 
			data_valid.write(false);
			wait();
    		}    
	}
 }
} // end of entry function

