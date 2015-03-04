/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2001 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_int.cpp -- contains interface definitions between sc_int and
                sc_signed, sc_unsigned, and definitions for sc_int_subref.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#if defined( __HP_aCC )
#define _INCLUDE_LONGLONG
#endif

#include "systemc/kernel/sc_macros.h"
#include "systemc/datatypes/int/sc_signed.h"
#include "systemc/datatypes/int/sc_unsigned.h"
#include "systemc/datatypes/int/sc_int.h"
#include "systemc/datatypes/bit/sc_bv.h"
#include "systemc/datatypes/bit/sc_lv.h"
#include "systemc/utils/sc_exception.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
//
//  Base class for sc_int.
// ----------------------------------------------------------------------------

sc_int_base::sc_int_base( int w )
    : num(0), width(w)
{
#ifdef DEBUG_SYSTEMC
    check_bitwidth(w);
#endif
}

sc_int_base::sc_int_base(int_type v, int w) 
    : width(w) 
{ 
#ifdef DEBUG_SYSTEMC
    check_bitwidth(w);
    check_signed_value(w, v);
#endif
    int S = (SC_INTWIDTH - w); 
    num = v << S >> S; 
}

sc_int_base&
sc_int_base::operator = (int_type v)
{
#ifdef DEBUG_SYSTEMC
    check_signed_value(v, width);
#endif
    int S = (SC_INTWIDTH - width);
    num = v << S >> S;
    return *this;
}

// sc_signed and sc_unsigned
sc_int_base::sc_int_base(const sc_signed& v)
{
  width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif

  int S = SC_INTWIDTH - width;

  for (int i = width - 1; i >= 0; i--) {
    set(i, v.test(i));
  }

  num = num << S >> S;
}

sc_int_base::sc_int_base(const sc_unsigned& v)
{
  width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif

  int S = SC_INTWIDTH - width;

  for (int i = width - 1; i >= 0; i--) {
    set(i, v.test(i));
  }
  num = num << S >> S;
}

sc_int_base& 
sc_int_base::operator=(const sc_signed& v)
{
  int S = SC_INTWIDTH - width;
  int vlen = v.length();
  int minlen = sc_min(width, vlen);

  int i = 0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  bit sgn = v.sign();
  for (; i < width; ++i) 
    set(i, sgn);     /* sign-extend */
  
  num = num << S >> S;
  
  return *this;
}

sc_int_base& 
sc_int_base::operator=(const sc_unsigned& v)
{
  int S = SC_INTWIDTH - width;
  int vlen = v.length();
  int minlen = sc_min(width, vlen);

  int i = 0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  for (; i < width; ++i)
    set(i, 0); /* zero-extend */
 
  num = num << S >> S;
  return *this;
}

// sc_int_subref
sc_int_base::sc_int_base(const sc_int_subref& v)
{
    width = v.m_left - v.m_right + 1;
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif
    num = v.operator int_type();
}

sc_int_base& 
sc_int_base::operator=(const sc_int_subref& v)
{
  num = v.operator int_type(); 
  return *this; 
}


