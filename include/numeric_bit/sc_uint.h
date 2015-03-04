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

    sc_uint.h -- A sc_int is an unsigned integer whose width is less
    than the machine's native integer width. We provide two
    implementations (i) sc_uint with bitwidth between 1 - 64 (ii)
    sc_uint with bitwidth between 1 - 32 Implementation (i) is the
    default implementation, while implementation (ii) can be used only
    if compiled with -D_32BIT_.  Unlike arbitrary precision,
    arithmetic and bitwise operations are performed using the native
    types (hence capped at 32/64 bits). The sc_uint integer is useful
    when the user does not need arbitrary precision and the
    performance is superior to sc_bigint/sc_biguint.

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


#ifndef SC_UINT_H
#define SC_UINT_H

#include "sc_nbdefs.h"
#include "../sc_exception.h"

#define MASK(W) ((~ const_zero_uint) >> (SC_INTWIDTH - W))

// Forward declarations
class sc_uint_bitref;
class sc_uint_subref;
class sc_uint_base;
class sc_bool_vector;
class sc_logic_vector;
class sc_signed;
class sc_unsigned;
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;

template<class T1, class T2> class sc_uint_concat;
template<int W> class sc_uint;
namespace sc_bv_ns
{
  template<int W> class sc_bv;
  template<int W> class sc_lv;
}
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_lv;

extern const uint_type mask_int[SC_INTWIDTH][SC_INTWIDTH];

// Base class for sc_uint. 

class sc_uint_base {
  
  friend class sc_uint_bitref;
  friend class sc_uint_subref;
  
public:
  /* Constructors */
  sc_uint_base() : num(0), width(SC_INTWIDTH) 
    { } 
  explicit sc_uint_base(int w);

  sc_uint_base(uint_type v, int w);

  sc_uint_base(const sc_uint_base&          v) 
    { width = v.width; num = v.num; }
  sc_uint_base(const sc_uint_subref&        v);
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> 
  sc_uint_base(const sc_uint_concat<T1,T2>& v)
    {
      width = v.bitwidth();
      num = (v.operator uint_type()) & MASK(width);
    }
#endif
  sc_uint_base(const sc_bool_vector&        v);
  sc_uint_base(const sc_logic_vector&       v);
  sc_uint_base(const sc_signed&             v);
  sc_uint_base(const sc_unsigned&           v);

  // Destructor
  ~sc_uint_base() { }

  // Implicit conversion to ...
  operator uint_type() const { return num; }
  
  int bitwidth() const { return width; }
  int length() const { return width; }
  
  unsigned int to_uint() const { return (unsigned int) num; }
  int to_int() const {return (int) num; }
  uint64 to_uint64() const { return num; }
  int64 to_int64() const { return num; }
#ifndef _32BIT_
  long long_low() const 
    { return (long) (num & const_32ones_ull); }
  long long_high() const 
    { return (long) ((num >> 32) & const_32ones_ull); }
#endif
  
  bool test(int i) const 
    { return (0 != (num & (const_one_uint << i))); }
  void set(int i) 
    { num |= (const_one_uint << i); }
  void set(int i, bool v)
    {
      //uint_type val = num;
      //val &= ~(const_one_uint << i);
      //val |= ((v & const_one_uint) << i);
      //num = val;
	  v? num |= uint_type(1)<<i : num &= ~(uint_type(1)<<i);
    }

  // Bit-select and part-select operators
  
  sc_uint_bitref operator [] (int i); 
  bool operator [] (int i) const 
    { return test(i); }
  sc_uint_subref range(int left, int right); 
  uint_type range(int left, int right) const;

  // Assignment operators
  sc_uint_base& operator = (uint_type                    v);

