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

    scfx_params.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_PARAMS_H
#define SCFX_PARAMS_H


#include "sc_fxcast_switch.h"
#include "sc_fxtype_params.h"


// ----------------------------------------------------------------------------
//  CLASS : scfx_params
//
//  ...
// ----------------------------------------------------------------------------

class scfx_params
{

public:

    // constructor

    scfx_params( const sc_fxtype_params&,
		 sc_enc,
		 const sc_fxcast_switch& );


    // query functions

    const sc_fxtype_params& type_params() const;
    sc_enc enc() const;
    const sc_fxcast_switch& cast_switch() const;


    // shortcuts

    int wl() const;
    int iwl() const;
    int fwl() const;
    sc_q_mode q_mode() const;
    sc_o_mode o_mode() const;
    int n_bits() const;


    // dump content

    void dump( ostream& ) const;

private:

    sc_fxtype_params _type_params;
    sc_enc           _enc;
    sc_fxcast_switch _cast_switch;
    
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// constructor

inline
scfx_params::scfx_params( const sc_fxtype_params& type_params_,
			  sc_enc enc_,
			  const sc_fxcast_switch& cast_sw )
: _type_params( type_params_ ),
  _enc( enc_ ),
  _cast_switch( cast_sw )
{
    if( _enc == SC_US && _type_params.o_mode() == SC_WRAP_SM )
    {
	sc_report::error( _SC_ID_INVALID_O_MODE,
			  "SC_WRAP_SM not defined for unsigned numbers" );
    }

}


// query functions

inline
const sc_fxtype_params&
scfx_params::type_params() const
{
    return _type_params;
}

inline
sc_enc
scfx_params::enc() const
{
    return _enc;
}

inline
const sc_fxcast_switch&
scfx_params::cast_switch() const
{
    return _cast_switch;
}


// shortcuts

inline
int
scfx_params::wl() const
{
    return _type_params.wl();
}

inline
int
scfx_params::iwl() const
{
    return _type_params.iwl();
}

inline
int
scfx_params::fwl() const
{
    return ( _type_params.wl() - _type_params.iwl() );
}

inline
sc_q_mode
scfx_params::q_mode() const
{
    return _type_params.q_mode();
}

inline
sc_o_mode
scfx_params::o_mode() const
{
    return _type_params.o_mode();
}

inline
int
scfx_params::n_bits() const
{
    return _type_params.n_bits();
}


// dump content

inline
void
scfx_params::dump( ostream& os ) const
{
    os << "scfx_params" << endl;
    os << "(" << endl;
    os << "type_params = ";
    _type_params.dump( os );
    os << "enc         = " << _enc << endl;
    os << "cast_switch = ";
    _cast_switch.dump( os );
    os << ")" << endl;
}


#endif

// Taf!
