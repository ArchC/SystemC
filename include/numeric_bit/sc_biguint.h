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

    sc_biguint.h -- Template version of sc_unsigned. This class
    enables compile-time bit widths for sc_unsigned numbers.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
 
******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Gene Bushayev. Synopsys, Inc. (gbush@synopsys.com)
    Description of Modification: Interface between sc_bigint and sc_bv / sc_lv.    

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_BIGUINT_H
#define SC_BIGUINT_H

#include "sc_signed.h"
#include "sc_unsigned.h"
#include "../sc_exception.h"

namespace sc_bv_ns
{
template<int W> class sc_bv;
template<int W> class sc_lv;
}
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_lv;

class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;

#ifdef SC_MAX_NBITS
template< int W = SC_MAX_NBITS >
#else
template< int W >
#endif
class sc_biguint : public sc_unsigned {

public:

  sc_biguint() : sc_unsigned(W)                            { }

  sc_biguint(const sc_biguint<W>&      v) : sc_unsigned(W) { *this = v; }

  sc_biguint(const sc_unsigned&        v) : sc_unsigned(W) { *this = v; }

  sc_biguint(const sc_unsigned_subref& v) : sc_unsigned(W) { *this = v; }

  sc_biguint(const sc_signed&          v) : sc_unsigned(W) { *this = v; }

  sc_biguint(const sc_signed_subref&   v) : sc_unsigned(W) { *this = v; }

  sc_biguint(const char*               v) : sc_unsigned(W) { *this = v; } 

  sc_biguint(int64                     v) : sc_unsigned(W) { *this = v; }

  sc_biguint(uint64                    v) : sc_unsigned(W) { *this = v; }

  sc_biguint(long                      v) : sc_unsigned(W) { *this = v; }

  sc_biguint(unsigned long             v) : sc_unsigned(W) { *this = v; }

  sc_biguint(int                       v) : sc_unsigned(W) { *this = v; } 

  sc_biguint(unsigned int              v) : sc_unsigned(W) { *this = v; }

  sc_biguint(double                    v) : sc_unsigned(W) { *this = v; }

//#ifdef SC_INCLUDE_FX
  explicit sc_biguint( const sc_fxval&  );
  explicit sc_biguint( const sc_fxval_fast& );
  explicit sc_biguint( const sc_fxnum&  );
  explicit sc_biguint( const sc_fxnum_fast& );
//#endif

  sc_biguint(const sc_bv<W>&);
  sc_biguint(const sc_lv<W>&);

#ifdef SC_LOGIC_VECTOR_H

  sc_biguint(const sc_logic_vector&    v) : sc_unsigned(W) { *this = v; }

  sc_biguint(const sc_bool_vector&     v) : sc_unsigned(W) { *this = v; }

#endif

#ifndef SC_MAX_NBITS
  ~sc_biguint()  { }
#endif
 
  sc_biguint<W>& operator=(const sc_biguint<W>&      v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(const sc_unsigned&        v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(const sc_unsigned_subref& v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(const sc_signed&          v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(const sc_signed_subref&   v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(const char*               v) 
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(int64                     v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(uint64                    v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(long                      v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(unsigned long             v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(int                       v) 
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(unsigned int              v) 
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(double                    v)
  { sc_unsigned::operator=(v); return *this; }

//#ifdef SC_INCLUDE_FX
  sc_biguint& operator = ( const sc_fxval& );
  sc_biguint& operator = ( const sc_fxval_fast& );
  sc_biguint& operator = ( const sc_fxnum& );
  sc_biguint& operator = ( const sc_fxnum_fast& );
//#endif

  sc_biguint& operator=( const sc_bv<W>& v);

  sc_biguint& operator=( const sc_lv<W>& v);


#ifdef SC_LOGIC_VECTOR_H

  sc_biguint<W>& operator=(const sc_logic_vector&    v)
  { sc_unsigned::operator=(v); return *this; }

  sc_biguint<W>& operator=(const sc_bool_vector&     v)
  { sc_unsigned::operator=(v); return *this; }

#endif
 
};

// Implementation of sc_bv and sc_lv parts.
template<int W>  
sc_biguint<W>::sc_biguint( const sc_bv<W>& v): sc_signed(W)
{
  for( int i = W-1; i >= 0; i-- )
    set( i, v[i] );
}

template<int W>  
sc_biguint<W>::sc_biguint( const sc_lv<W>& v ): sc_signed(W)
{
  for( int i = W-1; i >= 0; i-- ){
    if( v[i] > 1 )
    {
      REPORT_WARNING(2007," result is undefined");
      return;
    }
    set( i, v[i] );
  }
}

template<int W>
inline sc_biguint<W>& sc_biguint<W>::operator=( const sc_bv<W>& v )
{
  for( int i = W-1 ; i >= 0; i-- ){
    set( i, v[i] );
  }
  return *this;
}

template<int W>
inline 
sc_biguint<W>& 
sc_biguint<W>::operator=( const sc_lv<W>& v )
{
  for( int i = W-1 ; i >= 0; i-- ){
    if(v[i]>1)
    {
      REPORT_WARNING(2007," result is undefined");
      return *this;
    }
    set( i, v[i] );
  }
  return *this;
}

#endif
