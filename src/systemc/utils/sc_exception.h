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

  sc_exception.h -- Exception handling for SystemC classes. All SystemC
                    classes are expected to throw only sc_exception (and
                    derived from it) objects, standard C++ exceptions, and
                    structured exceptions (platform dependent).

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_EXCEPTION_H
#define SC_EXCEPTION_H


#include "systemc/utils/sc_string.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_exception
//
//  Exception base class.
// ----------------------------------------------------------------------------

class sc_exception
{
public:

    sc_exception();
    sc_exception( const sc_string& what_ );
    sc_exception( const sc_exception& );

    sc_exception& operator = ( const sc_exception& );

    virtual ~sc_exception();

    virtual const char* what() const;

protected:

    sc_string m_what;
};


#endif

// Taf!
