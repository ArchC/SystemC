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

  sc_runnable.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification: 

 *****************************************************************************/


#include "systemc/kernel/sc_runnable.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_runnable
//
//  Class that manages the ready-to-run queues.
// ----------------------------------------------------------------------------

sc_runnable::sc_runnable()
: m_push( -1 ),
  m_methods_push( 0 ),
  m_methods_pop( 0 ),
  m_threads_push( 0 ),
  m_threads_pop( 0 )
{}

sc_runnable::~sc_runnable()
{}


void
sc_runnable::alloc( int num_methods, int num_threads )
{
    m_methods[0].alloc( num_methods + 1 );
    m_methods[1].alloc( num_methods + 1 );
    m_threads[0].alloc( num_threads + 1 );
    m_threads[1].alloc( num_threads + 1 );
}


// Taf!
