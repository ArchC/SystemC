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

  sc_signed.h -- Arbitrary precision signed arithmetic.
 
    This file includes the definitions of sc_signed_bitref,
    sc_signed_subref, and sc_signed classes. The first two classes are
    proxy classes to reference one bit and a range of bits of a
    sc_signed number, respectively.

    An sc_signed number has the sign-magnitude representation
    internally. However, its interface guarantees a 2's-complement
    representation. The sign-magnitude representation is chosen
    because of its efficiency: The sc_signed and sc_unsigned types are
    optimized for arithmetic rather than bitwise operations. For
    arithmetic operations, the sign-magnitude representation performs
    better.

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

#ifndef SC_SIGNED_H
#define SC_SIGNED_H


#include "systemc/utils/sc_iostream.h"
#include "systemc/datatypes/int/sc_length_param.h"
#include "systemc/datatypes/int/sc_nbdefs.h"
#include "systemc/datatypes/int/sc_nbutils.h"
#include "systemc/datatypes/int/sc_nbexterns.h"


namespace sc_dt
{

// classes defined in this module
class sc_signed_bitref_r;
class sc_signed_bitref;
class sc_signed_subref_r;
class sc_signed_subref;
template <class T1, class T2>
    class sc_signed_concref_r; // NOT YET IMPLEMENTED
template <class T1, class T2>
    class sc_signed_concref;   // NOT YET IMPLEMENTED
class sc_signed;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
class sc_int_base;
class sc_uint_base;
class sc_signed;
class sc_unsigned;
class sc_unsigned_subref_r;
template <class T1, class T2>
    class sc_unsigned_concref_r; // NOT YET IMPLEMENTED
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;


#define NOT_YET_IMPLEMENTED                                                   \
    SC_REPORT_ERROR( SC_ID_NOT_IMPLEMENTED_,                                  \
                     "sc_signed/sc_bigint concatenation" )


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_bitref_r
//
//  Proxy class for sc_signed bit selection (r-value only).
// ----------------------------------------------------------------------------

class sc_signed_bitref_r
{
    friend class sc_signed;

protected:

    // constructor

    sc_signed_bitref_r( const sc_signed& obj_, int index_ )
	: m_obj( CCAST<sc_signed&>( obj_ ) ), m_index( index_ )
	{}

public:

    // copy constructor

    sc_signed_bitref_r( const sc_signed_bitref_r& a )
	: m_obj( a.m_obj ), m_index( a.m_index )
	{}


    // cloning

    sc_signed_bitref_r* clone() const
	{ return new sc_signed_bitref_r( *this ); }


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

    sc_signed& m_obj;
    int        m_index;

private:

    // disabled
    sc_signed_bitref_r();
    sc_signed_bitref_r& operator = ( const sc_signed_bitref_r& );
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_bitref_r, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
operator , ( sc_signed_bitref_r, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
operator , ( sc_signed_bitref_r, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref_r, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref_r, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( bool, sc_signed_bitref_r );


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_bitref_r, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
concat( sc_signed_bitref_r, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
concat( sc_signed_bitref_r, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref_r, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref_r, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( bool, sc_signed_bitref_r );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_bitref_r, sc_signed_concref<T1,T2> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_bitref, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
operator , ( sc_signed_bitref_r, sc_signed_bitref );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
operator , ( sc_signed_bitref, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
operator , ( sc_signed_bitref_r, sc_signed_subref );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
operator , ( sc_signed_bitref, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref_r, sc_signed& );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( bool, sc_signed_bitref );


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_bitref_r, sc_signed_concref<T1,T2> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_bitref, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
concat( sc_signed_bitref_r, sc_signed_bitref );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
concat( sc_signed_bitref, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
concat( sc_signed_bitref_r, sc_signed_subref );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
concat( sc_signed_bitref, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref_r, sc_signed& );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( bool, sc_signed_bitref );

#endif


inline
ostream&
operator << ( ostream&, const sc_signed_bitref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_bitref
//
//  Proxy class for sc_signed bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_signed_bitref
    : public sc_signed_bitref_r
{
    friend class sc_signed;


    // constructor

    sc_signed_bitref( sc_signed& obj_, int index_ )
	: sc_signed_bitref_r( obj_, index_ )
	{}

public:

    // copy constructor

    sc_signed_bitref( const sc_signed_bitref& a )
	: sc_signed_bitref_r( a )
	{}


    // cloning

    sc_signed_bitref* clone() const
	{ return new sc_signed_bitref( *this ); }


    // assignment operators

    sc_signed_bitref& operator = ( const sc_signed_bitref_r& );
    sc_signed_bitref& operator = ( const sc_signed_bitref& );
    sc_signed_bitref& operator = ( bool );

    sc_signed_bitref& operator &= ( bool );
    sc_signed_bitref& operator |= ( bool );
    sc_signed_bitref& operator ^= ( bool );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_signed_bitref();
};


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_bitref,sc_signed_concref<T1,T2> >
operator , ( sc_signed_bitref, sc_signed_concref<T1,T2> );

inline
sc_signed_concref<sc_signed_bitref,sc_signed_bitref>
operator , ( sc_signed_bitref, sc_signed_bitref );

inline
sc_signed_concref<sc_signed_bitref,sc_signed_subref>
operator , ( sc_signed_bitref, sc_signed_subref );

inline
sc_signed_concref<sc_signed_bitref,sc_signed>
operator , ( sc_signed_bitref, sc_signed& );


template <class T1, class T2>
inline
sc_signed_concref<sc_signed_bitref,sc_signed_concref<T1,T2> >
concat( sc_signed_bitref, sc_signed_concref<T1,T2> );

inline
sc_signed_concref<sc_signed_bitref,sc_signed_bitref>
concat( sc_signed_bitref, sc_signed_bitref );

inline
sc_signed_concref<sc_signed_bitref,sc_signed_subref>
concat( sc_signed_bitref, sc_signed_subref );

inline
sc_signed_concref<sc_signed_bitref,sc_signed>
concat( sc_signed_bitref, sc_signed& );


inline
istream&
operator >> ( istream&, sc_signed_bitref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref_r
//
//  Proxy class for sc_signed part selection (r-value only).
// ----------------------------------------------------------------------------

class sc_signed_subref_r
{
    friend class sc_signed;

protected:

    // constructor

    sc_signed_subref_r( const sc_signed& obj_, int left_, int right_ )
	: m_obj( CCAST<sc_signed&>( obj_ ) ),
	  m_left( left_ ), m_right( right_ )
	{}
  
public:

    // copy constructor

    sc_signed_subref_r( const sc_signed_subref_r& a )
	: m_obj( a.m_obj ), m_left( a.m_left ), m_right( a.m_right )
	{}


    // cloning

    sc_signed_subref_r* clone() const
	{ return new sc_signed_subref_r( *this ); }


    // capacity

    int length() const
        { return ( m_left - m_right + 1 ); }


    // implicit conversion to sc_signed

    operator sc_signed () const;


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

    sc_signed& m_obj;
    int        m_left;
    int        m_right;

private:

    // disabled
    sc_signed_subref_r();
    sc_signed_subref_r& operator = ( const sc_signed_subref_r& );
};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_subref_r, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
operator , ( sc_signed_subref_r, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
operator , ( sc_signed_subref_r, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref_r, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref_r, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( bool, sc_signed_subref_r );


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_subref_r, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
concat( sc_signed_subref_r, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
concat( sc_signed_subref_r, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref_r, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref_r, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( bool, sc_signed_subref_r );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_subref_r, sc_signed_concref<T1,T2> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_subref, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
operator , ( sc_signed_subref_r, sc_signed_bitref );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
operator , ( sc_signed_subref, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
operator , ( sc_signed_subref_r, sc_signed_subref );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
operator , ( sc_signed_subref, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref_r, sc_signed& );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( bool, sc_signed_subref );


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_subref_r, sc_signed_concref<T1,T2> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_subref, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
concat( sc_signed_subref_r, sc_signed_bitref );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
concat( sc_signed_subref, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
concat( sc_signed_subref_r, sc_signed_subref );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
concat( sc_signed_subref, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref_r, sc_signed& );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref, const sc_signed& );

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( bool, sc_signed_subref );

#endif


inline
ostream&
operator << ( ostream&, const sc_signed_subref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref
//
//  Proxy class for sc_signed part selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_signed_subref
    : public sc_signed_subref_r
{
    friend class sc_signed;


    // constructor

    sc_signed_subref( sc_signed& obj_, int left_, int right_ )
	: sc_signed_subref_r( obj_, left_, right_ )
	{}
  
public:

    // copy constructor

    sc_signed_subref( const sc_signed_subref& a )
	: sc_signed_subref_r( a )
	{}


    // cloning

    sc_signed_subref* clone() const
	{ return new sc_signed_subref( *this ); }


    // assignment operators

    sc_signed_subref& operator = ( const sc_signed_subref_r& a );
    sc_signed_subref& operator = ( const sc_signed_subref& a );
    sc_signed_subref& operator = ( const sc_signed& a );

    template <class T1, class T2>
    sc_signed_subref& operator = ( const sc_signed_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_signed_subref& operator = ( const sc_unsigned_subref_r& a );
    sc_signed_subref& operator = ( const sc_unsigned& a );

    template <class T1, class T2>
    sc_signed_subref& operator = ( const sc_unsigned_concref_r<T1,T2>& v )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_signed_subref& operator = ( const char* a );
    sc_signed_subref& operator = ( unsigned long a );
    sc_signed_subref& operator = ( long a );

    sc_signed_subref& operator = ( unsigned int a )
	{ return operator = ( (unsigned long) a ); }

    sc_signed_subref& operator = ( int a )
	{ return operator = ( (long) a ); }

    sc_signed_subref& operator = ( uint64 a );
    sc_signed_subref& operator = ( int64 a );
    sc_signed_subref& operator = ( double a );  
    sc_signed_subref& operator = ( const sc_int_base& a );
    sc_signed_subref& operator = ( const sc_uint_base& a );


    // other methods

    void scan( istream& is = cin );

private:

    // disabled
    sc_signed_subref();
};


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_subref,sc_signed_concref<T1,T2> >
operator , ( sc_signed_subref, sc_signed_concref<T1,T2> );

inline
sc_signed_concref<sc_signed_subref,sc_signed_bitref>
operator , ( sc_signed_subref, sc_signed_bitref );

inline
sc_signed_concref<sc_signed_subref,sc_signed_subref>
operator , ( sc_signed_subref, sc_signed_subref );

inline
sc_signed_concref<sc_signed_subref,sc_signed>
operator , ( sc_signed_subref, sc_signed& );


template <class T1, class T2>
inline
sc_signed_concref<sc_signed_subref,sc_signed_concref<T1,T2> >
concat( sc_signed_subref, sc_signed_concref<T1,T2> );

inline
sc_signed_concref<sc_signed_subref,sc_signed_bitref>
concat( sc_signed_subref, sc_signed_bitref );

inline
sc_signed_concref<sc_signed_subref,sc_signed_subref>
concat( sc_signed_subref, sc_signed_subref );

inline
sc_signed_concref<sc_signed_subref,sc_signed>
concat( sc_signed_subref, sc_signed& );


inline
istream&
operator >> ( istream&, sc_signed_subref& );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_signed_concref_r<T1,T2>
//
//  Proxy class for sc_signed concatenation (r-value only).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_signed_concref_r
{
public:

    // constructor

    sc_signed_concref_r( const T1& left_, const T2& right_, int delete_ = 0 )
	: m_left( CCAST<T1&>( left_ ) ), m_right( CCAST<T2&>( right_ ) ),
	  m_delete( delete_ ), m_refs( *new int( 1 ) )
	{ m_len = m_left.length() + m_right.length(); NOT_YET_IMPLEMENTED; }


    // copy constructor

    sc_signed_concref_r( const sc_signed_concref_r<T1,T2>& a )
	: m_left( a.m_left ), m_right( a.m_right ), m_len( a.m_len ),
	  m_delete( a.m_delete ), m_refs( a.m_refs )
	{ ++ m_refs; NOT_YET_IMPLEMENTED; }


    // destructor

    ~sc_signed_concref_r();


    // cloning

    sc_signed_concref_r<T1,T2>* clone() const
	{ return new sc_signed_concref_r<T1,T2>( *this ); }


    // capacity

    int length() const
	{ return m_len; }


    // implicit conversion to sc_signed

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
    sc_signed_concref_r();
    sc_signed_concref_r<T1,T2>& operator = (
	const sc_signed_concref_r<T1,T2>& );
};


// r-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
operator , ( sc_signed_concref_r<T1,T2>, sc_signed_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
operator , ( sc_signed_concref_r<T1,T2>, sc_signed_bitref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
operator , ( sc_signed_concref_r<T1,T2>, sc_signed_subref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref_r<T1,T2>, const sc_signed& );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref_r<T1,T2>, bool );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( bool, sc_signed_concref_r<T1,T2> );


template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
concat( sc_signed_concref_r<T1,T2>, sc_signed_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
concat( sc_signed_concref_r<T1,T2>, sc_signed_bitref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
concat( sc_signed_concref_r<T1,T2>, sc_signed_subref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref_r<T1,T2>, const sc_signed& );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref_r<T1,T2>, bool );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( bool, sc_signed_concref_r<T1,T2> );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
operator , ( sc_signed_concref_r<T1,T2>, sc_signed_concref<T3,T4> );

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
operator , ( sc_signed_concref<T1,T2>, sc_signed_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
operator , ( sc_signed_concref_r<T1,T2>, sc_signed_bitref );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
operator , ( sc_signed_concref<T1,T2>, sc_signed_bitref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
operator , ( sc_signed_concref_r<T1,T2>, sc_signed_subref );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
operator , ( sc_signed_concref<T1,T2>, sc_signed_subref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref_r<T1,T2>, sc_signed& );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref<T1,T2>, const sc_signed& );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref<T1,T2>, bool );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( bool, sc_signed_concref<T1,T2> );


template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
concat( sc_signed_concref_r<T1,T2>, sc_signed_concref<T3,T4> );

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
concat( sc_signed_concref<T1,T2>, sc_signed_concref_r<T3,T4> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
concat( sc_signed_concref_r<T1,T2>, sc_signed_bitref );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
concat( sc_signed_concref<T1,T2>, sc_signed_bitref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
concat( sc_signed_concref_r<T1,T2>, sc_signed_subref );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
concat( sc_signed_concref<T1,T2>, sc_signed_subref_r );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref_r<T1,T2>, sc_signed& );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref<T1,T2>, const sc_signed& );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref<T1,T2>, bool );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( bool, sc_signed_concref<T1,T2> );

#endif


template <class T1, class T2>
inline
ostream&
operator << ( ostream&, const sc_signed_concref_r<T1,T2>& );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_signed_concref<T1,T2>
//
//  Proxy class for sc_signed concatenation (r-value and l-value).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

template <class T1, class T2>
class sc_signed_concref
    : public sc_signed_concref_r<T1,T2>
{
public:

    // constructor

    sc_signed_concref( T1& left_, T2& right_, int delete_ = 0 )
	: sc_signed_concref_r<T1,T2>( left_, right_, delete_ )
	{}


    // copy constructor

    sc_signed_concref( const sc_signed_concref<T1,T2>& a )
	: sc_signed_concref_r<T1,T2>( a )
	{}


    // cloning

    sc_signed_concref<T1,T2>* clone() const
	{ return new sc_signed_concref<T1,T2>( *this ); }


    // assignment operators

    // NOT YET IMPLEMENTED


    // other methods

    void scan( istream& is = cin )
	{ NOT_YET_IMPLEMENTED; }

private:

    // disabled
    sc_signed_concref();
};


// l-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_concref<T3,T4> >
operator , ( sc_signed_concref<T1,T2>, sc_signed_concref<T3,T4> );

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_bitref>
operator , ( sc_signed_concref<T1,T2>, sc_signed_bitref );

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_subref>
operator , ( sc_signed_concref<T1,T2>, sc_signed_subref );

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed>
operator , ( sc_signed_concref<T1,T2>, sc_signed& );


template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_concref<T3,T4> >
concat( sc_signed_concref<T1,T2>, sc_signed_concref<T3,T4> );

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_bitref>
concat( sc_signed_concref<T1,T2>, sc_signed_bitref );

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_subref>
concat( sc_signed_concref<T1,T2>, sc_signed_subref );

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed>
concat( sc_signed_concref<T1,T2>, sc_signed& );


template <class T1, class T2>
inline
istream&
operator >> ( istream&, sc_signed_concref<T1,T2>& );


// ----------------------------------------------------------------------------
//  CLASS : sc_signed
//
//  Arbitrary precision signed number.
// ----------------------------------------------------------------------------

class sc_signed
{
    friend class sc_signed_bitref_r;
    friend class sc_signed_bitref;
    friend class sc_signed_subref_r;
    friend class sc_signed_subref;
    friend class sc_unsigned;
    friend class sc_unsigned_subref;

  // Needed for types using sc_signed.
  typedef bool elemtype;

public:

    // constructors

    explicit sc_signed( int nb = sc_length_param().len() );
    sc_signed( const sc_signed&   v );
    sc_signed( const sc_unsigned& v );


    // assignment operators

    sc_signed& operator = (const sc_signed&          v);
    sc_signed& operator = (const sc_signed_subref_r& a );

    template <class T1, class T2>
    sc_signed& operator = ( const sc_signed_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_signed& operator = (const sc_unsigned&        v);
    sc_signed& operator = (const sc_unsigned_subref_r& a );

    template <class T1, class T2>
    sc_signed& operator = ( const sc_unsigned_concref_r<T1,T2>& a )
	{ NOT_YET_IMPLEMENTED; return *this; }

    sc_signed& operator = (const char*               v);
    sc_signed& operator = (int64                     v);
    sc_signed& operator = (uint64                    v);
    sc_signed& operator = (long                      v);
    sc_signed& operator = (unsigned long             v);

    sc_signed& operator = (int                       v) 
	{ return operator=((long) v); }

    sc_signed& operator = (unsigned int              v) 
	{ return operator=((unsigned long) v); }

    sc_signed& operator = (double                    v);
    sc_signed& operator = (const sc_int_base&        v);
    sc_signed& operator = (const sc_uint_base&       v);

    sc_signed& operator = ( const sc_bv_base& );
    sc_signed& operator = ( const sc_lv_base& );

#ifdef SC_INCLUDE_FX
    sc_signed& operator = ( const sc_fxval& );
    sc_signed& operator = ( const sc_fxval_fast& );
    sc_signed& operator = ( const sc_fxnum& );
    sc_signed& operator = ( const sc_fxnum_fast& );
#endif


    // destructor

    ~sc_signed() 
	{ 
#ifndef SC_MAX_NBITS
	    delete [] digit; 
#endif
	}


  // Increment operators.
  sc_signed& operator ++ ();
  const sc_signed operator ++ (int);

  // Decrement operators.
  sc_signed& operator -- ();
  const sc_signed operator -- (int);


    // bit selection

    sc_signed_bitref operator [] ( int i )
	{ return sc_signed_bitref( *this, i ); }

    sc_signed_bitref_r operator [] ( int i ) const
	{ return sc_signed_bitref_r( *this, i ); }

    sc_signed_bitref bit( int i )
	{ return sc_signed_bitref( *this, i ); }

    sc_signed_bitref_r bit( int i ) const
	{ return sc_signed_bitref_r( *this, i ); }


    // part selection

    // Subref operators. Help access the range of bits from the ith to
    // jth. These indices have arbitrary precedence with respect to each
    // other, i.e., we can have i <= j or i > j. Note the equivalence
    // between range(i, j) and operator(i, j). Also note that
    // operator(i, i) returns a signed number that corresponds to the
    // bit operator[i], so these two forms are not the same.

    sc_signed_subref range( int i, int j )
	{ return sc_signed_subref( *this, i, j ); }

    sc_signed_subref_r range( int i, int j ) const
	{ return sc_signed_subref_r( *this, i, j ); }

    sc_signed_subref operator () ( int i, int j )
	{ return sc_signed_subref( *this, i, j ); }

    sc_signed_subref_r operator () ( int i, int j ) const
	{ return sc_signed_subref_r( *this, i, j ); }
  

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
  int  length() const { return nbits; }  // Bit width.
  bool iszero() const;                   // Is the number zero?
  bool sign() const;                     // Sign.

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

  friend sc_signed operator + (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator + (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator + (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator + (const sc_unsigned&  u, long                v); 
  friend sc_signed operator + (const sc_unsigned&  u, int                 v) 
    { return operator+(u, (long) v); }

  friend sc_signed operator + (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator + (long                u, const sc_unsigned&  v); 
  friend sc_signed operator + (int                 u, const sc_unsigned&  v)  
    { return operator+((long) u, v); } 

  friend sc_signed operator + (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator + (const sc_signed&    u, int64               v); 
  friend sc_signed operator + (const sc_signed&    u, uint64              v); 
  friend sc_signed operator + (const sc_signed&    u, long                v); 
  friend sc_signed operator + (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator + (const sc_signed&    u, int                 v) 
    { return operator+(u, (long) v); }
  friend sc_signed operator + (const sc_signed&    u, unsigned int        v) 
    { return operator+(u, (unsigned long) v); }

  friend sc_signed operator + (int64               u, const sc_signed&    v); 
  friend sc_signed operator + (uint64              u, const sc_signed&    v); 
  friend sc_signed operator + (long                u, const sc_signed&    v); 
  friend sc_signed operator + (unsigned long       u, const sc_signed&    v);
  friend sc_signed operator + (int                 u, const sc_signed&    v)  
    { return operator+((long) u, v); } 
  friend sc_signed operator + (unsigned int        u, const sc_signed&    v)  
    { return operator+((unsigned long) u, v); } 

  sc_signed& operator += (const sc_signed&    v); 
  sc_signed& operator += (const sc_unsigned&  v); 
  sc_signed& operator += (int64               v); 
  sc_signed& operator += (uint64              v); 
  sc_signed& operator += (long                v); 
  sc_signed& operator += (unsigned long       v); 
  sc_signed& operator += (int                 v) 
    { return operator+=((long) v); }
  sc_signed& operator += (unsigned int        v) 
    { return operator+=((unsigned long) v); }

  friend sc_signed operator + (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator + (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator + (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator + (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator + (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator + (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator += (const sc_int_base&  v);
  sc_signed& operator += (const sc_uint_base& v);

  // SUBtraction operators:
   
  friend sc_signed operator - (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator - (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator - (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_signed operator - (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator - (const sc_unsigned&  u, uint64              v); 
  friend sc_signed operator - (const sc_unsigned&  u, long                v); 
  friend sc_signed operator - (const sc_unsigned&  u, unsigned long       v);
  friend sc_signed operator - (const sc_unsigned&  u, int                v) 
    { return operator-(u, (long) v); }
  friend sc_signed operator - (const sc_unsigned&  u, unsigned int       v) 
    { return operator-(u, (unsigned long) v); }

  friend sc_signed operator - (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator - (uint64              u, const sc_unsigned&  v); 
  friend sc_signed operator - (long                u, const sc_unsigned&  v); 
  friend sc_signed operator - (unsigned long       u, const sc_unsigned&  v);
  friend sc_signed operator - (int                 u, const sc_unsigned&  v)  
    { return operator-((long) u, v); } 
  friend sc_signed operator - (unsigned int        u, const sc_unsigned& v)  
    { return operator-((unsigned long) u, v); } 

  friend sc_signed operator - (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator - (const sc_signed&    u, int64               v); 
  friend sc_signed operator - (const sc_signed&    u, uint64              v); 
  friend sc_signed operator - (const sc_signed&    u, long                v); 
  friend sc_signed operator - (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator - (const sc_signed&    u, int                 v) 
    { return operator-(u, (long) v); }
  friend sc_signed operator - (const sc_signed&    u, unsigned int        v) 
    { return operator-(u, (unsigned long) v); }

  friend sc_signed operator - (int64               u, const sc_signed&    v); 
  friend sc_signed operator - (uint64              u, const sc_signed&    v); 
  friend sc_signed operator - (long                u, const sc_signed&    v); 
  friend sc_signed operator - (unsigned long       u, const sc_signed&    v);
  friend sc_signed operator - (int                 u, const sc_signed&    v)  
    { return operator-((long) u, v); } 
  friend sc_signed operator - (unsigned int        u, const sc_signed&    v)  
    { return operator-((unsigned long) u, v); } 

  sc_signed& operator -= (const sc_signed&    v); 
  sc_signed& operator -= (const sc_unsigned&  v); 
  sc_signed& operator -= (int64               v); 
  sc_signed& operator -= (uint64              v); 
  sc_signed& operator -= (long                v); 
  sc_signed& operator -= (unsigned long       v); 
  sc_signed& operator -= (int                 v) 
    { return operator -= ((long) v); }
  sc_signed& operator -= (unsigned int        v) 
    { return operator -= ((unsigned long) v); }

  friend sc_signed operator - (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator - (const sc_unsigned&  u, const sc_uint_base& v);
  friend sc_signed operator - (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator - (const sc_uint_base& u, const sc_unsigned&  v);
  friend sc_signed operator - (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator - (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator - (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator - (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator -= (const sc_int_base&  v);
  sc_signed& operator -= (const sc_uint_base& v);

  // MULtiplication operators:
   
  friend sc_signed operator * (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator * (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator * (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator * (const sc_unsigned&  u, long                v); 
  friend sc_signed operator * (const sc_unsigned&  u, int                 v) 
    { return operator*(u, (long) v); }

  friend sc_signed operator * (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator * (long                u, const sc_unsigned&  v); 
  friend sc_signed operator * (int                 u, const sc_unsigned&  v)  
    { return operator*((long) u, v); } 

  friend sc_signed operator * (const sc_signed&  u, const sc_signed&  v);
  friend sc_signed operator * (const sc_signed&  u, int64             v); 
  friend sc_signed operator * (const sc_signed&  u, uint64            v); 
  friend sc_signed operator * (const sc_signed&  u, long              v); 
  friend sc_signed operator * (const sc_signed&  u, unsigned long     v);
  friend sc_signed operator * (const sc_signed&  u, int               v) 
    { return operator*(u, (long) v); }
  friend sc_signed operator * (const sc_signed&  u, unsigned int      v) 
    { return operator*(u, (unsigned long) v); }

  friend sc_signed operator * (int64             u, const sc_signed&  v); 
  friend sc_signed operator * (uint64            u, const sc_signed&  v); 
  friend sc_signed operator * (long              u, const sc_signed&  v); 
  friend sc_signed operator * (unsigned long     u, const sc_signed&  v);
  friend sc_signed operator * (int               u, const sc_signed&  v)  
    { return operator*((long) u, v); } 
  friend sc_signed operator * (unsigned int      u, const sc_signed&  v)  
    { return operator*((unsigned long) u, v); } 

  sc_signed& operator *= (const sc_signed&    v); 
  sc_signed& operator *= (const sc_unsigned&  v); 
  sc_signed& operator *= (int64               v); 
  sc_signed& operator *= (uint64              v); 
  sc_signed& operator *= (long                v); 
  sc_signed& operator *= (unsigned long       v); 
  sc_signed& operator *= (int                 v) 
    { return operator*=((long) v); }
  sc_signed& operator *= (unsigned int        v) 
    { return operator*=((unsigned long) v); }

  friend sc_signed operator * (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator * (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator * (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator * (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator * (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator * (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator *= (const sc_int_base&  v);
  sc_signed& operator *= (const sc_uint_base& v);

  // DIVision operators:
   
  friend sc_signed operator / (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator / (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator / (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator / (const sc_unsigned&  u, long                v); 
  friend sc_signed operator / (const sc_unsigned&  u, int                 v) 
    { return operator/(u, (long) v); }

  friend sc_signed operator / (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator / (long                u, const sc_unsigned&  v); 
  friend sc_signed operator / (int                 u, const sc_unsigned&  v)  
    { return operator/((long) u, v); } 

  friend sc_signed operator / (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator / (const sc_signed&    u, int64               v); 
  friend sc_signed operator / (const sc_signed&    u, uint64              v); 
  friend sc_signed operator / (const sc_signed&    u, long                v); 
  friend sc_signed operator / (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator / (const sc_signed&    u, int                 v) 
    { return operator/(u, (long) v); }
  friend sc_signed operator / (const sc_signed&    u, unsigned int        v) 
    { return operator/(u, (unsigned long) v); }

  friend sc_signed operator / (int64               u, const sc_signed&    v); 
  friend sc_signed operator / (uint64              u, const sc_signed&    v); 
  friend sc_signed operator / (long                u, const sc_signed&    v); 
  friend sc_signed operator / (unsigned long       u, const sc_signed&    v);
  friend sc_signed operator / (int                 u, const sc_signed&    v)  
    { return operator/((long) u, v); } 
  friend sc_signed operator / (unsigned int        u, const sc_signed&    v)  
    { return operator/((unsigned long) u, v); } 

  sc_signed& operator /= (const sc_signed&    v); 
  sc_signed& operator /= (const sc_unsigned&  v); 
  sc_signed& operator /= (int64               v); 
  sc_signed& operator /= (uint64              v); 
  sc_signed& operator /= (long                v); 
  sc_signed& operator /= (unsigned long       v); 
  sc_signed& operator /= (int                 v) 
    { return operator/=((long) v); }
  sc_signed& operator /= (unsigned int        v) 
    { return operator/=((unsigned long) v); }

  friend sc_signed operator / (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator / (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator / (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator / (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator / (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator / (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator /= (const sc_int_base&  v);
  sc_signed& operator /= (const sc_uint_base& v);

  // MODulo operators:
   
  friend sc_signed operator % (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator % (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator % (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator % (const sc_unsigned&  u, long                v); 
  friend sc_signed operator % (const sc_unsigned&  u, int                 v) 
    { return operator%(u, (long) v); }

  friend sc_signed operator % (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator % (long                u, const sc_unsigned&  v); 
  friend sc_signed operator % (int                 u, const sc_unsigned&  v)  
    { return operator%((long) u, v); } 

  friend sc_signed operator % (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator % (const sc_signed&    u, int64               v); 
  friend sc_signed operator % (const sc_signed&    u, uint64              v); 
  friend sc_signed operator % (const sc_signed&    u, long                v); 
  friend sc_signed operator % (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator % (const sc_signed&    u, int                 v) 
    { return operator%(u, (long) v); }
  friend sc_signed operator % (const sc_signed&    u, unsigned int        v) 
    { return operator%(u, (unsigned long) v); }

  friend sc_signed operator % (int64               u, const sc_signed&    v); 
  friend sc_signed operator % (uint64              u, const sc_signed&    v); 
  friend sc_signed operator % (long                u, const sc_signed&    v); 
  friend sc_signed operator % (unsigned long       u, const sc_signed&    v);
  friend sc_signed operator % (int                 u, const sc_signed&    v)  
    { return operator%((long) u, v); } 
  friend sc_signed operator % (unsigned int        u, const sc_signed&    v)  
    { return operator%((unsigned long) u, v); } 

  sc_signed& operator %= (const sc_signed&    v); 
  sc_signed& operator %= (const sc_unsigned&  v); 
  sc_signed& operator %= (int64               v); 
  sc_signed& operator %= (uint64              v); 
  sc_signed& operator %= (long                v); 
  sc_signed& operator %= (unsigned long       v); 
  sc_signed& operator %= (int                 v) 
    { return operator%=((long) v); }
  sc_signed& operator %= (unsigned int        v) 
    { return operator%=((unsigned long) v); }

  friend sc_signed operator % (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator % (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator % (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator % (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator % (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator % (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator %= (const sc_int_base&  v);
  sc_signed& operator %= (const sc_uint_base& v);

  // BITWISE OPERATORS:

  // Bitwise AND operators:
   
  friend sc_signed operator & (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator & (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator & (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator & (const sc_unsigned&  u, long                v); 
  friend sc_signed operator & (const sc_unsigned&  u, int                 v) 
    { return operator&(u, (long) v); }

  friend sc_signed operator & (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator & (long                u, const sc_unsigned&  v); 
  friend sc_signed operator & (int                 u, const sc_unsigned&  v)  
    { return operator&((long) u, v); } 

  friend sc_signed operator & (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator & (const sc_signed&    u, int64               v); 
  friend sc_signed operator & (const sc_signed&    u, uint64              v); 
  friend sc_signed operator & (const sc_signed&    u, long                v); 
  friend sc_signed operator & (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator & (const sc_signed&    u, int                 v) 
    { return operator&(u, (long) v); }
  friend sc_signed operator & (const sc_signed&    u, unsigned int        v) 
    { return operator&(u, (unsigned long) v); }

  friend sc_signed operator & (int64             u, const sc_signed&  v); 
  friend sc_signed operator & (uint64            u, const sc_signed&  v); 
  friend sc_signed operator & (long              u, const sc_signed&  v); 
  friend sc_signed operator & (unsigned long     u, const sc_signed&  v);
  friend sc_signed operator & (int               u, const sc_signed&  v)  
    { return operator&((long) u, v); } 
  friend sc_signed operator & (unsigned int      u, const sc_signed&  v)  
    { return operator&((unsigned long) u, v); } 

  sc_signed& operator &= (const sc_signed&    v); 
  sc_signed& operator &= (const sc_unsigned&  v); 
  sc_signed& operator &= (int64               v); 
  sc_signed& operator &= (uint64              v); 
  sc_signed& operator &= (long                v); 
  sc_signed& operator &= (unsigned long       v); 
  sc_signed& operator &= (int                 v) 
    { return operator&=((long) v); }
  sc_signed& operator &= (unsigned int        v) 
    { return operator&=((unsigned long) v); }

  friend sc_signed operator & (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator & (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator & (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator & (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator & (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator & (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator &= (const sc_int_base&  v);
  sc_signed& operator &= (const sc_uint_base& v);

  // Bitwise OR operators:
   
  friend sc_signed operator | (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator | (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator | (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator | (const sc_unsigned&  u, long                v); 
  friend sc_signed operator | (const sc_unsigned&  u, int                 v) 
    { return operator|(u, (long) v); }

  friend sc_signed operator | (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator | (long                u, const sc_unsigned&  v); 
  friend sc_signed operator | (int                 u, const sc_unsigned&  v)  
    { return operator|((long) u, v); } 

  friend sc_signed operator | (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator | (const sc_signed&    u, int64               v); 
  friend sc_signed operator | (const sc_signed&    u, uint64              v); 
  friend sc_signed operator | (const sc_signed&    u, long                v); 
  friend sc_signed operator | (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator | (const sc_signed&    u, int                 v) 
    { return operator|(u, (long) v); }
  friend sc_signed operator | (const sc_signed&    u, unsigned int        v) 
    { return operator|(u, (unsigned long) v); }

  friend sc_signed operator | (int64             u, const sc_signed&  v); 
  friend sc_signed operator | (uint64            u, const sc_signed&  v); 
  friend sc_signed operator | (long              u, const sc_signed&  v); 
  friend sc_signed operator | (unsigned long     u, const sc_signed&  v);
  friend sc_signed operator | (int               u, const sc_signed&  v)  
    { return operator|((long) u, v); } 
  friend sc_signed operator | (unsigned int      u, const sc_signed&  v)  
    { return operator|((unsigned long) u, v); } 

  sc_signed& operator |= (const sc_signed&    v); 
  sc_signed& operator |= (const sc_unsigned&  v); 
  sc_signed& operator |= (int64               v); 
  sc_signed& operator |= (uint64              v); 
  sc_signed& operator |= (long                v); 
  sc_signed& operator |= (unsigned long       v); 
  sc_signed& operator |= (int                 v) 
    { return operator|=((long) v); }
  sc_signed& operator |= (unsigned int        v) 
    { return operator|=((unsigned long) v); }

  friend sc_signed operator | (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator | (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator | (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator | (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator | (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator | (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator |= (const sc_int_base&  v);
  sc_signed& operator |= (const sc_uint_base& v);

  // Bitwise XOR operators:
   
  friend sc_signed operator ^ (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_signed operator ^ (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_signed operator ^ (const sc_unsigned&  u, int64               v); 
  friend sc_signed operator ^ (const sc_unsigned&  u, long                v); 
  friend sc_signed operator ^ (const sc_unsigned&  u, int                 v) 
    { return operator^(u, (long) v); }

  friend sc_signed operator ^ (int64               u, const sc_unsigned&  v); 
  friend sc_signed operator ^ (long                u, const sc_unsigned&  v); 
  friend sc_signed operator ^ (int                 u, const sc_unsigned&  v)  
    { return operator^((long) u, v); } 

  friend sc_signed operator ^ (const sc_signed&    u, const sc_signed&    v);
  friend sc_signed operator ^ (const sc_signed&    u, int64               v); 
  friend sc_signed operator ^ (const sc_signed&    u, uint64              v); 
  friend sc_signed operator ^ (const sc_signed&    u, long                v); 
  friend sc_signed operator ^ (const sc_signed&    u, unsigned long       v);
  friend sc_signed operator ^ (const sc_signed&    u, int                 v) 
    { return operator^(u, (long) v); }
  friend sc_signed operator ^ (const sc_signed&    u, unsigned int        v) 
    { return operator^(u, (unsigned long) v); }

  friend sc_signed operator ^ (int64             u, const sc_signed&  v); 
  friend sc_signed operator ^ (uint64            u, const sc_signed&  v); 
  friend sc_signed operator ^ (long              u, const sc_signed&  v); 
  friend sc_signed operator ^ (unsigned long     u, const sc_signed&  v);
  friend sc_signed operator ^ (int               u, const sc_signed&  v)  
    { return operator^((long) u, v); } 
  friend sc_signed operator ^ (unsigned int      u, const sc_signed&  v)  
    { return operator^((unsigned long) u, v); } 

  sc_signed& operator ^= (const sc_signed&    v); 
  sc_signed& operator ^= (const sc_unsigned&  v); 
  sc_signed& operator ^= (int64               v); 
  sc_signed& operator ^= (uint64              v); 
  sc_signed& operator ^= (long                v); 
  sc_signed& operator ^= (unsigned long       v); 
  sc_signed& operator ^= (int                 v) 
    { return operator^=((long) v); }
  sc_signed& operator ^= (unsigned int        v) 
    { return operator^=((unsigned long) v); }

  friend sc_signed operator ^ (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_signed operator ^ (const sc_int_base&  u, const sc_unsigned&  v);
  friend sc_signed operator ^ (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator ^ (const sc_signed&    u, const sc_uint_base& v); 
  friend sc_signed operator ^ (const sc_int_base&  u, const sc_signed&    v); 
  friend sc_signed operator ^ (const sc_uint_base& u, const sc_signed&    v); 
  sc_signed& operator ^= (const sc_int_base&  v);
  sc_signed& operator ^= (const sc_uint_base& v);

  // SHIFT OPERATORS:

  // LEFT SHIFT operators:
   
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator << (const sc_signed&    u, const sc_unsigned&  v); 

  friend   sc_signed operator << (const sc_signed&    u, const sc_signed&    v);
  friend   sc_signed operator << (const sc_signed&    u, int64               v); 
  friend   sc_signed operator << (const sc_signed&    u, uint64              v); 
  friend   sc_signed operator << (const sc_signed&    u, long                v); 
  friend   sc_signed operator << (const sc_signed&    u, unsigned long       v);
  friend   sc_signed operator << (const sc_signed&    u, int                 v) 
    { return operator<<(u, (long) v); }
  friend   sc_signed operator << (const sc_signed&    u, unsigned int        v) 
    { return operator<<(u, (unsigned long) v); }

  sc_signed& operator <<= (const sc_signed&    v); 
  sc_signed& operator <<= (const sc_unsigned&  v); 
  sc_signed& operator <<= (int64               v); 
  sc_signed& operator <<= (uint64              v); 
  sc_signed& operator <<= (long                v); 
  sc_signed& operator <<= (unsigned long       v); 
  sc_signed& operator <<= (int                 v) 
    { return operator<<=((long) v); }
  sc_signed& operator <<= (unsigned int        v) 
    { return operator<<=((unsigned long) v); }

  friend   sc_signed operator << (const sc_signed&    u, const sc_int_base&  v); 
  friend   sc_signed operator << (const sc_signed&    u, const sc_uint_base& v); 
  sc_signed& operator <<= (const sc_int_base&  v);
  sc_signed& operator <<= (const sc_uint_base& v);

  // RIGHT SHIFT operators:
   
  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator >> (const sc_signed&    u, const sc_unsigned&  v); 

  friend   sc_signed operator >> (const sc_signed&    u, const sc_signed&    v);
  friend   sc_signed operator >> (const sc_signed&    u, int64               v); 
  friend   sc_signed operator >> (const sc_signed&    u, uint64              v); 
  friend   sc_signed operator >> (const sc_signed&    u, long                v); 
  friend   sc_signed operator >> (const sc_signed&    u, unsigned long       v);
  friend   sc_signed operator >> (const sc_signed&    u, int                 v) 
    { return operator>>(u, (long) v); }
  friend   sc_signed operator >> (const sc_signed&    u, unsigned int        v) 
    { return operator>>(u, (unsigned long) v); }

  sc_signed& operator >>= (const sc_signed&    v); 
  sc_signed& operator >>= (const sc_unsigned&  v); 
  sc_signed& operator >>= (int64               v); 
  sc_signed& operator >>= (uint64              v); 
  sc_signed& operator >>= (long                v); 
  sc_signed& operator >>= (unsigned long       v); 
  sc_signed& operator >>= (int                 v) 
    { return operator>>=((long) v); }
  sc_signed& operator >>= (unsigned int        v) 
    { return operator>>=((unsigned long) v); }

  friend sc_signed operator >> (const sc_signed&    u, const sc_int_base&  v); 
  friend sc_signed operator >> (const sc_signed&    u, const sc_uint_base& v); 
  sc_signed& operator >>= (const sc_int_base&  v);
  sc_signed& operator >>= (const sc_uint_base& v);

  // Unary arithmetic operators
  friend sc_signed operator + (const sc_signed&   u);
  friend sc_signed operator - (const sc_signed&   u); 
  friend sc_signed operator - (const sc_unsigned& u);

  // LOGICAL OPERATORS:

  // Logical EQUAL operators:
   
  friend bool operator == (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator == (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator == (const sc_signed&    u, const sc_signed&    v);
  friend bool operator == (const sc_signed&    u, int64               v); 
  friend bool operator == (const sc_signed&    u, uint64              v); 
  friend bool operator == (const sc_signed&    u, long                v); 
  friend bool operator == (const sc_signed&    u, unsigned long       v);
  friend bool operator == (const sc_signed&    u, int                 v) 
    { return operator==(u, (long) v); }
  friend bool operator == (const sc_signed&    u, unsigned int        v) 
    { return operator==(u, (unsigned long) v); }

  friend bool operator == (int64               u, const sc_signed&    v); 
  friend bool operator == (uint64              u, const sc_signed&    v); 
  friend bool operator == (long                u, const sc_signed&    v); 
  friend bool operator == (unsigned long       u, const sc_signed&    v);
  friend bool operator == (int                 u, const sc_signed&    v)  
    { return operator==((long) u, v); } 
  friend bool operator == (unsigned int        u, const sc_signed&    v)  
    { return operator==((unsigned long) u, v); } 

  friend bool operator == (const sc_signed&    u, const sc_int_base&  v); 
  friend bool operator == (const sc_signed&    u, const sc_uint_base& v); 
  friend bool operator == (const sc_int_base&  u, const sc_signed&    v); 
  friend bool operator == (const sc_uint_base& u, const sc_signed&    v); 

  // Logical NOT_EQUAL operators:
   
  friend bool operator != (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator != (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator != (const sc_signed&    u, const sc_signed&    v);
  friend bool operator != (const sc_signed&    u, int64               v); 
  friend bool operator != (const sc_signed&    u, uint64              v); 
  friend bool operator != (const sc_signed&    u, long                v); 
  friend bool operator != (const sc_signed&    u, unsigned long       v);
  friend bool operator != (const sc_signed&    u, int                 v) 
    { return operator!=(u, (long) v); }
  friend bool operator != (const sc_signed&    u, unsigned int        v) 
    { return operator!=(u, (unsigned long) v); }

  friend bool operator != (int64               u, const sc_signed&    v); 
  friend bool operator != (uint64              u, const sc_signed&    v); 
  friend bool operator != (long                u, const sc_signed&    v); 
  friend bool operator != (unsigned long       u, const sc_signed&    v);
  friend bool operator != (int                 u, const sc_signed&    v)  
    { return operator!=((long) u, v); } 
  friend bool operator != (unsigned int        u, const sc_signed&    v)  
    { return operator!=((unsigned long) u, v); } 

  friend bool operator != (const sc_signed&    u, const sc_int_base&  v); 
  friend bool operator != (const sc_signed&    u, const sc_uint_base& v); 
  friend bool operator != (const sc_int_base&  u, const sc_signed&    v); 
  friend bool operator != (const sc_uint_base& u, const sc_signed&    v); 

  // Logical LESS_THAN operators:
   
  friend bool operator < (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator < (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator < (const sc_signed&    u, const sc_signed&    v);
  friend bool operator < (const sc_signed&    u, int64               v); 
  friend bool operator < (const sc_signed&    u, uint64              v); 
  friend bool operator < (const sc_signed&    u, long                v); 
  friend bool operator < (const sc_signed&    u, unsigned long       v);
  friend bool operator < (const sc_signed&    u, int                 v) 
    { return operator<(u, (long) v); }
  friend bool operator < (const sc_signed&    u, unsigned int        v) 
    { return operator<(u, (unsigned long) v); }

  friend bool operator < (int64               u, const sc_signed&    v); 
  friend bool operator < (uint64              u, const sc_signed&    v); 
  friend bool operator < (long                u, const sc_signed&    v); 
  friend bool operator < (unsigned long       u, const sc_signed&    v);
  friend bool operator < (int                 u, const sc_signed&    v)  
    { return operator<((long) u, v); } 
  friend bool operator < (unsigned int        u, const sc_signed&    v)  
    { return operator<((unsigned long) u, v); } 

  friend bool operator < (const sc_signed&    u, const sc_int_base&  v); 
  friend bool operator < (const sc_signed&    u, const sc_uint_base& v); 
  friend bool operator < (const sc_int_base&  u, const sc_signed&    v); 
  friend bool operator < (const sc_uint_base& u, const sc_signed&    v); 

  // Logical LESS_THAN_AND_EQUAL operators:
   
  friend bool operator <= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator <= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator <= (const sc_signed&    u, const sc_signed&    v);
  friend bool operator <= (const sc_signed&    u, int64               v); 
  friend bool operator <= (const sc_signed&    u, uint64              v); 
  friend bool operator <= (const sc_signed&    u, long                v); 
  friend bool operator <= (const sc_signed&    u, unsigned long       v);
  friend bool operator <= (const sc_signed&    u, int                 v) 
    { return operator<=(u, (long) v); }
  friend bool operator <= (const sc_signed&    u, unsigned int        v) 
    { return operator<=(u, (unsigned long) v); }

  friend bool operator <= (int64               u, const sc_signed&    v); 
  friend bool operator <= (uint64              u, const sc_signed&    v); 
  friend bool operator <= (long                u, const sc_signed&    v); 
  friend bool operator <= (unsigned long       u, const sc_signed&    v);
  friend bool operator <= (int                 u, const sc_signed&    v)  
    { return operator<=((long) u, v); } 
  friend bool operator <= (unsigned int        u, const sc_signed&    v)  
    { return operator<=((unsigned long) u, v); } 

  friend bool operator <= (const sc_signed&    u, const sc_int_base&  v); 
  friend bool operator <= (const sc_signed&    u, const sc_uint_base& v); 
  friend bool operator <= (const sc_int_base&  u, const sc_signed&    v); 
  friend bool operator <= (const sc_uint_base& u, const sc_signed&    v); 

  // Logical GREATER_THAN operators:
   
  friend bool operator > (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator > (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator > (const sc_signed&    u, const sc_signed&    v);
  friend bool operator > (const sc_signed&    u, int64               v); 
  friend bool operator > (const sc_signed&    u, uint64              v); 
  friend bool operator > (const sc_signed&    u, long                v); 
  friend bool operator > (const sc_signed&    u, unsigned long       v);
  friend bool operator > (const sc_signed&    u, int                 v) 
    { return operator>(u, (long) v); }
  friend bool operator > (const sc_signed&    u, unsigned int        v) 
    { return operator>(u, (unsigned long) v); }

  friend bool operator > (int64               u, const sc_signed&    v); 
  friend bool operator > (uint64              u, const sc_signed&    v); 
  friend bool operator > (long                u, const sc_signed&    v); 
  friend bool operator > (unsigned long       u, const sc_signed&    v);
  friend bool operator > (int                 u, const sc_signed&    v)  
    { return operator>((long) u, v); } 
  friend bool operator > (unsigned int        u, const sc_signed&    v)  
    { return operator>((unsigned long) u, v); } 

  friend bool operator > (const sc_signed&    u, const sc_int_base&  v); 
  friend bool operator > (const sc_signed&    u, const sc_uint_base& v); 
  friend bool operator > (const sc_int_base&  u, const sc_signed&    v); 
  friend bool operator > (const sc_uint_base& u, const sc_signed&    v); 

  // Logical GREATER_THAN_AND_EQUAL operators:
   
  friend bool operator >= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator >= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator >= (const sc_signed&    u, const sc_signed&    v);
  friend bool operator >= (const sc_signed&    u, int64               v); 
  friend bool operator >= (const sc_signed&    u, uint64              v); 
  friend bool operator >= (const sc_signed&    u, long                v); 
  friend bool operator >= (const sc_signed&    u, unsigned long       v);
  friend bool operator >= (const sc_signed&    u, int                 v) 
    { return operator>=(u, (long) v); }
  friend bool operator >= (const sc_signed&    u, unsigned int        v) 
    { return operator>=(u, (unsigned long) v); }

  friend bool operator >= (int64               u, const sc_signed&    v); 
  friend bool operator >= (uint64              u, const sc_signed&    v); 
  friend bool operator >= (long                u, const sc_signed&    v); 
  friend bool operator >= (unsigned long       u, const sc_signed&    v);
  friend bool operator >= (int                 u, const sc_signed&    v)  
    { return operator>=((long) u, v); } 
  friend bool operator >= (unsigned int        u, const sc_signed&    v)  
    { return operator>=((unsigned long) u, v); } 

  friend bool operator >= (const sc_signed&    u, const sc_int_base&  v); 
  friend bool operator >= (const sc_signed&    u, const sc_uint_base& v); 
  friend bool operator >= (const sc_int_base&  u, const sc_signed&    v); 
  friend bool operator >= (const sc_uint_base& u, const sc_signed&    v); 

  // Bitwise NOT operator (unary).
  friend sc_signed operator ~ (const sc_signed& u); 

  // Helper functions.
  friend sc_signed add_signed_friend(small_type us, 
                                     int unb,
                                     int und, 
                                     const unsigned long *ud, 
                                     small_type vs, 
                                     int vnb,
                                     int vnd,
                                     const unsigned long *vd);

  friend sc_signed sub_signed_friend(small_type us, 
                                     int unb,
                                     int und, 
                                     const unsigned long *ud, 
                                     small_type vs, 
                                     int vnb,
                                     int vnd, 
                                     const unsigned long *vd);
  
  friend sc_signed mul_signed_friend(small_type s,
                                     int unb,
                                     int und, 
                                     const unsigned long *ud, 
                                     int vnb,
                                     int vnd,
                                     const unsigned long *vd);
  
  friend sc_signed div_signed_friend(small_type s,
                                     int unb,
                                     int und, 
                                     const unsigned long *ud, 
                                     int vnb,
                                     int vnd,
                                     const unsigned long *vd);
  
  friend sc_signed mod_signed_friend(small_type us,
                                     int unb,
                                     int und, 
                                     const unsigned long *ud, 
                                     int vnb,
                                     int vnd,
                                     const unsigned long *vd);
  
  friend sc_signed and_signed_friend(small_type us, 
                                     int unb, 
                                     int und, 
                                     const unsigned long *ud, 
                                     small_type vs,
                                     int vnb, 
                                     int vnd,
                                     const unsigned long *vd);
  
  friend sc_signed or_signed_friend(small_type us, 
                                    int unb, 
                                    int und, 
                                    const unsigned long *ud, 
                                    small_type vs,
                                    int vnb, 
                                    int vnd,
                                    const unsigned long *vd);
  
  friend sc_signed xor_signed_friend(small_type us, 
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
  sc_signed(const sc_signed&   v, small_type s);
  sc_signed(const sc_unsigned& v, small_type s);

  // Create a signed number with the given attributes.
  sc_signed(small_type s, int nb, int nd, 
            unsigned long *d, bool alloc = true);

 // Create a signed number using the bits u[l..r].
  sc_signed(const sc_signed* u, int l, int r);

  // Private member functions. The called functions are inline functions.

  small_type default_sign() const
    { return SC_NOSIGN; }

  int num_bits(int nb) const { return nb; }

  bool check_if_outside(int bit_num) const;

  void copy_digits(int nb, int nd, const unsigned long *d)
    { copy_digits_signed(sgn, nbits, ndigits, digit, nb, nd, d); }

  void makezero()
    { sgn = make_zero(ndigits, digit); }

  // Conversion functions between 2's complement (2C) and
  // sign-magnitude (SM):
  void convert_2C_to_SM()
    { sgn = convert_signed_2C_to_SM(nbits, ndigits, digit); }

  void convert_SM_to_2C_to_SM()
    { sgn = convert_signed_SM_to_2C_to_SM(sgn, nbits, ndigits, digit); }

  void convert_SM_to_2C()
    { convert_signed_SM_to_2C(sgn, ndigits, digit); }

};


// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( const sc_signed&, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( const sc_signed&, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( const sc_signed&, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( const sc_signed&, const sc_signed& );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( const sc_signed&, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( bool, const sc_signed& );


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( const sc_signed&, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( const sc_signed&, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( const sc_signed&, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( const sc_signed&, const sc_signed& );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( const sc_signed&, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( bool, const sc_signed& );


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( const sc_signed&, sc_signed_concref<T1,T2> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed&, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( const sc_signed&, sc_signed_bitref );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( sc_signed&, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( const sc_signed&, sc_signed_subref );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( sc_signed&, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( const sc_signed&, sc_signed& );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( sc_signed&, const sc_signed& );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( sc_signed&, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( bool, sc_signed& );


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( const sc_signed&, sc_signed_concref<T1,T2> );

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( sc_signed&, sc_signed_concref_r<T1,T2> );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( const sc_signed&, sc_signed_bitref );

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( sc_signed&, sc_signed_bitref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( const sc_signed&, sc_signed_subref );

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( sc_signed&, sc_signed_subref_r );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( const sc_signed&, sc_signed& );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( sc_signed&, const sc_signed& );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( sc_signed&, bool );

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( bool, sc_signed& );

#endif


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref<sc_signed,sc_signed_concref<T1,T2> >
operator , ( sc_signed&, sc_signed_concref<T1,T2> );

inline
sc_signed_concref<sc_signed,sc_signed_bitref>
operator , ( sc_signed&, sc_signed_bitref );

inline
sc_signed_concref<sc_signed,sc_signed_subref>
operator , ( sc_signed&, sc_signed_subref );

inline
sc_signed_concref<sc_signed,sc_signed>
operator , ( sc_signed&, sc_signed& );


template <class T1, class T2>
inline
sc_signed_concref<sc_signed,sc_signed_concref<T1,T2> >
concat( sc_signed&, sc_signed_concref<T1,T2> );

inline
sc_signed_concref<sc_signed,sc_signed_bitref>
concat( sc_signed&, sc_signed_bitref );

inline
sc_signed_concref<sc_signed,sc_signed_subref>
concat( sc_signed&, sc_signed_subref );

inline
sc_signed_concref<sc_signed,sc_signed>
concat( sc_signed&, sc_signed& );


inline
ostream&
operator << ( ostream&, const sc_signed& );

inline
istream&
operator >> ( istream&, sc_signed& );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_signed_bitref_r
//
//  Proxy class for sc_signed bit selection (r-value only).
// ----------------------------------------------------------------------------

// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_bitref_r a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
operator , ( sc_signed_bitref_r a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
operator , ( sc_signed_bitref_r a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref_r a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref_r a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( bool a, sc_signed_bitref_r b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_bitref_r a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
concat( sc_signed_bitref_r a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
concat( sc_signed_bitref_r a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref_r a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref_r a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( bool a, sc_signed_bitref_r b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_bitref_r a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_bitref a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
operator , ( sc_signed_bitref_r a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
operator , ( sc_signed_bitref a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
operator , ( sc_signed_bitref_r a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
operator , ( sc_signed_bitref a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref_r a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
operator , ( sc_signed_bitref a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( bool a, sc_signed_bitref b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_bitref_r a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_bitref a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
concat( sc_signed_bitref_r a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>
concat( sc_signed_bitref a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
concat( sc_signed_bitref_r a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>
concat( sc_signed_bitref a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref_r a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_bitref_r,sc_signed>
concat( sc_signed_bitref a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_bitref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( bool a, sc_signed_bitref b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	aa, *b.clone(), 3 );
}

#endif


inline
ostream&
operator << ( ostream& os, const sc_signed_bitref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_bitref
//
//  Proxy class for sc_signed bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_bitref,sc_signed_concref<T1,T2> >
operator , ( sc_signed_bitref a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_bitref,sc_signed_bitref>
operator , ( sc_signed_bitref a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_bitref,sc_signed_subref>
operator , ( sc_signed_bitref a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_bitref,sc_signed>
operator , ( sc_signed_bitref a, sc_signed& b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
sc_signed_concref<sc_signed_bitref,sc_signed_concref<T1,T2> >
concat( sc_signed_bitref a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_bitref,sc_signed_bitref>
concat( sc_signed_bitref a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_bitref,sc_signed_subref>
concat( sc_signed_bitref a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_bitref,sc_signed>
concat( sc_signed_bitref a, sc_signed& b )
{
    return sc_signed_concref<sc_signed_bitref,sc_signed>(
	*a.clone(), b, 1 );
}


inline
istream&
operator >> ( istream& is, sc_signed_bitref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref_r
//
//  Proxy class for sc_signed part selection (r-value only).
// ----------------------------------------------------------------------------

// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_subref_r a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_subref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
operator , ( sc_signed_subref_r a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
operator , ( sc_signed_subref_r a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref_r a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref_r a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( bool a, sc_signed_subref_r b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_subref_r a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_subref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
concat( sc_signed_subref_r a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
concat( sc_signed_subref_r a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref_r a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref_r a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( bool a, sc_signed_subref_r b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_subref_r a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_subref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed_subref a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_subref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
operator , ( sc_signed_subref_r a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
operator , ( sc_signed_subref a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
operator , ( sc_signed_subref_r a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
operator , ( sc_signed_subref a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref_r a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
operator , ( sc_signed_subref a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( bool a, sc_signed_subref b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	aa, *b.clone(), 3 );
}


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_subref_r a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_subref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_concref_r<T1,T2> >
concat( sc_signed_subref a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed_subref_r,
	                       sc_signed_concref_r<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
concat( sc_signed_subref_r a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>
concat( sc_signed_subref a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
concat( sc_signed_subref_r a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>
concat( sc_signed_subref a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref_r a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), b, 1 );
}

inline
sc_signed_concref_r<sc_signed_subref_r,sc_signed>
concat( sc_signed_subref a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_subref_r,sc_signed>(
	*a.clone(), bb, 3 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( bool a, sc_signed_subref b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	aa, *b.clone(), 3 );
}

#endif


inline
ostream&
operator << ( ostream& os, const sc_signed_subref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref
//
//  Proxy class for sc_signed part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

inline
sc_signed_subref&
sc_signed_subref::operator = ( const char* a )
{
    sc_signed aa( length() );
    return ( *this = aa = a );
}


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_subref,sc_signed_concref<T1,T2> >
operator , ( sc_signed_subref a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_subref,sc_signed_bitref>
operator , ( sc_signed_subref a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_subref,sc_signed_subref>
operator , ( sc_signed_subref a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_subref,sc_signed>
operator , ( sc_signed_subref a, sc_signed& b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
sc_signed_concref<sc_signed_subref,sc_signed_concref<T1,T2> >
concat( sc_signed_subref a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed_concref<T1,T2> >(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_subref,sc_signed_bitref>
concat( sc_signed_subref a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed_bitref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_subref,sc_signed_subref>
concat( sc_signed_subref a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed_subref>(
	*a.clone(), *b.clone(), 3 );
}

inline
sc_signed_concref<sc_signed_subref,sc_signed>
concat( sc_signed_subref a, sc_signed& b )
{
    return sc_signed_concref<sc_signed_subref,sc_signed>(
	*a.clone(), b, 1 );
}


inline
istream&
operator >> ( istream& is, sc_signed_subref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_signed_concref<T1,T2>
//
//  Proxy class for sc_signed concatenation (r-value only).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

// destructor

template <class T1, class T2>
inline
sc_signed_concref_r<T1,T2>::~sc_signed_concref_r()
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
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed_concref_r<T3,T4> b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref_r<T1,T2> a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref_r<T1,T2> a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( bool a, sc_signed_concref_r<T1,T2> b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
concat( sc_signed_concref_r<T1,T2> a, sc_signed_concref_r<T3,T4> b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
concat( sc_signed_concref_r<T1,T2> a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
concat( sc_signed_concref_r<T1,T2> a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref_r<T1,T2> a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref_r<T1,T2> a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( bool a, sc_signed_concref_r<T1,T2> b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed_concref<T3,T4> b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
operator , ( sc_signed_concref<T1,T2> a, sc_signed_concref_r<T3,T4> b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
operator , ( sc_signed_concref<T1,T2> a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
operator , ( sc_signed_concref<T1,T2> a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref_r<T1,T2> a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref<T1,T2> a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
operator , ( sc_signed_concref<T1,T2> a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( bool a, sc_signed_concref<T1,T2> b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
concat( sc_signed_concref_r<T1,T2> a, sc_signed_concref<T3,T4> b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
                    sc_signed_concref_r<T3,T4> >
concat( sc_signed_concref<T1,T2> a, sc_signed_concref_r<T3,T4> b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_concref_r<T3,T4> >(
        *a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
concat( sc_signed_concref_r<T1,T2> a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_bitref_r>
concat( sc_signed_concref<T1,T2> a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_bitref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
concat( sc_signed_concref_r<T1,T2> a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed_subref_r>
concat( sc_signed_concref<T1,T2> a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,
	                       sc_signed_subref_r>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref_r<T1,T2> a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref<T1,T2> a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>
concat( sc_signed_concref<T1,T2> a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed_concref_r<T1,T2>,sc_signed>(
	*a.clone(), bb, 3 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( bool a, sc_signed_concref<T1,T2> b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	aa, *b.clone(), 3 );
}

#endif


template <class T1, class T2>
inline
ostream&
operator << ( ostream& os, const sc_signed_concref_r<T1,T2>& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_signed_concref<T1,T2>
//
//  Proxy class for sc_signed concatenation (r-value and l-value).
//  NOT YET IMPLEMENTED
// ----------------------------------------------------------------------------

// l-value concatenation operators and functions

template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_concref<T3,T4> >
operator , ( sc_signed_concref<T1,T2> a, sc_signed_concref<T3,T4> b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,
                             sc_signed_concref<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_bitref>
operator , ( sc_signed_concref<T1,T2> a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_subref>
operator , ( sc_signed_concref<T1,T2> a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed>
operator , ( sc_signed_concref<T1,T2> a, sc_signed& b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}


template <class T1, class T2, class T3, class T4>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_concref<T3,T4> >
concat( sc_signed_concref<T1,T2> a, sc_signed_concref<T3,T4> b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,
                             sc_signed_concref<T3,T4> >(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_bitref>
concat( sc_signed_concref<T1,T2> a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_bitref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_subref>
concat( sc_signed_concref<T1,T2> a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed_subref>(
	*a.clone(), *b.clone(), 3 );
}

template <class T1, class T2>
inline
sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed>
concat( sc_signed_concref<T1,T2> a, sc_signed& b )
{
    return sc_signed_concref<sc_signed_concref<T1,T2>,sc_signed>(
	*a.clone(), b, 1 );
}


template <class T1, class T2>
inline
istream&
operator >> ( istream& is, sc_signed_concref<T1,T2>& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed
//
//  Arbitrary precision signed number.
// ----------------------------------------------------------------------------

// r-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( const sc_signed& a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( const sc_signed& a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( const sc_signed& a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( const sc_signed& a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, b );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( const sc_signed& a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, bb, 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( bool a, const sc_signed& b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	aa, b, 1 );
}


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( const sc_signed& a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( const sc_signed& a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( const sc_signed& a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( const sc_signed& a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, b );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( const sc_signed& a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, bb, 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( bool a, const sc_signed& b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	aa, b, 1 );
}


#ifdef SC_DT_MIXED_COMMA_OPERATORS

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( const sc_signed& a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
operator , ( sc_signed& a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( const sc_signed& a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
operator , ( sc_signed& a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( const sc_signed& a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
operator , ( sc_signed& a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( const sc_signed& a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, b );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( sc_signed& a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, b );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( sc_signed& a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, bb, 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
operator , ( bool a, sc_signed& b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	aa, b, 1 );
}


template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( const sc_signed& a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

template <class T1, class T2>
inline
sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >
concat( sc_signed& a, sc_signed_concref_r<T1,T2> b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_concref_r<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( const sc_signed& a, sc_signed_bitref b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_bitref_r>
concat( sc_signed& a, sc_signed_bitref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_bitref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( const sc_signed& a, sc_signed_subref b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed_subref_r>
concat( sc_signed& a, sc_signed_subref_r b )
{
    return sc_signed_concref_r<sc_signed,sc_signed_subref_r>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( const sc_signed& a, sc_signed& b )
{
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, b );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( sc_signed& a, const sc_signed& b )
{
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, b );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( sc_signed& a, bool b )
{
    sc_signed& bb = *new sc_signed( 1 );
    bb = (b ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	a, bb, 2 );
}

inline
sc_signed_concref_r<sc_signed,sc_signed>
concat( bool a, sc_signed& b )
{
    sc_signed& aa = *new sc_signed( 1 );
    aa = (a ? -1 : 0);
    return sc_signed_concref_r<sc_signed,sc_signed>(
	aa, b, 1 );
}

#endif


// l-value concatenation operators and functions

template <class T1, class T2>
inline
sc_signed_concref<sc_signed,sc_signed_concref<T1,T2> >
operator , ( sc_signed& a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref<sc_signed,sc_signed_concref<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref<sc_signed,sc_signed_bitref>
operator , ( sc_signed& a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed,sc_signed_bitref>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref<sc_signed,sc_signed_subref>
operator , ( sc_signed& a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed,sc_signed_subref>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref<sc_signed,sc_signed>
operator , ( sc_signed& a, sc_signed& b )
{
    return sc_signed_concref<sc_signed,sc_signed>(
	a, b );
}


template <class T1, class T2>
inline
sc_signed_concref<sc_signed,sc_signed_concref<T1,T2> >
concat( sc_signed& a, sc_signed_concref<T1,T2> b )
{
    return sc_signed_concref<sc_signed,sc_signed_concref<T1,T2> >(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref<sc_signed,sc_signed_bitref>
concat( sc_signed& a, sc_signed_bitref b )
{
    return sc_signed_concref<sc_signed,sc_signed_bitref>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref<sc_signed,sc_signed_subref>
concat( sc_signed& a, sc_signed_subref b )
{
    return sc_signed_concref<sc_signed,sc_signed_subref>(
	a, *b.clone(), 2 );
}

inline
sc_signed_concref<sc_signed,sc_signed>
concat( sc_signed& a, sc_signed& b )
{
    return sc_signed_concref<sc_signed,sc_signed>(
	a, b );
}


inline
ostream&
operator << ( ostream& os, const sc_signed& a )
{
    a.print( os );
    return os;
}

inline
istream&
operator >> ( istream& is, sc_signed& a )
{
    a.scan( is );
    return is;
}


#undef NOT_YET_IMPLEMENTED

} // namespace sc_dt


#endif
