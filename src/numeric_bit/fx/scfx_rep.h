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

    scfx_rep.h - 

    Original Author: Robert Graulich. Synopsys, Inc. (graulich@synopsys.com)
                     Martin Janssen.  Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_REP_H
#define SCFX_REP_H


#include "scfx_mant.h"
#include "scfx_params.h"
#include "scfx_string.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

namespace sc_bv_ns
{
    class sc_bv_base;
};
using sc_bv_ns::sc_bv_base;

#include <limits.h>

class sc_signed;
class sc_unsigned;


const int min_mant = 4;

const int bits_in_int  = sizeof(int)  * CHAR_BIT;
const int bits_in_word = sizeof(word) * CHAR_BIT;


// ----------------------------------------------------------------------------
//  CLASS : scfx_index
// ----------------------------------------------------------------------------

class scfx_index
{

public:

    scfx_index( int wi_, int bi_ ) : _wi( wi_ ), _bi( bi_ ) {}

    int wi() const { return _wi; }
    int bi() const { return _bi; }

    void wi( int wi_ ) { _wi = wi_; }

private:

    int _wi;
    int _bi;

};


// ----------------------------------------------------------------------------
//  CLASS : scfx_rep
//
//  Arbitrary-precision fixed-point implementation class.
// ----------------------------------------------------------------------------

class scfx_rep
{
    enum state
    {
        normal,
        infinity,
        not_a_number
    };

public:

    // constructors

             scfx_rep();
    explicit scfx_rep( int );
    explicit scfx_rep( unsigned int );
    explicit scfx_rep( long );
    explicit scfx_rep( unsigned long );
    explicit scfx_rep( double );
    explicit scfx_rep( const char* );
    explicit scfx_rep( int64 );
    explicit scfx_rep( uint64 );
    explicit scfx_rep( const sc_signed& );
    explicit scfx_rep( const sc_unsigned& );


    // copy constructor

             scfx_rep( const scfx_rep& );


    // destructor

    ~scfx_rep();


    void* operator new( size_t );
    void  operator delete( void*, size_t );


    void from_string( const char*, int );

    double to_double() const;

    const char* to_string( sc_numrep, sc_fmt, const scfx_params* = 0 ) const;


    // assignment operator

    void operator = ( const scfx_rep& );

    friend void multiply( scfx_rep&, const scfx_rep&, const scfx_rep&,
			  int = _SC_DEFAULT_MAX_WL );

    friend scfx_rep*  neg__scfx_rep( const scfx_rep& );
    friend scfx_rep* mult__scfx_rep( const scfx_rep&, const scfx_rep&,
				     int = _SC_DEFAULT_MAX_WL );
    friend scfx_rep*  div__scfx_rep( const scfx_rep&, const scfx_rep&,
				     int = _SC_DEFAULT_DIV_WL );
    friend scfx_rep*  add__scfx_rep( const scfx_rep&, const scfx_rep&,
				     int = _SC_DEFAULT_MAX_WL );
    friend scfx_rep*  sub__scfx_rep( const scfx_rep&, const scfx_rep&,
				     int = _SC_DEFAULT_MAX_WL );
    friend scfx_rep*  lsh__scfx_rep( const scfx_rep&, int );
    friend scfx_rep*  rsh__scfx_rep( const scfx_rep&, int );

    void lshift( int );
    void rshift( int );

    friend int        cmp__scfx_rep( const scfx_rep&, const scfx_rep& );

    void cast( const scfx_params&, bool&, bool& );

    bool is_neg() const;
    bool is_zero() const;
    bool is_nan() const;
    bool is_inf() const;
    bool is_normal() const;

    void set_zero( int = 1 );
    void set_nan();
    void set_inf( int );

    bool   get_bit( int ) const;
    bool   set( int, const scfx_params& );
    bool clear( int, const scfx_params& );

    bool get_slice( int, int, const scfx_params&, sc_bv_base& ) const;
    bool set_slice( int, int, const scfx_params&, const sc_bv_base& );

    void print( ostream& ) const;
    void dump( ostream& ) const;

    void get_type( int&, int&, sc_enc& ) const;

    friend scfx_rep* quantization__scfx_rep( const scfx_rep&,
					     const scfx_params&,
					     bool& );
    friend scfx_rep*     overflow__scfx_rep( const scfx_rep&,
					     const scfx_params&,
					     bool& );

