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

    sc_fxval.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXVAL_H
#define SC_FXVAL_H


#include "scfx_rep.h"
#ifndef SC_FX_EXCLUDE_OTHER
#include "../sc_int.h"
#include "../sc_uint.h"
#include "../sc_signed.h"
#include "../sc_unsigned.h"
#endif
#include "sc_fxval_observer.h"


class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval
//
//  Fixed-point value type; arbitrary precision.
// ----------------------------------------------------------------------------

class sc_fxval
{

    friend class sc_fxnum;

protected:

    sc_fxval_observer* observer() const;

public:

    // internal use only;
    sc_fxval( scfx_rep* );


    explicit sc_fxval( sc_fxval_observer* = 0 );
             sc_fxval( int,
		       sc_fxval_observer* = 0 );
             sc_fxval( unsigned int,
		       sc_fxval_observer* = 0 );
             sc_fxval( long,
		       sc_fxval_observer* = 0 );
             sc_fxval( unsigned long,
		       sc_fxval_observer* = 0 );
             sc_fxval( double,
		       sc_fxval_observer* = 0 );
             sc_fxval( const char*,
		       sc_fxval_observer* = 0 );
             sc_fxval( const sc_fxval&,
		       sc_fxval_observer* = 0 );
             sc_fxval( const sc_fxval_fast&,
		       sc_fxval_observer* = 0 );
             sc_fxval( const sc_fxnum&,
		       sc_fxval_observer* = 0 );
             sc_fxval( const sc_fxnum_fast&,
		       sc_fxval_observer* = 0 );
#ifndef SC_FX_EXCLUDE_OTHER
    explicit sc_fxval( int64,
		       sc_fxval_observer* = 0 );
    explicit sc_fxval( uint64,
		       sc_fxval_observer* = 0 );
    explicit sc_fxval( const sc_int_base&,
		       sc_fxval_observer* = 0 );
    explicit sc_fxval( const sc_uint_base&,
		       sc_fxval_observer* = 0 );
    explicit sc_fxval( const sc_signed&,
		       sc_fxval_observer* = 0 );
    explicit sc_fxval( const sc_unsigned&,
		       sc_fxval_observer* = 0 );
#endif

    ~sc_fxval();


    // internal use only;
    const scfx_rep* get_rep() const;
    void            set_rep( scfx_rep* );


    // unary operators

    const sc_fxval  operator - () const;
    const sc_fxval& operator + () const;


    // unary functions

    friend void neg( sc_fxval&, const sc_fxval& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval operator op ( const sc_fxval&, tp );                \
    friend const sc_fxval operator op ( tp, const sc_fxval& );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_BIN_OP_OTHER(op)                                                 \
    DECL_BIN_OP_T(op,int64)                                                   \
    DECL_BIN_OP_T(op,uint64)                                                  \
    DECL_BIN_OP_T(op,const sc_int_base&)                                      \
    DECL_BIN_OP_T(op,const sc_uint_base&)                                     \
    DECL_BIN_OP_T(op,const sc_signed&)                                        \
    DECL_BIN_OP_T(op,const sc_unsigned&)
#else
#define DECL_BIN_OP_OTHER(op)
#endif

#define DECL_BIN_OP(op,dummy)                                                 \
    friend const sc_fxval operator op ( const sc_fxval&, const sc_fxval& );   \
    DECL_BIN_OP_T(op,int)                                                     \
    DECL_BIN_OP_T(op,unsigned int)                                            \
    DECL_BIN_OP_T(op,long)                                                    \
    DECL_BIN_OP_T(op,unsigned long)                                           \
    DECL_BIN_OP_T(op,double)                                                  \
    DECL_BIN_OP_T(op,const char*)                                             \
    DECL_BIN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_BIN_OP_T(op,const sc_fxnum_fast&)                                    \
    DECL_BIN_OP_OTHER(op)

    DECL_BIN_OP(*,mult)
    DECL_BIN_OP(+,add)
    DECL_BIN_OP(-,sub)
// declaration below doesn't compile with BCB5 (E2206)
//    DECL_BIN_OP(/,div)
// previous macro expanded
    friend const sc_fxval operator / ( const sc_fxval&, const sc_fxval& );
    DECL_BIN_OP_T(/,int)
    DECL_BIN_OP_T(/,unsigned int)
    DECL_BIN_OP_T(/,long)
    DECL_BIN_OP_T(/,unsigned long)
    DECL_BIN_OP_T(/,double)
    DECL_BIN_OP_T(/,const char*)
    DECL_BIN_OP_T(/,const sc_fxval_fast&)
    DECL_BIN_OP_T(/,const sc_fxnum_fast&)
//    DECL_BIN_OP_OTHER(/)
#ifndef SC_FX_EXCLUDE_OTHER
    DECL_BIN_OP_T(/,int64)                                                   \
    DECL_BIN_OP_T(/,uint64)                                                  \
    DECL_BIN_OP_T(/,const sc_int_base&)                                      \
    DECL_BIN_OP_T(/,const sc_uint_base&)                                     \
    DECL_BIN_OP_T(/,const sc_signed&)                                        \
    DECL_BIN_OP_T(/,const sc_unsigned&)
#endif


#undef DECL_BIN_OP_T
#undef DECL_BIN_OP_OTHER
#undef DECL_BIN_OP

    friend const sc_fxval operator << ( const sc_fxval&, int );
    friend const sc_fxval operator >> ( const sc_fxval&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval&, const sc_fxval&, tp );                       \
    friend void fnc ( sc_fxval&, tp, const sc_fxval& );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_BIN_FNC_OTHER(fnc)                                               \
    DECL_BIN_FNC_T(fnc,int64)                                                 \
    DECL_BIN_FNC_T(fnc,uint64)                                                \
    DECL_BIN_FNC_T(fnc,const sc_int_base&)                                    \
    DECL_BIN_FNC_T(fnc,const sc_uint_base&)                                   \
    DECL_BIN_FNC_T(fnc,const sc_signed&)                                      \
    DECL_BIN_FNC_T(fnc,const sc_unsigned&)
#else
#define DECL_BIN_FNC_OTHER(fnc)
#endif

#define DECL_BIN_FNC(fnc)                                                     \
    friend void fnc ( sc_fxval&, const sc_fxval&, const sc_fxval& );          \
    DECL_BIN_FNC_T(fnc,int)                                                   \
    DECL_BIN_FNC_T(fnc,unsigned int)                                          \
    DECL_BIN_FNC_T(fnc,long)                                                  \
    DECL_BIN_FNC_T(fnc,unsigned long)                                         \
    DECL_BIN_FNC_T(fnc,double)                                                \
    DECL_BIN_FNC_T(fnc,const char*)                                           \
    DECL_BIN_FNC_T(fnc,const sc_fxval_fast&)                                  \
    DECL_BIN_FNC_T(fnc,const sc_fxnum_fast&)                                  \
    DECL_BIN_FNC_OTHER(fnc)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC_OTHER
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval&, const sc_fxval&, int );
    friend void rshift( sc_fxval&, const sc_fxval&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxval&, tp );                          \
    friend bool operator op ( tp, const sc_fxval& );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_REL_OP_OTHER(op)                                                 \
    DECL_REL_OP_T(op,int64)                                                   \
    DECL_REL_OP_T(op,uint64)                                                  \
    DECL_REL_OP_T(op,const sc_int_base&)                                      \
    DECL_REL_OP_T(op,const sc_uint_base&)                                     \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)
