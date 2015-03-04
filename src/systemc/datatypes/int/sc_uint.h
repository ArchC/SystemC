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

  sc_uint.h -- A sc_int is an unsigned integer whose width is less than the
               machine's native integer width. We provide two implementations
               (i) sc_uint with bitwidth between 1 - 64 (ii) sc_uint with
               bitwidth between 1 - 32 Implementation (i) is the default
               implementation, while implementation (ii) can be used only if
               compiled with -D_32BIT_. Unlike arbitrary precision, arithmetic
               and bitwise operations are performed using the native types
               (hence capped at 32/64 bits). The sc_uint integer is useful
               when the user does not need arbitrary precision and the
               performance is superior to sc_bigint/sc_biguint.

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

#ifndef SC_UINT_H
#define SC_UINT_H


#include "systemc/datatypes/int/sc_nbdefs.h"
#include "systemc/utils/sc_exception.h"
#include "systemc/utils/sc_iostream.h"

#define MASK(W) ((~ const_zero_uint) >> (SC_INTWIDTH - W))

// Forward declarations
class sc_uint_bitref;
class sc_uint_subref;
class sc_uint_base;
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
    class sc_bv_base;
    class sc_lv_base;
}
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv_base;

extern const uint_type mask_int[SC_INTWIDTH][SC_INTWIDTH];

#if defined( _MSC_VER )
ostream& operator << ( ostream&, uint_type );
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_base
//
//  Base class for sc_uint.
// ----------------------------------------------------------------------------

class sc_uint_base
{
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
  template<class T1, class T2> 
  sc_uint_base(const sc_uint_concat<T1,T2>& v)
    {
      width = v.bitwidth();
      num = (v.operator uint_type()) & MASK(width);
    }
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
  template<class T1, class T2> 
  sc_uint_base& operator = (const sc_uint_concat<T1,T2>& v)
    {
      num = (v.operator uint_type()) & MASK(width);
      return *this;
    }
  sc_uint_base& operator = (const sc_signed&             v);
  sc_uint_base& operator = (const sc_unsigned&           v);
#ifdef SC_INCLUDE_FX
  sc_uint_base& operator = ( const sc_fxval& v );
  sc_uint_base& operator = ( const sc_fxval_fast& v );
  sc_uint_base& operator = ( const sc_fxnum& v );
  sc_uint_base& operator = ( const sc_fxnum_fast& v );
#endif
  sc_uint_base& operator = ( const sc_bv_base& );
  sc_uint_base& operator = ( const sc_lv_base& );

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

  void print( ostream& os ) const
      { os << to_uint64(); }

public: 

  // These can be protected at the expense of adding a inlined member
  // function to get 'num' in the derived class.
  uint_type num;
  int width;
};


// concatenation operators

inline
sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( sc_uint_base&, sc_uint_base& );

inline
const sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( const sc_uint_base&, const sc_uint_base& );

inline
sc_uint_concat<sc_uint_base,sc_uint_bitref>
operator , ( sc_uint_base&, sc_uint_bitref );

inline
const sc_uint_concat<sc_uint_base,sc_uint_bitref>
operator , ( const sc_uint_base&, const sc_uint_bitref& );

inline
sc_uint_concat<sc_uint_base,sc_uint_subref>
operator , ( sc_uint_base&, sc_uint_subref );

inline
const sc_uint_concat<sc_uint_base,sc_uint_subref>
operator , ( const sc_uint_base&, const sc_uint_subref& );

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >
operator , ( sc_uint_base&, sc_uint_concat<T1,T2> );

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >
operator , ( const sc_uint_base&, const sc_uint_concat<T1, T2>& );

inline
const sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( const sc_uint_base&, bool );

inline
const sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( bool, const sc_uint_base& );


// ----------------------------------------------------------------------------
//  CLASS : sc_uint<W>
//
//  Template class sc_uint<W> is the interface that the user sees. It
//  is derived from sc_uint_base and most of its methods are just
//  wrappers that call the corresponding method in the parent
//  class. Note that the width of sc_uint datatype is specified as a
//  template parameter.
// ----------------------------------------------------------------------------

