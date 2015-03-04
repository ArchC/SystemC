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

    sc_proxy.h -- proxy base class for vector data types

    Original Author: Gene Bushuyev, Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
//---------------------------------------------------------------------------

#ifndef sc_proxyH
#define sc_proxyH

#include "sc_iostream.h"
#include "numeric_bit/sc_signed.h"
#include "numeric_bit/sc_unsigned.h"
#include "numeric_bit/sc_int.h"
#include "numeric_bit/sc_uint.h"
#include "numeric_bit/sc_bigint.h"
#include "numeric_bit/sc_biguint.h"
#include "sc_signal.h"
#include "sc_string.h"
#include "sc_bit.h"
#include "sc_exception.h"
#include "sc_logic.h"
#include "sc_macros.h"

#if defined(__HP_aCC)
// aCC bug: premature instantiation
#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
// aCC bug: scope rules violation
#define for if(false);else for
#endif

#if defined(WIN32)
#include <limits>
const unsigned byte_size = std::numeric_limits<unsigned char>::digits;
#else
const unsigned byte_size = 8;
#endif
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif


template<int W> class sc_int;
template<int W> class sc_uint;
template<int W> class sc_bigint;
template<int W> class sc_biguint;
class sc_signed;
class sc_unsigned;
class sc_int_base;
class sc_uint_base;

