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

  sc_signal.cpp -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/communication/sc_signal.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<bool>
//
//  Specialization of sc_signal<T> for type bool.
// ----------------------------------------------------------------------------

const char* const sc_signal<bool>::kind_string = "sc_signal";


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<sc_logic>
//
//  Specialization of sc_signal<T> for type sc_logic.
// ----------------------------------------------------------------------------

const char* const sc_signal<sc_logic>::kind_string = "sc_signal";


// Taf!
