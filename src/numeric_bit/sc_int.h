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

    sc_int.h -- A sc_int is a signed integer whose width is less than
    the machine's native integer width. We provide two implementations
    (i) sc_int with bitwidth between 1 - 64 (ii) sc_int with bitwidth
    between 1 - 32 Implementation (i) is the default implementation,
    while implementation (ii) can be used only if the class library is
    compiled with -D_32BIT_.  Unlike arbitrary precision, arithmetic
    and bitwise operations are performed using the native types (hence
    capped at 32/64 bits). The sc_int integer is useful when the user
    does not need arbitrary precision and the performance is superior
    to sc_bigint/sc_biguint.

    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: 
     - Resolved ambiguity with sc_(un)signed.
     - Merged the code for 64- and 32-bit versions via the constants in sc_nbdefs.h.
     - Eliminated redundant file inclusions.
    
    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/


#ifndef SC_INT_H
#define SC_INT_H

#include "sc_nbdefs.h"
#include "../sc_exception.h"

// Forward declarations
class sc_int_bitref;
class sc_int_subref;
class sc_int_base;
class sc_bool_vector;
class sc_logic_vector;
class sc_signed;
class sc_unsigned;
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;

template<class T1, class T2> class sc_int_concat;
template<int W> class sc_int;
namespace sc_bv_ns
{
  template<int W> class sc_bv;
  template<int W> class sc_lv;
}
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_lv;

extern const uint_type mask_int[SC_INTWIDTH][SC_INTWIDTH];

// Base class for sc_int. 

class sc_int_base {
  
  friend class sc_int_bitref;
  friend class sc_int_subref;

public:
  /* Constructors */
  sc_int_base() : num(0), width(SC_INTWIDTH) 
    { } 
  explicit sc_int_base(int w);

  sc_int_base(int_type v, int w);

  sc_int_base(const sc_int_base&           v) 
    { width = v.width; num = v.num; }
  sc_int_base(const sc_int_subref&         v);
#if !defined(__SUNPRO_CC)
  template <class T1, class T2> 
  sc_int_base(const sc_int_concat<T1, T2>& v) 
  { 
      width = v.bitwidth();
      int S = SC_INTWIDTH - width;
      num = (v.operator int_type()) << S >> S;
  }
#endif
  sc_int_base(const sc_bool_vector&        v); 
  sc_int_base(const sc_logic_vector&       v);  
  sc_int_base(const sc_signed&             v);
  sc_int_base(const sc_unsigned&           v);

  // Destructor
  ~sc_int_base() { }

  // Implicit conversion to ...
  operator int_type() const { return num; }

  int bitwidth() const { return width; }
  int length() const { return width; }
  
  unsigned int to_uint() const { return (unsigned int) num; }
  int to_int() const { return (int) num; }
  int64 to_int64() const { return num; }
  uint64 to_uint64() const { return num; }
#ifndef _32BIT_
  long long_low() const
    { return (long) (num & const_32ones_ull); }
  long long_high() const
    { return (long) ((num >> 32)  & const_32ones_ull); }
#endif

  bool test(int i) const
    { return (0 != (num & (const_one_uint << i))); }
  void set(int i)
    { num |= (const_one_uint << i); }
  void set(int i, bool v)
    {
      //int_type val = num;
      //val &= ~(const_one_uint << i);
      //val |= ((v & const_one_uint) << i);
      //num = val;
	  v? num |= int_type(1)<<i : num &= ~(int_type(1)<<i);
    }

  // Bit-select and part-select operators
  
  sc_int_bitref operator [] (int i); 
  bool operator [] (int i) const 
    { return test(i); }
  sc_int_subref range(int left, int right); 
  int_type range(int left, int right) const;

