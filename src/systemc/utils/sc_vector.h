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

  sc_vector.h -- Simple implementation of a vector class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#ifndef SC_VECTOR_H
#define SC_VECTOR_H


extern "C" {
  typedef int (*CFT)( const void*, const void* );
} 


// ----------------------------------------------------------------------------
//  CLASS : sc_pvector_base
//
//  Simple vector base class.
// ----------------------------------------------------------------------------

class sc_pvector_base
{
protected:

    typedef void* item_type;

    typedef item_type* iterator;
    typedef const item_type* const_iterator;

    sc_pvector_base( int alloc = 10 );
    sc_pvector_base( const sc_pvector_base& );
    ~sc_pvector_base();


    int size() const
	{ return m_sz; }


    iterator begin()
        { return m_data; }

    const_iterator begin() const
        { return m_data; }

    iterator end()
        { return m_data + m_sz; }

    const_iterator end() const
        { return m_data + m_sz; }


    sc_pvector_base& operator = ( const sc_pvector_base& );


    void** raw_data()
	{ return m_data; }

    void* const* raw_data() const
	{ return m_data; }


    void*& operator [] ( int i );
    const void*& operator [] ( int i ) const;

    void*& fetch( int i )
	{ return m_data[i]; }

    const void*& fetch( int i ) const
	{ return (const void*&) m_data[i]; }


    void push_back( void* item )
    {
	if( m_sz == m_alloc ) {
	    resize( m_sz + 11 );
	}
	m_data[m_sz] = item;
	++ m_sz;
    }


    void erase_all()
        { m_sz = 0; }


    void sort( CFT compar );

    
    /* Change the ith item to new_item */
    void put( void* new_item, int i )
	{ m_data[i] = new_item; }

    void decr_count()
	{ m_sz ? m_sz -- : 0; }

    void decr_count( int k )
	{ m_sz -= k; if( m_sz < 0 ) m_sz = 0; }

private:

    void resize( int new_sz );

    int    m_alloc;
    int    m_sz;
    void** m_data;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_pvector<T>
//
//  Simple vector class.
// ----------------------------------------------------------------------------

template< class T >
class sc_pvector
: public sc_pvector_base
{
public:

    typedef T* iterator;
    typedef const T* const_iterator;

    sc_pvector( int alloc = 10 )
	: sc_pvector_base( alloc )
	{}

    sc_pvector( const sc_pvector<T>& rhs )
	: sc_pvector_base( rhs )
	{}

    ~sc_pvector()
	{}


    int size() const
	{ return sc_pvector_base::size(); }


    iterator begin()
        { return (iterator) sc_pvector_base::begin(); }

    const_iterator begin() const
        { return (const_iterator) sc_pvector_base::begin(); }

    iterator end()
        { return (iterator) sc_pvector_base::end(); }

    const_iterator end() const
        { return (const_iterator) sc_pvector_base::end(); }


    sc_pvector<T>& operator = ( const sc_pvector<T>& rhs )
	{ return (sc_pvector<T>&) sc_pvector_base::operator = ( rhs ); }


    T& operator [] ( int i )
	{ return (T&) sc_pvector_base::operator [] ( i ); }

    const T& operator [] ( int i ) const
	{ return (const T&) sc_pvector_base::operator [] ( i ); }

    T& fetch( int i )
	{ return (T&) sc_pvector_base::fetch( i ); }

    const T& fetch( int i ) const
	{ return (const T&) sc_pvector_base::fetch( i ); }


    T* raw_data()
	{ return (T*) sc_pvector_base::raw_data(); }

    const T* raw_data() const
	{ return (const T*) sc_pvector_base::raw_data(); }


    void push_back( T item )
	{ sc_pvector_base::push_back( (void*) item ); }


    void erase_all()
	{ sc_pvector_base::erase_all(); }

    void sort( CFT compar )
	{ sc_pvector_base::sort( compar ); }
    

    /* These methods have been added from Ptr_Array */
    
    void put( T item, int i )
	{ sc_pvector_base::put( (void*) item, i ); }

    void decr_count()
	{ sc_pvector_base::decr_count(); }

    void decr_count( int k )
	{ sc_pvector_base::decr_count( k ); }
};


#endif
