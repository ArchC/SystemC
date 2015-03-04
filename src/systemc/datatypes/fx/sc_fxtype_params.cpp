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

  sc_fxtype_params.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/datatypes/fx/sc_fxtype_params.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_fxtype_params
//
//  Fixed-point type parameters class.
// ----------------------------------------------------------------------------

const sc_string
sc_fxtype_params::to_string() const
{
    sc_string s;

    char buf[BUFSIZ];

    s += "(";
    sprintf( buf, "%d", m_wl );
    s += buf;
    s += ",";
    sprintf( buf, "%d", m_iwl );
    s += buf;
    s += ",";
    s += sc_dt::to_string( m_q_mode );
    s += ",";
    s += sc_dt::to_string( m_o_mode );
    s += ",";
    sprintf( buf, "%d", m_n_bits );
    s += buf;
    s += ")";

    return s;
}


void
sc_fxtype_params::print( ostream& os ) const
{
    os << to_string();
}

void
sc_fxtype_params::dump( ostream& os ) const
{
    os << "sc_fxtype_params" << endl;
    os << "(" << endl;
    os << "wl     = " << m_wl << endl;
    os << "iwl    = " << m_iwl << endl;
    os << "q_mode = " << m_q_mode << endl;
    os << "o_mode = " << m_o_mode << endl;
    os << "n_bits = " << m_n_bits << endl;
    os << ")" << endl;
}

} // namespace sc_dt


// Taf!