#else
#define DECL_REL_OP_OTHER(op)
#endif

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxval&, const sc_fxval& );             \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)                                           \
    DECL_REL_OP_T(op,double)                                                  \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_REL_OP_T(op,const sc_fxnum_fast&)                                    \
    DECL_REL_OP_OTHER(op)

    DECL_REL_OP(<)
    DECL_REL_OP(<=)
    DECL_REL_OP(>)
    DECL_REL_OP(>=)
    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP_OTHER
#undef DECL_REL_OP


    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxval& operator op( tp );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_ASN_OP_OTHER(op)                                                 \
    DECL_ASN_OP_T(op,int64)                                                   \
    DECL_ASN_OP_T(op,uint64)                                                  \
    DECL_ASN_OP_T(op,const sc_int_base&)                                      \
    DECL_ASN_OP_T(op,const sc_uint_base&)                                     \
    DECL_ASN_OP_T(op,const sc_signed&)                                        \
    DECL_ASN_OP_T(op,const sc_unsigned&)
#else
#define DECL_ASN_OP_OTHER(op)
#endif

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,int)                                                     \
    DECL_ASN_OP_T(op,unsigned int)                                            \
    DECL_ASN_OP_T(op,long)                                                    \
    DECL_ASN_OP_T(op,unsigned long)                                           \
    DECL_ASN_OP_T(op,double)                                                  \
    DECL_ASN_OP_T(op,const char*)                                             \
    DECL_ASN_OP_T(op,const sc_fxval&)                                         \
    DECL_ASN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_ASN_OP_T(op,const sc_fxnum&)                                         \
    DECL_ASN_OP_T(op,const sc_fxnum_fast&)                                    \
    DECL_ASN_OP_OTHER(op)

    DECL_ASN_OP(=)

    DECL_ASN_OP(*=)
    DECL_ASN_OP(/=)
    DECL_ASN_OP(+=)
    DECL_ASN_OP(-=)

    DECL_ASN_OP_T(<<=,int)
    DECL_ASN_OP_T(>>=,int)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP_OTHER
#undef DECL_ASN_OP


    // auto-increment and auto-decrement

    const sc_fxval operator ++ ( int );
    const sc_fxval operator -- ( int );

    sc_fxval& operator ++ ();
    sc_fxval& operator -- ();


    // implicit conversion

    operator double() const;            // necessary evil!


    // explicit conversion to primitive types

    short          to_short() const;
    unsigned short to_ushort() const;
    int            to_int() const;
    unsigned int   to_uint() const;
    long           to_long() const;
    unsigned long  to_ulong() const;
    float          to_float() const;
    double         to_double() const;


    // explicit conversion to character string

    const sc_string to_string() const;
    const sc_string to_string( sc_numrep ) const;
    const sc_string to_string( sc_fmt ) const;
    const sc_string to_string( sc_numrep, sc_fmt ) const;

    const sc_string to_dec() const;
    const sc_string to_bin() const;
    const sc_string to_oct() const;
    const sc_string to_hex() const;


    // query value

    bool is_neg() const;
    bool is_zero() const;
    bool is_nan() const;
    bool is_inf() const;
    bool is_normal() const;
    
    bool rounding_flag() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;


    // internal use only;
    bool get_bit( int ) const;

protected:

    sc_fxval_observer* lock_observer() const;
    void unlock_observer( sc_fxval_observer* ) const;


    void get_type( int&, int&, sc_enc& ) const;

    const sc_fxval quantization( const scfx_params&, bool& ) const;
    const sc_fxval     overflow( const scfx_params&, bool& ) const;

private:

    scfx_rep*                  _rep;

    mutable sc_fxval_observer* _observer;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast
//
//  Fixed-point value type; limited precision.
// ----------------------------------------------------------------------------

class sc_fxval_fast
{

    friend class sc_fxnum_fast;

protected:

    sc_fxval_fast_observer* observer() const;

public:

    explicit sc_fxval_fast( sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( int,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( unsigned int,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( long,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( unsigned long,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( double,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( const char*,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( const sc_fxval&,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( const sc_fxval_fast&,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( const sc_fxnum&,
			    sc_fxval_fast_observer* = 0 );
             sc_fxval_fast( const sc_fxnum_fast&,
			    sc_fxval_fast_observer* = 0 );
#ifndef SC_FX_EXCLUDE_OTHER
    explicit sc_fxval_fast( int64,
			    sc_fxval_fast_observer* = 0 );
    explicit sc_fxval_fast( uint64,
			    sc_fxval_fast_observer* = 0 );
    explicit sc_fxval_fast( const sc_int_base&,
			    sc_fxval_fast_observer* = 0 );
    explicit sc_fxval_fast( const sc_uint_base&,
			    sc_fxval_fast_observer* = 0 );
    explicit sc_fxval_fast( const sc_signed&,
			    sc_fxval_fast_observer* = 0 );
    explicit sc_fxval_fast( const sc_unsigned&,
			    sc_fxval_fast_observer* = 0 );
#endif

    ~sc_fxval_fast();

    // internal use only;
    double get_val() const;
    void set_val( double );


    // unary operators

    const sc_fxval_fast  operator - () const;
    const sc_fxval_fast& operator + () const;


    // unary functions

    friend void neg( sc_fxval_fast&, const sc_fxval_fast& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval_fast operator op ( const sc_fxval_fast&, tp );      \
    friend const sc_fxval_fast operator op ( tp, const sc_fxval_fast& );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_BIN_OP_OTHER(op)                                                 \
    DECL_BIN_OP_T(op,int64)                                                   \
    DECL_BIN_OP_T(op,uint64)                                                  \
    DECL_BIN_OP_T(op,const sc_int_base&)                                      \
    DECL_BIN_OP_T(op,const sc_uint_base&)                                     \
    DECL_BIN_OP_T(op,const sc_signed&)                                        \
    DECL_BIN_OP_T(op,const sc_unsigned&)
#else
#define DECL_BIN_OP_OTHER(op)
#endif

#define DECL_BIN_OP(op,dummy)                                                 \
    friend const sc_fxval_fast operator op ( const sc_fxval_fast&,            \
					     const sc_fxval_fast& );          \
    DECL_BIN_OP_T(op,int)                                                     \
    DECL_BIN_OP_T(op,unsigned int)                                            \
    DECL_BIN_OP_T(op,long)                                                    \
    DECL_BIN_OP_T(op,unsigned long)                                           \
    DECL_BIN_OP_T(op,double)                                                  \
    DECL_BIN_OP_T(op,const char*)                                             \
    DECL_BIN_OP_OTHER(op)

    DECL_BIN_OP(*,mult)
    DECL_BIN_OP(+,add)
    DECL_BIN_OP(-,sub)
// don't use macro
//    DECL_BIN_OP(/,div)
    friend const sc_fxval_fast operator / ( const sc_fxval_fast&,
					     const sc_fxval_fast& );
    DECL_BIN_OP_T(/,int)
    DECL_BIN_OP_T(/,unsigned int)
    DECL_BIN_OP_T(/,long)
    DECL_BIN_OP_T(/,unsigned long)
    DECL_BIN_OP_T(/,double)
    DECL_BIN_OP_T(/,const char*)
//    DECL_BIN_OP_OTHER(/)
#ifndef SC_FX_EXCLUDE_OTHER
    DECL_BIN_OP_T(/,int64)                                                   \
    DECL_BIN_OP_T(/,uint64)                                                  \
    DECL_BIN_OP_T(/,const sc_int_base&)                                      \
    DECL_BIN_OP_T(/,const sc_uint_base&)                                     \
    DECL_BIN_OP_T(/,const sc_signed&)                                        \
    DECL_BIN_OP_T(/,const sc_unsigned&)
#endif

#undef DECL_BIN_OP_T
#undef DECL_BIN_OP_OTHER
#undef DECL_BIN_OP

    friend const sc_fxval_fast operator << ( const sc_fxval_fast&, int );
    friend const sc_fxval_fast operator >> ( const sc_fxval_fast&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval_fast&, const sc_fxval_fast&, tp );             \
    friend void fnc ( sc_fxval_fast&, tp, const sc_fxval_fast& );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_BIN_FNC_OTHER(fnc)                                               \
    DECL_BIN_FNC_T(fnc,int64)                                                 \
    DECL_BIN_FNC_T(fnc,uint64)                                                \
    DECL_BIN_FNC_T(fnc,const sc_int_base&)                                    \
    DECL_BIN_FNC_T(fnc,const sc_uint_base&)                                   \
    DECL_BIN_FNC_T(fnc,const sc_signed&)                                      \
    DECL_BIN_FNC_T(fnc,const sc_unsigned&)
#else
#define DECL_BIN_FNC_OTHER(fnc)
#endif

#define DECL_BIN_FNC(fnc)                                                     \
    friend void fnc ( sc_fxval_fast&, const sc_fxval_fast&,                   \
                      const sc_fxval_fast& );                                 \
    DECL_BIN_FNC_T(fnc,int)                                                   \
    DECL_BIN_FNC_T(fnc,unsigned int)                                          \
    DECL_BIN_FNC_T(fnc,long)                                                  \
    DECL_BIN_FNC_T(fnc,unsigned long)                                         \
    DECL_BIN_FNC_T(fnc,double)                                                \
    DECL_BIN_FNC_T(fnc,const char*)                                           \
    DECL_BIN_FNC_T(fnc,const sc_fxval&)                                       \
    DECL_BIN_FNC_T(fnc,const sc_fxnum&)                                       \
    DECL_BIN_FNC_OTHER(fnc)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC_OTHER
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval_fast&, const sc_fxval_fast&, int );
    friend void rshift( sc_fxval_fast&, const sc_fxval_fast&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxval_fast&, tp );                     \
    friend bool operator op ( tp, const sc_fxval_fast& );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_REL_OP_OTHER(op)                                                 \
    DECL_REL_OP_T(op,int64)                                                   \
    DECL_REL_OP_T(op,uint64)                                                  \
    DECL_REL_OP_T(op,const sc_int_base&)                                      \
    DECL_REL_OP_T(op,const sc_uint_base&)                                     \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)
#else
#define DECL_REL_OP_OTHER(op)
#endif

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxval_fast&, const sc_fxval_fast& );   \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)                                           \
    DECL_REL_OP_T(op,double)                                                  \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_OTHER(op)

    DECL_REL_OP(<)
    DECL_REL_OP(<=)
    DECL_REL_OP(>)
    DECL_REL_OP(>=)
    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP_OTHER
#undef DECL_REL_OP


    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxval_fast& operator op( tp );

#ifndef SC_FX_EXCLUDE_OTHER
#define DECL_ASN_OP_OTHER(op)                                                 \
    DECL_ASN_OP_T(op,int64)                                                   \
    DECL_ASN_OP_T(op,uint64)                                                  \
    DECL_ASN_OP_T(op,const sc_int_base&)                                      \
    DECL_ASN_OP_T(op,const sc_uint_base&)                                     \
    DECL_ASN_OP_T(op,const sc_signed&)                                        \
    DECL_ASN_OP_T(op,const sc_unsigned&)
#else
#define DECL_ASN_OP_OTHER(op)
#endif

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,int)                                                     \
    DECL_ASN_OP_T(op,unsigned int)                                            \
    DECL_ASN_OP_T(op,long)                                                    \
    DECL_ASN_OP_T(op,unsigned long)                                           \
    DECL_ASN_OP_T(op,double)                                                  \
    DECL_ASN_OP_T(op,const char*)                                             \
    DECL_ASN_OP_T(op,const sc_fxval&)                                         \
    DECL_ASN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_ASN_OP_T(op,const sc_fxnum&)                                         \
    DECL_ASN_OP_T(op,const sc_fxnum_fast&)                                    \
    DECL_ASN_OP_OTHER(op)

    DECL_ASN_OP(=)

    DECL_ASN_OP(*=)
    DECL_ASN_OP(/=)
    DECL_ASN_OP(+=)
    DECL_ASN_OP(-=)

    DECL_ASN_OP_T(<<=,int)
    DECL_ASN_OP_T(>>=,int)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP_OTHER
#undef DECL_ASN_OP


    // auto-increment and auto-decrement

    const sc_fxval_fast operator ++ ( int );
    const sc_fxval_fast operator -- ( int );

    sc_fxval_fast& operator ++ ();
    sc_fxval_fast& operator -- ();


    // implicit conversion

    operator double() const;            // necessary evil!


    // explicit conversion to primitive types

    short          to_short() const;
    unsigned short to_ushort() const;
    int            to_int() const;
    unsigned int   to_uint() const;
    long           to_long() const;
    unsigned long  to_ulong() const;
    float          to_float() const;
    double         to_double() const;


    // explicit conversion to character string

    const sc_string to_string() const;
    const sc_string to_string( sc_numrep ) const;
    const sc_string to_string( sc_fmt ) const;
    const sc_string to_string( sc_numrep, sc_fmt ) const;

    const sc_string to_dec() const;
    const sc_string to_bin() const;
    const sc_string to_oct() const;
    const sc_string to_hex() const;


    // query value

    bool is_neg() const;
    bool is_zero() const;
    bool is_nan() const;
    bool is_inf() const;
    bool is_normal() const;

    bool rounding_flag() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;


    // internal use only;
    bool get_bit( int ) const;

protected:

    sc_fxval_fast_observer* lock_observer() const;
    void unlock_observer( sc_fxval_fast_observer* ) const;


    static double from_string( const char* );

private:

    double                          _val;

    mutable sc_fxval_fast_observer* _observer;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxval
//
//  Fixed-point value type; arbitrary precision.
// ----------------------------------------------------------------------------

// protected method

inline
sc_fxval_observer*
sc_fxval::observer() const
{
    return _observer;
}


// internal use only;
inline
sc_fxval::sc_fxval( scfx_rep* a )
: _rep( a ),
  _observer( 0 )
{}


// public constructors

inline
sc_fxval::sc_fxval( sc_fxval_observer* observer_ )
: _rep( new scfx_rep ),
  _observer( observer_ )
{
    _SC_FXVAL_OBSERVER_DEFAULT
    _SC_FXVAL_OBSERVER_CONSTRUCT( *this )
}

inline
sc_fxval::sc_fxval( const sc_fxval& a,
		    sc_fxval_observer* observer_ )
: _rep( new scfx_rep( *a._rep ) ),
  _observer( observer_ )
{
    _SC_FXVAL_OBSERVER_DEFAULT
    _SC_FXVAL_OBSERVER_READ( a )
    _SC_FXVAL_OBSERVER_CONSTRUCT( *this )
    _SC_FXVAL_OBSERVER_WRITE( *this )
}

#define DEFN_CTOR_T(tp,arg)                                                   \
inline                                                                        \
sc_fxval::sc_fxval( tp a,                                                     \
                    sc_fxval_observer* observer_ )                            \
: _rep( new scfx_rep( arg ) ),                                                \
  _observer( observer_ )                                                      \
{                                                                             \
    _SC_FXVAL_OBSERVER_DEFAULT                                                \
    _SC_FXVAL_OBSERVER_CONSTRUCT( *this )                                     \
    _SC_FXVAL_OBSERVER_WRITE( *this )                                         \
}

#define DEFN_CTOR_T_A(tp) DEFN_CTOR_T(tp,a)
#define DEFN_CTOR_T_B(tp) DEFN_CTOR_T(tp,a.to_double())
#define DEFN_CTOR_T_C(tp) DEFN_CTOR_T(tp,a.num)

DEFN_CTOR_T_A(int)
DEFN_CTOR_T_A(unsigned int)
DEFN_CTOR_T_A(long)
DEFN_CTOR_T_A(unsigned long)
DEFN_CTOR_T_A(double)
DEFN_CTOR_T_A(const char*)
DEFN_CTOR_T_B(const sc_fxval_fast&)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_CTOR_T_A(int64)
DEFN_CTOR_T_A(uint64)
DEFN_CTOR_T_C(const sc_int_base&)
DEFN_CTOR_T_C(const sc_uint_base&)
DEFN_CTOR_T_A(const sc_signed&)
DEFN_CTOR_T_A(const sc_unsigned&)
#endif

#undef DEFN_CTOR_T
#undef DEFN_CTOR_T_A
#undef DEFN_CTOR_T_B
#undef DEFN_CTOR_T_C


inline
sc_fxval::~sc_fxval()
{
    _SC_FXVAL_OBSERVER_DESTRUCT( *this )
    delete _rep;
}


// internal use only;
inline
const scfx_rep*
sc_fxval::get_rep() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep;
}

// internal use only;
inline
void
sc_fxval::set_rep( scfx_rep* rep_ )
{
    delete _rep;
    _rep = rep_;
    _SC_FXVAL_OBSERVER_WRITE( *this )
}


// unary operators

inline
const sc_fxval
sc_fxval::operator - () const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return sc_fxval( ::neg__scfx_rep( *_rep ) );
}

inline
const sc_fxval&
sc_fxval::operator + () const
{
    // _SC_FXVAL_OBSERVER_READ( *this )
    return *this;
}


// unary functions

inline
void
neg( sc_fxval& c, const sc_fxval& a )
{
    _SC_FXVAL_OBSERVER_READ( a )
    delete c._rep;
    c._rep = ::neg__scfx_rep( *a._rep );
    _SC_FXVAL_OBSERVER_WRITE( c )
}


// binary operators

#define DEFN_BIN_OP_T(op,fnc,tp)                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxval& a, tp b )                                       \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    return sc_fxval( ::fnc ## __scfx_rep( *a._rep, *tmp._rep ) );             \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxval& b )                                       \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    return sc_fxval( ::fnc ## __scfx_rep( *tmp._rep, *b._rep ) );             \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_BIN_OP_OTHER(op,fnc)                                             \
DEFN_BIN_OP_T(op,fnc,int64)                                                   \
DEFN_BIN_OP_T(op,fnc,uint64)                                                  \
DEFN_BIN_OP_T(op,fnc,const sc_int_base&)                                      \
DEFN_BIN_OP_T(op,fnc,const sc_uint_base&)                                     \
DEFN_BIN_OP_T(op,fnc,const sc_signed&)                                        \
DEFN_BIN_OP_T(op,fnc,const sc_unsigned&)
#else
#define DEFN_BIN_OP_OTHER(op,fnc)
#endif

#define DEFN_BIN_OP(op,fnc)                                                   \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxval& a, const sc_fxval& b )                          \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( a )                                              \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    return sc_fxval( ::fnc ## __scfx_rep( *a._rep, *b._rep ) );               \
}                                                                             \
                                                                              \
DEFN_BIN_OP_T(op,fnc,int)                                                     \
DEFN_BIN_OP_T(op,fnc,unsigned int)                                            \
DEFN_BIN_OP_T(op,fnc,long)                                                    \
DEFN_BIN_OP_T(op,fnc,unsigned long)                                           \
DEFN_BIN_OP_T(op,fnc,double)                                                  \
DEFN_BIN_OP_T(op,fnc,const char*)                                             \
DEFN_BIN_OP_T(op,fnc,const sc_fxval_fast&)                                    \
DEFN_BIN_OP_OTHER(op,fnc)

DEFN_BIN_OP(*,mult)
DEFN_BIN_OP(+,add)
DEFN_BIN_OP(-,sub)
// don't use macro
//DEFN_BIN_OP(/,div)
inline
const sc_fxval
operator / ( const sc_fxval& a, const sc_fxval& b )
{
    _SC_FXVAL_OBSERVER_READ( a )
    _SC_FXVAL_OBSERVER_READ( b )
    return sc_fxval( ::div__scfx_rep( *a._rep, *b._rep ) );
}

DEFN_BIN_OP_T(/,div,int)
DEFN_BIN_OP_T(/,div,unsigned int)
DEFN_BIN_OP_T(/,div,long)
DEFN_BIN_OP_T(/,div,unsigned long)
DEFN_BIN_OP_T(/,div,double)
DEFN_BIN_OP_T(/,div,const char*)
DEFN_BIN_OP_T(/,div,const sc_fxval_fast&)
//DEFN_BIN_OP_OTHER(/,div)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_BIN_OP_T(/,div,int64)                                                   \
DEFN_BIN_OP_T(/,div,uint64)                                                  \
DEFN_BIN_OP_T(/,div,const sc_int_base&)                                      \
DEFN_BIN_OP_T(/,div,const sc_uint_base&)                                     \
DEFN_BIN_OP_T(/,div,const sc_signed&)                                        \
DEFN_BIN_OP_T(/,div,const sc_unsigned&)
#endif

#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP_OTHER
#undef DEFN_BIN_OP


inline
const sc_fxval
operator << ( const sc_fxval& a, int b )
{
    _SC_FXVAL_OBSERVER_READ( a )
    return sc_fxval( ::lsh__scfx_rep( *a._rep, b ) );
}

inline
const sc_fxval
operator >> ( const sc_fxval& a, int b )
{
    _SC_FXVAL_OBSERVER_READ( a )
    return sc_fxval( ::rsh__scfx_rep( *a._rep, b ) );
}


// binary functions

#define DEFN_BIN_FNC_T(fnc,tp)                                                \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxval& a, tp b )                                  \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *a._rep, *tmp._rep );                       \
    _SC_FXVAL_OBSERVER_WRITE( c )                                             \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxval& b )                                  \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *tmp._rep, *b._rep );                       \
    _SC_FXVAL_OBSERVER_WRITE( c )                                             \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_BIN_FNC_OTHER(fnc)                                               \
DEFN_BIN_FNC_T(fnc,int64)                                                     \
DEFN_BIN_FNC_T(fnc,uint64)                                                    \
DEFN_BIN_FNC_T(fnc,const sc_int_base&)                                        \
DEFN_BIN_FNC_T(fnc,const sc_uint_base&)                                       \
DEFN_BIN_FNC_T(fnc,const sc_signed&)                                          \
DEFN_BIN_FNC_T(fnc,const sc_unsigned&)
#else
#define DEFN_BIN_FNC_OTHER(fnc)
#endif

#define DEFN_BIN_FNC(fnc)                                                     \
inline                                                                        \
void                                                                          \
fnc( sc_fxval& c, const sc_fxval& a, const sc_fxval& b )                      \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( a )                                              \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *a._rep, *b._rep );                         \
    _SC_FXVAL_OBSERVER_WRITE( c )                                             \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T(fnc,int)                                                       \
