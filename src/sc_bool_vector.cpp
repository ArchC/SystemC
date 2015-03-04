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

    sc_bool_vector.cpp - implementation of bool_vector, a vector of binary values

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <cstring>

#include "sc_iostream.h"

#include "sc_macros_int.h"
#include "sc_bvrep.h"
#include "sc_bit.h"

#include "sc_mempool.h"
#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
#include "sc_signal_delay.h"
#include "sc_bv.h"
#include "numeric_bit/sc_int.h"
#include "numeric_bit/sc_uint.h"

#ifdef __GNUC__
#define SIGNAL_ARRAY_DELAY sc_signal_array_delay_gnu<sc_bool_vector,bool>
#else
#define SIGNAL_ARRAY_DELAY sc_signal_array_delay<sc_bool_vector>
#endif
#if !defined(_MSC_VER) && !defined(__HP_aCC)
template <class T> 
inline T min(T t1, T t2) { return t1>t2?t2:t1;}
#endif

sc_bool_vector::sc_bool_vector( int len )
    : sc_array<bool>( len )
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( int len, const sc_bool_vector& ref )
    : sc_array<bool>( len, (const sc_array<bool>&) ref )
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( const sc_bool_vector& y )
    : sc_array<bool>(y)
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( bool** data_ptrs, int len )
    : sc_array<bool>( data_ptrs, len )
{
    str = 0;
}

sc_bool_vector::~sc_bool_vector()
{
    // Note that the size of the array to be freed must
    // be exactly the same as allocated -- see
    // sc_bool_vector::to_string()
    sc_mempool::release(str, 1 + length());
}

sc_bool_vector&
sc_bool_vector::sub( int i, int j )
{
    return dynamic_cast<sc_bool_vector&>(sc_array_base::sub(i,j));
}

sc_bool_vector::sc_bool_vector( bool* initialized_data, int len )
    : sc_array<bool>( initialized_data, len )
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( sc_bool_vector* v1, sc_bool_vector* v2 )
    : sc_array<bool>( v1, v2 )
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( sc_bool_vector* v1, bool* v2 )
    : sc_array<bool>( v1, v2 )
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( bool* v1, sc_bool_vector* v2 )
    : sc_array<bool>( v1, v2 )
{
    str = 0;
}

sc_bool_vector::sc_bool_vector( bool* v1, bool* v2 )
    : sc_array<bool>( v1, v2 )
{
    str = 0;
}


void
sc_bool_vector::assign_concat_with_string( const sc_bool_vector* v1, const char* v2 )
{
    int i, j;
    for (i = length() - 1, j = v1->length() - 1; j >= 0; --i, --j) {
        assign_help( i, *(v1->get_data_ptr(j)) );
    }
    for ( ; i >= 0; --i) {
        assign_help( i, (*v2 != '0') );
        ++v2;
    }
}

sc_bool_vector::sc_bool_vector( const sc_bool_vector* v1, const char* v2 )
    : sc_array<bool>( v1->length() + strlen(v2) )
{
    assign_concat_with_string( v1, v2 );
    str = 0;
}

void
sc_bool_vector::assign_concat_with_string( const char* v1, const sc_bool_vector* v2 )
{
    int i;
    int jlen = v2->length();
    for (i = length() - 1; i >= jlen; --i) {
        assign_help( i, (*v1 != '0') );
        ++v1;
    }
    for ( ; i >= 0; --i) {
        assign_help( i, *(v2->get_data_ptr(i)) );
    }
}

sc_bool_vector::sc_bool_vector( const char* v1, const sc_bool_vector* v2 )
    : sc_array<bool>( strlen(v1) + v2->length() )
{
    assign_concat_with_string( v1, v2 );
    str = 0;
}

