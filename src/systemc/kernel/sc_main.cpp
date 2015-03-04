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

  sc_main.cpp - Wrapper around user's toplevel routine `sc_main'.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/kernel/sc_cmnhdr.h"
#include "systemc/kernel/sc_externs.h"
#include "systemc/utils/sc_exception.h"
#include "systemc/utils/sc_iostream.h"


extern void pln();

static
inline
void
message_function( const char* s )
{
    cout << "\n" << s << endl;
}

bool sc_in_action = false;

int
main( int argc, char* argv[] )
{
    int status = 0;
    try
    {
        pln();

        // Perform initialization here
        sc_in_action = true;

        status = sc_main( argc, argv );

        // Perform cleanup here
        sc_in_action = false;
    }
    catch( const sc_exception& x )
    {
        message_function( x.what() );
    }
    catch( const char* s )
    {
        message_function( s );
    }
    catch( ... )
    {
        message_function( "UNKNOWN EXCEPTION OCCURED" );
    }

    return status;
}
