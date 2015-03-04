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
  sc_lv.cpp (.h) -- packed logic vector class

  Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/



/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
#ifndef sc_lvH
#define sc_lvH

#include "sc_bv.h"
#include "sc_logic.h"

class sc_logic_vector;
class sc_bool_vector;

namespace sc_bv_ns
{
//---------------------------------------------------------------------------
// base class - packed bit representation
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
  explicit sc_lv_base(const sc_logic& y);
  sc_lv_base(unsigned length, long init_value);
  sc_lv_base(const sc_lv_base& y){init(y.length()); sc_bv_ns::assign_(*this,y);}
  sc_lv_base(const char* y){init(strlen(y)); assign_(y);}
  explicit sc_lv_base(const sc_unsigned& v){init(v.length()); assign_(v);}
  explicit sc_lv_base(const sc_signed& v){init(v.length()); assign_(v);}
  explicit sc_lv_base(const sc_uint_base& v){init(v.length()); assign_(v);}
  explicit sc_lv_base(const sc_int_base& v){init(v.length()); assign_(v);}

#if !defined(__SUNPRO_CC)
  //sc_lv_base(const sc_bv_base& t){convert(t);}
#else
  sc_lv_base(const sc_bv_base& t){sc_bv_ns::assign_(*this,t);}
#endif
  explicit sc_lv_base(const sc_bool_vector& y);
  explicit sc_lv_base(const sc_logic_vector& y);
#if !defined(__SUNPRO_CC)
  template<class Y> sc_lv_base(const sc_proxy<Y>& y)
    {init(y.back_cast().length()); assign_(y);}
  template<class Y> sc_lv_base& operator=(const sc_proxy<Y>& y)
    { sc_bv_ns::assign_(*this,y); return *this;}
#endif
  sc_lv_base& operator=(const sc_lv_base& nv){sc_bv_ns::assign_(*this,nv); return *this;}
  sc_lv_base& operator=( const char* nv){assign_(nv); return *this;}
  sc_lv_base& operator=( const bool* nv){assign_(nv); return *this;}

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
  sc_lv_base& operator=(const sc_bool_vector& nv);
  sc_lv_base& operator=(const sc_logic_vector& nv);

    // common interface
    long get_bit(unsigned bit_number) const;
    void set_bit(unsigned bit_number, long value);
  unsigned long get_word(unsigned i) const {return data[i];}
  void set_word(unsigned i, unsigned long w) {data[i]=w;}
  unsigned long get_cword(unsigned i) const {return control[i];}
  void set_cword(unsigned i, unsigned long w) {control[i]=w;}
  // capacity functions
  int length()const { return N;}

  // member functions
  void set(unsigned long v); // fill with v
  void set(const sc_logic& v);
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
  sc_lv(const sc_bool_vector& v):sc_lv_base(v){verify_length();}
  sc_lv(const sc_logic_vector& v):sc_lv_base(v){verify_length();}
  explicit sc_lv(char c);
  sc_lv(const sc_unsigned& v);
  sc_lv(const sc_signed& v);
  sc_lv(const sc_uint_base& v);
  sc_lv(const sc_int_base& v);
  explicit sc_lv(const sc_logic& x);

#if !defined(__SUNPRO_CC)
  template<class Y> sc_lv(const sc_proxy<Y>& y)
    {init(y.back_cast().length()); assign_(y);}
  template<class T> sc_lv& operator=(const sc_proxy<T>& t)
    { sc_bv_ns::assign_(*this,t); return *this;}
#else
  // at least some constructors
  sc_lv(const sc_bv<W>& t){convert(t);verify_length();}
  sc_lv(const sc_lv_base& y):sc_lv_base(y){verify_length();}
  sc_lv(const sc_bv_base& t){convert(t);verify_length();}
#endif

    virtual sc_lv_base* clone();
  // Assignments may be made to a sc_lv from various types of
  // objects.
  sc_lv& operator=(const sc_lv& t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const char* t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}
  sc_lv& operator=( const bool* t)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(t));}

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
  sc_lv& operator=(const sc_bool_vector& v)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(v));}
  sc_lv& operator=(const sc_logic_vector& v)
   { return static_cast<sc_lv&>(sc_lv_base::operator=(v));}

  virtual void resize(unsigned long new_size);
};

// ----------------------------------------------------------------------------
// inline and template functions
// ----------------------------------------------------------------------------
template<int W>
sc_lv<W>::sc_lv(){init(W);}
template<int W>
void sc_lv<W>::resize(unsigned long new_size)
{REPORT_ERROR(2006,sc_string::to_string("sc_lv<%d>",W).c_str());}
template<int W>
sc_lv_base* sc_lv<W>::clone(){return new sc_lv<W>(*this);}
template<int W>
sc_lv<W>::sc_lv(long init_value):sc_lv_base(W,init_value){}
template<int W>
sc_lv<W>::sc_lv(bool init_value):sc_lv_base(W,(long)init_value){}
template<int W>
sc_lv<W>::sc_lv(const char* s):sc_lv_base(W){assign_(s);}
template<int W>
sc_lv<W>::sc_lv(int v):sc_lv_base(W){(*this)=v;}
template<int W>
sc_lv<W>::sc_lv(char c):sc_lv_base(W,sc_logic::char_to_logic[c]){}
template<int W>
sc_lv<W>::sc_lv(const sc_unsigned& v):sc_lv_base(W){ assign_(v);}
template<int W>
sc_lv<W>::sc_lv(const sc_signed& v):sc_lv_base(W){ assign_(v);}
template<int W>
sc_lv<W>::sc_lv(const sc_uint_base& v):sc_lv_base(W){ assign_(v);}
template<int W>
sc_lv<W>::sc_lv(const sc_int_base& v):sc_lv_base(W){ assign_(v);}
template<int W>
sc_lv<W>::sc_lv(const sc_logic& x):sc_lv_base(W){assign_(x.to_long());}

// inline functions
inline long sc_lv_base::get_bit(unsigned n) const
{
    return data[n/ul_size]>>n%ul_size & 1 |
           control[n/ul_size]>>n%ul_size<<1 & 2;
}

inline void sc_lv_base::set_bit(unsigned n, long value)
{
      data[n/ul_size] |= 1<< n%ul_size; // set 1
      control[n/ul_size] |= 1<< n%ul_size; // set 1
      data[n/ul_size] &= value<<n%ul_size | ~(1ul<<n%ul_size);
      control[n/ul_size] &= value>>1<<n%ul_size | ~(1ul<<n%ul_size);
}
//---------------------------------------------------------------------------
} // namespace sc_bv_ns

#if defined(__HP_aCC) && 0

template<class X>
inline const sc_lv_base sc_proxy<X>::operator,( const char* s) const
{
  // some compilers violate 13.3.3p1 (the best viable function)
  // and use non-const back_cast() function if used below
  // have to explicitly call const version
  sc_lv_base result = (back_const_cast(),sc_lv_base(s));
  return result;
}
// many compilers do not perform RVO, so we have to repeat the function
template<class X>
inline const sc_lv_base operator,( const char* s, const sc_proxy<X>& x)
{
  sc_lv_base result = (sc_lv_base(s),x.back_const_cast());
  return result;
}

#endif

#endif