namespace sc_bv_ns
// forward declaration of other vector classes
{
#if !defined(__HP_aCC)
using namespace std;
#endif

// constants for logical values and convesions
// see sc_logic.h

// bit-length of the underlying type
const unsigned ul_size = byte_size*sizeof(unsigned long);
// utility function
char* long_to_char(unsigned long l);
// conversion from typed string
sc_string convert_to_bin(const sc_string& s);
sc_string convert_to_logic(const sc_string& s);

#if !defined(__SUNPRO_CC)
// use to create binary constants
template<unsigned x> unsigned binary()
  { return x? 2*binary<x/10>() + x%10 : 0 ;}
// e.g. int i = binary<1010101>()
#endif
// forward declaration of the classes
template<class X> class sc_proxy; // common base class
class sc_bv_base;
class sc_lv_base;
template<int w> class sc_lv;
template<int w> class sc_bv;
template<int w> class sc_signal_rv;
template<class X> class sc_range;
template<class X, class Y> class sc_concat;
template<class X, class Y>  class sc_bitwise_and;
template<class X, class Y>  class sc_bitwise_or;
template<class X, class Y>  class sc_bitwise_xor;
template<class X> class sc_not;
template<class X> class sc_shift_left;
template<class X> class sc_shift_right;
template<class X> class sc_bit_proxy;
struct sc_bv_error;
// non-member assignment function
template<class X, class Y> void assign_(sc_proxy<X>& x, const sc_proxy<Y>& y);
// vector types that are not derived from sc_proxy
// definition is in sc_proxy.cpp file
// instantiations are in the respective modules
template<class X, class V> void assign_v_(sc_proxy<X>& x, const V& v);
#if !defined(__SUNPRO_CC)
template<class X, int W>
inline void assign_(sc_proxy<X>& x, const sc_int<W>& v){assign_v_(x,v);}
template<class X, int W>
inline void assign_(sc_proxy<X>& x, const sc_uint<W>& v){assign_v_(x,v);}
template<class X, int W>
inline void assign_(sc_proxy<X>& x, const sc_bigint<W>& v){assign_v_(x,v);}
template<class X, int W>
inline void assign_(sc_proxy<X>& x, const sc_biguint<W>& v){assign_v_(x,v);}
#endif
// non-member function verifies compatibility for assignment
template<class X, class Y>
inline void check_bounds(const sc_proxy<X>& ,const sc_proxy<Y>& );

//-------------------------------------------------------------------------
// sc_proxy class: this class is created for several purposes
// 1) hiding operators from global namespace
// that would be otherwise found by Koenig lookup
// 2) avoiding repeating the same operations in every class including proxies
// that could also be achieved by common base class, but this method allows
// 3) improve performance by using non-virtual functions
//-------------------------------------------------------------------------
template<class X> class sc_proxy
{
  public:
  //
  // comparisons
  //
  bool operator== (const char*)const;
  bool operator== (const bool*)const;
  bool operator== (const sc_unsigned&)const;
  bool operator== (const sc_signed&)const;
  bool operator== (int)const;
  bool operator== (unsigned int)const;
  bool operator== (long)const;
  bool operator== (unsigned long)const;
  bool operator== (char)const; // valid chars '0','1','x','X','z','Z'
  bool operator== (const sc_logic&)const;

  X& operator&= (const sc_unsigned&);
  X& operator|= (const sc_unsigned&);
  X& operator^= (const sc_unsigned&);

  X& operator&= (const sc_signed&);
  X& operator|= (const sc_signed&);
  X& operator^= (const sc_signed&);

  X& operator&= (unsigned int);
  X& operator|= (unsigned int);
  X& operator^= (unsigned int);

  X& operator&= (int);
  X& operator|= (int);
  X& operator^= (int);

  X& operator&= (unsigned long);
  X& operator|= (unsigned long);
  X& operator^= (unsigned long);

  X& operator&= (long);
  X& operator|= (long);
  X& operator^= (long);

  //
  // reduce operators
  //
  long and_reduce()  const;
  long nand_reduce() const { return sc_logic::not_table[and_reduce()&3]; }
  long or_reduce()   const;
  long nor_reduce()  const { return sc_logic::not_table[or_reduce()&3]; }
  long xor_reduce()  const;
  long xnor_reduce() const { return sc_logic::not_table[xor_reduce()&3]; }

  //
  // range selection
  //
  sc_range<X> range(unsigned hi_bit, unsigned lo_bit);
  const sc_range<const X> range(unsigned hi_bit, unsigned lo_bit) const;
  // the opposite of range for VSIA compatibility
  sc_range<X> operator()(unsigned hi_bit, unsigned lo_bit)
    {return range(lo_bit,hi_bit);}
  const sc_range<const X> operator()(unsigned hi_bit, unsigned lo_bit) const
    {return sc_range<const X>(back_const_cast(),lo_bit,hi_bit);}
  //
  // bit selection
  //
  sc_bit_proxy<X> operator [](unsigned n)
      {return sc_bit_proxy<X>(back_cast(),n);}
  long operator [](unsigned n) const
      { return back_cast().get_bit(n);}
  //
  // negation
  //
  const sc_not<X> operator~ ()const;
  //
  // rvalue concatenation operators ???
  //
  const sc_lv_base operator,( const char* s) const;
#if !defined(__HP_aCC)
  // aCC compiler tries to instantiate this template before its time and fails
  sc_concat<const X, const sc_lv<1> > operator,(const sc_logic& y) const
    { return sc_concat<const X, const sc_lv<1> >(back_cast(), sc_lv<1>(y));}
#endif
#if defined(_MSC_VER)
// VC6 has a bug in comma operator
// and Sun's SC5 doesn't implement member templates
// that's why all this mess
  template<class Y> sc_concat<X,Y> operator,(sc_proxy<Y>& y)
    { return sc_concat<X,Y>(back_cast(),y.back_cast());}
  template<class Y> sc_concat<const X, const Y> operator,(const sc_proxy<Y>& y)const
    { return sc_concat<const X, const Y>(back_cast(),y.back_cast());}
#endif

  //
  // string methods
  //
  sc_string to_string()const;
  const sc_string to_string(sc_numrep) const;
  // for compatibility purposes
  // no check is performed, may throw
#if !defined(SC_SAFE)
  void to_string( char* buffer ) const;
#endif
  // for compatibility purposes, not really necessary
  void print(ostream& os) const {os<<to_string();}
  // for debug purposes
  void dump(ostream& os) const
    {os<<"sc_bv Length="<<back_cast().length()<<"\n"""<<to_string()<<"""\n";}

  //
  // explicit conversion functions to other types
  //
  // to_signed() returns the signed-integer interpretation of sc_bv
  int to_int() const { return to_anything(); }
  unsigned to_uint() const {return to_anything();}
  int to_signed() const { return to_anything(); }
  unsigned to_unsigned() const {return to_anything();}
  long to_long() const {return to_anything();}
  unsigned long to_ulong() const {return to_anything();}

  // common assignments
#if !defined(__SUNPRO_CC)
  template<class Y> X& assign_(const sc_proxy<Y>& y)
    {sc_bv_ns::assign_(*this,y); return back_cast();}
#else
  // provide conversion to sc_lv_base temporary for Sun's SC5
  // warning: performance hit
  operator sc_lv_base() const;
#endif
  //
  // assignments from various types
  //
#if !defined(SC_SAFE)
  X& assign_(const char* b);
#endif
  X& assign_(const sc_logic_vector&);
  X& assign_(const sc_logic& L){back_cast().set_bit(0,L.to_long()); return back_cast();}
  X& assign_(const sc_logic::Log_enum& Le){back_cast().set_bit(0,Le);return back_cast();}
  // assignments of boolean type
#if !defined(SC_SAFE)
  X& assign_(const bool* b);
#endif
  X& assign_( const sc_unsigned& nv ){assign_v_(*this,nv); return back_cast();}
  X& assign_( const sc_signed& nv ){assign_v_(*this,nv); return back_cast();}
  X& assign_( const sc_uint_base& nv ){return assign_(uint64(nv));}
  X& assign_( const sc_int_base& nv ){return assign_(int64(nv));}
  X& assign_( long nv ){return assign_((unsigned long)nv);}
  X& assign_( unsigned long nv );
  X& assign_( int nv ) {return assign_((unsigned long)nv);}
  X& assign_( unsigned nv ){return assign_((unsigned long)nv);}
  X& assign_( int64 v );
  X& assign_( uint64 v );
  X& assign_( const sc_bit& b ){ return assign_((unsigned long)b.to_bool());}
  // we decided to change this function from v1.0 due to potential confusion
  // X& assign_( char c ){return assign_(sc_logic::char_to_logic[c]);} // v1.0
  X& assign_( char c ){return assign_((int)c);} // v1.0.1
  X& assign_(const sc_bool_vector&);

  public:
  // casts: these functions should be private
  // they are made public because Sun's 5.0 compiler doesn't
  // support member templates and thus they all were moved
  // outside of the class
  X& back_cast() {return static_cast<X&>(*this);}
  const X& back_cast()const {return static_cast<const X&>(*this);}
  const X& back_const_cast()const{return static_cast<const X&>(*this);}
  public:
  // additional functions required by VSIA
  X& operator <<= (int); // left shift with assignment
  X& operator >>= (int); // right shift with assignment

  protected:
  void check_bounds(const sc_bool_vector& v);
  void check_bounds(const sc_logic_vector& v);
  void check_bounds(unsigned n); // check if bit n accessible
  void check_wbounds(unsigned n);// check if word n accessible

  private:
  unsigned long to_anything() const;
};
//-------------------------------------------------------------------------
// main sc_proxy operators and functions (don't have to be friends)
//-------------------------------------------------------------------------
template<class X,class Y> inline
bool operator== (const sc_proxy<X>& x,const sc_proxy<Y>& y);
#if !defined(__SUNPRO_CC)
template<class X,int W> inline
bool operator== (const sc_proxy<X>&,const sc_int<W>&);
template<class X,int W> inline
bool operator== (const sc_proxy<X>&,const sc_uint<W>&);
template<class X,int W> inline
bool operator== (const sc_int<W>& i,const sc_proxy<X>& v) {return v==i;}
template<class X,int W> inline
bool operator== (const sc_uint<W>& i,const sc_proxy<X>& v){return v==i;}
#endif
template<class X> inline
bool operator== (const char* x,const sc_proxy<X>& b)  {return b==x;}
template<class X> inline
bool operator== (const bool* x,const sc_proxy<X>& b)  {return b==x;}
template<class X> inline
bool operator== (const sc_unsigned& x,const sc_proxy<X>& b) {return b==x;}
template<class X> inline
bool operator== (const sc_signed& x,const sc_proxy<X>& b) {return b==x;}
template<class X> inline
bool operator== (int x,const sc_proxy<X>& b) {return b==x;}
template<class X> inline
bool operator== (unsigned int x,const sc_proxy<X>& b) {return b==x;}
template<class X> inline
bool operator== (long x,const sc_proxy<X>& b) {return b==x;}
template<class X> inline
bool operator== (unsigned long x,const sc_proxy<X>& b) {return b==x;}

template<class X,class Y> inline
bool operator!= (const sc_proxy<X>& x,const sc_proxy<Y>& y) {return !(x==y);}
#if !defined(__SUNPRO_CC)
template<class X,int W> inline
bool operator!= (const sc_proxy<X>& x,const sc_int<W>& y)
{ return !(x==y);}
template<class X,int W> inline
bool operator!= (const sc_proxy<X>& x,const sc_uint<W>& y)
{ return !(x==y);}
template<class X,int W> inline
bool operator!= (const sc_int<W>& x,const sc_proxy<X>& y)
{ return !(x==y);}
template<class X,int W> inline
bool operator!= (const sc_uint<W>& x,const sc_proxy<X>& y)
{ return !(x==y);}
#endif
template<class X> inline
bool operator!= (const sc_proxy<X>& b, const char* x)  {return !(b==x);}
template<class X> inline
bool operator!= (const char* x,const sc_proxy<X>& b)  {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, const bool* x)  {return !(b==x);}
template<class X> inline
bool operator!= (const bool* x,const sc_proxy<X>& b)  {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, const sc_unsigned& x) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_unsigned& x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, const sc_signed& x) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_signed& x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, int x) {return !(b==x);}
template<class X> inline
bool operator!= (int x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, unsigned int x) {return !(b==x);}
template<class X> inline
bool operator!= (unsigned int x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, long x) {return !(b==x);}
template<class X> inline
bool operator!= (long x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, unsigned long x) {return !(b==x);}
template<class X> inline
bool operator!= (unsigned long x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, char x) {return !(b==x);}
template<class X> inline
bool operator!= (char x,const sc_proxy<X>& b) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_proxy<X>& b, const sc_logic& x) {return !(b==x);}
template<class X> inline
bool operator!= (const sc_logic& x,const sc_proxy<X>& b) {return !(b==x);}

// bitwise assignments returning proxy
// sc_proxy operators
template<class X,class Y> inline
X& operator&= (sc_proxy<X>& x,const sc_proxy<Y>& y);
template<class X,class Y> inline
X& operator|= (sc_proxy<X>& x,const sc_proxy<Y>& y);
template<class X,class Y> inline
X& operator^= (sc_proxy<X>& x,const sc_proxy<Y>& y);
// sc_int operators
#if !defined(__SUNPRO_CC)
template<class X,int W>  inline
X& operator&= (sc_proxy<X>&,const sc_int<W>&);
template<class X,int W>  inline
X& operator|= (sc_proxy<X>&,const sc_int<W>&);
template<class X,int W>  inline
X& operator^= (sc_proxy<X>&,const sc_int<W>&);
// sc_uint operators
template<class X,int W>  inline
X& operator&= (sc_proxy<X>&,const sc_uint<W>&);
template<class X,int W> inline
X& operator|= (sc_proxy<X>&,const sc_uint<W>&);
template<class X,int W> inline
X& operator^= (sc_proxy<X>&,const sc_uint<W>&);
#endif
// bitwise non-assignment returning proxy
template<class X,class Y> inline
const sc_bitwise_and<X,Y> operator& (const sc_proxy<X>& x,const sc_proxy<Y>& y);
template<class X,class Y> inline
const sc_bitwise_or<X,Y> operator| (const sc_proxy<X>& x,const sc_proxy<Y>& y);
template<class X,class Y> inline
const sc_bitwise_xor<X,Y> operator^ (const sc_proxy<X>& x,const sc_proxy<Y>& y);

#if !defined(_MSC_VER)
// VC6 has a bug in comma operator
// and Sun's SC5 doesn't implement member templates
// that's why all this mess

// l-value concatenation
template<class X,class Y> inline
sc_concat<X,Y> operator,(sc_proxy<X>& x,sc_proxy<Y>& y);

template<class X,class Y> inline
sc_concat<const X, const Y> operator,(const sc_proxy<X>& x,const sc_proxy<Y>& y);
#endif


// l-value shift operators
template<class X> inline
const sc_shift_left<X>  operator<<(const sc_proxy<X>& x,unsigned n);
template<class X>  inline
const sc_shift_right<X> operator>>(const sc_proxy<X>& x,unsigned n);

// rotations (cannot be a proxy type)
template<class X>
const X lrotate(const sc_proxy<X>& x, int n)
{
  if(n<0)
    REPORT_ERROR(1000,"");
  n %= x.back_cast().length(); // no need to rotate more than a full cirle
  if(n)
    return x.back_cast()<<n | x.back_cast()>>(x.back_cast().length()-n);
  else
    return x;
}
template<class X>
const X rrotate(const sc_proxy<X>& x, int n)
{
  if(n<0)
    REPORT_ERROR(1000,"");
  n %= x.back_cast().length(); // no need to rotate more than a full cirle
  if(n)
    return x.back_cast()>>n | x.back_cast()<<(x.back_cast().length()-n);
  else
    return x;
}

// reverse function
template<class X>
const sc_range<X> reverse(const sc_proxy<X>& x)
{
  return sc_range<X>(x,0,x.back_cast().length()-1);
}

// shift array of type T: T t<<n
template<class T>
unsigned long array_get_word(const T& p, unsigned length, unsigned index);
template<class T>
void array_set_word(T& p, const T t, unsigned length, unsigned index);


// ----------------------------------------------------------------------------
// implementation
// ----------------------------------------------------------------------------
// range select
template<class T>
sc_range<T> sc_proxy<T>::range(unsigned hi_bit, unsigned lo_bit)
{
  return sc_range<T>(back_cast(),hi_bit,lo_bit);
}
template<class T>
const sc_range<const T> sc_proxy<T>::range(unsigned hi_bit, unsigned lo_bit)const
{
  return sc_range<const T>(back_const_cast(),hi_bit,lo_bit);
}

// general templates (sc_proxy template)
#if defined(__SUNPRO_CC)
// warning: performance hit
template<class X>
sc_proxy<X>::operator sc_lv_base() const
{
  sc_lv_base v(back_cast().length());
  sc_bv_ns::assign_(v,back_cast());
  return v;
}
#endif

template<class X,class Y>
bool operator== (const sc_proxy<X>& x,const sc_proxy<Y>& y)
{
  bool equal = x.back_cast().length()==y.back_cast().length();
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  unsigned i=0;
  while(equal && i<len)
  {
    equal = x.back_cast().get_word(i)==y.back_cast().get_word(i) &&
            x.back_cast().get_cword(i)==y.back_cast().get_cword(i);
    i++;
  }
  return equal;
}
template<class X> bool sc_proxy<X>::operator== (const char* y)const
{
  bool equal = back_cast().length()==(int)strlen(y);
  unsigned len = back_cast().length();
  unsigned i=0;
  while(equal && i<len)
  {
    equal = back_cast().get_bit(i)==sc_logic::char_to_logic[y[len-i-1]];
    i++;
  }
  return equal;
}
template<class X> bool sc_proxy<X>::operator== (const bool* y)const
{
  bool equal = true;
  unsigned len = back_cast().length();
  unsigned i=0;
  while(equal && i<len)
  {
    equal = back_cast().get_bit(i) == (long)(y[i]);
    i++;
  }
  return equal;
}
template<class X> bool sc_proxy<X>::operator== (const sc_unsigned& y)const
{
  bool equal = static_cast<int>( back_cast().length() ) == y.length();
  int len = back_cast().length();
  int i = 0;
  while(equal && i<len)
  {
    equal = back_cast().get_bit(i) == (long)(y[i]);
    i++;
  }
  return equal;
}
template<class X> bool sc_proxy<X>::operator== (const sc_signed& y)const
{
  bool equal = static_cast<int>( back_cast().length() ) == y.length();
  int len = back_cast().length();
  int i = 0;
  while(equal && i<len)
  {
    equal = back_cast().get_bit(i) == (long)(y[i]);
    i++;
  }
  return equal;
}

#if !defined(__SUNPRO_CC)
template<class X,int W>
bool operator== (const sc_proxy<X> x, const sc_int<W>& y)
{
  bool equal = x.back_cast().length()==W;
  if(equal)
    equal = ((int64)y & ~0ul) == x.back_cast().get_word(0) &&
      x.back_cast().get_cword(0)==0;
    if(equal && W>ul_size)
      equal = ((int64)y>>ul_size & ~0ul) == x.back_cast().get_word(1) &&
        x.back_cast().get_cword(1)==0;
  return equal;
}

template<class X,int W>
bool operator== (const sc_proxy<X> x, const sc_uint<W>& y)
{
  bool equal = x.back_cast().length()==W;
  if(equal)
    equal = ((uint64)y & ~0ul) == x.back_cast().get_word(0) &&
      x.back_cast().get_cword(0)==0;
    if(equal && W>ul_size)
      equal = ((uint64)y>>ul_size & ~0ul) == x.back_cast().get_word(1) &&
        x.back_cast().get_cword(1)==0;
  return equal;
}
#endif

template<class X> bool sc_proxy<X>::operator== (int y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==(unsigned long)y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool sc_proxy<X>::operator== (unsigned int y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==(unsigned long)y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool sc_proxy<X>::operator== (long y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==(unsigned long)y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool sc_proxy<X>::operator== (unsigned long y)const
{
  return back_cast().length()<=(int)ul_size &&
         back_cast().get_word(0)==y &&
         back_cast().get_cword(0)==0;
}
template<class X> bool sc_proxy<X>::operator== (char y)const
{
  if(y!='0' && y!='1' && y!='x' && y!='X' && y!='z' && y!='Z')
    REPORT_WARNING(2010,sc_string::to_string(" %c is not a correct value",y).c_str());
  return back_cast().length()==1 && y>0 &&
    back_cast().get_bit(0)==sc_logic::char_to_logic[y];
}

template<class X> bool sc_proxy<X>::operator== (const sc_logic& y)const
{
  return (*this)==y.to_long();
}

template<class X,class Y> X& operator&= (sc_proxy<X>& x,const sc_proxy<Y>& y)
{
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  if(x.back_cast().length()!=y.back_cast().length())
    REPORT_ERROR(1002,"");
  for(unsigned i=0; i<len; i++)
  {
    unsigned long control = x.back_cast().get_cword(i)&y.back_cast().get_word(i) |
      x.back_cast().get_cword(i) & y.back_cast().get_cword(i) | x.back_cast().get_word(i)&
      y.back_cast().get_cword(i);
    x.back_cast().set_cword(i,control);
    x.back_cast().set_word(i, x.back_cast().get_word(i) & y.back_cast().get_word(i) |
      control);
  }
  return x.back_cast();
}
template<class X,class Y> X& operator|= (sc_proxy<X>& x,const sc_proxy<Y>& y)
{
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  if(x.back_cast().length()!=y.back_cast().length())
    REPORT_ERROR(1002,"");
  for(unsigned i=0; i<len; i++)
  {
    unsigned long control = x.back_cast().get_cword(i)&~y.back_cast().get_word(i) |
      x.back_cast().get_cword(i)&y.back_cast().get_cword(i) |
      ~x.back_cast().get_word(i)&y.back_cast().get_cword(i);
    x.back_cast().set_cword(i,control);
    x.back_cast().set_word(i, x.back_cast().get_word(i) | y.back_cast().get_word(i) |
       control);
  }
  return x.back_cast();
}
template<class X,class Y> X& operator^= (sc_proxy<X>& x,const sc_proxy<Y>& y)
{
  unsigned len = (x.back_cast().length()-1)/ul_size+1;
  if(x.back_cast().length()!=y.back_cast().length())
    REPORT_ERROR(1002,"");
  for(unsigned i=0; i<len; i++)
  {
    unsigned long control = x.back_cast().get_cword(i)|y.back_cast().get_cword(i);
    x.back_cast().set_cword(i,control);
    x.back_cast().set_word(i, x.back_cast().get_word(i) ^ y.back_cast().get_word(i) |
      control);
  }
  return x.back_cast();
}

template<class X> X& sc_proxy<X>::operator&= (const sc_unsigned& y)
{
  const unsigned len = min(back_cast().length(),y.length());
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=sc_logic::and_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator|= (const sc_unsigned& y)
{
  const unsigned len = min(back_cast().length(),y.length());
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=sc_logic::or_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator^= (const sc_unsigned& y)
{
  const unsigned len = min(back_cast().length(),y.length());
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=sc_logic::xor_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}

template<class X> X& sc_proxy<X>::operator&= (const sc_signed& y)
{
  const unsigned len = min(back_cast().length(),(unsigned)(y.length()));
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=sc_logic::and_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator|= (const sc_signed& y)
{
  const unsigned len = min(back_cast().length(),(unsigned)(y.length()));
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=sc_logic::or_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator^= (const sc_signed& y)
{
  const unsigned len = min(back_cast().length(),(unsigned)(y.length()));
  for(unsigned i=0; i<len; i++)
  {
    back_cast()[i]=sc_logic::xor_table[back_cast()[i]][y[i]];
  }
  return back_cast();
}

#if !defined(__SUNPRO_CC)
template<class X,int W>
X& operator&= (sc_proxy<X>& x,const sc_int<W>& y)
{
  const unsigned len = min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=sc_logic::and_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator|= (sc_proxy<X>& x,const sc_int<W>& y)
{
  const unsigned len = min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=sc_logic::or_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator^= (sc_proxy<X>& x,const sc_int<W>& y)
{
  const unsigned len = min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=sc_logic::xor_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}

template<class X,int W>
X& operator&= (sc_proxy<X>& x,const sc_uint<W>& y)
{
  const unsigned len = min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=sc_logic::and_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator|= (sc_proxy<X>& x,const sc_uint<W>& y)
{
  const unsigned len = min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=sc_logic::or_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
template<class X,int W>
X& operator^= (sc_proxy<X>& x,const sc_uint<W>& y)
{
  const unsigned len = min(x.back_cast().length(),(unsigned)W);
  for(unsigned i=0; i<len; i++)
    x.back_cast()[i]=sc_logic::xor_table[x.back_cast()[i]][y[i]];
  return x.back_cast();
}
#endif

template<class X> X& sc_proxy<X>::operator^= (unsigned int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator&= (unsigned int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator|= (unsigned int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X> X& sc_proxy<X>::operator^= (int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator&= (int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator|= (int y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X> X& sc_proxy<X>::operator^= (unsigned long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator&= (unsigned long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator|= (unsigned long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X> X& sc_proxy<X>::operator^= (long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)^y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator&= (long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)&y|control);
  return back_cast();
}
template<class X> X& sc_proxy<X>::operator|= (long y)
{
  unsigned long control = back_cast().get_cword(0)&y;
  back_cast().set_word(0, back_cast().get_word(0)|y|control);
  return back_cast();
}

template<class X>
unsigned long sc_proxy<X>::to_anything() const
{
  // only 0 word is returned
  if(back_cast().length()==0)
    return 0; // assume empty vector is equivalent to 0
  // can't convert logic values other than 0 and 1
  if(back_cast().get_cword(0))
    REPORT_WARNING(2003,"");
  return back_cast().get_word(0);
}
template<class T>
sc_string sc_proxy<T>::to_string()const
{
   unsigned N = back_cast().length();
   sc_string buff(N+1);
   for(unsigned i=0; i<N; i++)
     buff.set(i, sc_logic::logic_to_char[back_cast()[N-i-1]&3]);
   buff.set(N,0);
   return buff;
}
template<class T>
const sc_string sc_proxy<T>::to_string(sc_numrep base) const
{ // slow
#if defined(FORMATTED_STRING_CONVERSION)
  sc_unsigned us(to_string());
  return us(base);
#else
  REPORT_ERROR(2,"");
#endif
}

#if !defined(SC_SAFE)
template<class T>
void sc_proxy<T>::to_string( char* buffer ) const
{
  unsigned N = back_cast().length();
  for(unsigned i=0; i<N; i++)
    buffer[i] = sc_logic::logic_to_char[back_cast().get_bit(N-i-1)&3];
  buffer[N]=0;
}
#endif

template<class X>
ostream& operator<<(ostream& os, const sc_proxy<X>& f)
{
  return os<<f.back_cast().to_string();
}

// operators returning bitwise proxies
template<class X,class Y>
const sc_bitwise_and<X,Y> operator& (const sc_proxy<X>& x,const sc_proxy<Y>& y)
{ return sc_bitwise_and<X,Y>(x.back_cast(),y.back_cast()); }
template<class X,class Y>
const sc_bitwise_or<X,Y> operator| (const sc_proxy<X>& x,const sc_proxy<Y>& y)
{ return sc_bitwise_or<X,Y>(x.back_cast(),y.back_cast()); }
template<class X,class Y>
const sc_bitwise_xor<X,Y> operator^ (const sc_proxy<X>& x,const sc_proxy<Y>& y)
{ return sc_bitwise_xor<X,Y>(x.back_cast(),y.back_cast());}
// operators returning negation proxies
template<class X> const sc_not<X> sc_proxy<X>::operator~ () const
{ return sc_not<X>(back_cast()); }
// operators returning concat functions  proxies
#if !defined(_MSC_VER)
// this operators are defined in sc_proxy class for VC6 compiler
template<class X,class Y>
inline sc_concat<X,Y> operator,(sc_proxy<X>& x, sc_proxy<Y>& y)
{ return sc_concat<X,Y>(x.back_cast(),y.back_cast());}

template<class X,class Y>
inline sc_concat<const X, const Y> operator,(const sc_proxy<X>& x, const sc_proxy<Y>& y)
{ return sc_concat<const X, const Y>(x.back_cast(),y.back_cast());}
#endif

#if !defined(__HP_aCC)

template<class X>
inline const sc_lv_base sc_proxy<X>::operator,( const char* s) const
{
  // some compilers violate 13.3.3p1 (the best viable function)
  // and use non-const back_cast() function if used below
  // have to explicitly call const version
  sc_lv_base result = (back_const_cast(),sc_lv_base(s));
  return result;
}
#if !defined(_MSC_VER)
template<class X>
inline const sc_lv_base operator,( const char* s, const sc_proxy<X>& x)
{
  sc_lv_base result = (sc_lv_base(s),x.back_const_cast());
  return result;
}
#endif
#endif

template<class X, class Y>
inline void check_bounds(const sc_proxy<X>& x,const sc_proxy<Y>& y)
{ // verify compatibility for assignment
  if(x.back_cast().length()!=y.back_cast().length())
    REPORT_ERROR(1001,"");
}
template<class X>
inline void sc_proxy<X>::check_bounds(const sc_bool_vector& v)
{
  if(v.length()!=back_cast().length())
    REPORT_ERROR(1001,"");
}
template<class X>
inline void sc_proxy<X>::check_bounds(const sc_logic_vector& v)
{
  if(v.length()!=back_cast().length())
    REPORT_ERROR(1001,"");
}
template<class X>
inline void sc_proxy<X>::check_bounds(unsigned n) // check if bit n accessible
{
  if((int)n>=back_cast().length()) REPORT_ERROR(1000,"");
}
template<class X>
inline void sc_proxy<X>::check_wbounds(unsigned n)// check if word n accessible
{
   if(n>(back_cast().length()-1)/ul_size) REPORT_ERROR(1000,"");
}

// non-member template for common assignments
template<class X, class Y>
void assign_(sc_proxy<X>& x, const sc_proxy<Y>& y)
{
  if((void*)&x == (void*)&y) return; // no self-assignment
  check_bounds(x,y);
  const unsigned reserved = (x.back_cast().length()-1)/ul_size +1;
  for(register unsigned i =0; i<reserved; i++)
  {
     x.back_cast().set_word(i,y.back_cast().get_word(i));
     x.back_cast().set_cword(i,y.back_cast().get_cword(i));
  }
  x.back_cast().clean_tail();
}
// vector types that are not derived from sc_proxy
// must have length() function and operator[]
template<class X, class V>
void assign_v_(sc_proxy<X>& x, const V& v)
{
  int len = x.back_cast().length();
  // it's not clear what to do when lengths mismatch
  // at this time issue a warning
  if(len> v.length())
  {
    REPORT_WARNING(2008,"");
    len = v.length();
  }
  else
    if(x.back_cast().length()< v.length())
      REPORT_WARNING(2009,"");
  // since general types do not support block-copy
  // have to use bit-copy
  for(int i = 0; i<len; i++)
    x.back_cast().set_bit(i,v[i]);
  for(int i = len; i< x.back_cast().length(); i++)
    x.back_cast().set_bit(i,0);
}

// assignment from a char string
#if !defined(SC_SAFE)
template<class X> X& sc_proxy<X>::assign_(const char* bb)
{ // ATTN: only common bits are assigned
  const sc_string b = convert_to_logic(bb);
#ifndef _MSC_VER
  int len = min(back_cast().length(),b.length());
#else
  int len = min(back_cast().length(),b.length());
#endif
  register int i;
  for(i =0; i<len; i++)
     back_cast().set_bit(i,sc_logic::char_to_logic[b[len-i-1]]);
  for(i =len; i<back_cast().length(); i++)
     back_cast().set_bit(i,0);
  back_cast().clean_tail();
  return back_cast();
}
template<class X> X& sc_proxy<X>::assign_(const bool* b)
{
  // verification of the length is impossible
  // may throw
  for(register int i =0; i<back_cast().length(); i++)
     back_cast().set_bit(i,b[i]);
  back_cast().clean_tail();
  return back_cast();
}
#endif

template<class X> X& sc_proxy<X>::assign_( unsigned long nv )
{
  back_cast().set_word(0,nv);
  back_cast().set_cword(0,0);
  int words = (back_cast().length()-1)/ul_size+1;
  // must fill the rest with 0
  for(int i=1; i<words; i++)
  {
    back_cast().set_word(i,0);
    back_cast().set_cword(i,0);
  }
  back_cast().clean_tail();
  return back_cast();
}

template<class X> X& sc_proxy<X>::assign_( int64 v )
{
  back_cast().set_word(0,(uint64)v & ~0ul);
  back_cast().set_cword(0,0);
  if(back_cast().length()>(int)ul_size) // int64 can be longer than 1 long
  {
    back_cast().set_word(1,(uint64)v>>ul_size & ~0ul);
    back_cast().set_cword(1,0);
    int words = (back_cast().length()-1)/ul_size+1;
    // must fill the rest with 0
    for(int i=2; i<words; i++)
    {
      back_cast().set_word(i,0);
      back_cast().set_cword(i,0);
    }
  }
  back_cast().clean_tail();
  return back_cast();
}

template<class X> X& sc_proxy<X>::assign_( uint64 v )
{
  back_cast().set_word(0,v & ~0ul);
  back_cast().set_cword(0,0);
  if(back_cast().length()>(int)ul_size)
  {
    back_cast().set_word(1,v>>ul_size & ~0ul);
    back_cast().set_cword(1,0);
    int words = (back_cast().length()-1)/ul_size+1;
    // must fill the rest with 0
    for(int i=2; i<words; i++)
    {
      back_cast().set_word(i,0);
      back_cast().set_cword(i,0);
    }
  }
  back_cast().clean_tail();
  return back_cast();
}

template<class X> X& sc_proxy<X>::assign_(const sc_bool_vector& v)
{ // slow
  check_bounds(v);
  for(int i=0; i<back_cast().length(); i++)
    back_cast().set_bit(i,v[i]);
  back_cast().clean_tail();
  return back_cast();
}

template<class X> X& sc_proxy<X>::assign_(const sc_logic_vector& v)
{ // slow
  check_bounds(v);
  for(int i=0; i<back_cast().length(); i++)
  // must convert sc_logic values to longs used by sc_lv
    back_cast().set_bit(i,v[i].to_long());
  back_cast().clean_tail();
  return back_cast();
}

// reduction operators
template<class X>
long sc_proxy<X>::and_reduce()  const
{
  const unsigned len = back_cast().length();
  const unsigned full_words = len/ul_size;
  register bool finished = false;
  register bool zero=false;
  register bool undefined=false;
  register unsigned i = 0;
  // no need to go beyond the first 0
  while(!finished && i<full_words)
  {
    zero =  back_cast().get_word(i)!=~0ul;
    undefined =  back_cast().get_cword(i)!=0;
    finished = zero || undefined;
    i++;
  }
  i=full_words*ul_size;
  while(!finished && i<len)
  {
    register long value = back_cast().get_bit(i);
    zero =  !(value & 1);
    undefined =  value & 2;
    finished = zero || undefined;
    i++;
  }
  if(undefined) return 3;
  else
    return !zero;
}

template<class X>
long sc_proxy<X>::or_reduce()   const
{
  const unsigned len = back_cast().length();
  const unsigned full_words = len/ul_size;
  register bool finished = false;
  register bool zero=false;
  register bool undefined=false;
  register unsigned i = 0;
  // no need to go beyond the first 1
  while(!finished && i<full_words)
  {
    zero =  back_cast().get_word(i)==0;
    undefined =  back_cast().get_cword(i)!=0;
    finished = !zero || undefined;
    i++;
  }
  i=full_words*ul_size;
  while(!finished && i<len)
  {
    register long value = back_cast().get_bit(i);
    zero =  !(value & 1);
    undefined =  value & 2;
    finished = !zero || undefined;
    i++;
  }
  if(undefined) return 3;
  else
    return !zero;
}
template<class X>
long sc_proxy<X>::xor_reduce()  const
{
  const unsigned len = back_cast().length();
  const unsigned n_words = (len-1)/ul_size+1;
  register unsigned long result = 0;
  register unsigned i = 0;
  register bool undefined = false;
  while(!undefined && i<n_words)
  {
    result ^= back_cast().get_word(i);
    undefined = back_cast().get_cword(i)!=0;
    i++;
  }
  i = 1;
  while(!undefined && i<ul_size)
  {
    result ^=(result>>i)&1;
    i++;
  }
  result = result &1;
  if(undefined)
    return 3;
  else
    return result;
}
// l-value shift operators
template<class X> inline
const sc_shift_left<X>  operator<<(const sc_proxy<X>& x,unsigned n)
{
   return sc_shift_left<X>(x.back_cast(),n);
}
template<class X>  inline
const sc_shift_right<X> operator>>(const sc_proxy<X>& x,unsigned n)
{
   return sc_shift_right<X>(x.back_cast(),n);
}

// additional functions required by VSIA
template<class X>
X& sc_proxy<X>::operator <<= (int i) // left shift with assignment
{
  X temp((*this)<<i);
  (*this) = temp;
  return back_cast();
}
template<class X>
X& sc_proxy<X>::operator >>= (int i) // right shift with assignment
{
  X temp((*this)>>i);
  (*this) = temp;
  return back_cast();
}

// -------------------- end of sc_proxy -----------------------------------------


// get word from array of type T with shift
template<class T>
unsigned long array_get_word(const T& p, unsigned index)
{
  const unsigned shift = index%ul_size;
  const unsigned word = index / ul_size;
  unsigned long result;
  if(index>=p.length())
    REPORT_ERROR(1000,"");
  if(shift == 0)
    result = p.get_word(word);
  else
    if((word+1)*ul_size>p.length()) // incomplete word
      result = p.get_word(word)>>shift;
    else
      result = p.get_word(word)>>shift | p.get_word(word+1)<<(ul_size - shift);
  // zero the end bits
#if defined(__HP_aCC)
  if(index+ul_size > p.length())
    return result & ~0ul>>(shift-p.length()%ul_size);
#else
  if(index+ul_size > length)
    return result & ~0ul>>(shift-length%ul_size);
#endif
  else
    return result;
}

// set word to array of type T with shift
template<class T>
void array_set_word(T& p, const T t, unsigned length, unsigned index)
{
  const unsigned shift = index%ul_size;
  const unsigned word = index / ul_size;
#if defined(__HP_aCC)
  const unsigned len_shift = length % ul_size;
#else
  const unsigned len_shift = length % T_zise;
#endif
  T result;
  if(index>=length)
    REPORT_ERROR(1000,"");
  if(index+ul_size > length)
  {
    if(shift == 0)
      p.set_word(word, p.get_word(word)>>length%ul_size<< length%ul_size
        | t<<(ul_size-length%ul_size)>>(ul_size-length%ul_size));
    else
      if((word+1)*ul_size>length) // incomplete word
      {
        T mask = ~0ul>>shift<<(shift+ul_size-len_shift)<< (ul_size-len_shift);
        p.set_word(word, p.get_word(word) & ~mask |
          t<<(shift+ul_size-len_shift)>>(ul_size-len_shift));
      }
      else
      {
        p.set_word(word, p.get_word(word)<<(ul_size-shift)>>(ul_size-shift)
          | t<<shift);
        p.set_word(word+1, p.get_word(word+1)>>len_shift<<len_shift |
          t<<(shift-len_shift)>>(shift-len_shift) >>(ul_size-shift));
      }
  }
  else
    if(shift == 0)
      p.set_word(word)=t;
    else
      if((word+1)*ul_size>length) // incomplete word
        p.set_word(word, p.get_word(word)<<(ul_size-shift)>>(ul_size-shift)
         | t<<shift);
      else
      {
        p.set_word(word, p.get_word(word)<<(ul_size-shift)>>(ul_size-shift)
          | t<<shift);
        p.set_word(word+1, p.get_word(word+1)>>shift<<shift | t>>(ul_size-shift));
      }
}
} // namespace
//---------------------------------------------------------------------------
#endif
