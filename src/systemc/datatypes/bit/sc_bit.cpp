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

  sc_bit.cpp -- Bit class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/datatypes/bit/sc_bit.h"
#include "systemc/utils/sc_exception.h"


bool
sc_bit::from_int( int r )
{
    if( r != 0 && r != 1 ) {
	REPORT_ERROR( 1006, "sc_bit" );
    }
    return ( r == 0 ? false : true );
}

bool
sc_bit::from_char( char r )
{
    if( r != '0' && r != '1' ) {
	REPORT_ERROR( 1006, "sc_bit" );
    }
    return ( r == '0' ? false : true );
}


void
sc_bit::print( ostream& os ) const
{
    os << to_char();
}


// operators

// relational operators

// MANDATORY

/// #if !defined(__GNUC__)

bool
operator == ( const sc_bit& a, const sc_bit& b )
{
    return (bool) a == (bool) b;
}

bool
operator == ( const sc_bit& a, int i )
{
    return (int) a == i;
}

bool
operator == ( const sc_bit& a, bool b )
{
    return (bool) a == b;
}

bool
operator == ( const sc_bit& a, char c )
{
    return (int) a == (int) (c-'0');
}

bool
operator == ( int i, const sc_bit& a )
{
    return a == i;
}

bool
operator == ( bool b, const sc_bit& a )
{
    return a == b;
}

bool
operator == ( char c, const sc_bit& a )
{
    return a == c;
}


// MANDATORY

bool
operator != ( const sc_bit& a, const sc_bit& b )
{
    return (bool) a != (bool) b;
}

bool
operator != ( const sc_bit& a, int i )
{
    return (int) a != i;
}

bool
operator != ( const sc_bit& a, bool b )
{
    return (bool) a!= b;
}

bool
operator != ( const sc_bit& a, char c )
{
    return (int) a != (int) (c-'0');
}

bool
operator != ( int i, const sc_bit& a )
{
    return a != i;
}

bool
operator != ( bool b, const sc_bit& a )
{
    return a != b;
}

bool
operator != ( char c, const sc_bit& a )
{
    return a != c;
}


// bitwise operators and functions

// bitwise complement

// MANDATORY

const sc_bit
operator ~ ( const sc_bit& a )
{
    sc_bit result( a );
    result.m_val = !result.m_val;
    return result;
}


// bitwise or

// MANDATORY

const sc_bit
operator | ( const sc_bit& a, const sc_bit& b )
{
    sc_bit result( a );
    return result |= b;
}

const sc_bit
operator | ( const sc_bit& a, int i )
{
    sc_bit result( a );
    return result |= i;
}

const sc_bit
operator | ( const sc_bit& a, bool b )
{
    sc_bit result( a );
    return result |= b;
}

const sc_bit
operator | ( const sc_bit& a, char c )
{
    sc_bit result( a );
    return result |= c;
}

const sc_bit
operator | ( int i, const sc_bit& a )
{
    sc_bit result( a );
    return result |= i;
}

const sc_bit
operator | ( bool b, const sc_bit& a )
{
    sc_bit result( a );
    return result |= b;
}

const sc_bit
operator | ( char c, const sc_bit& a )
{
    sc_bit result( a );
    return result |= c;
}


// bitwise and

// MANDATORY

const sc_bit
operator & ( const sc_bit& a, const sc_bit& b )
{
    sc_bit result( a );
    return result &= b;
}

const sc_bit
operator & ( const sc_bit& a, int i )
{
    sc_bit result( a );
    return result &= i;
}

const sc_bit
operator & ( const sc_bit& a, bool b )
{
    sc_bit result( a );
    return result &= b;
}

const sc_bit
operator & ( const sc_bit& a, char c )
{
    sc_bit result( a );
    return result &= c;
}

const sc_bit
operator & ( int i, const sc_bit& a )
{
    sc_bit result( a );
    return result &= i;
}

const sc_bit
operator & ( bool b, const sc_bit& a )
{
    sc_bit result( a );
    return result &= b;
}

const sc_bit
operator & ( char c, const sc_bit& a )
{
    sc_bit result( a );
    return result &= c;
}


// bitwise exor

// MANDATORY

const sc_bit
operator ^ ( const sc_bit& a, const sc_bit& b )
{
    sc_bit result( a );
    return result ^= b;
}

const sc_bit
operator ^ ( const sc_bit& a, int i )
{
    sc_bit result( a );
    return result &= i;
}

const sc_bit
operator ^ ( const sc_bit& a, bool b )
{
    sc_bit result( a );
    return result ^= b;
}

const sc_bit
operator ^ ( const sc_bit& a, char c )
{
    sc_bit result( a );
    return result ^= c;
}

const sc_bit
operator ^ ( int i, const sc_bit& a )
{
    sc_bit result( a );
    return result ^= i;
}

const sc_bit
operator ^ ( bool b, const sc_bit& a )
{
    sc_bit result( a );
    return result ^= b;
}

const sc_bit
operator ^ ( char c, const sc_bit& a )
{
    sc_bit result( a );
    return result ^= c;
}

/// #endif
