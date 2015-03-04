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

  sc_vector.cpp -- Simple implementation of a vector class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <stdlib.h>
#include <string.h>

#include "systemc/utils/sc_iostream.h"
#include "systemc/utils/sc_vector.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_pvector_base
//
//  Simple vector base class.
// ----------------------------------------------------------------------------

sc_pvector_base::sc_pvector_base( int alloc )
{
    m_alloc = alloc;
    m_sz = 0;
    m_data = new void*[m_alloc];
}

sc_pvector_base::sc_pvector_base( const sc_pvector_base& rhs )
{
    m_alloc = rhs.m_alloc;
    m_sz = rhs.m_sz;
    m_data = new void*[m_alloc];
    memcpy( m_data, rhs.m_data, rhs.m_sz * sizeof(void*) );
}

sc_pvector_base::~sc_pvector_base()
{
    delete[] m_data;
}

sc_pvector_base&
sc_pvector_base::operator = ( const sc_pvector_base& rhs )
{
    if( &rhs == this ) {
        return *this;
    }
    if( m_alloc < rhs.m_alloc ) {
        delete[] m_data;
        m_data = new void*[rhs.m_alloc];
        m_alloc = rhs.m_alloc;
    }
    m_sz = rhs.m_sz;
    memcpy( m_data, rhs.m_data, rhs.m_sz * sizeof(void*) );
    return *this;
}

void*&
sc_pvector_base::operator [] ( int i )
{
    if( i >= m_sz ) {
        if( i + 1 > m_alloc ) {
            resize( i + 11 );
	}
        m_sz = i + 1;
    }
    return m_data[i];
}

const void*&
sc_pvector_base::operator [] ( int i ) const
{
   return (const void*&)( ((sc_pvector_base*) this)->operator [] ( i ) );
}

void
sc_pvector_base::resize( int new_sz )
{
    if( new_sz <= m_alloc ) {
	return;
    }
    m_alloc = new_sz;
    void** new_data = new void*[new_sz];
    memcpy( new_data, m_data, m_sz * sizeof(void*) );
    delete[] m_data;
    m_data = new_data;
}

void
sc_pvector_base::sort( CFT compar )
{
    qsort( m_data, m_sz, sizeof(void*), compar );
}
