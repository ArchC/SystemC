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

  sc_unsigned.h -- Arbitrary precision unsigned arithmetic.
 
    This file includes the definitions of sc_unsigned_bitref,
    sc_unsigned_subref, and sc_unsigned classes. The first two classes
    are proxy classes to reference one bit and a range of bits of a
    sc_unsigned number, respectively.

    An sc_signed number has the sign-magnitude representation
    internally. However, its interface guarantees a 2's-complement
    representation. The sign-magnitude representation is chosen
    because of its efficiency: The sc_signed and sc_unsigned types are
    optimized for arithmetic rather than bitwise operations. For
    arithmetic operations, the sign-magnitude representation performs
    better.

    It is also important to note that an sc_unsigned number with n
    bits is equivalent to an sc_signed non-negative number with n + 1
    bits.

    The implementations of sc_signed and sc_unsigned classes are
    almost identical: Most of the member and friend functions are
    defined in sc_nbcommon.cpp and sc_nbfriends.cpp so that they can
    be shared by both of these classes. These functions are chosed by
    defining a few macros before including them such as IF_SC_SIGNED
    and CLASS_TYPE. Our implementation choices are mostly dictated by
    performance considerations in that we tried to provide the most
    efficient sc_signed and sc_unsigned types without compromising
    their interface.

    For the behavior of operators, we have two semantics: the old and
    new. The most important difference between these two semantics is
    that the old semantics is closer to C/C++ semantics in that the
    result type of a binary operator on unsigned and signed arguments
    is unsigned; the new semantics, on the other hand, requires the
    result type be signed. The new semantics is required by the VSIA
    C/C++ data types standard. We have implemented the new semantics.

  Original Author: Ali Dasdan, Synopsys, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_UNSIGNED_H
#define SC_UNSIGNED_H


#include "systemc/utils/sc_iostream.h"
#include "systemc/datatypes/int/sc_length_param.h"
#include "systemc/datatypes/int/sc_nbdefs.h"
#include "systemc/datatypes/int/sc_nbutils.h"
#include "systemc/datatypes/int/sc_nbexterns.h"


namespace sc_dt
{

// classes defined in this module
class sc_unsigned_bitref_r;
class sc_unsigned_bitref;
class sc_unsigned_subref_r;
class sc_unsigned_subref;
template <class T1, class T2>
    class sc_unsigned_concref_r; // NOT YET IMPLEMENTED
template <class T1, class T2>
    class sc_unsigned_concref;   // NOT YET IMPLEMENTED
class sc_unsigned;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
class sc_int_base;
class sc_uint_base;
class sc_signed;
class sc_signed_subref_r;
template <class T1, class T2>
    class sc_signed_concref_r; // NOT YET IMPLEMENTED
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;


#define NOT_YET_IMPLEMENTED                                                   \
    SC_REPORT_ERROR( SC_ID_NOT_IMPLEMENTED_,                                  \
                     "sc_unsigned/sc_biguint concatenation" )


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref_r
//
//  Proxy class for sc_unsigned bit selection (r-value only).
// ----------------------------------------------------------------------------

class sc_unsigned_bitref_r
{
    friend class sc_unsigned;

protected:

    // constructor

    sc_unsigned_bitref_r( const sc_unsigned& obj_, int index_ )
	: m_obj( CCAST<sc_unsigned&>( obj_ ) ), m_index( index_ )
	{}

public:

    // copy constructor

    sc_unsigned_bitref_r( const sc_unsigned_bitref_r& a )
	: m_obj( a.m_obj ), m_index( a.m_index )
	{}


    // cloning

    sc_unsigned_bitref_r* clone() const
	{ return new sc_unsigned_bitref_r( *this ); }


    // capacity

    int length() const
	{ return 1; }


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

    sc_unsigned& m_obj;
    int          m_index;

private:

    // disabled
    sc_unsigned_bitref_r();
    sc_unsigned_bitref_r& operator = ( const sc_unsigned_bitref_r& );
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_bitref_r, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_bitref_r, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_bitref_r, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref_r, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref_r, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( bool, sc_unsigned_bitref_r );


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_bitref_r, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_bitref_r, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
concat( sc_unsigned_bitref_r, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref_r, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref_r, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( bool, sc_unsigned_bitref_r );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_bitref_r, sc_unsigned_concref<T1,T2> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_bitref, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_bitref_r, sc_unsigned_bitref );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_bitref, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_bitref_r, sc_unsigned_subref );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_bitref, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref_r, sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( bool, sc_unsigned_bitref );


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_bitref_r, sc_unsigned_concref<T1,T2> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_bitref, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_bitref_r, sc_unsigned_bitref );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_bitref, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
concat( sc_unsigned_bitref_r, sc_unsigned_subref );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
concat( sc_unsigned_bitref, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref_r, sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( bool, sc_unsigned_bitref );

#endif


inline
ostream&
operator << ( ostream&, const sc_unsigned_bitref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref
//
//  Proxy class for sc_unsigned bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_unsigned_bitref
    : public sc_unsigned_bitref_r
{
    friend class sc_unsigned;


    // constructor

    sc_unsigned_bitref( sc_unsigned& obj_, int index_ )
	: sc_unsigned_bitref_r( obj_, index_ )
	{}

public:

    // copy constructor

    sc_unsigned_bitref( const sc_unsigned_bitref& a )
	: sc_unsigned_bitref_r( a )
	{}


    // cloning

    sc_unsigned_bitref* clone() const
	{ return new sc_unsigned_bitref( *this ); }


    // assignment operators

    sc_unsigned_bitref& operator = ( const sc_unsigned_bitref_r& );
    sc_unsigned_bitref& operator = ( const sc_unsigned_bitref& );
    sc_unsigned_bitref& operator = ( bool );

    sc_unsigned_bitref& operator &= ( bool );
    sc_unsigned_bitref& operator |= ( bool );
    sc_unsigned_bitref& operator ^= ( bool );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_unsigned_bitref();
};


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_concref<T1,T2> >
operator , ( sc_unsigned_bitref, sc_unsigned_concref<T1,T2> );

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_bitref>
operator , ( sc_unsigned_bitref, sc_unsigned_bitref );

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_subref>
operator , ( sc_unsigned_bitref, sc_unsigned_subref );

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned>
operator , ( sc_unsigned_bitref, sc_unsigned& );


template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_concref<T1,T2> >
concat( sc_unsigned_bitref, sc_unsigned_concref<T1,T2> );

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_bitref>
concat( sc_unsigned_bitref, sc_unsigned_bitref );

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_subref>
concat( sc_unsigned_bitref, sc_unsigned_subref );

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned>
concat( sc_unsigned_bitref, sc_unsigned& );


inline
istream&
operator >> ( istream&, sc_unsigned_bitref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref_r
//
//  Proxy class for sc_unsigned part selection (r-value only).
// ----------------------------------------------------------------------------

class sc_unsigned_subref_r
{
    friend class sc_unsigned;

protected:

    // constructor

    sc_unsigned_subref_r( const sc_unsigned& obj_, int left_, int right_ )
	: m_obj( CCAST<sc_unsigned&>( obj_ ) ),
	  m_left( left_ ), m_right( right_ )
	{}
  
public:

    // copy constructor

    sc_unsigned_subref_r( const sc_unsigned_subref_r& a )
	: m_obj( a.m_obj ), m_left( a.m_left ), m_right( a.m_right )
	{}


    // cloning

    sc_unsigned_subref_r* clone() const
	{ return new sc_unsigned_subref_r( *this ); }


    // capacity

    int length() const
        { return ( m_left - m_right + 1 ); }


    // implicit conversion to sc_unsigned

    operator sc_unsigned () const;


    // explicit conversions

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

    sc_unsigned& m_obj;
    int          m_left;
    int          m_right;

private:

    // disabled
    sc_unsigned_subref_r();
    sc_unsigned_subref_r& operator = ( const sc_unsigned_subref_r& );
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_subref_r, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_subref_r, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_subref_r, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref_r, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref_r, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( bool, sc_unsigned_subref_r );


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_subref_r, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_subref_r, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
concat( sc_unsigned_subref_r, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref_r, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref_r, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( bool, sc_unsigned_subref_r );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_subref_r, sc_unsigned_concref<T1,T2> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_subref, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_subref_r, sc_unsigned_bitref );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_subref, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_subref_r, sc_unsigned_subref );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_subref, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref_r, sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( bool, sc_unsigned_subref );


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_subref_r, sc_unsigned_concref<T1,T2> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_subref, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_subref_r, sc_unsigned_bitref );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_subref, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
concat( sc_unsigned_subref_r, sc_unsigned_subref );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
concat( sc_unsigned_subref, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref_r, sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( bool, sc_unsigned_subref );

#endif


inline
ostream&
operator << ( ostream&, const sc_unsigned_subref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref
//
//  Proxy class for sc_unsigned part selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_unsigned_subref
    : public sc_unsigned_subref_r
{
    friend class sc_unsigned;


    // constructor

    sc_unsigned_subref( sc_unsigned& obj_, int left_, int right_ )
	: sc_unsigned_subref_r( obj_, left_, right_ )
	{}
  
public:

    // copy constructor

    sc_unsigned_subref( const sc_unsigned_subref& a )
	: sc_unsigned_subref_r( a )
	{}


    // cloning

    sc_unsigned_subref* clone() const
	{ return new sc_unsigned_subref( *this ); }


    // assignment operators

    sc_unsigned_subref& operator = ( const sc_unsigned_subref_r& a );
    sc_unsigned_subref& operator = ( const sc_unsigned_subref& a );
    sc_unsigned_subref& operator = ( const sc_unsigned& a );

    template <class T1, class T2>
    sc_unsigned_subref& operator = ( const sc_unsigned_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_unsigned_subref& operator = ( const sc_signed_subref_r& a );
    sc_unsigned_subref& operator = ( const sc_signed& a );

    template <class T1, class T2>
    sc_unsigned_subref& operator = ( const sc_signed_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_unsigned_subref& operator = ( const char* a );
    sc_unsigned_subref& operator = ( unsigned long a );
    sc_unsigned_subref& operator = ( long a );

    sc_unsigned_subref& operator = ( unsigned int a )
	{ return operator = ( (unsigned long) a ); }

    sc_unsigned_subref& operator = ( int a )
	{ return operator = ( (long) a ); }

    sc_unsigned_subref& operator = ( uint64 a );
    sc_unsigned_subref& operator = ( int64 a );
    sc_unsigned_subref& operator = ( double a );  
    sc_unsigned_subref& operator = ( const sc_int_base& a );
    sc_unsigned_subref& operator = ( const sc_uint_base& a );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_unsigned_subref();
};


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_concref<T1,T2> >
operator , ( sc_unsigned_subref, sc_unsigned_concref<T1,T2> );

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_bitref>
operator , ( sc_unsigned_subref, sc_unsigned_bitref );

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_subref>
operator , ( sc_unsigned_subref, sc_unsigned_subref );

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned>
operator , ( sc_unsigned_subref, sc_unsigned& );


template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_concref<T1,T2> >
concat( sc_unsigned_subref, sc_unsigned_concref<T1,T2> );

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_bitref>
concat( sc_unsigned_subref, sc_unsigned_bitref );

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_subref>
concat( sc_unsigned_subref, sc_unsigned_subref );

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned>
concat( sc_unsigned_subref, sc_unsigned& );


inline
istream&
operator >> ( istream&, sc_unsigned_subref& );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_unsigned_concref_r<T1,T2>
//
//  Proxy class for sc_unsigned concatenation (r-value only).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_unsigned_concref_r
{
public:

    // constructor

    sc_unsigned_concref_r( const T1& left_, const T2& right_, int delete_ = 0 )
	: m_left( CCAST<T1&>( left_ ) ), m_right( CCAST<T2&>( right_ ) ),
	  m_delete( delete_ ), m_refs( *new int( 1 ) )
	{ m_len = m_left.length() + m_right.length(); NOT_YET_IMPLEMENTED; }


    // copy constructor

    sc_unsigned_concref_r( const sc_unsigned_concref_r<T1,T2>& a )
	: m_left( a.m_left ), m_right( a.m_right ), m_len( a.m_len ),
	  m_delete( a.m_delete ), m_refs( a.m_refs )
	{ ++ m_refs; NOT_YET_IMPLEMENTED; }


    // destructor

    ~sc_unsigned_concref_r();


    // cloning

    sc_unsigned_concref_r<T1,T2>* clone() const
	{ return new sc_unsigned_concref_r<T1,T2>( *this ); }


    // capacity

    int length() const
	{ return m_len; }


    // implicit conversion to sc_unsigned

    // NOT YET IMPLEMENTED


    // explicit conversions

    // NOT YET IMPLEMENTED


    // explicit conversion to character string

    // NOT YET IMPLEMENTED


    // other methods

    void print( ostream& os = cout ) const
	{ NOT_YET_IMPLEMENTED; }

protected:

    T1&          m_left;
    T2&          m_right;
    int          m_len;
    mutable int  m_delete;
    mutable int& m_refs;

private:

    // disabled
    sc_unsigned_concref_r();
    sc_unsigned_concref_r<T1,T2>& operator = (
	const sc_unsigned_concref_r<T1,T2>& );
};


// r-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned_bitref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned_subref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref_r<T1,T2>, const sc_unsigned& );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref_r<T1,T2>, bool );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( bool, sc_unsigned_concref_r<T1,T2> );


template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned_bitref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned_subref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref_r<T1,T2>, const sc_unsigned& );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref_r<T1,T2>, bool );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( bool, sc_unsigned_concref_r<T1,T2> );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned_concref<T3,T4> );

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned_bitref );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned_bitref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned_subref );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned_subref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref_r<T1,T2>, sc_unsigned& );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref<T1,T2>, const sc_unsigned& );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref<T1,T2>, bool );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( bool, sc_unsigned_concref<T1,T2> );