DEFN_BIN_FNC_T(fnc,unsigned int)                                              \
DEFN_BIN_FNC_T(fnc,long)                                                      \
DEFN_BIN_FNC_T(fnc,unsigned long)                                             \
DEFN_BIN_FNC_T(fnc,double)                                                    \
DEFN_BIN_FNC_T(fnc,const char*)                                               \
DEFN_BIN_FNC_T(fnc,const sc_fxval_fast&)                                      \
DEFN_BIN_FNC_OTHER(fnc)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC_OTHER
#undef DEFN_BIN_FNC


inline
void
lshift( sc_fxval& c, const sc_fxval& a, int b )
{
    _SC_FXVAL_OBSERVER_READ( a )
    delete c._rep;
    c._rep = ::lsh__scfx_rep( *a._rep, b );
    _SC_FXVAL_OBSERVER_WRITE( c )
}

inline
void
rshift( sc_fxval& c, const sc_fxval& a, int b )
{
    _SC_FXVAL_OBSERVER_READ( a )
    delete c._rep;
    c._rep = ::rsh__scfx_rep( *a._rep, b );
    _SC_FXVAL_OBSERVER_WRITE( c )
}


// relational (including equality) operators

#define DEFN_REL_OP_T(op,ret,tp)                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval& a, tp b )                                       \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    int result = ::cmp__scfx_rep( *a._rep, *tmp._rep );                       \
    return ( ret );                                                           \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxval& b )                                       \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    int result = ::cmp__scfx_rep( *tmp._rep, *b._rep );                       \
    return ( ret );                                                           \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_REL_OP_OTHER(op,ret)                                             \