  // Assignment operators
  sc_int_base& operator = (int_type                     v);
  sc_int_base& operator = (const sc_int_base&           v) 
    { int S = (SC_INTWIDTH - width); num = v.num << S >> S; return *this; }
  sc_int_base& operator = (const sc_int_subref&         v); 
#if !defined(__SUNPRO_CC)
  template< class T1, class T2 > 
  sc_int_base& operator = (const sc_int_concat<T1, T2>& v)
    { int S = SC_INTWIDTH - width; num = (v.operator int_type()) << S >> S; return *this; }
#endif
  sc_int_base& operator = (const sc_bool_vector&        v);
  sc_int_base& operator = (const sc_logic_vector&       v);
  sc_int_base& operator = (const sc_signed&             v);
  sc_int_base& operator = (const sc_unsigned&           v);
  sc_int_base& operator = (const sc_fxval&              v);
  sc_int_base& operator = (const sc_fxval_fast&         v);
  sc_int_base& operator = (const sc_fxnum&              v);
  sc_int_base& operator = (const sc_fxnum_fast&         v);

  // Arithmetic operators
  sc_int_base& operator += (int_type v);
  sc_int_base& operator -= (int_type v);
  sc_int_base& operator *= (int_type v);
  sc_int_base& operator /= (int_type v);
  sc_int_base& operator %= (int_type v);

  
  // Bitwise operators
  sc_int_base& operator &= (int_type v);
  sc_int_base& operator |= (int_type v);
  sc_int_base& operator ^= (int_type v); 
  sc_int_base& operator <<= (int_type v);
  sc_int_base& operator >>= (int_type v);

  // Prefix and postfix increment and decrement operators
  sc_int_base& operator ++ (); // prefix
  const sc_int_base operator ++ (int); // postfix
  sc_int_base& operator -- (); //prefix
  const sc_int_base operator -- (int); //postfix 

  // Relational operators
  friend bool operator == (const sc_int_base& u, const sc_int_base& v)
    { return u.num == v.num; }
  friend bool operator != (const sc_int_base& u, const sc_int_base& v)
    { return u.num != v.num; }
  friend bool operator <  (const sc_int_base& u, const sc_int_base& v)
    { return u.num < v.num; }
  friend bool operator <= (const sc_int_base& u, const sc_int_base& v)
    { return u.num <= v.num; }
  friend bool operator >  (const sc_int_base& u, const sc_int_base& v)
    { return u.num > v.num; }
  friend bool operator >= (const sc_int_base& u, const sc_int_base& v)
    { return u.num >= v.num; }

  // concatenation
  sc_int_concat<sc_int_base,sc_int_base> operator ,(sc_int_base& r);
  sc_int_concat<sc_int_base,sc_int_base> operator ,( const sc_int_base& r )const;
  sc_int_concat<sc_int_base,sc_int_bitref> operator ,(sc_int_bitref& r);
  sc_int_concat<sc_int_base,sc_int_bitref> operator ,( const sc_int_bitref& r )const;
  sc_int_concat<sc_int_base,sc_int_subref> operator ,(sc_int_subref& r);
  sc_int_concat<sc_int_base,sc_int_subref> operator ,(const sc_int_subref& r)const;
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
  operator ,(sc_int_concat<T1,T2>& r )
  { return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >( *this, r );  }

  template<class T1, class T2>   sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
  operator ,(const sc_int_concat<T1, T2>& r )const
  {
    return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >(
    const_cast<sc_int_base&>(*this), const_cast<sc_int_concat<T1,T2>&>(r) );
  }
#endif

public:
  // These can be protected at the expense of adding a inlined member
  // function to get 'num' in the derived class.
  int_type num;
  int width;
};  // sc_int_base


// Template class sc_int<W> is the interface that the user sees. It is
// derived from sc_int_base and most of its methods are just wrappers
// that call the corresponding method in the parent class. Note that
// the width of sc_int datatype is specified as a template parameter.

template< int W = SC_INTWIDTH>
class sc_int : public sc_int_base {
  friend class sc_int_bitref;
  friend class sc_int_subref;

public:
  // Constructors
  sc_int() : sc_int_base(W) 
    { }
  sc_int(int_type                      v);
  sc_int(const sc_int<W>&              v) : sc_int_base(v) 
    { }
  sc_int(const sc_int_base&            v) : sc_int_base(v) 
    { }
  sc_int(const sc_int_subref&          v) : sc_int_base(v) 
    { }
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> 
  sc_int(const sc_int_concat<T1,T2>&   v) : sc_int_base(v) 
    { }
#endif
  sc_int(const sc_bool_vector&         v) : sc_int_base(v) 
    { }
  sc_int(const sc_logic_vector&        v) : sc_int_base(v) 
    { }
  sc_int(const sc_signed&              v) : sc_int_base(v)
    { }
  sc_int(const sc_unsigned&            v) : sc_int_base(v) 
    { }
  explicit sc_int(const sc_fxval&      v);
  explicit sc_int(const sc_fxval_fast& v);
  explicit sc_int(const sc_fxnum&      v);
  explicit sc_int(const sc_fxnum_fast& v);
  sc_int(const sc_bv<W>&               v);
  sc_int(const sc_lv<W>&               v);
  
