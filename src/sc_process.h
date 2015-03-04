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

    sc_process.h -- Declarations for process classes. Requires
    stdlib.h -- We need size_t.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_PROCESS_H
#define SC_PROCESS_H

#include "sc_iostream.h"
#include "sc_constants.h"

/* Define this macro if we want to use member function pointers to implement
   process dispatch.  Otherwise, we'll use a hack that involves calling a 
   nonmember auxiliary function (see sc_module.h for the implementation, defined
   inside the user's constructor) which in turn invokes the member function.
   The relative performances of these alternatives are to a large extent
   machine- and compiler- dependent; it's not clear a priori which is superior. */
#if !defined(_MSC_VER)
#define SC_USE_MEMBER_FUNC_PTR
#endif

class sc_module;
#ifndef SC_USE_MEMBER_FUNC_PTR
typedef void (*SC_ENTRY_FUNC)(sc_module*);
#define SC_DEFUNCT_PROCESS_FUNCTION &sc_defunct_process_function
#else
typedef void (sc_module::*SC_ENTRY_FUNC)();
#define SC_DEFUNCT_PROCESS_FUNCTION &sc_module::defunct
#endif

class sc_process_b; /* base class for all kinds of processes */
typedef class sc_sync_process*  sc_sync_process_handle;
typedef class sc_async_process* sc_async_process_handle;
typedef class sc_aproc_process* sc_aproc_process_handle;

extern void sc_set_stack_size(sc_sync_process_handle sync_h, size_t size);
extern void sc_set_stack_size(sc_aproc_process_handle aproc_h, size_t size);

#endif
