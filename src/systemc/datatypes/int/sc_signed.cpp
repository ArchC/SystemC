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

  sc_signed.cpp -- Arbitrary precision signed arithmetic.
 
                   This file includes the definitions of sc_signed_bitref,
                   sc_signed_subref, and sc_signed classes. The first two
                   classes are proxy classes to reference one bit and a range
                   of bits of a sc_signed number, respectively. This file also
                   includes sc_nbcommon.cpp and sc_nbfriends.cpp, which
                   contain the definitions shared by sc_unsigned.

  Original Author: Ali Dasdan, Synopsys, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <ctype.h>
#include <math.h>

#include "systemc/kernel/sc_cmnhdr.h"
#include "systemc/kernel/sc_macros.h"
#include "systemc/datatypes/int/sc_signed.h"
#include "systemc/datatypes/int/sc_unsigned.h"
#include "systemc/datatypes/int/sc_int_base.h"
#include "systemc/datatypes/int/sc_uint_base.h"
#include "systemc/datatypes/int/sc_int_ids.h"
#include "systemc/datatypes/bit/sc_bv_base.h"
#include "systemc/datatypes/bit/sc_lv_base.h"
#include "systemc/datatypes/fx/sc_fix.h"
#include "systemc/datatypes/fx/scfx_other_defs.h"
#include "systemc/utils/sc_exception.h"


