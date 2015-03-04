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

  sc_fxcast_switch.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Gene Bushuyev, Synopsys, Inc.
  Description of Modification: - fix explicit instantiation syntax.
    
      Name, Affiliation, Date:
  Description of Modification: 

 *****************************************************************************/


#include "systemc/datatypes/fx/sc_fxcast_switch.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS : sc_fxcast_switch
//
//  Fixed-point cast switch class.
// ----------------------------------------------------------------------------

const sc_string
sc_fxcast_switch::to_string() const
{
    return sc_dt::to_string( m_sw );
}


void
sc_fxcast_switch::print( ostream& os ) const
{
    os << sc_dt::to_string( m_sw );
}

void
sc_fxcast_switch::dump( ostream& os ) const
{
    os << "sc_fxcast_switch" << endl;
    os << "(" << endl;
    os << "sw = " << sc_dt::to_string( m_sw ) << endl;
    os << ")" << endl;
}

} // namespace sc_dt


// Taf!
