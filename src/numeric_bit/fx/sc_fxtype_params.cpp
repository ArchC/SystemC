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

    sc_fxtype_params.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_fxtype_params.h"


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
    sprintf( buf, "%d", _wl );
    s += buf;
    s += ",";
    sprintf( buf, "%d", _iwl );
    s += buf;
    s += ",";
    s += ::to_string( _q_mode );
    s += ",";
    s += ::to_string( _o_mode );
    s += ",";
    sprintf( buf, "%d", _n_bits );
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
    os << "wl     = " << _wl << endl;
    os << "iwl    = " << _iwl << endl;
    os << "q_mode = " << _q_mode << endl;
    os << "o_mode = " << _o_mode << endl;
    os << "n_bits = " << _n_bits << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  TYPEDEF : sc_fxtype_context
//
//  Context type for the fixed-point type parameters.
// ----------------------------------------------------------------------------

//template class sc_context<sc_fxtype_params>;


// Taf!
