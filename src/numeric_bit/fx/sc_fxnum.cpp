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

    sc_fxnum.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <math.h>

#include "sc_fxnum.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_bitref
//
//  Proxy class for bit-selection in class sc_fxnum, behaves like sc_bit.
// ----------------------------------------------------------------------------

bool
sc_fxnum_bitref::get() const
{
    return _num.get_bit( _idx );
}

void
sc_fxnum_bitref::set( bool high )
{
    _num.set_bit( _idx, high );
}


// print or dump content

void
sc_fxnum_bitref::print( ostream& os ) const
{
    if( get() )
	os << "1";
    else
	os << "0";
}

void
sc_fxnum_bitref::dump( ostream& os ) const
{
    os << "sc_fxnum_bitref" << endl;
    os << "(" << endl;
    os << "num = ";
    _num.dump( os );
    os << "idx = " << _idx << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_bitref
//
//  Proxy class for bit-selection in class sc_fxnum_fast, behaves like sc_bit.
// ----------------------------------------------------------------------------

bool
sc_fxnum_fast_bitref::get() const
{
    return _num.get_bit( _idx );
}

void
sc_fxnum_fast_bitref::set( bool high )
{
    _num.set_bit( _idx, high );
}


// print or dump content

void
sc_fxnum_fast_bitref::print( ostream& os ) const
{
    if( get() )
	os << "1";
    else
	os << "0";
}

void
sc_fxnum_fast_bitref::dump( ostream& os ) const
{
    os << "sc_fxnum_fast_bitref" << endl;
    os << "(" << endl;
    os << "num = ";
    _num.dump( os );
    os << "idx = " << _idx << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_subref
//
//  Proxy class for part-selection in class sc_fxnum,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

bool
sc_fxnum_subref::get() const
{
    return _num.get_slice( _from, _to, _bv );
}

bool
sc_fxnum_subref::set()
{
    return _num.set_slice( _from, _to, _bv );
}


// print or dump content

void
sc_fxnum_subref::print( ostream& os ) const
{
    get();
    _bv.print( os );
}

void
sc_fxnum_subref::dump( ostream& os ) const
{
    os << "sc_fxnum_subref" << endl;
    os << "(" << endl;
    os << "num  = ";
    _num.dump( os );
    os << "from = " << _from << endl;
    os << "to   = " << _to << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_subref
//
//  Proxy class for part-selection in class sc_fxnum_fast,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

bool
sc_fxnum_fast_subref::get() const
{
    return _num.get_slice( _from, _to, _bv );
}

bool
sc_fxnum_fast_subref::set()
{
    return _num.set_slice( _from, _to, _bv );
}


// print or dump content

void
sc_fxnum_fast_subref::print( ostream& os ) const
{
    get();
    _bv.print( os );
}

void
sc_fxnum_fast_subref::dump( ostream& os ) const
{
    os << "sc_fxnum_fast_subref" << endl;
    os << "(" << endl;
    os << "num  = ";
    _num.dump( os );
    os << "from = " << _from << endl;
    os << "to   = " << _to << endl;
    os << ")" << endl;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum
//
//  Base class for the fixed-point types; arbitrary precision.
// ----------------------------------------------------------------------------

// explicit conversion to character string

const sc_string
sc_fxnum::to_string() const
{
    return sc_string( _rep->to_string( SC_DEC, SC_F, &_params ) );
}

const sc_string
sc_fxnum::to_string( sc_numrep numrep ) const
{
    return sc_string( _rep->to_string( numrep, SC_F, &_params ) );
}

const sc_string
sc_fxnum::to_string( sc_fmt fmt ) const
{
    return sc_string( _rep->to_string( SC_DEC, fmt, &_params ) );
}

const sc_string
sc_fxnum::to_string( sc_numrep numrep, sc_fmt fmt ) const
{
    return sc_string( _rep->to_string( numrep, fmt, &_params ) );
}


const sc_string
sc_fxnum::to_dec() const
{
    return sc_string( _rep->to_string( SC_DEC, SC_F, &_params ) );
}

const sc_string
sc_fxnum::to_bin() const
{
    return sc_string( _rep->to_string( SC_BIN, SC_F, &_params ) );
}

const sc_string
sc_fxnum::to_oct() const
{
    return sc_string( _rep->to_string( SC_OCT, SC_F, &_params ) );
}

const sc_string
sc_fxnum::to_hex() const
{
    return sc_string( _rep->to_string( SC_HEX, SC_F, &_params ) );
}


// print or dump content

void
sc_fxnum::print( ostream& os ) const
{
    os << _rep->to_string( SC_DEC, SC_F, &_params );
}

void
sc_fxnum::dump( ostream& os ) const
{
    os << "sc_fxnum" << endl;
    os << "(" << endl;
    os << "rep      = ";
    _rep->dump( os );
    os << "params   = ";
    _params.dump( os );
    os << "q_flag   = " << _q_flag << endl;
    os << "o_flag   = " << _o_flag << endl;
    // TO BE COMPLETED
    // os << "observer = ";
    // if( _observer != 0 )
    //     _observer->dump( os );
    // else
    //     os << "0" << endl;
    os << ")" << endl;
}


sc_fxnum_observer*
sc_fxnum::lock_observer() const
{
    _SC_ASSERT( _observer != 0, "lock observer failed" );
    sc_fxnum_observer* tmp = _observer;
    _observer = 0;
    return tmp;
}

void
sc_fxnum::unlock_observer( sc_fxnum_observer* observer_ ) const
{
    _SC_ASSERT( observer_ != 0, "unlock observer failed" );
    _observer = observer_;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast
//
//  Base class for the fixed-point types; limited precision.
// ----------------------------------------------------------------------------

static
void
quantization( double& c, const scfx_params& params, bool& q_flag )
{
    int fwl = params.wl() - params.iwl();
    double scale = scfx_pow2( fwl );
    double val = scale * c;
    double int_part;
    double frac_part = modf( val, &int_part );

    q_flag = ( frac_part != 0.0 );

    if( q_flag )
    {
        val = int_part;

	switch( params.q_mode() )
	{
            case SC_TRN:			// truncation
	    {
	        if( val < 0.0 )
		    val -= 1.0;
		break;
	    }
            case SC_RND:			// rounding to plus infinity
	    {
		if( val >= 0.0 )
		{
		    if( frac_part >= 0.5 )
			val += 1.0;
		}
		else
		{
		    if( frac_part < -0.5 )
			val -= 1.0;
		}
		break;
	    }
            case SC_TRN_ZERO:			// truncation to zero
	    {
	        break;
	    }
            case SC_RND_INF:			// rounding to infinity
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part >= 0.5 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part <= -0.5 )
		        val -= 1.0;
		}
		break;
	    }
            case SC_RND_CONV:			// convergent rounding
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part > 0.5 ||
			frac_part == 0.5 && fmod( int_part, 2.0 ) != 0.0 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part < -0.5 ||
			frac_part == -0.5 && fmod( int_part, 2.0 ) != 0.0 )
		        val -= 1.0;
		}
		break;
	    }
            case SC_RND_ZERO:			// rounding to zero
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part > 0.5 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part < -0.5 )
		        val -= 1.0;
		}
		break;
	    }
            case SC_RND_MIN_INF:		// rounding to minus infinity
	    {
	        if( val >= 0.0 )
		{
		    if( frac_part > 0.5 )
		        val += 1.0;
		}
		else
		{
		    if( frac_part <= -0.5 )
		        val -= 1.0;
		}
		break;
	    }
            default:
	        ;
	}
    }

    val /= scale;
    c = val;
}