namespace sc_dt
{

// ----------------------------------------------------------------------------
//  SECTION: Public members.
// ----------------------------------------------------------------------------

// The public members are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Public members - Assignment operators.
// ----------------------------------------------------------------------------

// assignment operators

sc_signed&
sc_signed::operator = ( const char* a )
{
    if( a == 0 ) {
	SC_REPORT_ERROR( SC_ID_CONVERSION_FAILED_,
			 "character string is zero" );
    }
    if( *a == 0 ) {
	SC_REPORT_ERROR( SC_ID_CONVERSION_FAILED_,
			 "character string is empty" );
    }
    try {
	int len = length();
	sc_fix aa( a, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
	return this->operator = ( aa );
    } catch( sc_exception ) {
	char msg[BUFSIZ];
	sprintf( msg, "character string '%s' is not valid", a );
	SC_REPORT_ERROR( SC_ID_CONVERSION_FAILED_, msg );
	// never reached
	return *this;
    }
}

sc_signed&
sc_signed::operator=(int64 v)
{
  sgn = get_sign(v);
  // v >= 0 now.
  if (sgn == SC_ZERO)
    vec_zero(ndigits, digit);
  else {
    from_uint(ndigits, digit, (uint64) v);
    if (nbits <= BITS_PER_INT64)
      convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_signed&
sc_signed::operator=(uint64 v)
{
  sgn = get_sign(v);
  if (sgn == SC_ZERO)
    vec_zero(ndigits, digit);
  else {
    from_uint(ndigits, digit, v);
    if (nbits <= BITS_PER_INT64)
      convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_signed&
sc_signed::operator=(long v)
{
  sgn = get_sign(v);
  // v >= 0 now.
  if (sgn == SC_ZERO)
    vec_zero(ndigits, digit);
  else {
    from_uint(ndigits, digit, (unsigned long) v);
    if (nbits <= BITS_PER_LONG)
      convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_signed&
sc_signed::operator=(unsigned long v)
{
  sgn = get_sign(v);
  if (sgn == SC_ZERO)
    vec_zero(ndigits, digit);
  else {
    from_uint(ndigits, digit, v);
    if (nbits <= BITS_PER_LONG)
      convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_signed&
sc_signed::operator=(double v)
{
  is_bad_double(v);
  if (v < 0) {
    v = -v;
    sgn = SC_NEG;
  }
  else
    sgn = SC_POS;
  register int i = 0;
  while (floor(v) && (i < ndigits)) {
#ifndef WIN32
    digit[i++] = (unsigned long) floor(remainder(v, DIGIT_RADIX));
#else
    digit[i++] = (unsigned long) floor(fmod(v, DIGIT_RADIX));
#endif
    v /= DIGIT_RADIX;
  }
  vec_zero(i, ndigits, digit);
  convert_SM_to_2C_to_SM();
  return *this;  
}


// ----------------------------------------------------------------------------

sc_signed&
sc_signed::operator = ( const sc_bv_base& v )
{
    int minlen = sc_min( nbits, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	safe_set( i, v.get_bit( i ), digit );
    }
    for( ; i < nbits; ++ i ) {
	safe_set( i, 0, digit );  // zero-extend
    }
    convert_2C_to_SM();
    return *this;
}

sc_signed&
sc_signed::operator = ( const sc_lv_base& v )
{
    int minlen = sc_min( nbits, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	safe_set( i, sc_logic( v.get_bit( i ) ).to_bool(), digit );
    }
    for( ; i < nbits; ++ i ) {
	safe_set( i, 0, digit );  // zero-extend
    }
    convert_2C_to_SM();
    return *this;
}


// explicit conversion to character string

const sc_string
sc_signed::to_string( sc_numrep numrep ) const
{
    int len = length();
    sc_fix aa( *this, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
    return aa.to_string( numrep );
}

const sc_string
sc_signed::to_string( sc_numrep numrep, bool w_prefix ) const
{
    int len = length();
    sc_fix aa( *this, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
    return aa.to_string( numrep, w_prefix );
}


// ----------------------------------------------------------------------------
//  SECTION: Interfacing with sc_int_base
// ----------------------------------------------------------------------------

sc_signed&
sc_signed::operator = (const sc_int_base& v)
{ return operator=((int64) v); }


sc_signed
operator + ( const sc_unsigned& u, const sc_int_base& v )
{ return operator + ( u, SCAST<int64>( v ) ); }

sc_signed
operator + ( const sc_int_base& u, const sc_unsigned& v )
{ return operator + ( SCAST<int64>( u ), v ); }

sc_signed 
operator + (const sc_signed& u, const sc_int_base& v) 
{ return operator+(u, (int64) v); }

sc_signed 
operator + (const sc_int_base& u, const sc_signed& v) 
{ return operator+((int64) u, v); }

sc_signed& 
sc_signed::operator += (const sc_int_base& v)
{ return operator+=((int64) v); }


sc_signed 
operator - (const sc_unsigned& u, const sc_int_base& v) 
{ return operator-(u, (int64) v); }

sc_signed 
operator - (const sc_int_base& u, const sc_unsigned& v) 
{ return operator-((int64) u, v); }

sc_signed 
operator-(const sc_signed& u, const sc_int_base& v) 
{ return operator-(u, (int64) v); }

sc_signed 
operator - (const sc_int_base& u, const sc_signed& v) 
{ return operator-((int64) u, v); }

sc_signed& 
sc_signed::operator -= (const sc_int_base& v)
{ return operator-=((int64) v); }


sc_signed
operator * ( const sc_unsigned& u, const sc_int_base& v )
{ return operator * ( u, SCAST<int64>( v ) ); }

sc_signed
operator * ( const sc_int_base& u, const sc_unsigned& v )
{ return operator * ( SCAST<int64>( u ), v ); }

sc_signed 
operator * (const sc_signed& u, const sc_int_base& v) 
{ return operator*(u, (int64) v); }

sc_signed 
operator * (const sc_int_base& u, const sc_signed& v) 
{ return operator*((int64) u, v); }

sc_signed& 
sc_signed::operator *= (const sc_int_base& v)
{ return operator*=((int64) v); }


sc_signed
operator / ( const sc_unsigned& u, const sc_int_base& v )
{ return operator / ( u, SCAST<int64>( v ) ); }

sc_signed
operator / ( const sc_int_base& u, const sc_unsigned& v )
{ return operator / ( SCAST<int64>( u ), v ); }

sc_signed 
operator / (const sc_signed& u, const sc_int_base& v) 
{ return operator/(u, (int64) v); }

sc_signed 
operator / (const sc_int_base& u, const sc_signed& v) 
{ return operator/((int64) u, v); }

sc_signed& 
sc_signed::operator /= (const sc_int_base& v)
{ return operator/=((int64) v); }


sc_signed
operator % ( const sc_unsigned& u, const sc_int_base& v )
{ return operator % ( u, SCAST<int64>( v ) ); }

sc_signed
operator % ( const sc_int_base& u, const sc_unsigned& v )
{ return operator % ( SCAST<int64>( u ), v ); }

sc_signed 
operator % (const sc_signed& u, const sc_int_base& v) 
{ return operator%(u, (int64) v); }

sc_signed 
operator % (const sc_int_base& u, const sc_signed& v) 
{ return operator%((int64) u, v); }

sc_signed& 
sc_signed::operator %= (const sc_int_base& v)
{ return operator%=((int64) v); }


sc_signed
operator & ( const sc_unsigned& u, const sc_int_base& v )
{ return operator & ( u, SCAST<int64>( v ) ); }

sc_signed
operator & ( const sc_int_base& u, const sc_unsigned& v )
{ return operator & ( SCAST<int64>( u ), v ); }

sc_signed 
operator & (const sc_signed& u, const sc_int_base& v) 
{ return operator&(u, (int64) v); }

sc_signed 
operator & (const sc_int_base& u, const sc_signed& v) 
{ return operator&((int64) u, v); }

sc_signed& 
sc_signed::operator &= (const sc_int_base& v)
{ return operator&=((int64) v); }


sc_signed
operator | ( const sc_unsigned& u, const sc_int_base& v )
{ return operator | ( u, SCAST<int64>( v ) ); }

sc_signed
operator | ( const sc_int_base& u, const sc_unsigned& v )
{ return operator | ( SCAST<int64>( u ), v ); }

sc_signed 
operator | (const sc_signed& u, const sc_int_base& v) 
{ return operator|(u, (int64) v); }

sc_signed 
operator | (const sc_int_base& u, const sc_signed& v) 
{ return operator|((int64) u, v); }

sc_signed& 
sc_signed::operator |= (const sc_int_base& v)
{ return operator|=((int64) v); }


sc_signed
operator ^ ( const sc_unsigned& u, const sc_int_base& v )
{ return operator ^ ( u, SCAST<int64>( v ) ); }

sc_signed
operator ^ ( const sc_int_base& u, const sc_unsigned& v )
{ return operator ^ ( SCAST<int64>( u ), v ); }

sc_signed 
operator ^ (const sc_signed& u, const sc_int_base& v) 
{ return operator^(u, (int64) v); }

sc_signed 
operator ^ (const sc_int_base& u, const sc_signed& v) 
{ return operator^((int64) u, v); }

sc_signed& 
sc_signed::operator ^= (const sc_int_base& v)
{ return operator^=((int64) v); }


sc_signed 
operator << (const sc_signed& u, const sc_int_base& v)
{ return operator<<(u, (int64) v); }

sc_signed& 
sc_signed::operator <<= (const sc_int_base& v)
{ return operator<<=((int64) v); }


sc_signed 
operator >> (const sc_signed&    u, const sc_int_base&  v)
{ return operator>>(u, (int64) v); }

sc_signed& 
sc_signed::operator >>= (const sc_int_base&  v)
{ return operator>>=((int64) v); }


bool 
operator == (const sc_signed& u, const sc_int_base& v)
{ return operator==(u, (int64) v); }

bool 
operator == (const sc_int_base& u, const sc_signed& v) 
{ return operator==((int64) u, v); }


bool 
operator != (const sc_signed& u, const sc_int_base& v)
{ return operator!=(u, (int64) v); }

bool 
operator != (const sc_int_base& u, const sc_signed& v) 
{ return operator!=((int64) u, v); }


bool 
operator < (const sc_signed& u, const sc_int_base& v)
{ return operator<(u, (int64) v); }

bool 
operator < (const sc_int_base& u, const sc_signed& v) 
{ return operator<((int64) u, v); }


bool 
operator <= (const sc_signed& u, const sc_int_base& v)
{ return operator<=(u, (int64) v); }

bool
operator <= (const sc_int_base& u, const sc_signed& v) 
{ return operator<=((int64) u, v); }


bool 
operator > (const sc_signed& u, const sc_int_base& v)
{ return operator>(u, (int64) v); }

bool 
operator > (const sc_int_base& u, const sc_signed& v) 
{ return operator>((int64) u, v); }


bool 
operator >= (const sc_signed& u, const sc_int_base& v)
{ return operator>=(u, (int64) v); }

bool 
operator >= (const sc_int_base& u, const sc_signed& v) 
{ return operator>=((int64) u, v); }


// ----------------------------------------------------------------------------
//  SECTION: Interfacing with sc_uint_base
// ----------------------------------------------------------------------------

sc_signed&
sc_signed::operator = (const sc_uint_base& v)
{ return operator=((uint64) v); }


sc_signed
operator + (const sc_signed& u, const sc_uint_base& v) 
{ return operator+(u, (uint64) v); }

sc_signed
operator + (const sc_uint_base& u, const sc_signed& v) 
{ return operator+((uint64) u, v); }

sc_signed&
sc_signed::operator += (const sc_uint_base& v)
{ return operator+=((uint64) v); }


sc_signed
operator - (const sc_unsigned& u, const sc_uint_base& v) 
{ return operator-(u, (uint64) v); }

sc_signed
operator - (const sc_uint_base& u, const sc_unsigned& v) 
{ return operator-((uint64) u, v); }

sc_signed
operator - (const sc_signed& u, const sc_uint_base& v) 
{ return operator-(u, (uint64) v); }

sc_signed
operator - (const sc_uint_base& u, const sc_signed& v) 
{ return operator-((uint64) u, v); }

sc_signed&
sc_signed::operator -= (const sc_uint_base& v)
{ return operator-=((uint64) v); }


sc_signed 
operator * (const sc_signed& u, const sc_uint_base& v) 
{ return operator*(u, (uint64) v); }

sc_signed 
operator * (const sc_uint_base& u, const sc_signed& v) 
{ return operator*((uint64) u, v); }

sc_signed& 
sc_signed::operator *= (const sc_uint_base& v)
{ return operator*=((uint64) v); }


sc_signed
operator / (const sc_signed& u, const sc_uint_base& v) 
{ return operator/(u, (uint64) v); }

sc_signed
operator / (const sc_uint_base& u, const sc_signed& v) 
{ return operator/((uint64) u, v); }

sc_signed&
sc_signed::operator /= (const sc_uint_base& v)
{ return operator/=((uint64) v); }


sc_signed 
operator % (const sc_signed& u, const sc_uint_base& v) 
{ return operator%(u, (uint64) v); }

sc_signed 
operator % (const sc_uint_base& u, const sc_signed& v) 
{ return operator%((uint64) u, v); }

sc_signed& 
sc_signed::operator %= (const sc_uint_base& v)
{ return operator%=((uint64) v); }


sc_signed 
operator & (const sc_signed& u, const sc_uint_base& v) 
{ return operator&(u, (uint64) v); }

sc_signed 
operator & (const sc_uint_base& u, const sc_signed& v) 
{ return operator&((uint64) u, v); }

sc_signed& 
sc_signed::operator &= (const sc_uint_base& v)
{ return operator&=((uint64) v); }


sc_signed 
operator | (const sc_signed& u, const sc_uint_base& v) 
{ return operator|(u, (uint64) v); }

sc_signed 
operator | (const sc_uint_base& u, const sc_signed& v) 
{ return operator|((uint64) u, v); }

sc_signed& 
sc_signed::operator |= (const sc_uint_base& v)
{ return operator|=((uint64) v); }


sc_signed 
operator ^ (const sc_signed& u, const sc_uint_base& v) 
{ return operator^(u, (uint64) v); }

sc_signed 
operator ^ (const sc_uint_base& u, const sc_signed& v) 
{ return operator^((uint64) u, v); }

sc_signed& 
sc_signed::operator ^= (const sc_uint_base& v)
{ return operator^=((uint64) v); }


sc_signed 
operator << (const sc_signed& u, const sc_uint_base& v)
{ return operator<<(u, (uint64) v); }

sc_signed& 
sc_signed::operator <<= (const sc_uint_base& v)
{ return operator<<=((uint64) v); }


sc_signed
operator >> (const sc_signed&    u, const sc_uint_base&  v)
{ return operator>>(u, (uint64) v); }

sc_signed& 
sc_signed::operator >>= (const sc_uint_base&  v)
{ return operator>>=((uint64) v); }


bool 
operator == (const sc_signed& u, const sc_uint_base& v)
{ return operator==(u, (uint64) v); }

bool 
operator == (const sc_uint_base& u, const sc_signed& v) 
{ return operator==((uint64) u, v); }


bool 
operator != (const sc_signed& u, const sc_uint_base& v)
{ return operator!=(u, (uint64) v); }

bool 
operator != (const sc_uint_base& u, const sc_signed& v) 
{ return operator!=((uint64) u, v); }


bool 
operator < (const sc_signed& u, const sc_uint_base& v)
{ return operator<(u, (uint64) v); }

bool 
operator < (const sc_uint_base& u, const sc_signed& v) 
{ return operator<((uint64) u, v); }


bool 
operator <= (const sc_signed& u, const sc_uint_base& v)
{ return operator<=(u, (uint64) v); }

bool 
operator <= (const sc_uint_base& u, const sc_signed& v) 
{ return operator<=((uint64) u, v); }


bool 
operator > (const sc_signed& u, const sc_uint_base& v)
{ return operator>(u, (uint64) v); }

bool 
operator > (const sc_uint_base& u, const sc_signed& v) 
{ return operator>((uint64) u, v); }


bool 
operator >= (const sc_signed& u, const sc_uint_base& v)
{ return operator>=(u, (uint64) v); }

bool 
operator >= (const sc_uint_base& u, const sc_signed& v) 
{ return operator>=((uint64) u, v); }


// ----------------------------------------------------------------------------
//  SECTION: Input and output operators
// ----------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Operator macros.
// ----------------------------------------------------------------------------

#define CONVERT_LONG(u) \
small_type u ## s = get_sign(u);                        \
unsigned long u ## d[DIGITS_PER_ULONG];                    \
from_uint(DIGITS_PER_ULONG, u ## d, (unsigned long) u); 

#define CONVERT_LONG_2(u) \
unsigned long u ## d[DIGITS_PER_ULONG];                     \
from_uint(DIGITS_PER_ULONG, u ## d, (unsigned long) u); 

#define CONVERT_INT64(u) \
small_type u ## s = get_sign(u);                   \
unsigned long u ## d[DIGITS_PER_UINT64];              \
from_uint(DIGITS_PER_UINT64, u ## d, (uint64) u); 

#define CONVERT_INT64_2(u) \
unsigned long u ## d[DIGITS_PER_UINT64];              \
from_uint(DIGITS_PER_UINT64, u ## d, (uint64) u); 


// ----------------------------------------------------------------------------
//  SECTION: PLUS operators: +, +=, ++
// ----------------------------------------------------------------------------

// Cases to consider when computing u + v:
// 1. 0 + v = v
// 2. u + 0 = u
// 3. if sgn(u) == sgn(v)
//    3.1 u + v = +(u + v) = sgn(u) * (u + v) 
//    3.2 (-u) + (-v) = -(u + v) = sgn(u) * (u + v)
// 4. if sgn(u) != sgn(v)
//    4.1 u + (-v) = u - v = sgn(u) * (u - v)
//    4.2 (-u) + v = -(u - v) ==> sgn(u) * (u - v)
//
// Specialization of above cases for computing ++u or u++: 
// 1. 0 + 1 = 1
// 3. u + 1 = u + 1 = sgn(u) * (u + 1)
// 4. (-u) + 1 = -(u - 1) = sgn(u) * (u - 1)

sc_signed
operator+(const sc_unsigned& u, const sc_signed& v)
{

  if (u.sgn == SC_ZERO) // case 1
    return sc_signed(v);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(u);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_signed& u, const sc_unsigned& v)
{

  if (u.sgn == SC_ZERO) // case 1
    return sc_signed(v);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(u);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_signed& u, const sc_signed& v)
{

  if (u.sgn == SC_ZERO) // case 1
    return sc_signed(v);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(u);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator+(const sc_signed &u, int64 v)
{

  if (v == 0)  // case 2
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator+(int64 u, const sc_signed &v)
{

  if (u == 0) // case 1
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4

  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_unsigned &u, int64 v)
{

  if (v == 0)  // case 2
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator+(int64 u, const sc_unsigned &v)
{

  if (u == 0) // case 1
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4

  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_signed &u, uint64 v)
{

  if (v == 0)  // case 2
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator+(uint64 u, const sc_signed &v)
{

  if (u == 0) // case 1
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4

  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_signed &u, long v)
{

  if (v == 0)  // case 2
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator+(long u, const sc_signed &v)
{

  if (u == 0) // case 1
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_unsigned &u, long v)
{

  if (v == 0)  // case 2
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator+(long u, const sc_unsigned &v)
{

  if (u == 0) // case 1
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator+(const sc_signed &u, unsigned long v)
{

  if (v == 0) // case 2
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator+(unsigned long u, const sc_signed &v)
{

  if (u == 0) // case 1
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.

// ----------------------------------------------------------------------------
//  SECTION: MINUS operators: -, -=, --
// ----------------------------------------------------------------------------

// Cases to consider when computing u + v:
// 1. u - 0 = u 
// 2. 0 - v = -v
// 3. if sgn(u) != sgn(v)
//    3.1 u - (-v) = u + v = sgn(u) * (u + v)
//    3.2 (-u) - v = -(u + v) ==> sgn(u) * (u + v)
// 4. if sgn(u) == sgn(v)
//    4.1 u - v = +(u - v) = sgn(u) * (u - v) 
//    4.2 (-u) - (-v) = -(u - v) = sgn(u) * (u - v)
//
// Specialization of above cases for computing --u or u--: 
// 1. 0 - 1 = -1
// 3. (-u) - 1 = -(u + 1) = sgn(u) * (u + 1)
// 4. u - 1 = u - 1 = sgn(u) * (u - 1)

sc_signed
operator-(const sc_unsigned& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(v, -v.sgn);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_unsigned& u, const sc_signed& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(v, -v.sgn);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_signed& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(v, -v.sgn);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_signed& u, const sc_signed& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(v, -v.sgn);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_signed &u, int64 v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator-(int64 u, const sc_signed& v)
{

  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4

  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_unsigned &u, int64 v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator-(int64 u, const sc_unsigned& v)
{

  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4

  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_signed &u, uint64 v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4

  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator-(uint64 u, const sc_signed& v)
{

  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_unsigned &u, uint64 v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4

  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator-(uint64 u, const sc_unsigned& v)
{

  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_signed &u, long v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator-(long u, const sc_signed& v)
{

  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_unsigned &u, long v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator-(long u, const sc_unsigned& v)
{

  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_signed &u, unsigned long v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator-(unsigned long u, const sc_signed& v)
{
  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator-(const sc_unsigned &u, unsigned long v)
{

  if (v == 0) // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO) // case 2
    return sc_signed(-vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           -vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator-(unsigned long u, const sc_unsigned& v)
{
  if (u == 0) // case 1
    return sc_signed(v, -v.sgn);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO) // case 2
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           -v.sgn, v.nbits, v.ndigits, v.digit);

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: MULTIPLICATION operators: *, *=
// ----------------------------------------------------------------------------

// Cases to consider when computing u * v:
// 1. u * 0 = 0 * v = 0
// 2. 1 * v = v and -1 * v = -v
// 3. u * 1 = u and u * -1 = -u
// 4. u * v = u * v

sc_signed
operator*(const sc_unsigned& u, const sc_signed& v)
{
 
  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) // case 1
    return sc_signed();

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator*(const sc_signed& u, const sc_unsigned& v)
{
 
  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) // case 1
    return sc_signed();

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator*(const sc_signed& u, const sc_signed& v)
{
 
  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) // case 1
    return sc_signed();

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator*(const sc_signed& u, int64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_INT64_2(v);

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator*(int64 u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_INT64_2(u);

  // cases 2-4
  return mul_signed_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator*(const sc_unsigned& u, int64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_INT64_2(v);

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator*(int64 u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_INT64_2(u);

  // cases 2-4
  return mul_signed_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator*(const sc_signed& u, uint64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_INT64_2(v);

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator*(uint64 u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_INT64_2(u);

  // cases 2-4
  return mul_signed_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator*(const sc_signed& u, long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_LONG_2(v);

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}


sc_signed
operator*(long u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_LONG_2(u);

  // cases 2-4
  return mul_signed_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator*(const sc_unsigned& u, long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_LONG_2(v);

  // cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}


sc_signed
operator*(long u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_LONG_2(u);

  // cases 2-4
  return mul_signed_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator*(const sc_signed& u, unsigned long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_LONG_2(v);

  // else cases 2-4
  return mul_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}

sc_signed
operator*(unsigned long u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_signed();

  CONVERT_LONG_2(u);

  // cases 2-4
  return mul_signed_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: DIVISION operators: /, /=
// ----------------------------------------------------------------------------

// Cases to consider when finding the quotient q = floor(u/v):
// Note that u = q * v + r for r < q.
// 1. 0 / 0 or u / 0 => error
// 2. 0 / v => 0 = 0 * v + 0
// 3. u / v && u = v => u = 1 * u + 0  - u or v can be 1 or -1
// 4. u / v && u < v => u = 0 * v + u  - u can be 1 or -1
// 5. u / v && u > v => u = q * v + r  - v can be 1 or -1

sc_signed
operator/(const sc_unsigned& u, const sc_signed& v)
{

  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) {
    div_by_zero(v.sgn); // case 1
    return sc_signed();  // case 2
  }

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator/(const sc_signed& u, const sc_unsigned& v)
{

  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) {
    div_by_zero(v.sgn); // case 1
    return sc_signed();  // case 2
  }

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator/(const sc_signed& u, const sc_signed& v)
{

  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) {
    div_by_zero(v.sgn); // case 1
    return sc_signed();  // case 2
  }

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator/(const sc_signed& u, int64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator/(int64 u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(u);

  // other cases
  return div_signed_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator/(const sc_unsigned& u, int64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator/(int64 u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(u);

  // other cases
  return div_signed_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator/(const sc_signed& u, uint64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator/(uint64 u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2

  }

  CONVERT_INT64_2(u);

  // other cases
  return div_signed_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator/(const sc_signed& u, long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}


sc_signed
operator/(long u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(u);

  // other cases
  return div_signed_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator/(const sc_unsigned& u, long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}


sc_signed
operator/(long u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(u);

  // other cases
  return div_signed_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}


sc_signed
operator/(const sc_signed& u, unsigned long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return div_signed_friend(s, u.nbits, u.ndigits, u.digit, 
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}


sc_signed
operator/(unsigned long u, const sc_signed& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2

  }

  CONVERT_LONG_2(u);

  // other cases
  return div_signed_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
                           v.nbits, v.ndigits, v.digit);
  
}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: MOD operators: %, %=.
// ----------------------------------------------------------------------------

// Cases to consider when finding the remainder r = u % v:
// Note that u = q * v + r for r < q.
// 1. 0 % 0 or u % 0 => error
// 2. 0 % v => 0 = 0 * v + 0
// 3. u % v && u = v => u = 1 * u + 0  - u or v can be 1 or -1
// 4. u % v && u < v => u = 0 * v + u  - u can be 1 or -1
// 5. u % v && u > v => u = q * v + r  - v can be 1 or -1

sc_signed
operator%(const sc_unsigned& u, const sc_signed& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);
}


sc_signed
operator%(const sc_signed& u, const sc_unsigned& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);
}


sc_signed
operator%(const sc_signed& u, const sc_signed& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.nbits, v.ndigits, v.digit);
}


sc_signed
operator%(const sc_signed& u, int64 v)
{

  small_type vs = get_sign(v);

  if ((u.sgn == SC_ZERO) || (vs == SC_ZERO)) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator%(int64 u, const sc_signed& v)
{

  small_type us = get_sign(u);

  if ((us == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(u);

  // other cases
  return mod_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator%(const sc_unsigned& u, int64 v)
{

  small_type vs = get_sign(v);

  if ((u.sgn == SC_ZERO) || (vs == SC_ZERO)) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator%(int64 u, const sc_unsigned& v)
{

  small_type us = get_sign(u);

  if ((us == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(u);

  // other cases
  return mod_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator%(const sc_signed& u, uint64 v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator%(uint64 u, const sc_signed& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_INT64(u);

  // other cases
  return mod_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator%(const sc_signed& u, long v)
{

  small_type vs = get_sign(v);

  if ((u.sgn == SC_ZERO) || (vs == SC_ZERO)) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
}


sc_signed
operator%(long u, const sc_signed& v)
{

  small_type us = get_sign(u);

  if ((us == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(u);

  // other cases
  return mod_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator%(const sc_unsigned& u, long v)
{

  small_type vs = get_sign(v);

  if ((u.sgn == SC_ZERO) || (vs == SC_ZERO)) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
}


sc_signed
operator%(long u, const sc_unsigned& v)
{

  small_type us = get_sign(u);

  if ((us == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(u);

  // other cases
  return mod_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.nbits, v.ndigits, v.digit);

}


sc_signed
operator%(const sc_signed& u, unsigned long v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) {
    div_by_zero(v);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return mod_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator%(unsigned long u, const sc_signed& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_signed();  // case 2
  }

  CONVERT_LONG(u);

  // other cases
  return mod_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.nbits, v.ndigits, v.digit);

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Bitwise AND operators: &, &=
// ----------------------------------------------------------------------------

// Cases to consider when computing u & v:
// 1. u & 0 = 0 & v = 0
// 2. u & v => sgn = +
// 3. (-u) & (-v) => sgn = -
// 4. u & (-v) => sgn = +
// 5. (-u) & v => sgn = +

sc_signed
operator&(const sc_unsigned& u, const sc_signed& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_signed& u, const sc_unsigned& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_signed& u, const sc_signed& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_signed& u, int64 v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_signed();

  CONVERT_INT64(v);

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator&(int64 u, const sc_signed& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  CONVERT_INT64(u);

  // other cases
  return and_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_unsigned& u, int64 v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_signed();

  CONVERT_INT64(v);

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator&(int64 u, const sc_unsigned& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  CONVERT_INT64(u);

  // other cases
  return and_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_signed& u, uint64 v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_signed();

  CONVERT_INT64(v);

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_signed
operator&(uint64 u, const sc_signed& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  CONVERT_INT64(u);

  // other cases
  return and_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_signed& u, long v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_signed();

  CONVERT_LONG(v);

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator&(long u, const sc_signed& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  CONVERT_LONG(u);

  // other cases
  return and_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_unsigned& u, long v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_signed();

  CONVERT_LONG(v);

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator&(long u, const sc_unsigned& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  CONVERT_LONG(u);

  // other cases
  return and_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator&(const sc_signed& u, unsigned long v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_signed();

  CONVERT_LONG(v);

  // other cases
  return and_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator&(unsigned long u, const sc_signed& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_signed();

  CONVERT_LONG(u);

  // other cases
  return and_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Bitwise OR operators: |, |=
// ----------------------------------------------------------------------------

// Cases to consider when computing u | v:
// 1. u | 0 = u
// 2. 0 | v = v
// 3. u | v => sgn = +
// 4. (-u) | (-v) => sgn = -
// 5. u | (-v) => sgn = -
// 6. (-u) | v => sgn = -

sc_signed
operator|(const sc_unsigned& u, const sc_signed& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(v);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_signed& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(v);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_signed& u, const sc_signed& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(v);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_signed& u, int64 v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator|(int64 u, const sc_signed& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return or_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_unsigned& u, int64 v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator|(int64 u, const sc_unsigned& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return or_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_signed& u, uint64 v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator|(uint64 u, const sc_signed& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return or_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_signed& u, long v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator|(long u, const sc_signed& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return or_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_unsigned& u, long v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator|(long u, const sc_unsigned& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return or_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator|(const sc_signed& u, unsigned long v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return or_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                          vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator|(unsigned long u, const sc_signed& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return or_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                          v.sgn, v.nbits, v.ndigits, v.digit);

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Bitwise XOR operators: ^, ^=
// ----------------------------------------------------------------------------

// Cases to consider when computing u ^ v:
// Note that  u ^ v = (~u & v) | (u & ~v).
// 1. u ^ 0 = u
// 2. 0 ^ v = v
// 3. u ^ v => sgn = +
// 4. (-u) ^ (-v) => sgn = -
// 5. u ^ (-v) => sgn = -
// 6. (-u) ^ v => sgn = +

sc_signed
operator^(const sc_unsigned& u, const sc_signed& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(v);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_signed& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(v);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_signed& u, const sc_signed& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_signed(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(v);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_signed& u, int64 v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator^(int64 u, const sc_signed& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return xor_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_unsigned& u, int64 v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_signed
operator^(int64 u, const sc_unsigned& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return xor_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_signed& u, uint64 v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}

sc_signed
operator^(uint64 u, const sc_signed& v)
{
  if (u == 0)
    return sc_signed(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return xor_signed_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_signed& u, long v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator^(long u, const sc_signed& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return xor_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_unsigned& u, long v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_signed
operator^(long u, const sc_unsigned& v)
{

  if (u == 0)
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return xor_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_signed
operator^(const sc_signed& u, unsigned long v)
{

  if (v == 0)  // case 1
    return sc_signed(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_signed(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return xor_signed_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                           vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}

sc_signed
operator^(unsigned long u, const sc_signed& v)
{
  if (u == 0)
    return sc_signed(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_signed(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return xor_signed_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                           v.sgn, v.nbits, v.ndigits, v.digit);

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Bitwise NOT operator: ~
// ----------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: LEFT SHIFT operators: <<, <<=
// ----------------------------------------------------------------------------

sc_signed
operator<<(const sc_signed& u, const sc_unsigned& v)
{
  if (v.sgn == SC_ZERO)
    return sc_signed(u);

  return operator<<(u, v.to_ulong());
}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: RIGHT SHIFT operators: >>, >>=
// ----------------------------------------------------------------------------

sc_signed
operator>>(const sc_signed& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)
    return sc_signed(u);

  return operator>>(u, v.to_ulong());

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Unary arithmetic operators.
// ----------------------------------------------------------------------------

sc_signed
operator+(const sc_signed& u)
{
  return sc_signed(u);
}

sc_signed
operator-(const sc_signed& u)
{
  return sc_signed(u, -u.sgn);
}

sc_signed
operator-(const sc_unsigned& u)
{
  return sc_signed(u, -u.sgn);
}


// ----------------------------------------------------------------------------
//  SECTION: EQUAL operator: ==
// ----------------------------------------------------------------------------

bool
operator==(const sc_signed& u, const sc_signed& v)
{
 
  if (u.sgn != v.sgn)
    return false;

  if (&u == &v)
    return true;
  
  if (vec_skip_and_cmp(u.ndigits, u.digit, v.ndigits, v.digit) != 0)
    return false;

  return true;

}


bool
operator==(const sc_signed& u, int64 v)
{

  CONVERT_INT64(v);

  if (u.sgn != vs)
    return false;

  if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_INT64, vd) != 0)
    return false;

  return true;
  
}


bool
operator==(int64 u, const sc_signed& v)
{

  CONVERT_INT64(u);

  if (us != v.sgn)
    return false;

  if (vec_skip_and_cmp(DIGITS_PER_INT64, ud, v.ndigits, v.digit) != 0)
    return false;

  return true;
  
}


bool
operator==(const sc_signed& u, uint64 v)
{

  CONVERT_INT64(v);

  if (u.sgn != vs)
    return false;

  if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_INT64, vd) != 0)
    return false;

  return true;
  
}


bool
operator==(uint64 u, const sc_signed& v)
{

  CONVERT_INT64(u);

  if (us != v.sgn)
    return false;

  if (vec_skip_and_cmp(DIGITS_PER_INT64, ud, v.ndigits, v.digit) != 0)
    return false;

  return true;
  
}


bool
operator==(const sc_signed& u, long v)
{

  CONVERT_LONG(v);

  if (u.sgn != vs)
    return false;

  if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_LONG, vd) != 0)
    return false;

  return true;
  
}


bool
operator==(long u, const sc_signed& v)
{

  CONVERT_LONG(u);

  if (us != v.sgn)
    return false;

  if (vec_skip_and_cmp(DIGITS_PER_LONG, ud, v.ndigits, v.digit) != 0)
    return false;

  return true;
  
}


bool
operator==(const sc_signed& u, unsigned long v)
{

  CONVERT_LONG(v);

  if (u.sgn != vs)
    return false;

  if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_LONG, vd) != 0)
    return false;

  return true;
  
}


bool
operator==(unsigned long u, const sc_signed& v)
{

  CONVERT_LONG(u);

  if (us != v.sgn)
    return false;

  if (vec_skip_and_cmp(DIGITS_PER_LONG, ud, v.ndigits, v.digit) != 0)
    return false;

  return true;
  
}


// ----------------------------------------------------------------------------
//  SECTION: NOT_EQUAL operator: !=
// ----------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: LESS THAN operator: <
// ----------------------------------------------------------------------------

bool
operator<(const sc_signed& u, const sc_signed& v)
{

  if (u.sgn < v.sgn)
    return true;

  if (u.sgn > v.sgn)
    return false;

  // u.sgn == v.sgn

  if (&u == &v)
    return false;

  if (u.sgn == SC_POS) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, v.ndigits, v.digit) < 0)
      return true;

  }
  else if (u.sgn == SC_NEG) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, v.ndigits, v.digit) > 0)
      return true;

  }

  return false;

}


bool
operator<(const sc_signed& u, int64 v)
{

  CONVERT_INT64(v);

  if (u.sgn < vs)
    return true;

  if (u.sgn > vs)
    return false;

  // u.sgn == vs

  if (vs == SC_POS) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_INT64, vd) < 0)
      return true;

  }
  else if (vs == SC_NEG) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_INT64, vd) > 0)
      return true;

  }

  return false;

}


bool
operator<(int64 u, const sc_signed& v)
{

  CONVERT_INT64(u);

  if (us < v.sgn)
    return true;

  if (us > v.sgn)
    return false;

  // us == v.sgn

  if (us == SC_POS) {

    if (vec_skip_and_cmp(DIGITS_PER_INT64, ud, v.ndigits, v.digit) < 0)
      return true;

  }
  else if (us == SC_NEG) {

    if (vec_skip_and_cmp(DIGITS_PER_INT64, ud, v.ndigits, v.digit) > 0)
      return true;

  }

  return false;

}


bool
operator<(const sc_signed& u, uint64 v)
{

  CONVERT_INT64(v);

  if (u.sgn < vs)
    return true;

  if (u.sgn > vs)
    return false;

  // u.sgn == vs

  if (vs == SC_POS) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_INT64, vd) < 0)
      return true;
    
  }

  return false;

}


bool
operator<(uint64 u, const sc_signed& v)
{

  CONVERT_INT64(u);

  if (us < v.sgn)
    return true;

  if (us > v.sgn)
    return false;

  // us == v.sgn

  if (us == SC_POS) {

    if (vec_skip_and_cmp(DIGITS_PER_INT64, ud, v.ndigits, v.digit) < 0)
      return true;

  }

  return false;    

}


bool
operator<(const sc_signed& u, long v)
{

  CONVERT_LONG(v);

  if (u.sgn < vs)
    return true;

  if (u.sgn > vs)
    return false;

  // u.sgn == vs

  if (vs == SC_POS) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_LONG, vd) < 0)
      return true;

  }
  else if (vs == SC_NEG) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_LONG, vd) > 0)
      return true;

  }

  return false;

}


bool
operator<(long u, const sc_signed& v)
{
  CONVERT_LONG(u);

  if (us < v.sgn)
    return true;

  if (us > v.sgn)
    return false;

  // us == v.sgn

  if (us == SC_POS) {

    if (vec_skip_and_cmp(DIGITS_PER_LONG, ud, v.ndigits, v.digit) < 0)
      return true;

  }
  else if (us == SC_NEG) {

    if (vec_skip_and_cmp(DIGITS_PER_LONG, ud, v.ndigits, v.digit) > 0)
      return true;

  }

  return false;
}


bool
operator<(const sc_signed& u, unsigned long v)
{
  CONVERT_LONG(v);

  if (u.sgn < vs)
    return true;

  if (u.sgn > vs)
    return false;

  // u.sgn == vs

  if (vs == SC_POS) {

    if (vec_skip_and_cmp(u.ndigits, u.digit, DIGITS_PER_LONG, vd) < 0)
      return true;
    
  }

  return false;
}


bool
operator<(unsigned long u, const sc_signed& v)
{
  CONVERT_LONG(u);

  if (us < v.sgn)
    return true;

  if (us > v.sgn)
    return false;

  // us == v.sgn

  if (us == SC_POS) {

    if (vec_skip_and_cmp(DIGITS_PER_LONG, ud, v.ndigits, v.digit) < 0)
      return true;

  }

  return false;    
}


// ---------------------------------------------------------------------------
//  SECTION: LESS THAN or EQUAL operator: <=
// ---------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ---------------------------------------------------------------------------
//  SECTION: GREATER THAN operator: >
// ---------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ---------------------------------------------------------------------------
//  SECTION: GREATER THAN or EQUAL operator: >=
// ---------------------------------------------------------------------------

// Operators in this section are included from sc_nbcommon.cpp.


// ---------------------------------------------------------------------------
//  SECTION: Public members - Other utils.
// ---------------------------------------------------------------------------

bool 
sc_signed::iszero() const
{
  if (sgn == SC_ZERO)
    return true;
  else if (sgn != SC_NOSIGN) 
    return false;
  else 
    return check_for_zero(ndigits, digit);
}


bool
sc_signed::sign() const
{
  if (sgn == SC_NEG)
    return 1;
  else if (sgn != SC_NOSIGN)
    return 0;
  else
    return ((digit[ndigits - 1] & one_and_zeros(bit_ord(nbits - 1))) != 0);
}

// The rest of the utils in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Private members.
// ----------------------------------------------------------------------------

// The private members in this section are included from sc_nbcommon.cpp.

#define CLASS_TYPE sc_signed
#define CLASS_TYPE_STR "sc_signed"

#define ADD_HELPER add_signed_friend
#define SUB_HELPER sub_signed_friend
#define MUL_HELPER mul_signed_friend
#define DIV_HELPER div_signed_friend
#define MOD_HELPER mod_signed_friend
#define AND_HELPER and_signed_friend
#define  OR_HELPER  or_signed_friend
#define XOR_HELPER xor_signed_friend 

#include "sc_nbfriends.inc"

#undef  SC_UNSIGNED
#define SC_SIGNED
#define IF_SC_SIGNED              1  // 1 = sc_signed
#define CLASS_TYPE_SUBREF         sc_signed_subref_r
#define OTHER_CLASS_TYPE          sc_unsigned
#define OTHER_CLASS_TYPE_SUBREF   sc_unsigned_subref_r

#define MUL_ON_HELPER mul_on_help_signed
#define DIV_ON_HELPER div_on_help_signed
#define MOD_ON_HELPER mod_on_help_signed

#include "sc_nbcommon.inc"

#undef MOD_ON_HELPER
#undef DIV_ON_HELPER
#undef MUL_ON_HELPER

#undef OTHER_CLASS_TYPE_SUBREF
#undef OTHER_CLASS_TYPE
#undef CLASS_TYPE_SUBREF
#undef IF_SC_SIGNED
#undef SC_SIGNED

#undef XOR_HELPER
#undef  OR_HELPER
#undef AND_HELPER
#undef MOD_HELPER
#undef DIV_HELPER
#undef MUL_HELPER
#undef SUB_HELPER
#undef ADD_HELPER

#undef CLASS_TYPE
#undef CLASS_TYPE_STR

#include "sc_signed_bitref.inc"
#include "sc_signed_subref.inc"

#undef CONVERT_LONG
#undef CONVERT_LONG_2
#undef CONVERT_INT64
#undef CONVERT_INT64_2

} // namespace sc_dt


// End of file.