  sc_uint_base& operator = (const sc_uint_subref&        v); 
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> 
  sc_uint_base& operator = (const sc_uint_concat<T1,T2>& v)
    {
      num = (v.operator uint_type()) & MASK(width);
      return *this;
    }
#endif
  sc_uint_base& operator = (const sc_bool_vector&        v);
  sc_uint_base& operator = (const sc_logic_vector&       v);
  sc_uint_base& operator = (const sc_signed&             v);
  sc_uint_base& operator = (const sc_unsigned&           v);
  sc_uint_base& operator = (const sc_fxval&              v);
  sc_uint_base& operator = (const sc_fxval_fast&         v);
  sc_uint_base& operator = (const sc_fxnum&              v);
  sc_uint_base& operator = (const sc_fxnum_fast&         v);

  // Arithmetic operators
  sc_uint_base& operator += (uint_type v);
  sc_uint_base& operator -= (uint_type v);
  sc_uint_base& operator *= (uint_type v);
  sc_uint_base& operator /= (uint_type v);
  sc_uint_base& operator %= (uint_type v);

  // Bitwise operators
  sc_uint_base& operator &= (uint_type v);
  sc_uint_base& operator |= (uint_type v);
  sc_uint_base& operator ^= (uint_type v);
  sc_uint_base& operator <<= (uint_type v);
  sc_uint_base& operator >>= (uint_type v);

  // Prefix and postfix increment and decrement operators
  sc_uint_base& operator ++ (); // prefix
  const sc_uint_base operator ++ (int); // postfix
  sc_uint_base& operator -- (); // prefix
  const sc_uint_base operator -- (int); // postfix

  friend bool operator == (const sc_uint_base& u, const sc_uint_base& v)
    { return u.num == v.num; }
  friend bool operator != (const sc_uint_base& u, const sc_uint_base& v)
    { return u.num != v.num; }
  friend bool operator <  (const sc_uint_base& u, const sc_uint_base& v)
    { return u.num < v.num; }
  friend bool operator <= (const sc_uint_base& u, const sc_uint_base& v)
    { return u.num <= v.num; }
  friend bool operator >  (const sc_uint_base& u, const sc_uint_base& v)
    { return u.num > v.num; }
  friend bool operator >= (const sc_uint_base& u, const sc_uint_base& v)
    { return u.num >= v.num; }
  // comma operators
  sc_uint_concat<sc_uint_base,sc_uint_base> operator ,(sc_uint_base& r);
  sc_uint_concat<sc_uint_base,sc_uint_base> operator ,(const sc_uint_base& r )const;
  sc_uint_concat<sc_uint_base,sc_uint_bitref> operator ,( sc_uint_bitref& r);
  sc_uint_concat<sc_uint_base,sc_uint_bitref> operator ,(const sc_uint_bitref& r )const;
  sc_uint_concat<sc_uint_base,sc_uint_subref> operator ,( sc_uint_subref& r);
  sc_uint_concat<sc_uint_base,sc_uint_subref> operator ,(const sc_uint_subref& r)const;

  #if !defined(__SUNPRO_CC)
  template<class T1, class T2>
  sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> > operator ,(sc_uint_concat<T1,T2>& r )
  { return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >( *this, r ); }

  template<class T1, class T2>
  sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> > operator ,(const sc_uint_concat<T1, T2>& r )const
  {
    return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >(
    const_cast<sc_uint_base&>(*this), const_cast<sc_uint_concat<T1,T2>&>(r) );
  }
  #endif

public: 
  // These can be protected at the expense of adding a inlined member
  // function to get 'num' in the derived class.
  uint_type num;
  int width;
};


// Template class sc_uint<W> is the interface that the user sees. It
// is derived from sc_uint_base and most of its methods are just
// wrappers that call the corresponding method in the parent
// class. Note that the width of sc_uint datatype is specified as a
// template parameter.