template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned_concref<T3,T4> );

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
concat( sc_unsigned_concref<T1,T2>, sc_unsigned_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned_bitref );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
concat( sc_unsigned_concref<T1,T2>, sc_unsigned_bitref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned_subref );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
concat( sc_unsigned_concref<T1,T2>, sc_unsigned_subref_r );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref_r<T1,T2>, sc_unsigned& );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref<T1,T2>, const sc_unsigned& );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref<T1,T2>, bool );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( bool, sc_unsigned_concref<T1,T2> );

#endif


template <class T1, class T2>
inline
ostream&
operator << ( ostream&, const sc_unsigned_concref_r<T1,T2>& );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_unsigned_concref<T1,T2>
//
//  Proxy class for sc_unsigned concatenation (r-value and l-value).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_unsigned_concref
    : public sc_unsigned_concref_r<T1,T2>
{
public:

    // constructor

    sc_unsigned_concref( T1& left_, T2& right_, int delete_ = 0 )
	: sc_unsigned_concref_r<T1,T2>( left_, right_, delete_ )
	{}


    // copy constructor

    sc_unsigned_concref( const sc_unsigned_concref<T1,T2>& a )
	: sc_unsigned_concref_r<T1,T2>( a )
	{}


    // cloning

    sc_unsigned_concref<T1,T2>* clone() const
	{ return new sc_unsigned_concref<T1,T2>( *this ); }


    // assignment operators

    // NOT YET IMPLEMENTED


    // other methods

    void scan( istream& is = cin )
	{ NOT_YET_IMPLEMENTED; }

private:

    // disabled
    sc_unsigned_concref();
};


// l-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_concref<T3,T4> >
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned_concref<T3,T4> );

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_bitref>
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned_bitref );

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_subref>
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned_subref );

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref<T1,T2>, sc_unsigned& );


template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_concref<T3,T4> >
concat( sc_unsigned_concref<T1,T2>, sc_unsigned_concref<T3,T4> );

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_bitref>
concat( sc_unsigned_concref<T1,T2>, sc_unsigned_bitref );

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_subref>
concat( sc_unsigned_concref<T1,T2>, sc_unsigned_subref );

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref<T1,T2>, sc_unsigned& );


