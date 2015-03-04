/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2001 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_context_switch.h -- Manage context switching.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_CONTEXT_SWITCH
#define SC_CONTEXT_SWITCH


#include "systemc/kernel/sc_cmnhdr.h"

class sc_simcontext;


#ifndef WIN32

struct qt_t;

extern "C" {
    typedef void* (*AFT)( qt_t*, void*, void* );
}

extern void context_switch( AFT yieldhelper, void* data, void*, qt_t* qt );

#else

extern void context_switch( LPVOID data );

#endif


#endif