template< int W = SC_INTWIDTH>
class sc_uint : public sc_uint_base {
  friend class sc_uint_bitref;
  friend class sc_uint_subref;

public:
  // Constructors
  sc_uint() : sc_uint_base(W) 
    { }
  sc_uint(uint_type                     v);
  sc_uint(const sc_uint<W>&             v) : sc_uint_base(v) 
    { }
  sc_uint(const sc_uint_base&           v) : sc_uint_base(v) 
    { }
  sc_uint(const sc_uint_subref&         v) : sc_uint_base(v) 
    { }
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> 
  sc_uint(const sc_uint_concat<T1,T2>&  v) : sc_uint_base(v) 
    { }
#endif
  sc_uint(const sc_bool_vector&         v) : sc_uint_base(v) 
    { }
  sc_uint(const sc_logic_vector&        v) : sc_uint_base(v) 
    { }
  sc_uint(const sc_signed&              v) : sc_uint_base(v) 
    { }
  sc_uint(const sc_unsigned&            v) : sc_uint_base(v) 
    { }
  explicit sc_uint(const sc_fxval&      v);
  explicit sc_uint(const sc_fxval_fast& v);
  explicit sc_uint(const sc_fxnum&      v);
  explicit sc_uint(const sc_fxnum_fast& v);
  sc_uint(const sc_bv<W>&               v);
  sc_uint(const sc_lv<W>&               v);

