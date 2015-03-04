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

  sc_uint.cpp -- contains interface definitions between sc_uint and
                 sc_signed, sc_unsigned, and definitions for sc_uint_subref.

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
#include "systemc/datatypes/int/sc_uint.h"
#include "systemc/datatypes/bit/sc_bv.h"
#include "systemc/datatypes/bit/sc_lv.h"
#include "systemc/utils/sc_exception.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_base
//
//  Base class for sc_uint.
// ----------------------------------------------------------------------------

sc_uint_base::sc_uint_base( int w )
: num(0), width(w)
{
#ifdef DEBUG_SYSTEMC
    check_bitwidth(w);
#endif
}

sc_uint_base::sc_uint_base(uint_type v, int w) 
: width(w) 
{ 
#ifdef DEBUG_SYSTEMC
    check_bitwidth(w);
    check_unsigned_value(w, v);
#endif

    num = v & MASK(w);
}

// sc_signed and sc_unsigned
sc_uint_base::sc_uint_base(const sc_signed& v)
{
  width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif  
  for (int i = width - 1 ; i >= 0; i--)
    set(i, v.test(i));
  num &= MASK(width);
}

sc_uint_base::sc_uint_base(const sc_unsigned& v)
{
  width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif  
  for (int i = width - 1 ; i >= 0; i--)
    set(i, v.test(i));
  num &= MASK(width);
}


sc_uint_base& 
sc_uint_base::operator = (uint_type v)
{
#ifdef DEBUG_SYSTEMC
    check_unsigned_value(width, v);
#endif

    num = v & MASK(width); 
    return *this; 

}

sc_uint_base&
sc_uint_base::operator=(const sc_signed& v)
{
  int vlen = v.length();
  int minlen = sc_min(width, vlen);

  int i = 0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  bit sgn = v.sign();
  for (; i < width; ++i) 
    set(i, sgn);     /* sign-extend */

  num &= MASK(width);
  
  return *this;
}

sc_uint_base& 
sc_uint_base::operator=(const sc_unsigned& v)
{
  int vlen = v.length();
  int minlen = sc_min(width, vlen);

  int i=0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  for (; i < width; ++i)
    set(i, 0); /* zero-extend */
 
  num &= MASK(width);
  return *this;
}

// sc_uint_subref
sc_uint_base::sc_uint_base(const sc_uint_subref& v)
{
    width = v.m_left - v.m_right + 1;
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif
    num = v.operator uint_type();
}

sc_uint_base& 
sc_uint_base::operator=(const sc_uint_subref& v)
{
  num = v.operator uint_type(); 
  return *this; 
}


sc_uint_base&
sc_uint_base::operator = ( const sc_bv_base& v )
{
    int minlen = sc_min( width, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	set( i, v.get_bit( i ) );
    }
    for( ; i < width; ++ i ) {
	set( i, 0 );  // zero-extend
    }
    num &= MASK( width );
    return *this;
}

sc_uint_base&
sc_uint_base::operator = ( const sc_lv_base& v )
{
    int minlen = sc_min( width, v.length() );
    int i = 0;
    for( ; i < minlen; ++ i ) {
	set( i, sc_logic( v.get_bit( i ) ).to_bool() );
    }
    for( ; i < width; ++ i ) {
	set( i, 0 );  // zero-extend
    }
    num &= MASK( width );
    return *this;
}


// Arithmetic operators

sc_uint_base& 
sc_uint_base::operator += (uint_type v) 
{
    uint_type val = num;
    val += v;
    num = val & MASK(width);
    return *this;
}
 
sc_uint_base& 
sc_uint_base::operator -= (uint_type v) 
{
    uint_type val = num;
    val -= v;
    num = val & MASK(width);
    return *this;
}

sc_uint_base& 
sc_uint_base::operator *= (uint_type v) 
{
    uint_type val = num;
    val *= v;
    num = val & MASK(width);
    return *this;
}

sc_uint_base& 
sc_uint_base::operator /= (uint_type v) 
{
    uint_type val = num;
    val /= v;
    num = val & MASK(width);
    return *this;
}

sc_uint_base& 
sc_uint_base::operator %= (uint_type v) 
{
    uint_type val = num;
    val %= v;
    num = val & MASK(width);
    return *this;
}
  
