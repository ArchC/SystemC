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

    scfx_string.h - 

    Original Author: Robert Graulich. Synopsys, Inc. (graulich@synopsys.com)
                     Martin Janssen.  Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_STRING_H
#define SCFX_STRING_H

#include <stdio.h>


// ----------------------------------------------------------------------------
//  CLASS : scfx_string
//
//  Simple string class for internal use.
// ----------------------------------------------------------------------------

class scfx_string {

    size_t _len;
    size_t _alloc;
    char*  _buffer;

    void resize( size_t );

public:

    scfx_string();

    ~scfx_string();

    int length() const;

    void clear();

    char& operator [] ( int );

    void append( int );
    void discard( int );
    void remove( int );

    void operator += ( char );
    void operator += ( const char* );

    operator const char* ();

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
scfx_string::resize( size_t i )
{
    if( i >= _alloc )
    {
	char* temp = new char[_alloc *= 2];
	
	for( int j = 0; j < (int) _len; j ++ )
	    temp[j] = _buffer[j];

	delete [] _buffer;
	_buffer = temp;
    }
}


inline
scfx_string::scfx_string()
: _len( 0 ), _alloc( BUFSIZ ), _buffer( new char[_alloc] )
{}


inline
scfx_string::~scfx_string()
{
    delete [] _buffer;
}


inline
int
scfx_string::length() const
{
    return _len;
}


inline
void
scfx_string::clear()
{
    _len = 0;
}


inline
char&
scfx_string::operator [] ( int i )
{
    resize( i );
    return _buffer[i];
}


inline
void
scfx_string::append( int n )
{
    _len += n;
}

inline
void
scfx_string::discard( int n )
{
    _len -= n;
}

inline
void
scfx_string::remove( int i )
{
    for( int j = i + 1; j < (int) _len; ++ j )
	_buffer[j - 1] = _buffer[j];
    -- _len;
}


inline
void
scfx_string::operator += ( char c )
{
    this->operator [] ( _len ) = c;
    _len ++;
}

inline
void
scfx_string::operator += ( const char* s )
{
    while( *s )
	(*this) += *s ++;
}


inline
scfx_string::operator const char*()
{
    this->operator [] ( _len ) = '\0';
    return _buffer;
}


#endif

// Taf!
