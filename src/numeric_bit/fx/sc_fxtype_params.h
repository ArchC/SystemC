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

    sc_fxtype_params.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXTYPE_PARAMS_H
#define SC_FXTYPE_PARAMS_H


#include "sc_context.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_fxtype_params
//
//  Fixed-point type parameters class.
// ----------------------------------------------------------------------------

class sc_fxtype_params
{

public:

             sc_fxtype_params();
             sc_fxtype_params( int, int );
             sc_fxtype_params(           sc_q_mode, sc_o_mode, int = 0 );
             sc_fxtype_params( int, int, sc_q_mode, sc_o_mode, int = 0 );
             sc_fxtype_params( const sc_fxtype_params& );
    explicit sc_fxtype_params( sc_without_context );

    sc_fxtype_params& operator = ( const sc_fxtype_params& );

    friend bool operator == ( const sc_fxtype_params&,
                              const sc_fxtype_params& );
    friend bool operator != ( const sc_fxtype_params&,
			      const sc_fxtype_params& );

    int wl() const;
    void wl( int );

    int iwl() const;
    void iwl( int );

    sc_q_mode q_mode() const;
    void q_mode( sc_q_mode );

    sc_o_mode o_mode() const;
    void o_mode( sc_o_mode );

    int n_bits() const;
    void n_bits( int );

    const sc_string to_string() const;

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    int       _wl;
    int       _iwl;
    sc_q_mode _q_mode;
    sc_o_mode _o_mode;
    int       _n_bits;

};


// ----------------------------------------------------------------------------
//  TYPEDEF : sc_fxtype_context
//
//  Context type for the fixed-point type parameters.
// ----------------------------------------------------------------------------

typedef sc_context<sc_fxtype_params> sc_fxtype_context;


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
sc_fxtype_params::sc_fxtype_params()
{
    *this = sc_fxtype_context::default_value();
}

inline
sc_fxtype_params::sc_fxtype_params( int wl_, int iwl_ )
{
    *this = sc_fxtype_context::default_value();

    _SC_CHECK_WL( wl_ );
    _wl  = wl_;
    _iwl = iwl_;
}

inline
sc_fxtype_params::sc_fxtype_params( sc_q_mode q_mode_,
                                    sc_o_mode o_mode_, int n_bits_ )
{
    *this = sc_fxtype_context::default_value();

    _SC_CHECK_N_BITS( n_bits_ );
    _q_mode = q_mode_;
    _o_mode = o_mode_;
    _n_bits = n_bits_;
}

inline
sc_fxtype_params::sc_fxtype_params( int wl_, int iwl_,
                                    sc_q_mode q_mode_,
                                    sc_o_mode o_mode_, int n_bits_ )
{
    _SC_CHECK_WL( wl_ );
    _SC_CHECK_N_BITS( n_bits_ );
    _wl     = wl_;
    _iwl    = iwl_;
    _q_mode = q_mode_;
    _o_mode = o_mode_;
    _n_bits = n_bits_;
}

inline
sc_fxtype_params::sc_fxtype_params( const sc_fxtype_params& a )
: _wl( a._wl ), _iwl( a._iwl ),
  _q_mode( a._q_mode ),
  _o_mode( a._o_mode ), _n_bits( a._n_bits )
{}

inline
sc_fxtype_params::sc_fxtype_params( sc_without_context )
: _wl    ( _SC_DEFAULT_WL ),
  _iwl   ( _SC_DEFAULT_IWL ),
  _q_mode( _SC_DEFAULT_Q_MODE ),
  _o_mode( _SC_DEFAULT_O_MODE ),
  _n_bits( _SC_DEFAULT_N_BITS )
{}


inline
sc_fxtype_params&
sc_fxtype_params::operator = ( const sc_fxtype_params& a )
{
    if( &a != this )
    {
        _wl     = a._wl;
	_iwl    = a._iwl;
	_q_mode = a._q_mode;
	_o_mode = a._o_mode;
	_n_bits = a._n_bits;
    }
    return *this;
}


inline
bool
operator == ( const sc_fxtype_params& a, const sc_fxtype_params& b )
{
    return ( a._wl     == b._wl     &&
	     a._iwl    == b._iwl    &&
	     a._q_mode == b._q_mode &&
	     a._o_mode == b._o_mode &&
	     a._n_bits == b._n_bits );
}

inline
bool
operator != ( const sc_fxtype_params& a, const sc_fxtype_params& b )
{
    return ( a._wl     != b._wl     ||
	     a._iwl    != b._iwl    ||
	     a._q_mode != b._q_mode ||
	     a._o_mode != b._o_mode ||
	     a._n_bits != b._n_bits );
}


inline
int
sc_fxtype_params::wl() const
{
    return _wl;
}

inline
void
sc_fxtype_params::wl( int wl_ )
{
    _SC_CHECK_WL( wl_ );
    _wl = wl_;
}


inline
int
sc_fxtype_params::iwl() const
{
    return _iwl;
}

inline
void
sc_fxtype_params::iwl( int iwl_ )
{
    _iwl = iwl_;
}


inline
sc_q_mode
sc_fxtype_params::q_mode() const
{
    return _q_mode;
}

inline
void
sc_fxtype_params::q_mode( sc_q_mode q_mode_ )
{
    _q_mode = q_mode_;
}


inline
sc_o_mode
sc_fxtype_params::o_mode() const
{
    return _o_mode;
}

inline
void
sc_fxtype_params::o_mode( sc_o_mode o_mode_ )
{
    _o_mode = o_mode_;
}


inline
int
sc_fxtype_params::n_bits() const
{
    return _n_bits;
}

inline
void
sc_fxtype_params::n_bits( int n_bits_ )
{
    _SC_CHECK_N_BITS( n_bits_ );
    _n_bits = n_bits_;
}


inline
ostream&
operator << ( ostream& os, const sc_fxtype_params& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
