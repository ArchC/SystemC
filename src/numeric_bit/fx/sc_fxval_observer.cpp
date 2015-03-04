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

    sc_fxval_observer.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fxval_observer.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_observer
//
//  Abstract base class for fixed-point value type observers;
//  arbitrary precision.
// ----------------------------------------------------------------------------

sc_fxval_observer* (*sc_fxval_observer::default_observer) () = 0;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast_observer
//
//  Abstract base class for fixed-point value type observers;
//  limited precision.
// ----------------------------------------------------------------------------

sc_fxval_fast_observer* (*sc_fxval_fast_observer::default_observer) () = 0;


// Taf!
