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

  sc_int.h -- A sc_int is a signed integer whose width is less than the
              machine's native integer width. We provide two implementations
              (i) sc_int with bitwidth between 1 - 64 (ii) sc_int with
              bitwidth between 1 - 32 Implementation (i) is the default
              implementation, while implementation (ii) can be used only if
              the class library is compiled with -D_32BIT_. Unlike arbitrary
              precision, arithmetic and bitwise operations are performed
              using the native types (hence capped at 32/64 bits). The sc_int
              integer is useful when the user does not need arbitrary
              precision and the performance is superior to
              sc_bigint/sc_biguint.

  Original Author: Amit Rao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Resolved ambiguity with sc_(un)signed.
                               - Merged the code for 64- and 32-bit versions
                                 via the constants in sc_nbdefs.h.
                               - Eliminated redundant file inclusions.
    
      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_INT_H
#define SC_INT_H


#include "systemc/datatypes/int/sc_nbdefs.h"
#include "systemc/utils/sc_exception.h"
#include "systemc/utils/sc_iostream.h"

// Forward declarations
class sc_int_bitref;
class sc_int_subref;
class sc_int_base;
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
    class sc_bv_base;
    class sc_lv_base;
}
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv_base;

extern const uint_type mask_int[SC_INTWIDTH][SC_INTWIDTH];

#if defined( _MSC_VER )
ostream& operator << ( ostream&, int_type );
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
//
//  Base class for sc_int.
// ----------------------------------------------------------------------------

class sc_int_base
{
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
  template <class T1, class T2> 
  sc_int_base(const sc_int_concat<T1, T2>& v) 
  { 
      width = v.bitwidth();
      int S = SC_INTWIDTH - width;
      num = (v.operator int_type()) << S >> S;
  }
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
  template< class T1, class T2 > 
  sc_int_base& operator = (const sc_int_concat<T1, T2>& v)
    { int S = SC_INTWIDTH - width; num = (v.operator int_type()) << S >> S; return *this; }
  sc_int_base& operator = (const sc_signed&             v);
  sc_int_base& operator = (const sc_unsigned&           v);
#ifdef SC_INCLUDE_FX
  sc_int_base& operator = ( const sc_fxval& v );
  sc_int_base& operator = ( const sc_fxval_fast& v );
  sc_int_base& operator = ( const sc_fxnum& v );
  sc_int_base& operator = ( const sc_fxnum_fast& v );
#endif
  sc_int_base& operator = ( const sc_bv_base& );
  sc_int_base& operator = ( const sc_lv_base& );

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

  void print( ostream& os ) const
      { os << to_int64(); }

public:

  // These can be protected at the expense of adding a inlined member
  // function to get 'num' in the derived class.
  int_type num;
  int width;
};


// concatenation operators

inline
sc_int_concat<sc_int_base,sc_int_base>
operator , ( sc_int_base&, sc_int_base& );

inline
const sc_int_concat<sc_int_base,sc_int_base>
operator , ( const sc_int_base&, const sc_int_base& );

inline
sc_int_concat<sc_int_base,sc_int_bitref>
operator , ( sc_int_base&, sc_int_bitref );

inline
const sc_int_concat<sc_int_base,sc_int_bitref>
operator , ( const sc_int_base&, const sc_int_bitref& );

inline
sc_int_concat<sc_int_base,sc_int_subref>
operator , ( sc_int_base&, sc_int_subref );

inline
const sc_int_concat<sc_int_base,sc_int_subref>
operator , ( const sc_int_base&, const sc_int_subref& );

template <class T1, class T2>
inline
sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
operator , ( sc_int_base&, sc_int_concat<T1,T2> );

template <class T1, class T2>
inline
const sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
operator , ( const sc_int_base&, const sc_int_concat<T1, T2>& );

inline
const sc_int_concat<sc_int_base,sc_int_base>
operator , ( const sc_int_base&, bool );

