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
 
    bios.cpp -- System Bios 
 
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
#include "bios.h"
#include "directive.h"


void bios::entry()
{
 unsigned address;

 while (true) {
    	wait_until(cs.delayed() == true); 
    	address = addr.read();
	if (address < BOOT_LENGTH) {		// in BOOTING STAGE
    		if (we.read() == true) { // Write operation
      			wait(wait_cycles-1);
      			imemory[address] = datain.read();
    		}
    		else { // Read operation
      			if (wait_cycles > 2)
				wait(wait_cycles-2); // Introduce delay needed
        			dataout.write(imemory[address]);

				if (PRINT_BIOS) {
					printf("------------------------\n");
					printf("BIOS: fetching mem[%d]\n", address);
					printf("BIOS: (%0x)", imemory[address]);
 					cout.setf(ios::dec,ios::basefield);
   					cout << " at CSIM " << sc_time_stamp() << endl;
					printf("------------------------\n");
				}

				bios_valid.write(true);
      				wait();
				bios_valid.write(false);
				wait();
    		}    
	} else {
				bios_valid.write(false);
				wait();
	}
 }
} // end of entry function

