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

    scfx_mant.h - 

    Original Author: Robert Graulich. Synopsys, Inc. (graulich@synopsys.com)
                     Martin Janssen.  Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_MANT_H
#define SCFX_MANT_H


#include "scfx_ieee.h"
#include "scfx_utils.h"
#include "../../sc_macros.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

typedef unsigned long  word;
typedef unsigned short half_word;


// ----------------------------------------------------------------------------
//  CLASS : scfx_mant
//
//  Mantissa class.
// ----------------------------------------------------------------------------

class scfx_mant
{

    word* _array;
    int   _size;

public:

    explicit scfx_mant( size_t );
             scfx_mant( const scfx_mant& );

    scfx_mant& operator = ( const scfx_mant& );

    ~scfx_mant();

    void clear();

    void resize_to( int, int = 0 );

    int size() const;

    word  operator [] ( int ) const;
    word& operator [] ( int );

    half_word  half_at( int ) const;
    half_word& half_at( int );

    half_word* half_addr( int = 0 ) const;

private:

    static word* alloc( size_t );
    static void free( word*, size_t );

    static word* alloc_word( size_t size );
    static void free_word( word* array, size_t size );

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
int
scfx_mant::size() const
{
    return _size;
}


inline
word*
scfx_mant::alloc( size_t size )
{
#if defined( SCFX_BIG_ENDIAN )
    return alloc_word( size ) + ( size - 1 );
#elif defined( SCFX_LITTLE_ENDIAN )
    return alloc_word( size );
#endif
}

inline
void
scfx_mant::free( word* mant, size_t size )
{
#if defined( SCFX_BIG_ENDIAN )
    free_word( mant - ( size - 1 ), size );
#elif defined( SCFX_LITTLE_ENDIAN )
    free_word( mant, size );
#endif
}

inline
word
scfx_mant::operator[]( int i ) const
{
    _SC_ASSERT( i >= 0 && i < _size, "mantissa index out of range" );
#if defined( SCFX_BIG_ENDIAN )
    return _array[-i];
#elif defined( SCFX_LITTLE_ENDIAN )
    return _array[i];
#endif
}

inline
word&
scfx_mant::operator[]( int i )
{
    _SC_ASSERT( i >= 0 && i < _size, "mantissa index out of range" );
#if defined( SCFX_BIG_ENDIAN )
    return _array[-i];
#elif defined( SCFX_LITTLE_ENDIAN )
    return _array[i];
#endif
}

inline
scfx_mant::scfx_mant( size_t size )
{
    _array = alloc( _size = size );
}

inline
scfx_mant::scfx_mant( const scfx_mant& rhs )
{
    _array = alloc( _size = rhs._size );
    for( int i = 0; i < _size; i ++ )
    {
        (*this)[i] = rhs[i];
    }
}

inline
scfx_mant&
scfx_mant::operator = ( const scfx_mant& rhs )
{
    if( &rhs != this )
    {
        if( _size != rhs._size )
	{
	    free( _array, _size );
	    _array = alloc( _size = rhs._size );
	}

	for( int i = 0; i < _size; i ++ )
	{
	    (*this)[i] = rhs[i];
	}
    }
    return *this;
}

inline
scfx_mant::~scfx_mant()
{
    if( _array != 0 )
    {
        free( _array, _size );
    }
}

inline
void
scfx_mant::clear()
{
    for( int i = 0; i < _size; i ++ )
    {
        (*this)[i] = 0;
    }
}

inline
void
scfx_mant::resize_to( int size, int restore )
{
    if( size == _size )
    {
        return;
    }

    if( ! _array )
    {
        _array = alloc( _size = size );
    }
    else
    {
        word* p = alloc( size );

	if( restore )
	{
	    int end = min( size, _size );
	    if( restore == 1 )		// msb resized -> align at 0
	    {
	        for( int i = 0; i < size; i ++ )
		{
		    if( i < end )
		    {
#if defined( SCFX_BIG_ENDIAN )
		        p[-i] = _array[-i];
#elif defined( SCFX_LITTLE_ENDIAN )
			p[i] = _array[i];
#endif
		    }
		    else
		    {
#if defined( SCFX_BIG_ENDIAN )
		        p[-i] = 0;
#elif defined( SCFX_LITTLE_ENDIAN )
			p[i] = 0;
#endif
		    }
		}
	    }
	    else			// lsb resized -> align at size-1
	    {
	        for( int i = 0; i < size; i ++ )
		{
		    if( i < end )
		    {
#if defined( SCFX_BIG_ENDIAN )
		        p[-size+1+i] = _array[-_size+1+i];
#elif defined( SCFX_LITTLE_ENDIAN )
			p[size-1-i] = _array[_size-1-i];
#endif
		    }
		    else
		    {
#if defined( SCFX_BIG_ENDIAN )
		        p[-size+1+i] = 0;
#elif defined( SCFX_LITTLE_ENDIAN )
			p[size-1-i] = 0;
#endif
		    }
		}
	    }
	}

	free( _array, _size );
	_array = p;
	_size = size;
    }
}

inline
half_word
scfx_mant::half_at( int i ) const
{
    _SC_ASSERT( ( i >> 1 ) >= 0 && ( i >> 1 ) < _size,
		"mantissa index out of range" );
#if defined( SCFX_BIG_ENDIAN )
    return reinterpret_cast<half_word*>( _array )[-i];
#elif defined( SCFX_LITTLE_ENDIAN )
    return reinterpret_cast<half_word*>( _array )[i];
#endif
}

inline
half_word&
scfx_mant::half_at( int i )
{
    _SC_ASSERT( ( i >> 1 ) >= 0 && ( i >> 1 ) < _size,
		"mantissa index out of range" );
#if defined( SCFX_BIG_ENDIAN )
    return reinterpret_cast<half_word*>( _array )[-i];
#elif defined( SCFX_LITTLE_ENDIAN )
    return reinterpret_cast<half_word*>( _array )[i];
#endif
}

inline
half_word*
scfx_mant::half_addr( int i ) const
{
    _SC_ASSERT( i >= 0 && i < _size, "mantissa index out of range" );
#if defined( SCFX_BIG_ENDIAN )
    return reinterpret_cast<half_word*>( _array - i ) + 1;
#elif defined( SCFX_LITTLE_ENDIAN )
    return reinterpret_cast<half_word*>( _array + i );
#endif
}


// ----------------------------------------------------------------------------
//  one's complement of a mantissa
// ----------------------------------------------------------------------------

inline
void
complement( scfx_mant& target, const scfx_mant& source, int size )
{
    for( int i = 0; i < size; i ++ )
    {
        target[i] = ~source[i];
    }
}


// ----------------------------------------------------------------------------
//  increment mantissa
// ----------------------------------------------------------------------------

inline
void
inc( scfx_mant& mant )
{
    for( int i = 0; i < mant.size(); i ++ )
    {
        if( ++ mant[i] )
	{
	    break;
	}
    }
}


// ----------------------------------------------------------------------------
//  CLASS : scfx_mant_ref
//
//  Mantissa reference class.
// ----------------------------------------------------------------------------

class scfx_mant_ref
{