static
void
overflow( double& c, const scfx_params& params, bool& o_flag )
{
    int iwl = params.iwl();
    int fwl = params.wl() - iwl;
    double full_circle = scfx_pow2( iwl );
    double resolution = scfx_pow2( -fwl );
    double low, high;
    if( params.enc() == SC_TC )
    {
	high = full_circle / 2.0 - resolution;
	if( params.o_mode() == SC_SAT_SYM )
	    low = - high;
	else
	    low = - full_circle / 2.0;
    }
    else
    {
	low = 0.0;
	high = full_circle - resolution;
    }
    double val = c;
    sc_fxval_fast c2 = c;

    bool under = ( val < low );
    bool over = ( val > high );

    o_flag = ( under || over );

    if( o_flag )
    {
        switch( params.o_mode() )
	{
            case SC_WRAP:			// wrap-around
	    {
		int n_bits = params.n_bits();

	        if( n_bits == 0 )
		{
		    // wrap-around all 'wl' bits
		    val -= floor( val / full_circle ) * full_circle;
		    if( val > high )
			val -= full_circle;
		}
		else if( n_bits < params.wl() )
		{
		    double X = scfx_pow2( iwl - n_bits );

		    // wrap-around least significant 'wl - n_bits' bits
		    val -= floor( val / X ) * X;
		    if( val > ( X - resolution ) )
			val -= X;
		    
		    // saturate most significant 'n_bits' bits
		    if( under )
		        val += low;
		    else
		    {
		        if( params.enc() == SC_TC )
			    val += full_circle / 2.0 - X;
			else
			    val += full_circle - X;
		    }
		}
		else
		{
		    // saturate all 'wl' bits
		    if( under )
			val = low;
		    else
			val = high;
		}
		break;
	    }
            case SC_SAT:			// saturation
            case SC_SAT_SYM:			// symmetrical saturation
	    {
	        if( under )
		    val = low;
		else
		    val = high;
		break;
	    }
            case SC_SAT_ZERO:			// saturation to zero
	    {
	        val = 0.0;
		break;
	    }
            case SC_WRAP_SM:			// sign magnitude wrap-around
	    {
		_SC_ERROR_IF( params.enc() == SC_US,
			      "SC_WRAP_SM not defined for unsigned numbers" );
	
		int n_bits = params.n_bits();

		if( n_bits == 0 )
		{
		    // invert conditionally
		    if( c2.get_bit( iwl ) != c2.get_bit( iwl - 1 ) )
			val = -val - resolution;

		    // wrap-around all 'wl' bits
		    val -= floor( val / full_circle ) * full_circle;
		    if( val > high )
			val -= full_circle;
		}
		else if( n_bits == 1 )
		{
		    // invert conditionally
		    if( c2.is_neg() != c2.get_bit( iwl - 1 ) )
			val = -val - resolution;

		    // wrap-around all 'wl' bits
		    val -= floor( val / full_circle ) * full_circle;
		    if( val > high )
			val -= full_circle;
		}
		else if( n_bits < params.wl() )
		{
		    // invert conditionally
		    if( c2.is_neg() == c2.get_bit( iwl - n_bits ) )
			val = -val - resolution;
		    
		    double X = scfx_pow2( iwl - n_bits );

		    // wrap-around least significant 'wl - n_bits' bits
		    val -= floor( val / X ) * X;
		    if( val > ( X - resolution ) )
			val -= X;

		    // saturate most significant 'n_bits' bits
		    if( under )
		        val += low;
		    else
			val += full_circle / 2.0 - X;
		} else {
		    // saturate all 'wl' bits
		    if( under )
			val = low;
		    else
			val = high;
		}
	        break;
	    }
            default:
	        ;
	}

	c = val;
    }
}


