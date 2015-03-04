/******************************************************************************
    Copyright (c) 1996-2001 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    sc_except.h - exception classes to be handled by SystemC

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: 
	    Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)
    Description of Modification:
	    had to add empty public default and copy constructors to satisfy VC6.0
    
    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/


#ifndef SC_EXCEPT_H
#define SC_EXCEPT_H

/*
 * These classes are intentionally empty.  Their raison d'etre is for
 * the implemetation of `watching'.
 */

class sc_user {
    /*EMPTY*/
public:
	sc_user(){}
	sc_user(const sc_user&){}
};

class sc_halt {
    /*EMPTY*/
public:
	sc_halt(){}
	sc_halt(const sc_halt&){}
};

#endif