    bool rounding_flag() const;

private:

    friend void  align( const scfx_rep&, const scfx_rep&, int&, int&,
			scfx_mant_ref&, scfx_mant_ref& );
    friend int   compare_msw( const scfx_rep&, const scfx_rep& );
    friend int   compare_msw_ff( const scfx_rep& lhs, const scfx_rep& rhs );
    unsigned int divide_by_ten();
    int          find_lsw() const;
    int          find_msw() const;
    void         find_sw();
    void         multiply_by_ten();
    void         normalize( int );
    scfx_mant*   resize( int, int ) const;
    void         set_bin( int );
    void         set_oct( int, int );
    void         set_hex( int, int );
    void         shift_left( int );
    void         shift_right( int );

    const scfx_index calc_indices( int ) const;

    void o_extend( const scfx_index&, sc_enc );
    bool o_bit_at( const scfx_index& ) const;
    bool o_zero_left( const scfx_index& ) const;
    bool o_zero_right( const scfx_index& ) const;
    void o_set_low( const scfx_index&, sc_enc );
    void o_set_high( const scfx_index&, const scfx_index&, sc_enc, int = 1 );
    void o_set( const scfx_index&, const scfx_index&, sc_enc, bool );
    void o_invert( const scfx_index& );
    bool q_bit( const scfx_index& ) const;
    void q_clear( const scfx_index& );
    void q_incr( const scfx_index& );
    bool q_odd( const scfx_index& ) const;
    bool q_zero( const scfx_index& ) const;

    void resize_to( int, int = 0 );
    int  size() const;
    void toggle_tc();

    friend void print_dec( scfx_string&, const scfx_rep&, sc_fmt );
    friend void print_other( scfx_string&, const scfx_rep&, sc_numrep, sc_fmt,
			     const scfx_params* );

    void quantization( const scfx_params&, bool& );
    void     overflow( const scfx_params&, bool& );

    friend int compare_abs( const scfx_rep&, const scfx_rep& );

    void round( int );

private:

