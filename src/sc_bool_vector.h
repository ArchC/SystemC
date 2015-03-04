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

    sc_bool_vector.h -- vector of binary values

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_BOOL_VECTOR_H
#define SC_BOOL_VECTOR_H

#include "sc_array.h"
#include "sc_signal_array.h"
#include "numeric_bit/sc_nbdefs.h"

//
//  Forward class declarations
//
class sc_bool_vector;
class sc_signal_bool_vector;
class sc_unsigned;
class sc_signed;
class sc_logic_vector;
template <class A, class T> class sc_channel_array_gnu;

namespace sc_bv_ns
{
  class sc_bv_base;
}

/* Proxy classes for holding result of operations */
struct sc_bool_vector_andop1 {
    const sc_bool_vector* a;
    const sc_bool_vector* b;
    sc_bool_vector_andop1(const sc_bool_vector* x, const sc_bool_vector* y)
        : a(x), b(y) { }
};

struct sc_bool_vector_andop2 {
    const sc_bool_vector* a;
    const char* b;
    sc_bool_vector_andop2(const sc_bool_vector* x, const char* y)
        : a(x), b(y) { }
};

struct sc_bool_vector_orop1 {
    const sc_bool_vector* a;
    const sc_bool_vector* b;
    sc_bool_vector_orop1(const sc_bool_vector* x, const sc_bool_vector* y)
        : a(x), b(y) { }
};

struct sc_bool_vector_orop2 {
    const sc_bool_vector* a;
    const char* b;
    sc_bool_vector_orop2(const sc_bool_vector* x, const char* y)
        : a(x), b(y) { }
};

struct sc_bool_vector_xorop1 {
    const sc_bool_vector* a;
    const sc_bool_vector* b;
    sc_bool_vector_xorop1(const sc_bool_vector* x, const sc_bool_vector* y)
        : a(x), b(y) { }
};

struct sc_bool_vector_xorop2 {
    const sc_bool_vector* a;
    const char* b;
    sc_bool_vector_xorop2(const sc_bool_vector* x, const char* y)
        : a(x), b(y) { }
};

struct sc_bool_vector_notop {
    const sc_bool_vector* a;
    sc_bool_vector_notop(const sc_bool_vector* x) :  a(x) { }
};

//
//  `sc_bool_vector' is a specialization of sc_array<bool>.  In addition to
//  the methods it inherits from sc_array<bool>, `sc_bool_vector' also
//  provides operators for performing bitwise operations (|, &, ^, ~),
//  as well as conversions to native signed integers, unsigned integers,
//  and C-string (char[] of `0's and `1's).  The comma operator
//  (operator,()) allows the concatenation of two sc_bool_vectors, a
//  sc_bool_vector with a bool variable, a bool variable with a
//  sc_bool_vector, a sc_bool_vector with a string.  The result of
//  concatenation serves as a reference into the constituent vectors.
//  If any of these constituents is a const object or a string, then
//  the result is also const (i.e., certain destructive operators may
//  not be used).
//

class sc_bool_vector : public sc_array<bool> {
    friend class sc_logic_vector;
    friend class sc_unsigned;
    friend class sc_signed;
    friend class sc_2d<sc_bool_vector>;
    friend class sc_channel_array_gnu<sc_bool_vector, bool>;

public:
    // Constructor: takes the length of the vector.  The left bound will
    // be 0, and the right bound will be (len - 1).  Note that if you
    // say
    //
    //     sc_bool_vector foo = 6;
    //
    // you get a sc_bool_vector of length 6 instead of a sc_bool_vector with
    // a value 6.  The explicit specifier prevents implicit type
    // conversion from taking place.
    explicit sc_bool_vector( int len );

    // Constructor: creates a SystemC array reference to an existing
    // `sc_bool_vector' with possibly different bounds.  
    sc_bool_vector( int len, const sc_bool_vector& ref );

    // Copy constructor
    sc_bool_vector( const sc_bool_vector& y );

    sc_bool_vector( const sc_bool_vector_andop1& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_andop1(x.a, x.b); }
    sc_bool_vector( const sc_bool_vector_andop2& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_andop2(x.a, x.b); }

    sc_bool_vector( const sc_bool_vector_orop1& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_andop1(x.a, x.b); }
    sc_bool_vector( const sc_bool_vector_orop2& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_andop2(x.a, x.b); }

    sc_bool_vector( const sc_bool_vector_xorop1& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_xorop1(x.a, x.b); }
    sc_bool_vector( const sc_bool_vector_xorop2& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_xorop2(x.a, x.b); }