  // Assignment operators
  sc_uint& operator = (uint_type                    v);
  sc_uint& operator = (const sc_uint_base&          v)
    {
      num = v.num & MASK(W);
      return *this;
    }
  sc_uint& operator = (const sc_uint_subref&        v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint& operator = (const sc_uint<W>&            v) 
    { num = v.num; return *this; }
#if !defined(__SUNPRO_CC)
  template<class T1, class T2> 
  sc_uint& operator = (const sc_uint_concat<T1,T2>& v) 
    { sc_uint_base::operator=(v); return *this; }
#endif
  sc_uint& operator = (const sc_bool_vector&        v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint& operator = (const sc_logic_vector&       v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint& operator = (const sc_signed&             v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint& operator = (const sc_unsigned&           v) 
    { sc_uint_base::operator=(v); return *this;}
  sc_uint& operator = (const sc_fxval&              v);
  sc_uint& operator = (const sc_fxval_fast&         v);
  sc_uint& operator = (const sc_fxnum&              v);
  sc_uint& operator = (const sc_fxnum_fast&         v);
  sc_uint& operator = (const sc_bv<W>&              v);
  sc_uint& operator = (const sc_lv<W>&              v);

  // Arithmetic operators
  sc_uint& operator += (uint_type v) 
    {
      uint_type val = num;
      val += v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator -= (uint_type v) 
    {
      uint_type val = num;
      val -= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator *= (uint_type v) 
    {
      uint_type val = num;
      val *= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator /= (uint_type v) 
    {
      uint_type val = num;
      val /= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator %= (uint_type v) 
    {
      uint_type val = num;
      val %= v;
      num = val & MASK(W);
      return *this;
    }
  
  // Bitwise operators
  sc_uint& operator &= (uint_type v) 
    {
      uint_type val = num;
      val &= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator |= (uint_type v) 
    {
      uint_type val = num;
      val |= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator ^= (uint_type v) 
    {
      uint_type val = num;
      val ^= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator <<= (uint_type v) 
    {
      uint_type val = num;
      val <<= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint& operator >>= (uint_type v) 
    { num >>= v; return *this; }
  
  // Prefix and postfix increment and decrement operators
  sc_uint& operator ++ () // prefix
    {
      uint_type val = num;
      ++val;
      val &= MASK(W); 
      num = val;
      return *this;
    }
  const sc_uint operator ++ (int) // postfix
    {
      uint_type val = num;   
      uint_type tmp = val;
      val++;
      val &= MASK(W); 
      num = val;
      return sc_uint<W>(tmp);
    }
  sc_uint& operator -- () // prefix
    {
      uint_type val = num;  
      --val;
      val &= MASK(W);
      num = val;
      return *this;
    }
  const sc_uint operator -- (int) // postfix 
    {
      uint_type val = num;
      uint_type tmp = val;
      val--;
      val &= MASK(W);
      num = val;
      return sc_uint<W>(tmp);
    }     

  // Inherit bit-select and part-select operators from sc_uint_base.
    
  // Conatenation operators are global functions that are friends of sc_uint_base.
};  // sc_uint

// Class that implements bit-selection on sc_uint

class sc_uint_bitref {
  friend class sc_uint_base;

public:
  int bitwidth() const { return 1; }

  sc_uint_bitref& operator = (const sc_uint_bitref& b)
    {
      obj->set(index, (b.obj)->test(b.index));
      return *this;
    }
  sc_uint_bitref& operator = (bool b)
    {
      obj->set(index, b);
      return *this;
    }
  sc_uint_bitref& operator &= (bool b)
    {
      obj->num &= ~ ((!b) << index);
      return *this;
    }
  sc_uint_bitref& operator |= (bool b)
    {
      //obj->num |= ((b & const_one_uint) << index);
	  if(b) obj->num |= uint_type(1) << index;
      return *this;
    }
  sc_uint_bitref& operator ^= (bool b)
    {
      //obj->num ^= ((b & const_one_uint) << index);
      obj->num ^= uint_type(b) << index;
      return *this;
    }

  operator bool() const {return obj->test(index); }
  bool operator!() const {return ! obj->test(index); }

  // comma operators
  sc_uint_concat<sc_uint_bitref,sc_uint_base> operator ,( sc_uint_base& r );
  sc_uint_concat<sc_uint_bitref,sc_uint_base> operator ,( const sc_uint_base& r )const;
  sc_uint_concat<sc_uint_bitref,sc_uint_bitref> operator,(sc_uint_bitref& r );
  sc_uint_concat<sc_uint_bitref,sc_uint_bitref> operator,(const sc_uint_bitref& r )const;
  sc_uint_concat<sc_uint_bitref,sc_uint_subref> operator,( sc_uint_subref& r );
  sc_uint_concat<sc_uint_bitref,sc_uint_subref> operator,(const sc_uint_subref& r )const;

  #if !defined(__SUNPRO_CC)
  template<class T1, class T2>
  sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> > operator,( sc_uint_concat<T1,T2>& r )
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >( *this, r );
  }

  template<class T1, class T2 >
  sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> > operator,( const sc_uint_concat<T1,T2>& r )
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >(
    const_cast<sc_uint_bitref&>(*this), const_cast<sc_uint_concat<T1,T2>&>(r) );
  }
  #endif
private:
  sc_uint_base* obj;
  int index;

  sc_uint_bitref(sc_uint_base* u, int li) { obj = u; index = li; }
};

// gcc 2.95.2 bug - bool comparison mapped into int comparison must
// provide explicit conversion, otherwise gcc is confused inline
// bool operator==(bool b,sc_uint_bitref& t){ return t==b; }

// Class that implements part-selection on sc_uint

class sc_uint_subref {
  friend class sc_uint_base;
  
public:
  int bitwidth() const { return (left - right + 1); }

  // Implicit conversion to ...
  operator uint_type() const;

  // Assignment operators
  sc_uint_subref& operator = (const sc_uint_base& v);
  sc_uint_subref& operator = (const sc_uint_subref& v);
  sc_uint_subref& operator =(uint_type v);
  // comma operators
  sc_uint_concat<sc_uint_subref,sc_uint_base> operator,(sc_uint_base& r );
  sc_uint_concat<sc_uint_subref,sc_uint_base> operator,(const sc_uint_base& r ) const;
  sc_uint_concat<sc_uint_subref,sc_uint_bitref> operator,(sc_uint_bitref& r );
  sc_uint_concat<sc_uint_subref,sc_uint_bitref> operator,(const sc_uint_bitref& r )const;
  sc_uint_concat<sc_uint_subref,sc_uint_subref> operator,(sc_uint_subref& r );
  sc_uint_concat<sc_uint_subref,sc_uint_subref> operator,( const sc_uint_subref& r )const;

  #if !defined(__SUNPRO_CC)
  template<class T1, class T2>
  sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> > operator,(sc_uint_concat<T1,T2>& r )
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >( *this, r );
  }

  template<class T1, class T2>
  sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> > operator,(const sc_uint_concat<T1, T2>& r )const
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >(
    const_cast<sc_uint_subref&>(*this), const_cast<sc_uint_concat<T1,T2>&>(r) );
  }
  #endif
private:
  sc_uint_base* obj;
  int left;
  int right;
  
  sc_uint_subref(sc_uint_base* u, int l, int r) { obj = u; left = l; right = r; }
};

// Proxy class to implement concatenation

template<class T1, class T2>
class sc_uint_concat {
public:
  // Constructors
  sc_uint_concat(T1& l, T2& r) : left(l), right(r) 
    {
      len = l.bitwidth() + r.bitwidth();
    }
  sc_uint_concat<T1,T2>(const sc_uint_concat<T1,T2>& v) : left(v.left), right(v.right), len(v.len)
    { }

  int bitwidth() const { return len; }

  // Implicit conversion to ...
  operator uint_type() const 
    {   
      uint_type mask = ( ~const_zero_uint >> ( SC_INTWIDTH - right.bitwidth()) );
      uint_type high = (left << right.bitwidth()) & (~mask);
      uint_type low  = right & mask;
      uint_type tmp = high | low;
      return (tmp << (SC_INTWIDTH - len) >> (SC_INTWIDTH - len));
    }

  // Assignment operators
#if !defined(__SUNPRO_CC)
  template<class T3, class T4>
  sc_uint_concat<T1,T2>& operator = (const sc_uint_concat<T3,T4>& v) // lvalue use
    { 
      return operator=(v.operator uint_type()); 
    }
#endif
  sc_uint_concat& operator = (uint_type v)
    {
      uint_type mask = ( ~const_zero_uint >> ( SC_INTWIDTH - right.bitwidth()) );
      right = v & mask;
      left = (v & ~mask) >> right.bitwidth();
      return *this;
    }
  sc_uint_concat& operator = (const sc_uint_base& v)
    {
      return operator=(v.operator uint_type());
    }
  sc_uint_concat& operator = (const sc_uint_subref& v)
    {
      return operator=(v.operator uint_type());
    }
  // comma operators
#if defined(__BCPLUSPLUS__)
// bug in BCB5 doesn't allow to define these operators outside the class
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> operator,(sc_uint_base& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>( *this, r );
  }
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> operator,(const sc_uint_base& r )const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>(
    const_cast<sc_uint_concat<T1,T2>&>(*this), const_cast<sc_uint_base&>(r) );
  }
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref> operator,( sc_uint_bitref& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>( *this, r );
  }
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>  operator,( const sc_uint_bitref& r )const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>(
    const_cast<sc_uint_concat<T1,T2>&>(*this), const_cast<sc_uint_bitref&>(r) );
  }
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> operator,( sc_uint_subref& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>( *this, r );
  }
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> operator,( const sc_uint_subref& r )const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>(
    const_cast<sc_uint_concat<T1,T2>&>(*this), const_cast<sc_uint_subref&>(r) );
  }
#else
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> operator,(sc_uint_base& r );
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> operator,(const sc_uint_base& r )const;
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref> operator,( sc_uint_bitref& r );
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>  operator,( const sc_uint_bitref& r )const;
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> operator,( sc_uint_subref& r );
  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> operator,( const sc_uint_subref& r )const;
#endif

  #if !defined(__SUNPRO_CC)
  template<class T3, class T4>
  sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >
  operator,( sc_uint_concat<T3,T4>& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >( *this, r );
  }

  template<class T3, class T4>
  sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >
  operator,( const sc_uint_concat<T3,T4>& r ) const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >(
    const_cast<sc_uint_concat<T1,T2>&>(*this),  const_cast<sc_uint_concat<T3,T4>&>(r) );
  }
  #endif //SUN

private:
  T1& left;
  T2& right;
  int len;
};

//#include "sc_uint_defs.h"

extern void check_bitwidth( int w );
extern void check_index( int w );
extern void check_range( int left, int right );
extern void check_unsigned_value( int w, uint_type v );

#if defined(_MSC_VER)
ostream& operator << (ostream& os, uint_type);
#endif

// implementation
template<int W>  
inline
sc_uint<W>::sc_uint(uint_type v) : sc_uint_base(W)
{
#ifdef DEBUG_SYSTEMC
  check_unsigned_value(W, v);
#endif
  num = v & MASK(W);
}

template<int W>  
sc_uint<W>::sc_uint(const sc_bv<W>& v)
{
  width = W;
  int S = SC_INTWIDTH - width;
  for (int i = width - 1; i >= 0; i--) {
    set(i, v[i]);
  }
  num = num << S >> S;
}

template<int W>  
sc_uint<W>::sc_uint(const sc_lv<W>& v)
{
  width = W;
  int S = SC_INTWIDTH - width;
  for (int i = width - 1; i >= 0; i--) {
    if(v[i] > 1)
    {
      REPORT_WARNING(2007,"sc_uint::sc_uint(const sc_lv<W>& v) - construction aborted");
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
sc_uint<W>& 
sc_uint<W>::operator=(uint_type v)
{
#ifdef DEBUG_SYSTEMC
  check_unsigned_value(W, v);
#endif
  num = v & MASK(W);
  return *this;
}

template<int W>
inline 
sc_uint<W>& 
sc_uint<W>::operator=(const sc_bv<W>& v)
{
  int S = SC_INTWIDTH - width;
  for (int i = W - 1 ; i >= 0; i--) {
    set(i, v[i]);
  }
  num = num << S >> S;
  return *this;
}

template<int W>
inline
sc_uint<W>& 
sc_uint<W>::operator=(const sc_lv<W>& v)
{
  int S = SC_INTWIDTH - width;
  for (int i = W - 1 ; i >= 0; i--) {
    if(v[i] > 1)
    {
      REPORT_WARNING(2007,"sc_uint::operator=(const sc_lv<W>& v) - assignment ignored");
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

  // comma operators sc_uint_base class
inline sc_uint_concat<sc_uint_base,sc_uint_base> sc_uint_base::operator ,(sc_uint_base& r)
  { return sc_uint_concat<sc_uint_base,sc_uint_base>( *this , r ); }

inline  sc_uint_concat<sc_uint_base,sc_uint_base> sc_uint_base::operator ,(const sc_uint_base& r )const
  {
    return sc_uint_concat<sc_uint_base,sc_uint_base>(
    const_cast<sc_uint_base&>(*this), const_cast<sc_uint_base&>(r) );
  }

inline  sc_uint_concat<sc_uint_base,sc_uint_bitref> sc_uint_base::operator ,( sc_uint_bitref& r)
  { return sc_uint_concat<sc_uint_base,sc_uint_bitref>( *this, r ); }

inline  sc_uint_concat<sc_uint_base,sc_uint_bitref> sc_uint_base::operator ,(const sc_uint_bitref& r )const
  {
    return sc_uint_concat<sc_uint_base,sc_uint_bitref>(
    const_cast<sc_uint_base&>(*this), const_cast<sc_uint_bitref&>(r) );
  }

inline  sc_uint_concat<sc_uint_base,sc_uint_subref> sc_uint_base::operator ,( sc_uint_subref& r)
  { return sc_uint_concat<sc_uint_base,sc_uint_subref>( *this, r );  }

inline  sc_uint_concat<sc_uint_base,sc_uint_subref> sc_uint_base::operator ,(const sc_uint_subref& r)const
  {
    return sc_uint_concat<sc_uint_base,sc_uint_subref>(
    const_cast<sc_uint_base&>(*this), const_cast<sc_uint_subref&>(r) );
  }

  // comma operators sc_uint_subref class
inline  sc_uint_concat<sc_uint_subref,sc_uint_base> sc_uint_subref::operator,(sc_uint_base& r )
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_base>( *this, r );
  }

inline  sc_uint_concat<sc_uint_subref,sc_uint_base> sc_uint_subref::operator,(const sc_uint_base& r ) const
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_base>(
    const_cast<sc_uint_subref&>(*this), const_cast<sc_uint_base&>(r) );
  }

inline  sc_uint_concat<sc_uint_subref,sc_uint_bitref> sc_uint_subref::operator,(sc_uint_bitref& r )
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_bitref>( *this, r );
  }

inline  sc_uint_concat<sc_uint_subref,sc_uint_bitref> sc_uint_subref::operator,(const sc_uint_bitref& r )const
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_bitref>(
    const_cast<sc_uint_subref&>(*this), const_cast<sc_uint_bitref&>(r) );
  }

inline  sc_uint_concat<sc_uint_subref,sc_uint_subref> sc_uint_subref::operator,(sc_uint_subref& r )
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_subref>( *this, r );
  }

inline  sc_uint_concat<sc_uint_subref,sc_uint_subref> sc_uint_subref::operator,( const sc_uint_subref& r )const
  {
    return sc_uint_concat<sc_uint_subref,sc_uint_subref>(
    const_cast<sc_uint_subref&>(*this), const_cast<sc_uint_subref&>(r) );
  }

  // comma operators sc_uint_concat template
#if !defined(__BCPLUSPLUS__)
template<class T1, class T2>
inline  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> sc_uint_concat<T1,T2>::operator,(sc_uint_base& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>( *this, r );
  }

template<class T1, class T2>
inline  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base> sc_uint_concat<T1,T2>::operator,(const sc_uint_base& r )const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>(
    const_cast<sc_uint_concat<T1,T2>&>(*this), const_cast<sc_uint_base&>(r) );
  }

template<class T1, class T2>
inline  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref> sc_uint_concat<T1,T2>::operator,( sc_uint_bitref& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>( *this, r );
  }

template<class T1, class T2>
inline  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>  sc_uint_concat<T1,T2>::operator,( const sc_uint_bitref& r )const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>(
    const_cast<sc_uint_concat<T1,T2>&>(*this), const_cast<sc_uint_bitref&>(r) );
  }

template<class T1, class T2>
inline  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> sc_uint_concat<T1,T2>::operator,( sc_uint_subref& r )
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>( *this, r );
  }

template<class T1, class T2>
inline  sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref> sc_uint_concat<T1,T2>::operator,( const sc_uint_subref& r )const
  {
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>(
    const_cast<sc_uint_concat<T1,T2>&>(*this), const_cast<sc_uint_subref&>(r) );
  }
#endif
  // comma operators sc_uint_bitref class
inline  sc_uint_concat<sc_uint_bitref,sc_uint_base> sc_uint_bitref::operator ,( sc_uint_base& r )
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_base>( *this, r );
  }

inline  sc_uint_concat<sc_uint_bitref,sc_uint_base> sc_uint_bitref::operator ,( const sc_uint_base& r )const
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_base>(
    const_cast<sc_uint_bitref&>(*this), const_cast<sc_uint_base&>(r) );
  }

