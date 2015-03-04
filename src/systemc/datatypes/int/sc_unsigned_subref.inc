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

  sc_unsigned_subref.h -- Proxy class that is declared in sc_unsigned.h.  

  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref_r
//
//  Proxy class for sc_unsigned part selection (r-value only).
// ----------------------------------------------------------------------------

// implicit conversion to sc_unsigned

sc_unsigned_subref_r::operator sc_unsigned () const
{
    return sc_unsigned( &m_obj, m_left, m_right );
}


// explicit conversions

int
sc_unsigned_subref_r::to_int() const
{
    sc_unsigned a(  &m_obj, m_left, m_right );
    return a.to_int();
}

unsigned int
sc_unsigned_subref_r::to_uint() const
{
    sc_unsigned a( &m_obj, m_left, m_right );
    return a.to_uint();
}

long
sc_unsigned_subref_r::to_long() const
{
    sc_unsigned a( &m_obj, m_left, m_right );
    return a.to_long();
}

unsigned long
sc_unsigned_subref_r::to_ulong() const
{
    sc_unsigned a( &m_obj, m_left, m_right );
    return a.to_ulong();
}

int64
sc_unsigned_subref_r::to_int64() const
{
    sc_unsigned a( &m_obj, m_left, m_right );
    return a.to_int64();
}

uint64
sc_unsigned_subref_r::to_uint64() const
{
    sc_unsigned a( &m_obj, m_left, m_right );
    return a.to_uint64();
}

double
sc_unsigned_subref_r::to_double() const
{
    sc_unsigned a( &m_obj, m_left, m_right );
    return a.to_double();
}


// explicit conversion to character string

const sc_string
sc_unsigned_subref_r::to_string( sc_numrep numrep ) const
{
    sc_unsigned a( length() );
    a = *this;
    return a.to_string( numrep );
}

const sc_string
sc_unsigned_subref_r::to_string( sc_numrep numrep, bool w_prefix ) const
{
    sc_unsigned a( length() );
    a = *this;
    return a.to_string( numrep, w_prefix );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref
//
//  Proxy class for sc_unsigned part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( const sc_unsigned_subref_r& a )
{
    return operator = ( SCAST<sc_unsigned>( a ) );
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( const sc_unsigned_subref& a )
{
    if( this == &a ) {
	return *this;
    }
    return operator = ( static_cast<sc_unsigned>( a ) );
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( const sc_unsigned& v )
{
    int l = sc_min( m_left, v.nbits - 1 + m_right );
    for( int i = m_right; i <= l; ++ i ) {
	m_obj.set( i, v.test( i - m_right ) );
    }
    return *this;
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( const sc_signed_subref_r& v )
{
    return operator = ( static_cast<sc_signed>( v ) );
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( const sc_signed& v )
{
    int l = sc_min( m_left, v.nbits - 1 + m_right );
    for( int i = m_right; i <= l; ++ i ) {
	m_obj.set( i, v.test( i - m_right ) );
    }
    return *this;
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( unsigned long v )
{
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj.set( i, static_cast<bool>( v & 1 ) );
	v >>= 1;
    }
    return *this;
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( long v ) 
{
    unsigned long v2 = (unsigned long) v;
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj.set( i, static_cast<bool>( v2 & 1 ) );
	v2 >>= 1;
    }
    return *this;
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( uint64  v )
{
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj.set( i, static_cast<bool>( v & 1 ) );
	v >>= 1;
    }
    return *this;
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( int64 v ) 
{
    uint64 v2 = (uint64) v;
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj.set( i, static_cast<bool>( v2 & 1 ) );
	v2 >>= 1;
    }
    return *this;
}

sc_unsigned_subref& 
sc_unsigned_subref::operator = ( double v )
{
    is_bad_double(v);

    int nb = m_left - m_right + 1;
    int nd = DIV_CEIL(nb);

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[nd];
#endif

    if (v < 0)
	v = -v;

    register int i = 0;

    while (floor(v) && (i < nd)) {
#ifndef WIN32
	d[i++] = (unsigned long) floor(remainder(v, DIGIT_RADIX));
#else
	d[i++] = (unsigned long) floor(fmod(v, DIGIT_RADIX));
#endif
	v /= DIGIT_RADIX;
    }

    vec_zero(i, nd, d);

    unsigned long val = 1;  // Bit value.
    int j = 0;   // Current digit in d.

    i = 0;  // Current bit in d.

    while (i < nb) {

	m_obj.set(i + m_right, (bool) (d[j] & val));

	++i;

	if (i % BITS_PER_DIGIT == 0) {
	    val = 1;
	    ++j;
	}
	else
	    val <<= 1;
    }

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

    return *this;
}

sc_unsigned_subref&
sc_unsigned_subref::operator = ( const sc_int_base& a )
{
    return operator = ( (int64) a );
}

sc_unsigned_subref&
sc_unsigned_subref::operator = ( const sc_uint_base& a )
{
    return operator = ( (uint64) a );
}


// other methods

void
sc_unsigned_subref::scan( istream& is )
{
    sc_string s;
    is >> s;
    *this = s.c_str();
}


// End of file