    scfx_mant _mant;
    int       _wp;
    int       _sign;
    state     _state;
    int       _msw;
    int       _lsw;
    bool      _r_flag;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
scfx_rep::set_zero( int sign )
{
    _mant.clear();
    _wp = _msw = _lsw = 0;
    _sign = sign;
    _state = normal;
}

inline
void
scfx_rep::set_nan()
{
    _mant.resize_to( min_mant );
    _state = not_a_number;
}

inline
void
scfx_rep::set_inf( int sign )
{
    _mant.resize_to( min_mant );
    _state = infinity;
    _sign = sign;
}


// constructors

inline
scfx_rep::scfx_rep( const char* s )
: _mant( min_mant ), _wp( 2 ), _sign( 1 ), _state( normal ), _r_flag( false )
{
    from_string( s, _SC_DEFAULT_CTE_WL );
}


// destructor

inline
scfx_rep::~scfx_rep()
{}


// assignment operator

inline
void
scfx_rep::operator = ( const scfx_rep& f )
{
    if( &f != this )
    {
        _mant  = f._mant;
	_wp    = f._wp;
	_sign  = f._sign;
	_state = f._state;
	_msw   = f._msw;
	_lsw   = f._lsw;
	round( _SC_DEFAULT_MAX_WL );
    }
}

inline
scfx_rep*
neg__scfx_rep( const scfx_rep& a )
{
    scfx_rep& c = *new scfx_rep( a );
    c._sign = - c._sign;
    return &c;
}

inline
scfx_rep*
mult__scfx_rep( const scfx_rep& a, const scfx_rep& b, int max_wl )
{
    scfx_rep& c = *new scfx_rep;
    ::multiply( c, a, b, max_wl );
    return &c;
}

inline
scfx_rep*
lsh__scfx_rep( const scfx_rep& a, int b )
{
    scfx_rep& c = *new scfx_rep( a );
    c.lshift( b );
    return &c;
}

inline
scfx_rep*
rsh__scfx_rep( const scfx_rep& a, int b )
{
    scfx_rep& c = *new scfx_rep( a );
    c.rshift( b );
    return &c;
}

inline
int
scfx_rep::size() const
{
    return _mant.size();
}

inline
bool
scfx_rep::is_neg() const
{
    return ( _sign == -1 );
}

inline
bool
scfx_rep::is_zero() const
{
    if( _state != normal )
        return false;

    for( int i = 0; i < size(); i ++ )
    {
        if( _mant[i] )
	    return false;
    }

    return true;
}

inline
bool
scfx_rep::is_nan() const
{
    return ( _state == not_a_number );
}

inline
bool
scfx_rep::is_inf() const
{
    return ( _state == infinity );
}

inline
bool
scfx_rep::is_normal() const
{
    return ( _state == normal );
}

inline
scfx_rep*
quantization__scfx_rep( const scfx_rep& a,
			const scfx_params& params,
			bool& q_flag )
{
    scfx_rep& c = *new scfx_rep( a );
    c.quantization( params, q_flag );
    return &c;
}

inline
scfx_rep*
overflow__scfx_rep( const scfx_rep& a,
		    const scfx_params& params,
		    bool& o_flag )
{
    scfx_rep& c = *new scfx_rep( a );
    c.overflow( params, o_flag );
    return &c;
}

inline
bool
scfx_rep::rounding_flag() const
{
    return _r_flag;
}

inline
void
scfx_rep::resize_to( int new_size, int restore )
{
    if( restore == -1 )
    {
        int size_incr = new_size - size();
	_wp += size_incr;
	_msw += size_incr;
	_lsw += size_incr;
    }
    _mant.resize_to( new_size, restore );
}

inline
const scfx_index
scfx_rep::calc_indices( int n ) const
{
    int wi = n / bits_in_word + _wp;
    int bi = n % bits_in_word;

    if( bi < 0 )
    {
        bi += bits_in_word;
	-- wi;
    }

    return scfx_index( wi, bi );
}

inline
void
scfx_rep::o_extend( const scfx_index& x, sc_enc enc )
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );
    
    if( enc == SC_US || ( _mant[wi] & ( 1 << bi ) ) == 0 )
    {
        if( bi != bits_in_word - 1 )
	    _mant[wi] &= ~( -1 << ( bi + 1 ) );
	for( int i = wi + 1; i < size(); ++ i )
	    _mant[i] = 0;
	_sign = 1;
    }
    else
    {
        if( bi != bits_in_word - 1 )
	    _mant[wi] |= ( -1 << ( bi + 1 ) );
	for( int i = wi + 1; i < size(); ++ i )
	    _mant[i] = static_cast<word>( -1 );
	_sign = -1;
    }
}

inline
bool
scfx_rep::o_bit_at( const scfx_index& x ) const
{
    int wi = x.wi();
    int bi = x.bi();
    
    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    return ( _mant[wi] & ( 1 << bi ) ) != 0;
}

inline
bool
scfx_rep::o_zero_left( const scfx_index& x ) const
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    bool zero = true;
    if( bi != bits_in_word - 1 )
        zero = ( _mant[wi] & ( -1 << ( bi + 1 ) ) ) == 0;
    for( int i = wi + 1; i < size(); ++ i )
	zero = zero && _mant[i] == 0;

    return zero;
}

inline
bool
scfx_rep::o_zero_right( const scfx_index& x ) const
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    bool zero = ( _mant[wi] & ~( -1 << bi ) ) == 0;
    for( int i = wi - 1; i >= 0; -- i )
	zero = zero && _mant[i] == 0;

    return zero;
}

inline
void
scfx_rep::o_set_low( const scfx_index& x, sc_enc enc )
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    _mant.clear();

    if( enc == SC_TC )
    {
	_mant[wi] |= ( 1 << bi );
	_sign = -1;
    }
    else
	_sign = 1;
}

inline
void
scfx_rep::o_set_high( const scfx_index& x, const scfx_index& x2,
		      sc_enc enc, int sign )
{
    int wi = x.wi();
    int bi = x.bi();
    int wi2 = x2.wi();
    int bi2 = x2.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );
    _SC_ASSERT( wi2 >= 0 && wi2 < size(), "word index out of range" );
    
    int i;

    for( i = 0; i < size(); ++ i )
	_mant[i] = static_cast<word>( -1 );

    _mant[wi] &= ~( -1 << bi );
    for( i = wi + 1; i < size(); ++ i )
	_mant[i] = 0;

    _mant[wi2] &= ( -1 << bi2 );
    for( i = wi2 - 1; i >= 0; -- i )
	_mant[i] = 0;
    
    if( enc == SC_TC )
	_sign = sign;
    else
    {
	_mant[wi] |= ( 1 << bi );
	_sign = 1;
    }
}