void
sc_fxnum_fast::cast()
{
    scfx_ieee_double id( _val );
    _SC_ERROR_IF( id.is_nan() || id.is_inf(), _SC_ID_INVALID_VALUE );

    if( _params.cast_switch() == SC_ON )
    {
        _q_flag = false;
	_o_flag = false;

	// check for special cases

	if( id.is_zero() )
	{
	    if( id.negative() != 0 )
	        _val = -_val;
	    return;
	}

	// perform casting

	::quantization( _val, _params, _q_flag );
	::overflow( _val, _params, _o_flag );

	// check for special case: -0

	id = _val;
	if( id.is_zero() && id.negative() != 0 )
	    _val = -_val;
    }
}


// defined in sc_fxval.cpp;
extern
const char*
to_string( const scfx_ieee_double&,
	   sc_numrep,
	   sc_fmt,
	   const scfx_params* = 0 );


// explicit conversion to character string

const sc_string
sc_fxnum_fast::to_string() const
{
    return sc_string( ::to_string( _val, SC_DEC, SC_F, &_params ) );
}

const sc_string
sc_fxnum_fast::to_string( sc_numrep numrep ) const
{
    return sc_string( ::to_string( _val, numrep, SC_F, &_params ) );
}

const sc_string
sc_fxnum_fast::to_string( sc_fmt fmt ) const
{
    return sc_string( ::to_string( _val, SC_DEC, fmt, &_params ) );
}

const sc_string
sc_fxnum_fast::to_string( sc_numrep numrep, sc_fmt fmt ) const
{
    return sc_string( ::to_string( _val, numrep, fmt, &_params ) );
}


const sc_string
sc_fxnum_fast::to_dec() const
{
    return sc_string( ::to_string( _val, SC_DEC, SC_F, &_params ) );
}

const sc_string
sc_fxnum_fast::to_bin() const
{
    return sc_string( ::to_string( _val, SC_BIN, SC_F, &_params ) );
}

const sc_string
sc_fxnum_fast::to_oct() const
{
    return sc_string( ::to_string( _val, SC_OCT, SC_F, &_params ) );
}

const sc_string
sc_fxnum_fast::to_hex() const
{
    return sc_string( ::to_string( _val, SC_HEX, SC_F, &_params ) );
}


// print or dump content

void
sc_fxnum_fast::print( ostream& os ) const
{
    os << ::to_string( _val, SC_DEC, SC_F, &_params );
}

