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

    sc_helpers.h - helper function declarations

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_HELPERS_H
#define SC_HELPERS_H

/* Requires "qt/qt.h" */

#ifndef WIN32
extern "C" {
    void* sc_clock_edge_yieldhelp(qt_t*, void*, void*);

    void* sc_sync_aborthelp( qt_t*, void* old, void* );
    void* sc_sync_yieldhelp( qt_t* sp, void* old, void* );
    void  sc_sync_only( void*, void* p, qt_userf_t* );

    void* sc_aproc_aborthelp(qt_t*, void* old, void*);
    void* sc_aproc_yieldhelp(qt_t*, void* old, void*);
    void  sc_aproc_only(void*, void* p, qt_userf_t*);
}
#endif

#endif
