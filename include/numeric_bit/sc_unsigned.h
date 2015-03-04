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


/***************************************************************************

    sc_unsigned.h -- Arbitrary precision unsigned arithmetic.
 
    This file includes the definitions of sc_unsigned_bitref,
    sc_unsigned_subref, and sc_unsigned classes. The first two classes
    are proxy classes to reference one bit and a range of bits of a
    sc_unsigned number, respectively.

    An sc_signed number has the sign-magnitude representation
    internally. However, its interface guarantees a 2's-complement
    representation. The sign-magnitude representation is chosen
    because of its efficiency: The sc_signed and sc_unsigned types are
    optimized for arithmetic rather than bitwise operations. For
    arithmetic operations, the sign-magnitude representation performs
    better.

    It is also important to note that an sc_unsigned number with n
    bits is equivalent to an sc_signed non-negative number with n + 1
    bits.

    The implementations of sc_signed and sc_unsigned classes are
    almost identical: Most of the member and friend functions are
    defined in sc_nbcommon.cpp and sc_nbfriends.cpp so that they can
    be shared by both of these classes. These functions are chosed by
    defining a few macros before including them such as IF_SC_SIGNED
    and CLASS_TYPE. Our implementation choices are mostly dictated by
    performance considerations in that we tried to provide the most
    efficient sc_signed and sc_unsigned types without compromising
    their interface.

    For the behavior of operators, we have two semantics: the old and
    new. The most important difference between these two semantics is
    that the old semantics is closer to C/C++ semantics in that the
    result type of a binary operator on unsigned and signed arguments
    is unsigned; the new semantics, on the other hand, requires the
    result type be signed. The new semantics is required by the VSIA
    C/C++ data types standard. In our implementation, one can choose
    between these two semantics by defining or undefining
    NEW_SEMANTICS variable.
 
    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
  
  **************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_UNSIGNED_H
#define SC_UNSIGNED_H

#include "../sc_iostream.h"
#include "sc_nbdefs.h"
#include "sc_nbutils.h"
#include "sc_nbexterns.h"

template< class A > class sc_2d;
class sc_logic_vector;
class sc_bool_vector;
class sc_int_base;
class sc_uint_base;
class sc_unsigned;
class sc_signed;
class sc_signed_subref;
//#ifdef SC_INCLUDE_FX
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CLASS: sc_unsigned_bitref -- Proxy class to reference a single bit.
/////////////////////////////////////////////////////////////////////////////

class sc_unsigned_bitref {

  friend class sc_unsigned;

  friend istream& operator >> (istream& is, sc_unsigned_bitref&       u);
  friend ostream& operator << (ostream& os, const sc_unsigned_bitref& u);

public:

  sc_unsigned_bitref& operator = (const sc_unsigned_bitref& v);
  sc_unsigned_bitref& operator = (bool v);

  sc_unsigned_bitref& operator &= (bool v);
  sc_unsigned_bitref& operator |= (bool v);
  sc_unsigned_bitref& operator ^= (bool v);

  operator  bool () const;
  bool operator ~ () const;

private:

  sc_unsigned* snum;  // Signed number.
  int index;

  sc_unsigned_bitref(sc_unsigned* u, int i) { snum = u; index = i; }
  
};
// gcc 2.95.2 bug - bool comparison mapped into int comparison
// must provide explicit conversion, otherwise gcc is confused
//inline bool operator==(bool b,sc_unsigned_bitref& t) {return t==b;}

/////////////////////////////////////////////////////////////////////////////
// CLASS: sc_unsigned_subref -- Proxy class to reference a range of bits.
/////////////////////////////////////////////////////////////////////////////

class sc_unsigned_subref {

  friend class sc_unsigned;

  friend istream& operator >> (istream& is, sc_unsigned_subref&       u);
  friend ostream& operator << (ostream& os, const sc_unsigned_subref& u);
  
public:

  sc_unsigned_subref& operator = (const sc_unsigned_subref& v);
  sc_unsigned_subref& operator = (const sc_unsigned&        v);
  sc_unsigned_subref& operator = (const sc_signed_subref&   v);
  sc_unsigned_subref& operator = (const sc_signed&          v);
  sc_unsigned_subref& operator = (const char*               v);
  sc_unsigned_subref& operator = (int64                     v);
  sc_unsigned_subref& operator = (uint64                    v);
  sc_unsigned_subref& operator = (long                      v);
  sc_unsigned_subref& operator = (unsigned long             v);
  sc_unsigned_subref& operator = (int                       v) 
    { return operator=((long) v); }
  sc_unsigned_subref& operator = (unsigned int              v) 
    { return operator=((unsigned long) v); }
  sc_unsigned_subref& operator = (double                    v);  
  sc_unsigned_subref& operator = (const sc_int_base&        v);
  sc_unsigned_subref& operator = (const sc_uint_base&       v);

  operator  sc_unsigned() const;

private:

  sc_unsigned* snum;  // Signed number.
  int left, right;

  sc_unsigned_subref(sc_unsigned* u, int l, int r)
    { snum = u; left = l; right = r; }
};

/////////////////////////////////////////////////////////////////////////////
// CLASS: sc_unsigned -- Arbitrary precision unsigned number.
/////////////////////////////////////////////////////////////////////////////

class sc_unsigned {

  friend class sc_unsigned_bitref;
  friend class sc_unsigned_subref;
  friend class sc_signed;
  friend class sc_signed_subref;
  friend class sc_2d<sc_unsigned>;

  friend istream& operator >> (istream& is, sc_unsigned&       u);
  friend ostream& operator << (ostream& os, const sc_unsigned& u);

  // Needed for types using sc_unsigned.
  typedef bool elemtype;

public:

  // Public constructor.
  explicit sc_unsigned(int nb);
  explicit sc_unsigned(const char *v);

  // Public copy constructors.
  sc_unsigned(const sc_unsigned& v);
  sc_unsigned(const sc_signed&   v);

  // Assignment operators.
  sc_unsigned& operator = (const sc_unsigned&        v);
  sc_unsigned& operator = (const sc_unsigned_subref& v);
  sc_unsigned& operator = (const sc_signed&          v);
  sc_unsigned& operator = (const sc_signed_subref&   v);
  sc_unsigned& operator = (const char*               v);
  sc_unsigned& operator = (int64                     v);
  sc_unsigned& operator = (uint64                    v);
  sc_unsigned& operator = (long                      v);
  sc_unsigned& operator = (unsigned long             v);
  sc_unsigned& operator = (int                       v) 
    { return operator=((long) v); }
  sc_unsigned& operator = (unsigned int              v) 
    { return operator=((unsigned long) v); }
  sc_unsigned& operator = (double                    v);
  sc_unsigned& operator = (const sc_int_base&        v);
  sc_unsigned& operator = (const sc_uint_base&       v);

//#ifdef SC_INCLUDE_FX
  // Interfacing with fixed point types.
  sc_unsigned& operator = (const sc_fxval&           v);
  sc_unsigned& operator = (const sc_fxval_fast&      v);
  sc_unsigned& operator = (const sc_fxnum&           v);
  sc_unsigned& operator = (const sc_fxnum_fast&      v);
//#endif

  // Destructor.
  ~sc_unsigned()
    {
#ifndef SC_MAX_NBITS
      delete [] digit;
#endif
    }

#ifdef SC_LOGIC_VECTOR_H
  // Interfacing with sc_logic_ and sc_bool_ vectors.
  sc_unsigned(const sc_logic_vector& v);
  sc_unsigned(const sc_bool_vector&  v);
  sc_unsigned& operator = (const sc_logic_vector& v);
  sc_unsigned& operator = (const sc_bool_vector&  v);
#endif

  // Increment operators.
  sc_unsigned& operator ++ ();
  const sc_unsigned operator ++ (int);

  // Decrement operators.
  sc_unsigned& operator -- ();
  const sc_unsigned operator -- (int);

  // Bitref operators. Help access the ith bit.
  sc_unsigned_bitref operator [] (int i)
    { return sc_unsigned_bitref(this, i); }

  const bool operator [] (int i) const
    { return test(i); }

  // Subref operators. Help access the range of bits from the ith to
  // jth. These indices have arbitrary precedence with respect to each
  // other, i.e., we can have i <= j or i > j. Note the equivalence
  // between range(i, j) and operator (i, j). Also note that
  // operator (i, i) returns an unsigned number that corresponds to the
  // bit operator [i], so these two forms are not the same.
  sc_unsigned_subref range(int i, int j)
    { return sc_unsigned_subref(this, i, j); }

  const sc_unsigned range(int i, int j) const
    { return sc_unsigned(this, i, j); }

  sc_unsigned_subref operator () (int i, int j) 
    { return sc_unsigned_subref(this, i, j); }

  const sc_unsigned operator () (int i, int j) const
    { return sc_unsigned(this, i, j); }

  // Conversion functions: to convert to (mostly) built-in types in
  // that to_X converts to a number of type X.
  sc_string     to_string(sc_numrep base = SC_DEC, 
                          bool formatted = false) const;
  sc_string     to_string(int base, bool formatted = false) const
    { return to_string(sc_numrep(base), formatted); }
  
  int64         to_int64() const;
  uint64        to_uint64() const;
  long          to_long() const;
  unsigned long to_ulong() const;
  unsigned long to_unsigned_long() const { return to_ulong(); }  // For VSI.
  int           to_int() const;
  int           to_signed() const { return to_int(); }
  unsigned int  to_uint() const;
  unsigned int  to_unsigned() const { return to_uint(); }
  unsigned int  to_unsigned_int() const { return to_uint(); }    // For VSI.
  double        to_double() const;

  // Print functions. dump prints the internals of the class.
  void print() const { print(cout); }
  void print(ostream &os) const;
  void dump() const { dump(cout); }
  void dump(ostream &os) const;

  // Functions to find various properties.    
  int  length() const { return nbits - 1; }  // Bit width.
  bool iszero() const;                       // Is the number zero?
  bool sign() const { return 0; }            // Sign.

  // Functions to access individual bits.  
  bool test(int i) const;      // Is the ith bit 0 or 1?
  void set(int i);             // Set the ith bit to 1.
  void clear(int i);           // Set the ith bit to 0.
  void set(int i, bool v)      // Set the ith bit to v.
    { if (v) set(i); else clear(i);  }
  void invert(int i)           // Negate the ith bit.
    { if (test(i)) clear(i); else set(i);  }

  // Make the number equal to its mirror image.
  void reverse();

  // Get/set a packed bit representation of the number.
  void get_packed_rep(unsigned long *buf) const;
  void set_packed_rep(unsigned long *buf);

  /*
    The comparison of the old and new semantics are as follows:

    Let s = sc_signed, 
        u = sc_unsigned,
        un = { uint64, unsigned long, unsigned int },
        sn = { int64, long, int, char* }, and
        OP = { +, -, *, /, % }.

    Old semantics:                     New semantics:
      u OP u -> u                        u OP u -> u
      s OP u -> u                        s OP u -> s
      u OP s -> u                        u OP s -> s
      s OP s -> s                        s OP s -> s

      u OP un = un OP u -> u             u OP un = un OP u -> u
      u OP sn = sn OP u -> u             u OP sn = sn OP u -> s

      s OP un = un OP s -> s             s OP un = un OP s -> s
      s OP sn = sn OP s -> s             s OP sn = sn OP s -> s

    In the new semantics, the result is u if both operands are u; the
    result is s otherwise. The only exception is subtraction. The result
    of a subtraction is always s.

    The old semantics is like C/C++ semantics on integer types; the
    new semantics is due to the VSIA C/C++ data types standard.  
   */