DEFN_REL_OP_T(op,ret,int64)                                                   \
DEFN_REL_OP_T(op,ret,uint64)                                                  \
DEFN_REL_OP_T(op,ret,const sc_int_base&)                                      \
DEFN_REL_OP_T(op,ret,const sc_uint_base&)                                     \
DEFN_REL_OP_T(op,ret,const sc_signed&)                                        \
DEFN_REL_OP_T(op,ret,const sc_unsigned&)
#else
#define DEFN_REL_OP_OTHER(op,ret)
#endif

#define DEFN_REL_OP(op,ret)                                                   \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval& a, const sc_fxval& b)                           \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( a )                                              \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    int result = ::cmp__scfx_rep( *a._rep, *b._rep );                         \
    return ( ret );                                                           \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,ret,int)                                                     \
DEFN_REL_OP_T(op,ret,unsigned int)                                            \
DEFN_REL_OP_T(op,ret,long)                                                    \
DEFN_REL_OP_T(op,ret,unsigned long)                                           \
DEFN_REL_OP_T(op,ret,double)                                                  \
DEFN_REL_OP_T(op,ret,const char*)                                             \
DEFN_REL_OP_T(op,ret,const sc_fxval_fast&)                                    \
DEFN_REL_OP_OTHER(op,ret)

DEFN_REL_OP(<,result < 0)
DEFN_REL_OP(<=,result <= 0)
DEFN_REL_OP(>,result > 0 && result != 2)
DEFN_REL_OP(>=,result >= 0 && result != 2)
DEFN_REL_OP(==,result == 0)
DEFN_REL_OP(!=,result != 0)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP_OTHER
#undef DEFN_REL_OP


// assignment operators

inline
sc_fxval&
sc_fxval::operator = ( const sc_fxval& a )
{
    if( &a != this )
    {
	_SC_FXVAL_OBSERVER_READ( a )
	*_rep = *a._rep;
	_SC_FXVAL_OBSERVER_WRITE( *this )
    }
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
inline                                                                        \
sc_fxval&                                                                     \
sc_fxval::operator = ( tp b )                                                 \
{                                                                             \
    sc_fxval tmp( b );                                                        \
    *_rep = *tmp._rep;                                                        \
    _SC_FXVAL_OBSERVER_WRITE( *this )                                         \
    return *this;                                                             \
}

DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(double)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const sc_fxval_fast&)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_ASN_OP_T(int64)
DEFN_ASN_OP_T(uint64)
DEFN_ASN_OP_T(const sc_int_base&)
DEFN_ASN_OP_T(const sc_uint_base&)
DEFN_ASN_OP_T(const sc_signed&)
DEFN_ASN_OP_T(const sc_unsigned&)
#endif

#undef DEFN_ASN_OP_T


#define DEFN_ASN_OP_T(op,fnc,tp)                                              \
inline                                                                        \
sc_fxval&                                                                     \
sc_fxval::operator op ( tp b )                                                \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( *this )                                          \
    sc_fxval tmp( b );                                                        \
    scfx_rep* new_rep = ::fnc ## __scfx_rep( *_rep, *tmp._rep );              \
    delete _rep;                                                              \
    _rep = new_rep;                                                           \
    _SC_FXVAL_OBSERVER_WRITE( *this )                                         \
    return *this;                                                             \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_ASN_OP_OTHER(op,fnc)                                             \
