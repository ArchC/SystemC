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

    sc_context_switch.cpp -- manage context switching.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef WIN32
#include "qt/qt.h"
#else
#include "sc_cmnhdr.h"
#include <Windows.h>
#endif
#include "sc_context_switch.h"
#include "sc_measure.h"


#ifndef WIN32
void
context_switch( AFT yieldhelper, void* data, void* user, qt_t* qt )
{
    begin_context_switch();
    QT_BLOCK( yieldhelper, data, user, qt );
    end_context_switch();
}

#else
void
context_switch( LPVOID lpFiber )
{
    begin_context_switch();
    SwitchToFiber( lpFiber );
    end_context_switch();
}
#endif
