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

  sc_bit.h -- Bit class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_BIT_H
#define SC_BIT_H


#include "systemc/datatypes/bit/sc_logic.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_bit
//
//  Bit class.
//  Note: VSIA compatibility indicated.
// ----------------------------------------------------------------------------

class sc_bit
{
private:

    bool m_val;

    static bool from_int( int );
    static bool from_char( char );

public:

    // constructors
    // MANDATORY

    sc_bit()
	: m_val( false )
	{}
    explicit sc_bit( bool r )
	: m_val( r )
	{}
    explicit sc_bit( int r )
	: m_val( from_int( r ) )
	{}
    explicit sc_bit( char r )
	: m_val( from_char( r ) )
	{}
    explicit sc_bit( const sc_logic& r )  // non-VSIA
	: m_val( r.to_bool() )
	{}


    // copy constructor
    // MANDATORY

    sc_bit( const sc_bit& r )
	: m_val( r.m_val )
	{}


    // destructor
    // MANDATORY

    ~sc_bit()
	{}


    // assignments
    // MANDATORY

    sc_bit& operator = ( const sc_bit& r )
	{ m_val = r.m_val; return *this; }
    sc_bit& operator = ( int r )
	{ m_val = from_int( r ); return *this; }
    sc_bit& operator = ( bool r )
	{ m_val = r; return *this; }
    sc_bit& operator = ( char r )
	{ m_val = from_char( r ); return *this; }
    sc_bit& operator = ( const sc_logic& r )  // non-VSIA
	{ m_val = r.to_bool(); return *this; }

    sc_bit& operator &= ( const sc_bit& r )
	{ m_val = ( m_val && r.m_val ); return *this; }
    sc_bit& operator &= ( int r )
	{ m_val = ( m_val && from_int( r ) ); return *this; }
    sc_bit& operator &= ( bool r )
	{ m_val = ( m_val && r ); return *this; }
    sc_bit& operator &= ( char r )
	{ m_val = ( m_val && from_char( r ) ); return *this; }

    sc_bit& operator |= ( const sc_bit& r )
	{ m_val = ( m_val || r.m_val ); return *this; }
    sc_bit& operator |= ( int r )
	{ m_val = ( m_val || from_int( r ) ); return *this; }
    sc_bit& operator |= ( bool r )
	{ m_val = ( m_val || r ); return *this; }
    sc_bit& operator |= ( char r )
	{ m_val = ( m_val || from_char( r ) ); return *this; }

    sc_bit& operator ^= ( const sc_bit& r )
	{ m_val = ( m_val != r.m_val ); return *this; }
    sc_bit& operator ^= ( int r )
	{ m_val = ( m_val != from_int( r ) ); return *this; }
    sc_bit& operator ^= ( bool r )
	{ m_val = ( m_val != r ); return *this; }
    sc_bit& operator ^= ( char r )
	{ m_val = ( m_val != from_char( r ) ); return *this; }


    // conversions
    // MANDATORY

    operator bool() const
	{ return m_val; }
    bool to_bool() const  // non-VSIA
	{ return m_val; }
    char to_char() const
	{ return ( m_val ? '1' : '0' ); }


// ----------------------------------------------------------------------------
//  VSIA compatibility issues
// ----------------------------------------------------------------------------

    // operators

    // relational operators

///     // gcc 2.95.2 has a bug that causes ambiguity with many types
/// #if !defined(__GNUC__)
    // MANDATORY
    friend bool operator == (const sc_bit&, const sc_bit&);
    friend bool operator == (const sc_bit&, int);
    friend bool operator == (const sc_bit&, bool);
    friend bool operator == (const sc_bit&, char);
    friend bool operator == (int, const sc_bit&);
    friend bool operator == (bool, const sc_bit&);
    friend bool operator == (char, const sc_bit&);
/// #endif
    // OPTIONAL
    friend bool equal (const sc_bit&, const sc_bit&);
    friend bool equal (const sc_bit&, int);
    friend bool equal (const sc_bit&, bool);
    friend bool equal (const sc_bit&, char);
    friend bool equal (int, const sc_bit&);
    friend bool equal (bool, const sc_bit&);
    friend bool equal (char, const sc_bit&);

    // MANDATORY
/// #if !defined(__GNUC__)
    friend bool operator != (const sc_bit&, const sc_bit&);
    friend bool operator != (const sc_bit&, int);
    friend bool operator != (const sc_bit&, bool);
    friend bool operator != (const sc_bit&, char);
    friend bool operator != (int, const sc_bit&);
    friend bool operator != (bool, const sc_bit&);
    friend bool operator != (char, const sc_bit&);
/// #endif
    // OPTIONAL
    friend bool not_equal (const sc_bit&, const sc_bit&);
    friend bool not_equal (const sc_bit&, int);
    friend bool not_equal (const sc_bit&, bool);
    friend bool not_equal (const sc_bit&, char);
    friend bool not_equal (int, const sc_bit&);
    friend bool not_equal (bool, const sc_bit&);
    friend bool not_equal (char, const sc_bit&);

    // bitwise operators and functions

