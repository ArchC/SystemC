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
  sc_bv.cpp (.h) -- packed bit vector class

  Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)
             No animals were harmed during the creation of this code.

******************************************************************************/



/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/
#ifndef sc_bvH
#define sc_bvH

#include "sc_proxy.h"
#include "sc_bit_proxies.h"

class sc_bool_vector;
class sc_logic_vector;


namespace sc_bv_ns
{

//-------------------------------------------------------------------------
// base class - packed bit representation
//-------------------------------------------------------------------------
class sc_bv_base: public sc_proxy<sc_bv_base>
{
  typedef sc_proxy<sc_bv_base> inherited;
  protected:
  unsigned long* data; // data array
  unsigned N; // array length
  friend class sc_lv_base;

  explicit sc_bv_base(char);
  void init(unsigned long);
  
  public:
  // ctor/dtor
#if !defined(__SUNPRO_CC)
  template<class T> sc_bv_base(const sc_proxy<T>& y)
	{init(y.back_cast().length());assign_(y);}
#else
  // provide at least construction from sc_lv_base
  sc_bv_base(const sc_lv_base& v);
#endif

  sc_bv_base(){init(1);} // VSIA requirements, don't ever use it; to be amended
  sc_bv_base(const sc_bv_base&);
  virtual ~sc_bv_base(){if(N) delete[] data;}
  explicit sc_bv_base(unsigned long length){init(length);}
  explicit sc_bv_base(long length){init(length);}
  explicit sc_bv_base(unsigned length){init(length);}
  explicit sc_bv_base(int length){init(length);}
  sc_bv_base(unsigned length, bool init_value);
  explicit sc_bv_base(const sc_bool_vector& v);
  sc_bv_base(const char*);
  explicit sc_bv_base(const sc_unsigned& v){init(v.length()); assign_(v);}
  explicit sc_bv_base(const sc_signed& v){init(v.length()); assign_(v);}
  explicit sc_bv_base(const sc_uint_base& v){init(v.length()); assign_(v);}
  explicit sc_bv_base(const sc_int_base& v){init(v.length()); assign_(v);}

  // Assignments may be made to a sc_bv from various types of
  // objects. Only common bits are changed.
#if !defined(__SUNPRO_CC)
  template<class T> sc_bv_base& operator=(const sc_proxy<T>& t){return assign_(t);}
  sc_bv_base& operator=(const sc_bv_base& t){return assign_(t);}
#else
  sc_bv_base& operator=(const sc_bv_base& t)
    { sc_bv_ns::assign_((*this),t); return *this;}
  sc_bv_base& operator=(const sc_lv_base& t);
#endif
  sc_bv_base& operator=( const char* t){return assign_(t);}
  sc_bv_base& operator=( const bool* t){return assign_(t);}
  sc_bv_base& operator=( const sc_unsigned& t ){return assign_(t);}
  sc_bv_base& operator=( const sc_signed& t ){return assign_(t);}
  sc_bv_base& operator=( const sc_uint_base& t ){return assign_(t);}
  sc_bv_base& operator=( const sc_int_base& t ){return assign_(t);}
  sc_bv_base& operator=( long t ){return assign_(t);}
  sc_bv_base& operator=( unsigned long t ) {return assign_(t);}
  sc_bv_base& operator=( int t ) {return assign_(t);}
  sc_bv_base& operator=( unsigned t ){return assign_(t);}
  sc_bv_base& operator=( int64 t ){return assign_(t);}
  sc_bv_base& operator=( uint64 t ){return assign_(t);}
  sc_bv_base& operator=( char t ){return assign_(t);}
  sc_bv_base& operator=( const sc_bit& t ){return assign_(t);}
  sc_bv_base& operator=(const sc_bool_vector& t);

  // common functions thru which sc_proxy implements all operators
  inline long get_bit(unsigned n) const;
  inline void set_bit(unsigned bit_number, long value);
  unsigned long get_word(unsigned i) const {return data[i];}
  void set_word(unsigned i, unsigned long w) {data[i]=w;}
  unsigned long get_cword(unsigned i) const {return 0;}
  void set_cword(unsigned i, unsigned long w);
  int length()const { return N;}
  // operators specific to sc_bv_base
#if !defined(__GNUC__)
  // gcc 2.95.2 bug makes it impossible to use operators with char*
  using sc_proxy<sc_bv_base>::operator &=;
  using sc_proxy<sc_bv_base>::operator |=;
  using sc_proxy<sc_bv_base>::operator ^=;
  sc_bv_base& operator &= (const char* s){return (*this)&=sc_bv_base(s);}
  sc_bv_base& operator |= (const char* s){return (*this)|=sc_bv_base(s);}
  sc_bv_base& operator ^= (const char* s){return (*this)^=sc_bv_base(s);}
  sc_bv_base operator & (const char* s)const
    { sc_bv_base b(*this); return b&=s;}
  sc_bv_base operator | (const char* s)const
    { sc_bv_base b(*this); return b|=s;}
  sc_bv_base operator ^ (const char* s)const
    { sc_bv_base b(*this); return b^=s;}
  friend sc_bv_base operator &(const char* s, const sc_bv_base& b){ return b&s;}
  friend sc_bv_base operator |(const char* s, const sc_bv_base& b){ return b|s;}
  friend sc_bv_base operator ^(const char* s, const sc_bv_base& b){ return b^s;}
#endif
  // member functions
  void set(unsigned long v=0); // fill with v
  virtual void resize(unsigned long new_size);