sc_int_base&
sc_int_base::operator = ( const sc_bv_base& v )
{
    int minlen = sc_min( width, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    for( ; i < width; ++ i ) {
	set( i, 0 );  // zero-extend
    }
    int S = SC_INTWIDTH - width;
    num = num << S >> S;
    return *this;
}

sc_int_base&
sc_int_base::operator = ( const sc_lv_base& v )
{
    int minlen = sc_min( width, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	set( i, sc_logic( v.get_bit( i ) ).to_bool() );
    }
    for( ; i < width; ++ i ) {
	set( i, 0 );  // zero-extend
    }
    int S = SC_INTWIDTH - width;
    num = num << S >> S;
    return *this;
}


// Arithmetic operators
sc_int_base& 
sc_int_base::operator += (int_type v) 
{ 
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val += v;
    num = val << S >> S;
    return *this; 
}
  
sc_int_base& 
sc_int_base::operator -= (int_type v) 
{ 
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val -= v;
    num = val << S >> S;
    return *this;
}
 
sc_int_base& 
sc_int_base::operator *= (int_type v) 
{ 
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val *= v;
    num = val << S >> S; 
    return *this;
}

sc_int_base& 
sc_int_base::operator /= (int_type v) 
{ 
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val /= v; 
    num = val << S >> S; 
    return *this; 
}

sc_int_base& 
sc_int_base::operator %= (int_type v)
{ 
    int_type val = num;
    int S = SC_INTWIDTH - width;
    val %= v; 
    num = val << S >> S;
    return *this; 
}
  
// Bitwise operators
sc_int_base& 
sc_int_base::operator &= (int_type v) 
{ 
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val &= v;
    num = val << S >> S;
    return *this;
}

sc_int_base& 
sc_int_base::operator |= (int_type v) 
{
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val |= v;
    num = val << S >> S;
    return *this;
}

sc_int_base& 
sc_int_base::operator ^= (int_type v) 
{
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val ^= v;
    num = val << S >> S;
    return *this;
}

sc_int_base& 
sc_int_base::operator <<= (int_type v) 
{
    int_type val = num;
    int S = SC_INTWIDTH - width; 
    val <<= v;
    num = val << S >> S;
    return *this;
}

sc_int_base& 
sc_int_base::operator >>= (int_type v) 
{
    num >>= v;  // No sign extension needed.
    return *this;
}

// Prefix and postfix increment and decrement operators
sc_int_base& 
sc_int_base::operator ++ () // prefix
{
    int_type val = num;
    int S = SC_INTWIDTH - width;
    ++val;
    num = val << S >> S;
    return *this;
}

const sc_int_base 
sc_int_base::operator ++ (int) // postfix
{
    int_type val = num;
    int S = SC_INTWIDTH - width;
    int_type tmp = val;
    val++;
    num = val << S >> S;
    return sc_int_base(tmp, width);
}
 
sc_int_base& 
sc_int_base::operator -- () //prefix
{
    int_type val = num;
    int S = SC_INTWIDTH - width;
    --val;
    num = val << S >> S; 
    return *this;
}
  

const sc_int_base 
sc_int_base::operator -- (int) //postfix 
{
      int_type val = num;  
      int S = SC_INTWIDTH - width;
      int_type tmp = val;
      val--;
      num = val << S >> S;
      return sc_int_base(tmp, width);
}



sc_int_bitref 
sc_int_base::operator[] (int i)
{
#ifdef DEBUG_SYSTEMC
    check_index(i);
#endif
    return sc_int_bitref(this, i); 
}

sc_int_subref 
sc_int_base::range(int left, int right)
{
#ifdef DEBUG_SYSTEMC
    check_index(left);
    check_index(right);
#endif
    
    check_range( left, right );
    
    return sc_int_subref(this, left, right);
}

int_type 
sc_int_base::range(int left, int right) const
{
#ifdef DEBUG_SYSTEMC
    check_index(left);
    check_index(right);
#endif

    check_range( left, right );
    
    return num << (SC_INTWIDTH - (left + 1)) >> (SC_INTWIDTH - (left + 1) + right);  
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref
//
//  Class that implements bit-selection on sc_int.
// ----------------------------------------------------------------------------

sc_int_bitref& 
sc_int_bitref::operator = (const sc_int_bitref& b)
{
    int_type val;
    int len = m_obj->width;

    m_obj->set(m_index, (b.m_obj)->test(b.m_index));
    if(m_index == len - 1){
	val = m_obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	m_obj->num = val;
    }
    return *this;
}

sc_int_bitref& 
sc_int_bitref::operator = (bool b)
{
    int_type val;
    int len = m_obj->width;
    m_obj->set(m_index, b);
    if(m_index == len - 1){
	val = m_obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	m_obj->num = val;
    }
    return *this;
}


sc_int_bitref& 
sc_int_bitref::operator &= ( bool b )
{
    if( ! b ) {
	m_obj->num &= ~(const_one_uint << m_index);
	int len = m_obj->width;
	if( m_index == len - 1 ) {
	    int_type val = m_obj->num;
	    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	    m_obj->num = val;
	}
    }
    return *this;
}

sc_int_bitref& 
sc_int_bitref::operator |= ( bool b )
{
    if( b ) {
	m_obj->num |= (const_one_uint << m_index);
	int len = m_obj->width;
	if( m_index == len - 1 ) {
	    int_type val = m_obj->num;
	    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	    m_obj->num = val;
	}
    }
    return *this;
}

sc_int_bitref&
sc_int_bitref::operator ^= ( bool b )
{
    if( b ) {
	m_obj->num ^= (const_one_uint << m_index);
	int len = m_obj->width;
	if( m_index == len - 1 ) {
	    int_type val = m_obj->num;
	    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	    m_obj->num = val;
	}
    }
    return *this;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref
//
//  Class that implements part-selection on sc_int.
// ----------------------------------------------------------------------------

// conversion

sc_int_subref::operator int_type() const 
{
    int_type val = m_obj->num;
    if(m_left == m_obj->width - 1)
        return val >> m_right;
    else
        return val << (SC_INTWIDTH - (m_left + 1))
                   >> (SC_INTWIDTH - (m_left + 1) + m_right);
}


// assignment

sc_int_subref& 
sc_int_subref::operator = (const sc_int_base& v)
{
    int_type val = m_obj->num;
    int len = m_obj->width;
    
    // Mask the bits out that are part-selected
    val &= mask_int[m_left][m_right];
    
    // Set the part-selected bits
    val |= (v.num << m_right) & ~mask_int[m_left][m_right];
    
    // Normalize the value
    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
    
    m_obj->num = val;
    return *this;
}

sc_int_subref& 
sc_int_subref::operator = (const sc_int_subref& v)
{
    if(this == &v)
        return *this;
    return operator=(v.operator int_type());
}
  
sc_int_subref& 
sc_int_subref::operator = (int_type v)
{
    int_type val = m_obj->num;
    int len = m_obj->width;
    
    // Mask the bits out that are part-selected
    val &= mask_int[m_left][m_right];
    
    // Set the part-selected bits
    val |= (v << m_right) & ~mask_int[m_left][m_right];
    
    // Normalize the value
    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
    
    m_obj->num = val;
    return *this;
}


void
check_bitwidth( int w )
{
    if( w <= 0 || w > SC_INTWIDTH ) {
	char errmsg[512];
	sprintf(errmsg, "Illegal bitwidth %d of sc_int/sc_uint variable. Legal bitwidth 1 <= W <= %d", w, SC_INTWIDTH );
	REPORT_ERROR( 1000, errmsg );
    }
}
    
void
check_index( int w )
{
    if( w < 0 || w >= SC_INTWIDTH ) {
	char errmsg[512];
	sprintf(errmsg, "Illegal bit index %d of sc_int/sc_uint variable. Legal index range 0 <= W <= %d", w, SC_INTWIDTH-1 );
	REPORT_ERROR( 1000, errmsg );
    }
}

void 
check_range( int left, int right )
{
    if(left < right) {
	char errmsg[512];
	sprintf(errmsg, "left index %d < right index %d during part selection of sc_int/sc_uint variable", left, right);
	REPORT_ERROR(1000,errmsg);
    }
}

void
check_signed_value( int w, int_type v )
{
  int_type limit = (int_type)1 << (w - 1);
  if (! ((-limit <= v) && (v < limit))) {
    char errmsg[512];
    sprintf(errmsg, "Value cannot fit into a width of %d", w);
    REPORT_WARNING(1000,errmsg);
  }
}


// end of file
