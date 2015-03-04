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

  sc_int_base.h -- A sc_int is a signed integer whose length is less than the
              machine's native integer length. We provide two implementations
              (i) sc_int with length between 1 - 64, and (ii) sc_int with
              length between 1 - 32. Implementation (i) is the default
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

#ifndef SC_INT_BASE_H
#define SC_INT_BASE_H


#include "systemc/datatypes/int/sc_int_ids.h"
#include "systemc/datatypes/int/sc_length_param.h"
#include "systemc/datatypes/int/sc_nbdefs.h"
#include "systemc/utils/sc_iostream.h"


namespace sc_dt
{

// classes defined in this module
template <class T1, class T2> class sc_int_concref_r;
template <class T1, class T2> class sc_int_concref;
class sc_int_bitref_r;
class sc_int_bitref;
class sc_int_subref_r;
class sc_int_subref;
class sc_int_base;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
class sc_signed;
class sc_unsigned;
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;


extern const uint_type mask_int[SC_INTWIDTH][SC_INTWIDTH];


// to avoid code bloat in sc_int_concref<T1,T2>

extern
void
sc_int_concref_invalid_length( int length );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_int_concref_r<T1,T2>
//
//  Proxy class for sc_int concatenation (r-value only).
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_int_concref_r
{
    // support methods

    void check_length() const
	{ if( m_len > SC_INTWIDTH ) sc_int_concref_invalid_length( m_len ); }

public:

    // constructor

    sc_int_concref_r( const T1& left_, const T2& right_, int delete_ = 0 )
        : m_left( CCAST<T1&>( left_ ) ), m_right( CCAST<T2&>( right_ ) ),
	  m_delete( delete_ ), m_refs( *new int( 1 ) )
        { m_len = m_left.length() + m_right.length(); check_length(); }


    // copy constructor

    sc_int_concref_r( const sc_int_concref_r<T1,T2>& a )
        : m_left( a.m_left ), m_right( a.m_right ), m_len( a.m_len ),
          m_delete( a.m_delete ), m_refs( a.m_refs )
	{ ++ m_refs; }


    // destructor

    ~sc_int_concref_r();


    // cloning

    sc_int_concref_r<T1,T2>* clone() const
        { return new sc_int_concref_r<T1,T2>( *this ); }


    // capacity

    int length() const
	{ return m_len; }

#ifdef SC_DT_DEPRECATED
    int bitwidth() const
        { return length(); }
#endif


    // reduce methods

    bool and_reduce() const;

    bool nand_reduce() const
	{ return ( ! and_reduce() ); }

    bool or_reduce() const;

    bool nor_reduce() const
	{ return ( ! or_reduce() ); }

    bool xor_reduce() const;

    bool xnor_reduce() const
	{ return ( ! xor_reduce() ); }


    // implicit conversion to int_type

    operator int_type() const;


    // explicit conversions

    int_type value() const
	{ return operator int_type(); }


    int           to_int() const;
    unsigned int  to_uint() const;
    long          to_long() const;
    unsigned long to_ulong() const;
    int64         to_int64() const;
    uint64        to_uint64() const;
    double        to_double() const;


    // explicit conversion to character string

    const sc_string to_string( sc_numrep numrep = SC_DEC ) const;
    const sc_string to_string( sc_numrep numrep, bool w_prefix ) const;


    // other methods

    void print( ostream& os = cout ) const
	{ os << to_string(); }

protected:

    T1&          m_left;
    T2&          m_right;
    int          m_len;
    mutable int  m_delete;
    mutable int& m_refs;

private:

    // disabled
    sc_int_concref_r();
    sc_int_concref_r<T1,T2>& operator = ( const sc_int_concref_r<T1,T2>& );
};


// r-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
operator , ( sc_int_concref_r<T1,T2>, sc_int_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
operator , ( sc_int_concref_r<T1,T2>, sc_int_bitref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
operator , ( sc_int_concref_r<T1,T2>, sc_int_subref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref_r<T1,T2>, const sc_int_base& );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref_r<T1,T2>, bool );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( bool, sc_int_concref_r<T1,T2> );


template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
concat( sc_int_concref_r<T1,T2>, sc_int_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
concat( sc_int_concref_r<T1,T2>, sc_int_bitref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
concat( sc_int_concref_r<T1,T2>, sc_int_subref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref_r<T1,T2>, const sc_int_base& );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref_r<T1,T2>, bool );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( bool, sc_int_concref_r<T1,T2> );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
operator , ( sc_int_concref_r<T1,T2>, sc_int_concref<T3,T4> );

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
operator , ( sc_int_concref<T1,T2>, sc_int_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
operator , ( sc_int_concref_r<T1,T2>, sc_int_bitref );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
operator , ( sc_int_concref<T1,T2>, sc_int_bitref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
operator , ( sc_int_concref_r<T1,T2>, sc_int_subref );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
operator , ( sc_int_concref<T1,T2>, sc_int_subref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref_r<T1,T2>, sc_int_base& );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref<T1,T2>, const sc_int_base& );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref<T1,T2>, bool );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( bool, sc_int_concref<T1,T2> );


template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
concat( sc_int_concref_r<T1,T2>, sc_int_concref<T3,T4> );

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
concat( sc_int_concref<T1,T2>, sc_int_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
concat( sc_int_concref_r<T1,T2>, sc_int_bitref );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
concat( sc_int_concref<T1,T2>, sc_int_bitref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
concat( sc_int_concref_r<T1,T2>, sc_int_subref );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
concat( sc_int_concref<T1,T2>, sc_int_subref_r );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref_r<T1,T2>, sc_int_base& );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref<T1,T2>, const sc_int_base& );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref<T1,T2>, bool );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( bool, sc_int_concref<T1,T2> );

#endif


template <class T1, class T2>
inline
ostream&
operator << ( ostream&, const sc_int_concref_r<T1,T2>& );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_int_concref<T1,T2>
//
//  Proxy class for sc_int concatenation (r-value and l-value).
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_int_concref
    : public sc_int_concref_r<T1,T2>
{
public:

    // constructor

    sc_int_concref( T1& left_, T2& right_, int delete_ = 0 )
	: sc_int_concref_r<T1,T2>( left_, right_, delete_ )
	{}


    // copy constructor

    sc_int_concref( const sc_int_concref<T1,T2>& a )
	: sc_int_concref_r<T1,T2>( a )
	{}


    // cloning

    sc_int_concref<T1,T2>* clone() const
        { return new sc_int_concref<T1,T2>( *this ); }


    // assignment operators

    sc_int_concref<T1,T2>& operator = ( int_type v );

    template <class T3, class T4>
    sc_int_concref<T1,T2>& operator = ( const sc_int_concref_r<T3,T4>& a )
        { return operator = ( a.operator int_type() ); }

    template <class T3, class T4>
    sc_int_concref<T1,T2>& operator = ( const sc_int_concref<T3,T4>& a )
        { return operator = ( a.operator int_type() ); }

    sc_int_concref<T1,T2>& operator = ( const sc_int_base& a );
    sc_int_concref<T1,T2>& operator = ( const sc_int_subref_r& a );
    sc_int_concref<T1,T2>& operator = ( const char* a );

    sc_int_concref<T1,T2>& operator = ( unsigned long a )
	{ return operator = ( (int_type) a ); }

    sc_int_concref<T1,T2>& operator = ( long a )
	{ return operator = ( (int_type) a ); }

    sc_int_concref<T1,T2>& operator = ( unsigned int a )
	{ return operator = ( (int_type) a ); }

    sc_int_concref<T1,T2>& operator = ( int a )
	{ return operator = ( (int_type) a ); }

    sc_int_concref<T1,T2>& operator = ( uint64 a )
	{ return operator = ( (int_type) a ); }

    sc_int_concref<T1,T2>& operator = ( double a )
	{ return operator = ( (int_type) a ); }

    sc_int_concref<T1,T2>& operator = ( const sc_signed& );
    sc_int_concref<T1,T2>& operator = ( const sc_unsigned& );
    sc_int_concref<T1,T2>& operator = ( const sc_bv_base& );
    sc_int_concref<T1,T2>& operator = ( const sc_lv_base& );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_int_concref();
};


// l-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_concref<T3,T4> >
operator , ( sc_int_concref<T1,T2>, sc_int_concref<T3,T4> );

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_bitref>
operator , ( sc_int_concref<T1,T2>, sc_int_bitref );

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_subref>
operator , ( sc_int_concref<T1,T2>, sc_int_subref );

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_base>
operator , ( sc_int_concref<T1,T2>, sc_int_base& );


template <class T1, class T2, class T3, class T4>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_concref<T3,T4> >
concat( sc_int_concref<T1,T2>, sc_int_concref<T3,T4> );

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_bitref>
concat( sc_int_concref<T1,T2>, sc_int_bitref );

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_subref>
concat( sc_int_concref<T1,T2>, sc_int_subref );

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_base>
concat( sc_int_concref<T1,T2>, sc_int_base& );


template <class T1, class T2>
inline
istream&
operator >> ( istream&, sc_int_concref<T1,T2>& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref_r
//
//  Proxy class for sc_int bit selection (r-value only).
// ----------------------------------------------------------------------------

class sc_int_bitref_r
{
    friend class sc_int_base;

protected:

    // constructor
  
    sc_int_bitref_r( const sc_int_base& obj_, int index_ )
        : m_obj( CCAST<sc_int_base&>( obj_ ) ), m_index( index_ )
        {}

public:

    // copy constructor

    sc_int_bitref_r( const sc_int_bitref_r& a )
        : m_obj( a.m_obj ), m_index( a.m_index )
        {}


    // cloning

    sc_int_bitref_r* clone() const
        { return new sc_int_bitref_r( *this ); }


    // capacity

    int length() const
	{ return 1; }

#ifdef SC_DT_DEPRECATED
    int bitwidth() const
	{ return length(); }
#endif


    // implicit conversion to bool

    operator bool () const;
    bool operator ! () const;
    bool operator ~ () const;


    // explicit conversions

    bool value() const
	{ return operator bool(); }

    bool to_bool() const
	{ return operator bool(); }


    // other methods

    void print( ostream& os = cout ) const
	{ os << to_bool(); }

protected:

    sc_int_base& m_obj;
    int          m_index;

private:

    // disabled
    sc_int_bitref_r();
    sc_int_bitref_r& operator = ( const sc_int_bitref_r& );
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_bitref_r, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
operator , ( sc_int_bitref_r, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
operator , ( sc_int_bitref_r, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref_r, const sc_int_base& );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref_r, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( bool, sc_int_bitref_r );


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_bitref_r, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
concat( sc_int_bitref_r, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
concat( sc_int_bitref_r, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref_r, const sc_int_base& );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref_r, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( bool, sc_int_bitref_r );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_bitref_r, sc_int_concref<T1,T2> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_bitref, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
operator , ( sc_int_bitref_r, sc_int_bitref );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
operator , ( sc_int_bitref, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
operator , ( sc_int_bitref_r, sc_int_subref );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
operator , ( sc_int_bitref, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref_r, sc_int_base& );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref, const sc_int_base& );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( bool, sc_int_bitref );


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_bitref_r, sc_int_concref<T1,T2> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_bitref, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
concat( sc_int_bitref_r, sc_int_bitref );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
concat( sc_int_bitref, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
concat( sc_int_bitref_r, sc_int_subref );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
concat( sc_int_bitref, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref_r, sc_int_base& );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref, const sc_int_base& );

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( bool, sc_int_bitref );

#endif


inline
ostream&
operator << ( ostream&, const sc_int_bitref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref
//
//  Proxy class for sc_int bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_int_bitref
    : public sc_int_bitref_r
{
    friend class sc_int_base;


    // constructor
  
    sc_int_bitref( sc_int_base& obj_, int index_ )
	: sc_int_bitref_r( obj_, index_ )
        {}

public:

    // copy constructor

    sc_int_bitref( const sc_int_bitref& a )
	: sc_int_bitref_r( a )
        {}


    // cloning

    sc_int_bitref* clone() const
        { return new sc_int_bitref( *this ); }


    // assignment operators

    sc_int_bitref& operator = ( const sc_int_bitref_r& b );
    sc_int_bitref& operator = ( const sc_int_bitref& b );
    sc_int_bitref& operator = ( bool b );

    sc_int_bitref& operator &= ( bool b );
    sc_int_bitref& operator |= ( bool b );
    sc_int_bitref& operator ^= ( bool b );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_int_bitref();
};


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref<sc_int_bitref,sc_int_concref<T1,T2> >
operator , ( sc_int_bitref, sc_int_concref<T1,T2> );

inline
sc_int_concref<sc_int_bitref,sc_int_bitref>
operator , ( sc_int_bitref, sc_int_bitref );

inline
sc_int_concref<sc_int_bitref,sc_int_subref>
operator , ( sc_int_bitref, sc_int_subref );

inline
sc_int_concref<sc_int_bitref,sc_int_base>
operator , ( sc_int_bitref, sc_int_base& );


template <class T1, class T2>
inline
sc_int_concref<sc_int_bitref,sc_int_concref<T1,T2> >
concat( sc_int_bitref, sc_int_concref<T1,T2> );

inline
sc_int_concref<sc_int_bitref,sc_int_bitref>
concat( sc_int_bitref, sc_int_bitref );

inline
sc_int_concref<sc_int_bitref,sc_int_subref>
concat( sc_int_bitref, sc_int_subref );

inline
sc_int_concref<sc_int_bitref,sc_int_base>
concat( sc_int_bitref, sc_int_base& );


inline
istream&
operator >> ( istream&, sc_int_bitref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref_r
//
//  Proxy class for sc_int part selection (r-value only).
// ----------------------------------------------------------------------------

class sc_int_subref_r
{
    friend class sc_int_base;

protected:

    // constructor

    sc_int_subref_r( const sc_int_base& obj_, int left_, int right_ )
        : m_obj( CCAST<sc_int_base&>( obj_ ) ),
	  m_left( left_ ), m_right( right_ )
        {}
  
public:

    // copy constructor

    sc_int_subref_r( const sc_int_subref_r& a )
        : m_obj( a.m_obj ), m_left( a.m_left ), m_right( a.m_right )
        {}


    // cloning

    sc_int_subref_r* clone() const
        { return new sc_int_subref_r( *this ); }


    // capacity

    int length() const
        { return ( m_left - m_right + 1 ); }

#ifdef SC_DT_DEPRECATED
    int bitwidth() const
	{ return length(); }
#endif


    // reduce methods

    bool and_reduce() const;

    bool nand_reduce() const
	{ return ( ! and_reduce() ); }

    bool or_reduce() const;

    bool nor_reduce() const
	{ return ( ! or_reduce() ); }

    bool xor_reduce() const;

    bool xnor_reduce() const
	{ return ( ! xor_reduce() ); }


    // implicit conversion to int_type

    operator int_type () const;


    // explicit conversions

    int_type value() const
	{ return operator int_type(); }


    int           to_int() const;
    unsigned int  to_uint() const;
    long          to_long() const;
    unsigned long to_ulong() const;
    int64         to_int64() const;
    uint64        to_uint64() const;
    double        to_double() const;


    // explicit conversion to character string

    const sc_string to_string( sc_numrep numrep = SC_DEC ) const;
    const sc_string to_string( sc_numrep numrep, bool w_prefix ) const;


    // other methods

    void print( ostream& os = cout ) const
	{ os << to_string(); }

protected:

    sc_int_base& m_obj;
    int          m_left;
    int          m_right;

private:

    // disabled
    sc_int_subref_r();
    sc_int_subref_r& operator = ( const sc_int_subref_r& );
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_subref_r, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
operator , ( sc_int_subref_r, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
operator , ( sc_int_subref_r, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref_r, const sc_int_base& );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref_r, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( bool, sc_int_subref_r );


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_subref_r, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
concat( sc_int_subref_r, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
concat( sc_int_subref_r, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref_r, const sc_int_base& );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref_r, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( bool, sc_int_subref_r );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_subref_r, sc_int_concref<T1,T2> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_subref, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
operator , ( sc_int_subref_r, sc_int_bitref );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
operator , ( sc_int_subref, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
operator , ( sc_int_subref_r, sc_int_subref );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
operator , ( sc_int_subref, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref_r, sc_int_base& );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref, const sc_int_base& );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( bool, sc_int_subref );


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_subref_r, sc_int_concref<T1,T2> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_subref, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
concat( sc_int_subref_r, sc_int_bitref );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
concat( sc_int_subref, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
concat( sc_int_subref_r, sc_int_subref );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
concat( sc_int_subref, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref_r, sc_int_base& );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref, const sc_int_base& );

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( bool, sc_int_subref );

#endif


inline
ostream&
operator << ( ostream&, const sc_int_subref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref
//
//  Proxy class for sc_int part selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_int_subref
    : public sc_int_subref_r
{
    friend class sc_int_base;


    // constructor

    sc_int_subref( sc_int_base& obj_, int left_, int right_ )
	: sc_int_subref_r( obj_, left_, right_ )
        {}
  
public:

    // copy constructor

    sc_int_subref( const sc_int_subref& a )
	: sc_int_subref_r( a )
        {}


    // cloning

    sc_int_subref* clone() const
        { return new sc_int_subref( *this ); }


    // assignment operators

    sc_int_subref& operator = ( int_type v );
    sc_int_subref& operator = ( const sc_int_base& a );

    sc_int_subref& operator = ( const sc_int_subref_r& a )
	{ return operator = ( a.operator int_type() ); }

    sc_int_subref& operator = ( const sc_int_subref& a )
	{ return operator = ( a.operator int_type() ); }

    template <class T1, class T2>
    sc_int_subref& operator = ( const sc_int_concref_r<T1,T2>& a )
        { return operator = ( a.operator int_type() ); }

    sc_int_subref& operator = ( const char* a );

    sc_int_subref& operator = ( unsigned long a )
	{ return operator = ( (int_type) a ); }

    sc_int_subref& operator = ( long a )
	{ return operator = ( (int_type) a ); }

    sc_int_subref& operator = ( unsigned int a )
	{ return operator = ( (int_type) a ); }

    sc_int_subref& operator = ( int a )
	{ return operator = ( (int_type) a ); }

    sc_int_subref& operator = ( uint64 a )
	{ return operator = ( (int_type) a ); }

    sc_int_subref& operator = ( double a )
	{ return operator = ( (int_type) a ); }

    sc_int_subref& operator = ( const sc_signed& );
    sc_int_subref& operator = ( const sc_unsigned& );
    sc_int_subref& operator = ( const sc_bv_base& );
    sc_int_subref& operator = ( const sc_lv_base& );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_int_subref();
};


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref<sc_int_subref,sc_int_concref<T1,T2> >
operator , ( sc_int_subref, sc_int_concref<T1,T2> );

inline
sc_int_concref<sc_int_subref,sc_int_bitref>
operator , ( sc_int_subref, sc_int_bitref );

inline
sc_int_concref<sc_int_subref,sc_int_subref>
operator , ( sc_int_subref, sc_int_subref );

inline
sc_int_concref<sc_int_subref,sc_int_base>
operator , ( sc_int_subref, sc_int_base& );


template <class T1, class T2>
inline
sc_int_concref<sc_int_subref,sc_int_concref<T1,T2> >
concat( sc_int_subref, sc_int_concref<T1,T2> );

inline
sc_int_concref<sc_int_subref,sc_int_bitref>
concat( sc_int_subref, sc_int_bitref );

inline
sc_int_concref<sc_int_subref,sc_int_subref>
concat( sc_int_subref, sc_int_subref );

inline
sc_int_concref<sc_int_subref,sc_int_base>
concat( sc_int_subref, sc_int_base& );


inline
istream&
operator >> ( istream&, sc_int_subref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
//
//  Base class for sc_int.
// ----------------------------------------------------------------------------

class sc_int_base
{
    friend class sc_int_bitref_r;
    friend class sc_int_bitref;
    friend class sc_int_subref_r;
    friend class sc_int_subref;


    // support methods

    void invalid_length() const;
    void invalid_index( int i ) const;
    void invalid_range( int l, int r ) const;

    void check_length() const
	{ if( m_len <= 0 || m_len > SC_INTWIDTH ) { invalid_length(); } }

    void check_index( int i ) const
	{ if( i < 0 || i >= m_len ) { invalid_index( i ); } }

    void check_range( int l, int r ) const
	{ if( r < 0 || l >= m_len || l < r ) { invalid_range( l, r ); } }

    void check_value() const;

    void extend_sign()
	{
#ifdef DEBUG_SYSTEMC
	    check_value();
#endif
	    m_val = ( m_val << m_ulen >> m_ulen );
	}

public:

    // constructors

    explicit sc_int_base( int w = sc_length_param().len() )
	: m_val( 0 ), m_len( w ), m_ulen( SC_INTWIDTH - m_len )
	{ check_length(); }

    sc_int_base( int_type v, int w )
	: m_val( v ), m_len( w ), m_ulen( SC_INTWIDTH - m_len )
	{ check_length(); extend_sign(); }

    sc_int_base( const sc_int_base& a )
	: m_val( a.m_val ), m_len( a.m_len ), m_ulen( a.m_ulen )
	{}

    explicit sc_int_base( const sc_int_subref_r& a )
        : m_val( a ), m_len( a.length() ), m_ulen( SC_INTWIDTH - m_len )
        { extend_sign(); }

    template <class T1, class T2> 
    explicit sc_int_base( const sc_int_concref_r<T1,T2>& a )
        : m_val( a ), m_len( a.length() ), m_ulen( SC_INTWIDTH - m_len )
        { extend_sign(); }

    explicit sc_int_base( const sc_signed& a );
    explicit sc_int_base( const sc_unsigned& a );


    // destructor

    ~sc_int_base()
	{}


    // assignment operators

    sc_int_base& operator = ( int_type v )
	{ m_val = v; extend_sign(); return *this; }

    sc_int_base& operator = ( const sc_int_base& a )
	{ m_val = a.m_val; extend_sign(); return *this; }

    sc_int_base& operator = ( const sc_int_subref_r& a )
        { m_val = a; extend_sign(); return *this; }

    template <class T1, class T2>
    sc_int_base& operator = ( const sc_int_concref_r<T1,T2>& a )
        { m_val = a; extend_sign(); return *this; }

    sc_int_base& operator = ( const sc_signed& a );
    sc_int_base& operator = ( const sc_unsigned& a );

#ifdef SC_INCLUDE_FX
    sc_int_base& operator = ( const sc_fxval& a );
    sc_int_base& operator = ( const sc_fxval_fast& a );
    sc_int_base& operator = ( const sc_fxnum& a );
    sc_int_base& operator = ( const sc_fxnum_fast& a );
#endif

    sc_int_base& operator = ( const sc_bv_base& a );
    sc_int_base& operator = ( const sc_lv_base& a );

    sc_int_base& operator = ( const char* a );

    sc_int_base& operator = ( unsigned long a )
	{ m_val = a; extend_sign(); return *this; }

    sc_int_base& operator = ( long a )
	{ m_val = a; extend_sign(); return *this; }

    sc_int_base& operator = ( unsigned int a )
	{ m_val = a; extend_sign(); return *this; }

    sc_int_base& operator = ( int a )
	{ m_val = a; extend_sign(); return *this; }

    sc_int_base& operator = ( uint64 a )
	{ m_val = a; extend_sign(); return *this; }

    sc_int_base& operator = ( double a )
	{ m_val = (int_type) a; extend_sign(); return *this; }


    // arithmetic assignment operators

    sc_int_base& operator += ( int_type v )
	{ m_val += v; extend_sign(); return *this; }

    sc_int_base& operator -= ( int_type v )
	{ m_val -= v; extend_sign(); return *this; }

    sc_int_base& operator *= ( int_type v )
	{ m_val *= v; extend_sign(); return *this; }

    sc_int_base& operator /= ( int_type v )
	{ m_val /= v; extend_sign(); return *this; }

    sc_int_base& operator %= ( int_type v )
	{ m_val %= v; extend_sign(); return *this; }


    // bitwise assignment operators

    sc_int_base& operator &= ( int_type v )
	{ m_val &= v; extend_sign(); return *this; }

    sc_int_base& operator |= ( int_type v )
	{ m_val |= v; extend_sign(); return *this; }

    sc_int_base& operator ^= ( int_type v )
	{ m_val ^= v; extend_sign(); return *this; }


    sc_int_base& operator <<= ( int_type v )
	{ m_val <<= v; extend_sign(); return *this; }

    sc_int_base& operator >>= ( int_type v )
	{ m_val >>= v; /* no sign extension needed */ return *this; }


    // prefix and postfix increment and decrement operators

    sc_int_base& operator ++ () // prefix
	{ ++ m_val; extend_sign(); return *this; }

    const sc_int_base operator ++ ( int ) // postfix
	{ sc_int_base tmp( *this ); ++ m_val; extend_sign(); return tmp; }
 
    sc_int_base& operator -- () // prefix
	{ -- m_val; extend_sign(); return *this; }

    const sc_int_base operator -- ( int ) // postfix
	{ sc_int_base tmp( *this ); -- m_val; extend_sign(); return tmp; }


    // relational operators

    friend bool operator == ( const sc_int_base& a, const sc_int_base& b )
	{ return a.m_val == b.m_val; }

    friend bool operator != ( const sc_int_base& a, const sc_int_base& b )
	{ return a.m_val != b.m_val; }

    friend bool operator <  ( const sc_int_base& a, const sc_int_base& b )
	{ return a.m_val < b.m_val; }

    friend bool operator <= ( const sc_int_base& a, const sc_int_base& b )
	{ return a.m_val <= b.m_val; }

    friend bool operator >  ( const sc_int_base& a, const sc_int_base& b )
	{ return a.m_val > b.m_val; }

    friend bool operator >= ( const sc_int_base& a, const sc_int_base& b )
	{ return a.m_val >= b.m_val; }


    // bit selection

    sc_int_bitref   operator [] ( int i );
    sc_int_bitref_r operator [] ( int i ) const;

    sc_int_bitref   bit( int i );
    sc_int_bitref_r bit( int i ) const;


    // part selection

    sc_int_subref   operator () ( int left, int right );
    sc_int_subref_r operator () ( int left, int right ) const;

    sc_int_subref   range( int left, int right );
    sc_int_subref_r range( int left, int right ) const;


    // bit access, without bounds checking or sign extension

    bool test( int i ) const
	{ return ( 0 != (m_val & (UINT_ONE << i)) ); }

    void set( int i )
	{ m_val |= (UINT_ONE << i); }

    void set( int i, bool v )
	{ v ? m_val |= (UINT_ONE << i) : m_val &= ~(UINT_ONE << i); }


    // capacity

    int length() const
	{ return m_len; }

#ifdef SC_DT_DEPRECATED
    int bitwidth() const
	{ return length(); }
#endif


    // reduce methods

    bool and_reduce() const;

    bool nand_reduce() const
	{ return ( ! and_reduce() ); }

    bool or_reduce() const;

    bool nor_reduce() const
	{ return ( ! or_reduce() ); }

    bool xor_reduce() const;

    bool xnor_reduce() const
	{ return ( ! xor_reduce() ); }


    // implicit conversion to int_type

    operator int_type() const
	{ return m_val; }


    // explicit conversions

    int_type value() const
	{ return operator int_type(); }


    int to_int() const
	{ return (int) m_val; }

    unsigned int to_uint() const
	{ return (unsigned int) m_val; }

    long to_long() const
	{ return (long) m_val; }

    unsigned long to_ulong() const
	{ return (unsigned long) m_val; }

    int64 to_int64() const
	{ return (int64) m_val; }

    uint64 to_uint64() const
	{ return (uint64) m_val; }

    double to_double() const
	{ return (double) m_val; }


#ifndef _32BIT_
    long long_low() const
	{ return (long) (m_val & UINT64_32ONES); }

    long long_high() const
	{ return (long) ((m_val >> 32) & UINT64_32ONES); }
#endif


    // explicit conversion to character string

    const sc_string to_string( sc_numrep numrep = SC_DEC ) const;
    const sc_string to_string( sc_numrep numrep, bool w_prefix ) const;


    // other methods

    void print( ostream& os = cout ) const
	{ os << to_string(); }

    void scan( istream& is = cin );

protected:

    int_type m_val;   // value
    int      m_len;   // length
    int      m_ulen;  // unused length
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( const sc_int_base&, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( const sc_int_base&, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( const sc_int_base&, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( const sc_int_base&, const sc_int_base& );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( const sc_int_base&, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( bool, const sc_int_base& );


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( const sc_int_base&, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( const sc_int_base&, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( const sc_int_base&, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( const sc_int_base&, const sc_int_base& );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( const sc_int_base&, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( bool, const sc_int_base& );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( const sc_int_base&, sc_int_concref<T1,T2> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( sc_int_base&, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( const sc_int_base&, sc_int_bitref );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( sc_int_base&, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( const sc_int_base&, sc_int_subref );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( sc_int_base&, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( const sc_int_base&, sc_int_base& );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( sc_int_base&, const sc_int_base& );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( sc_int_base&, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( bool, sc_int_base& );


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( const sc_int_base&, sc_int_concref<T1,T2> );

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( sc_int_base&, sc_int_concref_r<T1,T2> );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( const sc_int_base&, sc_int_bitref );

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( sc_int_base&, sc_int_bitref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( const sc_int_base&, sc_int_subref );

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( sc_int_base&, sc_int_subref_r );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( const sc_int_base&, sc_int_base& );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( sc_int_base&, const sc_int_base& );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( sc_int_base&, bool );

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( bool, sc_int_base& );

#endif


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref<sc_int_base,sc_int_concref<T1,T2> >
operator , ( sc_int_base&, sc_int_concref<T1,T2> );

inline
sc_int_concref<sc_int_base,sc_int_bitref>
operator , ( sc_int_base&, sc_int_bitref );

inline
sc_int_concref<sc_int_base,sc_int_subref>
operator , ( sc_int_base&, sc_int_subref );

inline
sc_int_concref<sc_int_base,sc_int_base>
operator , ( sc_int_base&, sc_int_base& );


template <class T1, class T2>
inline
sc_int_concref<sc_int_base,sc_int_concref<T1,T2> >
concat( sc_int_base&, sc_int_concref<T1,T2> );

inline
sc_int_concref<sc_int_base,sc_int_bitref>
concat( sc_int_base&, sc_int_bitref );

inline
sc_int_concref<sc_int_base,sc_int_subref>
concat( sc_int_base&, sc_int_subref );

inline
sc_int_concref<sc_int_base,sc_int_base>
concat( sc_int_base&, sc_int_base& );


inline
ostream&
operator << ( ostream&, const sc_int_base& );

inline
istream&
operator >> ( istream&, sc_int_base& );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_int_concref_r<T1,T2>
//
//  Proxy class for sc_int concatenation (r-value only).
// ----------------------------------------------------------------------------

// destructor

template <class T1, class T2>
inline
sc_int_concref_r<T1,T2>::~sc_int_concref_r()
{
    if( -- m_refs == 0 ) {
	delete &m_refs;
	if( m_delete == 0 ) {
	    return;
	}
	if( m_delete & 1 ) {
	    delete &m_left;
	}
	if( m_delete & 2 ) {
	    delete &m_right;
	}
    }
}


// reduce methods

template <class T1, class T2>
inline
bool
sc_int_concref_r<T1,T2>::and_reduce() const
{
    sc_int_base a( *this );
    return a.and_reduce();
}

template <class T1, class T2>
inline
bool
sc_int_concref_r<T1,T2>::or_reduce() const
{
    sc_int_base a( *this );
    return a.or_reduce();
}

template <class T1, class T2>
inline
bool
sc_int_concref_r<T1,T2>::xor_reduce() const
{
    sc_int_base a( *this );
    return a.xor_reduce();
}


// implicit conversion to int_type

template <class T1, class T2>
inline
sc_int_concref_r<T1,T2>::operator int_type() const
{
    int_type mask = ( ~UINT_ZERO >> (SC_INTWIDTH - m_right.length()) );
    int_type high = ((int_type) m_left << m_right.length()) & (~mask);
    int_type low  = m_right & mask;
    int_type tmp = high | low;
    int ulen = SC_INTWIDTH - m_len;
    return ( tmp << ulen >> ulen );
}


// explicit conversions

template <class T1, class T2>
inline
int
sc_int_concref_r<T1,T2>::to_int() const
{
    sc_int_base a( *this );
    return a.to_int();
}

template <class T1, class T2>
inline
unsigned int
sc_int_concref_r<T1,T2>::to_uint() const
{
    sc_int_base a( *this );
    return a.to_uint();
}

template <class T1, class T2>
inline
long
sc_int_concref_r<T1,T2>::to_long() const
{
    sc_int_base a( *this );
    return a.to_long();
}

template <class T1, class T2>
inline
unsigned long
sc_int_concref_r<T1,T2>::to_ulong() const
{
    sc_int_base a( *this );
    return a.to_ulong();
}

template <class T1, class T2>
inline
int64
sc_int_concref_r<T1,T2>::to_int64() const
{
    sc_int_base a( *this );
    return a.to_int64();
}

template <class T1, class T2>
inline
uint64
sc_int_concref_r<T1,T2>::to_uint64() const
{
    sc_int_base a( *this );
    return a.to_uint64();
}

template <class T1, class T2>
inline
double
sc_int_concref_r<T1,T2>::to_double() const
{
    sc_int_base a( *this );
    return a.to_double();
}


// explicit conversion to character string

template <class T1, class T2>
inline
const sc_string
sc_int_concref_r<T1,T2>::to_string( sc_numrep numrep ) const
{
    sc_int_base a( *this );
    return a.to_string( numrep );
}

template <class T1, class T2>
inline
const sc_string
sc_int_concref_r<T1,T2>::to_string( sc_numrep numrep, bool w_prefix ) const
{
    sc_int_base a( *this );
    return a.to_string( numrep, w_prefix );
}


// functional notation for the reduce methods

template <class T1, class T2>
inline
bool
and_reduce( const sc_int_concref_r<T1,T2>& a )
{
    return a.and_reduce();
}

template <class T1, class T2>
inline
bool
nand_reduce( const sc_int_concref_r<T1,T2>& a )
{
    return a.nand_reduce();
}

template <class T1, class T2>
inline
bool
or_reduce( const sc_int_concref_r<T1,T2>& a )
{
    return a.or_reduce();
}

template <class T1, class T2>
inline
bool
nor_reduce( const sc_int_concref_r<T1,T2>& a )
{
    return a.nor_reduce();
}

template <class T1, class T2>
inline
bool
xor_reduce( const sc_int_concref_r<T1,T2>& a )
{
    return a.xor_reduce();
}

template <class T1, class T2>
inline
bool
xnor_reduce( const sc_int_concref_r<T1,T2>& a )
{
    return a.xnor_reduce();
}


// r-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
operator , ( sc_int_concref_r<T1,T2> a, sc_int_concref_r<T3,T4> b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,
	sc_int_concref_r<T3,T4> >( *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
operator , ( sc_int_concref_r<T1,T2> a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
operator , ( sc_int_concref_r<T1,T2> a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref_r<T1,T2> a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref_r<T1,T2> a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( bool a, sc_int_concref_r<T1,T2> b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
concat( sc_int_concref_r<T1,T2> a, sc_int_concref_r<T3,T4> b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,
	sc_int_concref_r<T3,T4> >( *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
concat( sc_int_concref_r<T1,T2> a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
concat( sc_int_concref_r<T1,T2> a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref_r<T1,T2> a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref_r<T1,T2> a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( bool a, sc_int_concref_r<T1,T2> b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
operator , ( sc_int_concref_r<T1,T2> a, sc_int_concref<T3,T4> b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,
	sc_int_concref_r<T3,T4> >( *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
operator , ( sc_int_concref<T1,T2> a, sc_int_concref_r<T3,T4> b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,
	sc_int_concref_r<T3,T4> >( *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
operator , ( sc_int_concref_r<T1,T2> a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
operator , ( sc_int_concref<T1,T2> a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
operator , ( sc_int_concref_r<T1,T2> a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
operator , ( sc_int_concref<T1,T2> a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref_r<T1,T2> a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref<T1,T2> a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
operator , ( sc_int_concref<T1,T2> a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( bool a, sc_int_concref<T1,T2> b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
concat( sc_int_concref_r<T1,T2> a, sc_int_concref<T3,T4> b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,
	sc_int_concref_r<T3,T4> >( *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_concref_r<T3,T4> >
concat( sc_int_concref<T1,T2> a, sc_int_concref_r<T3,T4> b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,
	sc_int_concref_r<T3,T4> >( *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
concat( sc_int_concref_r<T1,T2> a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>
concat( sc_int_concref<T1,T2> a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
concat( sc_int_concref_r<T1,T2> a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>
concat( sc_int_concref<T1,T2> a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref_r<T1,T2> a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref<T1,T2> a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>
concat( sc_int_concref<T1,T2> a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_concref_r<T1,T2>,sc_int_base>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( bool a, sc_int_concref<T1,T2> b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}

#endif


template <class T1, class T2>
inline
ostream&
operator << ( ostream& os, const sc_int_concref_r<T1,T2>& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_int_concref<T1,T2>
//
//  Proxy class for sc_int concatenation (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( int_type v )
{
    int_type mask = ( ~UINT_ZERO >> (SC_INTWIDTH - this->m_right.length()) );
    this->m_right = v & mask;
    this->m_left = (v & ~mask) >> this->m_right.length();
    return *this;
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const sc_int_base& a )
{
    return operator = ( a.operator int_type() );
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const sc_int_subref_r& a )
{
    return operator = ( a.operator int_type() );
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const char* a )
{
    sc_int_base aa( this->length() );
    return ( *this = aa = a );
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const sc_signed& a )
{
    sc_int_base aa( this->length() );
    return ( *this = aa = a );
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const sc_unsigned& a )
{
    sc_int_base aa( this->length() );
    return ( *this = aa = a );
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const sc_bv_base& a )
{
    sc_int_base aa( this->length() );
    return ( *this = aa = a );
}

template <class T1, class T2>
inline
sc_int_concref<T1,T2>&
sc_int_concref<T1,T2>::operator = ( const sc_lv_base& a )
{
    sc_int_base aa( this->length() );
    return ( *this = aa = a );
}


// other methods

template <class T1, class T2>
inline
void
sc_int_concref<T1,T2>::scan( istream& is )
{
    sc_string s;
    is >> s;
    *this = s.c_str();
}


// l-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_concref<T3,T4> >
operator , ( sc_int_concref<T1,T2> a, sc_int_concref<T3,T4> b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_concref<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_bitref>
operator , ( sc_int_concref<T1,T2> a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_subref>
operator , ( sc_int_concref<T1,T2> a, sc_int_subref b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_base>
operator , ( sc_int_concref<T1,T2> a, sc_int_base& b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_concref<T3,T4> >
concat( sc_int_concref<T1,T2> a, sc_int_concref<T3,T4> b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_concref<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_bitref>
concat( sc_int_concref<T1,T2> a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_subref>
concat( sc_int_concref<T1,T2> a, sc_int_subref b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref<sc_int_concref<T1,T2>,sc_int_base>
concat( sc_int_concref<T1,T2> a, sc_int_base& b )
{
    return sc_int_concref<sc_int_concref<T1,T2>,sc_int_base>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
istream&
operator >> ( istream& is, sc_int_concref<T1,T2>& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref_r
//
//  Proxy class for sc_int bit selection (r-value only).
// ----------------------------------------------------------------------------

// implicit conversion to bool

inline
sc_int_bitref_r::operator bool () const
{
    return m_obj.test( m_index );
}

inline
bool
sc_int_bitref_r::operator ! () const
{
    return ! m_obj.test( m_index );
}

inline
bool
sc_int_bitref_r::operator ~ () const
{
    return ! m_obj.test( m_index );
}


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_bitref_r a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
operator , ( sc_int_bitref_r a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
operator , ( sc_int_bitref_r a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref_r a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref_r a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( bool a, sc_int_bitref_r b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_bitref_r a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
concat( sc_int_bitref_r a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
concat( sc_int_bitref_r a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref_r a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref_r a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( bool a, sc_int_bitref_r b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_bitref_r a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_bitref a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
operator , ( sc_int_bitref_r a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
operator , ( sc_int_bitref a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
operator , ( sc_int_bitref_r a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
operator , ( sc_int_bitref a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref_r a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
operator , ( sc_int_bitref a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( bool a, sc_int_bitref b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_bitref_r a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_bitref a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
concat( sc_int_bitref_r a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>
concat( sc_int_bitref a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
concat( sc_int_bitref_r a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>
concat( sc_int_bitref a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref_r a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_bitref_r,sc_int_base>
concat( sc_int_bitref a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_bitref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( bool a, sc_int_bitref b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	aa, *b.clone(), 3 );
}

#endif


inline
ostream&
operator << ( ostream& os, const sc_int_bitref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_bitref
//
//  Proxy class for sc_int bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_int_bitref& 
sc_int_bitref::operator = ( const sc_int_bitref_r& b )
{
    m_obj.set( m_index, (bool) b );
    m_obj.extend_sign();
    return *this;
}

inline
sc_int_bitref& 
sc_int_bitref::operator = ( const sc_int_bitref& b )
{
    m_obj.set( m_index, (bool) b );
    m_obj.extend_sign();
    return *this;
}

inline
sc_int_bitref& 
sc_int_bitref::operator = ( bool b )
{
    m_obj.set( m_index, b );
    m_obj.extend_sign();
    return *this;
}


inline
sc_int_bitref& 
sc_int_bitref::operator &= ( bool b )
{
    if( ! b ) {
	m_obj.set( m_index, b );
	m_obj.extend_sign();
    }
    return *this;
}

inline
sc_int_bitref& 
sc_int_bitref::operator |= ( bool b )
{
    if( b ) {
	m_obj.set( m_index, b );
	m_obj.extend_sign();
    }
    return *this;
}

inline
sc_int_bitref&
sc_int_bitref::operator ^= ( bool b )
{
    if( b ) {
	m_obj.m_val ^= (UINT_ONE << m_index);
	m_obj.extend_sign();
    }
    return *this;
}


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref<sc_int_bitref,sc_int_concref<T1,T2> >
operator , ( sc_int_bitref a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref<sc_int_bitref,sc_int_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_bitref,sc_int_bitref>
operator , ( sc_int_bitref a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_bitref,sc_int_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_bitref,sc_int_subref>
operator , ( sc_int_bitref a, sc_int_subref b )
{
    return sc_int_concref<sc_int_bitref,sc_int_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_bitref,sc_int_base>
operator , ( sc_int_bitref a, sc_int_base& b )
{
    return sc_int_concref<sc_int_bitref,sc_int_base>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
sc_int_concref<sc_int_bitref,sc_int_concref<T1,T2> >
concat( sc_int_bitref a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref<sc_int_bitref,sc_int_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_bitref,sc_int_bitref>
concat( sc_int_bitref a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_bitref,sc_int_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_bitref,sc_int_subref>
concat( sc_int_bitref a, sc_int_subref b )
{
    return sc_int_concref<sc_int_bitref,sc_int_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_bitref,sc_int_base>
concat( sc_int_bitref a, sc_int_base& b )
{
    return sc_int_concref<sc_int_bitref,sc_int_base>(
	*a.clone(), b, 1 );
}


inline
istream&
operator >> ( istream& is, sc_int_bitref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref_r
//
//  Proxy class for sc_int part selection (r-value only).
// ----------------------------------------------------------------------------

// implicit conversion to int_type

inline
sc_int_subref_r::operator int_type() const 
{
    int_type val = m_obj.m_val;
    int uleft = SC_INTWIDTH - (m_left + 1);
    int uright = uleft + m_right;
    return ( val << uleft >> uright );
}


// reduce methods

inline
bool
sc_int_subref_r::and_reduce() const
{
    sc_int_base a( *this );
    return a.and_reduce();
}

inline
bool
sc_int_subref_r::or_reduce() const
{
    sc_int_base a( *this );
    return a.or_reduce();
}

inline
bool
sc_int_subref_r::xor_reduce() const
{
    sc_int_base a( *this );
    return a.xor_reduce();
}


// explicit conversions

inline
int
sc_int_subref_r::to_int() const
{
    sc_int_base a( *this );
    return a.to_int();
}

inline
unsigned int
sc_int_subref_r::to_uint() const
{
    sc_int_base a( *this );
    return a.to_uint();
}

inline
long
sc_int_subref_r::to_long() const
{
    sc_int_base a( *this );
    return a.to_long();
}

inline
unsigned long
sc_int_subref_r::to_ulong() const
{
    sc_int_base a( *this );
    return a.to_ulong();
}

inline
int64
sc_int_subref_r::to_int64() const
{
    sc_int_base a( *this );
    return a.to_int64();
}

inline
uint64
sc_int_subref_r::to_uint64() const
{
    sc_int_base a( *this );
    return a.to_uint64();
}

inline
double
sc_int_subref_r::to_double() const
{
    sc_int_base a( *this );
    return a.to_double();
}


// explicit conversion to character string

inline
const sc_string
sc_int_subref_r::to_string( sc_numrep numrep ) const
{
    sc_int_base a( *this );
    return a.to_string( numrep );
}

inline
const sc_string
sc_int_subref_r::to_string( sc_numrep numrep, bool w_prefix ) const
{
    sc_int_base a( *this );
    return a.to_string( numrep, w_prefix );
}


// functional notation for the reduce methods

inline
bool
and_reduce( const sc_int_subref_r& a )
{
    return a.and_reduce();
}

inline
bool
nand_reduce( const sc_int_subref_r& a )
{
    return a.nand_reduce();
}

inline
bool
or_reduce( const sc_int_subref_r& a )
{
    return a.or_reduce();
}

inline
bool
nor_reduce( const sc_int_subref_r& a )
{
    return a.nor_reduce();
}

inline
bool
xor_reduce( const sc_int_subref_r& a )
{
    return a.xor_reduce();
}

inline
bool
xnor_reduce( const sc_int_subref_r& a )
{
    return a.xnor_reduce();
}


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_subref_r a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
operator , ( sc_int_subref_r a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
operator , ( sc_int_subref_r a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref_r a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref_r a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( bool a, sc_int_subref_r b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_subref_r a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
concat( sc_int_subref_r a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
concat( sc_int_subref_r a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref_r a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref_r a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( bool a, sc_int_subref_r b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_subref_r a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
operator , ( sc_int_subref a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
operator , ( sc_int_subref_r a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
operator , ( sc_int_subref a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
operator , ( sc_int_subref_r a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
operator , ( sc_int_subref a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref_r a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
operator , ( sc_int_subref a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( bool a, sc_int_subref b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_subref_r a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >
concat( sc_int_subref a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
concat( sc_int_subref_r a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>
concat( sc_int_subref a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
concat( sc_int_subref_r a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>
concat( sc_int_subref a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref_r a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), b, 1 );
}

inline
sc_int_concref_r<sc_int_subref_r,sc_int_base>
concat( sc_int_subref a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_subref_r,sc_int_base>(
	*a.clone(), bb, 3 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( bool a, sc_int_subref b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	aa, *b.clone(), 3 );
}

#endif


inline
ostream&
operator << ( ostream& os, const sc_int_subref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_subref
//
//  Proxy class for sc_int part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_int_subref&
sc_int_subref::operator = ( const sc_int_base& a )
{
    return operator = ( a.operator int_type() );
}

inline
sc_int_subref&
sc_int_subref::operator = ( const char* a )
{
    sc_int_base aa( length() );
    return ( *this = aa = a );
}


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref<sc_int_subref,sc_int_concref<T1,T2> >
operator , ( sc_int_subref a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref<sc_int_subref,sc_int_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_subref,sc_int_bitref>
operator , ( sc_int_subref a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_subref,sc_int_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_subref,sc_int_subref>
operator , ( sc_int_subref a, sc_int_subref b )
{
    return sc_int_concref<sc_int_subref,sc_int_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_subref,sc_int_base>
operator , ( sc_int_subref a, sc_int_base& b )
{
    return sc_int_concref<sc_int_subref,sc_int_base>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
sc_int_concref<sc_int_subref,sc_int_concref<T1,T2> >
concat( sc_int_subref a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref<sc_int_subref,sc_int_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_subref,sc_int_bitref>
concat( sc_int_subref a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_subref,sc_int_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_subref,sc_int_subref>
concat( sc_int_subref a, sc_int_subref b )
{
    return sc_int_concref<sc_int_subref,sc_int_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_int_concref<sc_int_subref,sc_int_base>
concat( sc_int_subref a, sc_int_base& b )
{
    return sc_int_concref<sc_int_subref,sc_int_base>(
	*a.clone(), b, 1 );
}


inline
istream&
operator >> ( istream& is, sc_int_subref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_int_base
//
//  Base class for sc_int.
// ----------------------------------------------------------------------------

// bit selection

inline
sc_int_bitref
sc_int_base::operator [] ( int i )
{
    check_index( i );
    return sc_int_bitref( *this, i ); 
}

inline
sc_int_bitref_r
sc_int_base::operator [] ( int i ) const
{
    check_index( i );
    return sc_int_bitref_r( *this, i );
}


inline
sc_int_bitref
sc_int_base::bit( int i )
{
    check_index( i );
    return sc_int_bitref( *this, i );
}

inline
sc_int_bitref_r
sc_int_base::bit( int i ) const
{
    check_index( i );
    return sc_int_bitref_r( *this, i );
}


// part selection

inline
sc_int_subref
sc_int_base::operator () ( int left, int right )
{
    check_range( left, right );
    return sc_int_subref( *this, left, right );
}

inline
sc_int_subref_r
sc_int_base::operator () ( int left, int right ) const
{
    check_range( left, right );
    return sc_int_subref_r( *this, left, right );
}


inline
sc_int_subref
sc_int_base::range( int left, int right )
{
    check_range( left, right );
    return sc_int_subref( *this, left, right );
}

inline
sc_int_subref_r
sc_int_base::range( int left, int right ) const
{
    check_range( left, right );
    return sc_int_subref_r( *this, left, right );
}


// functional notation for the reduce methods

inline
bool
and_reduce( const sc_int_base& a )
{
    return a.and_reduce();
}

inline
bool
nand_reduce( const sc_int_base& a )
{
    return a.nand_reduce();
}

inline
bool
or_reduce( const sc_int_base& a )
{
    return a.or_reduce();
}

inline
bool
nor_reduce( const sc_int_base& a )
{
    return a.nor_reduce();
}

inline
bool
xor_reduce( const sc_int_base& a )
{
    return a.xor_reduce();
}

inline
bool
xnor_reduce( const sc_int_base& a )
{
    return a.xnor_reduce();
}


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( const sc_int_base& a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( const sc_int_base& a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( const sc_int_base& a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( const sc_int_base& a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, b );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( const sc_int_base& a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, bb, 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( bool a, const sc_int_base& b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	aa, b, 1 );
}


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( const sc_int_base& a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( const sc_int_base& a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( const sc_int_base& a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( const sc_int_base& a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, b );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( const sc_int_base& a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, bb, 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( bool a, const sc_int_base& b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	aa, b, 1 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( const sc_int_base& a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
operator , ( sc_int_base& a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( const sc_int_base& a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
operator , ( sc_int_base& a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( const sc_int_base& a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
operator , ( sc_int_base& a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( const sc_int_base& a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, b );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( sc_int_base& a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, b );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( sc_int_base& a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, bb, 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
operator , ( bool a, sc_int_base& b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	aa, b, 1 );
}


template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( const sc_int_base& a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >
concat( sc_int_base& a, sc_int_concref_r<T1,T2> b )
{
    return sc_int_concref_r<sc_int_base,sc_int_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( const sc_int_base& a, sc_int_bitref b )
{
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_bitref_r>
concat( sc_int_base& a, sc_int_bitref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( const sc_int_base& a, sc_int_subref b )
{
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_subref_r>
concat( sc_int_base& a, sc_int_subref_r b )
{
    return sc_int_concref_r<sc_int_base,sc_int_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( const sc_int_base& a, sc_int_base& b )
{
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, b );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( sc_int_base& a, const sc_int_base& b )
{
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, b );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( sc_int_base& a, bool b )
{
    sc_int_base& bb = *new sc_int_base( 1 );
    bb = (b ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	a, bb, 2 );
}

inline
sc_int_concref_r<sc_int_base,sc_int_base>
concat( bool a, sc_int_base& b )
{
    sc_int_base& aa = *new sc_int_base( 1 );
    aa = (a ? -1 : 0);
    return sc_int_concref_r<sc_int_base,sc_int_base>(
	aa, b, 1 );
}

#endif


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_int_concref<sc_int_base,sc_int_concref<T1,T2> >
operator , ( sc_int_base& a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref<sc_int_base,sc_int_concref<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_int_concref<sc_int_base,sc_int_bitref>
operator , ( sc_int_base& a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_base,sc_int_bitref>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref<sc_int_base,sc_int_subref>
operator , ( sc_int_base& a, sc_int_subref b )
{
    return sc_int_concref<sc_int_base,sc_int_subref>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref<sc_int_base,sc_int_base>
operator , ( sc_int_base& a, sc_int_base& b )
{
    return sc_int_concref<sc_int_base,sc_int_base>(
	a, b );
}


template <class T1, class T2>
inline
sc_int_concref<sc_int_base,sc_int_concref<T1,T2> >
concat( sc_int_base& a, sc_int_concref<T1,T2> b )
{
    return sc_int_concref<sc_int_base,sc_int_concref<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_int_concref<sc_int_base,sc_int_bitref>
concat( sc_int_base& a, sc_int_bitref b )
{
    return sc_int_concref<sc_int_base,sc_int_bitref>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref<sc_int_base,sc_int_subref>
concat( sc_int_base& a, sc_int_subref b )
{
    return sc_int_concref<sc_int_base,sc_int_subref>(
	a, *b.clone(), 2 );
}

inline
sc_int_concref<sc_int_base,sc_int_base>
concat( sc_int_base& a, sc_int_base& b )
{
    return sc_int_concref<sc_int_base,sc_int_base>(
	a, b );
}


inline
ostream&
operator << ( ostream& os, const sc_int_base& a )
{
    a.print( os );
    return os;
}

inline
istream&
operator >> ( istream& is, sc_int_base& a )
{
    a.scan( is );
    return is;
}

} // namespace sc_dt


#endif

// Taf!
