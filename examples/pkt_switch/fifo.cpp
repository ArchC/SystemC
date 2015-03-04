/***********************************************************************
   Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

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

    fifo.cpp - This is the functinality file for the SystemC structure "fifo"

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/
#include "pkt.h"
#include "fifo.h"

    void fifo::pkt_in(const pkt& data_pkt)
    {
      regs[pntr++] = data_pkt; empty = false;
      if (pntr == 4) full = true;      
    }

    pkt fifo::pkt_out()
    {
       pkt temp;
       temp = regs[0];
       if (--pntr == 0) empty = true;
       else 
	{ 
            regs[0] = regs[1];
	    regs[1] = regs[2];
	    regs[2] = regs[3];
        } 
      return(temp);  
    }