    // bitwise complement

/// #if !defined(__GNUC__)
    // MANDATORY
    friend const sc_bit operator ~ (const sc_bit&);
/// #endif
    // OPTIONAL (RECOMMENDED?)
    sc_bit& b_not ();

    // OPTIONAL
    friend const sc_bit b_not (const sc_bit&);
    // RECOMMENDED
    friend void b_not (sc_bit& r, const sc_bit&);


    // bitwise or
    // MANDATORY
/// #if !defined(__GNUC__)
    friend const sc_bit operator | (const sc_bit&, const sc_bit&);
    friend const sc_bit operator | (const sc_bit&, int);
    friend const sc_bit operator | (const sc_bit&, bool);
    friend const sc_bit operator | (const sc_bit&, char);
    friend const sc_bit operator | (int, const sc_bit&);
    friend const sc_bit operator | (bool, const sc_bit&);
    friend const sc_bit operator | (char, const sc_bit&);
/// #endif
    // OPTIONAL
    friend const sc_bit b_or (const sc_bit&, const sc_bit&);
    friend const sc_bit b_or (const sc_bit&, int);
    friend const sc_bit b_or (const sc_bit&, bool);
    friend const sc_bit b_or (const sc_bit&, char);
    friend const sc_bit b_or (int, const sc_bit&);
    friend const sc_bit b_or (bool, const sc_bit&);
    friend const sc_bit b_or (char, const sc_bit&);

    // RECOMMENDED
    friend void b_or (sc_bit& r, const sc_bit&, const sc_bit&);
    friend void b_or (sc_bit& r, const sc_bit&, int);
    friend void b_or (sc_bit& r, const sc_bit&, bool);
    friend void b_or (sc_bit& r, const sc_bit&, char);
    friend void b_or (sc_bit& r, int, const sc_bit&);
    friend void b_or (sc_bit& r, bool, const sc_bit&);
    friend void b_or (sc_bit& r, char, const sc_bit&);


    // bitwise and

    // MANDATORY
/// #if !defined(__GNUC__)
    friend const sc_bit operator & (const sc_bit&, const sc_bit&);
    friend const sc_bit operator & (const sc_bit&, int);
    friend const sc_bit operator & (const sc_bit&, bool);
    friend const sc_bit operator & (const sc_bit&, char);
    friend const sc_bit operator & (int, const sc_bit&);
    friend const sc_bit operator & (bool, const sc_bit&);
    friend const sc_bit operator & (char, const sc_bit&);
/// #endif

    // OPTIONAL
    friend const sc_bit b_and (const sc_bit&, const sc_bit&);
    friend const sc_bit b_and (const sc_bit&, int);
    friend const sc_bit b_and (const sc_bit&, bool);
    friend const sc_bit b_and (const sc_bit&, char);
    friend const sc_bit b_and (int, const sc_bit&);
    friend const sc_bit b_and (bool, const sc_bit&);
    friend const sc_bit b_and (char, const sc_bit&);
    // RECOMMENDED
    friend void b_and (sc_bit& r, const sc_bit&, const sc_bit&);
    friend void b_and (sc_bit& r, const sc_bit&, int);
    friend void b_and (sc_bit& r, const sc_bit&, bool);
    friend void b_and (sc_bit& r, const sc_bit&, char);
    friend void b_and (sc_bit& r, int, const sc_bit&);
    friend void b_and (sc_bit& r, bool, const sc_bit&);
    friend void b_and (sc_bit& r, char, const sc_bit&);

    // bitwise exor

    // MANDATORY
/// #if !defined(__GNUC__)
    friend const sc_bit operator ^ (const sc_bit&, const sc_bit&);
    friend const sc_bit operator ^ (const sc_bit&, int);
    friend const sc_bit operator ^ (const sc_bit&, bool);
    friend const sc_bit operator ^ (const sc_bit&, char);
    friend const sc_bit operator ^ (int, const sc_bit&);
    friend const sc_bit operator ^ (bool, const sc_bit&);
    friend const sc_bit operator ^ (char, const sc_bit&);
/// #endif

    // OPTIONAL
    friend const sc_bit b_xor (const sc_bit&, const sc_bit&);
    friend const sc_bit b_xor (const sc_bit&, int);
    friend const sc_bit b_xor (const sc_bit&, bool);
    friend const sc_bit b_xor (const sc_bit&, char);
    friend const sc_bit b_xor (int, const sc_bit&);
    friend const sc_bit b_xor (bool, const sc_bit&);
    friend const sc_bit b_xor (char, const sc_bit&);


    // RECOMMENDED
    friend void b_xor (sc_bit& r, const sc_bit&, const sc_bit&);
    friend void b_xor (sc_bit& r, const sc_bit&, int);
    friend void b_xor (sc_bit& r, const sc_bit&, bool);
    friend void b_xor (sc_bit& r, const sc_bit&, char);
    friend void b_xor (sc_bit& r, int, const sc_bit&);
    friend void b_xor (sc_bit& r, bool, const sc_bit&);
    friend void b_xor (sc_bit& r, char, const sc_bit&);


    void print( ostream& ) const;
};


// ----------------------------------------------------------------------------

inline
ostream&
operator << ( ostream& os, const sc_bit& a )
{
    a.print( os );
    return os;
}


#endif
