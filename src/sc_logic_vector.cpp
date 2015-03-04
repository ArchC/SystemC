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

    sc_logic_vector.cpp - implementation of sc_logic_vector.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <cstring>


#include "sc_macros_int.h"
#include "sc_bvrep.h"
#include "sc_bit.h"

#include "sc_mempool.h"
#include "sc_signal.h"
#include "sc_logic_vector.h"

#include "sc_bool_vector.h"

#include "sc_lv.h"
#include "numeric_bit/sc_int.h"
#include "numeric_bit/sc_uint.h"

#ifdef __GNUC__
#define SIGNAL_ARRAY_DELAY sc_signal_array_delay_gnu<sc_logic_vector,sc_logic>
#else
#define SIGNAL_ARRAY_DELAY sc_signal_array_delay<sc_logic_vector>
#endif


#if !defined(_MSC_VER) && !defined(__HP_aCC)
template <class T>
inline T min(T t1, T t2) { return t1>t2?t2:t1;}
#endif

sc_logic_vector::sc_logic_vector( int len )
    : sc_array<sc_logic>( len )
{
    str = 0;
}

sc_logic_vector::sc_logic_vector( int len, const sc_logic_vector& ref )
    : sc_array<sc_logic>( len, (const sc_array<sc_logic>&) ref )
{
    str = 0;
}

sc_logic_vector::sc_logic_vector( const sc_logic_vector& y )
    : sc_array<sc_logic>(y)
{
    str = 0;
}

sc_logic_vector::~sc_logic_vector()
{
    sc_mempool::release(str, 1 + length());
}

sc_logic_vector&
sc_logic_vector::sub( int i, int j )
{
    return dynamic_cast<sc_logic_vector&>(sc_array_base::sub( i, j ));
}


sc_logic_vector::sc_logic_vector( sc_logic* initialized_data, int len )
    : sc_array<sc_logic>( initialized_data, len )
{
    str = 0;
}

sc_logic_vector::sc_logic_vector( sc_logic** data_ptrs, int len )
    : sc_array<sc_logic>( data_ptrs, len )
{
    str = 0;
}

/******************************************************************************
   Now we come to the constructors responsible for creating concatenated
   arrays ...
******************************************************************************/

sc_logic_vector::sc_logic_vector( sc_logic_vector* v1, sc_logic_vector* v2 )
    : sc_array<sc_logic>( v1, v2 )
{
    str = 0;
}

sc_logic_vector::sc_logic_vector( sc_logic_vector* v1, sc_logic* v2 )
    : sc_array<sc_logic>( v1, v2 )
{
    str = 0;
}

sc_logic_vector::sc_logic_vector( sc_logic* v1, sc_logic_vector* v2 )
    : sc_array<sc_logic>( v1, v2 )
{
    str = 0;
}

sc_logic_vector::sc_logic_vector( sc_logic* v1, sc_logic* v2 )
    : sc_array<sc_logic>( v1, v2 )
{
    str = 0;
}


void
sc_logic_vector::assign_concat_with_string( const sc_logic_vector* v1, const char* v2 )
{
    int i, j;
    for (i = length() - 1, j = v1->length() - 1; j >= 0; --i, --j) {
        assign_help( i, *(v1->get_data_ptr(j)) );
    }
    for ( ; i >= 0; --i) {
        assign_help( i, sc_logic(*v2) );
        ++v2;
    }
}

/* When a char* is used as an argument for concatenation, the resulting
   vector is a const and thus has its own storage! */
sc_logic_vector::sc_logic_vector( const sc_logic_vector* v1, const char* v2 )
    : sc_array<sc_logic>( v1->length() + strlen(v2) )
{
    assign_concat_with_string( v1, v2 );
    str = 0;
}


void
sc_logic_vector::assign_concat_with_string( const char* v1, const sc_logic_vector* v2 )
{
    int i;
    int jlen = v2->length();
    for (i = length() - 1; i >= jlen; --i) {
        assign_help( i, sc_logic(*v1) );
        ++v1;
    }
    for ( ; i >= 0; --i) {
        assign_help( i, *(v2->get_data_ptr(i)) );
    }
}