template< int W /* = SC_INTWIDTH */>
class sc_uint
: public sc_uint_base
{
  friend class sc_uint_bitref;
  friend class sc_uint_subref;

public:

  // Constructors
  sc_uint()
    : sc_uint_base( W )
    {}
  sc_uint( uint_type v );
  sc_uint( const sc_uint<W>& v )
    : sc_uint_base( v )
    {}
  sc_uint( const sc_uint_base& v )
    : sc_uint_base( W )
    { (*this) = v; }
  sc_uint( const sc_uint_subref& v )
    : sc_uint_base( W )
    { (*this) = v; }
  template <class T1, class T2>
  sc_uint( const sc_uint_concat<T1,T2>& v )
    : sc_uint_base( W )
    { (*this) = v; }
  sc_uint( const sc_signed& v )
    : sc_uint_base( W )
    { (*this) = v; }
  sc_uint( const sc_unsigned& v )
    : sc_uint_base( W )
    { (*this) = v; }

#ifdef SC_INCLUDE_FX

  explicit sc_uint( const sc_fxval& v )
    : sc_uint_base( W )
    { *this = v; }

  explicit sc_uint( const sc_fxval_fast& v )
    : sc_uint_base( W )
    { *this = v; }

  explicit sc_uint( const sc_fxnum& v )
    : sc_uint_base( W )
    { *this = v; }

  explicit sc_uint( const sc_fxnum_fast& v )
    : sc_uint_base( W )
    { *this = v; }

#endif

  sc_uint( const sc_bv_base& v )
      : sc_uint_base( W )
      { *this = v; }
  sc_uint( const sc_lv_base& v )
      : sc_uint_base( W )
      { *this = v; }

  // Assignment operators
  sc_uint<W>& operator = (uint_type                    v);
  sc_uint<W>& operator = (const sc_uint_base&          v)
    {
      num = v.num & MASK(W);
      return *this;
    }
  sc_uint<W>& operator = (const sc_uint_subref&        v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint<W>& operator = (const sc_uint<W>&            v) 
    { num = v.num; return *this; }
  template<class T1, class T2> 
  sc_uint<W>& operator = (const sc_uint_concat<T1,T2>& v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint<W>& operator = (const sc_signed&             v) 
    { sc_uint_base::operator=(v); return *this; }
  sc_uint<W>& operator = (const sc_unsigned&           v) 
    { sc_uint_base::operator=(v); return *this;}

#ifdef SC_INCLUDE_FX

  sc_uint<W>& operator = ( const sc_fxval& v )
    { sc_uint_base::operator = ( v ); return *this; }

  sc_uint<W>& operator = ( const sc_fxval_fast& v )
    { sc_uint_base::operator = ( v ); return *this; }

  sc_uint<W>& operator = ( const sc_fxnum& v )
    { sc_uint_base::operator = ( v ); return *this; }

  sc_uint<W>& operator = ( const sc_fxnum_fast& v )
    { sc_uint_base::operator = ( v ); return *this; }

#endif

  sc_uint<W>& operator = ( const sc_bv_base& v )
      { sc_uint_base::operator = ( v ); return *this; }
  sc_uint<W>& operator = ( const sc_lv_base& v )
      { sc_uint_base::operator = ( v ); return *this; }

  // Arithmetic operators
  sc_uint<W>& operator += (uint_type v) 
    {
      uint_type val = num;
      val += v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator -= (uint_type v) 
    {
      uint_type val = num;
      val -= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator *= (uint_type v) 
    {
      uint_type val = num;
      val *= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator /= (uint_type v) 
    {
      uint_type val = num;
      val /= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator %= (uint_type v) 
    {
      uint_type val = num;
      val %= v;
      num = val & MASK(W);
      return *this;
    }
  
  // Bitwise operators
  sc_uint<W>& operator &= (uint_type v) 
    {
      uint_type val = num;
      val &= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator |= (uint_type v) 
    {
      uint_type val = num;
      val |= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator ^= (uint_type v) 
    {
      uint_type val = num;
      val ^= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator <<= (uint_type v) 
    {
      uint_type val = num;
      val <<= v;
      num = val & MASK(W);
      return *this;
    }
  sc_uint<W>& operator >>= (uint_type v) 
    { num >>= v; return *this; }
  
  // Prefix and postfix increment and decrement operators
  sc_uint<W>& operator ++ () // prefix
    {
      uint_type val = num;
      ++val;
      val &= MASK(W); 
      num = val;
      return *this;
    }
  const sc_uint<W> operator ++ (int) // postfix
    {
      uint_type val = num;   
      uint_type tmp = val;
      val++;
      val &= MASK(W); 
      num = val;
      return sc_uint<W>(tmp);
    }
  sc_uint<W>& operator -- () // prefix
    {
      uint_type val = num;  
      --val;
      val &= MASK(W);
      num = val;
      return *this;
    }
  const sc_uint<W> operator -- (int) // postfix 
    {
      uint_type val = num;
      uint_type tmp = val;
      val--;
      val &= MASK(W);
      num = val;
      return sc_uint<W>(tmp);
    }

  // Inherit bit-select and part-select operators from sc_uint_base.
    
  // Concatenation operators are global functions.
};


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_bitref
//
//  Class that implements bit-selection on sc_uint.
// ----------------------------------------------------------------------------

class sc_uint_bitref
{
    friend class sc_uint_base;

private:

    sc_uint_bitref( sc_uint_base* obj_, int index_ )
        : m_obj( obj_ ), m_index( index_ )
        {}

public:

    sc_uint_bitref( const sc_uint_bitref& a )
        : m_obj( a.m_obj ), m_index( a.m_index )
        {}

    sc_uint_bitref* clone()
        { return new sc_uint_bitref( *this ); }

    int bitwidth() const
        { return 1; }

    sc_uint_bitref& operator = ( const sc_uint_bitref& b )
    {
	m_obj->set( m_index, (b.m_obj)->test( b.m_index ) );
	return *this;
    }
    sc_uint_bitref& operator = ( bool b )
    {
	m_obj->set( m_index, b );
	return *this;
    }
    sc_uint_bitref& operator &= ( bool b )
    {
	m_obj->num &= ~ ((! b) << m_index);
	return *this;
    }
    sc_uint_bitref& operator |= ( bool b )
    {
	// m_obj->num |= ((b & const_one_uint) << m_index);
	if( b ) m_obj->num |= uint_type( 1 ) << m_index;
	return *this;
    }
    sc_uint_bitref& operator ^= ( bool b )
    {
	// m_obj->num ^= ((b & const_one_uint) << m_index);
	m_obj->num ^= uint_type( b ) << m_index;
	return *this;
    }

    operator bool () const
        { return m_obj->test( m_index ); }
    bool operator ! () const
        { return ! m_obj->test( m_index ); }

private:

    sc_uint_base* m_obj;
    int           m_index;

private:

    // disabled
    sc_uint_bitref();
};


// concatenation operators

inline
sc_uint_concat<sc_uint_bitref,sc_uint_base>
operator , ( sc_uint_bitref, sc_uint_base& );

inline
const sc_uint_concat<sc_uint_bitref,sc_uint_base>
operator , ( const sc_uint_bitref&, const sc_uint_base& );

inline
sc_uint_concat<sc_uint_bitref,sc_uint_bitref>
operator , ( sc_uint_bitref, sc_uint_bitref );

inline
sc_uint_concat<sc_uint_bitref,sc_uint_subref>
operator , ( sc_uint_bitref, sc_uint_subref );

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >
operator , ( sc_uint_bitref, sc_uint_concat<T1,T2> );

inline
const sc_uint_concat<sc_uint_bitref,sc_uint_base>
operator , ( const sc_uint_bitref&, bool );

inline
const sc_uint_concat<sc_uint_base,sc_uint_bitref>
operator , ( bool, const sc_uint_bitref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_subref
//
//  Class that implements part-selection on sc_uint.
// ----------------------------------------------------------------------------

class sc_uint_subref
{
    friend class sc_uint_base;

private:

    sc_uint_subref( sc_uint_base* obj_, int left_, int right_ )
        : m_obj( obj_ ), m_left( left_ ), m_right( right_ )
        {}
  
public:

    sc_uint_subref( const sc_uint_subref& a )
        : m_obj( a.m_obj ), m_left( a.m_left ), m_right( a.m_right )
        {}

    sc_uint_subref* clone()
        { return new sc_uint_subref( *this ); }

    int bitwidth() const
        { return ( m_left - m_right + 1 ); }

    // conversion
    operator uint_type() const;

    // assignment
    sc_uint_subref& operator = ( const sc_uint_base& v );
    sc_uint_subref& operator = ( const sc_uint_subref& v );
    template <class T1, class T2>
    sc_uint_subref& operator = ( const sc_uint_concat<T1,T2>& v )
        { return operator = ( v.operator uint_type() ); }
    sc_uint_subref& operator = ( uint_type v );

private:

    sc_uint_base* m_obj;
    int           m_left;
    int           m_right;

private:

    // disabled
    sc_uint_subref();
};


// concatenation operators

inline
sc_uint_concat<sc_uint_subref,sc_uint_base>
operator , ( sc_uint_subref, sc_uint_base& );

inline
const sc_uint_concat<sc_uint_subref,sc_uint_base>
operator , ( const sc_uint_subref&, const sc_uint_base& );

inline
sc_uint_concat<sc_uint_subref,sc_uint_bitref>
operator , ( sc_uint_subref, sc_uint_bitref );

inline
sc_uint_concat<sc_uint_subref,sc_uint_subref>
operator , ( sc_uint_subref, sc_uint_subref );

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >
operator , ( sc_uint_subref, sc_uint_concat<T1,T2> );

inline
const sc_uint_concat<sc_uint_subref,sc_uint_base>
operator , ( const sc_uint_subref&, bool );

inline
const sc_uint_concat<sc_uint_base,sc_uint_subref>
operator , ( bool, const sc_uint_subref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_concat<T1,T2>
//
//  Proxy class to implement concatenation.
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_uint_concat
{
public:

    // constructors

    sc_uint_concat( T1& left_, T2& right_, int delete_ = 0 )
        : m_left( left_ ), m_right( right_ ), m_delete( delete_ )
        { m_len = m_left.bitwidth() + m_right.bitwidth(); }

    sc_uint_concat( const sc_uint_concat<T1,T2>& v )
        : m_left( v.m_left ), m_right( v.m_right ), m_len( v.m_len ),
          m_delete( v.m_delete )
        { v.m_delete = 0; }

    // destructor

    ~sc_uint_concat()
    {
	if( m_delete == 0 ) { return; }
	if( m_delete & 1 )  { delete &m_left; }
	if( m_delete & 2 )  { delete &m_right; }
    }

    sc_uint_concat<T1,T2>* clone()
        { return new sc_uint_concat<T1,T2>( *this ); }

    int bitwidth() const
        { return m_len; }

    // conversion

    operator uint_type() const 
    {   
	uint_type mask = ( ~const_zero_uint >>
			   ( SC_INTWIDTH - m_right.bitwidth()) );
	uint_type high = (m_left << m_right.bitwidth()) & (~mask);
	uint_type low  = m_right & mask;
	uint_type tmp = high | low;
	return (tmp << (SC_INTWIDTH - m_len) >> (SC_INTWIDTH - m_len));
    }

    // assignment

    template <class T3, class T4>
    sc_uint_concat<T1,T2>& operator = ( const sc_uint_concat<T3,T4>& v )
        { return operator = ( v.operator uint_type() ); }

    sc_uint_concat<T1,T2>& operator = ( uint_type v )
    {
	uint_type mask = ( ~const_zero_uint >>
			   ( SC_INTWIDTH - m_right.bitwidth()) );
	m_right = v & mask;
	m_left = (v & ~mask) >> m_right.bitwidth();
	return *this;
    }

    sc_uint_concat<T1,T2>& operator = ( const sc_uint_base& v )
        { return operator = ( v.operator uint_type() ); }

    sc_uint_concat<T1,T2>& operator = ( const sc_uint_subref& v )
        { return operator = ( v.operator uint_type() ); }

private:

    T1& m_left;
    T2& m_right;
    int m_len;
    mutable int m_delete;

private:

    // disabled
    sc_uint_concat();
};


// concatenation operators

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>
operator , ( sc_uint_concat<T1,T2>, sc_uint_base& );

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>
operator , ( const sc_uint_concat<T1,T2>&, const sc_uint_base& );

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>
operator , ( sc_uint_concat<T1,T2>, sc_uint_bitref );

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>
operator , ( sc_uint_concat<T1,T2>, sc_uint_subref );

template <class T1, class T2, class T3, class T4>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_concat<T3,T4> >
operator , ( sc_uint_concat<T1,T2>, sc_uint_concat<T3,T4> );

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>
operator , ( const sc_uint_concat<T1,T2>&, bool );

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >
operator , ( bool, const sc_uint_concat<T1,T2>& );


extern void check_bitwidth( int w );
extern void check_index( int w );
extern void check_range( int left, int right );
extern void check_unsigned_value( int w, uint_type v );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_uint_base
//
//  Base class for sc_uint.
// ----------------------------------------------------------------------------

// concatenation operators

inline
sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( sc_uint_base& a, sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_base>( a, b );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( const sc_uint_base& a, const sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_base>(
	CCAST<sc_uint_base&>( a ), CCAST<sc_uint_base&>( b ) );
}

inline
sc_uint_concat<sc_uint_base,sc_uint_bitref>
operator , ( sc_uint_base& a, sc_uint_bitref b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_bitref>( a, *b.clone(), 2 );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_bitref>
operator , ( const sc_uint_base& a, const sc_uint_bitref& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_bitref>(
	CCAST<sc_uint_base&>( a ), CCAST<sc_uint_bitref&>( b ) );
}

inline
sc_uint_concat<sc_uint_base,sc_uint_subref>
operator , ( sc_uint_base& a, sc_uint_subref b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_subref>( a, *b.clone(), 2 );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_subref>
operator , ( const sc_uint_base& a, const sc_uint_subref& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_subref>(
	CCAST<sc_uint_base&>( a ), CCAST<sc_uint_subref&>( b ) );
}

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >
operator , ( sc_uint_base& a, sc_uint_concat<T1,T2> b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >
operator , ( const sc_uint_base& a, const sc_uint_concat<T1, T2>& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >(
	CCAST<sc_uint_base&>( a ), CCAST<sc_uint_concat<T1,T2>&>( b ) );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( const sc_uint_base& a, bool b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_base>(
	CCAST<sc_uint_base&>( a ), *new sc_uint<1>( b ? 1 : 0 ), 2 );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_base>
operator , ( bool a, const sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_base>(
	*new sc_uint<1>( a ? 1 : 0 ), CCAST<sc_uint_base&>( b ), 1 );
}


inline
ostream&
operator << ( ostream& os, const sc_uint_base& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_uint<W>
//
//  Template class sc_uint<W> is the interface that the user sees. It
//  is derived from sc_uint_base and most of its methods are just
//  wrappers that call the corresponding method in the parent
//  class. Note that the width of sc_uint datatype is specified as a
//  template parameter.
// ----------------------------------------------------------------------------

template <int W>
inline
sc_uint<W>::sc_uint( uint_type v )
: sc_uint_base( W )
{
#ifdef DEBUG_SYSTEMC
    check_unsigned_value( W, v );
#endif
    num = v & MASK( W );
}

template <int W>
inline
sc_uint<W>& 
sc_uint<W>::operator = ( uint_type v )
{
#ifdef DEBUG_SYSTEMC
    check_unsigned_value( W, v );
#endif
    num = v & MASK( W );
    return *this;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_bitref
//
//  Class that implements bit-selection on sc_uint.
// ----------------------------------------------------------------------------

// concatenation operators

inline
sc_uint_concat<sc_uint_bitref,sc_uint_base>
operator , ( sc_uint_bitref a, sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_bitref,sc_uint_base>( *a.clone(), b, 1 );
}

inline
const sc_uint_concat<sc_uint_bitref,sc_uint_base>
operator , ( const sc_uint_bitref& a, const sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_bitref,sc_uint_base>(
	CCAST<sc_uint_bitref&>( a ), CCAST<sc_uint_base&>( b ) );
}

inline
sc_uint_concat<sc_uint_bitref,sc_uint_bitref>
operator , ( sc_uint_bitref a, sc_uint_bitref b )
{
    return sc_uint_concat<sc_uint_bitref,sc_uint_bitref>( *a.clone(),
							  *b.clone(), 3 );
}

inline
sc_uint_concat<sc_uint_bitref,sc_uint_subref>
operator , ( sc_uint_bitref a, sc_uint_subref b )
{
    return sc_uint_concat<sc_uint_bitref,sc_uint_subref>( *a.clone(),
							  *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >
operator , ( sc_uint_bitref a, sc_uint_concat<T1,T2> b )
{
    return sc_uint_concat<sc_uint_bitref,sc_uint_concat<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
const sc_uint_concat<sc_uint_bitref,sc_uint_base>
operator , ( const sc_uint_bitref& a, bool b )
{
    return sc_uint_concat<sc_uint_bitref,sc_uint_base>(
	CCAST<sc_uint_bitref&>( a ), *new sc_uint<1>( b ? 1 : 0 ), 2 );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_bitref>
operator , ( bool a, const sc_uint_bitref& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_bitref>(
	*new sc_uint<1>( a ? 1 : 0 ), CCAST<sc_uint_bitref&>( b ), 1 );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_subref
//
//  Class that implements part-selection on sc_uint.
// ----------------------------------------------------------------------------


// concatenation operators

inline
sc_uint_concat<sc_uint_subref,sc_uint_base>
operator , ( sc_uint_subref a, sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_subref,sc_uint_base>( *a.clone(), b, 1 );
}

inline
const sc_uint_concat<sc_uint_subref,sc_uint_base>
operator , ( const sc_uint_subref& a, const sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_subref,sc_uint_base>(
	CCAST<sc_uint_subref&>( a ), CCAST<sc_uint_base&>( b ) );
}

inline
sc_uint_concat<sc_uint_subref,sc_uint_bitref>
operator , ( sc_uint_subref a, sc_uint_bitref b )
{
    return sc_uint_concat<sc_uint_subref,sc_uint_bitref>( *a.clone(),
							  *b.clone(), 3 );
}

inline
sc_uint_concat<sc_uint_subref,sc_uint_subref>
operator , ( sc_uint_subref a, sc_uint_subref b )
{
    return sc_uint_concat<sc_uint_subref,sc_uint_subref>( *a.clone(),
							  *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >
operator , ( sc_uint_subref a, sc_uint_concat<T1,T2> b )
{
    return sc_uint_concat<sc_uint_subref,sc_uint_concat<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
const sc_uint_concat<sc_uint_subref,sc_uint_base>
operator , ( const sc_uint_subref& a, bool b )
{
    return sc_uint_concat<sc_uint_subref,sc_uint_base>(
	CCAST<sc_uint_subref&>( a ), *new sc_uint<1>( b ? 1 : 0 ), 2 );
}

inline
const sc_uint_concat<sc_uint_base,sc_uint_subref>
operator , ( bool a, const sc_uint_subref& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_subref>(
	*new sc_uint<1>( a ? 1 : 0 ), CCAST<sc_uint_subref&>( b ), 1 );
}


inline
ostream&
operator << ( ostream& os, const sc_uint_subref& a )
{
    return os << SCAST<uint_type>( a );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_uint_concat<T1,T2>
//
//  Proxy class to implement concatenation.
// ----------------------------------------------------------------------------

// concatenation operators

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>
operator , ( sc_uint_concat<T1,T2> a, sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>
operator , ( const sc_uint_concat<T1,T2>& a, const sc_uint_base& b )
{
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>(
	CCAST<sc_uint_concat<T1,T2>&>( a ), CCAST<sc_uint_base&>( b ) );
}

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>
operator , ( sc_uint_concat<T1,T2> a, sc_uint_bitref b )
{
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>
operator , ( sc_uint_concat<T1,T2> a, sc_uint_subref b )
{
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_concat<T3,T4> >
operator , ( sc_uint_concat<T1,T2> a, sc_uint_concat<T3,T4> b )
{
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_concat<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>
operator , ( const sc_uint_concat<T1,T2>& a, bool b )
{
    return sc_uint_concat<sc_uint_concat<T1,T2>,sc_uint_base>(
	CCAST<sc_uint_concat<T1,T2>&>( a ), *new sc_uint<1>( b ? 1 : 0 ), 2 );
}

template <class T1, class T2>
inline
const sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >
operator , ( bool a, const sc_uint_concat<T1,T2>& b )
{
    return sc_uint_concat<sc_uint_base,sc_uint_concat<T1,T2> >(
	*new sc_uint<1>( a ? 1 : 0 ), CCAST<sc_uint_concat<T1,T2>&>( b ), 1 );
}


#endif