inline
void
scfx_rep::o_set( const scfx_index& x, const scfx_index& x3,
		 sc_enc enc, bool under )
{
    int wi = x.wi();
    int bi = x.bi();
    int wi3 = x3.wi();
    int bi3 = x3.bi();
    
    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );
    _SC_ASSERT( wi3 >= 0 && wi3 < size(), "word index out of range" );

    if( bi3 != bits_in_word - 1 )
    {
	if( under )
	    _mant[wi3] &= ~( -1 << ( bi3 + 1 ) );
	else
	    _mant[wi3] |= ( -1 << ( bi3 + 1 ) );
    }
    for( int i = wi3 + 1; i < size(); ++ i )
    {
	if( under )
	    _mant[i] = 0;
	else
	    _mant[i] = static_cast<word>( -1 );
    }
	
    if( enc == SC_TC )
    {
	if( under )
	    _mant[wi] |= ( 1 << bi );
	else
	    _mant[wi] &= ~( 1 << bi );
    }
}

inline
void
scfx_rep::o_invert( const scfx_index& x2 )
{
    int wi2 = x2.wi();
    int bi2 = x2.bi();

    _mant[wi2] ^= ( -1 << bi2 );
    for( int i = wi2 + 1; i < size(); ++ i )
	_mant[i] = ~ _mant[i];
}

inline
bool
scfx_rep::q_bit( const scfx_index& x ) const
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    if( bi != 0 )
        return ( _mant[wi] & ( 1 << ( bi - 1 ) ) ) != 0;
    else if( wi != 0 )
        return ( _mant[wi - 1] & ( 1 << ( bits_in_word - 1 ) ) ) != 0;
    else
        return false;
}

inline
void
scfx_rep::q_clear( const scfx_index& x )
{
    int wi = x.wi();
    int bi = x.bi();
    
    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    _mant[wi] &= ( -1 << bi );
    for( int i = wi - 1; i >= 0; -- i )
        _mant[i] = 0;
}

inline
void
scfx_rep::q_incr( const scfx_index& x )
{
    int wi = x.wi();
    int bi = x.bi();
    
    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    word old_val = _mant[wi];
    _mant[wi] += ( 1 << bi );
    if( _mant[wi] <= old_val )
    {
        if( wi + 1 == size() )
          resize_to( size() + 1, 1 );

        for( int i = wi + 1; i < size(); ++ i )
	{
	    if( ++ _mant[i] != 0 )
	        break;
	}
    }
}

inline
bool
scfx_rep::q_odd( const scfx_index& x ) const
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    return ( _mant[wi] & ( 1 << bi ) ) != 0;
}

inline
bool
scfx_rep::q_zero( const scfx_index& x ) const
{
    int wi = x.wi();
    int bi = x.bi();

    _SC_ASSERT( wi >= 0 && wi < size(), "word index out of range" );

    bool zero;

    if( bi != 0 )
    {
        zero = ( _mant[wi] & ~( -1 << (bi - 1) ) ) == 0;
	for( int i = wi - 1; i >= 0; -- i )
	    zero = zero && _mant[i] == 0;
    }
    else if( wi != 0 )
    {
        zero = ( _mant[wi - 1] & ~( -1 << (bits_in_word - 1) ) ) == 0;
	for( int i = wi - 2; i >= 0; -- i )
	    zero = zero && _mant[i] == 0;
    }
    else
        zero = true;

    return zero;
}

inline
int
scfx_rep::find_lsw() const
{
    for( int i = 0; i < size(); i ++ )
    {
        if( _mant[i] )
	    return i;
    }
    return 0;
}

inline
int
scfx_rep::find_msw() const
{
    for( int i = size() - 1; i >= 0; i -- )
    {
        if( _mant[i] )
	    return i;
    }
    return 0;
}

inline
void
scfx_rep::find_sw()
{
    _lsw = find_lsw();
    _msw = find_msw();
}

inline
void
scfx_rep::toggle_tc()
{
    if( is_neg() )
    {
        complement( _mant, _mant, _mant.size() );
	inc( _mant );
    }
}


#endif

// Taf!
