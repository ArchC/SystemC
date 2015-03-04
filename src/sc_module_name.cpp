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

    sc_module_name.cpp -- An object used to help manage object names 
                          and hierarchy

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <stdlib.h>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_object.h"
#include "sc_simcontext.h"
#include "sc_object_manager.h"
#include "sc_module_int.h"
#include "sc_module_name.h"
#include "sc_exception.h"

sc_module_name::sc_module_name(const char* nm)
{
    simc = sc_get_curr_simcontext();
    name = nm;
    module = 0;
    simc->get_object_manager()->push_module_name(this);
}

sc_module_name::sc_module_name(const sc_module_name& nm)
{
    simc = sc_get_curr_simcontext();
    name = nm.name;
    module = 0;
    simc->get_object_manager()->push_module_name(this);
}

sc_module_name::~sc_module_name()
{
    sc_module_name* smn = simc->get_object_manager()->pop_module_name();
    if (this != smn || 0 == module)
        REPORT_ERROR(4001,"");
    sc_module_helpers::end_module(module);
}

sc_module_name::operator const char*() const
{
    return name;
}