inline  sc_uint_concat<sc_uint_bitref,sc_uint_bitref> sc_uint_bitref::operator,(sc_uint_bitref& r )
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_bitref>(*this, r);
  }

inline  sc_uint_concat<sc_uint_bitref,sc_uint_bitref> sc_uint_bitref::operator,(const sc_uint_bitref& r )const
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_bitref>(
    const_cast<sc_uint_bitref&>(*this), const_cast<sc_uint_bitref&>(r) );
  }

inline  sc_uint_concat<sc_uint_bitref,sc_uint_subref> sc_uint_bitref::operator,( sc_uint_subref& r )
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_subref>(*this, r );
  }

inline  sc_uint_concat<sc_uint_bitref,sc_uint_subref> sc_uint_bitref::operator,(const sc_uint_subref& r )const
  {
    return sc_uint_concat<sc_uint_bitref,sc_uint_subref>(
    const_cast<sc_uint_bitref&>(*this), const_cast<sc_uint_subref&>(r) );
  }

// implementation of concatenation operators that require templates
// for Sun compiler only, because it's the only compiler that doesn't
// support member templates
#if defined(__SUNPRO_C)
template<class T1, class T2>
inline sc_uint_concat<sc_uint_base, sc_uint_concat<T1,T2> >
operator ,(sc_uint_base& l, sc_uint_concat<T1,T2>& r)
{
  return sc_uint_concat<sc_uint_base, sc_uint_concat<T1,T2> >(l, r);
}

