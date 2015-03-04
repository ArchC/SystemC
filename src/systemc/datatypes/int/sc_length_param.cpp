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

  sc_length_param.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-03-19

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/datatypes/int/sc_length_param.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_length_param
//
//  Length parameter type.
// ----------------------------------------------------------------------------

const sc_string
sc_length_param::to_string() const
{
    sc_string s;

    char buf[BUFSIZ];

    s += "(";
    sprintf( buf, "%d", m_len );
    s += buf;
    s += ")";

    return s;
}


void
sc_length_param::print( ostream& os ) const
{
    os << to_string();
}

void
sc_length_param::dump( ostream& os ) const
{
    os << "sc_length_param" << endl;
    os << "(" << endl;
    os << "len = " << m_len << endl;
    os << ")" << endl;
}

} // namespace sc_dt


// Taf!