  // Assignment operators
  sc_int& operator = (int_type                     v);
  sc_int& operator = (const sc_int_base&           v) 
    { num = v.num << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); return *this; }
  sc_int& operator = (const sc_int_subref&         v) 
    { sc_int_base::operator=(v); return *this; }
  sc_int& operator = (const sc_int<W>&             v) 
    { num = v.num; return *this; }
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> 
  sc_int& operator = (const sc_int_concat<T1, T2>& v) 
    { sc_int_base::operator=(v); return *this; }
#endif
  sc_int& operator = (const sc_bool_vector&        v) 
    { sc_int_base::operator=(v); return *this; }
  sc_int& operator = (const sc_logic_vector&       v) 
    { sc_int_base::operator=(v); return *this; }
  sc_int& operator = (const sc_signed&             v) 
    { sc_int_base::operator=(v); return *this;}
  sc_int& operator = (const sc_unsigned&           v) 
    { sc_int_base::operator=(v); return *this; }
  sc_int& operator = (const sc_fxval&              v);
  sc_int& operator = (const sc_fxval_fast&         v);
  sc_int& operator = (const sc_fxnum&              v);
  sc_int& operator = (const sc_fxnum_fast&         v);
  sc_int& operator = (const sc_bv<W>&              v);
  sc_int& operator = (const sc_lv<W>&              v);
  
  // Use sc_int_base's operator int64() const; Note that g++ does not
  // compile this correctly; if we define operator int64() here, g++
  // fails to make it mask the one defined in the base class.
  // operator int64() const { return num; }

  // Arithmetic operators
  sc_int& operator += (int_type v) 
    { 
      int_type val = num;
      val += v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator -= (int_type v) 
    {
      int_type val = num;
      val -= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator *= (int_type v) 
    {
      int_type val = num;
      val *= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator /= (int_type v) 
    {
      int_type val = num;
      val /= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator %= (int_type v) 
    {
      int_type val = num;
      val %= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  
  // Bitwise operators
  sc_int& operator &= (int_type v) 
    {
      int_type val = num;
      val &= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator |= (int_type v) 
    {
      int_type val = num;
      val |= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator ^= (int_type v) 
    {
      int_type val = num;
      val ^= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator <<= (int_type v) 
    {
      int_type val = num;
      val <<= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int& operator >>= (int_type v) 
    { num >>= v; return *this; }
  
  // Prefix and postfix increment and decrement operators
  sc_int& operator ++ () // prefix
    {
      int_type val = num;  
      ++val;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  const sc_int operator ++ (int) // postfix
    {
      int_type val = num;
      int_type tmp = val;
      val++;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return sc_int<W>(tmp);
    }
  sc_int& operator -- () //prefix
    {
      int_type val = num;
      --val;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); 
      return *this;
    }
  const sc_int operator -- (int) // postfix 
    {
      int_type val = num;
      int_type tmp = val;
      val--;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return sc_int<W>(tmp);
    }

  // Inherit bit-select and part-select operators from sc_int_base.
    
  // Concatenation operators are global functions that are friends of sc_int_base.
};  // sc_int 

// Class that implements bit-selection on sc_int

class sc_int_bitref {
  friend class sc_int_base;

public:
  int bitwidth() const { return 1; }

  sc_int_bitref& operator = (const sc_int_bitref& b);
  sc_int_bitref& operator = (bool b);
  sc_int_bitref& operator &= (bool b);
  sc_int_bitref& operator |= (bool b);
  sc_int_bitref& operator ^= (bool b);
  operator bool() const {return obj->test(index); }
  bool operator!() const {return ! obj->test(index); }
  // comma operators
  sc_int_concat<sc_int_bitref,sc_int_base> operator ,( sc_int_base& r );
  sc_int_concat<sc_int_bitref,sc_int_base> operator ,( const sc_int_base& r ) const;
  sc_int_concat<sc_int_bitref,sc_int_bitref> operator,(sc_int_bitref& r );
  sc_int_concat<sc_int_bitref,sc_int_bitref> operator,(const sc_int_bitref& r ) const;
  sc_int_concat<sc_int_bitref,sc_int_subref> operator,( sc_int_subref& r );
  sc_int_concat<sc_int_bitref,sc_int_subref> operator,( const sc_int_subref& r ) const;

#if !defined(__SUNPRO_CC)

  template<class T1, class T2>
  sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> > operator,( sc_int_concat<T1,T2>& r )
  {
    return sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >( *this, r );
  }

  template<class T1, class T2 >
  sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> > operator,(const sc_int_concat<T1,T2>& r ) const
  {
    return sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >( const_cast<sc_int_bitref&>(*this),
    const_cast<sc_int_concat<T1,T2>&>(r) );
  }
#endif

private:
  sc_int_base* obj;
  int index;
  
  sc_int_bitref(sc_int_base* u, int li) { obj = u; index = li; }
};


// Class that implements part-selection on sc_int

class sc_int_subref {
  friend class sc_int_base;
  
public:
  int bitwidth() const { return (left - right + 1); }

  // Implicit conversion to ...
  operator int_type() const;

  // Assignment operators
  sc_int_subref& operator = (const sc_int_base& v);
  sc_int_subref& operator = (const sc_int_subref& v);
  sc_int_subref& operator = (int_type v);
  // comma operators
  sc_int_concat<sc_int_subref,sc_int_base> operator,(sc_int_base& r );
  sc_int_concat<sc_int_subref,sc_int_base> operator,(const sc_int_base& r ) const;
  sc_int_concat<sc_int_subref,sc_int_bitref> operator,(sc_int_bitref& r );
  sc_int_concat<sc_int_subref,sc_int_bitref> operator,(const sc_int_bitref& r ) const;
  sc_int_concat<sc_int_subref,sc_int_subref> operator,(sc_int_subref& r );
  sc_int_concat<sc_int_subref,sc_int_subref> operator,(const sc_int_subref& r ) const;

  #if !defined(__SUNPRO_CC)
  template<class T1, class T2>
  sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> > operator,(sc_int_concat<T1,T2>& r )
  {
    return sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >( *this, r );
  }

  template<class T1, class T2>
  sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> > operator,( const sc_int_concat<T1, T2>& r )const
  {
    return sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >( const_cast<sc_int_subref&>(*this), const_cast<sc_int_concat<T1,T2>&>(r) );
  }
  #endif
private:
  sc_int_base* obj;
  int left;
  int right;

  sc_int_subref(sc_int_base* u, int l, int r) { obj = u; left = l; right = r; }
};

// Proxy class to implement concatenation

template<class T1, class T2>
class sc_int_concat {
public:
  // Constructors
  sc_int_concat(T1& l, T2& r) : left(l), right(r) 
    {
      len = l.bitwidth() + r.bitwidth();
    }
  sc_int_concat(const sc_int_concat<T1,T2>& v) : left(v.left), right(v.right), len(v.len)
    { }

  int bitwidth() const { return len; }

  // Implicit conversion to ...
  operator int_type() const 
    {   
      int_type mask = ( ~const_zero_uint >> ( SC_INTWIDTH - right.bitwidth()) );
      int_type high = (left << right.bitwidth()) & (~mask);
      int_type low  = right & mask;
      int_type tmp = high | low;
      return (tmp << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len));
    }

  // Assignment operators

  // Sun's SC5.0 doesn't support member templates
#if !defined(__SUNPRO_CC)
  template<class T3, class T4>
  sc_int_concat<T1,T2>& operator = (const sc_int_concat<T3,T4>& v) // lvalue use
    { 
      return operator=(v.operator int_type()); 
    }
#else
  //  operator const sc_int_base() { return sc_int_base(operator int_type()); }
#endif
  sc_int_concat& operator = (int_type v)
    {
      int_type mask = ( ~const_zero_uint >> ( SC_INTWIDTH - right.bitwidth()) );
      right = v & mask;
      left = (v & ~mask) >> right.bitwidth();
      return *this;
    }
  sc_int_concat& operator=(const sc_int_base& v)
    { return operator=(v.operator int_type()); }
  sc_int_concat& operator=(const sc_int_subref& v)
    { return operator=(v.operator int_type()); }

  // comma operators
#if defined(__BCPLUSPLUS__)
// BCB5 has a bug that prevents moving definitions outside of the class
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> operator,(sc_int_base& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>( *this, r );
}
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> operator,(const sc_int_base& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>(
  const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_base&>(r) );
}
sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref> operator,(sc_int_bitref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>( *this, r );
}
sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>  operator,(const sc_int_bitref& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>(
   const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_bitref&>(r) );
}
sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> operator,(sc_int_subref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>( *this, r );
}
sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> operator,(const sc_int_subref& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>(
  const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_subref&>(r) );
}
#else
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> operator,(sc_int_base& r );
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> operator,(const sc_int_base& r )const;
sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref> operator,(sc_int_bitref& r );
sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>  operator,(const sc_int_bitref& r )const;
sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> operator,(sc_int_subref& r );
sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> operator,(const sc_int_subref& r )const;
#endif

#if !defined(__SUNPRO_CC)
template<class T3, class T4>
inline sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> > operator,(sc_int_concat<T3,T4>& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >( *this, r );
}

template<class T3, class T4>
inline sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> > operator,(const sc_int_concat<T3,T4>& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >(
  const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_concat<T3,T4>&>(r) );
}
#endif
private:
  T1& left;
  T2& right;
  int len;
};




//#include "sc_int_defs.h"

extern void check_bitwidth( int w );
extern void check_index( int w );
extern void check_range( int left, int right );
extern void check_signed_value( int w, int_type v );

#if defined(_MSC_VER)
ostream& operator << (ostream& os, int_type);
#endif

// implementation
template<int W>
inline
sc_int<W>::sc_int(int_type v) : sc_int_base(W) 
{ 
#ifdef DEBUG_SYSTEMC
  check_signed_value(W, v);
#endif
  num = v << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); 
}

template<int W>  
sc_int<W>::sc_int(const sc_bv<W>& v)
{
  width = W;
  int S = SC_INTWIDTH - width;
  for (int i = width - 1; i >= 0; i--) {
    set(i, v[i]);
  }
  num = num << S >> S;
}

template<int W>  
sc_int<W>::sc_int(const sc_lv<W>& v)
{
  width = W;
  int S = SC_INTWIDTH - width;
  for (int i = width - 1; i >= 0; i--) {
    if(v[i] > 1)
    {
      REPORT_WARNING(2007,"sc_int::sc_int(const sc_lv<W>& v) - construction aborted");
      break;
    }
    else
      set(i, v[i]);
  }
  num = num << S >> S;
}

#if defined(__BCPLUSPLUS__)
#pragma warn -8027
#endif

template<int W>
inline 
sc_int<W>& 
sc_int<W>::operator=(int_type v)
{
#ifdef DEBUG_SYSTEMC
  check_signed_value(W, v);
#endif
  num = v << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); 
  return *this;
}

template<int W>
inline 
sc_int<W>& 
sc_int<W>::operator=(const sc_bv<W>& v)
{
  int S = SC_INTWIDTH - width;
  for (int i = W-1; i >= 0; i--) {
    set(i, v[i]);
  }
  num = num << S >> S;
  return *this;
}

template<int W>
inline 
sc_int<W>& 
sc_int<W>::operator=(const sc_lv<W>& v)
{
  int S = SC_INTWIDTH - width;
  for (int i = W - 1; i >= 0; i--) {
    if(v[i] > 1)
    {
      REPORT_WARNING(2007,"sc_int::operator=(const sc_lv<W>& v) - operation ignored");
      break;
    }
    else
      set(i, v[i]);
  }
  num = num << S >> S;
  return *this;
}

#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif

// implementation of comma operators

// sc_int_base class

inline  sc_int_concat<sc_int_base,sc_int_base>
sc_int_base::operator ,(sc_int_base& r)
{
  return sc_int_concat<sc_int_base,sc_int_base>(*this,r );
}

inline  sc_int_concat<sc_int_base,sc_int_base>
sc_int_base::operator ,( const sc_int_base& r )const
{
  return sc_int_concat<sc_int_base,sc_int_base>( const_cast<sc_int_base&>(*this),
  const_cast<sc_int_base&>(r) );
}

inline  sc_int_concat<sc_int_base,sc_int_bitref>
sc_int_base::operator ,(sc_int_bitref& r)
{
  return sc_int_concat<sc_int_base,sc_int_bitref>( *this, r );
}

inline  sc_int_concat<sc_int_base,sc_int_bitref>
sc_int_base::operator ,( const sc_int_bitref& r )const
{
  return sc_int_concat<sc_int_base,sc_int_bitref>( const_cast<sc_int_base&>(*this),
  const_cast<sc_int_bitref&>(r) );
}

inline  sc_int_concat<sc_int_base,sc_int_subref>
sc_int_base::operator ,(sc_int_subref& r)
{
  return sc_int_concat<sc_int_base,sc_int_subref>( *this, r );
}

inline  sc_int_concat<sc_int_base,sc_int_subref>
sc_int_base::operator ,(const sc_int_subref& r) const
{
  return sc_int_concat<sc_int_base,sc_int_subref>( const_cast<sc_int_base&>(*this),
  const_cast<sc_int_subref&>(r) );
}

// sc_int_bitref class

inline  sc_int_concat<sc_int_bitref,sc_int_base> sc_int_bitref::operator ,( sc_int_base& r )
  { return sc_int_concat<sc_int_bitref,sc_int_base>( *this, r ); }

inline  sc_int_concat<sc_int_bitref,sc_int_base> sc_int_bitref::operator ,( const sc_int_base& r ) const
  {
    return sc_int_concat<sc_int_bitref,sc_int_base>( const_cast<sc_int_bitref&>(*this),
    const_cast<sc_int_base&>(r) );
  }

inline  sc_int_concat<sc_int_bitref,sc_int_bitref> sc_int_bitref::operator,(sc_int_bitref& r )
  { return sc_int_concat<sc_int_bitref,sc_int_bitref>(*this, r); }

inline  sc_int_concat<sc_int_bitref,sc_int_bitref> sc_int_bitref::operator,(const sc_int_bitref& r ) const
  {
    return sc_int_concat<sc_int_bitref,sc_int_bitref>( const_cast<sc_int_bitref&>(*this),
    const_cast<sc_int_bitref&>(r) );
  }

inline  sc_int_concat<sc_int_bitref,sc_int_subref> sc_int_bitref::operator,( sc_int_subref& r )
  { return sc_int_concat<sc_int_bitref,sc_int_subref>( *this, r ); }

inline  sc_int_concat<sc_int_bitref,sc_int_subref> sc_int_bitref::operator,( const sc_int_subref& r ) const
  {
    return sc_int_concat<sc_int_bitref,sc_int_subref>( const_cast<sc_int_bitref&>(*this),
    const_cast<sc_int_subref&>(r) );
  }
// sc_int_subref class
inline  sc_int_concat<sc_int_subref,sc_int_base> sc_int_subref::operator,(sc_int_base& r )
  {
    return sc_int_concat<sc_int_subref,sc_int_base>( *this, r );
  }

inline  sc_int_concat<sc_int_subref,sc_int_base> sc_int_subref::operator,(const sc_int_base& r ) const
  {
    return sc_int_concat<sc_int_subref,sc_int_base>( const_cast<sc_int_subref&>(*this), const_cast<sc_int_base&>(r) );
  }

inline  sc_int_concat<sc_int_subref,sc_int_bitref> sc_int_subref::operator,(sc_int_bitref& r )
  {
    return sc_int_concat<sc_int_subref,sc_int_bitref>( *this, r );
  }

inline  sc_int_concat<sc_int_subref,sc_int_bitref> sc_int_subref::operator,(const sc_int_bitref& r ) const
  {
    return sc_int_concat<sc_int_subref,sc_int_bitref>( const_cast<sc_int_subref&>(*this), const_cast<sc_int_bitref&>(r) );
  }

inline  sc_int_concat<sc_int_subref,sc_int_subref> sc_int_subref::operator,(sc_int_subref& r )
  {
    return sc_int_concat<sc_int_subref,sc_int_subref>( *this, r );
  }

inline  sc_int_concat<sc_int_subref,sc_int_subref> sc_int_subref::operator,(const sc_int_subref& r ) const
  {
    return sc_int_concat<sc_int_subref,sc_int_subref>( const_cast<sc_int_subref&>(*this), const_cast<sc_int_subref&>(r) );
  }

// sc_int_concat template

#if !defined(__BCPLUSPLUS__)
template<class T1, class T2>
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> sc_int_concat<T1,T2>::operator,(sc_int_base& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>( *this, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_base> sc_int_concat<T1,T2>::operator,(const sc_int_base& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>(
  const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_base&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref> sc_int_concat<T1,T2>::operator,(sc_int_bitref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>( *this, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>  sc_int_concat<T1,T2>::operator,(const sc_int_bitref& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>(
   const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_bitref&>(r) );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> sc_int_concat<T1,T2>::operator,(sc_int_subref& r )
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>( *this, r );
}

template<class T1, class T2>
inline sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref> sc_int_concat<T1,T2>::operator,(const sc_int_subref& r )const
{
  return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>(
  const_cast<sc_int_concat<T1,T2>&>(*this), const_cast<sc_int_subref&>(r) );
}
#endif


#if defined(__SUNPRO_CC)
// cannot make it a member template because SC5 doesn't implement them
template<class T1, class T2>
inline
sc_int_concat<sc_int_base, sc_int_concat<T1,T2> >
operator ,(sc_int_base& l, sc_int_concat<T1,T2>& r)
{
  return sc_int_concat<sc_int_base, sc_int_concat<T1,T2> >(l, r);
}

template<class T1, class T2>
inline
sc_int_concat<sc_int_base, sc_int_concat<T1,T2> >
operator ,(const sc_int_base& l, const sc_int_concat<T1, T2>& r)
{
  return sc_int_concat<sc_int_base, sc_int_concat<T1,T2> >(const_cast<sc_int_base&>(l),
                                                           const_cast<sc_int_concat<T1,T2>&>(r));
}

template<class T1, class T2>
inline
sc_int_concat<sc_int_bitref, sc_int_concat<T1,T2> >
operator,(sc_int_bitref& l, sc_int_concat<T1,T2>& r)
{
  return sc_int_concat<sc_int_bitref, sc_int_concat<T1,T2> >(l, r);
}

template<class T1, class T2 >
inline
sc_int_concat<sc_int_bitref, sc_int_concat<T1,T2> >
operator,(const sc_int_bitref& l, const sc_int_concat<T1,T2>& r)
{
  return sc_int_concat<sc_int_bitref, sc_int_concat<T1,T2> >(const_cast<sc_int_bitref&>(l),
                                                             const_cast<sc_int_concat<T1,T2>&>(r));
}

template<class T1, class T2>
inline
sc_int_concat<sc_int_subref, sc_int_concat<T1,T2> >
operator,(sc_int_subref& l, sc_int_concat<T1,T2>& r)
{
  return sc_int_concat<sc_int_subref, sc_int_concat<T1,T2> >(l, r);
}

template<class T1, class T2>
inline
sc_int_concat<sc_int_subref, sc_int_concat<T1,T2> >
operator,(const sc_int_subref& l, const sc_int_concat<T1, T2>& r)
{
  return sc_int_concat<sc_int_subref, sc_int_concat<T1,T2> >(const_cast<sc_int_subref&>(l),
                                                             const_cast<sc_int_concat<T1,T2>&>(r));
}

template<class T1,class T2, class T3, class T4>
inline
sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >
operator,(sc_int_concat<T1,T2>& l, sc_int_concat<T3,T4>& r)
{
  return sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >(l, r);
}

template<class T1,class T2, class T3, class T4>
inline
sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >
operator,(const sc_int_concat<T1,T2>& l, const sc_int_concat<T3,T4>& r)
{
  return sc_int_concat<sc_int_concat<T1,T2>, sc_int_concat<T3,T4> >(const_cast<sc_int_concat<T1,T2>&>(l),
                                                                    const_cast<sc_int_concat<T3,T4>&>(r));
}
#endif //SUN

#endif
