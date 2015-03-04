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

  sc_unsigned.cpp -- Arbitrary precision signed arithmetic.

    This file includes the definitions of sc_unsigned_bitref,
    sc_unsigned_subref, and sc_unsigned classes. The first two classes
    are proxy classes to reference one bit and a range of bits of a
    sc_unsigned number, respectively. This file also includes
    sc_nbcommon.cpp and sc_nbfriends.cpp, which contain the
    definitions shared by sc_unsigned.

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
#include "systemc/datatypes/int/sc_unsigned.h"
#include "systemc/datatypes/int/sc_signed.h"
#include "systemc/datatypes/int/sc_int_base.h"
#include "systemc/datatypes/int/sc_uint_base.h"
#include "systemc/datatypes/int/sc_int_ids.h"
#include "systemc/datatypes/bit/sc_bv_base.h"
#include "systemc/datatypes/bit/sc_lv_base.h"
#include "systemc/datatypes/fx/sc_ufix.h"
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

sc_unsigned&
sc_unsigned::operator = ( const char* a )
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
	sc_ufix aa( a, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
	return this->operator = ( aa );
    } catch( sc_exception ) {
	char msg[BUFSIZ];
	sprintf( msg, "character string '%s' is not valid", a );
	SC_REPORT_ERROR( SC_ID_CONVERSION_FAILED_, msg );
	// never reached
	return *this;
    }
}