  public: // make it private later
  // copy the internal buffer into ptr
  void get_packed_rep(unsigned long* ptr) const
    { if(data && N) memmove(ptr,data,((N-1)/ul_size+1)*sizeof(unsigned long) );}
  void set_packed_rep(const unsigned long* new_buffer);
  void clean_tail();
  // additional functions required by VSIA
  sc_bv_base (const char*, int);  // With length argument.
};
//-------------------------------------------------------------------------
// sc_bv<> bit-vector class
//---------------------------------------------------------------------------
template<int W>
class sc_bv : public sc_bv_base
{
  public:
  // ctor/dtor
  sc_bv():sc_bv_base(W){} // uninitialized
  explicit sc_bv(bool init_value):sc_bv_base(W,init_value){}
#if !defined(__SUNPRO_CC)
  template<class X> sc_bv(const sc_proxy<X>& t):sc_bv_base(t)
  {
    if(W!=t.back_cast().length())
     REPORT_ERROR(1002,"different lengths");
  }
#endif
  sc_bv(const sc_bv& y ):sc_bv_base(y){}
  sc_bv(const char*);
  explicit sc_bv(char c):sc_bv_base(W,sc_logic::char_to_logic[c]){}// potentially confusing
  explicit sc_bv(int v):sc_bv_base(W){(*this)=v;}
  explicit sc_bv(const sc_bool_vector& v):sc_bv_base(v){}
  explicit sc_bv(const sc_logic_vector& v);
  explicit sc_bv(const sc_unsigned& v):sc_bv_base(W){ assign_(v);}
  explicit sc_bv(const sc_signed& v):sc_bv_base(W){ assign_(v);}
  explicit sc_bv(const sc_uint_base& v):sc_bv_base(W){ assign_(v);}
  explicit sc_bv(const sc_int_base& v):sc_bv_base(W){ assign_(v);}

  virtual sc_bv_base* clone(){return new sc_bv<W>(*this);}
  // when assignments are made from arithmetic types or strings
  // only common bits are assigned
  // assignment from bit-vectors are allowed only when they
  // have the same lengths
#if !defined(__SUNPRO_CC)
  template<class T> sc_bv& operator=(const sc_proxy<T>& t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
#endif
  sc_bv& operator=(const sc_bv& t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const char* t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const bool* t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_unsigned& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_signed& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_uint_base& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_int_base& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( long t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( unsigned long t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( int t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( unsigned t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( char t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_bit& t )
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=(int64 t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=(uint64 t)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(t));}
  sc_bv& operator=( const sc_int<W>& t )
   { return operator=((int64)t);}
  sc_bv& operator=( const sc_uint<W>& t )
   { return operator=((uint64)t);}
  sc_bv& operator=(const sc_bool_vector& v)
   { return static_cast<sc_bv&>(sc_bv_base::operator=(v));}
  sc_bv& operator=(const sc_logic_vector& v);

  virtual void resize(unsigned long new_size)
   { REPORT_ERROR(2006," - sc_bv"); }
};


// ----------------------------------------------------------------------------
// sc_bv_base templates
// ----------------------------------------------------------------------------
#if defined(__BCPLUSPLUS__)
#pragma warn -ccc
#pragma warn -rch
#endif
inline long sc_bv_base::get_bit(unsigned n) const
{ // gcc compiles idiv into a function call
  // instead of calling native processor command
  // ignore compiler warnings that conditions are
  // always true
  if(ul_size==32)
    return (data[n>>5] & (1<< (n & 0x1F)))!=0;
  else
    return (data[n/ul_size]&(1<<n%ul_size)) !=0;
}

inline void sc_bv_base::set_bit(unsigned n, long value)
{
  if(value<0 || value>1)
  {
    //sprintf(s,"sc_bv_base::set_bit(%d,%dl)",n,value);
    REPORT_WARNING(2010," sc_bv_base::set_bit");
    return;
  }
  if(ul_size==32)
  {
    if(value)
      data[n>>5] |= 1<< (n & 0x1F);
    else
      data[n>>5] &= ~(1<< (n & 0x1F));
  }
  else
  {
    if(value)
      data[n/ul_size] |= 1<< n%ul_size;
    else
      data[n/ul_size] &= ~(1<< n%ul_size);
  }
}
#if defined(__BCPLUSPLUS__)
#pragma warn +ccc
#pragma warn +rch
#endif

inline void sc_bv_base::set_cword(unsigned i, unsigned long w)
{
#if defined(SYSTEMC_DEBUG)
  check_wbounds(i);
#endif
#if !defined(SC_NO_WARNINGS)
  if(w) REPORT_WARNING(2004,"");
#endif
}

// ----------------------------------------------------------------------------
// sc_bv<> templates
// ----------------------------------------------------------------------------
template<int W>
sc_bv<W>::sc_bv(const char* ss)
{ // slow
   const sc_string s = convert_to_bin(ss);
   const unsigned s_length = s.length();
   if(s_length==0)
     REPORT_ERROR(2001,"");
   data = new unsigned long[(W-1)/ul_size +1]; // can throw
   N=W;
   assign_(s);
   clean_tail();
}
template<int W>
sc_bv<W>::sc_bv(const sc_logic_vector& v)
{
   if(W!=(unsigned)v.length())
    REPORT_ERROR(2000,"");
   const unsigned reserved = (W-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   N=W;
   assign_(v);
   clean_tail();
}
template<int W>
sc_bv<W>& sc_bv<W>::operator=(const sc_logic_vector& v)
{
  return static_cast<sc_bv<W> >(assign_(v));
}

}
//---------------------------------------------------------------------------
#endif
