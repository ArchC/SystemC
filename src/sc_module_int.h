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

    sc_module_int.h -- Internal header file used to decouple tiresome includes.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: Implementation of the function
    test_module_prm. It is explained in sc_module.cpp.

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_MODULE_INT_H
#define SC_MODULE_INT_H

#include "sc_module_prm.h"

class sc_module_helpers {
    friend class sc_module;
    friend class sc_module_name;
    static void end_module(sc_module*);
    // Test and bind pxxx, which can be a port or a signal, to module mod.
    static bool test_module_prm(sc_module *mod, sc_module_prm& pxxx);
};

#endif