// Bitwise operators
sc_uint_base& 
sc_uint_base::operator &= (uint_type v) 
{
    uint_type val = num;
    val &= v;
    num = val & MASK(width);
    return *this;
}
  
sc_uint_base& 
sc_uint_base::operator |= (uint_type v) 
{
    uint_type val = num;
    val |= v;
    num = val & MASK(width);
    return *this;
}

sc_uint_base& 
sc_uint_base::operator ^= (uint_type v) 
{
    uint_type val = num;
    val ^= v;
    num = val & MASK(width);
    return *this;
}
 
sc_uint_base& 
sc_uint_base::operator <<= (uint_type v) 
{
    uint_type val = num;
    val <<= v;
    num = val & MASK(width);
    return *this;
}

sc_uint_base& 
sc_uint_base::operator >>= (uint_type v) 
{
    uint_type val = num;
    val >>= v;
    num = val & MASK(width);
    return *this;
}
  
// Prefix and postfix increment and decrement operators
sc_uint_base& 
sc_uint_base::operator ++ () // prefix
{
    uint_type val = num;
    ++val;
    val &= MASK(width); 
    num = val;
    return *this;
}

const sc_uint_base 
sc_uint_base::operator ++ (int) // postfix
{
    uint_type val = num;
    uint_type tmp = val;
    val++;
    val &= MASK(width); 
    num = val;
    return sc_uint_base(tmp, width);
}

sc_uint_base& 
sc_uint_base::operator -- () // prefix
{
    uint_type val = num;
    --val;
    val &= MASK(width);
    num = val;
      return *this;
}

const sc_uint_base 
sc_uint_base::operator -- (int) // postfix 
{
    uint_type val = num;
    uint_type tmp = val;
    val--;
    val &= MASK(width);
    num = val;
    return sc_uint_base(tmp, width);
}

sc_uint_bitref 
sc_uint_base::operator[] (int i)
{
#ifdef DEBUG_SYSTEMC
    check_index(i);
#endif
  return sc_uint_bitref(this, i); 
}

sc_uint_subref 
sc_uint_base::range(int left, int right)
{
#ifdef DEBUG_SYSTEMC
    check_index(left);
    check_index(right);
#endif
    
    check_range( left, right );

    return sc_uint_subref(this, left, right);
}

uint_type 
sc_uint_base::range(int left, int right) const
{
#ifdef DEBUG_SYSTEMC
    check_index(left);
    check_index(right);
#endif
    
    check_range( left, right );
    
  if(left < width - 1)
    return ((num & ((const_one_uint << (left + 1)) - 1)) >> right); 
  else
    return num >> right; 
}


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_subref
//
//  Class that implements part-selection on sc_uint.
// ----------------------------------------------------------------------------

// conversion
  
sc_uint_subref::operator uint_type() const 
{
    uint_type val = m_obj->num;
    if(m_left == m_obj->width - 1)
        return val >> m_right;
    else
        return ((val & ((const_one_uint << (m_left + 1)) - 1)) >> m_right);
}


// assignment

sc_uint_subref& 
sc_uint_subref::operator = ( const sc_uint_base& v )
{
    uint_type val = m_obj->num;
    
    // Mask the bits out that are part-selected
    val &= mask_int[m_left][m_right];
    
    // Set the part-selected bits
    val |= (v.num << m_right) & ~mask_int[m_left][m_right];
    
    // Normalize the value 
    val &= MASK(m_obj->width);
    
    m_obj->num = val;
    
    return *this;
}

sc_uint_subref& 
sc_uint_subref::operator = ( const sc_uint_subref& v )
{
    if(this == &v)
        return *this;
    return operator=(v.operator uint_type());
}

sc_uint_subref& 
sc_uint_subref::operator = ( uint_type v )
{
    uint_type val = m_obj->num;
    
    // Mask the bits out that are part-selected
    val &= mask_int[m_left][m_right];
    
    // Set the part-selected bits
    val |= (v << m_right) & ~mask_int[m_left][m_right];
    
    // Normalize the value
    val &= MASK(m_obj->width);
    
    m_obj->num = val;
    return *this;
}


void
check_unsigned_value( int w, uint_type v )
{
  uint_type limit = MASK(w);
  if (! ((0 <= v) && (v <= limit))) {
    char errmsg[512];
    sprintf(errmsg, "Value cannot fit into a width of %d", w);
    REPORT_WARNING(1000,errmsg);
  }
}


// end of file
