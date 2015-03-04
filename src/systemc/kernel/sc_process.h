/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_process.h -- Declarations for process classes. Requires
                  stdlib.h -- We need size_t.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affiliation, date and
    changes you are making here.

        Name, Affiliation, Date:
    Description of Modification:

 *****************************************************************************/

#ifndef SC_PROCESS_H
#define SC_PROCESS_H

#include "systemc/utils/sc_iostream.h"
#include "systemc/kernel/sc_constants.h"

/*
 *  Define this macro if we want to use member function pointers to implement
 *  process dispatch. Otherwise, we'll use a hack that involves calling a 
 *  nonmember auxiliary function (see sc_module.h for the implementation,
 *  defined inside the user's constructor) which in turn invokes the member
 *  function. The relative performances of these alternatives are to a large
 *  extent machine- and compiler- dependent; it's not clear a priori which is
 *  superior.
 */
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
typedef class sc_cthread_process* sc_cthread_handle;
typedef class sc_method_process*  sc_method_handle;
typedef class sc_thread_process*  sc_thread_handle;

extern void sc_set_stack_size( sc_thread_handle, size_t );


#endif
