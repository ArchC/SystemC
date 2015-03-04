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

  sc_semaphore.cpp -- The sc_semaphore primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/communication/sc_semaphore.h"
#include "systemc/kernel/sc_simcontext.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_semaphore
//
//  The sc_semaphore primitive channel class.
// ----------------------------------------------------------------------------

const char* const sc_semaphore::kind_string = "sc_semaphore";


// constructors

sc_semaphore::sc_semaphore( int init_value_ )
: sc_prim_channel( sc_gen_unique_name( "semaphore" ) ),
  m_value( init_value_ )
{
    assert( m_value >= 0 );
}

sc_semaphore::sc_semaphore( const char* name_, int init_value_ )
: sc_prim_channel( name_ ), m_value( init_value_ )
{
    assert( m_value >= 0 );
}


// interface methods

// lock (take) the semaphore, block if not available

int
sc_semaphore::wait()
{
    while( in_use() ) {
	sc_prim_channel::wait( m_free );
    }
    -- m_value;
    return 0;
}


// lock (take) the semaphore, return -1 if not available

int
sc_semaphore::trywait()
{
    if( in_use() ) {
	return -1;
    }
    -- m_value;
    return 0;
}


// unlock (give) the semaphore

int
sc_semaphore::post()
{
    ++ m_value;
    m_free.notify();
    return 0;
}


// Taf!