template <class T1, class T2>
inline
istream&
operator >> ( istream&, sc_unsigned_concref<T1,T2>& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
//
//  Arbitrary precision unsigned number.
// ----------------------------------------------------------------------------

class sc_unsigned
{
    friend class sc_unsigned_bitref_r;
    friend class sc_unsigned_bitref;
    friend class sc_unsigned_subref_r;
    friend class sc_unsigned_subref;
    friend class sc_signed;
    friend class sc_signed_subref;

  // Needed for types using sc_unsigned.
  typedef bool elemtype;

public:

    // constructors

    explicit sc_unsigned( int nb = sc_length_param().len() );
    sc_unsigned( const sc_unsigned& v );
    sc_unsigned( const sc_signed&   v );


    // assignment operators

    sc_unsigned& operator = (const sc_unsigned&        v);
    sc_unsigned& operator = (const sc_unsigned_subref_r& a );

    template <class T1, class T2>
    sc_unsigned& operator = ( const sc_unsigned_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_unsigned& operator = (const sc_signed&          v);
    sc_unsigned& operator = (const sc_signed_subref_r& a );

    template <class T1, class T2>
    sc_unsigned& operator = ( const sc_signed_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_unsigned& operator = ( const char*               v);
    sc_unsigned& operator = ( int64                     v);
    sc_unsigned& operator = ( uint64                    v);
    sc_unsigned& operator = ( long                      v);
    sc_unsigned& operator = ( unsigned long             v);

    sc_unsigned& operator = ( int                       v) 
	{ return operator=((long) v); }

    sc_unsigned& operator = ( unsigned int              v) 
	{ return operator=((unsigned long) v); }

    sc_unsigned& operator = ( double                    v);
    sc_unsigned& operator = ( const sc_int_base&        v);
    sc_unsigned& operator = ( const sc_uint_base&       v);

    sc_unsigned& operator = ( const sc_bv_base& );
    sc_unsigned& operator = ( const sc_lv_base& );

#ifdef SC_INCLUDE_FX
    sc_unsigned& operator = ( const sc_fxval& );
    sc_unsigned& operator = ( const sc_fxval_fast& );
    sc_unsigned& operator = ( const sc_fxnum& );
    sc_unsigned& operator = ( const sc_fxnum_fast& );
#endif


    // destructor

    ~sc_unsigned()
	{
#ifndef SC_MAX_NBITS
	    delete [] digit;
#endif
	}


  // Increment operators.
  sc_unsigned& operator ++ ();
  const sc_unsigned operator ++ (int);

  // Decrement operators.
  sc_unsigned& operator -- ();
  const sc_unsigned operator -- (int);


    // bit selection

    sc_unsigned_bitref operator [] ( int i )
	{ return sc_unsigned_bitref( *this, i ); }

    sc_unsigned_bitref_r operator [] ( int i ) const
	{ return sc_unsigned_bitref_r( *this, i ); }

    sc_unsigned_bitref bit( int i )
	{ return sc_unsigned_bitref( *this, i ); }

    sc_unsigned_bitref_r bit( int i ) const
	{ return sc_unsigned_bitref_r( *this, i ); }


    // part selection

    // Subref operators. Help access the range of bits from the ith to
    // jth. These indices have arbitrary precedence with respect to each
    // other, i.e., we can have i <= j or i > j. Note the equivalence
    // between range(i, j) and operator (i, j). Also note that
    // operator (i, i) returns an unsigned number that corresponds to the
    // bit operator [i], so these two forms are not the same.

    sc_unsigned_subref range( int i, int j )
	{ return sc_unsigned_subref( *this, i, j ); }

    sc_unsigned_subref_r range( int i, int j ) const
	{ return sc_unsigned_subref_r( *this, i, j ); }

    sc_unsigned_subref operator () ( int i, int j ) 
	{ return sc_unsigned_subref( *this, i, j ); }

    sc_unsigned_subref_r operator () ( int i, int j ) const
	{ return sc_unsigned_subref_r( *this, i, j ); }


    // explicit conversions

    int           to_int() const;
    unsigned int  to_uint() const;
    long          to_long() const;
    unsigned long to_ulong() const;
    int64         to_int64() const;
    uint64        to_uint64() const;
    double        to_double() const;

#ifdef SC_DT_DEPRECATED
    int to_signed() const
	{ return to_int(); }

    unsigned int to_unsigned() const
	{ return to_uint(); }
#endif

    // explicit conversion to character string

    const sc_string to_string( sc_numrep numrep = SC_DEC ) const;
    const sc_string to_string( sc_numrep numrep, bool w_prefix ) const;


    // Print functions. dump prints the internals of the class.

    void print( ostream& os = cout ) const
	{ os << to_string(); }

    void scan( istream& is = cin );

    void dump( ostream& os = cout ) const;


  // Functions to find various properties.    
  int  length() const { return nbits - 1; }  // Bit width.
  bool iszero() const;                       // Is the number zero?
  bool sign() const { return 0; }            // Sign.

  // Functions to access individual bits.  
  bool test(int i) const;      // Is the ith bit 0 or 1?
  void set(int i);             // Set the ith bit to 1.
  void clear(int i);           // Set the ith bit to 0.
  void set(int i, bool v)      // Set the ith bit to v.
    { if (v) set(i); else clear(i);  }
  void invert(int i)           // Negate the ith bit.
    { if (test(i)) clear(i); else set(i);  }

  // Make the number equal to its mirror image.
  void reverse();

  // Get/set a packed bit representation of the number.
  void get_packed_rep(unsigned long *buf) const;
  void set_packed_rep(unsigned long *buf);

  /*
    The comparison of the old and new semantics are as follows:

    Let s = sc_signed, 
        u = sc_unsigned,
        un = { uint64, unsigned long, unsigned int },
        sn = { int64, long, int, char* }, and
        OP = { +, -, *, /, % }.

    Old semantics:                     New semantics:
      u OP u -> u                        u OP u -> u
      s OP u -> u                        s OP u -> s
      u OP s -> u                        u OP s -> s
      s OP s -> s                        s OP s -> s

      u OP un = un OP u -> u             u OP un = un OP u -> u
      u OP sn = sn OP u -> u             u OP sn = sn OP u -> s

      s OP un = un OP s -> s             s OP un = un OP s -> s
      s OP sn = sn OP s -> s             s OP sn = sn OP s -> s

    In the new semantics, the result is u if both operands are u; the
    result is s otherwise. The only exception is subtraction. The result
    of a subtraction is always s.

    The old semantics is like C/C++ semantics on integer types; the
    new semantics is due to the VSIA C/C++ data types standard.  
   */

  // ARITHMETIC OPERATORS:

  // ADDition operators:
   
  friend   sc_signed operator + (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator + (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator + (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator + (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator + (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned int        v) 
    { return operator+(u, (unsigned long) v); }

  friend   sc_signed operator + (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator + (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator + (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator + (unsigned int        u, const sc_unsigned&  v)  
    { return operator+((unsigned long) u,  v); } 

  sc_unsigned& operator += (const sc_signed&    v); 
  sc_unsigned& operator += (const sc_unsigned&  v); 
  sc_unsigned& operator += (int64               v); 
  sc_unsigned& operator += (uint64              v); 
  sc_unsigned& operator += (long                v); 
  sc_unsigned& operator += (unsigned long       v); 
  sc_unsigned& operator += (int                 v) 
    { return operator+=((long) v); }
  sc_unsigned& operator += (unsigned int        v) 
    { return operator+=((unsigned long) v); }

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator + (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator + (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator + (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator += (const sc_int_base&  v);
  sc_unsigned& operator += (const sc_uint_base& v);

  // SUBtraction operators:
   
  friend   sc_signed operator - (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator - (const sc_signed&    u, const sc_unsigned&  v); 

  friend   sc_signed operator - (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator - (const sc_unsigned&  u, int64               v); 
  friend   sc_signed operator - (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator - (const sc_unsigned&  u, long                v); 
  friend   sc_signed operator - (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator - (const sc_unsigned&  u, int                 v); 
  friend   sc_signed operator - (const sc_unsigned&  u, unsigned int        v);

  friend   sc_signed operator - (int64               u, const sc_unsigned&  v); 
  friend   sc_signed operator - (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator - (long                u, const sc_unsigned&  v); 
  friend   sc_signed operator - (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator - (int                 u, const sc_unsigned&  v);
  friend   sc_signed operator - (unsigned int        u, const sc_unsigned&  v); 

  sc_unsigned& operator -= (const sc_signed&    v); 
  sc_unsigned& operator -= (const sc_unsigned&  v); 
  sc_unsigned& operator -= (int64               v); 
  sc_unsigned& operator -= (uint64              v); 
  sc_unsigned& operator -= (long                v); 
  sc_unsigned& operator -= (unsigned long       v); 
  sc_unsigned& operator -= (int                 v) 
    { return operator-=((long) v); }
  sc_unsigned& operator -= (unsigned int        v) 
    { return operator-=((unsigned long) v); }

  friend   sc_signed operator - (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator - (const sc_unsigned&  u, const sc_int_base&  v);
  friend   sc_signed operator - (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator - (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator -= (const sc_int_base&  v);
  sc_unsigned& operator -= (const sc_uint_base& v);

  // MULtiplication operators:
   
  friend   sc_signed operator * (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator * (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator * (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator * (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator * (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned int        v) 
    { return operator*(u, (unsigned long) v); }

  friend   sc_signed operator * (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator * (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator * (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator * (unsigned int        u, const sc_unsigned&  v)  
    { return operator*((unsigned long) u,  v); } 

  sc_unsigned& operator *= (const sc_signed&    v); 
  sc_unsigned& operator *= (const sc_unsigned&  v); 
  sc_unsigned& operator *= (int64               v); 
  sc_unsigned& operator *= (uint64              v); 
  sc_unsigned& operator *= (long                v); 
  sc_unsigned& operator *= (unsigned long       v); 
  sc_unsigned& operator *= (int                 v) 
    { return operator*=((long) v); }
  sc_unsigned& operator *= (unsigned int        v) 
    { return operator*=((unsigned long) v); }

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator * (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator * (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator * (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator *= (const sc_int_base&  v);
  sc_unsigned& operator *= (const sc_uint_base& v);

  // DIVision operators:
   
  friend   sc_signed operator / (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator / (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator / (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator / (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator / (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned int        v) 
    { return operator/(u, (unsigned long) v); }

  friend   sc_signed operator / (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator / (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator / (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator / (unsigned int        u, const sc_unsigned&  v)  
    { return operator/((unsigned long) u,  v); } 

  sc_unsigned& operator /= (const sc_signed&    v); 
  sc_unsigned& operator /= (const sc_unsigned&  v); 
  sc_unsigned& operator /= (int64               v); 
  sc_unsigned& operator /= (uint64              v); 
  sc_unsigned& operator /= (long                v); 
  sc_unsigned& operator /= (unsigned long       v); 
  sc_unsigned& operator /= (int                 v) 
    { return operator/=((long) v); }
  sc_unsigned& operator /= (unsigned int        v) 
    { return operator/=((unsigned long) v); }

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator / (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator / (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator / (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator /= (const sc_int_base&  v);
  sc_unsigned& operator /= (const sc_uint_base& v);

  // MODulo operators:
   
  friend   sc_signed operator % (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator % (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator % (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator % (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator % (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned int        v) 
    { return operator%(u, (unsigned long) v); }

  friend   sc_signed operator % (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator % (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator % (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator % (unsigned int        u, const sc_unsigned&  v)  
    { return operator%((unsigned long) u,  v); } 

  sc_unsigned& operator %= (const sc_signed&    v); 
  sc_unsigned& operator %= (const sc_unsigned&  v); 
  sc_unsigned& operator %= (int64               v); 
  sc_unsigned& operator %= (uint64              v); 
  sc_unsigned& operator %= (long                v); 
  sc_unsigned& operator %= (unsigned long       v); 
  sc_unsigned& operator %= (int                 v) 
    { return operator%=((long) v); }
  sc_unsigned& operator %= (unsigned int        v) 
    { return operator%=((unsigned long) v); }

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator % (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator % (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator % (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator %= (const sc_int_base&  v);
  sc_unsigned& operator %= (const sc_uint_base& v);

  // BITWISE OPERATORS:

  // Bitwise AND operators:
   
  friend   sc_signed operator & (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator & (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator & (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator & (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator & (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned int        v) 
    { return operator&(u, (unsigned long) v); }

  friend   sc_signed operator & (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator & (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator & (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator & (unsigned int        u, const sc_unsigned&  v)  
    { return operator&((unsigned long) u,  v); } 

  sc_unsigned& operator &= (const sc_signed&    v); 
  sc_unsigned& operator &= (const sc_unsigned&  v); 
  sc_unsigned& operator &= (int64               v); 
  sc_unsigned& operator &= (uint64              v); 
  sc_unsigned& operator &= (long                v); 
  sc_unsigned& operator &= (unsigned long       v); 
  sc_unsigned& operator &= (int                 v) 
    { return operator&=((long) v); }
  sc_unsigned& operator &= (unsigned int        v) 
    { return operator&=((unsigned long) v); }

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator & (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator & (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator & (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator &= (const sc_int_base&  v);
  sc_unsigned& operator &= (const sc_uint_base& v);

  // Bitwise OR operators:
   
  friend   sc_signed operator | (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator | (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator | (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator | (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator | (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned int        v) 
    { return operator|(u, (unsigned long) v); }

  friend   sc_signed operator | (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator | (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator | (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator | (unsigned int        u, const sc_unsigned&  v)  
    { return operator|((unsigned long) u,  v); } 

  sc_unsigned& operator |= (const sc_signed&    v); 
  sc_unsigned& operator |= (const sc_unsigned&  v); 
  sc_unsigned& operator |= (int64               v); 
  sc_unsigned& operator |= (uint64              v); 
  sc_unsigned& operator |= (long                v); 
  sc_unsigned& operator |= (unsigned long       v); 
  sc_unsigned& operator |= (int                 v) 
    { return operator|=((long) v); }
  sc_unsigned& operator |= (unsigned int        v) 
    { return operator|=((unsigned long) v); }

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator | (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator | (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator | (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator |= (const sc_int_base&  v);
  sc_unsigned& operator |= (const sc_uint_base& v);

  // Bitwise XOR operators:
   
  friend   sc_signed operator ^ (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator ^ (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator ^ (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned int        v) 
    { return operator^(u, (unsigned long) v); }

  friend   sc_signed operator ^ (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator ^ (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator ^ (unsigned int        u, const sc_unsigned&  v)  
    { return operator^((unsigned long) u,  v); } 

  sc_unsigned& operator ^= (const sc_signed&    v); 
  sc_unsigned& operator ^= (const sc_unsigned&  v); 
  sc_unsigned& operator ^= (int64               v); 
  sc_unsigned& operator ^= (uint64              v); 
  sc_unsigned& operator ^= (long                v); 
  sc_unsigned& operator ^= (unsigned long       v); 
  sc_unsigned& operator ^= (int                 v) 
    { return operator^=((long) v); }
  sc_unsigned& operator ^= (unsigned int        v) 
    { return operator^=((unsigned long) v); }

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator ^ (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator ^= (const sc_int_base&  v);
  sc_unsigned& operator ^= (const sc_uint_base& v);

  // SHIFT OPERATORS:

  // LEFT SHIFT operators:
   
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator << (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int                 v) 
    { return operator<<(u, (long) v); }
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned int        v) 
    { return operator<<(u, (unsigned long) v); }

  sc_unsigned& operator <<= (const sc_signed&    v); 
  sc_unsigned& operator <<= (const sc_unsigned&  v); 
  sc_unsigned& operator <<= (int64               v); 
  sc_unsigned& operator <<= (uint64              v); 
  sc_unsigned& operator <<= (long                v); 
  sc_unsigned& operator <<= (unsigned long       v); 
  sc_unsigned& operator <<= (int                 v) 
    { return operator<<=((long) v); }
  sc_unsigned& operator <<= (unsigned int        v) 
    { return operator<<=((unsigned long) v); }

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_uint_base& v);
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned& operator <<= (const sc_int_base&  v);
  sc_unsigned& operator <<= (const sc_uint_base& v);

  // RIGHT SHIFT operators:
   
  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator >> (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int                 v) 
    { return operator>>(u, (long) v); }
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned int        v) 
    { return operator>>(u, (unsigned long) v); }

  sc_unsigned& operator >>= (const sc_signed&    v); 
  sc_unsigned& operator >>= (const sc_unsigned&  v); 
  sc_unsigned& operator >>= (int64               v); 
  sc_unsigned& operator >>= (uint64              v); 
  sc_unsigned& operator >>= (long                v); 
  sc_unsigned& operator >>= (unsigned long       v); 
  sc_unsigned& operator >>= (int                 v) 
    { return operator>>=((long) v); }
  sc_unsigned& operator >>= (unsigned int        v) 
    { return operator>>=((unsigned long) v); }

  friend sc_unsigned operator >> ( const sc_unsigned& , const sc_uint_base& );
  friend sc_unsigned operator >> ( const sc_unsigned&, const sc_int_base& );
  sc_unsigned& operator >>= (const sc_int_base&  v);
  sc_unsigned& operator >>= (const sc_uint_base& v);

  // Unary arithmetic operators
  friend sc_unsigned operator + (const sc_unsigned& u);
  friend   sc_signed operator - (const sc_unsigned& u);

  // LOGICAL OPERATORS:

  // Logical EQUAL operators:
   
  friend bool operator == (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator == (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator == (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator == (const sc_unsigned&  u, int64               v); 
  friend bool operator == (const sc_unsigned&  u, uint64              v); 
  friend bool operator == (const sc_unsigned&  u, long                v); 
  friend bool operator == (const sc_unsigned&  u, unsigned long       v);
  friend bool operator == (const sc_unsigned&  u, int                 v) 
    { return operator==(u, (long) v); }
  friend bool operator == (const sc_unsigned&  u, unsigned int        v) 
    { return operator==(u, (unsigned long) v); }

  friend bool operator == (int64               u, const sc_unsigned&  v); 
  friend bool operator == (uint64              u, const sc_unsigned&  v); 
  friend bool operator == (long                u, const sc_unsigned&  v); 
  friend bool operator == (unsigned long       u, const sc_unsigned&  v);
  friend bool operator == (int                 u, const sc_unsigned&  v)  
    { return operator==((long) u,  v); } 
  friend bool operator == (unsigned int        u, const sc_unsigned&  v)  
    { return operator==((unsigned long) u,  v); } 

  friend bool operator == (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator == (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator == (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator == (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical NOT_EQUAL operators:
   
  friend bool operator != (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator != (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator != (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator != (const sc_unsigned&  u, int64               v); 
  friend bool operator != (const sc_unsigned&  u, uint64              v); 
  friend bool operator != (const sc_unsigned&  u, long                v); 
  friend bool operator != (const sc_unsigned&  u, unsigned long       v);
  friend bool operator != (const sc_unsigned&  u, int                 v) 
    { return operator!=(u, (long) v); }
  friend bool operator != (const sc_unsigned&  u, unsigned int        v) 
    { return operator!=(u, (unsigned long) v); }

  friend bool operator != (int64               u, const sc_unsigned&  v); 
  friend bool operator != (uint64              u, const sc_unsigned&  v); 
  friend bool operator != (long                u, const sc_unsigned&  v); 
  friend bool operator != (unsigned long       u, const sc_unsigned&  v);
  friend bool operator != (int                 u, const sc_unsigned&  v)  
    { return operator!=((long) u,  v); } 
  friend bool operator != (unsigned int        u, const sc_unsigned&  v)  
    { return operator!=((unsigned long) u,  v); } 

  friend bool operator != (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator != (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator != (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator != (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN operators:
   
  friend bool operator < (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator < (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator < (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator < (const sc_unsigned&  u, int64               v); 
  friend bool operator < (const sc_unsigned&  u, uint64              v); 
  friend bool operator < (const sc_unsigned&  u, long                v); 
  friend bool operator < (const sc_unsigned&  u, unsigned long       v);
  friend bool operator < (const sc_unsigned&  u, int                 v) 
    { return operator<(u, (long) v); }
  friend bool operator < (const sc_unsigned&  u, unsigned int        v) 
    { return operator<(u, (unsigned long) v); }

  friend bool operator < (int64               u, const sc_unsigned&  v); 
  friend bool operator < (uint64              u, const sc_unsigned&  v); 
  friend bool operator < (long                u, const sc_unsigned&  v); 
  friend bool operator < (unsigned long       u, const sc_unsigned&  v);
  friend bool operator < (int                 u, const sc_unsigned&  v)  
    { return operator<((long) u,  v); } 
  friend bool operator < (unsigned int        u, const sc_unsigned&  v)  
    { return operator<((unsigned long) u,  v); } 

  friend bool operator < (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator < (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator < (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator < (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN_AND_EQUAL operators:
   
  friend bool operator <= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator <= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator <= (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator <= (const sc_unsigned&  u, int64               v); 
  friend bool operator <= (const sc_unsigned&  u, uint64              v); 
  friend bool operator <= (const sc_unsigned&  u, long                v); 
  friend bool operator <= (const sc_unsigned&  u, unsigned long       v);
  friend bool operator <= (const sc_unsigned&  u, int                 v) 
    { return operator<=(u, (long) v); }
  friend bool operator <= (const sc_unsigned&  u, unsigned int        v) 
    { return operator<=(u, (unsigned long) v); }

  friend bool operator <= (int64               u, const sc_unsigned&  v); 
  friend bool operator <= (uint64              u, const sc_unsigned&  v); 
  friend bool operator <= (long                u, const sc_unsigned&  v); 
  friend bool operator <= (unsigned long       u, const sc_unsigned&  v);
  friend bool operator <= (int                 u, const sc_unsigned&  v)  
    { return operator<=((long) u,  v); } 
  friend bool operator <= (unsigned int        u, const sc_unsigned&  v)  
    { return operator<=((unsigned long) u,  v); } 

  friend bool operator <= (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator <= (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator <= (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator <= (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN operators:
   
  friend bool operator > (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator > (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator > (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator > (const sc_unsigned&  u, int64               v); 
  friend bool operator > (const sc_unsigned&  u, uint64              v); 
  friend bool operator > (const sc_unsigned&  u, long                v); 
  friend bool operator > (const sc_unsigned&  u, unsigned long       v);
  friend bool operator > (const sc_unsigned&  u, int                 v) 
    { return operator>(u, (long) v); }
  friend bool operator > (const sc_unsigned&  u, unsigned int        v) 
    { return operator>(u, (unsigned long) v); }

  friend bool operator > (int64               u, const sc_unsigned&  v); 
  friend bool operator > (uint64              u, const sc_unsigned&  v); 
  friend bool operator > (long                u, const sc_unsigned&  v); 
  friend bool operator > (unsigned long       u, const sc_unsigned&  v);
  friend bool operator > (int                 u, const sc_unsigned&  v)  
    { return operator>((long) u,  v); } 
  friend bool operator > (unsigned int        u, const sc_unsigned&  v)  
    { return operator>((unsigned long) u,  v); } 

  friend bool operator > (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator > (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator > (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator > (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN_AND_EQUAL operators:
   
  friend bool operator >= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator >= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator >= (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator >= (const sc_unsigned&  u, int64               v); 
  friend bool operator >= (const sc_unsigned&  u, uint64              v); 
  friend bool operator >= (const sc_unsigned&  u, long                v); 
  friend bool operator >= (const sc_unsigned&  u, unsigned long       v);
  friend bool operator >= (const sc_unsigned&  u, int                 v) 
    { return operator>=(u, (long) v); }
  friend bool operator >= (const sc_unsigned&  u, unsigned int        v) 
    { return operator>=(u, (unsigned long) v); }

  friend bool operator >= (int64               u, const sc_unsigned&  v); 
  friend bool operator >= (uint64              u, const sc_unsigned&  v); 
  friend bool operator >= (long                u, const sc_unsigned&  v); 
  friend bool operator >= (unsigned long       u, const sc_unsigned&  v);
  friend bool operator >= (int                 u, const sc_unsigned&  v)  
    { return operator>=((long) u,  v); } 
  friend bool operator >= (unsigned int        u, const sc_unsigned&  v)  
    { return operator>=((unsigned long) u,  v); } 

  friend bool operator >= (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator >= (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator >= (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator >= (const sc_int_base&  u, const sc_unsigned&  v);

  // Bitwise NOT operator (unary).
  friend sc_unsigned operator ~ (const sc_unsigned& u); 

  // Helper functions.
  friend int compare_unsigned(small_type us, 
                              int unb,
                              int und, 
                              const unsigned long *ud, 
                              small_type vs, 
                              int vnb,
                              int vnd,
                              const unsigned long *vd,
                              small_type if_u_signed = 0,
                              small_type if_v_signed = 0);
  
  friend sc_unsigned add_unsigned_friend(small_type us, 
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned sub_unsigned_friend(small_type us, 
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs, 
                                         int vnb,
                                         int vnd, 
                                         const unsigned long *vd);
  
  friend sc_unsigned mul_unsigned_friend(small_type s,
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned div_unsigned_friend(small_type s,
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned mod_unsigned_friend(small_type us,
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned and_unsigned_friend(small_type us, 
                                         int unb, 
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs,
                                         int vnb, 
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned or_unsigned_friend(small_type us, 
                                        int unb, 
                                        int und, 
                                        const unsigned long *ud, 
                                        small_type vs,
                                        int vnb, 
                                        int vnd,
                                        const unsigned long *vd);
  
  friend sc_unsigned xor_unsigned_friend(small_type us, 
                                         int unb, 
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs,
                                         int vnb, 
                                         int vnd,
                                         const unsigned long *vd);
  
private:

  small_type  sgn;         // Shortened as s.
  int nbits;       // Shortened as nb.
  int ndigits;     // Shortened as nd.

#ifdef SC_MAX_NBITS
  unsigned long digit[DIV_CEIL(SC_MAX_NBITS)];   // Shortened as d.
#else
  unsigned long *digit;                       // Shortened as d.
#endif

  // Private constructors:

  // Create a copy of v with sign s.
  sc_unsigned(const sc_unsigned& v, small_type s);
  sc_unsigned(const sc_signed&   v, small_type s);

  // Create an unsigned number with the given attributes.
  sc_unsigned(small_type s, int nb, int nd, 
              unsigned long *d, bool alloc = true);

  // Create an unsigned number using the bits u[l..r].
  sc_unsigned(const sc_unsigned* u, int l, int r);

  // Private member functions. The called functions are inline functions.

  small_type default_sign() const
    { return SC_POS; }

  int num_bits(int nb) const { return nb + 1; }

  bool check_if_outside(int bit_num) const;

  void copy_digits(int nb, int nd, const unsigned long *d)
    { copy_digits_unsigned(sgn, nbits, ndigits, digit, nb, nd, d); }
  
  void makezero()
    { sgn = make_zero(ndigits, digit); }

  // Conversion functions between 2's complement (2C) and
  // sign-magnitude (SM):
  void convert_2C_to_SM()
    { sgn = convert_unsigned_2C_to_SM(nbits, ndigits, digit); }

  void convert_SM_to_2C_to_SM()
    { sgn = convert_unsigned_SM_to_2C_to_SM(sgn, nbits, ndigits, digit); }

  void convert_SM_to_2C()
    { convert_unsigned_SM_to_2C(sgn, ndigits, digit); }

};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( const sc_unsigned&, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( const sc_unsigned&, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( const sc_unsigned&, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( const sc_unsigned&, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( const sc_unsigned&, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( bool, const sc_unsigned& );


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( const sc_unsigned&, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( const sc_unsigned&, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( const sc_unsigned&, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( const sc_unsigned&, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( const sc_unsigned&, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( bool, const sc_unsigned& );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( const sc_unsigned&, sc_unsigned_concref<T1,T2> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned&, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( const sc_unsigned&, sc_unsigned_bitref );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( sc_unsigned&, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( const sc_unsigned&, sc_unsigned_subref );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( sc_unsigned&, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( const sc_unsigned&, sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( sc_unsigned&, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( sc_unsigned&, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( bool, sc_unsigned& );


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( const sc_unsigned&, sc_unsigned_concref<T1,T2> );

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned&, sc_unsigned_concref_r<T1,T2> );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( const sc_unsigned&, sc_unsigned_bitref );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( sc_unsigned&, sc_unsigned_bitref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( const sc_unsigned&, sc_unsigned_subref );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( sc_unsigned&, sc_unsigned_subref_r );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( const sc_unsigned&, sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( sc_unsigned&, const sc_unsigned& );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( sc_unsigned&, bool );

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( bool, sc_unsigned& );

#endif


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_concref<T1,T2> >
operator , ( sc_unsigned&, sc_unsigned_concref<T1,T2> );

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_bitref>
operator , ( sc_unsigned&, sc_unsigned_bitref );

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_subref>
operator , ( sc_unsigned&, sc_unsigned_subref );

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned>
operator , ( sc_unsigned&, sc_unsigned& );


template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_concref<T1,T2> >
concat( sc_unsigned&, sc_unsigned_concref<T1,T2> );

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_bitref>
concat( sc_unsigned&, sc_unsigned_bitref );

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_subref>
concat( sc_unsigned&, sc_unsigned_subref );

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned>
concat( sc_unsigned&, sc_unsigned& );


inline
ostream&
operator << ( ostream&, const sc_unsigned& );

inline
istream&
operator >> ( istream&, sc_unsigned& );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref_r
//
//  Proxy class for sc_unsigned bit selection (r-value only).
// ----------------------------------------------------------------------------

// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_bitref_r a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_bitref_r a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_bitref_r a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref_r a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref_r a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( bool a, sc_unsigned_bitref_r b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_bitref_r a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_bitref_r a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
concat( sc_unsigned_bitref_r a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref_r a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref_r a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( bool a, sc_unsigned_bitref_r b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_bitref_r a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_bitref a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_bitref_r a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_bitref a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_bitref_r a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_bitref a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref_r a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
operator , ( sc_unsigned_bitref a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( bool a, sc_unsigned_bitref b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_bitref_r a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_bitref a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_bitref_r a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_bitref a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
concat( sc_unsigned_bitref_r a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>
concat( sc_unsigned_bitref a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref_r a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>
concat( sc_unsigned_bitref a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_bitref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( bool a, sc_unsigned_bitref b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	aa, *b.clone(), 3 );
}

#endif


inline
ostream&
operator << ( ostream& os, const sc_unsigned_bitref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref
//
//  Proxy class for sc_unsigned bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_concref<T1,T2> >
operator , ( sc_unsigned_bitref a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_bitref>
operator , ( sc_unsigned_bitref a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_subref>
operator , ( sc_unsigned_bitref a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned>
operator , ( sc_unsigned_bitref a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_concref<T1,T2> >
concat( sc_unsigned_bitref a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_bitref>
concat( sc_unsigned_bitref a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_subref>
concat( sc_unsigned_bitref a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned>
concat( sc_unsigned_bitref a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned_bitref,sc_unsigned>(
	*a.clone(), b, 1 );
}


inline
istream&
operator >> ( istream& is, sc_unsigned_bitref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref_r
//
//  Proxy class for sc_unsigned part selection (r-value only).
// ----------------------------------------------------------------------------

// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_subref_r a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_subref_r a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_subref_r a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref_r a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref_r a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( bool a, sc_unsigned_subref_r b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_subref_r a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_subref_r a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
concat( sc_unsigned_subref_r a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref_r a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref_r a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( bool a, sc_unsigned_subref_r b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_subref_r a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned_subref a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_subref_r a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
operator , ( sc_unsigned_subref a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_subref_r a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
operator , ( sc_unsigned_subref a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref_r a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
operator , ( sc_unsigned_subref a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( bool a, sc_unsigned_subref b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_subref_r a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned_subref a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,
	                         sc_unsigned_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_subref_r a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>
concat( sc_unsigned_subref a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
concat( sc_unsigned_subref_r a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>
concat( sc_unsigned_subref a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref_r a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), b, 1 );
}

inline
sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>
concat( sc_unsigned_subref a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_subref_r,sc_unsigned>(
	*a.clone(), bb, 3 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( bool a, sc_unsigned_subref b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	aa, *b.clone(), 3 );
}

#endif


inline
ostream&
operator << ( ostream& os, const sc_unsigned_subref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref
//
//  Proxy class for sc_unsigned part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_unsigned_subref&
sc_unsigned_subref::operator = ( const char* a )
{
    sc_unsigned aa( length() );
    return ( *this = aa = a );
}


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_concref<T1,T2> >
operator , ( sc_unsigned_subref a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_bitref>
operator , ( sc_unsigned_subref a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_subref>
operator , ( sc_unsigned_subref a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned>
operator , ( sc_unsigned_subref a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_concref<T1,T2> >
concat( sc_unsigned_subref a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_bitref>
concat( sc_unsigned_subref a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_subref>
concat( sc_unsigned_subref a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_unsigned_concref<sc_unsigned_subref,sc_unsigned>
concat( sc_unsigned_subref a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned_subref,sc_unsigned>(
	*a.clone(), b, 1 );
}


inline
istream&
operator >> ( istream& is, sc_unsigned_subref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_unsigned_concref_r<T1,T2>
//
//  Proxy class for sc_unsigned concatenation (r-value only).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

// destructor

template <class T1, class T2>
inline
sc_unsigned_concref_r<T1,T2>::~sc_unsigned_concref_r()
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


// r-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_concref_r<T3,T4> b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref_r<T1,T2> a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref_r<T1,T2> a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( bool a, sc_unsigned_concref_r<T1,T2> b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_concref_r<T3,T4> b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref_r<T1,T2> a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref_r<T1,T2> a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( bool a, sc_unsigned_concref_r<T1,T2> b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_concref<T3,T4> b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned_concref_r<T3,T4> b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref_r<T1,T2> a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref<T1,T2> a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref<T1,T2> a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( bool a, sc_unsigned_concref<T1,T2> b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_concref<T3,T4> b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
                      sc_unsigned_concref_r<T3,T4> >
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned_concref_r<T3,T4> b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_bitref_r>
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned_subref_r>
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,
	                         sc_unsigned_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref_r<T1,T2> a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref<T1,T2> a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref<T1,T2> a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned_concref_r<T1,T2>,sc_unsigned>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( bool a, sc_unsigned_concref<T1,T2> b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}

#endif


template <class T1, class T2>
inline
ostream&
operator << ( ostream& os, const sc_unsigned_concref_r<T1,T2>& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_unsigned_concref<T1,T2>
//
//  Proxy class for sc_unsigned concatenation (r-value and l-value).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

// l-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_concref<T3,T4> >
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned_concref<T3,T4> b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,
                               sc_unsigned_concref<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_bitref>
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_subref>
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned>
operator , ( sc_unsigned_concref<T1,T2> a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_concref<T3,T4> >
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned_concref<T3,T4> b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,
                               sc_unsigned_concref<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_bitref>
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_subref>
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned>
concat( sc_unsigned_concref<T1,T2> a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned_concref<T1,T2>,sc_unsigned>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
istream&
operator >> ( istream& is, sc_unsigned_concref<T1,T2>& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
//
//  Arbitrary precision signed number.
// ----------------------------------------------------------------------------

// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( const sc_unsigned& a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( const sc_unsigned& a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( const sc_unsigned& a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( const sc_unsigned& a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, b );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( const sc_unsigned& a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, bb, 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( bool a, const sc_unsigned& b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	aa, b, 1 );
}


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( const sc_unsigned& a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( const sc_unsigned& a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( const sc_unsigned& a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( const sc_unsigned& a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, b );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( const sc_unsigned& a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, bb, 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( bool a, const sc_unsigned& b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	aa, b, 1 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( const sc_unsigned& a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
operator , ( sc_unsigned& a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( const sc_unsigned& a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
operator , ( sc_unsigned& a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( const sc_unsigned& a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
operator , ( sc_unsigned& a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( const sc_unsigned& a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, b );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( sc_unsigned& a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, b );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( sc_unsigned& a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, bb, 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
operator , ( bool a, sc_unsigned& b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	aa, b, 1 );
}


template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( const sc_unsigned& a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >
concat( sc_unsigned& a, sc_unsigned_concref_r<T1,T2> b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( const sc_unsigned& a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>
concat( sc_unsigned& a, sc_unsigned_bitref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( const sc_unsigned& a, sc_unsigned_subref b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>
concat( sc_unsigned& a, sc_unsigned_subref_r b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( const sc_unsigned& a, sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, b );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( sc_unsigned& a, const sc_unsigned& b )
{
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, b );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( sc_unsigned& a, bool b )
{
    sc_unsigned& bb = *new sc_unsigned( 1 );
    bb = (b ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	a, bb, 2 );
}

inline
sc_unsigned_concref_r<sc_unsigned,sc_unsigned>
concat( bool a, sc_unsigned& b )
{
    sc_unsigned& aa = *new sc_unsigned( 1 );
    aa = (a ? 1 : 0);
    return sc_unsigned_concref_r<sc_unsigned,sc_unsigned>(
	aa, b, 1 );
}

#endif


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_concref<T1,T2> >
operator , ( sc_unsigned& a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned_concref<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_bitref>
operator , ( sc_unsigned& a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned_bitref>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_subref>
operator , ( sc_unsigned& a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned_subref>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned>
operator , ( sc_unsigned& a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned>(
	a, b );
}


template <class T1, class T2>
inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_concref<T1,T2> >
concat( sc_unsigned& a, sc_unsigned_concref<T1,T2> b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned_concref<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_bitref>
concat( sc_unsigned& a, sc_unsigned_bitref b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned_bitref>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned_subref>
concat( sc_unsigned& a, sc_unsigned_subref b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned_subref>(
	a, *b.clone(), 2 );
}

inline
sc_unsigned_concref<sc_unsigned,sc_unsigned>
concat( sc_unsigned& a, sc_unsigned& b )
{
    return sc_unsigned_concref<sc_unsigned,sc_unsigned>(
	a, b );
}


inline
ostream&
operator << ( ostream& os, const sc_unsigned& a )
{
    a.print( os );
    return os;
}

inline
istream&
operator >> ( istream& is, sc_unsigned& a )
{
    a.scan( is );
    return is;
}


#undef NOT_YET_IMPLEMENTED

} // namespace sc_dt


#endif