    sc_bool_vector( const sc_bool_vector_notop& x )
        : sc_array<bool>(x.a->length()), str(0) { asgn_notop(x.a); }

protected:
    // Constructors for derived class to supply data_ptrs
    sc_bool_vector( bool** data_ptrs, int len );

public:
    ~sc_bool_vector();

    // Assignments may be made to a sc_bool_vector from various types of
    // objects.
    sc_bool_vector& operator=( const sc_bool_vector& nv );
    sc_bool_vector& operator=( const sc_logic_vector& nv );
    sc_bool_vector& operator=( const char* nv );
    sc_bool_vector& operator=( const bool* nv );
    sc_bool_vector& operator=( const sc_unsigned& nv );
    sc_bool_vector& operator=( const sc_signed& nv );
    sc_bool_vector& operator=( long nv );
    sc_bool_vector& operator=( unsigned long nv );
    sc_bool_vector& operator=( int nv )      { return operator=(long(nv));           }
    sc_bool_vector& operator=( unsigned nv ) { return operator=((unsigned long) nv); }
    sc_bool_vector& operator=( int64 nv );
    sc_bool_vector& operator=( uint64 nv );
    sc_bool_vector& operator=( const sc_int_base& v);
    sc_bool_vector& operator=( const sc_uint_base& v);

    sc_bool_vector& operator=( const sc_bool_vector_andop1& x )
        { return asgn_andop1(x.a, x.b); }
    sc_bool_vector& operator=( const sc_bool_vector_andop2& x )
        { return asgn_andop2(x.a, x.b); }

    sc_bool_vector& operator=( const sc_bool_vector_orop1& x )
        { return asgn_orop1(x.a, x.b); }
    sc_bool_vector& operator=( const sc_bool_vector_orop2& x )
        { return asgn_orop2(x.a, x.b); }

    sc_bool_vector& operator=( const sc_bool_vector_xorop1& x )
        { return asgn_xorop1(x.a, x.b); }
    sc_bool_vector& operator=( const sc_bool_vector_xorop2& x )
        { return asgn_xorop2(x.a, x.b); }

    sc_bool_vector& operator=( const sc_bool_vector_notop& x )
        { return asgn_notop(x.a); }

    sc_bool_vector& operator=( const sc_bv_ns::sc_bv_base&);

    // Comparison operators
    bool operator==( const sc_bool_vector& rhs ) const;
    bool operator==( const sc_logic_vector& rhs ) const;
    bool operator==( const char* rhs ) const;
    bool operator!=( const sc_bool_vector& rhs ) const;
    bool operator!=( const sc_logic_vector& rhs ) const;
    bool operator!=( const char* rhs ) const;

    bool and_reduce()  const;
    bool nand_reduce() const { return !and_reduce(); }
    bool or_reduce()   const;
    bool nor_reduce()  const { return !or_reduce(); }
    bool xor_reduce()  const;
    bool xnor_reduce() const { return !xor_reduce(); }

    // Subvector methods.  Note that the subscript operator[] is
    // inherited from sc_array<bool>.  Note also the const vs.
    // nonconst versions.  See sc_array.h for the defn. of the macro.
    DECLARE_PARTSEL(sc_bool_vector)

public:
    // Operators for performing bitwise operations
    // with sc_bool_vector, sc_logic_vector, or char*
    sc_bool_vector& operator&=( const sc_bool_vector& rhs );
    sc_bool_vector& operator&=( const sc_logic_vector& rhs );
    sc_bool_vector& operator&=( const char* rhs );
    sc_bool_vector& operator|=( const sc_bool_vector& rhs );
    sc_bool_vector& operator|=( const sc_logic_vector& rhs );
    sc_bool_vector& operator|=( const char* rhs );
    sc_bool_vector& operator^=( const sc_bool_vector& rhs );
    sc_bool_vector& operator^=( const sc_logic_vector& rhs );
    sc_bool_vector& operator^=( const char* rhs );

    // Perform logical inversion
    sc_bool_vector& negate();

    // There are two forms of to_string().  The first form
    // returns a const char* to an internal buffer, and the
    // second form takes the buffer as an argument and stores
    // the string (`0's and `1's) representation in `buffer'.
    const char* to_string() const;
    void to_string( char* buffer ) const;

    // to_signed() returns the signed-integer interpretation of the
    // sc_bool_vector; the MSB (left) is treated as the sign-bit.  If
    // the length of the sc_bool_vector is bigger than 32 for whatever
    // value of 32 your C++ compiler has, then the result will be
    // truncated.  The sign bit will however be retained.
    int to_signed() const;

