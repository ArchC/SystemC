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
 
    icache.cpp -- Instruction Cache Unit 
 
    Original Author: Martin Wang. Synopsys, Inc. (mwang@synopsys.com)
 
******************************************************************************/
 
/******************************************************************************
 
    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:
 
    Modifier Name & Affiliation:
      Gene Bushuyev, Synopsys, Inc. (gbush@synopsys.com)
    Description of Modification:
      bug - read/write outside of allocated memory
 
    Modifier Name & Affiliation:
    Description of Modification:
 
******************************************************************************/


 


#include <iostream.h>
#include "systemc.h"
#include "icache.h"
#include "directive.h"


void icache::entry()
{
    unsigned int 	address;

 while (true) {
    	wait_until(cs.delayed() == true); 
    	address = addr.read();
	if (address == BOOT_LENGTH) {
		 printf("ICU ALERT: *********************************************************************\n");
		 printf("         : *****************************AFTER RESET*****************************\n");
		 printf("ICU ALERT: *********************************************************************\n");
	}
	if (address >= BOOT_LENGTH) {
		if (ld_valid.read() == true) {
			pid = ld_data.read();
			printf("------------------------\n");
			printf("ICU: PID = [%d]", pid); 
                       	cout << " at CSIM " << sc_time_stamp() << endl;
			printf("------------------------\n");
      			wait();
      			wait();
		}
    		if (we.read() == true) { // Write operation
      			wait();
			if (address < MAX_CODE_LENGTH && address >= 0)
                          icmemory[address] = datain.read();
                        else
                          printf("ICU ALERT: **MEMORY OUT OF RANGE**\n");
      			wait();
    		}
    		else { // Read operation
			wait(); // Introduce delay needed
			if (address >= MAX_CODE_LENGTH || address < 0) {
				dataout.write(0xffffffff);
				printf("ICU ALERT: **MEMORY OUT OF RANGE**\n");
			}
                        else
        		  dataout.write(icmemory[address]);

			icache_valid.write(true);

			if (PRINT_ICU) {
				printf("------------------------\n");
				printf("ICU: fetching mem[%d]\n", address);
                                if (address < MAX_CODE_LENGTH && address >= 0) 
				  printf("ICU: (%0x)", icmemory[address]); 
       		                cout.setf(ios::dec,ios::basefield);
                        	cout << " at CSIM " << sc_time_stamp() << endl;
				printf("------------------------\n");
			}

      			wait();
			icache_valid.write(false);
      			wait();
    		}    
	}
 }
} // end of entry function