DEFN_ASN_OP_T(op,fnc,int64)                                                   \
DEFN_ASN_OP_T(op,fnc,uint64)                                                  \
DEFN_ASN_OP_T(op,fnc,const sc_int_base&)                                      \
DEFN_ASN_OP_T(op,fnc,const sc_uint_base&)                                     \
DEFN_ASN_OP_T(op,fnc,const sc_signed&)                                        \
DEFN_ASN_OP_T(op,fnc,const sc_unsigned&)
#else
#define DEFN_ASN_OP_OTHER(op,fnc)
#endif

#define DEFN_ASN_OP(op,fnc)                                                   \
inline                                                                        \
sc_fxval&                                                                     \
sc_fxval::operator op ( const sc_fxval& b )                                   \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( *this )                                          \
    _SC_FXVAL_OBSERVER_READ( b )                                              \
    scfx_rep* new_rep = ::fnc ## __scfx_rep( *_rep, *b._rep );                \
    delete _rep;                                                              \
    _rep = new_rep;                                                           \
    _SC_FXVAL_OBSERVER_WRITE( *this )                                         \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,fnc,int)                                                     \
DEFN_ASN_OP_T(op,fnc,unsigned int)                                            \
DEFN_ASN_OP_T(op,fnc,long)                                                    \
DEFN_ASN_OP_T(op,fnc,unsigned long)                                           \
DEFN_ASN_OP_T(op,fnc,double)                                                  \
DEFN_ASN_OP_T(op,fnc,const char*)                                             \
DEFN_ASN_OP_T(op,fnc,const sc_fxval_fast&)                                    \
DEFN_ASN_OP_OTHER(op,fnc)

DEFN_ASN_OP(*=,mult)
DEFN_ASN_OP(/=,div)
DEFN_ASN_OP(+=,add)
DEFN_ASN_OP(-=,sub)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP_OTHER
#undef DEFN_ASN_OP


inline
sc_fxval&
sc_fxval::operator <<= ( int b )
{
    _SC_FXVAL_OBSERVER_READ( *this )
    _rep->lshift( b );
    _SC_FXVAL_OBSERVER_WRITE( *this )
    return *this;
}

inline
sc_fxval&
sc_fxval::operator >>= ( int b )
{
    _SC_FXVAL_OBSERVER_READ( *this )
    _rep->rshift( b );
    _SC_FXVAL_OBSERVER_WRITE( *this )
    return *this;
}


// auto-increment and auto-decrement

inline
const sc_fxval
sc_fxval::operator ++ ( int )
{
    sc_fxval c = *this;
    (*this) += 1;
    return c;
}

inline
const sc_fxval
sc_fxval::operator -- ( int )
{
    sc_fxval c = *this;
    (*this) -= 1;
    return c;
}

inline
sc_fxval&
sc_fxval::operator ++ ()
{
    (*this) += 1;
    return *this;
}

inline
sc_fxval&
sc_fxval::operator -- ()
{
    (*this) -= 1;
    return *this;
}


// implicit conversion

inline
sc_fxval::operator double() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->to_double();
}


// explicit conversion to primitive types

inline
short
sc_fxval::to_short() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<short>( _rep->to_double() );
}

inline
unsigned short
sc_fxval::to_ushort() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<unsigned short>( _rep->to_double() );
}

inline
int
sc_fxval::to_int() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<int>( _rep->to_double() );
}

inline
unsigned int
sc_fxval::to_uint() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<unsigned int>( _rep->to_double() );
}

inline
long
sc_fxval::to_long() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<long>( _rep->to_double() );
}

inline
unsigned long
sc_fxval::to_ulong() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<unsigned long>( _rep->to_double() );
}

inline
float
sc_fxval::to_float() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return static_cast<float>( _rep->to_double() );
}

inline
double
sc_fxval::to_double() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->to_double();
}


// query value

inline
bool
sc_fxval::is_neg() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->is_neg();
}

inline
bool
sc_fxval::is_zero() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->is_zero();
}

inline
bool
sc_fxval::is_nan() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->is_nan();
}

inline
bool
sc_fxval::is_inf() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->is_inf();
}

inline
bool
sc_fxval::is_normal() const
{
    _SC_FXVAL_OBSERVER_READ( *this )
    return _rep->is_normal();
}


inline
bool
sc_fxval::rounding_flag() const
{
    return _rep->rounding_flag();
}


// internal use only;
inline
bool
sc_fxval::get_bit( int i ) const
{
    return _rep->get_bit( i );
}


// protected methods and friend functions

inline
void
sc_fxval::get_type( int& wl, int& iwl, sc_enc& enc ) const
{
    _rep->get_type( wl, iwl, enc );
}


inline
const sc_fxval
sc_fxval::quantization( const scfx_params& params, bool& q_flag ) const
{
    return sc_fxval( ::quantization__scfx_rep( *_rep, params, q_flag ) );
}

inline
const sc_fxval
sc_fxval::overflow( const scfx_params& params, bool& o_flag ) const
{
    return sc_fxval( ::overflow__scfx_rep( *_rep, params, o_flag ) );
}


inline
ostream&
operator << ( ostream& os, const sc_fxval& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast
//
//  Fixed-point value type; limited precision.
// ----------------------------------------------------------------------------

// protected method

inline
sc_fxval_fast_observer*
sc_fxval_fast::observer() const
{
    return _observer;
}


// public constructors

inline
sc_fxval_fast::sc_fxval_fast( sc_fxval_fast_observer* observer_ )
: _val( 0.0 ),
  _observer( observer_ )
{
    _SC_FXVAL_FAST_OBSERVER_DEFAULT
    _SC_FXVAL_FAST_OBSERVER_CONSTRUCT( *this )
}

inline
sc_fxval_fast::sc_fxval_fast( const sc_fxval_fast& a,
			      sc_fxval_fast_observer* observer_ )
: _val( a._val ),
  _observer( observer_ )
{
    _SC_FXVAL_FAST_OBSERVER_DEFAULT
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    _SC_FXVAL_FAST_OBSERVER_CONSTRUCT( *this )
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
}

#define DEFN_CTOR_T(tp,arg)                                                   \
inline                                                                        \
sc_fxval_fast::sc_fxval_fast( tp a,                                           \
                              sc_fxval_fast_observer* observer_ )             \
: _val( arg ),                                                                \
  _observer( observer_ )                                                      \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_DEFAULT                                           \
    _SC_FXVAL_FAST_OBSERVER_CONSTRUCT( *this )                                \
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )                                    \
}

#define DEFN_CTOR_T_A(tp) DEFN_CTOR_T(tp,static_cast<double>( a ))
#define DEFN_CTOR_T_B(tp) DEFN_CTOR_T(tp,from_string( a ))
#define DEFN_CTOR_T_C(tp) DEFN_CTOR_T(tp,a.to_double())
#define DEFN_CTOR_T_D(tp) DEFN_CTOR_T(tp,static_cast<double>( a.num ))
#define DEFN_CTOR_T_E(tp) DEFN_CTOR_T(tp,static_cast<double>( static_cast<int64>( a.num ) ))