    // to_unsigned() returns the unsigned-integer interpretation of the
    // sc_bool_vector.  If the length of the sc_bool_vector is bigger than 32
    // for whatever value of 32 your C++ compiler has, then the result
    // will be truncated.
    unsigned to_unsigned() const;

    void print(ostream& os) const;
    void dump(ostream& os) const;

    // comma operators for concatenation
    sc_bool_vector& operator,(sc_bool_vector& y )
    { return *(sc_bool_vector::get_concat_array( this, &y ));}

    sc_bool_vector& operator,( bool& y )
    { return *(sc_bool_vector::get_concat_array( this, &y ));}

    const sc_bool_vector& operator,( const char* y ) const
    { return *(sc_bool_vector::get_concat_array( (sc_bool_vector*)this, y ));}

    const sc_bool_vector& operator,( const sc_signal_bool_vector& y ) const
    { 
        return *(sc_bool_vector::get_concat_array( (sc_bool_vector*)this,
	 const_cast<sc_bool_vector*>(
	 reinterpret_cast<const sc_bool_vector*>(&y) )));  
    }

    const sc_bool_vector& operator,( const sc_bool_vector& y )const
    { return *(sc_bool_vector::get_concat_array( (sc_bool_vector*)this,
       (sc_bool_vector*) &y ));}

    const sc_bool_vector& operator,( const sc_signal<bool>& y )const
    { return *(sc_bool_vector::get_concat_array( (sc_bool_vector*)this,
      (bool*) y.get_data_ptr() ));}

    const sc_bool_vector& operator,(const bool& y ) const
    { return *(sc_bool_vector::get_concat_array( const_cast<sc_bool_vector*>(this), (bool*) &y ));}

protected:
    // This `virtual constructor' is required in any subtype of
    // sc_array_base, and is used to generate subvectors of the correct
    // type.  The base vector is presented as the first argument, and
    // the subvector bounds are specified as `i' and `j'.
    sc_array_base* vctor( const sc_array_base* p, int i, int j ) const;

    // This is the actual constructor called by the virtual constructor.
    sc_bool_vector( const sc_array_base* p, int i, int j );

    // This is used by operator|(), operator&(), and operator^().
    sc_bool_vector( bool* initialized_data, int len );

    // These constructors are called by the concatenation operators.
    sc_bool_vector( sc_bool_vector* v1, sc_bool_vector* v2 );
    sc_bool_vector( sc_bool_vector* v1, bool* v2 );
    sc_bool_vector( bool* v1, sc_bool_vector* v2 );
    sc_bool_vector( bool* v1, bool* v2 );
    sc_bool_vector( const sc_bool_vector* v1, const char* v2 );
    sc_bool_vector( const char* v1, const sc_bool_vector* v2 );

    void assign_concat_with_string( const sc_bool_vector* v1, const char* v2 );
    void assign_concat_with_string( const char* v1, const sc_bool_vector* v2 );

private:
    mutable char* str;    // Stores the string-representation of the sc_bool_vector

public:
    // These static functions are used as auxiliary functions for the
    // concatenation operators.  In other words, the concatenation
    // operators are simply `wrappers' arounds these functions.  These
    // functions, in turn, call `the_concat_manager' to get a unique
    // object that represents the concatenated array.

    // DO NOT CALL THESE FUNCTIONS DIRECTLY

    static sc_bool_vector* get_concat_array( sc_bool_vector*, sc_bool_vector* );
    static sc_bool_vector* get_concat_array( sc_bool_vector*, bool* );
    static sc_bool_vector* get_concat_array( bool*, sc_bool_vector* );
    static sc_bool_vector* get_concat_array( bool*, bool* );
    static const sc_bool_vector* get_concat_array( sc_bool_vector*, const char* );
    static const sc_bool_vector* get_concat_array( const char*, sc_bool_vector* );

protected:
    // These static functions dynamically create instances of
    // concatenated arrays.  They have a uniform interface with other
    // vector types because `the_concat_manager' remains typeless, and
    // must utilize these functions to create the correct type of objects.
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );
    static sc_array_base* new_array_vc( sc_array_base* x, const char* y );
    static sc_array_base* new_array_cv( const char* y, sc_array_base* x );

