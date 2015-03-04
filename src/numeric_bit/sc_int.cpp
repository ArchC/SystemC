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

    sc_int.cpp -- contains interface definitions between sc_int and
    sc_bool_vector, sc_logic_vector, sc_signed, sc_unsigned, and
    definitions for sc_int_subref.

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
#include "sc_int.h"

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

// sc_bool_vector and sc_logic_vector
// inline 
sc_int_base::sc_int_base(const sc_bool_vector& v)
{
    width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif

    int S = SC_INTWIDTH - width;
    for (int i = width - 1; i >= 0; i--) {
	set(i, *(v.get_data_ptr(i)));
    }
    num = num << S >> S;
}

// inline 
sc_int_base::sc_int_base(const sc_logic_vector& v)
{
    width = v.length();
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif
    int S = SC_INTWIDTH - width;
    for (int i = width - 1; i >= 0; i--) {
	set(i, v.get_data_ptr(i)->to_bool());
    }
    num = num << S >> S;
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

// inline 
sc_int_base& 
sc_int_base::operator=(const sc_bool_vector& v)
{
  int S = SC_INTWIDTH - width;
  int vlen = v.length();
  int len = min(width, vlen);
  
  for (int i = len - 1 ; i >= 0; i--) {
    set(i, *(v.get_data_ptr(i)));
  }
  num = num << S >> S;
  return *this;
}
  
// inline 
sc_int_base& 
sc_int_base::operator=(const sc_logic_vector& v)
{
  int S = SC_INTWIDTH - width;
  int vlen = v.length();
  int len = min(width, vlen);
  
  for (int i = len - 1; i >= 0; i--) {
    set(i, v.get_data_ptr(i)->to_bool());
  }
  
  num = num << S >> S;
  return *this;
}

// sc_signed and sc_unsigned
// inline 
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

// inline 
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

// inline 
sc_int_base& 
sc_int_base::operator=(const sc_signed& v)
{
  int S = SC_INTWIDTH - width;
  int vlen = v.length();
  int minlen = min(width, vlen);

  int i = 0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  bit sgn = v.sign();
  for (; i < width; ++i) 
    set(i, sgn);     /* sign-extend */
  
  num = num << S >> S;
  
  return *this;
}

// inline 
sc_int_base& 
sc_int_base::operator=(const sc_unsigned& v)
{
  int S = SC_INTWIDTH - width;
  int vlen = v.length();
  int minlen = min(width, vlen);

  int i = 0;
  for (; i < minlen; ++i)
    set(i, v.test(i));
  for (; i < width; ++i)
    set(i, 0); /* zero-extend */
 
  num = num << S >> S;
  return *this;
}

// sc_int_subref
// inline 
sc_int_base::sc_int_base(const sc_int_subref& v)
{
    width = v.left - v.right + 1; 
#ifdef DEBUG_SYSTEMC
    check_bitwidth(width);
#endif
    num = v.operator int_type();
}

// inline 
sc_int_base& 
sc_int_base::operator=(const sc_int_subref& v)
{
  num = v.operator int_type(); 
  return *this; 
}


//Arithmetic operators
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



// inline 
sc_int_bitref 
sc_int_base::operator[] (int i)
{
#ifdef DEBUG_SYSTEMC
    check_index(i);
#endif
  return sc_int_bitref(this, i); 
}

// inline 
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

// inline 
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


//sc_int_bitref method definitions

sc_int_bitref& 
sc_int_bitref::operator = (const sc_int_bitref& b)
    {
      int_type val;
      int len = obj->width;

      obj->set(index, (b.obj)->test(b.index));
      if(index == len - 1){
	val = obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	obj->num = val;
      }
      return *this;
    }


sc_int_bitref& 
sc_int_bitref::operator = (bool b)
{
    int_type val;
    int len = obj->width;
    
    obj->set(index, b);
    if(index == len - 1){
	val = obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	obj->num = val;
      }
    return *this;
}


sc_int_bitref& 
sc_int_bitref::operator &= (bool b)
{
    int_type val;
    int len = obj->width;
    
    obj->num &= ~ ((!b) << index);
    if(index == len - 1){
	val = obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	obj->num = val;
    }
    return *this;
}


sc_int_bitref& 
sc_int_bitref::operator |= (bool b)
{
    int_type val;
    int len = obj->width;
    obj->num |= ((b & const_one_uint) << index);
    if(index == len - 1) {
	val = obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	obj->num = val;
    }
    return *this;
}


sc_int_bitref& 
sc_int_bitref::operator ^= (bool b)
{
    int_type val;
    int len = obj->width;
    obj->num ^= ((b & const_one_uint) << index);
    if (index == len - 1){
	val = obj->num;
	val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
	obj->num = val;
    }
    return *this;
}

//sc_int_subref definitions

// Implicit conversion to ...
sc_int_subref::operator int_type() const 
{ 
    int_type val = obj->num;
    if(left == obj->width - 1)
        return val >> right;
    else
        return val << (SC_INTWIDTH - (left + 1)) >> (SC_INTWIDTH - (left + 1) + right); 
}

  // Assignment operators
sc_int_subref& 
sc_int_subref::operator = (const sc_int_base& v)
{ 
    int_type val = obj->num;
    int len = obj->width;
    
    // Mask the bits out that are part-selected
    val &= mask_int[left][right];
    
    // Set the part-selected bits
    val |= (v.num << right) & ~mask_int[left][right];
    
    // Normalize the value
    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
    
    obj->num = val;
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
    int_type val = obj->num;
    int len = obj->width;
    
    // Mask the bits out that are part-selected
    val &= mask_int[left][right];
    
    // Set the part-selected bits
    val |= (v << right) & ~mask_int[left][right];
    
    // Normalize the value
    val = val << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len);
    
      obj->num = val;
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


