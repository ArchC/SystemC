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

    sc_module_prm.h -- Contains the class sc_module_prm. This class is
    used to implement operator() in sc_module.h. This operator in turn
    implements the positional connection method for modules. Ports and
    signals that are to be connected to the ports of a module are
    first converted to an object of sc_module_prm. In operator(),
    these objects are converted back to their original types.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_MODULE_PRM_H
#define SC_MODULE_PRM_H

class sc_module_prm {
  /* EMPTY */  
};

// Null object for default arguments of operator() in sc_module.h.
extern sc_module_prm SC_MODULE_PRM_NULL;

#endif