template<class T1, class T2>
inline
sc_uint_concat<sc_uint_base, sc_uint_concat<T1,T2> >
operator ,(const sc_uint_base& l, const sc_uint_concat<T1, T2>& r)
{
  return sc_uint_concat<sc_uint_base, sc_uint_concat<T1,T2> >(const_cast<sc_uint_base&>(l),
                                                              const_cast<sc_uint_concat<T1,T2>&>(r));
}

template<class T1, class T2>
inline
sc_uint_concat<sc_uint_bitref, sc_uint_concat<T1,T2> >
operator,(sc_uint_bitref& l, sc_uint_concat<T1,T2>& r)
{
  return sc_uint_concat<sc_uint_bitref, sc_uint_concat<T1,T2> >(l, r);
}

template<class T1, class T2 >
inline
sc_uint_concat<sc_uint_bitref, sc_uint_concat<T1,T2> >
operator,(const sc_uint_bitref& l, const sc_uint_concat<T1,T2>& r)
{
  return sc_uint_concat<sc_uint_bitref, sc_uint_concat<T1,T2> >(const_cast<sc_uint_bitref&>(l),
                                                                const_cast<sc_uint_concat<T1,T2>&>(r));
}

template<class T1, class T2>
inline
sc_uint_concat<sc_uint_subref, sc_uint_concat<T1,T2> >
operator,(sc_uint_subref& l, sc_uint_concat<T1,T2>& r)
{
  return sc_uint_concat<sc_uint_subref, sc_uint_concat<T1,T2> >(l, r);
}

template<class T1, class T2>
inline
sc_uint_concat<sc_uint_subref, sc_uint_concat<T1,T2> >
operator,(const sc_uint_subref& l, const sc_uint_concat<T1, T2>& r)
{
  return sc_uint_concat<sc_uint_subref, sc_uint_concat<T1,T2> >(const_cast<sc_uint_subref&>(l),
                                                                const_cast<sc_uint_concat<T1,T2>&>(r));
}

template<class T1,class T2, class T3, class T4>
inline
sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >
operator,(sc_uint_concat<T1,T2>& l, sc_uint_concat<T3,T4>& r)
{
  return sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >(l, r);
}

template<class T1,class T2, class T3, class T4>
inline
sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >
operator,(const sc_uint_concat<T1,T2>& l, const sc_uint_concat<T3,T4>& r)
{
  return sc_uint_concat<sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> >(const_cast<sc_uint_concat<T1,T2>&>(l),
                                                                       const_cast<sc_uint_concat<T3,T4>&>(r));
}
#endif //SUN

#endif

