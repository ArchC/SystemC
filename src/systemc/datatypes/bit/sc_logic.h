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

  sc_logic.h -- C++ implementation of logic type. Behaves
                pretty much the same way as HDLs except with 4 values.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_LOGIC_H
#define SC_LOGIC_H


#include "systemc/utils/sc_iostream.h"
#include "systemc/kernel/sc_macros.h"
#include "systemc/utils/sc_mempool.h"
#include "systemc/utils/sc_exception.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_logic
//
//  Four-valued logic type.
// ----------------------------------------------------------------------------

class sc_logic
{
public:

    enum Log_enum { Log_0 = 0, Log_1 = 1, Log_Z = 2, Log_X = 3, Log_Error };

private:

    Log_enum val;

    friend class sc_logic_resolve;

    static Log_enum test_value( long value )
    {
	// it's not obvious what to do in case of wrong value
	// at present we issue warning and assign Log_X
	if( value < Log_0 || value >= Log_Error ) {
	    char str[32];
	    sprintf( str, " sc_logic(%ld)", value );
	    REPORT_WARNING( 1006, str );
	    return Log_X;
	}
	return Log_enum( value );
    }

public:

    // conversion tables
    static const Log_enum char_to_logic[128];
    static const char logic_to_char[4];
    static const Log_enum and_table[4][4];
    static const Log_enum or_table[4][4];
    static const Log_enum xor_table[4][4];
    static const Log_enum not_table[4];

    static void* operator new( size_t, void* p )
	{ return p; } // placement-new
    static void* operator new( size_t sz )
	{ return sc_mempool::allocate( sz ); }
    static void  operator delete( void* p, size_t sz )
	{ sc_mempool::release( p, sz ); }
    static void* operator new[]( size_t sz )
	{ return sc_mempool::allocate( sz ); }
    static void  operator delete[]( void* p, size_t sz )
	{ sc_mempool::release( p, sz ); }

    // Default onstructor - assign the unknown value.
    sc_logic()
	{ val = Log_X; }
    // this constructor is not explicit because there is no
    // built-in conversion of standard types to enum
    sc_logic( Log_enum r )
	{ val = test_value( r ); }

    // Copy constructor - copy the val member.
    sc_logic( const sc_logic& r )
	{ val = r.val; }

    // the following constructors are explicit to avoid ambiguities
    // caused by implicit conversion of widely used types
    explicit sc_logic( char r )
	{ val = test_value( char_to_logic[r] ); }
    explicit sc_logic( long r )
	{ val = char_to_logic[test_value( r )]; }
    explicit sc_logic( int r )
	{ val = char_to_logic[test_value( r )]; }
    explicit sc_logic( bool r )
	{ val = r ? Log_1 : Log_0; }

    // Destructor
    ~sc_logic()
	{}

    // Bitwise operators on sc_logic
    sc_logic operator & ( const sc_logic& r ) const
        { return sc_logic( and_table[val][test_value( r.val )] ); }
    sc_logic operator | ( const sc_logic& r ) const
        { return sc_logic( or_table[val][test_value( r.val )] );  }
    sc_logic operator ^ ( const sc_logic& r ) const
        { return sc_logic( xor_table[val][test_value( r.val )] ); }
    sc_logic operator ~ () const
	{ return sc_logic( not_table[val] ); }

    // Assignment operators from sc_logic and from char
    sc_logic& operator = ( Log_enum r )
	{ val = test_value( r ); return *this; }
    sc_logic& operator = ( const sc_logic& r )
	{ val = test_value( r.val ); return *this; }
    // this is questionable whether we need assignment from char
    sc_logic& operator = ( char r )
	{ val = test_value( char_to_logic[r] ); return *this; }
    sc_logic& operator = ( long r )
	{ val = char_to_logic[test_value( r )]; return *this; }
    sc_logic& operator = ( int r )
	{ val = char_to_logic[test_value( r )]; return *this; }
    sc_logic& operator = ( bool r )
	{ val = r ? Log_1 : Log_0; return *this; }

    // op= operators
    sc_logic& operator &= ( const sc_logic& r )
        { val = and_table[val][test_value( r.val )]; return *this; }
    sc_logic& operator |= ( const sc_logic& r )
        { val = or_table[val][test_value( r.val )]; return *this; }
    sc_logic& operator ^= ( const sc_logic& r )
        { val = xor_table[val][test_value( r.val )]; return *this; }

    // Negates the value of this sc_logic
    sc_logic& negate()
	{ val = not_table[val]; return *this; }

    // Comparison operators with sc_logic and with char
    bool operator == ( const sc_logic& r ) const
	{ return (val == r.val); }
    bool operator == ( char r ) const
	{ return (val == char_to_logic[r]); }
    bool operator != ( const sc_logic& r ) const
	{ return (val != r.val); }
    bool operator != ( char r ) const
	{ return (val != char_to_logic[r]); }

    sc_logic operator ! ()
	{ return sc_logic( not_table[val] ); }

    // Type cast into char
    char to_char() const
	{ return logic_to_char[val]; }

    long to_long() const
	{ return val; }

    bool is_01() const
	{ return val == Log_0 || val == Log_1; }
    void check_01() const;

    bool to_bool() const
	{ check_01(); return val != Log_0; }
    void print( ostream& os ) const
	{ os << to_char(); }
};


// ----------------------------------------------------------------------------

#if 0

// THESE CAUSE PROBLEMS WITH IMPLICIT CONVERSION FROM sc_signal<unsigned>
// to unsigned, e.g. in the expression (foo == 5).  Spurious sc_logic
// object is created ...

inline
bool
operator == ( char a, const sc_logic& b )
{
    return ( b.operator == ( a ) );
}

inline
bool
operator != ( char a, const sc_logic& b )
{
    return ( b.operator != ( a ) );
}

#endif


// dangerous comparisons, because almost anything can be converted to bool
inline
bool
operator == ( bool a, const sc_logic& b )
{
    return b == a;
}

inline
bool
operator != ( bool a, const sc_logic& b )
{
    return b != a;
}


inline
ostream&
operator << ( ostream& os, const sc_logic& a )
{
    a.print( os );
    return os;
}


extern const sc_logic sc_logic_X;
extern const sc_logic sc_logic_0;
extern const sc_logic sc_logic_1;
extern const sc_logic sc_logic_Z;


#endif