protected:
    typedef const char* asgn_type1;
    typedef const bool* asgn_type2;
    class asgn_type3{ }; sc_bool_vector& operator=(const asgn_type3&) { return *this; }
    class asgn_type4{ }; sc_bool_vector& operator=(const asgn_type4&) { return *this; }
    class asgn_type5{ }; sc_bool_vector& operator=(const asgn_type5&) { return *this; }

    sc_bool_vector& asgn_andop1(const sc_bool_vector* a, const sc_bool_vector* b);
    sc_bool_vector& asgn_andop2(const sc_bool_vector* a, const char* b);
    sc_bool_vector& asgn_orop1(const sc_bool_vector* a, const sc_bool_vector* b);
    sc_bool_vector& asgn_orop2(const sc_bool_vector* a, const char* b);
    sc_bool_vector& asgn_xorop1(const sc_bool_vector* a, const sc_bool_vector* b);
    sc_bool_vector& asgn_xorop2(const sc_bool_vector* a, const char* b);
    sc_bool_vector& asgn_notop(const sc_bool_vector* a);
    
public:
    typedef bool elemtype;

};


// operator,() for various combinations of operands.  Note that if at
// least a parameter is a const, the resulting type is also a const.
// Note also that concatentation of a sc_bool_vector with a
// sc_signal_bool_vector results in a const sc_bool_vector.
#if !defined(_MSC_VER)
extern sc_bool_vector& operator,( bool&, sc_bool_vector& );
extern const sc_bool_vector& operator,( const char*, const sc_bool_vector& );
extern const sc_bool_vector& operator,( const bool&, const sc_bool_vector& );
extern const sc_bool_vector& operator,( const sc_signal<bool>&, const sc_bool_vector& );
#endif

/*---------------------------------------------------------------------------*/

//
//  `sc_signal_bool_vector' is the signal form of `sc_bool_vector'.
//

class sc_signal_bool_vector : public sc_signal_array< sc_bool_vector >
{
    friend class sc_clock;

public:
    typedef sc_signal_array<sc_bool_vector> templ_type;
    typedef templ_type super;

    explicit sc_signal_bool_vector( int len );
    sc_signal_bool_vector( const char* nm, int len );
    sc_signal_bool_vector( int len, const sc_signal_bool_vector& ref );

protected:
    sc_signal_bool_vector( const sc_signal_bool_vector& );

public:
    // Destructor
    ~sc_signal_bool_vector();

    DECLARE_PARTSEL(sc_signal_bool_vector)

public:
#define DECL_WRITE(T) \
    sc_signal_bool_vector& write(T nv); \
    sc_signal_bool_vector& operator=(T nv) \
    { \
        return write(nv); \
    }
    DECL_WRITE( const sc_array<bool>& )
    sc_signal_bool_vector& operator=( const sc_signal_bool_vector& v )
    {   /* Always remember to define this assignment operator! */
        return write( v.read() );
    }
    DECL_WRITE( const bool* )
    DECL_WRITE( const char* )
    DECL_WRITE( const sc_signed& )
    DECL_WRITE( const sc_unsigned& )
    DECL_WRITE( int )
    DECL_WRITE( unsigned )
    DECL_WRITE( int64 )
    DECL_WRITE( uint64 )
    DECL_WRITE( const sc_logic_vector& )
    DECL_WRITE( const sc_bool_vector_andop1& )
    DECL_WRITE( const sc_bool_vector_andop2& )
    DECL_WRITE( const sc_bool_vector_orop1& )
    DECL_WRITE( const sc_bool_vector_orop2& )
    DECL_WRITE( const sc_bool_vector_xorop1& )
    DECL_WRITE( const sc_bool_vector_xorop2& )
    DECL_WRITE( const sc_bool_vector_notop& )

#undef DECL_WRITE

protected:
    sc_array_base* vctor( const sc_array_base* p, int i, int j ) const;
    sc_signal_bool_vector( const sc_array_base* p, int i, int j );

private:
    void operator=(void*)  const { assert(false); }
    void operator&=(void*) const { assert(false); }
    void operator|=(void*) const { assert(false); }
    void operator^=(void*) const { assert(false); }
    void negate()          const { assert(false); }

protected:
    sc_signal_bool_vector( sc_signal_bool_vector*, sc_signal_bool_vector* );
    sc_signal_bool_vector( sc_signal_bool_vector*, sc_signal<bool>* );
    sc_signal_bool_vector( sc_signal<bool>*, sc_signal_bool_vector* );
    sc_signal_bool_vector( sc_signal<bool>*, sc_signal<bool>* );

public:
    static sc_signal_bool_vector* get_concat_array( sc_signal_bool_vector*, sc_signal_bool_vector* );
    static sc_signal_bool_vector* get_concat_array( sc_signal_bool_vector*, sc_signal<bool>* );
    static sc_signal_bool_vector* get_concat_array( sc_signal<bool>*, sc_signal_bool_vector* );
    static sc_signal_bool_vector* get_concat_array( sc_signal<bool>*, sc_signal<bool>* );

protected:
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );

public:
    void print(ostream& os) const;
    void dump(ostream& os) const;
    void trace( sc_trace_file* tf ) const;

    // comma operators
    const sc_bool_vector& operator,(const sc_bool_vector& y ) const
    {
       return *(sc_bool_vector::get_concat_array(
         const_cast<sc_signal_bool_vector*>(this),
         const_cast<sc_bool_vector*>(&y)));
    }
    sc_signal_bool_vector& operator,( sc_signal_bool_vector& y )
    { return *(sc_signal_bool_vector::get_concat_array( this, &y ));}

    sc_signal_bool_vector& operator,( sc_signal<bool>& y )
    { return *(sc_signal_bool_vector::get_concat_array(this,&y ));}

    const sc_signal_bool_vector& operator,( const sc_signal_bool_vector& y )const
    {
        return *(sc_signal_bool_vector::get_concat_array(
          const_cast<sc_signal_bool_vector*>(this),
          const_cast<sc_signal_bool_vector*>(&y) ));
    }

    const sc_signal_bool_vector&
    operator,( const sc_signal<bool>& y ) const
    {
        return *(sc_signal_bool_vector::get_concat_array(
         const_cast<sc_signal_bool_vector*>(this),
         const_cast<sc_signal<bool>*>(&y) ));
    }

};

#if !defined(_MSC_VER)
extern sc_signal_bool_vector& operator,( sc_signal<bool>&, sc_signal_bool_vector& );
extern const sc_signal_bool_vector& operator,( const sc_signal<bool>&, const sc_signal_bool_vector& );
#endif
extern ostream& operator<<(ostream&, const sc_bool_vector&);

// inline functions implementation

inline bool sc_bool_vector::operator==( const sc_bool_vector& rhs ) const
{ return sc_array<bool>::operator==(rhs);}

inline bool sc_bool_vector::operator!=( const sc_bool_vector& rhs ) const
{ return sc_array<bool>::operator!=(rhs);}

inline bool
sc_bool_vector::operator!=( const sc_logic_vector& rhs ) const
{    return !(sc_bool_vector::operator==(rhs));}

inline bool
sc_bool_vector::operator!=( const char* rhs ) const
{    return !(sc_bool_vector::operator==(rhs));}


inline const sc_bool_vector_andop1 operator&( const sc_bool_vector& a, const sc_bool_vector& b )
{    return sc_bool_vector_andop1(&a, &b);}

inline const sc_bool_vector_andop2 operator&( const sc_bool_vector& a, const char* b )
{    return sc_bool_vector_andop2(&a, b);}

inline const sc_bool_vector_andop2 operator&( const char* a, const sc_bool_vector& b )
{    return sc_bool_vector_andop2(&b, a);}

inline const sc_bool_vector_orop1 operator|( const sc_bool_vector& a, const sc_bool_vector& b )
{    return sc_bool_vector_orop1(&a, &b);}

inline const sc_bool_vector_orop2 operator|( const sc_bool_vector& a, const char* b )
{    return sc_bool_vector_orop2(&a, b);}
inline const sc_bool_vector_orop2 operator|( const char* a, const sc_bool_vector& b )
{    return sc_bool_vector_orop2(&b, a);}

inline const sc_bool_vector_xorop1 operator^( const sc_bool_vector& a, const sc_bool_vector& b )
{    return sc_bool_vector_xorop1( &a, &b );}

inline const sc_bool_vector_xorop2 operator^( const sc_bool_vector& a, const char* b )
{    return sc_bool_vector_xorop2( &a, b );}

inline const sc_bool_vector_xorop2 operator^( const char* a, const sc_bool_vector& b )
{    return sc_bool_vector_xorop2( &b, a );}

inline sc_bool_vector operator~( const sc_bool_vector& a )
{    return sc_bool_vector_notop( &a );}

inline bool and_reduce( const sc_bool_vector& a ) { return a.and_reduce(); }
inline bool nand_reduce( const sc_bool_vector& a ) { return a.nand_reduce(); }
inline bool or_reduce( const sc_bool_vector& a ) { return a.or_reduce(); }
inline bool nor_reduce( const sc_bool_vector& a ) { return a.nor_reduce(); }
inline bool xor_reduce( const sc_bool_vector& a ) { return a.xor_reduce(); }
inline bool xnor_reduce( const sc_bool_vector& a ) { return a.xnor_reduce(); }

#endif
