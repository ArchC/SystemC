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

  sc_string.h -- Implementation of a simple string class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#ifndef SC_STRING_H
#define SC_STRING_H


#include "systemc/utils/sc_iostream.h"


// classes defined in this module
class sc_string;

// forward class declarations
class sc_string_rep;


// ----------------------------------------------------------------------------
//  ENUM : sc_numrep
//
//  Enumeration of number representations for character string conversion.
// ----------------------------------------------------------------------------

enum sc_numrep
{
    SC_NOBASE = 0,
    SC_BIN    = 2,
    SC_OCT    = 8,
    SC_DEC    = 10,
    SC_HEX    = 16,
    SC_BIN_US,
    SC_BIN_SM,
    SC_OCT_US,
    SC_OCT_SM,
    SC_HEX_US,
    SC_HEX_SM,
    SC_CSD
};


const sc_string to_string( sc_numrep );


// ----------------------------------------------------------------------------
//  CLASS : sc_string
//
//  String class (yet another).
// ----------------------------------------------------------------------------

class sc_string 
{
    friend ostream& operator << ( ostream& os, const sc_string& a );
    friend istream& operator >> ( istream& is, sc_string& a );

public:

    //  constructors

    explicit sc_string( int size = 16 );
    sc_string( const char* s );
    sc_string( const char* s, int n ); // get first n chars from the string
    sc_string( const sc_string& s );


    // destructor

    ~sc_string();


    // concatenation and assignment

    sc_string& operator = ( const char* s );
    sc_string& operator = ( const sc_string& s );

    sc_string& operator += ( const char* s );
    sc_string& operator += ( char c );
    sc_string& operator += ( const sc_string& s );

    sc_string operator + ( const char* s ) const;
    sc_string operator + ( char c ) const;
    sc_string operator + ( const sc_string& s ) const;

    friend sc_string operator + ( const char* s, const sc_string& t );


    // returns substring [first,last]

    sc_string substr( int first, int last ) const;


    // string comparison operators

    bool operator == ( const char* s ) const;
    bool operator != ( const char* s ) const;
    bool operator <  ( const char* s ) const;
    bool operator <= ( const char* s ) const;
    bool operator >  ( const char* s ) const;
    bool operator >= ( const char* s ) const;
    bool operator == ( const sc_string& s ) const;
    bool operator != ( const sc_string& s ) const;
    bool operator <  ( const sc_string& s ) const;
    bool operator <= ( const sc_string& s ) const;
    bool operator >  ( const sc_string& s ) const;
    bool operator >= ( const sc_string& s ) const;

    //
    // returns length of the string (excluding trailing \0)
    //
    int length() const;

    //
    // returns c-style string
    //
    const char* c_str() const;
    //
    // returns c-style string
    //
    operator const char*() const;
    //
    // returns character at "index" position
    //
    char operator[](int index) const;
    //
    // l-value subscript
    //
    char& operator[](int index);

    // formatted string (see printf description)
    static sc_string to_string(const char* format, ...);
    //
    //       conveniece formatting functions for common types
    //       e.g. sc_string("a=%d, s is %s").fmt(1).fmt("string")
    //       should produce: a=1, s is string
    //       it should be safe: if less arguments specified
    //       it should print %specifier; extra arguments should be ignored
    // TODO: if the type of the argument is incompatible with format 
    //       specifier it should be ignored
    //
    // must have it inlined because of some compilers
    template<class T> sc_string& fmt(const T& t)
	{
	    // search %
	    int index;
	    int len = length();
	    sc_string temp(*this);
	    do
	    {
		index = temp.pos("%");
		if(index == length()-1) 
		    return *this;
		temp = substr(index+1,len-1);
	    } while(temp[0] != '%');
	    int f_len = (int)temp.fmt_length(); // lenght of format field
	    temp = to_string(substr(0,index+f_len-1).c_str(),t);
	    return (*this) = temp + substr(index+f_len,len);
	}
    sc_string& fmt(const sc_string& s);
    //
    // find position of substring in this string 
    // returns -1 if not found
    //
    int pos(const sc_string& sub_string)const;
    //
    // remove "count" characters from "index"
    //
    sc_string& remove(unsigned index, unsigned length);
    //
    // insert "substring" before "index"
    //
    sc_string& insert(const sc_string& sub_string, unsigned index);
    //
    // returns true if the character at byte index in this string matches 
    // any character in the delimiters string
    //
    bool is_delimiter(const sc_string& str, unsigned index)const;
    //
    // returns true if string contains the character
    //
    bool contains(char c)const;
    //
    // produce upper case string from this one
    //
    sc_string uppercase()const;
    //
    // produce lower case string from this one
    //
    sc_string lowercase()const;
    //
    // legacy methods
    //
    static sc_string make_str(long n);
    void set( int index, char c );
    int cmp( const char* s ) const;
    int cmp( const sc_string& s ) const;


    void print( ostream& os = cout ) const;

private:

    sc_string( sc_string_rep* r );

    sc_string_rep* rep;

    void test(int position)const;
    unsigned fmt_length()const;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
ostream&
operator << ( ostream& os, sc_numrep numrep )
{
    os << to_string( numrep );
    return os;
}


inline
ostream&
operator << ( ostream& os, const sc_string& a )
{
    a.print( os );
    return os;
}


#endif
