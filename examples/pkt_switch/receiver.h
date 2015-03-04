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

    receiver.h - This is the interface file for the asynchronous process "receiver".

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
struct receiver: sc_module {
  sc_in<pkt> pkt_in;  
  sc_in<sc_int<4> > sink_id;  
  int first;
  SC_CTOR(receiver) {
      SC_METHOD(entry); 
      sensitive(pkt_in);
      first = 1;
    }  
 void entry();
};
