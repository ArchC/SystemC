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

  sc_bit_proxies.h -- Proxy classes for vector data types.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
//---------------------------------------------------------------------------

#ifndef sc_bit_proxiesH
#define sc_bit_proxiesH

#include "systemc/datatypes/bit/sc_proxy.h"

namespace sc_bv_ns
{

// the following classes are defined in this module
template <class X> class sc_range;
template <class X, class Y> class sc_concat;
template <class X, class Y> class sc_bitwise_and;
template <class X, class Y> class sc_bitwise_or;
template <class X, class Y> class sc_bitwise_xor;
template <class X> class sc_not;
template <class X> class sc_shift_left;
template <class X> class sc_shift_right;
template <class X> class sc_bit_proxy;


//---------------------------------------------------------------------------
// sc_bit_proxy
// a proxy class for all classes, including other proxies and sc_lv
//-------------------------------------------------------------------------

template <class X>
class sc_bit_proxy
: public sc_proxy<sc_bit_proxy<X> >
{
    friend class sc_bv_base;
    friend class sc_lv_base;
    X& rep;
    unsigned index;
    // the following constructors/assignments are prohibited
    sc_bit_proxy(char);
    sc_bit_proxy(int);
public:
    // constructors
    sc_bit_proxy(X& x, unsigned n):rep(x),index(n){}
    sc_bit_proxy(const sc_bit_proxy& r):rep(r.rep),index(r.index){}
    // assignments
    sc_bit_proxy& operator=(const sc_bit_proxy& r);
    sc_bit_proxy& operator=(long r);
    sc_bit_proxy& operator=(int r){return operator=((long)r);}
    sc_bit_proxy& operator=(bool r){return operator=((long)r);}
    sc_bit_proxy& operator=(const sc_bit& r){return operator=((bool)r);}
    sc_bit_proxy& operator=(const sc_logic& r){return (*this)=r.to_long();}
    // conversions
    long to_long() const {return const_cast<const X&>(rep)[index];}
    operator const sc_logic()const
      {return sc_logic(const_cast<const X&>(rep)[index]);}

    // logical
    sc_bit_proxy& operator &= ( const sc_bit_proxy& s );
    sc_bit_proxy& operator &= ( bool r );
    sc_bit_proxy& operator &= ( const sc_logic& r );
    sc_bit_proxy& operator |= ( const sc_bit_proxy& s );
    sc_bit_proxy& operator |= ( bool r );
    sc_bit_proxy& operator |= ( const sc_logic& r );
    sc_bit_proxy& operator ^= ( const sc_bit_proxy& s );
    sc_bit_proxy& operator ^= ( bool r );
    sc_bit_proxy& operator ^= ( const sc_logic& r );
    long operator & ( const sc_bit_proxy& s ) const;
    long operator & ( bool r ) const;
    long operator & ( const sc_logic& r ) const;
    long operator | ( const sc_bit_proxy& s ) const;
    long operator | ( bool r ) const;
    long operator | ( const sc_logic& r ) const;
    long operator ^ ( const sc_bit_proxy& s ) const;
    long operator ^ ( bool r ) const;
    long operator ^ ( const sc_logic& r ) const;
    long operator ~ ()const;

    // common interface
    long get_bit(unsigned bit_number) const;
    void set_bit(unsigned bit_number, long value);
    unsigned long get_word(unsigned i) const;
    void set_word(unsigned i, unsigned long w);
    unsigned long get_cword(unsigned i) const;
    void set_cword(unsigned i, unsigned long w);
    int length()const;
};


//-------------------------------------------------------------------------
// proxies for bool operands
//-------------------------------------------------------------------------

template <class X>
inline
long
operator & ( bool r, const sc_bit_proxy<X>& s )
{
    return ( s & r );
}

template <class X>
inline
long
operator | ( bool r, const sc_bit_proxy<X>& s )
{
    return ( s | r );
}

template <class X>
inline
long
operator ^ ( bool r, const sc_bit_proxy<X>& s )
{
    return ( s ^ r );
}


//---------------------------------------------------------------------------
//  shift proxies (r-value only)
//---------------------------------------------------------------------------

template <class X>
class sc_shift_left
: public sc_proxy<sc_shift_left<X> >
{
  const X& b;
  int n;
  public:
  sc_shift_left(const X& b, int n): b(b),n(n){};
  sc_shift_left(const sc_shift_left& bn):b(bn.b),n(bn.n){};
  // bit selection
  long get_bit(unsigned i) const;
  // member functions
  int length()const {return b.length();}
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};

template <class X>
class sc_shift_right
: public sc_proxy<sc_shift_right<X> >
{
  const X& b;
  int n;
  public:
  sc_shift_right(const X& b, int n): b(b),n(n){};
  sc_shift_right(const sc_shift_right& bn):b(bn.b),n(bn.n){};
  // bit selection
  long get_bit(unsigned i) const;
  // member functions
  int length()const {return b.length();}
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_range<X>
//
//  Range proxy.
// ----------------------------------------------------------------------------

template <class X>
class sc_range
: public sc_proxy<sc_range<X> >
{
public:

    // constructors
    sc_range( X& bv, unsigned hi_bit, unsigned lo_bit )
	: bv( bv ), hi_bit( hi_bit ), lo_bit( lo_bit )
	{
	    if( (int) hi_bit >= bv.length() ||
		(int) lo_bit >= bv.length() ) {
		REPORT_ERROR( 1000, "" );
	    }
	}
    sc_range( const sc_range<X>& bn )
	: bv( bn.bv ), hi_bit( bn.hi_bit ), lo_bit( bn.lo_bit )
	{}

    virtual sc_range<X>* clone()
	{ return new sc_range<X>( *this ); }

    // assignment operators
    template <class T>
    sc_range<X>& operator = ( const sc_proxy<T>& b )
        { this->assign_( b ); return *this; }
    sc_range<X>& operator = ( const sc_range<X>& b );
    sc_range<X>& operator = ( const char* b );
    sc_range<X>& operator = ( const bool* b );

    long get_bit( unsigned i ) const;
    inline void set_bit( unsigned n, long value );

    int length() const
	{
	    return ( hi_bit > lo_bit ? int( hi_bit - lo_bit )
		                     : int( lo_bit - hi_bit ) ) + 1;
	}

    bool reversed() const
	{ return lo_bit > hi_bit; }

    unsigned long get_word( unsigned i )const;
    void set_word( unsigned i, unsigned long w );

    unsigned long get_cword( unsigned i ) const;
    void set_cword( unsigned i, unsigned long w );

    void clean_tail()
	{ bv.clean_tail(); }

private:

    mutable X& bv;
    unsigned hi_bit,lo_bit;
};


//---------------------------------------------------------------------------
// bitwise proxies
//---------------------------------------------------------------------------
// sc_bitwise_and
//---------------------------------------------------------------------------

template <class X, class Y>
class sc_bitwise_and
: public sc_proxy<sc_bitwise_and<X,Y> >
{
  const X& b1;
  const Y& b2;
  public:
  sc_bitwise_and(const X& b1, const Y& b2):b1(b1),b2(b2)
    {if(b1.length()!=b2.length()) REPORT_ERROR(1002,"");}
  sc_bitwise_and(const sc_bitwise_and& b):b1(b.b1),b2(b.b2){}
  // no assignments available
  // bit selection
  long get_bit(unsigned i) const;
  // member functions
  int length()const { return b1.length();}
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const
    {return b1.get_word(i)&b2.get_word(i) | get_cword(i);}
  unsigned long get_cword(unsigned i) const;
};


//---------------------------------------------------------------------------
// sc_bitwise_or
//---------------------------------------------------------------------------

template <class X, class Y>
class sc_bitwise_or
: public sc_proxy<sc_bitwise_or<X,Y> >
{
  const X& b1;
  const Y& b2;
  public:
  sc_bitwise_or(const X& b1, const Y& b2):b1(b1),b2(b2)
    {if(b1.length()!=b2.length()) REPORT_ERROR(1002,"");}
  sc_bitwise_or(const sc_bitwise_or& b):b1(b.b1),b2(b.b2){}
  // no assignments available
  // bit selection
  long get_bit(unsigned n) const {return b1.get_bit(n)||b2.get_bit(n);}
  // member functions
  int length()const { return b1.length();}
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};


//---------------------------------------------------------------------------
// sc_bitwise_xor
//---------------------------------------------------------------------------

template <class X, class Y>
class sc_bitwise_xor
: public sc_proxy<sc_bitwise_xor<X,Y> >
{
  const X& b1;
  const Y& b2;
  public:
  sc_bitwise_xor(const X& b1, const Y& b2):b1(b1),b2(b2)
    {if(b1.length()!=b2.length()) REPORT_ERROR(1002,"");}
  sc_bitwise_xor(const sc_bitwise_xor& b):b1(b.b1),b2(b.b2){}
  // no assignments available
  // bit selection
  long get_bit(unsigned n) const {return b1.get_bit(n)^b2.get_bit(n);}
  // member functions
  int length()const { return b1.length();}
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const;
  unsigned long get_cword(unsigned i) const;
};


//---------------------------------------------------------------------------
// sc_not
//---------------------------------------------------------------------------
template <class X>
class sc_not
: public sc_proxy<sc_not<X> >
{
  const X& b;
  public:
  sc_not(const X& b):b(b){};
  sc_not(const sc_not<X>& bn):b(bn.b){};
  // no assignments available
  long get_bit(unsigned n) const;
  // member functions
  int length()const{return b.length();};
  // get the whole word of the underlying type for fast operations
  // no range check
  unsigned long get_word(unsigned i)const {return ~b.get_word(i) | get_cword(i);}
  unsigned long get_cword(unsigned i) const  {return b.get_cword(i); }
};


// ----------------------------------------------------------------------------
//  CLASS : sc_concat<X,Y>
//
//  Concatenation proxy.
// ----------------------------------------------------------------------------

template <class X, class Y>
class sc_concat
: public sc_proxy<sc_concat<X,Y> >
{
public:

    // constructors
    sc_concat( X&, const char* );
    // the ordering of words is  (bv1[N1-1]...bv1[0] bv2[N2-1]...bv2[0])
    sc_concat( X& bv1, Y& bv2, int delete_ = 0 )
	: bv1( bv1 ), bv2( bv2 ), m_delete( delete_ )
	{}
    sc_concat( const sc_concat<X,Y>& bn )
	: bv1( bn.bv1 ), bv2( bn.bv2 ), m_delete( bn.m_delete )
	{ bn.m_delete = 0; }

    // destructor
    virtual ~sc_concat();

    virtual sc_concat<X,Y>* clone()
	{ return new sc_concat<X,Y>( *this ); }

    // assignment operators
    template<class T>
    sc_concat<X,Y>& operator = ( const sc_proxy<T>& b )
        { return this->assign_( b ); }
    sc_concat<X,Y>& operator = ( const sc_concat<X,Y>& b )
	{ return this->assign_( b ); }
    sc_concat<X,Y>& operator = ( const char* b )
	{ return this->assign_( b ); }
    sc_concat<X,Y>& operator = ( const bool* b )
	{ return this->assign_( b ); }

    // bit selection
    long get_bit( unsigned n ) const;
    void set_bit( unsigned bit_number, long value );

    int length() const
	{ return bv1.length() + bv2.length(); }

    unsigned long get_word( unsigned i ) const;
    void set_word( unsigned i, unsigned long w );

    unsigned long get_cword( unsigned i ) const;
    void set_cword( unsigned i, unsigned long w );

    void clean_tail()
	{ bv1.clean_tail(); bv2.clean_tail(); }

private:

    mutable X& bv1; //hi
    mutable Y& bv2; //lo
    mutable int m_delete;
};

// ------- end of interface ---------------------------------------------------


// implementation

//---------------------------------------------------------------------------
// sc_bit_proxy
//-------------------------------------------------------------------------

template <class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator = ( const sc_bit_proxy<X>& r )
{ // set the value
  rep.set_bit(index,r.to_long());
  return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator=(long r)
{
  rep.set_bit(index,r);
  return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator &=(const sc_bit_proxy<X>& s)
{
  rep.set_bit(index, sc_logic::and_table[s.to_long()][to_long()]);
  return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator&=(bool r)
{
  rep.set_bit(index,sc_logic::and_table[r][to_long()]);
  return *this;
}

template <class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator &= ( const sc_logic& r )
{
    rep.set_bit( index, sc_logic::and_table[r.to_long()][to_long()] );
    return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator |=(const sc_bit_proxy<X>& s)
{
  rep.set_bit(index, sc_logic::or_table[s.to_long()][to_long()]);
  return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator|=(bool r)
{
  rep.set_bit(index,sc_logic::or_table[r][to_long()]);
  return *this;
}

template <class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator |= ( const sc_logic& r )
{
    rep.set_bit( index, sc_logic::or_table[r.to_long()][to_long()] );
    return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator ^=(const sc_bit_proxy<X>& s)
{
  rep.set_bit(index, sc_logic::xor_table[s.to_long()][to_long()]);
  return *this;
}

template<class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator^=(bool r)
{
  rep.set_bit(index,sc_logic::xor_table[r][to_long()]);
  return *this;
}

template <class X>
inline
sc_bit_proxy<X>&
sc_bit_proxy<X>::operator ^= ( const sc_logic& r )
{
    rep.set_bit( index, sc_logic::xor_table[r.to_long()][to_long()] );
    return *this;
}

template<class X>
inline
long
sc_bit_proxy<X>::operator &(const sc_bit_proxy<X>& s)const
{
  return sc_logic::and_table[s.to_long()][to_long()];
}

template<class X>
inline
long
sc_bit_proxy<X>::operator &(bool r) const
{
  return sc_logic::and_table[rep.get_bit(index)][r];
}

template <class X>
inline
long
sc_bit_proxy<X>::operator & ( const sc_logic& r ) const
{
    return sc_logic::and_table[rep.get_bit( index )][r.to_long()];
}

template<class X>
inline
long
sc_bit_proxy<X>::operator |(const sc_bit_proxy<X>& s)const
{
  return sc_logic::or_table[s.to_long()][to_long()];
}

template<class X>
inline
long
sc_bit_proxy<X>::operator |(bool r) const
{
  return sc_logic::or_table[rep.get_bit(index)][r];
}

template <class X>
inline
long
sc_bit_proxy<X>::operator | ( const sc_logic& r ) const
{
    return sc_logic::or_table[rep.get_bit( index )][r.to_long()];
}

template<class X>
inline
long
sc_bit_proxy<X>::operator ^(const sc_bit_proxy<X>& s)const
{
  return sc_logic::xor_table[s.to_long()][to_long()];
}

template<class X>
inline
long
sc_bit_proxy<X>::operator ^(bool r) const
{
  return sc_logic::xor_table[rep.get_bit(index)][r];
}

template <class X>
inline
long
sc_bit_proxy<X>::operator ^ ( const sc_logic& r ) const
{
    return sc_logic::xor_table[rep.get_bit( index )][r.to_long()];
}

template<class X>
inline
long
sc_bit_proxy<X>::operator ~()const
{
  return sc_logic::not_table[to_long()];
}


// common interface

template <class X>
inline
long
sc_bit_proxy<X>::get_bit( unsigned bit_number ) const
{
  if(bit_number==0)
    return rep.get_bit(index);
  else
    REPORT_ERROR(1000,"");
}

template <class X>
inline
void
sc_bit_proxy<X>::set_bit( unsigned bit_number, long value )
{
  if(bit_number==0)
    rep.set_bit(index,value);
  else
    REPORT_ERROR(1000,"");
}

template <class X>
inline
unsigned long
sc_bit_proxy<X>::get_word( unsigned i ) const
{
  return get_bit(i)&1;
}

template <class X>
inline
void
sc_bit_proxy<X>::set_word( unsigned i, unsigned long w )
{
  if(i==0)
    rep.set_word(index,w);
  else
    REPORT_ERROR(1000,"");
}

template <class X>
inline
unsigned long
sc_bit_proxy<X>::get_cword( unsigned i ) const
{
  if(i==0)
    return get_bit(i)&2;
  else
    REPORT_ERROR(1000,"");
}

template <class X>
inline
void
sc_bit_proxy<X>::set_cword( unsigned i, unsigned long w )
{
  if(i==0)
    rep.set_cword(index,w);
  else
    REPORT_ERROR(1000,"");
}

template <class X>
inline
int
sc_bit_proxy<X>::length() const
{
  return 1;
}


// ----------------------------------------------------------------------------
// sc_shift_right
// ----------------------------------------------------------------------------

template <class X>
inline
long
sc_shift_right<X>::get_bit( unsigned i ) const
{
  int ind = i+n;
  if(ind < b.length() && ind>=0)
    return b.get_bit(ind);
  else
    return 0;
}

template <class X>
inline
unsigned long
sc_shift_right<X>::get_word( unsigned i ) const
{
  unsigned lo = b.get_word(i)>>n; // left end filled with 0
  unsigned hi = i<b.length()-1?b.get_word(i+1)<<(ul_size-n) : 0;
  return lo | hi; // glue them together
}

template <class X>
inline
unsigned long
sc_shift_right<X>::get_cword( unsigned i ) const
{
  unsigned lo = b.get_cword(i)>>n; // left end filled with 0
  unsigned hi = i<b.length()-1?b.get_cword(i+1)<<(ul_size-n) : 0;
  return lo | hi; // glue them together
}


// ----------------------------------------------------------------------------
// sc_shift_left
// ----------------------------------------------------------------------------

template <class X>
inline
long
sc_shift_left<X>::get_bit( unsigned i ) const
{
  int ind = i-n;
  if(ind < b.length() && ind>=0)
    return b.get_bit(ind);
  else
    return 0;
}

template <class X>
inline
unsigned long
sc_shift_left<X>::get_word( unsigned i ) const
{
  unsigned hi = b.get_word(i)<<n; // right end filled with 0
  unsigned lo = i>0?b.get_word(i-1)>>(ul_size-n) : 0;
  return lo | hi; // glue them together
}

template <class X>
inline
unsigned long
sc_shift_left<X>::get_cword( unsigned i ) const
{
  unsigned hi = b.get_cword(i)<<n; // right end filled with 0
  unsigned lo = i>0?b.get_cword(i-1)>>(ul_size-n) : 0;
  return lo | hi; // glue them together
}


//---------------------------------------------------------------------------
// sc_bitwise_and
//---------------------------------------------------------------------------

template <class X, class Y>
inline
long
sc_bitwise_and<X,Y>::get_bit( unsigned i ) const
{
  const unsigned long b1_ = b1.get_bit(i);
  const unsigned long b2_ = b2.get_bit(i);
  const unsigned control = (b1_>>1 & b2_ | (b1_&b2_)>>1 | b1_ & b2_>>1)&1;
  return (b1_&b2_|control)&1 | control<<1;
}

template <class X, class Y>
inline
unsigned long
sc_bitwise_and<X,Y>::get_cword( unsigned i ) const
{
  const unsigned long c1 = b1.get_cword(i);
  const unsigned long c2 = b2.get_cword(i);
  if(c1==0 && c2==0) return 0;
  const unsigned long b1_ = b1.get_word(i);
  const unsigned long b2_ = b2.get_word(i);
  return c1&b2_ | c1&c2 | b1_&c2;
}


//---------------------------------------------------------------------------
// sc_bitwise_or
//---------------------------------------------------------------------------

template <class X, class Y>
inline
unsigned long
sc_bitwise_or<X,Y>::get_word( unsigned i ) const
{
  return b1.get_word(i)|b2.get_word(i) | get_cword(i);
}

template <class X, class Y>
inline
unsigned long
sc_bitwise_or<X,Y>::get_cword( unsigned i ) const
{
  return b1.get_cword(i)&~b2.get_word(i) | b1.get_cword(i)&b2.get_cword(i) |
         ~b1.get_word(i)&b2.get_cword(i);
}


//---------------------------------------------------------------------------
// sc_bitwise_xor
//---------------------------------------------------------------------------

template <class X, class Y>
inline
unsigned long
sc_bitwise_xor<X,Y>::get_word( unsigned i ) const
{
  return b1.get_word(i)^b2.get_word(i) | get_cword(i);
}

template <class X, class Y>
inline
unsigned long
sc_bitwise_xor<X,Y>::get_cword( unsigned i ) const
{
  return b1.get_cword(i) | b2.get_cword(i);
}


//---------------------------------------------------------------------------
// sc_not
//---------------------------------------------------------------------------

template <class X>
inline
long
sc_not<X>::get_bit( unsigned n ) const
{
  long x=b.get_bit(n);
  return x<2 ? !x : sc_logic::Log_X /* X-value */;
}


// ----------------------------------------------------------------------------
// sc_concat
// ----------------------------------------------------------------------------

// destructor

template <class X, class Y>
inline
sc_concat<X,Y>::~sc_concat()
{
    if( m_delete == 0 ) {
	return;
    }
    if( m_delete & 1 ) {
	delete &bv1;
    }
    if( m_delete & 2 ) {
	delete &bv2;
    }
}


template <class X, class Y>
inline
void
sc_concat<X,Y>::set_bit( unsigned i, long w )
{
  const unsigned len = bv2.length();
  if(i<len)
    bv2.set_bit(i,w);
  else
    if(i<len+bv1.length())
      bv1.set_bit(i-len,w);
    else
      REPORT_ERROR(1000,"");
}

template <class X, class Y>
inline
unsigned long
sc_concat<X,Y>::get_word( unsigned i ) const
{
  const unsigned shift = bv2.length()%ul_size;
  const unsigned border = bv2.length()/ul_size;
  if(shift==0)
  {
    if(i<border)
      return bv2.get_word(i);
    else
      return bv1.get_word(i-border);
  }
  else
    if(i<border)
      return bv2.get_word(i);
    else
    {// stiches are necessary
      if(i==border) { //the last word of b1
        return bv2.get_word(i) & ~0ul>>(ul_size-shift) | bv1.get_word(0)<<shift;
      }
      else // from the second word
      if(i<(bv2.length()+bv1.length()-1)/ul_size+1)
        return bv1.get_word(i-border-1)>>(ul_size-shift) |
               bv1.get_word(i-border)<<shift;
      else
        REPORT_ERROR(1000,"");
    }
}

template <class X, class Y>
inline
void
sc_concat<X,Y>::set_word( unsigned i, unsigned long w )
{
  unsigned shift = bv2.length()%ul_size;
  unsigned border = bv2.length()/ul_size; // number of full words
  if(shift==0)
  {
    if(i<border)
      bv2.set_word(i,w);
    else
      bv1.set_word(i-border,w);
  }
  else
    if(i<border)
      bv2.set_word(i,w);
    else
    if(i<(bv1.length()+bv1.length()-1)/ul_size+1)
    {// stiches are necessary
      if(i==border) //the last word of b1
      {
        bv2.set_word(i,w & ~0ul>>(ul_size-shift));
        bv1.set_word(0,bv1.get_word(0)& ~0ul<<(ul_size-shift) | w>>shift );
      }
      else // from the second word
      {
        bv1.set_word(i-border-1,bv1.get_word(i-border-1)& ~0ul>>shift |
            w<<(ul_size-shift));
        bv1.set_word(i-border,bv1.get_word(i-border)&~0ul<<(ul_size-shift)|
            w>>shift);
      }
    }
    else
      REPORT_ERROR(1000,"");
}

template <class X, class Y>
inline
unsigned long
sc_concat<X,Y>::get_cword( unsigned i ) const
{
  const unsigned shift = bv2.length()%ul_size;
  const unsigned border = bv2.length()/ul_size;
  if(shift==0)
  {
    if(i<border)
      return bv2.get_cword(i);
    else
      return bv1.get_cword(i-border);
  }
  else
    if(i<border)
      return bv2.get_cword(i);
    else
    {// stiches are necessary
      if(i==border) { //the last word of b1
        return bv2.get_cword(i) & ~0ul>>(ul_size-shift) | bv1.get_cword(0)<<shift;
      }
      else // from the second word
      if(i<(bv2.length()+bv1.length()-1)/ul_size+1)
        return bv1.get_cword(i-border-1)>>(ul_size-shift) |
               bv1.get_cword(i-border)<<shift;
      else
        REPORT_ERROR(1000,"");
    }
}

template <class X, class Y>
inline
void
sc_concat<X,Y>::set_cword( unsigned i, unsigned long w )
{
  unsigned shift = bv2.length()%ul_size;
  unsigned border = bv2.length()/ul_size; // number of full words
  if(shift==0)
  {
    if(i<border)
      bv2.set_cword(i,w);
    else
      bv1.set_cword(i-border,w);
  }
  else
    if(i<border)
      bv2.set_cword(i,w);
    else
    if(i<(bv2.length()+bv1.length()-1)/ul_size+1)
    {// stiches are necessary
      if(i==border) //the last word of b1
      {
        bv2.set_cword(i,w & ~0ul>>(ul_size-shift));
        bv1.set_cword(0,bv1.get_cword(0)& ~0ul<<(ul_size-shift) | w>>shift );
      }
      else // from the second word
      {
        bv1.set_cword(i-border-1,bv1.get_cword(i-border-1)& ~0ul>>shift |
            w<<(ul_size-shift));
        bv1.set_cword(i-border,bv1.get_cword(i-border)&~0ul<<(ul_size-shift)|
            w>>shift);
      }
    }
    else
      REPORT_ERROR(1000,"");
}

template <class X, class Y>
inline
long
sc_concat<X,Y>::get_bit( unsigned n ) const
{
  const unsigned len = bv2.length();
  if(n<len)
    return bv2.get_bit(n);
  else
    return bv1.get_bit(n-len);
}


// ----------------------------------------------------------------------------
// sc_range
// ----------------------------------------------------------------------------

#if !defined(_MSC_VER) && 0

template <class X>
template <class T>
inline
sc_range<X>&
sc_range<X>::operator = ( const sc_proxy<T>& b )
{
  unsigned len = sc_min(length(),b.back_cast().length());
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, b.back_cast()[i]);
    else
      bv.set_bit(lo_bit-i, b.back_cast()[i]);
  return *this;
}

#endif

// sc_range<X>::operator = ( const sc_range<X>& ) in sc_bv.h

template <class X>
inline
sc_range<X>&
sc_range<X>::operator = ( const char* b )
{
  unsigned len = sc_min(length(),(int)strlen(b));
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, sc_logic::char_to_logic[b[len-i-1]]);
    else
      bv.set_bit(lo_bit-i, sc_logic::char_to_logic[b[len-i-1]]);
  return *this;
}

template <class X>
inline
sc_range<X>&
sc_range<X>::operator = ( const bool* b )
{
  unsigned len = length();
  for(unsigned i=0; i<len; i++)
    if(!reversed())
      bv.set_bit(lo_bit+i, b[i]);
    else
      bv.set_bit(lo_bit-i, b[i]);
  return *this;
}

template <class X>
inline
long
sc_range<X>::get_bit( unsigned n ) const
{
  if(reversed())
    return bv.get_bit(lo_bit-n);
  else
    return bv.get_bit(lo_bit+n);
}

template <class X>
inline
void
sc_range<X>::set_bit( unsigned n, long value )
{
  if(reversed())
    bv.set_bit(lo_bit-n, value);
  else
    bv.set_bit(lo_bit+n, value);
}

template <class X>
inline
unsigned long
sc_range<X>::get_word( unsigned i ) const
{
  int n1=0;
  int n2=0;
  unsigned long result = 0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      result |= (const_cast<const X&>(bv)[n]&1ul)<<k++;
  }
  else
  {
    n1 = sc_min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      result |= (const_cast<const X&>(bv)[n]&1ul)<<k++;
  }
  return result;
}

template <class X>
inline
void
sc_range<X>::set_word( unsigned i, unsigned long w )
{
  int n1=0;
  int n2=0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      bv.set_bit(n,(w>>k++)&1ul | const_cast<const X&>(bv)[n]&2ul);
  }
  else
  {
    n1 = sc_min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      bv.set_bit(n,(w>>k++)&1ul | const_cast<const X&>(bv)[n]&2ul);
  }
}

template <class X>
inline
unsigned
long sc_range<X>::get_cword( unsigned i ) const
{
  int n1=0;
  int n2=0;
  unsigned long result = 0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      result |= ((const_cast<const X&>(bv)[n]&2ul) >> 1)<<k++;
  }
  else
  {
    n1 = sc_min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      result |= ((const_cast<const X&>(bv)[n]&2ul) >> 1)<<k++;
  }
  return result;
}

template <class X>
inline
void
sc_range<X>::set_cword( unsigned i, unsigned long w )
{
  int n1=0;
  int n2=0;
  unsigned k=0;
  if(reversed())
  {
    n1 = sc_max((int)hi_bit,(int)(lo_bit - i*ul_size));
    n2 = sc_max((int)hi_bit-1,(int)(n1-ul_size));
    for(int n = n1; n>n2; n--)
      bv.set_bit(n,((w>>k++)&1ul)<<1 | const_cast<const X&>(bv)[n]&1ul);
  }
  else
  {
    n1 = sc_min(lo_bit+i*ul_size,hi_bit);
    n2 = sc_min(n1 + ul_size,hi_bit+1);
    for(int n = n1; n<n2; n++)
      bv.set_bit(n,((w>>k++)&1ul)<<1 | const_cast<const X&>(bv)[n]&1ul);
  }
}

} // end namespace sc_bv_ns
//---------------------------------------------------------------------------
#endif
