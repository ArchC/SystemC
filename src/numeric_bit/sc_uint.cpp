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

    sc_uint.cpp -- contains interface definitions between sc_uint and
    sc_bool_vector, sc_logic_vector, sc_signed, sc_unsigned, and
    definitions for sc_uint_subref.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "../sc_exception.h"
#include "../sc_bool_vector.h"
#include "../sc_logic_vector.h"
#include "sc_signed.h"
#include "sc_unsigned.h"
#include "sc_uint.h"



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

// sc_bool_vector and sc_logic vector
// inline 
sc_uint_base::sc_uint_base(const sc_bool_vector& v)
{
    width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif

  for (int i = width - 1; i >= 0; i--)
      set(i, *(v.get_data_ptr(i)));
  num &= MASK(width);
}

// inline 
sc_uint_base::sc_uint_base(const sc_logic_vector& v) 
{
  width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif

  for (int i = width - 1; i >= 0; i--)
    set(i, v.get_data_ptr(i)->to_bool());
  num &= MASK(width);
}
  
// sc_signed and sc_unsigned
// inline 
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

// inline 
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

// inline 
sc_uint_base& 
sc_uint_base::operator=(const sc_bool_vector& v)
{
  int vlen = v.length();
  int len = min(width, vlen);
  for (int i = len - 1 ; i >= 0; i--) {
    set(i, *(v.get_data_ptr(i)));
  }
  num &= MASK(width);
  return *this;
}
  
// inline 
sc_uint_base& 
sc_uint_base::operator=(const sc_logic_vector& v)
{
  int vlen = v.length();
  int len = min(width, vlen);
  

  for (int i = len - 1; i >= 0; i--) {
    set(i, v.get_data_ptr(i)->to_bool());
  }
  
  num &= MASK(width);
  return *this;
}



// inline 
sc_uint_base& 
sc_uint_base::operator=(const sc_signed& v)
{
  int vlen = v.length();
  int minlen = min(width, vlen);

  int i = 0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  bit sgn = v.sign();
  for (; i < width; ++i) 
    set(i, sgn);     /* sign-extend */

  num &= MASK(width);
  
  return *this;
}

// inline 
sc_uint_base& 
sc_uint_base::operator=(const sc_unsigned& v)
{
  int vlen = v.length();
  int minlen = min(width, vlen);

  int i=0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  for (; i < width; ++i)
    set(i, 0); /* zero-extend */
 
  num &= MASK(width);
  return *this;
}

// sc_uint_subref
// inline 
sc_uint_base::sc_uint_base(const sc_uint_subref& v)
{
    width = v.left - v.right + 1; 
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif
    num = v.operator uint_type();
}

// inline 
sc_uint_base& 
sc_uint_base::operator=(const sc_uint_subref& v)
{
  num = v.operator uint_type(); 
  return *this; 
}

//Arithmetic operators

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

// inline 
sc_uint_bitref 
sc_uint_base::operator[] (int i)
{
#ifdef DEBUG_SYSTEMC
    check_index(i);
#endif
  return sc_uint_bitref(this, i); 
}

// inline 
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

// inline 
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


//sc_uint_subref definitions
// Implicit conversion to ...
  
sc_uint_subref::operator uint_type() const 
{ 
    uint_type val = obj->num;  
    if(left == obj->width - 1)
        return val >> right;
    else
        return ((val & ((const_one_uint << (left + 1)) - 1)) >> right); 
}

// Assignment operators
sc_uint_subref& 
sc_uint_subref::operator = (const sc_uint_base& v)
{ 
    uint_type val = obj->num;
    
    // Mask the bits out that are part-selected
    val &= mask_int[left][right];
    
    // Set the part-selected bits
    val |= (v.num << right) & ~mask_int[left][right]; 
    
    // Normalize the value 
    val &= MASK(obj->width);
    
    obj->num = val;
    
    return *this;
}

sc_uint_subref& 
sc_uint_subref::operator = (const sc_uint_subref& v)
{
	if(this == &v)
	    return *this;
	return operator=(v.operator uint_type());
}

sc_uint_subref& 
sc_uint_subref::operator =(uint_type v)
{
    uint_type val = obj->num;
    
    // Mask the bits out that are part-selected
    val &= mask_int[left][right];
    
    // Set the part-selected bits
    val |= (v << right) & ~mask_int[left][right]; 
    
    // Normalize the value
    val &= MASK(obj->width);
    
    obj->num = val;
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