    scfx_mant* _mant;
    bool       _not_const;

public:

    scfx_mant_ref();
    scfx_mant_ref( const scfx_mant& );
    scfx_mant_ref( scfx_mant* );

    scfx_mant_ref& operator = ( const scfx_mant& );
    scfx_mant_ref& operator = ( scfx_mant* );

    ~scfx_mant_ref();

    operator scfx_mant&();

    word operator [] ( int );

private:

    void remove_it();

    scfx_mant_ref( const scfx_mant_ref& );
    scfx_mant_ref& operator = ( const scfx_mant_ref& );

    void* operator new( size_t sz ) { return ::operator new( sz ); }

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
scfx_mant_ref::remove_it()
{
    if( _not_const )
    {
        delete _mant;
    }
}

inline
scfx_mant_ref::scfx_mant_ref()
: _mant( 0 ), _not_const( false )
{}

inline
scfx_mant_ref::scfx_mant_ref( const scfx_mant& mant )
: _mant( const_cast<scfx_mant*>( &mant ) ), _not_const( false )
{}

inline
scfx_mant_ref::scfx_mant_ref( scfx_mant* mant )
: _mant( mant ), _not_const( true )
{}

inline
scfx_mant_ref&
scfx_mant_ref::operator = ( const scfx_mant& mant )
{
    remove_it();

    _mant = const_cast<scfx_mant*>( &mant );
    _not_const = false;

    return *this;
}

inline
scfx_mant_ref&
scfx_mant_ref::operator = ( scfx_mant* mant )
{
    remove_it();

    _mant = mant;
    _not_const = true;

    return *this;
}

inline
scfx_mant_ref::~scfx_mant_ref()
{
    remove_it();
}

inline
scfx_mant_ref::operator scfx_mant&()
{
    // _SC_ASSERT( _not_const, "not allowed to modify mant" );
    return *_mant;
}

inline
word
scfx_mant_ref::operator [] ( int i )
{
    return (*_mant)[i];
}


#endif

// Taf!
