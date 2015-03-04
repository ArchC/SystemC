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

  sc_lv.h -- Packed logic vector class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef sc_lvH
#define sc_lvH

#include "systemc/datatypes/bit/sc_bv.h"
#include "systemc/datatypes/bit/sc_logic.h"

namespace sc_bv_ns
{

// ----------------------------------------------------------------------------
// base class - packed bit representation
// ----------------------------------------------------------------------------

class sc_lv_base : public sc_proxy<sc_lv_base>
{
  protected:
  unsigned long* data; // data array
  unsigned long* control;
  unsigned N; // array length
  typedef sc_logic elemtype;

  explicit sc_lv_base(char c);
  void init(unsigned long);
  public:
  // ctor/dtor
  sc_lv_base(){init(1);}
  virtual ~sc_lv_base(){delete[] data; delete[] control;}
  explicit sc_lv_base(unsigned long length){init(length);}
  explicit sc_lv_base(long length){init(length);}
  explicit sc_lv_base(unsigned length){init(length);}
  explicit sc_lv_base(int length){init(length);}
  explicit sc_lv_base( const sc_logic& y );
  sc_lv_base(unsigned length, long init_value);
  sc_lv_base(const sc_lv_base& y){init(y.length()); sc_bv_ns::assign_(*this,y);}
  sc_lv_base(const char* y){init(strlen(y)); assign_(y);}
  explicit sc_lv_base(const sc_unsigned& v){init(v.length()); assign_(v);}
  explicit sc_lv_base(const sc_signed& v){init(v.length()); assign_(v);}
  explicit sc_lv_base(const sc_uint_base& v){init(v.length()); assign_(v);}
  explicit sc_lv_base(const sc_int_base& v){init(v.length()); assign_(v);}

  //sc_lv_base(const sc_bv_base& t){convert(t);}
  template<class Y> sc_lv_base(const sc_proxy<Y>& y)
    {init(y.back_cast().length()); assign_(y);}
  template<class Y> sc_lv_base& operator=(const sc_proxy<Y>& y)
    { sc_bv_ns::assign_(*this,y); return *this;}
  sc_lv_base& operator=(const sc_lv_base& nv){sc_bv_ns::assign_(*this,nv); return *this;}
  sc_lv_base& operator=( const char* nv){assign_(nv); return *this;}
  sc_lv_base& operator=( const bool* nv){assign_(nv); return *this;}
  sc_lv_base& operator = ( const sc_logic* nv )
      { assign_( nv ); return *this; }