DEFN_CTOR_T_A(int)
DEFN_CTOR_T_A(unsigned int)
DEFN_CTOR_T_A(long)
DEFN_CTOR_T_A(unsigned long)
DEFN_CTOR_T_A(double)
DEFN_CTOR_T_B(const char*)
DEFN_CTOR_T_C(const sc_fxval&)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_CTOR_T_A(int64)
DEFN_CTOR_T_A(uint64)
DEFN_CTOR_T_D(const sc_int_base&)
#if defined( _MSC_VER ) && ! defined( _32BIT_ )
DEFN_CTOR_T_E(const sc_uint_base&)
#else
DEFN_CTOR_T_D(const sc_uint_base&)
#endif
DEFN_CTOR_T_C(const sc_signed&)
DEFN_CTOR_T_C(const sc_unsigned&)
#endif

#undef DEFN_CTOR_T
#undef DEFN_CTOR_T_A
#undef DEFN_CTOR_T_B
#undef DEFN_CTOR_T_C
#undef DEFN_CTOR_T_D
#undef DEFN_CTOR_T_E


inline
sc_fxval_fast::~sc_fxval_fast()
{
    _SC_FXVAL_FAST_OBSERVER_DESTRUCT( *this )
}


// internal use only;
inline
double
sc_fxval_fast::get_val() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return _val;
}

// internal use only;
inline
void
sc_fxval_fast::set_val( double val_ )
{
    _val = val_;
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
}


// unary operators

inline
const sc_fxval_fast
sc_fxval_fast::operator - () const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return sc_fxval_fast( - _val );
}

inline
const sc_fxval_fast&
sc_fxval_fast::operator + () const
{
    // _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return *this;
}


// unary functions

inline
void
neg( sc_fxval_fast& c, const sc_fxval_fast& a )
{
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    c._val = - a._val;
    _SC_FXVAL_FAST_OBSERVER_WRITE( c )
}


// binary operators

#define DEFN_BIN_OP_T(op,tp)                                                  \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    return sc_fxval_fast( a._val op tmp._val );                               \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    return sc_fxval_fast( tmp._val op b._val );                               \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_BIN_OP_OTHER(op)                                                 \
DEFN_BIN_OP_T(op,int64)                                                       \
DEFN_BIN_OP_T(op,uint64)                                                      \
DEFN_BIN_OP_T(op,const sc_int_base&)                                          \
DEFN_BIN_OP_T(op,const sc_uint_base&)                                         \
DEFN_BIN_OP_T(op,const sc_signed&)                                            \
DEFN_BIN_OP_T(op,const sc_unsigned&)
#else
#define DEFN_BIN_OP_OTHER(op)
#endif

#define DEFN_BIN_OP(op,dummy)                                                 \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, const sc_fxval_fast& b )                \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( a )                                         \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    return sc_fxval_fast( a._val op b._val );                                 \
}                                                                             \
                                                                              \
DEFN_BIN_OP_T(op,int)                                                         \
DEFN_BIN_OP_T(op,unsigned int)                                                \
DEFN_BIN_OP_T(op,long)                                                        \
DEFN_BIN_OP_T(op,unsigned long)                                               \
DEFN_BIN_OP_T(op,double)                                                      \
DEFN_BIN_OP_T(op,const char*)                                                 \
DEFN_BIN_OP_OTHER(op)

DEFN_BIN_OP(*,mult)
DEFN_BIN_OP(+,add)
DEFN_BIN_OP(-,sub)
//DEFN_BIN_OP(/,div)
inline
const sc_fxval_fast
operator / ( const sc_fxval_fast& a, const sc_fxval_fast& b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    _SC_FXVAL_FAST_OBSERVER_READ( b )
    return sc_fxval_fast( a._val / b._val );
}

DEFN_BIN_OP_T(/,int)
DEFN_BIN_OP_T(/,unsigned int)
DEFN_BIN_OP_T(/,long)
DEFN_BIN_OP_T(/,unsigned long)
DEFN_BIN_OP_T(/,double)
DEFN_BIN_OP_T(/,const char*)
//DEFN_BIN_OP_OTHER(/)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_BIN_OP_T(/,int64)
DEFN_BIN_OP_T(/,uint64)
DEFN_BIN_OP_T(/,const sc_int_base&)
DEFN_BIN_OP_T(/,const sc_uint_base&)
DEFN_BIN_OP_T(/,const sc_signed&)
DEFN_BIN_OP_T(/,const sc_unsigned&)
#endif


#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP_OTHER
#undef DEFN_BIN_OP


inline
const sc_fxval_fast
operator << ( const sc_fxval_fast& a, int b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    return sc_fxval_fast( a._val * scfx_pow2( b ) );
}

inline
const sc_fxval_fast
operator >> ( const sc_fxval_fast& a, int b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    return sc_fxval_fast( a._val * scfx_pow2( -b ) );
}


// binary functions

#define DEFN_BIN_FNC_T(fnc,op,tp)                                             \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, tp b )                        \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    c._val = a._val op tmp._val;                                              \
    _SC_FXVAL_FAST_OBSERVER_WRITE( c )                                        \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxval_fast& b )                        \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    c._val = tmp._val op b._val;                                              \
    _SC_FXVAL_FAST_OBSERVER_WRITE( c )                                        \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_BIN_FNC_OTHER(fnc,op)                                            \
DEFN_BIN_FNC_T(fnc,op,int64)                                                  \
DEFN_BIN_FNC_T(fnc,op,uint64)                                                 \
DEFN_BIN_FNC_T(fnc,op,const sc_int_base&)                                     \
DEFN_BIN_FNC_T(fnc,op,const sc_uint_base&)                                    \
DEFN_BIN_FNC_T(fnc,op,const sc_signed&)                                       \
DEFN_BIN_FNC_T(fnc,op,const sc_unsigned&)
#else
#define DEFN_BIN_FNC_OTHER(fnc,op)
#endif

#define DEFN_BIN_FNC(fnc,op)                                                  \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, const sc_fxval_fast& b )      \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( a )                                         \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    c._val = a._val op b._val;                                                \
    _SC_FXVAL_FAST_OBSERVER_WRITE( c )                                        \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T(fnc,op,int)                                                    \
DEFN_BIN_FNC_T(fnc,op,unsigned int)                                           \
DEFN_BIN_FNC_T(fnc,op,long)                                                   \
DEFN_BIN_FNC_T(fnc,op,unsigned long)                                          \
DEFN_BIN_FNC_T(fnc,op,double)                                                 \
DEFN_BIN_FNC_T(fnc,op,const char*)                                            \
DEFN_BIN_FNC_OTHER(fnc,op)

DEFN_BIN_FNC(mult,*)
DEFN_BIN_FNC(div,/)
DEFN_BIN_FNC(add,+)
DEFN_BIN_FNC(sub,-)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC_OTHER
#undef DEFN_BIN_FNC