sc_logic_vector::sc_logic_vector( const char* v1, const sc_logic_vector* v2 )
    : sc_array<sc_logic>( strlen(v1) + v2->length() )
{
    assign_concat_with_string( v1, v2 );
    str = 0;
}


sc_logic_vector&
sc_logic_vector::operator=( const sc_logic_vector& nv )
{
    sc_array<sc_logic>::operator=(nv);
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( const char* nv )
{
    int str_len = strlen(nv);
    int len = min(length(),str_len);
    nv += str_len - len;
    for (int i = len - 1; i >= 0; --i) {
        assign_help( i, sc_logic(*nv) );
        ++nv;
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( const sc_logic* nv )
{
    sc_array<sc_logic>::operator=(nv);
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( long v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        assign_help( i, sc_logic(v & 1) );
        v >>= 1;
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( unsigned long v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        assign_help( i, sc_logic(long(v & 1)) );
        v >>= 1;
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( int64 v )
{
  int len = length();
    for (int i = 0; i < len; ++i) {
        assign_help( i, sc_logic(long(v & 1)) );
//#ifndef WIN32
//        assign_help( i, sc_logic(v & 1ULL) );
//#else
//	assign_help( i, sc_logic(v & 1i64) );
//#endif
        v >>= 1;
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( uint64 v )
{
  int len = length();
    for (int i = 0; i < len; ++i) {
        assign_help( i, sc_logic(long(v & 1)) );
//#ifndef WIN32
//        assign_help( i, bool(v & 1ULL) );
//#else
//	assign_help( i, sc_logic(v & 1i64) );
//#endif
        v >>= 1;
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator=( const sc_bool_vector& nv )
{
    int len = nv.length();
    check_length(len);
    for (int i = length() - 1; i >= 0; --i) {
        assign_help( i, sc_logic(*nv.get_data_ptr(i)) );
    }
    return *this;
}

sc_logic_vector& sc_logic_vector::operator=( const sc_bv_ns::sc_bv_base& v)
{
  if(length()!= v.length())
    REPORT_ERROR(2000,"");
  for(unsigned i=0; i<(unsigned)(length()); i++)
    (*this)[i]=v[i];
  return *this;
}
sc_logic_vector& sc_logic_vector::operator=( const sc_bv_ns::sc_lv_base& v)
{
  if(length()!= v.length())
    REPORT_ERROR(2000,"");
  for(unsigned i=0; i<(unsigned)(length()); i++)
    (*this)[i]=v[i];
  return *this;
}

sc_logic_vector& sc_logic_vector::operator=( const sc_int_base& v)
{return (*this)=v.to_int64();}
sc_logic_vector& sc_logic_vector::operator=( const sc_uint_base& v)
{return (*this)=v.to_uint64();}

bool
sc_logic_vector::operator==( const char* rhs ) const
{
    int len = length();
    rhs += (strlen(rhs) - len);
    for (int i = len - 1; i >= 0; --i) {
        if (*get_data_ptr(i) != *rhs)
            return false;
        rhs++;
    }
    return true;
}

bool
sc_logic_vector::operator==( const sc_bool_vector& rhs ) const
{
    int len = rhs.length();
    check_length(len);

    for (int i = len - 1; i >= 0; --i) {
        const sc_logic* su = get_data_ptr(i);
        if (*su != '0' && *su != '1')
            return false;
        if (su->to_bool() != (*rhs.get_data_ptr(i)))
            return false;
    }
    return true;
}

#include "sc_logic_vector_and.inc"
#include "sc_logic_vector_or.inc"
#include "sc_logic_vector_xor.inc"

sc_logic_vector&
sc_logic_vector::negate()
{
    for (int i = length() - 1; i >= 0; --i) {
        get_data_ptr(i)->negate();
    }
    return *this;
}

const char*
sc_logic_vector::to_string() const
{
    if (str == 0)
        str = (char*) sc_mempool::allocate(1 + length());
    to_string( str );
    return str;
}

void
sc_logic_vector::to_string( char* buffer ) const
{
    for (int i = length() - 1; i >= 0; --i) {
        *buffer++ = get_data_ptr(i)->to_char();
    }
    *buffer = '\0';
}

void
sc_logic_vector::dump(ostream& os) const
{
    sc_array_base::dump(os);
    os << "value = " << *this << endl;
}

int
sc_logic_vector::to_signed() const
{
    int len = length();
    int v;
    if ((unsigned)len > sizeof(int)*8) {
       REPORT_WARNING(2008,"");
        v = 0;
        for (int i = sizeof(int)*8 - 1; i >= 0; --i) {
            v = (v << 1) + int(get_data_ptr(i)->to_bool());
        }
    } else {
        v = - int(get_data_ptr(len - 1)->to_bool());
        for (int i = len - 2; i >= 0; --i) {
            v = (v << 1) + int(get_data_ptr(i)->to_bool());
        }
    }
    return v;
}

unsigned
sc_logic_vector::to_unsigned() const
{
    int len = length();
    unsigned v = 0;
    if ((unsigned) len > sizeof(int)*8) {
      REPORT_WARNING(2008,"");
        for (int i = sizeof(int)*8 - 1; i >= 0; --i) {
            v = (v << 1) + int(get_data_ptr(i)->to_bool());
        }
    } else {
        for (int i = len - 1; i >= 0; --i) {
            v = (v << 1) + int(get_data_ptr(i)->to_bool());
        }
    }
    return v;
}

sc_logic_vector::sc_logic_vector( const sc_array_base* p, int i, int j )
    : sc_array<sc_logic>( p, i, j )
{
    str = 0;
}

sc_array_base*
sc_logic_vector::vctor( const sc_array_base* p, int i, int j ) const
{
    return new sc_logic_vector( p, i, j );
}


ostream&
operator<<(ostream& os, const sc_logic_vector& slv)
{
    return (os << slv.to_string());
}

void
sc_logic_vector::print(ostream& os) const
{
    os << *this;
}

/*****************************************************************************/

sc_signal_logic_vector::sc_signal_logic_vector( int len )
    : super( len )
{
    /*EMPTY*/
}

sc_signal_logic_vector::sc_signal_logic_vector( const char* nm, int len )
    : super( nm, len )
{
    /*EMPTY*/
}

sc_signal_logic_vector::sc_signal_logic_vector( int len, const sc_signal_logic_vector& ref )
    : super( len, (const super&) ref )
{
    /*EMPTY*/
}

sc_signal_logic_vector::sc_signal_logic_vector( const sc_signal_logic_vector& ref )
    : super( ref )
{
    /*EMPTY*/
}

sc_signal_logic_vector::~sc_signal_logic_vector()
{
    /*EMPTY*/
}

sc_signal_logic_vector&
sc_signal_logic_vector::sub( int i, int j )
{
    return dynamic_cast<sc_signal_logic_vector&>(sc_array_base::sub(i,j));
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( const sc_array<sc_logic>& nv )
{
    super::write( nv );
    return *this;
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( const sc_logic* nv )
{
    super::write( nv );
    return *this;
}

static void
get_charstring_val( sc_logic& nv, const void* nva, int i )
{
    nv = ((char*)nva)[i];
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( const char* nva )
{
    nva += (strlen(nva) - length());
    super::write_help( get_charstring_val, nva, /*left_to_right*/ true );
    return *this;
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( int v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        super::write_help( i, sc_logic(v&1) );
        v >>= 1;
    }
    return *this;
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( int64 v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        super::write_help( i, sc_logic(long(v&1)) );
//#ifndef WIN32
//        super::write_help( i, char(v&1ULL) );
//#else
//        super::write_help( i, char(v&1i64) );
//#endif
        v >>= 1;
    }
    return *this;
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( unsigned v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        super::write_help( i, sc_logic(long(v&1)) );
        v >>= 1;
    }
    return *this;
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( uint64 v )
{
    int len = length();
    for (int i = 0; i < len; ++i) {
        super::write_help( i, sc_logic(long(v&1)) );
//#ifndef WIN32
//        super::write_help( i, char(v & 1ULL) );
//#else
//	super::write_help( i, char(v & 1i64) );
//#endif
        v >>= 1;
    }
    return *this;
}


void
get_sc_bool_vector_val(sc_logic& result, const void* nva, int i)
{
    const sc_bool_vector& bv = *((const sc_bool_vector*) nva);
    result = char(bv[i]);
}

sc_signal_logic_vector&
sc_signal_logic_vector::write( const sc_bool_vector& nv )
{
    check_length(nv.length());
    super::write_help( get_sc_bool_vector_val, (void*) &nv, /*left_to_right*/false );
    return *this;
}

sc_signal_logic_vector::sc_signal_logic_vector( const sc_array_base* p, int i, int j )
    : super( p, i, j )
{
    /*EMPTY*/
}

sc_array_base*
sc_signal_logic_vector::vctor( const sc_array_base* p, int i, int j ) const
{
    return new sc_signal_logic_vector( p, i, j );
}

sc_signal_logic_vector::sc_signal_logic_vector( sc_signal_logic_vector* v1, sc_signal_logic_vector* v2 )
    : super( v1, v2 )
{
    /*EMPTY*/
}

sc_signal_logic_vector::sc_signal_logic_vector( sc_signal_logic_vector* v1, sc_signal<sc_logic>* v2 )
    : super( v1, v2 )
{
    /*EMPTY*/
}

sc_signal_logic_vector::sc_signal_logic_vector( sc_signal<sc_logic>* v1, sc_signal_logic_vector* v2 )
    : super( v1, v2 )
{
    /*EMPTY*/
}

sc_signal_logic_vector::sc_signal_logic_vector( sc_signal<sc_logic>* v1, sc_signal<sc_logic>* v2 )
    : super( v1, v2 )
{
    /*EMPTY*/
}

/*****************************************************************************/

sc_logic_vector
operator~( const sc_logic_vector& a )
{
    int len = a.length();
    sc_logic* c = new sc_logic[len];
    for (int i = len - 1; i >= 0; --i) {
        c[i] = ~ a[i];
    }
    return sc_logic_vector( c, len );
}

/*****************************************************************************/

sc_logic_vector*
sc_logic_vector::get_concat_array( sc_logic_vector* x, sc_logic_vector* y )
{
    return (sc_logic_vector*) sc_array_base::get_concat_array( x, y, new_array_vv );
}

sc_logic_vector*
sc_logic_vector::get_concat_array( sc_logic_vector* x, sc_logic* y )
{
    return (sc_logic_vector*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
}

sc_logic_vector*
sc_logic_vector::get_concat_array( sc_logic* x, sc_logic_vector* y )
{
    return (sc_logic_vector*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
}

sc_logic_vector*
sc_logic_vector::get_concat_array( sc_logic* x, sc_logic* y )
{
    return (sc_logic_vector*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
}

const sc_logic_vector*
sc_logic_vector::get_concat_array( sc_logic_vector* x, const char* y )
{
    bool is_new;
    sc_logic_vector* sulv =
        (sc_logic_vector*) sc_array_base::get_concat_array( x, y, new_array_vc, &is_new );
    if (! is_new) {
        sulv->assign_concat_with_string( x, y );
    }
    return sulv;
}

const sc_logic_vector*
sc_logic_vector::get_concat_array( const char* x, sc_logic_vector* y )
{
    bool is_new;
    sc_logic_vector* sulv =
        (sc_logic_vector*) sc_array_base::get_concat_array( x, y, new_array_cv, &is_new );
    if (! is_new) {
        sulv->assign_concat_with_string( x, y );
    }
    return sulv;
}

sc_array_base*
sc_logic_vector::new_array_vv( sc_array_base* x, sc_array_base* y )
{
    return new sc_logic_vector( (sc_logic_vector*) x, (sc_logic_vector*) y );
}

sc_array_base*
sc_logic_vector::new_array_vs( sc_array_base* x, void* y )
{
    return new sc_logic_vector( (sc_logic_vector*) x, (sc_logic*) y );
}

sc_array_base*
sc_logic_vector::new_array_sv( void* x, sc_array_base* y )
{
    return new sc_logic_vector( (sc_logic*) x, (sc_logic_vector*) y );
}

sc_array_base*
sc_logic_vector::new_array_ss( void* x, void* y )
{
    return new sc_logic_vector( (sc_logic*) x, (sc_logic*) y );
}

sc_array_base*
sc_logic_vector::new_array_vc( sc_array_base* x, const char* y )
{
    return new sc_logic_vector( (sc_logic_vector*) x, (char*) y );
}

sc_array_base*
sc_logic_vector::new_array_cv( const char* x, sc_array_base* y )
{
    return new sc_logic_vector( (char*) x, (sc_logic_vector*) y );
}


#if !defined(_MSC_VER)

sc_logic_vector&
operator,( sc_logic& x, sc_logic_vector& y )
{
    return *(sc_logic_vector::get_concat_array( &x, &y ));
}

const sc_logic_vector&
operator,( const char* x, const sc_logic_vector& y )
{
    return *(sc_logic_vector::get_concat_array( x, (sc_logic_vector*) &y ));
}

const sc_logic_vector&
operator,( const sc_logic& x, const sc_logic_vector& y )
{
    return *(sc_logic_vector::get_concat_array( (sc_logic*) &x, (sc_logic_vector*) &y ));
}

const sc_logic_vector&
operator,( const sc_signal<sc_logic>& x, const sc_logic_vector& y )
{
    return *(sc_logic_vector::get_concat_array( (sc_logic*) x.get_data_ptr(), (sc_logic_vector*) &y ));
}
#endif
/*****************************************************************************/

sc_signal_logic_vector*
sc_signal_logic_vector::get_concat_array( sc_signal_logic_vector* x, sc_signal_logic_vector* y )
{
    return (sc_signal_logic_vector*) sc_array_base::get_concat_array( x, y, new_array_vv );
}

sc_signal_logic_vector*
sc_signal_logic_vector::get_concat_array( sc_signal_logic_vector* x, sc_signal<sc_logic>* y )
{
    return (sc_signal_logic_vector*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
}

sc_signal_logic_vector*
sc_signal_logic_vector::get_concat_array( sc_signal<sc_logic>* x, sc_signal_logic_vector* y )
{
    return (sc_signal_logic_vector*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
}

sc_signal_logic_vector*
sc_signal_logic_vector::get_concat_array( sc_signal<sc_logic>* x, sc_signal<sc_logic>* y )
{
    return (sc_signal_logic_vector*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
}

sc_array_base*
sc_signal_logic_vector::new_array_vv( sc_array_base* x, sc_array_base* y )
{
    return new sc_signal_logic_vector( (sc_signal_logic_vector*) x, (sc_signal_logic_vector*) y );
}

sc_array_base*
sc_signal_logic_vector::new_array_vs( sc_array_base* x, void* y )
{
    return new sc_signal_logic_vector( (sc_signal_logic_vector*) x, (sc_signal<sc_logic>*) y );
}

sc_array_base*
sc_signal_logic_vector::new_array_sv( void* x, sc_array_base* y )
{
    return new sc_signal_logic_vector( (sc_signal<sc_logic>*) x, (sc_signal_logic_vector*) y );
}

sc_array_base*
sc_signal_logic_vector::new_array_ss( void* x, void* y )
{
    return new sc_signal_logic_vector( (sc_signal<sc_logic>*) x, (sc_signal<sc_logic>*) y );
}


void
sc_signal_logic_vector::print(ostream& os) const
{
    os << read();
}

void
sc_signal_logic_vector::dump(ostream& os) const
{
    int i;
    int len = length();
    sc_signal_array_base::dump(os);
    sc_logic_vector::dump(os);
    os << "new value = ";
    for (i = len - 1; i >= 0; --i) {
        os << get_signal(i)->get_new_value();
    }
    os << "old value = ";
    for (i = len - 1; i >= 0; --i) {
        os << get_signal(i)->get_old_value();
    }
    os << endl;
}

void
sc_signal_logic_vector::trace( sc_trace_file* tf ) const
{
    ::sc_trace( tf, (const sc_logic_vector&) *this, name() );
}

#if !defined(_MSC_VER)

sc_signal_logic_vector&
operator,( sc_signal<sc_logic>& x, sc_signal_logic_vector& y )
{
    return *(sc_signal_logic_vector::get_concat_array( &x, &y ));
}

const sc_signal_logic_vector&
operator,( const sc_signal<sc_logic>& x, const sc_signal_logic_vector& y )
{
    return *(sc_signal_logic_vector::get_concat_array( (sc_signal<sc_logic>*) &x, (sc_signal_logic_vector*) &y ));
}
#endif