void
sc_fxnum_fast::dump( ostream& os ) const
{
    os << "sc_fxnum_fast" << endl;
    os << "(" << endl;
    os << "val      = " << _val << endl;
    os << "params   = ";
    _params.dump( os );
    os << "q_flag   = " << _q_flag << endl;
    os << "o_flag   = " << _o_flag << endl;
    // TO BE COMPLETED
    // os << "observer = ";
    // if( _observer != 0 )
    //     _observer->dump( os );
    // else
    //     os << "0" << endl;
    os << ")" << endl;
}


// internal use only;
bool
sc_fxnum_fast::get_bit( int i ) const
{
    scfx_ieee_double id( _val );
    if( id.is_zero() || id.is_nan() || id.is_inf() )
        return false;

    // convert to two's complement

    unsigned int m0 = id.mantissa0();
    unsigned int m1 = id.mantissa1();

    if( id.is_normal() )
        m0 += 1U << 20;

    if( id.negative() != 0 )
    {
	m0 = ~ m0;
	m1 = ~ m1;
	unsigned int tmp = m1;
	m1 += 1U;
	if( m1 <= tmp )
	    m0 += 1U;
    }

    // get the right bit

    int j = i - id.exponent();
    if( ( j += 20 ) >= 32 )
        return ( ( m0 & 1U << 31 ) != 0 );
    else if( j >= 0 )
        return ( ( m0 & 1U << j ) != 0 );
    else if( ( j += 32 ) >= 0 )
        return ( ( m1 & 1U << j ) != 0 );
    else
        return false;
}


bool
sc_fxnum_fast::set_bit( int i, bool high )
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
        return false;

    if( high )
    {
	if( get_bit( i ) )
	    return true;

	if( _params.enc() == SC_TC && i == _params.iwl() - 1 )
	    _val -= scfx_pow2( i );
	else
	    _val += scfx_pow2( i );
    }
    else
    {
	if( ! get_bit( i ) )
	    return true;

	if( _params.enc() == SC_TC && i == _params.iwl() - 1 )
	    _val += scfx_pow2( i );
	else
	    _val -= scfx_pow2( i );
    }

    return true;
}


bool
sc_fxnum_fast::get_slice( int i, int j, sc_bv_base& bv ) const
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
	return false;

    // convert to two's complement

    unsigned int m0 = id.mantissa0();
    unsigned int m1 = id.mantissa1();

    if( id.is_normal() )
        m0 += 1U << 20;

    if( id.negative() != 0 )
    {
	m0 = ~ m0;
	m1 = ~ m1;
	unsigned int tmp = m1;
	m1 += 1U;
	if( m1 <= tmp )
	    m0 += 1U;
    }

    // get the bits

    int l = j;
    for( int k = 0; k < static_cast<int>( bv.length() ); ++ k )
    {
	bool b = false;

        int n = l - id.exponent();
        if( ( n += 20 ) >= 32 )
	    b = ( ( m0 & 1U << 31 ) != 0 );
	else if( n >= 0 )
	    b = ( ( m0 & 1U << n ) != 0 );
	else if( ( n += 32 ) >= 0 )
	    b = ( ( m1 & 1U << n ) != 0 );

	bv[k] = b;

	if( i >= j )
	    ++ l;
	else
	    -- l;
    }

    return true;
}

bool
sc_fxnum_fast::set_slice( int i, int j, const sc_bv_base& bv )
{
    scfx_ieee_double id( _val );
    if( id.is_nan() || id.is_inf() )
        return false;

    // set the bits

    int l = j;
    for( int k = 0; k < static_cast<int>( bv.length() ); ++ k )
    {
	if( bv[k] )
	{
	    if( ! get_bit( l ) )
	    {
		if( _params.enc() == SC_TC && l == _params.iwl() - 1 )
		    _val -= scfx_pow2( l );
		else
		    _val += scfx_pow2( l );
	    }
	}
	else
	{
	    if( get_bit( l ) )
	    {
		if( _params.enc() == SC_TC && l == _params.iwl() - 1 )
		    _val += scfx_pow2( l );
		else
		    _val -= scfx_pow2( l );
	    }
	}


	if( i >= j )
	    ++ l;
	else
	    -- l;
    }

    return true;
}


sc_fxnum_fast_observer*
sc_fxnum_fast::lock_observer() const
{
    _SC_ASSERT( _observer != 0, "lock observer failed" );
    sc_fxnum_fast_observer* tmp = _observer;
    _observer = 0;
    return tmp;
}

void
sc_fxnum_fast::unlock_observer( sc_fxnum_fast_observer* observer_ ) const
{
    _SC_ASSERT( observer_ != 0, "unlock observer failed" );
    _observer = observer_;
}


// Taf!