inline
void
lshift( sc_fxval_fast& c, const sc_fxval_fast& a, int b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    c._val = a._val * scfx_pow2( b );
    _SC_FXVAL_FAST_OBSERVER_WRITE( c )
}

inline
void
rshift( sc_fxval_fast& c, const sc_fxval_fast& a, int b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( a )
    c._val = a._val * scfx_pow2( -b );
    _SC_FXVAL_FAST_OBSERVER_WRITE( c )
}


// relational (including equality) operators

#define DEFN_REL_OP_T(op,tp)                                                  \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, tp b )                                  \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    return ( a._val op tmp._val );                                            \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxval_fast& b )                                  \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    return ( tmp._val op b._val );                                            \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_REL_OP_OTHER(op)                                                 \
DEFN_REL_OP_T(op,int64)                                                       \
DEFN_REL_OP_T(op,uint64)                                                      \
DEFN_REL_OP_T(op,const sc_int_base&)                                          \
DEFN_REL_OP_T(op,const sc_uint_base&)                                         \
DEFN_REL_OP_T(op,const sc_signed&)                                            \
DEFN_REL_OP_T(op,const sc_unsigned&)
#else
#define DEFN_REL_OP_OTHER(op)
#endif

#define DEFN_REL_OP(op)                                                       \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, const sc_fxval_fast& b )                \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( a )                                         \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    return ( a._val op b._val );                                              \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,int)                                                         \
DEFN_REL_OP_T(op,unsigned int)                                                \
DEFN_REL_OP_T(op,long)                                                        \
DEFN_REL_OP_T(op,unsigned long)                                               \
DEFN_REL_OP_T(op,double)                                                      \
DEFN_REL_OP_T(op,const char*)                                                 \
DEFN_REL_OP_OTHER(op)

DEFN_REL_OP(<)
DEFN_REL_OP(<=)
DEFN_REL_OP(>)
DEFN_REL_OP(>=)
DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP_OTHER
#undef DEFN_REL_OP


// assignment operators

inline
sc_fxval_fast&
sc_fxval_fast::operator = ( const sc_fxval_fast& a )
{
    if( &a != this )
    {
	_SC_FXVAL_FAST_OBSERVER_READ( a )
	_val = a._val;
	_SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    }
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator = ( tp a )                                            \
{                                                                             \
    sc_fxval_fast tmp( a );                                                   \
    _val = tmp._val;                                                          \
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}

DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(double)
DEFN_ASN_OP_T(const char*)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_ASN_OP_T(int64)
DEFN_ASN_OP_T(uint64)
DEFN_ASN_OP_T(const sc_int_base&)
DEFN_ASN_OP_T(const sc_uint_base&)
DEFN_ASN_OP_T(const sc_signed&)
DEFN_ASN_OP_T(const sc_unsigned&)
#endif

#undef DEFN_ASN_OP_T


#define DEFN_ASN_OP_T(op,tp)                                                  \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( tp b )                                           \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( *this )                                     \
    sc_fxval_fast tmp( b );                                                   \
    _val op tmp._val;                                                         \
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}

#ifndef SC_FX_EXCLUDE_OTHER
#define DEFN_ASN_OP_OTHER(op)                                                 \
DEFN_ASN_OP_T(op,int64)                                                       \
DEFN_ASN_OP_T(op,uint64)                                                      \
DEFN_ASN_OP_T(op,const sc_int_base&)                                          \
DEFN_ASN_OP_T(op,const sc_uint_base&)                                         \
DEFN_ASN_OP_T(op,const sc_signed&)                                            \
DEFN_ASN_OP_T(op,const sc_unsigned&)
#else
#define DEFN_ASN_OP_OTHER(op)
#endif

#define DEFN_ASN_OP(op)                                                       \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( const sc_fxval_fast& b )                         \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( *this )                                     \
    _SC_FXVAL_FAST_OBSERVER_READ( b )                                         \
    _val op b._val;                                                           \
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,int)                                                         \
DEFN_ASN_OP_T(op,unsigned int)                                                \
DEFN_ASN_OP_T(op,long)                                                        \
DEFN_ASN_OP_T(op,unsigned long)                                               \
DEFN_ASN_OP_T(op,double)                                                      \
DEFN_ASN_OP_T(op,const char*)                                                 \
DEFN_ASN_OP_OTHER(op)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP_OTHER
#undef DEFN_ASN_OP


inline
sc_fxval_fast&
sc_fxval_fast::operator <<= ( int b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    _val *= scfx_pow2( b );
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    return *this;
}

inline
sc_fxval_fast&
sc_fxval_fast::operator >>= ( int b )
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    _val *= scfx_pow2( -b );
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    return *this;
}


// auto-increment and auto-decrement

inline
const sc_fxval_fast
sc_fxval_fast::operator ++ ( int )
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    double c = _val;
    _val = _val + 1;
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    return sc_fxval_fast( c );
}

inline
const sc_fxval_fast
sc_fxval_fast::operator -- ( int )
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    double c = _val;
    _val = _val - 1;
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    return sc_fxval_fast( c );
}

inline
sc_fxval_fast&
sc_fxval_fast::operator ++ ()
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    _val = _val + 1;
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    return *this;
}

inline
sc_fxval_fast&
sc_fxval_fast::operator -- ()
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    _val = _val - 1;
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
    return *this;
}


// implicit conversion

inline
sc_fxval_fast::operator double() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return _val;
}


// explicit conversion to primitive types

inline
short
sc_fxval_fast::to_short() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<short>( _val );
}

inline
unsigned short
sc_fxval_fast::to_ushort() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<unsigned short>( _val );
}

inline
int
sc_fxval_fast::to_int() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<int>( _val );
}

inline
unsigned int
sc_fxval_fast::to_uint() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<unsigned int>( _val );
}

inline
long
sc_fxval_fast::to_long() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<long>( _val );
}

inline
unsigned long
sc_fxval_fast::to_ulong() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<unsigned long>( _val );
}

inline
float
sc_fxval_fast::to_float() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return static_cast<float>( _val );
}

inline
double
sc_fxval_fast::to_double() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    return _val;
}


// query value

inline
bool
sc_fxval_fast::is_neg() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return ( id.negative() != 0 );
}

inline
bool
sc_fxval_fast::is_zero() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return id.is_zero();
}

inline
bool
sc_fxval_fast::is_nan() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return id.is_nan();
}

inline
bool
sc_fxval_fast::is_inf() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return id.is_inf();
}

inline
bool
sc_fxval_fast::is_normal() const
{
    _SC_FXVAL_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return ( id.is_normal() || id.is_subnormal() || id.is_zero() );
}


inline
bool
sc_fxval_fast::rounding_flag() const
{
    // does not apply to sc_fxval_fast; included for API compatibility
    return false;
}


inline
ostream&
operator << ( ostream& os, const sc_fxval_fast& a )
{
    a.print( os );
    return os;
}


#endif

// Taf!