sc_unsigned&
sc_unsigned::operator=(int64 v)
{
  if (v == 0) {
    sgn = SC_ZERO;
    vec_zero(ndigits, digit);
  }
  else {
    sgn = SC_POS;
    from_uint(ndigits, digit, (uint64) v);
    convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_unsigned&
sc_unsigned::operator=(uint64 v)
{
  if (v == 0) {
    sgn = SC_ZERO;
    vec_zero(ndigits, digit);
  }
  else {
    sgn = SC_POS;
    from_uint(ndigits, digit, v);
    convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_unsigned&
sc_unsigned::operator=(long v)
{
  if (v == 0) {
    sgn = SC_ZERO;
    vec_zero(ndigits, digit);
  }
  else {
    sgn = SC_POS;
    from_uint(ndigits, digit, (unsigned long) v);
    convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_unsigned&
sc_unsigned::operator=(unsigned long v)
{
  if (v == 0) {
    sgn = SC_ZERO;
    vec_zero(ndigits, digit);
  }
  else {
    sgn = SC_POS;
    from_uint(ndigits, digit, v);
    convert_SM_to_2C_to_SM();
  }
  return *this;
}

sc_unsigned&
sc_unsigned::operator=(double v)
{
  is_bad_double(v);
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

sc_unsigned&
sc_unsigned::operator = ( const sc_bv_base& v )
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

sc_unsigned&
sc_unsigned::operator = ( const sc_lv_base& v )
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
sc_unsigned::to_string( sc_numrep numrep ) const
{
    int len = length();
    sc_ufix aa( *this, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
    return aa.to_string( numrep );
}

const sc_string
sc_unsigned::to_string( sc_numrep numrep, bool w_prefix ) const
{
    int len = length();
    sc_ufix aa( *this, len, len, SC_TRN, SC_WRAP, 0, SC_ON );
    return aa.to_string( numrep, w_prefix );
}


// ----------------------------------------------------------------------------
//  SECTION: Interfacing with sc_int_base
// ----------------------------------------------------------------------------

sc_unsigned& 
sc_unsigned::operator= (const sc_int_base& v)
{ return operator=((int64) v); }

sc_unsigned& 
sc_unsigned::operator+=(const sc_int_base& v)
{ return operator+=((int64) v); }

sc_unsigned& 
sc_unsigned::operator-=(const sc_int_base& v)
{ return operator-=((int64) v); }

sc_unsigned& 
sc_unsigned::operator*=(const sc_int_base& v)
{ return operator*=((int64) v); }

sc_unsigned& 
sc_unsigned::operator/=(const sc_int_base& v)
{ return operator/=((int64) v); }

sc_unsigned& 
sc_unsigned::operator%=(const sc_int_base& v)
{ return operator%=((int64) v); }

sc_unsigned& 
sc_unsigned::operator&=(const sc_int_base& v)
{ return operator&=((int64) v); }

sc_unsigned& 
sc_unsigned::operator|=(const sc_int_base& v)
{ return operator|=((int64) v); }

sc_unsigned& 
sc_unsigned::operator^=(const sc_int_base& v)
{ return operator^=((int64) v); }

sc_unsigned 
operator<<(const sc_unsigned& u, const sc_int_base& v)
{ return operator<<(u, (int64) v); }
sc_unsigned& 
sc_unsigned::operator<<=(const sc_int_base& v)
{ return operator<<=((int64) v); }

sc_unsigned 
operator>>(const sc_unsigned&    u, const sc_int_base&  v)
{ return operator>>(u, (int64) v); }
sc_unsigned& 
sc_unsigned::operator>>=(const sc_int_base&  v)
{ return operator>>=((int64) v); }

bool 
operator==(const sc_unsigned& u, const sc_int_base& v)
{ return operator==(u, (int64) v); }
bool 
operator==(const sc_int_base& u, const sc_unsigned& v) 
{ return operator==((int64) u, v); }

bool 
operator!=(const sc_unsigned& u, const sc_int_base& v)
{ return operator!=(u, (int64) v); }
bool 
operator!=(const sc_int_base& u, const sc_unsigned& v) 
{ return operator!=((int64) u, v); }

bool 
operator<(const sc_unsigned& u, const sc_int_base& v)
{ return operator<(u, (int64) v); }
bool 
operator<(const sc_int_base& u, const sc_unsigned& v) 
{ return operator<((int64) u, v); }

bool 
operator<=(const sc_unsigned& u, const sc_int_base& v)
{ return operator<=(u, (int64) v); }
bool 
operator<=(const sc_int_base& u, const sc_unsigned& v) 
{ return operator<=((int64) u, v); }

bool 
operator>(const sc_unsigned& u, const sc_int_base& v)
{ return operator>(u, (int64) v); }
bool 
operator>(const sc_int_base& u, const sc_unsigned& v) 
{ return operator>((int64) u, v); }

bool 
operator>=(const sc_unsigned& u, const sc_int_base& v)
{ return operator>=(u, (int64) v); }
bool 
operator>=(const sc_int_base& u, const sc_unsigned& v) 
{ return operator>=((int64) u, v); }


// ----------------------------------------------------------------------------
//  SECTION: Interfacing with sc_uint_base
// ----------------------------------------------------------------------------

sc_unsigned& 
sc_unsigned::operator= (const sc_uint_base& v)
{ return operator=((uint64) v); }

sc_unsigned 
operator+(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator+(u, (uint64) v); }
sc_unsigned 
operator+(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator+((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator+=(const sc_uint_base& v)
{ return operator+=((uint64) v); }

sc_unsigned& 
sc_unsigned::operator-=(const sc_uint_base& v)
{ return operator-=((uint64) v); }

sc_unsigned 
operator*(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator*(u, (uint64) v); }
sc_unsigned 
operator*(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator*((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator*=(const sc_uint_base& v)
{ return operator*=((uint64) v); }

sc_unsigned 
operator/(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator/(u, (uint64) v); }
sc_unsigned 
operator/(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator/((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator/=(const sc_uint_base& v)
{ return operator/=((uint64) v); }

sc_unsigned 
operator%(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator%(u, (uint64) v); }
sc_unsigned 
operator%(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator%((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator%=(const sc_uint_base& v)
{ return operator%=((uint64) v); }

sc_unsigned 
operator&(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator&(u, (uint64) v); }
sc_unsigned 
operator&(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator&((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator&=(const sc_uint_base& v)
{ return operator&=((uint64) v); }

sc_unsigned 
operator|(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator|(u, (uint64) v); }
sc_unsigned 
operator|(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator|((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator|=(const sc_uint_base& v)
{ return operator|=((uint64) v); }

sc_unsigned 
operator^(const sc_unsigned& u, const sc_uint_base& v) 
{ return operator^(u, (uint64) v); }
sc_unsigned 
operator^(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator^((uint64) u, v); }
sc_unsigned& 
sc_unsigned::operator^=(const sc_uint_base& v)
{ return operator^=((uint64) v); }

sc_unsigned 
operator<<(const sc_unsigned& u, const sc_uint_base& v)
{ return operator<<(u, (uint64) v); }
sc_unsigned& 
sc_unsigned::operator<<=(const sc_uint_base& v)
{ return operator<<=((uint64) v); }

sc_unsigned 
operator>>(const sc_unsigned&    u, const sc_uint_base&  v)
{ return operator>>(u, (uint64) v); }
sc_unsigned& 
sc_unsigned::operator>>=(const sc_uint_base&  v)
{ return operator>>=((uint64) v); }

bool 
operator==(const sc_unsigned& u, const sc_uint_base& v)
{ return operator==(u, (uint64) v); }
bool 
operator==(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator==((uint64) u, v); }

bool 
operator!=(const sc_unsigned& u, const sc_uint_base& v)
{ return operator!=(u, (uint64) v); }
bool 
operator!=(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator!=((uint64) u, v); }

bool 
operator<(const sc_unsigned& u, const sc_uint_base& v)
{ return operator<(u, (uint64) v); }
bool 
operator<(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator<((uint64) u, v); }

bool 
operator<=(const sc_unsigned& u, const sc_uint_base& v)
{ return operator<=(u, (uint64) v); }
bool 
operator<=(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator<=((uint64) u, v); }

bool 
operator>(const sc_unsigned& u, const sc_uint_base& v)
{ return operator>(u, (uint64) v); }
bool 
operator>(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator>((uint64) u, v); }

bool 
operator>=(const sc_unsigned& u, const sc_uint_base& v)
{ return operator>=(u, (uint64) v); }
bool 
operator>=(const sc_uint_base& u, const sc_unsigned& v) 
{ return operator>=((uint64) u, v); }


// ----------------------------------------------------------------------------
//  SECTION: Input and output operators
// ----------------------------------------------------------------------------

// The operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Operator macros.
// ----------------------------------------------------------------------------

#define CONVERT_LONG(u) \
small_type u ## s = get_sign(u);                   \
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

sc_unsigned
operator+(const sc_unsigned& u, const sc_unsigned& v)
{

  if (u.sgn == SC_ZERO) // case 1
    return sc_unsigned(v);

  if (v.sgn == SC_ZERO) // case 2
    return sc_unsigned(u);

  // cases 3 and 4
  return add_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             v.sgn, v.nbits, v.ndigits, v.digit);
  
}


sc_unsigned
operator+(const sc_unsigned &u, uint64 v)
{

  if (v == 0)  // case 2
    return sc_unsigned(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_unsigned(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // cases 3 and 4
  return add_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_unsigned
operator+(uint64 u, const sc_unsigned &v)
{

  if (u == 0) // case 1
    return sc_unsigned(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // cases 3 and 4

  return add_unsigned_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                             v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator+(const sc_unsigned &u, unsigned long v)
{

  if (v == 0) // case 2
    return sc_unsigned(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 1
    return sc_unsigned(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // cases 3 and 4
  return add_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_unsigned
operator+(unsigned long u, const sc_unsigned &v)
{

  if (u == 0) // case 1
    return sc_unsigned(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)  // case 2
    return sc_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // cases 3 and 4
  return add_unsigned_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
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

// The operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: MULTIPLICATION operators: *, *=
// ----------------------------------------------------------------------------

// Cases to consider when computing u * v:
// 1. u * 0 = 0 * v = 0
// 2. 1 * v = v and -1 * v = -v
// 3. u * 1 = u and u * -1 = -u
// 4. u * v = u * v

sc_unsigned
operator*(const sc_unsigned& u, const sc_unsigned& v)
{
 
  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) // case 1
    return sc_unsigned();

  // cases 2-4
  return mul_unsigned_friend(s, u.nbits, u.ndigits, u.digit,
                             v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator*(const sc_unsigned& u, uint64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_unsigned();

  CONVERT_INT64_2(v);

  // cases 2-4
  return mul_unsigned_friend(s, u.nbits, u.ndigits, u.digit, 
                             BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_unsigned
operator*(uint64 u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_unsigned();

  CONVERT_INT64_2(u);

  // cases 2-4
  return mul_unsigned_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                             v.nbits, v.ndigits, v.digit);
  
}


sc_unsigned
operator*(const sc_unsigned& u, unsigned long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) // case 1
    return sc_unsigned();

  CONVERT_LONG_2(v);

  // else cases 2-4
  return mul_unsigned_friend(s, u.nbits, u.ndigits, u.digit, 
                             BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}

sc_unsigned
operator*(unsigned long u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) // case 1
    return sc_unsigned();

  CONVERT_LONG_2(u);

  // cases 2-4
  return mul_unsigned_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
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

sc_unsigned
operator/(const sc_unsigned& u, const sc_unsigned& v)
{

  small_type s = mul_signs(u.sgn, v.sgn);

  if (s == SC_ZERO) {
    div_by_zero(v.sgn); // case 1
    return sc_unsigned();  // case 2
  }

  // other cases
  return div_unsigned_friend(s, u.nbits, u.ndigits, u.digit,
                             v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator/(const sc_unsigned& u, uint64 v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_unsigned();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return div_unsigned_friend(s, u.nbits, u.ndigits, u.digit, 
                             BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_unsigned
operator/(uint64 u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_unsigned();  // case 2

  }

  CONVERT_INT64_2(u);

  // other cases
  return div_unsigned_friend(s, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, 
                             v.nbits, v.ndigits, v.digit);
  
}


sc_unsigned
operator/(const sc_unsigned& u, unsigned long v)
{

  small_type s = mul_signs(u.sgn, get_sign(v));

  if (s == SC_ZERO) {
    div_by_zero(v);  // case 1
    return sc_unsigned();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return div_unsigned_friend(s, u.nbits, u.ndigits, u.digit, 
                             BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
}


sc_unsigned
operator/(unsigned long u, const sc_unsigned& v)
{

  small_type s = mul_signs(v.sgn, get_sign(u));

  if (s == SC_ZERO) {
    div_by_zero(v.sgn);  // case 1
    return sc_unsigned();  // case 2

  }

  CONVERT_LONG_2(u);

  // other cases
  return div_unsigned_friend(s, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, 
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

sc_unsigned
operator%(const sc_unsigned& u, const sc_unsigned& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_unsigned();  // case 2
  }

  // other cases
  return mod_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             v.nbits, v.ndigits, v.digit);
}


sc_unsigned
operator%(const sc_unsigned& u, uint64 v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) {
    div_by_zero(v);  // case 1
    return sc_unsigned();  // case 2
  }

  CONVERT_INT64_2(v);

  // other cases
  return mod_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_unsigned
operator%(uint64 u, const sc_unsigned& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_unsigned();  // case 2
  }

  CONVERT_INT64(u);

  // other cases
  return mod_unsigned_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                             v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator%(const sc_unsigned& u, unsigned long v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) {
    div_by_zero(v);  // case 1
    return sc_unsigned();  // case 2
  }

  CONVERT_LONG_2(v);

  // other cases
  return mod_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_unsigned
operator%(unsigned long u, const sc_unsigned& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) {
    div_by_zero(v.sgn);  // case 1
    return sc_unsigned();  // case 2
  }

  CONVERT_LONG(u);

  // other cases
  return mod_unsigned_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
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

sc_unsigned
operator&(const sc_unsigned& u, const sc_unsigned& v)
{

  if ((u.sgn == SC_ZERO) || (v.sgn == SC_ZERO)) // case 1
    return sc_unsigned();

  // other cases
  return and_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator&(const sc_unsigned& u, uint64 v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_unsigned();

  CONVERT_INT64(v);

  // other cases
  return and_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
}


sc_unsigned
operator&(uint64 u, const sc_unsigned& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_unsigned();

  CONVERT_INT64(u);

  // other cases
  return and_unsigned_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                             v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator&(const sc_unsigned& u, unsigned long v)
{

  if ((u.sgn == SC_ZERO) || (v == 0)) // case 1
    return sc_unsigned();

  CONVERT_LONG(v);

  // other cases
  return and_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_unsigned
operator&(unsigned long u, const sc_unsigned& v)
{

  if ((u == 0) || (v.sgn == SC_ZERO)) // case 1
    return sc_unsigned();

  CONVERT_LONG(u);

  // other cases
  return and_unsigned_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
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

sc_unsigned
operator|(const sc_unsigned& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_unsigned(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_unsigned(v);

  // other cases
  return or_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                            v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator|(const sc_unsigned& u, uint64 v)
{

  if (v == 0)  // case 1
    return sc_unsigned(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_unsigned(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return or_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                            vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}


sc_unsigned
operator|(uint64 u, const sc_unsigned& v)
{

  if (u == 0)
    return sc_unsigned(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return or_unsigned_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                            v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator|(const sc_unsigned& u, unsigned long v)
{

  if (v == 0)  // case 1
    return sc_unsigned(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_unsigned(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return or_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                            vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}


sc_unsigned
operator|(unsigned long u, const sc_unsigned& v)
{

  if (u == 0)
    return sc_unsigned(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return or_unsigned_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
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

sc_unsigned
operator^(const sc_unsigned& u, const sc_unsigned& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return sc_unsigned(u);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_unsigned(v);

  // other cases
  return xor_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator^(const sc_unsigned& u, uint64 v)
{

  if (v == 0)  // case 1
    return sc_unsigned(u);

  CONVERT_INT64(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_unsigned(vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd, false);

  // other cases
  return xor_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

}

sc_unsigned
operator^(uint64 u, const sc_unsigned& v)
{
  if (u == 0)
    return sc_unsigned(v);

  CONVERT_INT64(u);

  if (v.sgn == SC_ZERO)
    return sc_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud, false);

  // other cases
  return xor_unsigned_friend(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                             v.sgn, v.nbits, v.ndigits, v.digit);

}


sc_unsigned
operator^(const sc_unsigned& u, unsigned long v)
{

  if (v == 0)  // case 1
    return sc_unsigned(u);

  CONVERT_LONG(v);

  if (u.sgn == SC_ZERO)  // case 2
    return sc_unsigned(vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd, false);

  // other cases
  return xor_unsigned_friend(u.sgn, u.nbits, u.ndigits, u.digit,
                             vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

}

sc_unsigned
operator^(unsigned long u, const sc_unsigned& v)
{
  if (u == 0)
    return sc_unsigned(v);

  CONVERT_LONG(u);

  if (v.sgn == SC_ZERO)
    return sc_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud, false);

  // other cases
  return xor_unsigned_friend(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
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

sc_unsigned
operator<<(const sc_unsigned& u, const sc_signed& v)
{
  if ((v.sgn == SC_ZERO) || (v.sgn == SC_NEG))
    return sc_unsigned(u);

  return operator<<(u, v.to_ulong());
}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: RIGHT SHIFT operators: >>, >>=
// ----------------------------------------------------------------------------

sc_unsigned
operator>>(const sc_unsigned& u, const sc_signed& v)
{

  if ((v.sgn == SC_ZERO) || (v.sgn == SC_NEG))
    return sc_unsigned(u);

  return operator>>(u, v.to_long());

}

// The rest of the operators in this section are included from
// sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Unary arithmetic operators.
// ----------------------------------------------------------------------------

sc_unsigned
operator+(const sc_unsigned& u)
{
  return sc_unsigned(u);
}


// ----------------------------------------------------------------------------
//  SECTION: EQUAL operator: ==
// ----------------------------------------------------------------------------

bool
operator==(const sc_unsigned& u, const sc_unsigned& v)
{
  if (&u == &v)
    return true;
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       v.sgn, v.nbits, v.ndigits, v.digit) != 0)
    return false;
  return true;
}


bool
operator==(const sc_unsigned& u, const sc_signed& v)
{
  if (v.sgn == SC_NEG)
    return false;
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       v.sgn, v.nbits, v.ndigits, v.digit, 0, 1) != 0)
    return false;
  return true;
}


bool
operator==(const sc_signed& u, const sc_unsigned& v)
{
  if (u.sgn == SC_NEG)
    return false;
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       v.sgn, v.nbits, v.ndigits, v.digit, 1, 0) != 0)
    return false;
  return true;
}


bool
operator==(const sc_unsigned& u, int64 v)
{
  if (v < 0)
    return false;
  CONVERT_INT64(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd) != 0)
    return false;
  return true;
}


bool
operator==(int64 u, const sc_unsigned& v)
{
  if (u < 0)
    return false;
  CONVERT_INT64(u);
  if (compare_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) != 0)
    return false;
  return true;
}


bool
operator==(const sc_unsigned& u, uint64 v)
{
  CONVERT_INT64(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd) != 0)
    return false;
  return true;
}


bool
operator==(uint64 u, const sc_unsigned& v)
{
  CONVERT_INT64(u);
  if (compare_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) != 0)
    return false;
  return true;
}


bool
operator==(const sc_unsigned& u, long v)
{
  if (v < 0)
    return false;
  CONVERT_LONG(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd) != 0)
    return false;
  return true;
}


bool
operator==(long u, const sc_unsigned& v)
{
  if (u < 0)
    return false;
  CONVERT_LONG(u);
  if (compare_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) != 0)
    return false;
  return true;
}


bool
operator==(const sc_unsigned& u, unsigned long v)
{
  CONVERT_LONG(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd) != 0)
    return false;
  return true;
}


bool
operator==(unsigned long u, const sc_unsigned& v)
{
  CONVERT_LONG(u);
  if (compare_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) != 0)
    return false;
  return true;
}


// ----------------------------------------------------------------------------
//  SECTION: NOT_EQUAL operator: !=
// ----------------------------------------------------------------------------

bool
operator!=(const sc_unsigned& u, const sc_signed& v)
{
  return (! operator==(u, v));
}


bool
operator!=(const sc_signed& u, const sc_unsigned& v)
{
  return (! operator==(u, v));
}

// The rest of the operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: LESS THAN operator: <
// ----------------------------------------------------------------------------

bool
operator<(const sc_unsigned& u, const sc_unsigned& v)
{
  if (&u == &v)
    return false;
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       v.sgn, v.nbits, v.ndigits, v.digit) < 0)
    return true;
  return false;
}


bool
operator<(const sc_unsigned& u, const sc_signed& v)
{
  if (v.sgn == SC_NEG)
    return false;
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       v.sgn, v.nbits, v.ndigits, v.digit, 0, 1) < 0)
    return true;
  return false;
}


bool
operator<(const sc_signed& u, const sc_unsigned& v)
{
  if (u.sgn == SC_NEG)
    return true;
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       v.sgn, v.nbits, v.ndigits, v.digit, 1, 0) < 0)
    return true;
  return false;
}


bool
operator<(const sc_unsigned& u, int64 v)
{
  if (v < 0)
    return false;
  CONVERT_INT64(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd) < 0)
    return true;
  return false;
}


bool
operator<(int64 u, const sc_unsigned& v)
{
  if (u < 0)
    return true;
  CONVERT_INT64(u);
  if (compare_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) < 0)
    return true;
  return false;
}


bool
operator<(const sc_unsigned& u, uint64 v)
{
  CONVERT_INT64(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd) < 0)
    return true;
  return false;
}


bool
operator<(uint64 u, const sc_unsigned& v)
{
  CONVERT_INT64(u);
  if (compare_unsigned(us, BITS_PER_UINT64, DIGITS_PER_UINT64, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) < 0)
    return true;
  return false;    
}


bool
operator<(const sc_unsigned& u, long v)
{
  if (v < 0)
    return false;
  CONVERT_LONG(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd) < 0)
    return true;
  return false;
}


bool
operator<(long u, const sc_unsigned& v)
{
  if (u < 0)
    return true;
  CONVERT_LONG(u);
  if (compare_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) < 0)
    return true;
  return false;
}


bool
operator<(const sc_unsigned& u, unsigned long v)
{
  CONVERT_LONG(v);
  if (compare_unsigned(u.sgn, u.nbits, u.ndigits, u.digit, 
                       vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd) < 0)
    return true;
  return false;
}


bool
operator<(unsigned long u, const sc_unsigned& v)
{
  CONVERT_LONG(u);
  if (compare_unsigned(us, BITS_PER_ULONG, DIGITS_PER_ULONG, ud,
                       v.sgn, v.nbits, v.ndigits, v.digit) < 0)
    return true;
  return false;    
}


// ----------------------------------------------------------------------------
//  SECTION: LESS THAN or EQUAL operator: <=
// ----------------------------------------------------------------------------

bool
operator<=(const sc_unsigned& u, const sc_signed& v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(const sc_signed& u, const sc_unsigned& v)
{
  return (operator<(u, v) || operator==(u, v));
}

// The rest of the operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: GREATER THAN operator: >
// ----------------------------------------------------------------------------

bool
operator>(const sc_unsigned& u, const sc_signed& v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(const sc_signed& u, const sc_unsigned& v)
{
  return (! (operator<=(u, v)));
}

// The rest of the operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: GREATER THAN or EQUAL operator: >=
// ----------------------------------------------------------------------------

bool
operator>=(const sc_unsigned& u, const sc_signed& v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(const sc_signed& u, const sc_unsigned& v)
{
  return (! (operator<(u, v)));
}

// The rest of the operators in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Friends 
// ----------------------------------------------------------------------------

// Compare u and v as unsigned and return r
//  r = 0 if u == v
//  r < 0 if u < v
//  r > 0 if u > v

int
compare_unsigned(small_type us, 
                 int unb, int und, const unsigned long *ud, 
                 small_type vs, 
                 int vnb, int vnd, const unsigned long *vd,
                 small_type if_u_signed, 
                 small_type if_v_signed)
{

  if (us == vs) {

    if (us == SC_ZERO)
      return 0;

    else {

      int cmp_res = vec_skip_and_cmp(und, ud, vnd, vd);

      if (us == SC_POS)
        return cmp_res;
      else
        return -cmp_res;

    }
  }
  else {

    if (us == SC_ZERO)
      return -vs;

    if (vs == SC_ZERO)
      return us;

    int cmp_res;

    int nd = (us == SC_NEG ? und : vnd);

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[nd];
#endif

    if (us == SC_NEG) {

      vec_copy(nd, d, ud);
      vec_complement(nd, d);
      trim(if_u_signed, unb, nd, d);
      cmp_res = vec_skip_and_cmp(nd, d, vnd, vd);

    }
    else {

      vec_copy(nd, d, vd);
      vec_complement(nd, d);
      trim(if_v_signed, vnb, nd, d);
      cmp_res = vec_skip_and_cmp(und, ud, nd, d);

    }

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

    return cmp_res;

  }
}


// ----------------------------------------------------------------------------
//  SECTION: Public members - Other utils.
// ----------------------------------------------------------------------------

bool 
sc_unsigned::iszero() const
{
  if (sgn == SC_ZERO)
    return true;

  else if (sgn == SC_NEG) {

    // A negative unsigned number can be zero, e.g., -16 in 4 bits, so
    // check that.

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[ndigits];
#endif

    vec_copy(ndigits, d, digit);
    vec_complement(ndigits, d);
    trim_unsigned(nbits, ndigits, d);

    bool res = check_for_zero(ndigits, d);

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

    return res;
    
  }
  else
    return false;
}

// The rest of the utils in this section are included from sc_nbcommon.cpp.


// ----------------------------------------------------------------------------
//  SECTION: Private members.
// ----------------------------------------------------------------------------

// The private members in this section are included from
// sc_nbcommon.cpp.

#define CLASS_TYPE sc_unsigned
#define CLASS_TYPE_STR "sc_unsigned"

#define ADD_HELPER add_unsigned_friend
#define SUB_HELPER sub_unsigned_friend
#define MUL_HELPER mul_unsigned_friend
#define DIV_HELPER div_unsigned_friend
#define MOD_HELPER mod_unsigned_friend
#define AND_HELPER and_unsigned_friend
#define  OR_HELPER  or_unsigned_friend
#define XOR_HELPER xor_unsigned_friend 

#include "sc_nbfriends.inc"

#undef  SC_SIGNED
#define SC_UNSIGNED
#define IF_SC_SIGNED              0  // 0 = sc_unsigned
#define CLASS_TYPE_SUBREF         sc_unsigned_subref_r
#define OTHER_CLASS_TYPE          sc_signed
#define OTHER_CLASS_TYPE_SUBREF   sc_signed_subref_r

#define MUL_ON_HELPER mul_on_help_unsigned
#define DIV_ON_HELPER div_on_help_unsigned
#define MOD_ON_HELPER mod_on_help_unsigned

#include "sc_nbcommon.inc"

#undef MOD_ON_HELPER
#undef DIV_ON_HELPER
#undef MUL_ON_HELPER

#undef OTHER_CLASS_TYPE_SUBREF
#undef OTHER_CLASS_TYPE
#undef CLASS_TYPE_SUBREF
#undef IF_SC_SIGNED
#undef SC_UNSIGNED

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

#include "sc_unsigned_bitref.inc"
#include "sc_unsigned_subref.inc"

#undef CONVERT_LONG
#undef CONVERT_LONG_2
#undef CONVERT_INT64
#undef CONVERT_INT64_2

} // namespace sc_dt


// End of file.
