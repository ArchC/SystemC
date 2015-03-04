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

    sc_module_name.h -- An object used to help manage object names 
                        and hierarchy

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_MODULE_NAME_H
#define SC_MODULE_NAME_H

class sc_module_name {
    friend class sc_module;
    friend class sc_object_manager;

public:
    sc_module_name(const char* nm);
    sc_module_name(const sc_module_name& nm);
    ~sc_module_name();
    operator const char*() const;

private:
    const char* name;
    sc_module* module;
    sc_module_name* next;
    sc_simcontext* simc;

    // We do not permit assignment here.
    sc_module_name& operator=(const sc_module_name&);
};

#endif
