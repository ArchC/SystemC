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

    receiver.cpp - This is the implementation file for the asynchronous process
                  "receiver".

    Original Author: Rashmi Goswami (rashmig@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

/***************************************************************************

   Copyright (c) 1998-2000 Synopsys, Inc.
   ALL RIGHTS RESERVED
   
*****************************************************************************/
#include "pkt.h"
#include "receiver.h"


void receiver:: entry()
{ pkt temp_val;
  // Ignore the first packet arriving on start-on
  if (first == 1) {first = 0;}
  else {
	temp_val = pkt_in.read();
	cout << "                                  .........................." << endl;
	cout << "                                  New Packet Received" << endl;
	cout << "                                  Receiver ID: " << (int)sink_id.read() + 1 << endl;
	cout << "                                  Packet Value: " << (int)temp_val.data << endl;
	cout << "                                  Sender ID: " << (int)temp_val.id + 1 << endl;
	cout << "                                  .........................." << endl;
   } 

}