#ifdef NEW_SEMANTICS

  // ARITHMETIC OPERATORS:

  // ADDition operators:
   
  friend   sc_signed operator + (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator + (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator + (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator + (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator + (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned int        v) 
    { return operator+(u, (unsigned long) v); }

  friend   sc_signed operator + (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator + (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator + (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator + (unsigned int        u, const sc_unsigned&  v)  
    { return operator+((unsigned long) u,  v); } 

  sc_unsigned& operator += (const sc_signed&    v); 
  sc_unsigned& operator += (const sc_unsigned&  v); 
  sc_unsigned& operator += (int64               v); 
  sc_unsigned& operator += (uint64              v); 
  sc_unsigned& operator += (long                v); 
  sc_unsigned& operator += (unsigned long       v); 
  sc_unsigned& operator += (int                 v) 
    { return operator+=((long) v); }
  sc_unsigned& operator += (unsigned int        v) 
    { return operator+=((unsigned long) v); }

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator + (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator + (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator + (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator += (const sc_int_base&  v);
  sc_unsigned& operator += (const sc_uint_base& v);

  // SUBtraction operators:
   
  friend   sc_signed operator - (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator - (const sc_signed&    u, const sc_unsigned&  v); 

  friend   sc_signed operator - (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator - (const sc_unsigned&  u, int64               v); 
  friend   sc_signed operator - (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator - (const sc_unsigned&  u, long                v); 
  friend   sc_signed operator - (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator - (const sc_unsigned&  u, int                 v); 
  friend   sc_signed operator - (const sc_unsigned&  u, unsigned int        v);

  friend   sc_signed operator - (int64               u, const sc_unsigned&  v); 
  friend   sc_signed operator - (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator - (long                u, const sc_unsigned&  v); 
  friend   sc_signed operator - (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator - (int                 u, const sc_unsigned&  v);
  friend   sc_signed operator - (unsigned int        u, const sc_unsigned&  v); 

  sc_unsigned& operator -= (const sc_signed&    v); 
  sc_unsigned& operator -= (const sc_unsigned&  v); 
  sc_unsigned& operator -= (int64               v); 
  sc_unsigned& operator -= (uint64              v); 
  sc_unsigned& operator -= (long                v); 
  sc_unsigned& operator -= (unsigned long       v); 
  sc_unsigned& operator -= (int                 v) 
    { return operator-=((long) v); }
  sc_unsigned& operator -= (unsigned int        v) 
    { return operator-=((unsigned long) v); }

  friend   sc_signed operator - (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator - (const sc_unsigned&  u, const sc_int_base&  v);
  friend   sc_signed operator - (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator - (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator -= (const sc_int_base&  v);
  sc_unsigned& operator -= (const sc_uint_base& v);

  // MULtiplication operators:
   
  friend   sc_signed operator * (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator * (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator * (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator * (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator * (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned int        v) 
    { return operator*(u, (unsigned long) v); }

  friend   sc_signed operator * (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator * (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator * (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator * (unsigned int        u, const sc_unsigned&  v)  
    { return operator*((unsigned long) u,  v); } 

  sc_unsigned& operator *= (const sc_signed&    v); 
  sc_unsigned& operator *= (const sc_unsigned&  v); 
  sc_unsigned& operator *= (int64               v); 
  sc_unsigned& operator *= (uint64              v); 
  sc_unsigned& operator *= (long                v); 
  sc_unsigned& operator *= (unsigned long       v); 
  sc_unsigned& operator *= (int                 v) 
    { return operator*=((long) v); }
  sc_unsigned& operator *= (unsigned int        v) 
    { return operator*=((unsigned long) v); }

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator * (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator * (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator * (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator *= (const sc_int_base&  v);
  sc_unsigned& operator *= (const sc_uint_base& v);

  // DIVision operators:
   
  friend   sc_signed operator / (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator / (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator / (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator / (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator / (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned int        v) 
    { return operator/(u, (unsigned long) v); }

  friend   sc_signed operator / (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator / (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator / (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator / (unsigned int        u, const sc_unsigned&  v)  
    { return operator/((unsigned long) u,  v); } 

  sc_unsigned& operator /= (const sc_signed&    v); 
  sc_unsigned& operator /= (const sc_unsigned&  v); 
  sc_unsigned& operator /= (int64               v); 
  sc_unsigned& operator /= (uint64              v); 
  sc_unsigned& operator /= (long                v); 
  sc_unsigned& operator /= (unsigned long       v); 
  sc_unsigned& operator /= (int                 v) 
    { return operator/=((long) v); }
  sc_unsigned& operator /= (unsigned int        v) 
    { return operator/=((unsigned long) v); }

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator / (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator / (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator / (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator /= (const sc_int_base&  v);
  sc_unsigned& operator /= (const sc_uint_base& v);

  // MODulo operators:
   
  friend   sc_signed operator % (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator % (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator % (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator % (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator % (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned int        v) 
    { return operator%(u, (unsigned long) v); }

  friend   sc_signed operator % (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator % (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator % (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator % (unsigned int        u, const sc_unsigned&  v)  
    { return operator%((unsigned long) u,  v); } 

  sc_unsigned& operator %= (const sc_signed&    v); 
  sc_unsigned& operator %= (const sc_unsigned&  v); 
  sc_unsigned& operator %= (int64               v); 
  sc_unsigned& operator %= (uint64              v); 
  sc_unsigned& operator %= (long                v); 
  sc_unsigned& operator %= (unsigned long       v); 
  sc_unsigned& operator %= (int                 v) 
    { return operator%=((long) v); }
  sc_unsigned& operator %= (unsigned int        v) 
    { return operator%=((unsigned long) v); }

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator % (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator % (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator % (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator %= (const sc_int_base&  v);
  sc_unsigned& operator %= (const sc_uint_base& v);

  // BITWISE OPERATORS:

  // Bitwise AND operators:
   
  friend   sc_signed operator & (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator & (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator & (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator & (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator & (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned int        v) 
    { return operator&(u, (unsigned long) v); }

  friend   sc_signed operator & (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator & (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator & (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator & (unsigned int        u, const sc_unsigned&  v)  
    { return operator&((unsigned long) u,  v); } 

  sc_unsigned& operator &= (const sc_signed&    v); 
  sc_unsigned& operator &= (const sc_unsigned&  v); 
  sc_unsigned& operator &= (int64               v); 
  sc_unsigned& operator &= (uint64              v); 
  sc_unsigned& operator &= (long                v); 
  sc_unsigned& operator &= (unsigned long       v); 
  sc_unsigned& operator &= (int                 v) 
    { return operator&=((long) v); }
  sc_unsigned& operator &= (unsigned int        v) 
    { return operator&=((unsigned long) v); }

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator & (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator & (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator & (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator &= (const sc_int_base&  v);
  sc_unsigned& operator &= (const sc_uint_base& v);

  // Bitwise OR operators:
   
  friend   sc_signed operator | (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator | (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator | (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator | (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator | (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned int        v) 
    { return operator|(u, (unsigned long) v); }

  friend   sc_signed operator | (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator | (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator | (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator | (unsigned int        u, const sc_unsigned&  v)  
    { return operator|((unsigned long) u,  v); } 

  sc_unsigned& operator |= (const sc_signed&    v); 
  sc_unsigned& operator |= (const sc_unsigned&  v); 
  sc_unsigned& operator |= (int64               v); 
  sc_unsigned& operator |= (uint64              v); 
  sc_unsigned& operator |= (long                v); 
  sc_unsigned& operator |= (unsigned long       v); 
  sc_unsigned& operator |= (int                 v) 
    { return operator|=((long) v); }
  sc_unsigned& operator |= (unsigned int        v) 
    { return operator|=((unsigned long) v); }

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator | (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator | (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator | (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator |= (const sc_int_base&  v);
  sc_unsigned& operator |= (const sc_uint_base& v);

  // Bitwise XOR operators:
   
  friend   sc_signed operator ^ (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator ^ (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator ^ (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned long       v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned int        v) 
    { return operator^(u, (unsigned long) v); }

  friend   sc_signed operator ^ (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator ^ (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (unsigned long       u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator ^ (unsigned int        u, const sc_unsigned&  v)  
    { return operator^((unsigned long) u,  v); } 

  sc_unsigned& operator ^= (const sc_signed&    v); 
  sc_unsigned& operator ^= (const sc_unsigned&  v); 
  sc_unsigned& operator ^= (int64               v); 
  sc_unsigned& operator ^= (uint64              v); 
  sc_unsigned& operator ^= (long                v); 
  sc_unsigned& operator ^= (unsigned long       v); 
  sc_unsigned& operator ^= (int                 v) 
    { return operator^=((long) v); }
  sc_unsigned& operator ^= (unsigned int        v) 
    { return operator^=((unsigned long) v); }

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator ^ (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (const sc_int_base&  u, const sc_unsigned&  v);
  sc_unsigned& operator ^= (const sc_int_base&  v);
  sc_unsigned& operator ^= (const sc_uint_base& v);

  // SHIFT OPERATORS:

  // LEFT SHIFT operators:
   
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator << (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int                 v) 
    { return operator<<(u, (long) v); }
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned int        v) 
    { return operator<<(u, (unsigned long) v); }

  sc_unsigned& operator <<= (const sc_signed&    v); 
  sc_unsigned& operator <<= (const sc_unsigned&  v); 
  sc_unsigned& operator <<= (int64               v); 
  sc_unsigned& operator <<= (uint64              v); 
  sc_unsigned& operator <<= (long                v); 
  sc_unsigned& operator <<= (unsigned long       v); 
  sc_unsigned& operator <<= (int                 v) 
    { return operator<<=((long) v); }
  sc_unsigned& operator <<= (unsigned int        v) 
    { return operator<<=((unsigned long) v); }

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_uint_base& v);
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned& operator <<= (const sc_int_base&  v);
  sc_unsigned& operator <<= (const sc_uint_base& v);

  // RIGHT SHIFT operators:
   
  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator >> (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int                 v) 
    { return operator>>(u, (long) v); }
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned int        v) 
    { return operator>>(u, (unsigned long) v); }

  sc_unsigned& operator >>= (const sc_signed&    v); 
  sc_unsigned& operator >>= (const sc_unsigned&  v); 
  sc_unsigned& operator >>= (int64               v); 
  sc_unsigned& operator >>= (uint64              v); 
  sc_unsigned& operator >>= (long                v); 
  sc_unsigned& operator >>= (unsigned long       v); 
  sc_unsigned& operator >>= (int                 v) 
    { return operator>>=((long) v); }
  sc_unsigned& operator >>= (unsigned int        v) 
    { return operator>>=((unsigned long) v); }

  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_uint_base& v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned& operator >>= (const sc_int_base&  v);
  sc_unsigned& operator >>= (const sc_uint_base& v);

  // Unary arithmetic operators
  friend sc_unsigned operator + (const sc_unsigned& u);
  friend   sc_signed operator - (const sc_unsigned& u);

#else  // OLD SEMANTICS

  // ARITHMETIC OPERATORS:

  // ADDition operators:
   
  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator + (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator + (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator + (const sc_unsigned&  u, int                 v) 
    { return operator+(u, (long) v); }
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned int        v) 
    { return operator+(u, (unsigned long) v); }

  friend sc_unsigned operator + (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator + (int                 u, const sc_unsigned&  v)  
    { return operator+((long) u,  v); } 
  friend sc_unsigned operator + (unsigned int        u, const sc_unsigned&  v)  
    { return operator+((unsigned long) u,  v); } 

  sc_unsigned& operator += (const sc_signed&    v); 
  sc_unsigned& operator += (const sc_unsigned&  v); 
  sc_unsigned& operator += (int64               v); 
  sc_unsigned& operator += (uint64              v); 
  sc_unsigned& operator += (long                v); 
  sc_unsigned& operator += (unsigned long       v); 
  sc_unsigned& operator += (int                 v) 
    { return operator+=((long) v); }
  sc_unsigned& operator += (unsigned int        v) 
    { return operator+=((unsigned long) v); }

  // SUBtraction operators:
   
  friend sc_unsigned operator - (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator - (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator - (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator - (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator - (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator - (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator - (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator - (const sc_unsigned&  u, int                 v) 
    { return operator-(u, (long) v); }
  friend sc_unsigned operator - (const sc_unsigned&  u, unsigned int        v) 
    { return operator-(u, (unsigned long) v); }

  friend sc_unsigned operator - (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator - (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator - (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator - (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator - (int                 u, const sc_unsigned&  v)  
    { return operator-((long) u,  v); } 
  friend sc_unsigned operator - (unsigned int        u, const sc_unsigned&  v)  
    { return operator-((unsigned long) u,  v); } 

  sc_unsigned& operator -= (const sc_signed&    v); 
  sc_unsigned& operator -= (const sc_unsigned&  v); 
  sc_unsigned& operator -= (int64               v); 
  sc_unsigned& operator -= (uint64              v); 
  sc_unsigned& operator -= (long                v); 
  sc_unsigned& operator -= (unsigned long       v); 
  sc_unsigned& operator -= (int                 v) 
    { return operator-=((long) v); }
  sc_unsigned& operator -= (unsigned int        v) 
    { return operator-=((unsigned long) v); }

  // MULtiplication operators:
   
  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator * (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator * (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator * (const sc_unsigned&  u, int                 v) 
    { return operator*(u, (long) v); }
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned int        v) 
    { return operator*(u, (unsigned long) v); }

  friend sc_unsigned operator * (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator * (int                 u, const sc_unsigned&  v)  
    { return operator*((long) u,  v); } 
  friend sc_unsigned operator * (unsigned int        u, const sc_unsigned&  v)  
    { return operator*((unsigned long) u,  v); } 

  sc_unsigned& operator *= (const sc_signed&    v); 
  sc_unsigned& operator *= (const sc_unsigned&  v); 
  sc_unsigned& operator *= (int64               v); 
  sc_unsigned& operator *= (uint64              v); 
  sc_unsigned& operator *= (long                v); 
  sc_unsigned& operator *= (unsigned long       v); 
  sc_unsigned& operator *= (int                 v) 
    { return operator*=((long) v); }
  sc_unsigned& operator *= (unsigned int        v) 
    { return operator*=((unsigned long) v); }

  // DIVision operators:
   
  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator / (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator / (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator / (const sc_unsigned&  u, int                 v) 
    { return operator/(u, (long) v); }
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned int        v) 
    { return operator/(u, (unsigned long) v); }

  friend sc_unsigned operator / (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator / (int                 u, const sc_unsigned&  v)  
    { return operator/((long) u,  v); } 
  friend sc_unsigned operator / (unsigned int        u, const sc_unsigned&  v)  
    { return operator/((unsigned long) u,  v); } 

  sc_unsigned& operator /= (const sc_signed&    v); 
  sc_unsigned& operator /= (const sc_unsigned&  v); 
  sc_unsigned& operator /= (int64               v); 
  sc_unsigned& operator /= (uint64              v); 
  sc_unsigned& operator /= (long                v); 
  sc_unsigned& operator /= (unsigned long       v); 
  sc_unsigned& operator /= (int                 v) 
    { return operator/=((long) v); }
  sc_unsigned& operator /= (unsigned int        v) 
    { return operator/=((unsigned long) v); }

  // MODulo operators:
   
  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator % (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator % (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator % (const sc_unsigned&  u, int                 v) 
    { return operator%(u, (long) v); }
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned int        v) 
    { return operator%(u, (unsigned long) v); }

  friend sc_unsigned operator % (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator % (int                 u, const sc_unsigned&  v)  
    { return operator%((long) u,  v); } 
  friend sc_unsigned operator % (unsigned int        u, const sc_unsigned&  v)  
    { return operator%((unsigned long) u,  v); } 

  sc_unsigned& operator %= (const sc_signed&    v); 
  sc_unsigned& operator %= (const sc_unsigned&  v); 
  sc_unsigned& operator %= (int64               v); 
  sc_unsigned& operator %= (uint64              v); 
  sc_unsigned& operator %= (long                v); 
  sc_unsigned& operator %= (unsigned long       v); 
  sc_unsigned& operator %= (int                 v) 
    { return operator%=((long) v); }
  sc_unsigned& operator %= (unsigned int        v) 
    { return operator%=((unsigned long) v); }

  // BITWISE OPERATORS:

  // Bitwise AND operators:
   
  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator & (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator & (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator & (const sc_unsigned&  u, int                 v) 
    { return operator&(u, (long) v); }
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned int        v) 
    { return operator&(u, (unsigned long) v); }

  friend sc_unsigned operator & (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator & (int                 u, const sc_unsigned&  v)  
    { return operator&((long) u,  v); } 
  friend sc_unsigned operator & (unsigned int        u, const sc_unsigned&  v)  
    { return operator&((unsigned long) u,  v); } 

  sc_unsigned& operator &= (const sc_signed&    v); 
  sc_unsigned& operator &= (const sc_unsigned&  v); 
  sc_unsigned& operator &= (int64               v); 
  sc_unsigned& operator &= (uint64              v); 
  sc_unsigned& operator &= (long                v); 
  sc_unsigned& operator &= (unsigned long       v); 
  sc_unsigned& operator &= (int                 v) 
    { return operator&=((long) v); }
  sc_unsigned& operator &= (unsigned int        v) 
    { return operator&=((unsigned long) v); }

  // Bitwise OR operators:
   
  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator | (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator | (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator | (const sc_unsigned&  u, int                 v) 
    { return operator|(u, (long) v); }
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned int        v) 
    { return operator|(u, (unsigned long) v); }

  friend sc_unsigned operator | (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator | (int                 u, const sc_unsigned&  v)  
    { return operator|((long) u,  v); } 
  friend sc_unsigned operator | (unsigned int        u, const sc_unsigned&  v)  
    { return operator|((unsigned long) u,  v); } 

  sc_unsigned& operator |= (const sc_signed&    v); 
  sc_unsigned& operator |= (const sc_unsigned&  v); 
  sc_unsigned& operator |= (int64               v); 
  sc_unsigned& operator |= (uint64              v); 
  sc_unsigned& operator |= (long                v); 
  sc_unsigned& operator |= (unsigned long       v); 
  sc_unsigned& operator |= (int                 v) 
    { return operator|=((long) v); }
  sc_unsigned& operator |= (unsigned int        v) 
    { return operator|=((unsigned long) v); }

  // Bitwise XOR operators:
   
  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_signed&    v); 
  friend sc_unsigned operator ^ (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator ^ (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator ^ (const sc_unsigned&  u, int                 v) 
    { return operator^(u, (long) v); }
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned int        v) 
    { return operator^(u, (unsigned long) v); }

  friend sc_unsigned operator ^ (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (uint64              u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (long                u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (unsigned long       u, const sc_unsigned&  v);
  friend sc_unsigned operator ^ (int                 u, const sc_unsigned&  v)  
    { return operator^((long) u,  v); } 
  friend sc_unsigned operator ^ (unsigned int        u, const sc_unsigned&  v)  
    { return operator^((unsigned long) u,  v); } 

  sc_unsigned& operator ^= (const sc_signed&    v); 
  sc_unsigned& operator ^= (const sc_unsigned&  v); 
  sc_unsigned& operator ^= (int64               v); 
  sc_unsigned& operator ^= (uint64              v); 
  sc_unsigned& operator ^= (long                v); 
  sc_unsigned& operator ^= (unsigned long       v); 
  sc_unsigned& operator ^= (int                 v) 
    { return operator^=((long) v); }
  sc_unsigned& operator ^= (unsigned int        v) 
    { return operator^=((unsigned long) v); }

  // SHIFT OPERATORS:

  // LEFT SHIFT operators:
   
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator << (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int                 v) 
    { return operator<<(u, (long) v); }
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned int        v) 
    { return operator<<(u, (unsigned long) v); }

  sc_unsigned& operator <<= (const sc_signed&    v); 
  sc_unsigned& operator <<= (const sc_unsigned&  v); 
  sc_unsigned& operator <<= (int64               v); 
  sc_unsigned& operator <<= (uint64              v); 
  sc_unsigned& operator <<= (long                v); 
  sc_unsigned& operator <<= (unsigned long       v); 
  sc_unsigned& operator <<= (int                 v) 
    { return operator<<=((long) v); }
  sc_unsigned& operator <<= (unsigned int        v) 
    { return operator<<=((unsigned long) v); }

  // RIGHT SHIFT operators:
   
  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator >> (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, long                v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned long       v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int                 v) 
    { return operator>>(u, (long) v); }
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned int        v) 
    { return operator>>(u, (unsigned long) v); }

  sc_unsigned& operator >>= (const sc_signed&    v); 
  sc_unsigned& operator >>= (const sc_unsigned&  v); 
  sc_unsigned& operator >>= (int64               v); 
  sc_unsigned& operator >>= (uint64              v); 
  sc_unsigned& operator >>= (long                v); 
  sc_unsigned& operator >>= (unsigned long       v); 
  sc_unsigned& operator >>= (int                 v) 
    { return operator>>=((long) v); }
  sc_unsigned& operator >>= (unsigned int        v) 
    { return operator>>=((unsigned long) v); }

  // Unary arithmetic operators
  friend sc_unsigned operator - (const sc_unsigned& u); 
  friend sc_unsigned operator + (const sc_unsigned& u);

#endif  // END OF OLD SEMANTICS

  // LOGICAL OPERATORS:

  // Logical EQUAL operators:
   
  friend bool operator == (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator == (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator == (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator == (const sc_unsigned&  u, int64               v); 
  friend bool operator == (const sc_unsigned&  u, uint64              v); 
  friend bool operator == (const sc_unsigned&  u, long                v); 
  friend bool operator == (const sc_unsigned&  u, unsigned long       v);
  friend bool operator == (const sc_unsigned&  u, int                 v) 
    { return operator==(u, (long) v); }
  friend bool operator == (const sc_unsigned&  u, unsigned int        v) 
    { return operator==(u, (unsigned long) v); }

  friend bool operator == (int64               u, const sc_unsigned&  v); 
  friend bool operator == (uint64              u, const sc_unsigned&  v); 
  friend bool operator == (long                u, const sc_unsigned&  v); 
  friend bool operator == (unsigned long       u, const sc_unsigned&  v);
  friend bool operator == (int                 u, const sc_unsigned&  v)  
    { return operator==((long) u,  v); } 
  friend bool operator == (unsigned int        u, const sc_unsigned&  v)  
    { return operator==((unsigned long) u,  v); } 

  friend bool operator == (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator == (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator == (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator == (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical NOT_EQUAL operators:
   
  friend bool operator != (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator != (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator != (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator != (const sc_unsigned&  u, int64               v); 
  friend bool operator != (const sc_unsigned&  u, uint64              v); 
  friend bool operator != (const sc_unsigned&  u, long                v); 
  friend bool operator != (const sc_unsigned&  u, unsigned long       v);
  friend bool operator != (const sc_unsigned&  u, int                 v) 
    { return operator!=(u, (long) v); }
  friend bool operator != (const sc_unsigned&  u, unsigned int        v) 
    { return operator!=(u, (unsigned long) v); }

  friend bool operator != (int64               u, const sc_unsigned&  v); 
  friend bool operator != (uint64              u, const sc_unsigned&  v); 
  friend bool operator != (long                u, const sc_unsigned&  v); 
  friend bool operator != (unsigned long       u, const sc_unsigned&  v);
  friend bool operator != (int                 u, const sc_unsigned&  v)  
    { return operator!=((long) u,  v); } 
  friend bool operator != (unsigned int        u, const sc_unsigned&  v)  
    { return operator!=((unsigned long) u,  v); } 

  friend bool operator != (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator != (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator != (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator != (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN operators:
   
  friend bool operator < (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator < (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator < (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator < (const sc_unsigned&  u, int64               v); 
  friend bool operator < (const sc_unsigned&  u, uint64              v); 
  friend bool operator < (const sc_unsigned&  u, long                v); 
  friend bool operator < (const sc_unsigned&  u, unsigned long       v);
  friend bool operator < (const sc_unsigned&  u, int                 v) 
    { return operator<(u, (long) v); }
  friend bool operator < (const sc_unsigned&  u, unsigned int        v) 
    { return operator<(u, (unsigned long) v); }

  friend bool operator < (int64               u, const sc_unsigned&  v); 
  friend bool operator < (uint64              u, const sc_unsigned&  v); 
  friend bool operator < (long                u, const sc_unsigned&  v); 
  friend bool operator < (unsigned long       u, const sc_unsigned&  v);
  friend bool operator < (int                 u, const sc_unsigned&  v)  
    { return operator<((long) u,  v); } 
  friend bool operator < (unsigned int        u, const sc_unsigned&  v)  
    { return operator<((unsigned long) u,  v); } 

  friend bool operator < (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator < (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator < (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator < (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN_AND_EQUAL operators:
   
  friend bool operator <= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator <= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator <= (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator <= (const sc_unsigned&  u, int64               v); 
  friend bool operator <= (const sc_unsigned&  u, uint64              v); 
  friend bool operator <= (const sc_unsigned&  u, long                v); 
  friend bool operator <= (const sc_unsigned&  u, unsigned long       v);
  friend bool operator <= (const sc_unsigned&  u, int                 v) 
    { return operator<=(u, (long) v); }
  friend bool operator <= (const sc_unsigned&  u, unsigned int        v) 
    { return operator<=(u, (unsigned long) v); }

  friend bool operator <= (int64               u, const sc_unsigned&  v); 
  friend bool operator <= (uint64              u, const sc_unsigned&  v); 
  friend bool operator <= (long                u, const sc_unsigned&  v); 
  friend bool operator <= (unsigned long       u, const sc_unsigned&  v);
  friend bool operator <= (int                 u, const sc_unsigned&  v)  
    { return operator<=((long) u,  v); } 
  friend bool operator <= (unsigned int        u, const sc_unsigned&  v)  
    { return operator<=((unsigned long) u,  v); } 

  friend bool operator <= (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator <= (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator <= (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator <= (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN operators:
   
  friend bool operator > (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator > (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator > (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator > (const sc_unsigned&  u, int64               v); 
  friend bool operator > (const sc_unsigned&  u, uint64              v); 
  friend bool operator > (const sc_unsigned&  u, long                v); 
  friend bool operator > (const sc_unsigned&  u, unsigned long       v);
  friend bool operator > (const sc_unsigned&  u, int                 v) 
    { return operator>(u, (long) v); }
  friend bool operator > (const sc_unsigned&  u, unsigned int        v) 
    { return operator>(u, (unsigned long) v); }

  friend bool operator > (int64               u, const sc_unsigned&  v); 
  friend bool operator > (uint64              u, const sc_unsigned&  v); 
  friend bool operator > (long                u, const sc_unsigned&  v); 
  friend bool operator > (unsigned long       u, const sc_unsigned&  v);
  friend bool operator > (int                 u, const sc_unsigned&  v)  
    { return operator>((long) u,  v); } 
  friend bool operator > (unsigned int        u, const sc_unsigned&  v)  
    { return operator>((unsigned long) u,  v); } 

  friend bool operator > (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator > (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator > (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator > (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN_AND_EQUAL operators:
   
  friend bool operator >= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator >= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator >= (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator >= (const sc_unsigned&  u, int64               v); 
  friend bool operator >= (const sc_unsigned&  u, uint64              v); 
  friend bool operator >= (const sc_unsigned&  u, long                v); 
  friend bool operator >= (const sc_unsigned&  u, unsigned long       v);
  friend bool operator >= (const sc_unsigned&  u, int                 v) 
    { return operator>=(u, (long) v); }
  friend bool operator >= (const sc_unsigned&  u, unsigned int        v) 
    { return operator>=(u, (unsigned long) v); }

  friend bool operator >= (int64               u, const sc_unsigned&  v); 
  friend bool operator >= (uint64              u, const sc_unsigned&  v); 
  friend bool operator >= (long                u, const sc_unsigned&  v); 
  friend bool operator >= (unsigned long       u, const sc_unsigned&  v);
  friend bool operator >= (int                 u, const sc_unsigned&  v)  
    { return operator>=((long) u,  v); } 
  friend bool operator >= (unsigned int        u, const sc_unsigned&  v)  
    { return operator>=((unsigned long) u,  v); } 

  friend bool operator >= (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator >= (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator >= (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator >= (const sc_int_base&  u, const sc_unsigned&  v);

  // Bitwise NOT operator (unary).
  friend sc_unsigned operator ~ (const sc_unsigned& u); 

  // Helper functions.
  friend int compare_unsigned(small_type us, 
                              int unb,
                              int und, 
                              const unsigned long *ud, 
                              small_type vs, 
                              int vnb,
                              int vnd,
                              const unsigned long *vd,
                              small_type if_u_signed = 0,
                              small_type if_v_signed = 0);
  
  friend sc_unsigned add_unsigned_friend(small_type us, 
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned sub_unsigned_friend(small_type us, 
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs, 
                                         int vnb,
                                         int vnd, 
                                         const unsigned long *vd);
  
  friend sc_unsigned mul_unsigned_friend(small_type s,
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned div_unsigned_friend(small_type s,
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned mod_unsigned_friend(small_type us,
                                         int unb,
                                         int und, 
                                         const unsigned long *ud, 
                                         int vnb,
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned and_unsigned_friend(small_type us, 
                                         int unb, 
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs,
                                         int vnb, 
                                         int vnd,
                                         const unsigned long *vd);
  
  friend sc_unsigned or_unsigned_friend(small_type us, 
                                        int unb, 
                                        int und, 
                                        const unsigned long *ud, 
                                        small_type vs,
                                        int vnb, 
                                        int vnd,
                                        const unsigned long *vd);
  
  friend sc_unsigned xor_unsigned_friend(small_type us, 
                                         int unb, 
                                         int und, 
                                         const unsigned long *ud, 
                                         small_type vs,
                                         int vnb, 
                                         int vnd,
                                         const unsigned long *vd);
  
private:

  small_type  sgn;         // Shortened as s.
  int nbits;       // Shortened as nb.
  int ndigits;     // Shortened as nd.

#ifdef SC_MAX_NBITS
  unsigned long digit[DIV_CEIL(SC_MAX_NBITS)];   // Shortened as d.
#else
  unsigned long *digit;                       // Shortened as d.
#endif

  // Private constructors:

  // Create zero.
  sc_unsigned();

  // Create a copy of v with sign s.
  sc_unsigned(const sc_unsigned& v, small_type s);
  sc_unsigned(const sc_signed&   v, small_type s);

  // Create an unsigned number with the given attributes.
  sc_unsigned(small_type s, int nb, int nd, 
              unsigned long *d, bool alloc = true);

  // Create an unsigned number using the bits u[l..r].
  sc_unsigned(const sc_unsigned* u, int l, int r);

  // Private member functions. The called functions are inline functions.

  small_type default_sign() const
    { return SC_POS; }

  int num_bits(int nb) const { return nb + 1; }

  bool check_if_outside(int bit_num) const;

  void copy_digits(int nb, int nd, const unsigned long *d)
    { copy_digits_unsigned(sgn, nbits, ndigits, digit, nb, nd, d); }
  
  void makezero()
    { sgn = make_zero(ndigits, digit); }

  // Conversion functions between 2's complement (2C) and
  // sign-magnitude (SM):
  void convert_2C_to_SM()
    { sgn = convert_unsigned_2C_to_SM(nbits, ndigits, digit); }

  void convert_SM_to_2C_to_SM()
    { sgn = convert_unsigned_SM_to_2C_to_SM(sgn, nbits, ndigits, digit); }

  void convert_SM_to_2C()
    { convert_unsigned_SM_to_2C(sgn, ndigits, digit); }

#if 0
  void convert_SM_to_2C_trimmed()
    { convert_unsigned_SM_to_2C_trimmed(sgn, nbits, ndigits, digit); }
#endif

  // Needed for types using sc_unsigned.
  typedef long          asgn_type1;
  typedef unsigned long asgn_type2;
  typedef int           asgn_type3;
  typedef unsigned int  asgn_type4;
  typedef const char*   asgn_type5;

};

#endif