  sc_lv_base& operator=( const sc_unsigned& nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( const sc_signed& nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( const sc_int_base& nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( const sc_uint_base& nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( long nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( unsigned long nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( int nv ) {assign_(nv); return *this;}
  sc_lv_base& operator=( unsigned nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( int64 nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( uint64 nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( char nv ){assign_(nv); return *this;}
  sc_lv_base& operator=( const sc_bit& nv ){assign_(nv); return *this;}

    // common interface
    long get_bit(unsigned bit_number) const;
    void set_bit(unsigned bit_number, long value);
  unsigned long get_word(unsigned i) const {return data[i];}
  void set_word(unsigned i, unsigned long w) {data[i]=w;}
  unsigned long get_cword(unsigned i) const {return control[i];}
  void set_cword(unsigned i, unsigned long w) {control[i]=w;}
  // capacity functions
  int length()const { return N;}


  // operators specific to sc_lv_base

  sc_lv_base& operator &= ( const sc_unsigned& a )
      { return sc_proxy<sc_lv_base>::operator &= ( a ); }
  sc_lv_base& operator |= ( const sc_unsigned& a )
      { return sc_proxy<sc_lv_base>::operator |= ( a ); }
  sc_lv_base& operator ^= ( const sc_unsigned& a )
      { return sc_proxy<sc_lv_base>::operator ^= ( a ); }

  sc_lv_base& operator &= ( const sc_signed& a )
      { return sc_proxy<sc_lv_base>::operator &= ( a ); }
  sc_lv_base& operator |= ( const sc_signed& a )
      { return sc_proxy<sc_lv_base>::operator |= ( a ); }
  sc_lv_base& operator ^= ( const sc_signed& a )
      { return sc_proxy<sc_lv_base>::operator ^= ( a ); }

  sc_lv_base& operator &= ( unsigned int a )
      { return sc_proxy<sc_lv_base>::operator &= ( a ); }
  sc_lv_base& operator |= ( unsigned int a )
      { return sc_proxy<sc_lv_base>::operator |= ( a ); }
  sc_lv_base& operator ^= ( unsigned int a )
      { return sc_proxy<sc_lv_base>::operator ^= ( a ); }

  sc_lv_base& operator &= ( int a )
      { return sc_proxy<sc_lv_base>::operator &= ( a ); }
  sc_lv_base& operator |= ( int a )
      { return sc_proxy<sc_lv_base>::operator |= ( a ); }
  sc_lv_base& operator ^= ( int a )
      { return sc_proxy<sc_lv_base>::operator ^= ( a ); }

  sc_lv_base& operator &= ( unsigned long a )
      { return sc_proxy<sc_lv_base>::operator &= ( a ); }
  sc_lv_base& operator |= ( unsigned long a )
      { return sc_proxy<sc_lv_base>::operator |= ( a ); }
  sc_lv_base& operator ^= ( unsigned long a )
      { return sc_proxy<sc_lv_base>::operator ^= ( a ); }

  sc_lv_base& operator &= ( long a )
      { return sc_proxy<sc_lv_base>::operator &= ( a ); }
  sc_lv_base& operator |= ( long a )
      { return sc_proxy<sc_lv_base>::operator |= ( a ); }
  sc_lv_base& operator ^= ( long a )
      { return sc_proxy<sc_lv_base>::operator ^= ( a ); }

  sc_lv_base& operator &= ( const char* s )
      { sc_lv_base a( N ); a = s; return ( *this &= a ); }
  sc_lv_base& operator |= ( const char* s )
      { sc_lv_base a( N ); a = s; return ( *this |= a ); }
  sc_lv_base& operator ^= ( const char* s )
      { sc_lv_base a( N ); a = s; return ( *this ^= a ); }

  sc_lv_base operator & ( const char* s ) const
      { sc_lv_base b( *this ); return b &= s; }
  sc_lv_base operator | ( const char* s ) const
      { sc_lv_base b( *this ); return b |= s; }
  sc_lv_base operator ^ ( const char* s ) const
      { sc_lv_base b( *this ); return b ^= s; }

  friend sc_lv_base operator & ( const char* s, const sc_lv_base& b )
      { return b & s; }
  friend sc_lv_base operator | ( const char* s, const sc_lv_base& b )
      { return b | s; }
  friend sc_lv_base operator ^ ( const char* s, const sc_lv_base& b )
      { return b ^ s; }

  // member functions
  virtual void resize(unsigned long new_size);
  bool is_01();

  public: // make private later
  void get_packed_rep(unsigned long* ptr) const
    { if(data && N) memmove(ptr,data,((N-1)/ul_size+1)*sizeof(unsigned long));}
  void set_packed_rep(const unsigned long* new_buffer);
  void get_control_rep(unsigned long* ptr) const
    { if(data && N) memmove(ptr,control,((N-1)/ul_size+1)*sizeof(unsigned long));}
  void set_control_rep(const unsigned long* new_buffer);
  
  void clean_tail();
};


//-------------------------------------------------------------------------
// sc_lv<> template
//---------------------------------------------------------------------------

template<int W>
class sc_lv : public sc_lv_base
{
    void verify_length() {if(length()!=W) REPORT_ERROR(2000,"");}
    public:
    explicit sc_lv(long init_value);
    explicit sc_lv(bool init_value);
    sc_lv();
    // Copy constructor
    sc_lv(const sc_lv& y ):sc_lv_base(y){}
  sc_lv(const char* s);
  sc_lv(int v);
  explicit sc_lv(char c);
  sc_lv(const sc_unsigned& v);
  sc_lv(const sc_signed& v);
  sc_lv(const sc_uint_base& v);
  sc_lv(const sc_int_base& v);
  explicit sc_lv(const sc_logic& x);

  template<class Y> sc_lv(const sc_proxy<Y>& y)
    {init(y.back_cast().length()); assign_(y);}
  template<class T> sc_lv& operator=(const sc_proxy<T>& t)
    { sc_bv_ns::assign_(*this,t); return *this;}

    virtual sc_lv_base* clone();
  // Assignments may be made to a sc_lv from various types of
  // objects.
  sc_lv& operator=(const sc_lv& t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const char* t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const bool* t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
    sc_lv& operator = ( const sc_logic* t )
	{ sc_lv_base::operator = ( t ); return *this; }

  sc_lv& operator=( const sc_unsigned& t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const sc_signed& t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const sc_uint_base& t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const sc_int_base& t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( long t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( unsigned long t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( int t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( unsigned t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( char t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const sc_bit& t )
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=(int64 t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=(uint64 t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const sc_int<W>& t ){ return operator=((int64)t);}
  sc_lv& operator=( const sc_uint<W>& t ){ return operator=((uint64)t);}
  virtual void resize(unsigned long new_size);
};


// ----------------------------------------------------------------------------
// inline and template functions
// ----------------------------------------------------------------------------

template <int W>
inline
sc_lv<W>::sc_lv()
{
    init( W );
}

template <int W>
inline
void
sc_lv<W>::resize( unsigned long new_size )
{
    REPORT_ERROR( 2006, sc_string::to_string( "sc_lv<%d>",W).c_str() );
}

template <int W>
inline
sc_lv_base*
sc_lv<W>::clone()
{
    return new sc_lv<W>( *this );
}

template <int W>
inline
sc_lv<W>::sc_lv( long init_value )
: sc_lv_base( W, init_value )
{}

template <int W>
inline
sc_lv<W>::sc_lv( bool init_value )
: sc_lv_base( W, (long) init_value )
{}

template <int W>
inline
sc_lv<W>::sc_lv( const char* s )
: sc_lv_base( W )
{
    assign_( s );
}

template <int W>
inline
sc_lv<W>::sc_lv( int v )
: sc_lv_base( W )
{
    *this = v;
}

template <int W>
inline
sc_lv<W>::sc_lv( char c )
: sc_lv_base( W, sc_logic::char_to_logic[c] )
{}

template <int W>
inline
sc_lv<W>::sc_lv( const sc_unsigned& v )
: sc_lv_base( W )
{
    assign_( v );
}

template <int W>
inline
sc_lv<W>::sc_lv( const sc_signed& v )
: sc_lv_base( W )
{
    assign_( v );
}

template <int W>
inline
sc_lv<W>::sc_lv( const sc_uint_base& v )
: sc_lv_base( W )
{
    assign_( v );
}

template <int W>
inline
sc_lv<W>::sc_lv( const sc_int_base& v )
: sc_lv_base( W )
{
    assign_( v );
}

template <int W>
inline
sc_lv<W>::sc_lv( const sc_logic& x )
: sc_lv_base( W, x.to_long() )
{}


// inline functions

inline
long
sc_lv_base::get_bit( unsigned n ) const
{
    unsigned int wi = n / ul_size; // word index
    unsigned int bi = n % ul_size; // bit index

    return data[wi] >> bi & 1 | control[wi] >> bi << 1 & 2;
}

inline
void
sc_lv_base::set_bit( unsigned n, long value )
{
    unsigned int wi = n / ul_size; // word index
    unsigned int bi = n % ul_size; // bit index
    unsigned long mask = 1ul << bi;

    data[wi]    |= mask; // set bit to 1
    control[wi] |= mask; // set bit to 1
    data[wi]    &= value << bi | ~mask;
    control[wi] &= value >> 1 << bi | ~mask;
}


// implementations for some sc_proxy methods

template <class X>
inline
const sc_lv_base
sc_proxy<X>::operator , ( const char* s ) const
{
    sc_lv_base result = ( back_const_cast(), sc_lv_base( s ) );
    return result;
}

template <class X>
inline
const sc_lv_base
operator , ( const char* s, const sc_proxy<X>& x )
{
    sc_lv_base result = ( sc_lv_base( s ), x.back_const_cast() );
    return result;
}

template <class X>
inline
const sc_concat<X,const sc_lv<1> >
sc_proxy<X>::operator , ( const sc_logic& y ) const
{
    return sc_concat<X,const sc_lv<1> >( CCAST<X&>( back_cast() ),
					 sc_lv<1>( y ) );
}


//---------------------------------------------------------------------------
} // namespace sc_bv_ns


#endif