sc_bool_vector&
sc_bool_vector::operator=( const sc_bool_vector& nv )
{
    sc_array<bool>::operator=(nv);
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator=( const char* nv )
{
    int str_len = strlen(nv);
    int len = min(length(),str_len);
    nv += str_len - len;
    for (int i = len - 1; i >= 0; --i) {
        assign_help( i, (*nv != '0') );
        ++nv;
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator=( const bool* nv )
{
    sc_array<bool>::operator=(nv);
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator=( const sc_logic_vector& nv )
{
    int len = nv.length();
    check_length(len);

    for (int i = length() - 1; i >= 0; --i) {
        assign_help( i, nv.get_data_ptr(i)->to_bool() );
    }        
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator=( long v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        assign_help( i, bool(v & 1) );
        v >>= 1;
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator=( unsigned long v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        assign_help( i, bool(v & 1) );
        v >>= 1;
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator=( int64 v )
{
  int len = length();
  for(int i=0; i < len; ++i ){
#ifndef WIN32
    assign_help(i, bool(v & 1ULL));
#else
    assign_help(i, bool(v & 1i64));
#endif
    v >>= 1;
  }
  return *this;
}

sc_bool_vector& 
sc_bool_vector::operator=( uint64 v )
{
  int len = length();
  for(int i = 0; i < len; ++i ){
#ifndef WIN32
    assign_help( i, bool(v & 1ULL));
#else
    assign_help( i, bool(v & 1i64));
#endif
    v >>= 1;
  }
  return *this;
}

sc_bool_vector& sc_bool_vector::operator=( const sc_bv_ns::sc_bv_base& v)
{
  if(length()!= v.length())
    REPORT_ERROR(2000,"");
  for(unsigned i=0; i<(unsigned)(length()); i++)
    (*this)[i]=v[i];
  return *this;
}

sc_bool_vector& sc_bool_vector::operator=( const sc_int_base& v)
{return (*this)=v.to_int64();}
sc_bool_vector& sc_bool_vector::operator=( const sc_uint_base& v)
{return (*this)=v.to_uint64();}


bool
sc_bool_vector::operator==( const sc_logic_vector& rhs ) const
{
    int len = rhs.length();
    check_length(len);

    for (int i = len - 1; i >= 0; --i) {
        const sc_logic* su = rhs.get_data_ptr(i);
        if (*su != '0' && *su != '1')
            return false;
        if (su->to_bool() != *get_data_ptr(i))
            return false;
    }
    return true;
}

bool
sc_bool_vector::operator==( const char* rhs ) const
{
    int len = length();
    rhs += (strlen(rhs) - len);
    for (int i = len - 1; i >= 0; --i) {
        if (*get_data_ptr(i) != (*rhs != '0'))
            return false;
        rhs++;
    }
    return true;
}

#include "sc_bool_vector_and.inc"
#include "sc_bool_vector_or.inc"
#include "sc_bool_vector_xor.inc"

sc_bool_vector&
sc_bool_vector::negate()
{
    for (int i = length() - 1; i >= 0; --i) {
        bool* data_ptr = get_data_ptr(i);
        *data_ptr = ! *data_ptr;
    }
    return *this;
}

const char*
sc_bool_vector::to_string() const
{
    if (0 == str)
        str = (char*) sc_mempool::allocate(1 + length());
    to_string( str );
    return str;
}

void
sc_bool_vector::to_string( char* buffer ) const
{
    for (int i = length() - 1; i >= 0; --i) {
        *buffer++ = char('0' + *get_data_ptr(i));
    }        
    *buffer = '\0';
}

int
sc_bool_vector::to_signed() const
{
    int len = length();
    int v;
    if ((unsigned)len > sizeof(int)*8) {
        REPORT_WARNING(2002,sc_string::to_string("%d",len).c_str());
        v = 0;
        for (int i = sizeof(int)*8 - 1; i >= 0; --i) {
            v = (v << 1) + int(*get_data_ptr(i));
        }
    } else {
        v = - int(*get_data_ptr(len - 1));
        for (int i = len - 2; i >= 0; --i) {
            v = (v << 1) + int(*get_data_ptr(i));
        }
    }
    return v;
}

unsigned
sc_bool_vector::to_unsigned() const
{
    int len = length();
    unsigned v = 0;
    if ((unsigned) len > sizeof(unsigned)*8) {
        REPORT_WARNING(2002,sc_string::to_string("%d",len).c_str());
        for (int i = sizeof(unsigned)*8 - 1; i >= 0; --i) {
            v = (v << 1) + int(*get_data_ptr(i));
        }
    } else {
        for (int i = len - 1; i >= 0; --i) {
            v = (v << 1) + int(*get_data_ptr(i));
        }
    }
    return v;
}

sc_bool_vector::sc_bool_vector( const sc_array_base* p, int i, int j )
    : sc_array<bool>( p, i, j )
{
    str = 0;
}

sc_array_base*
sc_bool_vector::vctor( const sc_array_base* p, int i, int j ) const
{
    return new sc_bool_vector( p, i, j );
}

ostream&
operator<<(ostream& os, const sc_bool_vector& bv)
{
    return (os << bv.to_string());
}

void
sc_bool_vector::print(ostream& os) const
{
    os << *this;
}

void
sc_bool_vector::dump(ostream& os) const
{
    sc_array_base::dump(os);
    os << "value = " << *this << endl;
}

/*---------------------------------------------------------------------------*/

sc_signal_bool_vector::sc_signal_bool_vector( int len )
    : super( len )
{
    /*EMPTY*/
}

sc_signal_bool_vector::sc_signal_bool_vector( const char* nm, int len )
    : super( nm, len )
{
    /*EMPTY*/
}

sc_signal_bool_vector::sc_signal_bool_vector( int len, const sc_signal_bool_vector& ref )
    : super( len, (const super&) ref )
{
    /*EMPTY*/
}

sc_signal_bool_vector::sc_signal_bool_vector( const sc_signal_bool_vector& ref )
    : super( ref )
{
    /*EMPTY*/
}

sc_signal_bool_vector::~sc_signal_bool_vector()
{
    /*EMPTY*/
}

sc_signal_bool_vector&
sc_signal_bool_vector::sub( int i, int j )
{
    return dynamic_cast<sc_signal_bool_vector&>(sc_array_base::sub(i,j));
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const sc_array<bool>& nv )
{
    return (sc_signal_bool_vector&) super::write( nv );
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const bool* nv )
{
    return (sc_signal_bool_vector&) super::write( nv );
}

static void
get_charstring_val( bool& nv, const void* nva, int i )
{
    nv = (((char*) nva)[i] != '0');
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const char* nva )
{
    nva += (strlen(nva) - length());
    super::write_help( get_charstring_val, nva, /*left_to_right*/ true );
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( int v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        super::write_help( i, bool(v & 1) );
        v >>= 1;
    }
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( int64 v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
#ifndef WIN32
        super::write_help( i, bool(v & 1ULL) );
#else
	super::write_help( i, bool(v & 1i64) );
#endif
        v >>= 1;
    }
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( unsigned v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        super::write_help( i, bool(v&1) );
        v >>= 1;
    }
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( uint64 v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
#ifndef WIN32
        super::write_help( i, bool(v & 1ULL) );
#else
        super::write_help( i, bool(v & 1i64) );
#endif
        v >>= 1;
    }
    return *this;
}

static void
get_sc_logic_vector_val(bool& result, const void* v, int i)
{
    const sc_logic_vector& suv = *((const sc_logic_vector*) v);
    result = suv[i].to_bool();
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const sc_logic_vector& v )
{
    check_length(v.length());
    super::write_help(get_sc_logic_vector_val, (void*) &v, /*left_to_right*/false);
    return *this;
}

sc_signal_bool_vector::sc_signal_bool_vector( const sc_array_base* p, int i, int j )
    : super( p, i, j )
{
    /*EMPTY*/
}

sc_array_base*
sc_signal_bool_vector::vctor( const sc_array_base* p, int i, int j ) const
{
    return new sc_signal_bool_vector(p, i, j);
}

sc_signal_bool_vector::sc_signal_bool_vector( sc_signal_bool_vector* v1, sc_signal_bool_vector* v2 )
    : sc_signal_array<sc_bool_vector>( v1, v2 )
{
    /*EMPTY*/
}

sc_signal_bool_vector::sc_signal_bool_vector( sc_signal_bool_vector* v1, sc_signal<bool>* v2 )
    : sc_signal_array<sc_bool_vector>( v1, v2 )
{
    /*EMPTY*/
}

sc_signal_bool_vector::sc_signal_bool_vector( sc_signal<bool>* v1, sc_signal_bool_vector* v2 )
    : sc_signal_array<sc_bool_vector>( v1, v2 )
{
    /*EMPTY*/
}

sc_signal_bool_vector::sc_signal_bool_vector( sc_signal<bool>* v1, sc_signal<bool>* v2 )
    : sc_signal_array<sc_bool_vector>( v1, v2 )
{
    /*EMPTY*/
}

/*---------------------------------------------------------------------------*/

sc_bool_vector*
sc_bool_vector::get_concat_array( sc_bool_vector* x, sc_bool_vector* y )
{
    return (sc_bool_vector*) sc_array_base::get_concat_array( x, y, new_array_vv );
}

sc_bool_vector*
sc_bool_vector::get_concat_array( sc_bool_vector* x, bool* y )
{
    return (sc_bool_vector*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
}

sc_bool_vector*
sc_bool_vector::get_concat_array( bool* x, sc_bool_vector* y )
{
    return (sc_bool_vector*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
}

sc_bool_vector*
sc_bool_vector::get_concat_array( bool* x, bool* y )
{
    return (sc_bool_vector*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
}

const sc_bool_vector*
sc_bool_vector::get_concat_array( sc_bool_vector* x, const char* y )
{
    bool is_new;
    sc_bool_vector* bv = (sc_bool_vector*) sc_array_base::get_concat_array( x, y, new_array_vc, &is_new );
    if (! is_new) {
        bv->assign_concat_with_string( x, y );
    }
    return bv;
}

const sc_bool_vector*
sc_bool_vector::get_concat_array( const char* x, sc_bool_vector* y )
{
    bool is_new;
    sc_bool_vector* bv = (sc_bool_vector*) sc_array_base::get_concat_array( x, y, new_array_cv, &is_new );
    if (! is_new) {
        bv->assign_concat_with_string( x, y );
    }
    return bv;
}

sc_array_base*
sc_bool_vector::new_array_vv( sc_array_base* x, sc_array_base* y )
{
    return new sc_bool_vector( (sc_bool_vector*) x, (sc_bool_vector*) y );
}

sc_array_base*
sc_bool_vector::new_array_vs( sc_array_base* x, void* y )
{
    return new sc_bool_vector( (sc_bool_vector*) x, (bool*) y );
}

sc_array_base*
sc_bool_vector::new_array_sv( void* x, sc_array_base* y )
{
    return new sc_bool_vector( (bool*) x, (sc_bool_vector*) y );
}

sc_array_base*
sc_bool_vector::new_array_ss( void* x, void* y )
{
    return new sc_bool_vector( (bool*) x, (bool*) y );
}

sc_array_base*
sc_bool_vector::new_array_vc( sc_array_base* x, const char* y )
{
    return new sc_bool_vector( (sc_bool_vector*) x, (char*) y );
}

sc_array_base*
sc_bool_vector::new_array_cv( const char* x, sc_array_base* y )
{
    return new sc_bool_vector( (char*) x, (sc_bool_vector*) y );
}


#if !defined(_MSC_VER)
// the following comma operators are not available in VC6.0
// because of the bug in VC6.0 compiler
sc_bool_vector& operator,( bool& x, sc_bool_vector& y )
{
    return *(sc_bool_vector::get_concat_array( &x, &y ));
}

const sc_bool_vector& operator,( const char* x, const sc_bool_vector& y )
{
    return *(sc_bool_vector::get_concat_array( x, (sc_bool_vector*) &y ));
}

const sc_bool_vector& operator,( const bool& x, const sc_bool_vector& y )
{
    return *(sc_bool_vector::get_concat_array( (bool*) &x, (sc_bool_vector*) &y ));
}

const sc_bool_vector& operator,( const sc_signal<bool>& x, const sc_bool_vector& y )
{
    return *(sc_bool_vector::get_concat_array( (bool*) x.get_data_ptr(),
                                               (sc_bool_vector*) &y ));
}
#endif

sc_signal_bool_vector*
sc_signal_bool_vector::get_concat_array( sc_signal_bool_vector* x, sc_signal_bool_vector* y )
{
    return (sc_signal_bool_vector*) sc_array_base::get_concat_array( x, y, new_array_vv );
}

sc_signal_bool_vector*
sc_signal_bool_vector::get_concat_array( sc_signal_bool_vector* x, sc_signal<bool>* y )
{
    return (sc_signal_bool_vector*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
}

sc_signal_bool_vector*
sc_signal_bool_vector::get_concat_array( sc_signal<bool>* x, sc_signal_bool_vector* y )
{
    return (sc_signal_bool_vector*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
}

sc_signal_bool_vector*
sc_signal_bool_vector::get_concat_array( sc_signal<bool>* x, sc_signal<bool>* y )
{
    return (sc_signal_bool_vector*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
}

sc_array_base*
sc_signal_bool_vector::new_array_vv( sc_array_base* x, sc_array_base* y )
{
    return new sc_signal_bool_vector( (sc_signal_bool_vector*) x, (sc_signal_bool_vector*) y );
}

sc_array_base*
sc_signal_bool_vector::new_array_vs( sc_array_base* x, void* y )
{
    return new sc_signal_bool_vector( (sc_signal_bool_vector*) x, (sc_signal<bool>*) y );
}

sc_array_base*
sc_signal_bool_vector::new_array_sv( void* x, sc_array_base* y )
{
    return new sc_signal_bool_vector( (sc_signal<bool>*) x, (sc_signal_bool_vector*) y );
}

sc_array_base*
sc_signal_bool_vector::new_array_ss( void* x, void* y )
{
    return new sc_signal_bool_vector( (sc_signal<bool>*) x, (sc_signal<bool>*) y );
}


void
sc_signal_bool_vector::print(ostream& os) const
{
    os << read();
}

void
sc_signal_bool_vector::dump(ostream& os) const
{
    int i;
    int len = length();
    sc_signal_array_base::dump(os);
    sc_bool_vector::dump(os);
    os << "new value = ";
    for (i = len - 1; i >= 0; --i) {
        os << get_signal(i)->get_new_value();
    }
    os << endl;
    os << "old value = ";
    for (i = len - 1; i >= 0; --i) {
        os << get_signal(i)->get_old_value();
    }
    os << endl;
}

void
sc_signal_bool_vector::trace( sc_trace_file* tf ) const
{
    ::sc_trace( tf, (const sc_bool_vector&) *this, name() );
}

#if !defined(_MSC_VER)

sc_signal_bool_vector&
operator,( sc_signal<bool>& x, sc_signal_bool_vector& y )
{
    return *(sc_signal_bool_vector::get_concat_array( &x, &y ));
}

const sc_signal_bool_vector&
operator,( const sc_signal<bool>& x, const sc_signal_bool_vector& y )
{
    return *(sc_signal_bool_vector::get_concat_array( (sc_signal<bool>*) &x, (sc_signal_bool_vector*) &y ));
}
#endif

sc_bool_vector&
sc_bool_vector::asgn_notop( const sc_bool_vector* a )
{
    int len = length();
    for (int i = len - 1; i >= 0; --i) {
        assign_help(i, ! *(a->get_data_ptr(i)));
    }
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const sc_bool_vector_notop& x )
{
    const sc_bool_vector* a = x.a;
    for (int i = length() - 1; i >= 0; --i) {
        super::write_help( i, ! *(a->get_data_ptr(i)) );
    }
    return *this;
}