inline
const sc_int_concat<sc_int_base,sc_int_base>
operator , ( bool, const sc_int_base& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int<W>
//
//  Template class sc_int<W> is the interface that the user sees. It is
//  derived from sc_int_base and most of its methods are just wrappers
//  that call the corresponding method in the parent class. Note that
//  the width of sc_int datatype is specified as a template parameter.
// ----------------------------------------------------------------------------

template< int W /* = SC_INTWIDTH */>
class sc_int
: public sc_int_base
{
  friend class sc_int_bitref;
  friend class sc_int_subref;

public:

  // Constructors
  sc_int()
    : sc_int_base( W )
    {}
  sc_int( int_type v );
  sc_int( const sc_int<W>& v )
    : sc_int_base( v )
    {}
  sc_int( const sc_int_base& v )
    : sc_int_base( W )
    { *this = v; }
  sc_int( const sc_int_subref& v )
    : sc_int_base( W )
    { *this = v; }
  template <class T1, class T2>
  sc_int( const sc_int_concat<T1,T2>& v )
    : sc_int_base( W )
    { *this = v; }
  sc_int( const sc_signed& v )
    : sc_int_base( W )
    { *this = v; }
  sc_int( const sc_unsigned& v )
    : sc_int_base( W )
    { *this = v; }

#ifdef SC_INCLUDE_FX

  explicit sc_int( const sc_fxval& v )
    : sc_int_base( W )
    { *this = v; }

  explicit sc_int( const sc_fxval_fast& v )
    : sc_int_base( W )
    { *this = v; }

  explicit sc_int( const sc_fxnum& v )
    : sc_int_base( W )
    { *this = v; }

  explicit sc_int( const sc_fxnum_fast& v )
    : sc_int_base( W )
    { *this = v; }
#endif

  sc_int( const sc_bv_base& v )
      : sc_int_base( W )
      { *this = v; }
  sc_int( const sc_lv_base& v )
      : sc_int_base( W )
      { *this = v; }

  // Assignment operators
  sc_int<W>& operator = (int_type                     v);
  sc_int<W>& operator = (const sc_int_base&           v) 
    { num = v.num << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); return *this; }
  sc_int<W>& operator = (const sc_int_subref&         v) 
    { sc_int_base::operator=(v); return *this; }
  sc_int<W>& operator = (const sc_int<W>&             v) 
    { num = v.num; return *this; }
  template<class T1, class T2> 
  sc_int<W>& operator = (const sc_int_concat<T1, T2>& v) 
    { sc_int_base::operator=(v); return *this; }
  sc_int<W>& operator = (const sc_signed&             v)
    { sc_int_base::operator=(v); return *this;}
  sc_int<W>& operator = (const sc_unsigned&           v) 
    { sc_int_base::operator=(v); return *this; }

#ifdef SC_INCLUDE_FX

  sc_int<W>& operator = ( const sc_fxval& v )
    { sc_int_base::operator = ( v ); return *this; }

  sc_int<W>& operator = ( const sc_fxval_fast& v )
    { sc_int_base::operator = ( v ); return *this; }

  sc_int<W>& operator = ( const sc_fxnum& v )
    { sc_int_base::operator = ( v ); return *this; }

  sc_int<W>& operator = ( const sc_fxnum_fast& v )
    { sc_int_base::operator = ( v ); return *this; }

#endif

  sc_int<W>& operator = ( const sc_bv_base& v )
      { sc_int_base::operator = ( v ); return *this; }
  sc_int<W>& operator = ( const sc_lv_base& v )
      { sc_int_base::operator = ( v ); return *this; }
  
  // Use sc_int_base's operator int64() const; Note that g++ does not
  // compile this correctly; if we define operator int64() here, g++
  // fails to make it mask the one defined in the base class.
  // operator int64() const { return num; }

  // Arithmetic operators
  sc_int<W>& operator += (int_type v) 
    { 
      int_type val = num;
      val += v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator -= (int_type v) 
    {
      int_type val = num;
      val -= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator *= (int_type v) 
    {
      int_type val = num;
      val *= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator /= (int_type v) 
    {
      int_type val = num;
      val /= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator %= (int_type v) 
    {
      int_type val = num;
      val %= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  
  // Bitwise operators
  sc_int<W>& operator &= (int_type v) 
    {
      int_type val = num;
      val &= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator |= (int_type v) 
    {
      int_type val = num;
      val |= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator ^= (int_type v) 
    {
      int_type val = num;
      val ^= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator <<= (int_type v) 
    {
      int_type val = num;
      val <<= v;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  sc_int<W>& operator >>= (int_type v) 
    { num >>= v; return *this; }
  
  // Prefix and postfix increment and decrement operators
  sc_int<W>& operator ++ () // prefix
    {
      int_type val = num;  
      ++val;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return *this;
    }
  const sc_int<W> operator ++ (int) // postfix
    {
      int_type val = num;
      int_type tmp = val;
      val++;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return sc_int<W>(tmp);
    }
  sc_int<W>& operator -- () //prefix
    {
      int_type val = num;
      --val;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); 
      return *this;
    }
  const sc_int<W> operator -- (int) // postfix 
    {
      int_type val = num;
      int_type tmp = val;
      val--;
      num = val << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W);
      return sc_int<W>(tmp);
    }

  // Inherit bit-select and part-select operators from sc_int_base.
    
  // Concatenation operators are global functions
};


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref
//
//  Class that implements bit-selection on sc_int.
// ----------------------------------------------------------------------------

class sc_int_bitref
{
    friend class sc_int_base;

private:
  
    sc_int_bitref( sc_int_base* obj_, int index_ )
        : m_obj( obj_ ), m_index( index_ )
        {}

public:

    sc_int_bitref( const sc_int_bitref& a )
        : m_obj( a.m_obj ), m_index( a.m_index )
        {}

    sc_int_bitref* clone()
        { return new sc_int_bitref( *this ); }

    int bitwidth() const
        { return 1; }

    sc_int_bitref& operator = ( const sc_int_bitref& b );
    sc_int_bitref& operator = ( bool b );

    sc_int_bitref& operator &= ( bool b );
    sc_int_bitref& operator |= ( bool b );
    sc_int_bitref& operator ^= ( bool b );

    operator bool () const
        { return m_obj->test( m_index ); }
    bool operator ! () const
        { return ! m_obj->test( m_index ); }

private:

    sc_int_base* m_obj;
    int          m_index;

private:

    // disabled
    sc_int_bitref();
};


// concatenation operators

inline
sc_int_concat<sc_int_bitref,sc_int_base>
operator , ( sc_int_bitref, sc_int_base& );

inline
const sc_int_concat<sc_int_bitref,sc_int_base>
operator , ( const sc_int_bitref&, const sc_int_base& );

inline
sc_int_concat<sc_int_bitref,sc_int_bitref>
operator , ( sc_int_bitref, sc_int_bitref );

inline
sc_int_concat<sc_int_bitref,sc_int_subref>
operator , ( sc_int_bitref, sc_int_subref );

template <class T1, class T2>
inline
sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >
operator , ( sc_int_bitref, sc_int_concat<T1,T2> );

inline
const sc_int_concat<sc_int_bitref,sc_int_base>
operator , ( const sc_int_bitref&, bool );

inline
const sc_int_concat<sc_int_base,sc_int_bitref>
operator , ( bool, const sc_int_bitref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref
//
//  Class that implements part-selection on sc_int.
// ----------------------------------------------------------------------------

class sc_int_subref
{
    friend class sc_int_base;

private:

    sc_int_subref( sc_int_base* obj_, int left_, int right_ )
        : m_obj( obj_ ), m_left( left_ ), m_right( right_ )
        {}
  
public:

    sc_int_subref( const sc_int_subref& a )
        : m_obj( a.m_obj ), m_left( a.m_left ), m_right( a.m_right )
        {}

    sc_int_subref* clone()
        { return new sc_int_subref( *this ); }

    int bitwidth() const
        { return ( m_left - m_right + 1 ); }

    // conversion
    operator int_type() const;

    // assignment
    sc_int_subref& operator = ( const sc_int_base& v );
    sc_int_subref& operator = ( const sc_int_subref& v );
    template <class T1, class T2>
    sc_int_subref& operator = ( const sc_int_concat<T1,T2>& v )
        { return operator = ( v.operator int_type() ); }
    sc_int_subref& operator = ( int_type v );

private:

    sc_int_base* m_obj;
    int          m_left;
    int          m_right;

private:

    // disabled
    sc_int_subref();
};


// concatenation operators

inline
sc_int_concat<sc_int_subref,sc_int_base>
operator , ( sc_int_subref, sc_int_base& );

inline
const sc_int_concat<sc_int_subref,sc_int_base>
operator , ( const sc_int_subref&, const sc_int_base& );

inline
sc_int_concat<sc_int_subref,sc_int_bitref>
operator , ( sc_int_subref, sc_int_bitref );

inline
sc_int_concat<sc_int_subref,sc_int_subref>
operator , ( sc_int_subref, sc_int_subref );

template <class T1, class T2>
inline
sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >
operator , ( sc_int_subref, sc_int_concat<T1,T2> );

inline
const sc_int_concat<sc_int_subref,sc_int_base>
operator , ( const sc_int_subref&, bool );

inline
const sc_int_concat<sc_int_base,sc_int_subref>
operator , ( bool, const sc_int_subref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_concat<T1,T2>
//
//  Proxy class to implement concatenation.
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_int_concat
{
public:

    // constructors

    sc_int_concat( T1& left_, T2& right_, int delete_ = 0 )
        : m_left( left_ ), m_right( right_ ), m_delete( delete_ )
        { m_len = m_left.bitwidth() + m_right.bitwidth(); }

    sc_int_concat( const sc_int_concat<T1,T2>& v )
        : m_left( v.m_left ), m_right( v.m_right ), m_len( v.m_len ),
          m_delete( v.m_delete )
        { v.m_delete = 0; }

    // destructor

    ~sc_int_concat()
    {
	if( m_delete == 0 ) { return; }
	if( m_delete & 1 )  { delete &m_left; }
	if( m_delete & 2 )  { delete &m_right; }
    }

    sc_int_concat<T1,T2>* clone()
        { return new sc_int_concat<T1,T2>( *this ); }

    int bitwidth() const
        { return m_len; }

    // conversion

    operator int_type() const
    {
	int_type mask = ( ~const_zero_uint >>
			  ( SC_INTWIDTH - m_right.bitwidth()) );
	int_type high = (m_left << m_right.bitwidth()) & (~mask);
	int_type low  = m_right & mask;
	int_type tmp = high | low;
	return (tmp << (SC_INTWIDTH - m_len) >> (SC_INTWIDTH - m_len));
    }

    // assignment

    template <class T3, class T4>
    sc_int_concat<T1,T2>& operator = ( const sc_int_concat<T3,T4>& v )
        { return operator = ( v.operator int_type() ); }

    sc_int_concat<T1,T2>& operator = ( int_type v )
    {
	int_type mask = ( ~const_zero_uint >>
			  ( SC_INTWIDTH - m_right.bitwidth()) );
	m_right = v & mask;
	m_left = (v & ~mask) >> m_right.bitwidth();
	return *this;
    }

    sc_int_concat<T1,T2>& operator = ( const sc_int_base& v )
        { return operator = ( v.operator int_type() ); }

    sc_int_concat<T1,T2>& operator = ( const sc_int_subref& v )
        { return operator = ( v.operator int_type() ); }

private:

    T1& m_left;
    T2& m_right;
    int m_len;
    mutable int m_delete;

private:

    // disabled
    sc_int_concat();
};


// concatenation operators

template <class T1, class T2>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>
operator , ( sc_int_concat<T1,T2>, sc_int_base& );

template <class T1, class T2>
inline
const sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>
operator , ( const sc_int_concat<T1,T2>&, const sc_int_base& );

template <class T1, class T2>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>
operator , ( sc_int_concat<T1,T2>, sc_int_bitref );

template <class T1, class T2>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>
operator , ( sc_int_concat<T1,T2>, sc_int_subref );

template <class T1, class T2, class T3, class T4>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_concat<T3,T4> >
operator , ( sc_int_concat<T1,T2>, sc_int_concat<T3,T4> );

template <class T1, class T2>
inline
const sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>
operator , ( const sc_int_concat<T1,T2>&, bool );

template <class T1, class T2>
inline
const sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
operator , ( bool, const sc_int_concat<T1,T2>& );


extern void check_bitwidth( int w );
extern void check_index( int w );
extern void check_range( int left, int right );
extern void check_signed_value( int w, int_type v );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
//
//  Base class for sc_int.
// ----------------------------------------------------------------------------

// concatenation operators

inline
sc_int_concat<sc_int_base,sc_int_base>
operator , ( sc_int_base& a, sc_int_base& b )
{
    return sc_int_concat<sc_int_base,sc_int_base>( a, b );
}

inline
const sc_int_concat<sc_int_base,sc_int_base>
operator , ( const sc_int_base& a, const sc_int_base& b )
{
    return sc_int_concat<sc_int_base,sc_int_base>(
	CCAST<sc_int_base&>( a ), CCAST<sc_int_base&>( b ) );
}

inline
sc_int_concat<sc_int_base,sc_int_bitref>
operator , ( sc_int_base& a, sc_int_bitref b )
{
    return sc_int_concat<sc_int_base,sc_int_bitref>( a, *b.clone(), 2 );
}

inline
const sc_int_concat<sc_int_base,sc_int_bitref>
operator , ( const sc_int_base& a, const sc_int_bitref& b )
{
    return sc_int_concat<sc_int_base,sc_int_bitref>(
	CCAST<sc_int_base&>( a ), CCAST<sc_int_bitref&>( b ) );
}

inline
sc_int_concat<sc_int_base,sc_int_subref>
operator , ( sc_int_base& a, sc_int_subref b )
{
    return sc_int_concat<sc_int_base,sc_int_subref>( a, *b.clone(), 2 );
}

inline
const sc_int_concat<sc_int_base,sc_int_subref>
operator , ( const sc_int_base& a, const sc_int_subref& b )
{
    return sc_int_concat<sc_int_base,sc_int_subref>(
	CCAST<sc_int_base&>( a ), CCAST<sc_int_subref&>( b ) );
}

template <class T1, class T2>
inline
sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
operator , ( sc_int_base& a, sc_int_concat<T1,T2> b )
{
    return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >( a,
							     *b.clone(), 2 );
}

template <class T1, class T2>
inline
const sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
operator , ( const sc_int_base& a, const sc_int_concat<T1, T2>& b )
{
    return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >(
	CCAST<sc_int_base&>( a ), CCAST<sc_int_concat<T1,T2>&>( b ) );
}

inline
const sc_int_concat<sc_int_base,sc_int_base>
operator , ( const sc_int_base& a, bool b )
{
    return sc_int_concat<sc_int_base,sc_int_base>(
	CCAST<sc_int_base&>( a ), *new sc_int<1>( b ? -1 : 0 ), 2 );
}

inline
const sc_int_concat<sc_int_base,sc_int_base>
operator , ( bool a, const sc_int_base& b )
{
    return sc_int_concat<sc_int_base,sc_int_base>(
	*new sc_int<1>( a ? -1 : 0 ), CCAST<sc_int_base&>( b ), 1 );
}


inline
ostream&
operator << ( ostream& os, const sc_int_base& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int<W>
//
//  Template class sc_int<W> is the interface that the user sees. It is
//  derived from sc_int_base and most of its methods are just wrappers
//  that call the corresponding method in the parent class. Note that
//  the width of sc_int datatype is specified as a template parameter.
// ----------------------------------------------------------------------------

template <int W>
inline
sc_int<W>::sc_int( int_type v )
: sc_int_base( W ) 
{
#ifdef DEBUG_SYSTEMC
    check_signed_value( W, v );
#endif
    num = v << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); 
}

template <int W>
inline 
sc_int<W>& 
sc_int<W>::operator = ( int_type v )
{
#ifdef DEBUG_SYSTEMC
    check_signed_value( W, v );
#endif
    num = v << (SC_INTWIDTH - W) >> (SC_INTWIDTH - W); 
    return *this;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref
//
//  Class that implements bit-selection on sc_int.
// ----------------------------------------------------------------------------

// concatenation operators

inline
sc_int_concat<sc_int_bitref,sc_int_base>
operator , ( sc_int_bitref a, sc_int_base& b )
{
    return sc_int_concat<sc_int_bitref,sc_int_base>( *a.clone(), b, 1 );
}

inline
const sc_int_concat<sc_int_bitref,sc_int_base>
operator , ( const sc_int_bitref& a, const sc_int_base& b )
{
    return sc_int_concat<sc_int_bitref,sc_int_base>(
	CCAST<sc_int_bitref&>( a ), CCAST<sc_int_base&>( b ) );
}

inline
sc_int_concat<sc_int_bitref,sc_int_bitref>
operator , ( sc_int_bitref a, sc_int_bitref b )
{
    return sc_int_concat<sc_int_bitref,sc_int_bitref>( *a.clone(),
						       *b.clone(), 3 );
}

inline
sc_int_concat<sc_int_bitref,sc_int_subref>
operator , ( sc_int_bitref a, sc_int_subref b )
{
    return sc_int_concat<sc_int_bitref,sc_int_subref>( *a.clone(),
						       *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >
operator , ( sc_int_bitref a, sc_int_concat<T1,T2> b )
{
    return sc_int_concat<sc_int_bitref,sc_int_concat<T1,T2> >( *a.clone(),
							       *b.clone(), 3 );
}

inline
const sc_int_concat<sc_int_bitref,sc_int_base>
operator , ( const sc_int_bitref& a, bool b )
{
    return sc_int_concat<sc_int_bitref,sc_int_base>(
	CCAST<sc_int_bitref&>( a ), *new sc_int<1>( b ? -1 : 0 ), 2 );
}

inline
const sc_int_concat<sc_int_base,sc_int_bitref>
operator , ( bool a, const sc_int_bitref& b )
{
    return sc_int_concat<sc_int_base,sc_int_bitref>(
	*new sc_int<1>( a ? -1 : 0 ), CCAST<sc_int_bitref&>( b ), 1 );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref
//
//  Class that implements part-selection on sc_int.
// ----------------------------------------------------------------------------

// concatenation operators

inline
sc_int_concat<sc_int_subref,sc_int_base>
operator , ( sc_int_subref a, sc_int_base& b )
{
    return sc_int_concat<sc_int_subref,sc_int_base>( *a.clone(), b, 1 );
}

inline
const sc_int_concat<sc_int_subref,sc_int_base>
operator , ( const sc_int_subref& a, const sc_int_base& b )
{
    return sc_int_concat<sc_int_subref,sc_int_base>(
	CCAST<sc_int_subref&>( a ), CCAST<sc_int_base&>( b ) );
}

inline
sc_int_concat<sc_int_subref,sc_int_bitref>
operator , ( sc_int_subref a, sc_int_bitref b )
{
    return sc_int_concat<sc_int_subref,sc_int_bitref>( *a.clone(),
						       *b.clone(), 3 );
}

inline
sc_int_concat<sc_int_subref,sc_int_subref>
operator , ( sc_int_subref a, sc_int_subref b )
{
    return sc_int_concat<sc_int_subref,sc_int_subref>( *a.clone(),
						       *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >
operator , ( sc_int_subref a, sc_int_concat<T1,T2> b )
{
    return sc_int_concat<sc_int_subref,sc_int_concat<T1,T2> >( *a.clone(),
							       *b.clone(), 3 );
}

inline
const sc_int_concat<sc_int_subref,sc_int_base>
operator , ( const sc_int_subref& a, bool b )
{
    return sc_int_concat<sc_int_subref,sc_int_base>(
	CCAST<sc_int_subref&>( a ), *new sc_int<1>( b ? -1 : 0 ), 2 );
}

inline
const sc_int_concat<sc_int_base,sc_int_subref>
operator , ( bool a, const sc_int_subref& b )
{
    return sc_int_concat<sc_int_base,sc_int_subref>(
	*new sc_int<1>( a ? -1 : 0 ), CCAST<sc_int_subref&>( b ), 1 );
}


inline
ostream&
operator << ( ostream& os, const sc_int_subref& a )
{
    return os << SCAST<int_type>( a );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_concat<T1,T2>
//
//  Proxy class to implement concatenation.
// ----------------------------------------------------------------------------

// concatenation operators

template <class T1, class T2>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>
operator , ( sc_int_concat<T1,T2> a, sc_int_base& b )
{
    return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>( *a.clone(), b, 1 );
}

template <class T1, class T2>
inline
const sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>
operator , ( const sc_int_concat<T1,T2>& a, const sc_int_base& b )
{
    return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>(
	CCAST<sc_int_concat<T1,T2>&>( a ), CCAST<sc_int_base&>( b ) );
}

template <class T1, class T2>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>
operator , ( sc_int_concat<T1,T2> a, sc_int_bitref b )
{
    return sc_int_concat<sc_int_concat<T1,T2>,sc_int_bitref>( *a.clone(),
							      *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>
operator , ( sc_int_concat<T1,T2> a, sc_int_subref b )
{
    return sc_int_concat<sc_int_concat<T1,T2>,sc_int_subref>( *a.clone(),
							      *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_int_concat<sc_int_concat<T1,T2>,sc_int_concat<T3,T4> >
operator , ( sc_int_concat<T1,T2> a, sc_int_concat<T3,T4> b )
{
    return sc_int_concat<sc_int_concat<T1,T2>,sc_int_concat<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
const sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>
operator , ( const sc_int_concat<T1,T2>& a, bool b )
{
    return sc_int_concat<sc_int_concat<T1,T2>,sc_int_base>(
	CCAST<sc_int_concat<T1,T2>&>( a ), *new sc_int<1>( b ? -1 : 0 ), 2 );
}

template <class T1, class T2>
inline
const sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >
operator , ( bool a, const sc_int_concat<T1,T2>& b )
{
    return sc_int_concat<sc_int_base,sc_int_concat<T1,T2> >(
	*new sc_int<1>( a ? -1 : 0 ), CCAST<sc_int_concat<T1,T2>&>( b ), 1 );
}


#endif
