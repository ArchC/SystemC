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

    sc_fxnum.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXNUM_H
#define SC_FXNUM_H


#include "../../sc_lv.h"
#include "sc_fxval.h"
#include "scfx_params.h"
#include "sc_fxnum_observer.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif

using sc_bv_ns::sc_lv_base;

#if defined(__HP_aCC)
// ATTN! aCC on HPUX platform doesn't implement Koenig lookup
// opening up namespace seems to solve the problem
using namespace sc_bv_ns;
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_bitref
//
//  Proxy class for bit-selection in class sc_fxnum, behaves like sc_bit.
// ----------------------------------------------------------------------------

class sc_fxnum_bitref
{

    friend class sc_fxnum;
    friend class sc_fxnum_fast_bitref;

    bool get() const;
    void set( bool );


    // constructor

    sc_fxnum_bitref( sc_fxnum&, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxnum_bitref& operator op ( tp );

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,const sc_fxnum_bitref&)                                  \
    DECL_ASN_OP_T(op,const sc_fxnum_fast_bitref&)                             \
    DECL_ASN_OP_T(op,const sc_bit&)                                           \
    DECL_ASN_OP_T(op,bool)

    DECL_ASN_OP(=)

    DECL_ASN_OP(&=)
    DECL_ASN_OP(|=)
    DECL_ASN_OP(^=)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP


    // implicit conversion

    operator bool() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum& _num;
    int       _idx;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_bitref
//
//  Proxy class for bit-selection in class sc_fxnum_fast, behaves like sc_bit.
// ----------------------------------------------------------------------------

class sc_fxnum_fast_bitref
{

    friend class sc_fxnum_fast;
    friend class sc_fxnum_bitref;

    bool get() const;
    void set( bool );


    // constructor

    sc_fxnum_fast_bitref( sc_fxnum_fast&, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(op,tp)                                                  \
    sc_fxnum_fast_bitref& operator op ( tp );

#define DECL_ASN_OP(op)                                                       \
    DECL_ASN_OP_T(op,const sc_fxnum_bitref&)                                  \
    DECL_ASN_OP_T(op,const sc_fxnum_fast_bitref&)                             \
    DECL_ASN_OP_T(op,const sc_bit&)                                           \
    DECL_ASN_OP_T(op,bool)

    DECL_ASN_OP(=)

    DECL_ASN_OP(&=)
    DECL_ASN_OP(|=)
    DECL_ASN_OP(^=)

#undef DECL_ASN_OP_T
#undef DECL_ASN_OP


    // implicit conversion

    operator bool() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum_fast& _num;
    int            _idx;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_subref
//
//  Proxy class for part-selection in class sc_fxnum,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

class sc_fxnum_subref
{

    friend class sc_fxnum;
    friend class sc_fxnum_fast_subref;

    bool get() const;
    bool set();


    // constructor

    sc_fxnum_subref( sc_fxnum&, int, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(tp)                                                     \
    sc_fxnum_subref& operator = ( tp );

    DECL_ASN_OP_T(const sc_fxnum_subref&)
    DECL_ASN_OP_T(const sc_fxnum_fast_subref&)
    DECL_ASN_OP_T(const sc_bv_base&)
    DECL_ASN_OP_T(const sc_lv_base&)
    DECL_ASN_OP_T(const sc_bool_vector&)
    DECL_ASN_OP_T(const char*)
    DECL_ASN_OP_T(const bool*)
    DECL_ASN_OP_T(const sc_signed&)
    DECL_ASN_OP_T(const sc_unsigned&)
    DECL_ASN_OP_T(const sc_int_base&)
    DECL_ASN_OP_T(const sc_uint_base&)
    DECL_ASN_OP_T(int64)
    DECL_ASN_OP_T(uint64)
    DECL_ASN_OP_T(int)
    DECL_ASN_OP_T(unsigned int)
    DECL_ASN_OP_T(long)
    DECL_ASN_OP_T(unsigned long)
    DECL_ASN_OP_T(char)

#undef DECL_ASN_OP_T

#define DECL_ASN_OP_T_A(op,tp)                                                \
    sc_fxnum_subref& operator op ## = ( tp );

#define DECL_ASN_OP_A(op)                                                     \
    DECL_ASN_OP_T_A(op,const sc_fxnum_subref&)                                \
    DECL_ASN_OP_T_A(op,const sc_fxnum_fast_subref&)                           \
    DECL_ASN_OP_T_A(op,const sc_bv_base&)                                     \
    DECL_ASN_OP_T_A(op,const sc_lv_base&)

    DECL_ASN_OP_A(&)
    DECL_ASN_OP_A(|)
    DECL_ASN_OP_A(^)

#undef DECL_ASN_OP_T_A
#undef DECL_ASN_OP_A


    // relational operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum_subref&, tp );                   \
    friend bool operator op ( tp, const sc_fxnum_subref& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxnum_subref&,                         \
			      const sc_fxnum_subref& );                       \
    friend bool operator op ( const sc_fxnum_subref&,                         \
			      const sc_fxnum_fast_subref& );                  \
    DECL_REL_OP_T(op,const sc_bv_base&)                                       \
    DECL_REL_OP_T(op,const sc_lv_base&)                                       \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const bool*)                                             \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)                                      \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)

    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP


    // reduce functions

    bool and_reduce() const;
    bool nand_reduce() const;
    bool or_reduce() const;
    bool nor_reduce() const;
    bool xor_reduce() const;
    bool xnor_reduce() const;


    // query parameter

    int length() const;


    // explicit conversions

    int           to_signed() const;
    unsigned int  to_unsigned() const;
    long          to_long() const;
    unsigned long to_ulong() const;

    const sc_string to_string() const;


    // implicit conversion

    operator sc_bv_base() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum& _num;
    int       _from;
    int       _to;

    sc_bv_base& _bv;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_subref
//
//  Proxy class for part-selection in class sc_fxnum_fast,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

class sc_fxnum_fast_subref
{

    friend class sc_fxnum_fast;
    friend class sc_fxnum_subref;

    bool get() const;
    bool set();


    // constructor

    sc_fxnum_fast_subref( sc_fxnum_fast&, int, int );

public:

    // assignment operators

#define DECL_ASN_OP_T(tp)                                                     \
    sc_fxnum_fast_subref& operator = ( tp );

    DECL_ASN_OP_T(const sc_fxnum_subref&)
    DECL_ASN_OP_T(const sc_fxnum_fast_subref&)
    DECL_ASN_OP_T(const sc_bv_base&)
    DECL_ASN_OP_T(const sc_lv_base&)
    DECL_ASN_OP_T(const sc_bool_vector&)
    DECL_ASN_OP_T(const char*)
    DECL_ASN_OP_T(const bool*)
    DECL_ASN_OP_T(const sc_signed&)
    DECL_ASN_OP_T(const sc_unsigned&)
    DECL_ASN_OP_T(const sc_int_base&)
    DECL_ASN_OP_T(const sc_uint_base&)
    DECL_ASN_OP_T(int64)
    DECL_ASN_OP_T(uint64)
    DECL_ASN_OP_T(int)
    DECL_ASN_OP_T(unsigned int)
    DECL_ASN_OP_T(long)
    DECL_ASN_OP_T(unsigned long)
    DECL_ASN_OP_T(char)

#undef DECL_ASN_OP_T

#define DECL_ASN_OP_T_A(op,tp)                                                \
    sc_fxnum_fast_subref& operator op ## = ( tp );

#define DECL_ASN_OP_A(op)                                                     \
    DECL_ASN_OP_T_A(op,const sc_fxnum_subref&)                                \
    DECL_ASN_OP_T_A(op,const sc_fxnum_fast_subref&)                           \
    DECL_ASN_OP_T_A(op,const sc_bv_base&)                                     \
    DECL_ASN_OP_T_A(op,const sc_lv_base&)

    DECL_ASN_OP_A(&)
    DECL_ASN_OP_A(|)
    DECL_ASN_OP_A(^)

#undef DECL_ASN_OP_T_A
#undef DECL_ASN_OP_A


    // relational operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum_fast_subref&, tp );              \
    friend bool operator op ( tp, const sc_fxnum_fast_subref& );

#define DECL_REL_OP(op)                                                       \
    friend bool operator op ( const sc_fxnum_fast_subref&,                    \
			      const sc_fxnum_fast_subref& );                  \
    friend bool operator op ( const sc_fxnum_fast_subref&,                    \
			      const sc_fxnum_subref& );                       \
    DECL_REL_OP_T(op,const sc_bv_base&)                                       \
    DECL_REL_OP_T(op,const sc_lv_base&)                                       \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const bool*)                                             \
    DECL_REL_OP_T(op,const sc_signed&)                                        \
    DECL_REL_OP_T(op,const sc_unsigned&)                                      \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)

    DECL_REL_OP(==)
    DECL_REL_OP(!=)

#undef DECL_REL_OP_T
#undef DECL_REL_OP


    // reduce functions

    bool and_reduce() const;
    bool nand_reduce() const;
    bool or_reduce() const;
    bool nor_reduce() const;
    bool xor_reduce() const;
    bool xnor_reduce() const;


    // query parameter

    int length() const;


    // explicit conversions

    int           to_signed() const;
    unsigned int  to_unsigned() const;
    long          to_long() const;
    unsigned long to_ulong() const;

    const sc_string to_string() const;


    // implicit conversion

    operator sc_bv_base() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;

private:

    sc_fxnum_fast& _num;
    int            _from;
    int            _to;

    sc_bv_base& _bv;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum
//
//  Base class for the fixed-point types; arbitrary precision.
// ----------------------------------------------------------------------------

class sc_fxnum
{

    friend class sc_fxval;

    friend class sc_fxnum_bitref;
    friend class sc_fxnum_subref;
    friend class sc_fxnum_fast_bitref;
    friend class sc_fxnum_fast_subref;

    friend class isdb_sc_fxnum_trace;
    friend class vcd_sc_fxnum_trace;
    friend class wif_sc_fxnum_trace;

protected:

    sc_fxnum_observer* observer() const;


    void cast();


    // constructors

    sc_fxnum( const sc_fxtype_params&,
	      sc_enc,
	      const sc_fxcast_switch&,
	      sc_fxnum_observer* );

#define DECL_CTOR_T(tp)                                                       \
    sc_fxnum( tp,                                                             \
	      const sc_fxtype_params&,                                        \
	      sc_enc,                                                         \
	      const sc_fxcast_switch&,                                        \
	      sc_fxnum_observer* );

    DECL_CTOR_T(int)
    DECL_CTOR_T(unsigned int)
    DECL_CTOR_T(long)
    DECL_CTOR_T(unsigned long)
    DECL_CTOR_T(double)
    DECL_CTOR_T(const char*)
    DECL_CTOR_T(const sc_fxval&)
    DECL_CTOR_T(const sc_fxval_fast&)
    DECL_CTOR_T(const sc_fxnum&)
    DECL_CTOR_T(const sc_fxnum_fast&)
#ifndef SC_FX_EXCLUDE_OTHER
    DECL_CTOR_T(int64)
    DECL_CTOR_T(uint64)
    DECL_CTOR_T(const sc_int_base&)
    DECL_CTOR_T(const sc_uint_base&)
    DECL_CTOR_T(const sc_signed&)
    DECL_CTOR_T(const sc_unsigned&)
#endif

#undef DECL_CTOR_T

    ~sc_fxnum();


    // internal use only;
    const scfx_rep* get_rep() const;

public:

    // unary operators

    const sc_fxval operator - () const;
    const sc_fxval operator + () const;


    // unary functions

    friend void neg( sc_fxval&, const sc_fxnum& );
    friend void neg( sc_fxnum&, const sc_fxnum& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval operator op ( const sc_fxnum&, tp );                \
    friend const sc_fxval operator op ( tp, const sc_fxnum& );

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
    friend const sc_fxval operator op ( const sc_fxnum&, const sc_fxnum& );   \
    DECL_BIN_OP_T(op,int)                                                     \
    DECL_BIN_OP_T(op,unsigned int)                                            \
    DECL_BIN_OP_T(op,long)                                                    \
    DECL_BIN_OP_T(op,unsigned long)                                           \
    DECL_BIN_OP_T(op,double)                                                  \
    DECL_BIN_OP_T(op,const char*)                                             \
    DECL_BIN_OP_T(op,const sc_fxval&)                                         \
    DECL_BIN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_BIN_OP_T(op,const sc_fxnum_fast&)                                    \
    DECL_BIN_OP_OTHER(op)

    DECL_BIN_OP(*,mult)
    DECL_BIN_OP(+,add)
    DECL_BIN_OP(-,sub)
// don't use macros
//    DECL_BIN_OP(/,div)
    friend const sc_fxval operator / ( const sc_fxnum&, const sc_fxnum& );
    DECL_BIN_OP_T(/,int)
    DECL_BIN_OP_T(/,unsigned int)
    DECL_BIN_OP_T(/,long)
    DECL_BIN_OP_T(/,unsigned long)
    DECL_BIN_OP_T(/,double)
    DECL_BIN_OP_T(/,const char*)
    DECL_BIN_OP_T(/,const sc_fxval&)
    DECL_BIN_OP_T(/,const sc_fxval_fast&)
    DECL_BIN_OP_T(/,const sc_fxnum_fast&)
//    DECL_BIN_OP_OTHER(op)
#ifndef SC_FX_EXCLUDE_OTHER
    DECL_BIN_OP_T(/,int64)
    DECL_BIN_OP_T(/,uint64)
    DECL_BIN_OP_T(/,const sc_int_base&)
    DECL_BIN_OP_T(/,const sc_uint_base&)
    DECL_BIN_OP_T(/,const sc_signed&)
    DECL_BIN_OP_T(/,const sc_unsigned&)
#endif

#undef DECL_BIN_OP_T
#undef DECL_BIN_OP_OTHER
#undef DECL_BIN_OP

    friend const sc_fxval operator << ( const sc_fxnum&, int );
    friend const sc_fxval operator >> ( const sc_fxnum&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval&, const sc_fxnum&, tp );                       \
    friend void fnc ( sc_fxval&, tp, const sc_fxnum& );                       \
    friend void fnc ( sc_fxnum&, const sc_fxnum&, tp );                       \
    friend void fnc ( sc_fxnum&, tp, const sc_fxnum& );

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
    friend void fnc ( sc_fxval&, const sc_fxnum&, const sc_fxnum& );          \
    friend void fnc ( sc_fxnum&, const sc_fxnum&, const sc_fxnum& );          \
    DECL_BIN_FNC_T(fnc,int)                                                   \
    DECL_BIN_FNC_T(fnc,unsigned int)                                          \
    DECL_BIN_FNC_T(fnc,long)                                                  \
    DECL_BIN_FNC_T(fnc,unsigned long)                                         \
    DECL_BIN_FNC_T(fnc,double)                                                \
    DECL_BIN_FNC_T(fnc,const char*)                                           \
    DECL_BIN_FNC_T(fnc,const sc_fxval&)                                       \
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

    friend void lshift( sc_fxval&, const sc_fxnum&, int );
    friend void rshift( sc_fxval&, const sc_fxnum&, int );
    friend void lshift( sc_fxnum&, const sc_fxnum&, int );
    friend void rshift( sc_fxnum&, const sc_fxnum&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum&, tp );                          \
    friend bool operator op ( tp, const sc_fxnum& );

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
    friend bool operator op ( const sc_fxnum&, const sc_fxnum& );             \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)                                           \
    DECL_REL_OP_T(op,double)                                                  \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const sc_fxval&)                                         \
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
    sc_fxnum& operator op( tp );

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

    sc_fxnum& operator ++ ();
    sc_fxnum& operator -- ();


    // bit selection

    const sc_fxnum_bitref operator [] ( int ) const;
    sc_fxnum_bitref       operator [] ( int );

    const sc_fxnum_bitref bit( int ) const;
    sc_fxnum_bitref       bit( int );


    // part selection

    const sc_fxnum_subref operator () ( int, int ) const;
    sc_fxnum_subref       operator () ( int, int );

    const sc_fxnum_subref range( int, int ) const;
    sc_fxnum_subref       range( int, int );


    const sc_fxnum_subref operator () () const;
    sc_fxnum_subref       operator () ();

    const sc_fxnum_subref range() const;
    sc_fxnum_subref       range();


    // implicit conversion

    operator double() const;		// necessary evil!


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

    // internal use only;
    bool is_normal() const;

    bool quantization_flag() const;
    bool overflow_flag() const;

    const sc_fxval value() const;


    // query parameters

    int       wl() const;
    int       iwl() const;
    sc_q_mode q_mode() const;
    sc_o_mode o_mode() const;
    int       n_bits() const;

    const sc_fxtype_params& type_params() const;

    const sc_fxcast_switch& cast_switch() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;


    // internal use only;
    void observer_read() const;


    // internal use only;
    bool get_bit( int ) const;

protected:

    bool set_bit( int, bool );


    bool get_slice( int, int, sc_bv_base& ) const;
    bool set_slice( int, int, const sc_bv_base& );


    sc_fxnum_observer* lock_observer() const;
    void unlock_observer( sc_fxnum_observer* ) const;

private:

    scfx_rep*                  _rep;

    scfx_params                _params;
    bool                       _q_flag;
    bool                       _o_flag;

    mutable sc_fxnum_observer* _observer;

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast
//
//  Base class for the fixed-point types; limited precision.
// ----------------------------------------------------------------------------

class sc_fxnum_fast
{

    friend class sc_fxval_fast;

    friend class sc_fxnum_bitref;
    friend class sc_fxnum_subref;
    friend class sc_fxnum_fast_bitref;
    friend class sc_fxnum_fast_subref;

    friend class isdb_sc_fxnum_fast_trace;
    friend class vcd_sc_fxnum_fast_trace;
    friend class wif_sc_fxnum_fast_trace;

protected:

    sc_fxnum_fast_observer* observer() const;


    void cast();


    // constructors

    sc_fxnum_fast( const sc_fxtype_params&,
		   sc_enc,
		   const sc_fxcast_switch&,
		   sc_fxnum_fast_observer* );

#define DECL_CTOR_T(tp)                                                       \
    sc_fxnum_fast( tp,                                                        \
	           const sc_fxtype_params&,                                   \
	           sc_enc,                                                    \
	           const sc_fxcast_switch&,                                   \
	           sc_fxnum_fast_observer* );

    DECL_CTOR_T(int)
    DECL_CTOR_T(unsigned int)
    DECL_CTOR_T(long)
    DECL_CTOR_T(unsigned long)
    DECL_CTOR_T(double)
    DECL_CTOR_T(const char*)
    DECL_CTOR_T(const sc_fxval&)
    DECL_CTOR_T(const sc_fxval_fast&)
    DECL_CTOR_T(const sc_fxnum&)
    DECL_CTOR_T(const sc_fxnum_fast&)
#ifndef SC_FX_EXCLUDE_OTHER
    DECL_CTOR_T(int64)
    DECL_CTOR_T(uint64)
    DECL_CTOR_T(const sc_int_base&)
    DECL_CTOR_T(const sc_uint_base&)
    DECL_CTOR_T(const sc_signed&)
    DECL_CTOR_T(const sc_unsigned&)
#endif

#undef DECL_CTOR_T

    ~sc_fxnum_fast();


    // internal use only;
    double get_val() const;

public:

    // unary operators

    const sc_fxval_fast operator - () const;
    const sc_fxval_fast operator + () const;


    // unary functions

    friend void neg( sc_fxval_fast&, const sc_fxnum_fast& );
    friend void neg( sc_fxnum_fast&, const sc_fxnum_fast& );


    // binary operators

#define DECL_BIN_OP_T(op,tp)                                                  \
    friend const sc_fxval_fast operator op ( const sc_fxnum_fast&, tp );      \
    friend const sc_fxval_fast operator op ( tp, const sc_fxnum_fast& );

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
    friend const sc_fxval_fast operator op ( const sc_fxnum_fast&,            \
					     const sc_fxnum_fast& );          \
    DECL_BIN_OP_T(op,int)                                                     \
    DECL_BIN_OP_T(op,unsigned int)                                            \
    DECL_BIN_OP_T(op,long)                                                    \
    DECL_BIN_OP_T(op,unsigned long)                                           \
    DECL_BIN_OP_T(op,double)                                                  \
    DECL_BIN_OP_T(op,const char*)                                             \
    DECL_BIN_OP_T(op,const sc_fxval_fast&)                                    \
    DECL_BIN_OP_OTHER(op)

    DECL_BIN_OP(*,mult)
    DECL_BIN_OP(+,add)
    DECL_BIN_OP(-,sub)
//    DECL_BIN_OP(/,div)
    friend const sc_fxval_fast operator / ( const sc_fxnum_fast&,
					     const sc_fxnum_fast& );
    DECL_BIN_OP_T(/,int)
    DECL_BIN_OP_T(/,unsigned int)
    DECL_BIN_OP_T(/,long)
    DECL_BIN_OP_T(/,unsigned long)
    DECL_BIN_OP_T(/,double)
    DECL_BIN_OP_T(/,const char*)
    DECL_BIN_OP_T(/,const sc_fxval_fast&)
//    DECL_BIN_OP_OTHER(op)
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

    friend const sc_fxval_fast operator << ( const sc_fxnum_fast&, int );
    friend const sc_fxval_fast operator >> ( const sc_fxnum_fast&, int );


    // binary functions

#define DECL_BIN_FNC_T(fnc,tp)                                                \
    friend void fnc ( sc_fxval_fast&, const sc_fxnum_fast&, tp );             \
    friend void fnc ( sc_fxval_fast&, tp, const sc_fxnum_fast& );             \
    friend void fnc ( sc_fxnum_fast&, const sc_fxnum_fast&, tp );             \
    friend void fnc ( sc_fxnum_fast&, tp, const sc_fxnum_fast& );

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
    friend void fnc ( sc_fxval_fast&, const sc_fxnum_fast&,                   \
		                      const sc_fxnum_fast& );                 \
    friend void fnc ( sc_fxnum_fast&, const sc_fxnum_fast&,                   \
		                      const sc_fxnum_fast& );                 \
    DECL_BIN_FNC_T(fnc,int)                                                   \
    DECL_BIN_FNC_T(fnc,unsigned int)                                          \
    DECL_BIN_FNC_T(fnc,long)                                                  \
    DECL_BIN_FNC_T(fnc,unsigned long)                                         \
    DECL_BIN_FNC_T(fnc,double)                                                \
    DECL_BIN_FNC_T(fnc,const char*)                                           \
    DECL_BIN_FNC_T(fnc,const sc_fxval&)                                       \
    DECL_BIN_FNC_T(fnc,const sc_fxval_fast&)                                  \
    DECL_BIN_FNC_T(fnc,const sc_fxnum&)                                       \
    DECL_BIN_FNC_OTHER(fnc)

    DECL_BIN_FNC(mult)
    DECL_BIN_FNC(div)
    DECL_BIN_FNC(add)
    DECL_BIN_FNC(sub)

#undef DECL_BIN_FNC_T
#undef DECL_BIN_FNC_OTHER
#undef DECL_BIN_FNC

    friend void lshift( sc_fxval_fast&, const sc_fxnum_fast&, int );
    friend void rshift( sc_fxval_fast&, const sc_fxnum_fast&, int );
    friend void lshift( sc_fxnum_fast&, const sc_fxnum_fast&, int );
    friend void rshift( sc_fxnum_fast&, const sc_fxnum_fast&, int );


    // relational (including equality) operators

#define DECL_REL_OP_T(op,tp)                                                  \
    friend bool operator op ( const sc_fxnum_fast&, tp );                     \
    friend bool operator op ( tp, const sc_fxnum_fast& );

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
    friend bool operator op ( const sc_fxnum_fast&, const sc_fxnum_fast& );   \
    DECL_REL_OP_T(op,int)                                                     \
    DECL_REL_OP_T(op,unsigned int)                                            \
    DECL_REL_OP_T(op,long)                                                    \
    DECL_REL_OP_T(op,unsigned long)                                           \
    DECL_REL_OP_T(op,double)                                                  \
    DECL_REL_OP_T(op,const char*)                                             \
    DECL_REL_OP_T(op,const sc_fxval_fast&)                                    \
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
    sc_fxnum_fast& operator op( tp );

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

    sc_fxnum_fast& operator ++ ();
    sc_fxnum_fast& operator -- ();


    // bit selection

    const sc_fxnum_fast_bitref operator [] ( int ) const;
    sc_fxnum_fast_bitref       operator [] ( int );

    const sc_fxnum_fast_bitref bit( int ) const;
    sc_fxnum_fast_bitref       bit( int );


    // part selection

    const sc_fxnum_fast_subref operator () ( int, int ) const;
    sc_fxnum_fast_subref       operator () ( int, int );

    const sc_fxnum_fast_subref range( int, int ) const;
    sc_fxnum_fast_subref       range( int, int );


    const sc_fxnum_fast_subref operator () () const;
    sc_fxnum_fast_subref       operator () ();

    const sc_fxnum_fast_subref range() const;
    sc_fxnum_fast_subref       range();


    // implicit conversion

    operator double() const;		// necessary evil!


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

    // internal use only;
    bool is_normal() const;

    bool quantization_flag() const;
    bool overflow_flag() const;

    const sc_fxval_fast value() const;


    // query parameters

    int       wl() const;
    int       iwl() const;
    sc_q_mode q_mode() const;
    sc_o_mode o_mode() const;
    int       n_bits() const;

    const sc_fxtype_params& type_params() const;

    const sc_fxcast_switch& cast_switch() const;


    // print or dump content

    void print( ostream& = cout ) const;
    void dump( ostream& = cout ) const;


    // internal use only;
    void observer_read() const;


    // internal use only;
    bool get_bit( int ) const;

protected:

    bool set_bit( int, bool );


    bool get_slice( int, int, sc_bv_base& ) const;
    bool set_slice( int, int, const sc_bv_base& );


    sc_fxnum_fast_observer* lock_observer() const;
    void unlock_observer( sc_fxnum_fast_observer* ) const;

private:

    double                          _val;

    scfx_params                     _params;
    bool                            _q_flag;
    bool                            _o_flag;

    mutable sc_fxnum_fast_observer* _observer;

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_bitref
//
//  Proxy class for bit-selection in class sc_fxnum, behaves like sc_bit.
// ----------------------------------------------------------------------------

// constructor

inline
sc_fxnum_bitref::sc_fxnum_bitref( sc_fxnum& num_, int idx_ )
: _num( num_ ), _idx( idx_ )
{}


// assignment operators

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( const sc_fxnum_bitref& a )
{
    if( &a != this )
    {
	_SC_FXNUM_OBSERVER_READ( a._num )
	set( a.get() );
	_SC_FXNUM_OBSERVER_WRITE( _num )
    }
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( const sc_fxnum_fast_bitref& a )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a._num )
    set( a.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( const sc_bit& a )
{
    set( static_cast<bool>( a ) );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator = ( bool a )
{
    set( a );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}


inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( const sc_fxnum_bitref& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    _SC_FXNUM_OBSERVER_READ( b._num )
    set( get() && b.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( const sc_fxnum_fast_bitref& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    _SC_FXNUM_FAST_OBSERVER_READ( b._num )
    set( get() && b.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( const sc_bit& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    set( get() && static_cast<bool>( b ) );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator &= ( bool b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    set( get() && b );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}


inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( const sc_fxnum_bitref& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    _SC_FXNUM_OBSERVER_READ( b._num )
    set( get() || b.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( const sc_fxnum_fast_bitref& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    _SC_FXNUM_FAST_OBSERVER_READ( b._num )
    set( get() || b.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( const sc_bit& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    set( get() || static_cast<bool>( b ) );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator |= ( bool b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    set( get() || b );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}


inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( const sc_fxnum_bitref& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    _SC_FXNUM_OBSERVER_READ( b._num )
    set( get() != b.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( const sc_fxnum_fast_bitref& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    _SC_FXNUM_FAST_OBSERVER_READ( b._num )
    set( get() != b.get() );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( const sc_bit& b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    set( get() != static_cast<bool>( b ) );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_bitref&
sc_fxnum_bitref::operator ^= ( bool b )
{
    _SC_FXNUM_OBSERVER_READ( _num )
    set( get() != b );
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}


// implicit conversion

inline
sc_fxnum_bitref::operator bool() const
{
    _SC_FXNUM_OBSERVER_READ( _num )
    return get();
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_bitref& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_bitref
//
//  Proxy class for bit-selection in class sc_fxnum_fast, behaves like sc_bit.
// ----------------------------------------------------------------------------

// constructor

inline
sc_fxnum_fast_bitref::sc_fxnum_fast_bitref( sc_fxnum_fast& num_, int idx_ )
: _num( num_ ), _idx( idx_ )
{}


// assignment operators

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( const sc_fxnum_bitref& a )
{
    _SC_FXNUM_OBSERVER_READ( a._num )
    set( a.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( const sc_fxnum_fast_bitref& a )
{
    if( &a != this )
    {
	_SC_FXNUM_FAST_OBSERVER_READ( a._num )
	set( a.get() );
	_SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    }
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( const sc_bit& a )
{
    set( static_cast<bool>( a ) );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator = ( bool a )
{
    set( a );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}


inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( const sc_fxnum_bitref& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    _SC_FXNUM_OBSERVER_READ( b._num )
    set( get() && b.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( const sc_fxnum_fast_bitref& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    _SC_FXNUM_FAST_OBSERVER_READ( b._num )
    set( get() && b.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( const sc_bit& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    set( get() && static_cast<bool>( b ) );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator &= ( bool b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    set( get() && b );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}


inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( const sc_fxnum_bitref& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    _SC_FXNUM_OBSERVER_READ( b._num )
    set( get() || b.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( const sc_fxnum_fast_bitref& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    _SC_FXNUM_FAST_OBSERVER_READ( b._num )
    set( get() || b.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( const sc_bit& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    set( get() || static_cast<bool>( b ) );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator |= ( bool b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    set( get() || b );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}


inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( const sc_fxnum_bitref& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    _SC_FXNUM_OBSERVER_READ( b._num )
    set( get() != b.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( const sc_fxnum_fast_bitref& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    _SC_FXNUM_FAST_OBSERVER_READ( b._num )
    set( get() != b.get() );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( const sc_bit& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    set( get() != static_cast<bool>( b ) );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_bitref&
sc_fxnum_fast_bitref::operator ^= ( bool b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    set( get() != b );
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}


// implicit conversion

inline
sc_fxnum_fast_bitref::operator bool() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    return get();
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_fast_bitref& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_subref
//
//  Proxy class for part-selection in class sc_fxnum,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

// constructor

inline
sc_fxnum_subref::sc_fxnum_subref( sc_fxnum& num_, int from_, int to_ )
: _num( num_ ), _from( from_ ), _to( to_ ),
  _bv( *new sc_bv_base( max( _from, _to ) - min( _from, _to ) + 1 ) )
{}


// assignment operators

inline
sc_fxnum_subref&
sc_fxnum_subref::operator = ( const sc_fxnum_subref& a )
{
    if( &a != this )
    {
	_bv = static_cast<sc_bv_base>( a );
	set();
	_SC_FXNUM_OBSERVER_WRITE( _num )
    }
    return *this;
}

inline
sc_fxnum_subref&
sc_fxnum_subref::operator = ( const sc_fxnum_fast_subref& a )
{
    _bv = static_cast<sc_bv_base>( a );
    set();
    _SC_FXNUM_OBSERVER_WRITE( _num )
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
inline                                                                        \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator = ( tp a )                                          \
{                                                                             \
    _bv = a;                                                                  \
    set();                                                                    \
    _SC_FXNUM_OBSERVER_WRITE( _num )                                          \
    return *this;                                                             \
}

DEFN_ASN_OP_T(const sc_bv_base&)
DEFN_ASN_OP_T(const sc_lv_base&)
DEFN_ASN_OP_T(const sc_bool_vector&)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const bool*)
DEFN_ASN_OP_T(const sc_signed&)
DEFN_ASN_OP_T(const sc_unsigned&)
DEFN_ASN_OP_T(const sc_int_base&)
DEFN_ASN_OP_T(const sc_uint_base&)
DEFN_ASN_OP_T(int64)
DEFN_ASN_OP_T(uint64)
DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(char)

#undef DEFN_ASN_OP_T


#define DEFN_ASN_OP_T(op,tp)                                                  \
inline                                                                        \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator op ## = ( tp a )                                    \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( _num )                                           \
    get();                                                                    \
    _bv = _bv op a;                                                           \
    set();                                                                    \
    _SC_FXNUM_OBSERVER_WRITE( _num )                                          \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
inline                                                                        \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator op ## = ( const sc_fxnum_subref& a )                \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( _num )                                           \
    get();                                                                    \
    _bv = _bv op static_cast<sc_bv_base>( a );                                \
    set();                                                                    \
    _SC_FXNUM_OBSERVER_WRITE( _num )                                          \
    return *this;                                                             \
}                                                                             \
                                                                              \
inline                                                                        \
sc_fxnum_subref&                                                              \
sc_fxnum_subref::operator op ## = ( const sc_fxnum_fast_subref& a )           \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( _num )                                           \
    get();                                                                    \
    _bv = _bv op static_cast<sc_bv_base>( a );                                \
    set();                                                                    \
    _SC_FXNUM_OBSERVER_WRITE( _num )                                          \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,const sc_bv_base&)                                           \
DEFN_ASN_OP_T(op,const sc_lv_base&)

DEFN_ASN_OP(&)
DEFN_ASN_OP(|)
DEFN_ASN_OP(^)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP


// relational operators

#define DEFN_REL_OP_T(op,tp)                                                  \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_subref& a, tp b )                                \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op b );                             \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum_subref& b )                                \
{                                                                             \
    return ( static_cast<sc_bv_base>( b ) op a );                             \
}

#define DEFN_REL_OP(op)                                                       \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_subref& a, const sc_fxnum_subref& b )            \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_subref& a, const sc_fxnum_fast_subref& b )       \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,const sc_bv_base&)                                           \
DEFN_REL_OP_T(op,const sc_lv_base&)                                           \
DEFN_REL_OP_T(op,const char*)                                                 \
DEFN_REL_OP_T(op,const bool*)                                                 \
DEFN_REL_OP_T(op,const sc_signed&)                                            \
DEFN_REL_OP_T(op,const sc_unsigned&)                                          \
DEFN_REL_OP_T(op,int)                                                         \
DEFN_REL_OP_T(op,unsigned int)                                                \
DEFN_REL_OP_T(op,long)                                                        \
DEFN_REL_OP_T(op,unsigned long)

DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// reduce functions

#define DEFN_RED_FNC(fnc)                                                     \
inline                                                                        \
bool                                                                          \
sc_fxnum_subref::fnc() const                                                  \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( _num )                                           \
    get();                                                                    \
    return _bv.fnc();                                                         \
}

DEFN_RED_FNC(and_reduce)
DEFN_RED_FNC(nand_reduce)
DEFN_RED_FNC(or_reduce)
DEFN_RED_FNC(nor_reduce)
DEFN_RED_FNC(xor_reduce)
DEFN_RED_FNC(xnor_reduce)

#undef DEFN_RED_FNC


// query parameter

inline
int
sc_fxnum_subref::length() const
{
    return _bv.length();
}


// explicit conversions

inline
int
sc_fxnum_subref::to_signed() const
{
    _SC_FXNUM_OBSERVER_READ( _num )
    get();
    return _bv.to_signed();
}

inline
unsigned int
sc_fxnum_subref::to_unsigned() const
{
    _SC_FXNUM_OBSERVER_READ( _num )
    get();
    return _bv.to_unsigned();
}

inline
long
sc_fxnum_subref::to_long() const
{
    _SC_FXNUM_OBSERVER_READ( _num )
    get();
    return _bv.to_long();
}

inline
unsigned long
sc_fxnum_subref::to_ulong() const
{
    _SC_FXNUM_OBSERVER_READ( _num )
    get();
    return _bv.to_ulong();
}


inline
const sc_string
sc_fxnum_subref::to_string() const
{
    get();
    return _bv.to_string();
}


// implicit conversion

inline
sc_fxnum_subref::operator sc_bv_base () const
{
    _SC_FXNUM_OBSERVER_READ( _num )
    get();
    return _bv;
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_subref& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_subref
//
//  Proxy class for part-selection in class sc_fxnum_fast,
//  behaves like sc_bv_base.
// ----------------------------------------------------------------------------

// constructor

inline
sc_fxnum_fast_subref::sc_fxnum_fast_subref( sc_fxnum_fast& num_,
					    int from_, int to_ )
: _num( num_ ), _from( from_ ), _to( to_ ),
  _bv( *new sc_bv_base( max( _from, _to ) - min( _from, _to ) + 1 ) )
{}


// assignment operators

inline
sc_fxnum_fast_subref&
sc_fxnum_fast_subref::operator = ( const sc_fxnum_subref& a )
{
    _bv = static_cast<sc_bv_base>( a );
    set();
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    return *this;
}

inline
sc_fxnum_fast_subref&
sc_fxnum_fast_subref::operator = ( const sc_fxnum_fast_subref& a )
{
    if( &a != this )
    {
	_bv = static_cast<sc_bv_base>( a );
	set();
	_SC_FXNUM_FAST_OBSERVER_WRITE( _num )
    }
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
inline                                                                        \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator = ( tp a )                                     \
{                                                                             \
    _bv = a;                                                                  \
    set();                                                                    \
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )                                     \
    return *this;                                                             \
}

DEFN_ASN_OP_T(const sc_bv_base&)
DEFN_ASN_OP_T(const sc_lv_base&)
DEFN_ASN_OP_T(const sc_bool_vector&)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const bool*)
DEFN_ASN_OP_T(const sc_signed&)
DEFN_ASN_OP_T(const sc_unsigned&)
DEFN_ASN_OP_T(const sc_int_base&)
DEFN_ASN_OP_T(const sc_uint_base&)
DEFN_ASN_OP_T(int64)
DEFN_ASN_OP_T(uint64)
DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(char)

#undef DEFN_ASN_OP_T


#define DEFN_ASN_OP_T(op,tp)                                                  \
inline                                                                        \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator op ## = ( tp a )                               \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( _num )                                      \
    get();                                                                    \
    _bv = _bv op a;                                                           \
    set();                                                                    \
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )                                     \
    return *this;                                                             \
}

#define DEFN_ASN_OP(op)                                                       \
inline                                                                        \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator op ## = ( const sc_fxnum_subref& a )           \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( _num )                                      \
    get();                                                                    \
    _bv = _bv op static_cast<sc_bv_base>( a );                                \
    set();                                                                    \
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )                                     \
    return *this;                                                             \
}                                                                             \
                                                                              \
inline                                                                        \
sc_fxnum_fast_subref&                                                         \
sc_fxnum_fast_subref::operator op ## = ( const sc_fxnum_fast_subref& a )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( _num )                                      \
    get();                                                                    \
    _bv = _bv op static_cast<sc_bv_base>( a );                                \
    set();                                                                    \
    _SC_FXNUM_FAST_OBSERVER_WRITE( _num )                                     \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,const sc_bv_base&)                                           \
DEFN_ASN_OP_T(op,const sc_lv_base&)

DEFN_ASN_OP(&)
DEFN_ASN_OP(|)
DEFN_ASN_OP(^)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP


// relational operators

#define DEFN_REL_OP_T(op,tp)                                                  \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast_subref& a, tp b )                           \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op b );                             \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum_fast_subref& b )                           \
{                                                                             \
    return ( static_cast<sc_bv_base>( b ) op a );                             \
}

#define DEFN_REL_OP(op)                                                       \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast_subref& a, const sc_fxnum_fast_subref& b )  \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast_subref& a, const sc_fxnum_subref& b )       \
{                                                                             \
    return ( static_cast<sc_bv_base>( a ) op static_cast<sc_bv_base>( b ) );  \
}                                                                             \
                                                                              \
DEFN_REL_OP_T(op,const sc_bv_base&)                                           \
DEFN_REL_OP_T(op,const sc_lv_base&)                                           \
DEFN_REL_OP_T(op,const char*)                                                 \
DEFN_REL_OP_T(op,const bool*)                                                 \
DEFN_REL_OP_T(op,const sc_signed&)                                            \
DEFN_REL_OP_T(op,const sc_unsigned&)                                          \
DEFN_REL_OP_T(op,int)                                                         \
DEFN_REL_OP_T(op,unsigned int)                                                \
DEFN_REL_OP_T(op,long)                                                        \
DEFN_REL_OP_T(op,unsigned long)

DEFN_REL_OP(==)
DEFN_REL_OP(!=)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// reduce functions

#define DEFN_RED_FNC(fnc)                                                     \
inline                                                                        \
bool                                                                          \
sc_fxnum_fast_subref::fnc() const                                             \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( _num )                                      \
    get();                                                                    \
    return _bv.fnc();                                                         \
}

DEFN_RED_FNC(and_reduce)
DEFN_RED_FNC(nand_reduce)
DEFN_RED_FNC(or_reduce)
DEFN_RED_FNC(nor_reduce)
DEFN_RED_FNC(xor_reduce)
DEFN_RED_FNC(xnor_reduce)

#undef DEFN_RED_FNC


// query parameter

inline
int
sc_fxnum_fast_subref::length() const
{
    return _bv.length();
}


// explicit conversions

inline
int
sc_fxnum_fast_subref::to_signed() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    get();
    return _bv.to_signed();
}

inline
unsigned int
sc_fxnum_fast_subref::to_unsigned() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    get();
    return _bv.to_unsigned();
}

inline
long
sc_fxnum_fast_subref::to_long() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    get();
    return _bv.to_long();
}

inline
unsigned long
sc_fxnum_fast_subref::to_ulong() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    get();
    return _bv.to_ulong();
}


inline
const sc_string
sc_fxnum_fast_subref::to_string() const
{
    get();
    return _bv.to_string();
}


// implicit conversion

inline
sc_fxnum_fast_subref::operator sc_bv_base () const
{
    _SC_FXNUM_FAST_OBSERVER_READ( _num )
    get();
    return _bv;
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_fast_subref& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum
//
//  Base class for the fixed-point types; arbitrary precision.
// ----------------------------------------------------------------------------

inline
sc_fxnum_observer*
sc_fxnum::observer() const
{
    return _observer;
}


inline
void
sc_fxnum::cast()
{
    _SC_ERROR_IF( ! _rep->is_normal(), _SC_ID_INVALID_VALUE );

    if( _params.cast_switch() == SC_ON )
	_rep->cast( _params, _q_flag, _o_flag );
}


// constructors

inline
sc_fxnum::sc_fxnum( const sc_fxtype_params& type_params_,
		    sc_enc enc_,
		    const sc_fxcast_switch& cast_sw,
		    sc_fxnum_observer* observer_ )
: _rep( new scfx_rep ),
  _params( type_params_, enc_, cast_sw ),
  _q_flag( false ),
  _o_flag( false ),
  _observer( observer_ )
{
    _SC_FXNUM_OBSERVER_DEFAULT
    _SC_FXNUM_OBSERVER_CONSTRUCT( *this )
}

#define DEFN_CTOR_T(tp,arg)                                                   \
inline                                                                        \
sc_fxnum::sc_fxnum( tp a,                                                     \
		    const sc_fxtype_params& type_params_,                     \
		    sc_enc enc_,                                              \
		    const sc_fxcast_switch& cast_sw,                          \
		    sc_fxnum_observer* observer_ )                            \
: _rep( new scfx_rep( arg ) ),                                                \
  _params( type_params_, enc_, cast_sw ),                                     \
  _q_flag( false ),                                                           \
  _o_flag( false ),                                                           \
  _observer( observer_ )                                                      \
{                                                                             \
    _SC_FXNUM_OBSERVER_DEFAULT                                                \
    cast();                                                                   \
    _SC_FXNUM_OBSERVER_CONSTRUCT( *this )                                     \
    _SC_FXNUM_OBSERVER_WRITE( *this )                                         \
}

#define DEFN_CTOR_T_A(tp) DEFN_CTOR_T(tp,a)
#define DEFN_CTOR_T_B(tp) DEFN_CTOR_T(tp,*a._rep)
#define DEFN_CTOR_T_C(tp) DEFN_CTOR_T(tp,a.to_double())
#define DEFN_CTOR_T_D(tp) DEFN_CTOR_T(tp,a.num)

DEFN_CTOR_T_A(int)
DEFN_CTOR_T_A(unsigned int)
DEFN_CTOR_T_A(long)
DEFN_CTOR_T_A(unsigned long)
DEFN_CTOR_T_A(double)
DEFN_CTOR_T_A(const char*)
DEFN_CTOR_T_B(const sc_fxval&)
DEFN_CTOR_T_C(const sc_fxval_fast&)
DEFN_CTOR_T_B(const sc_fxnum&)
DEFN_CTOR_T_C(const sc_fxnum_fast&)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_CTOR_T_A(int64)
DEFN_CTOR_T_A(uint64)
DEFN_CTOR_T_D(const sc_int_base&)
DEFN_CTOR_T_D(const sc_uint_base&)
DEFN_CTOR_T_A(const sc_signed&)
DEFN_CTOR_T_A(const sc_unsigned&)
#endif

#undef DEFN_CTOR_T
#undef DEFN_CTOR_T_A
#undef DEFN_CTOR_T_B
#undef DEFN_CTOR_T_C
#undef DEFN_CTOR_T_D


inline
sc_fxnum::~sc_fxnum()
{
    _SC_FXNUM_OBSERVER_DESTRUCT( *this )
    delete _rep;
}


// internal use only;
inline
const scfx_rep*
sc_fxnum::get_rep() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return _rep;
}


// unary operators

inline
const sc_fxval
sc_fxnum::operator - () const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return sc_fxval( ::neg__scfx_rep( *_rep ) );
}

inline
const sc_fxval
sc_fxnum::operator + () const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return sc_fxval( new scfx_rep( *_rep ) );
}


// unary functions

inline
void
neg( sc_fxval& c, const sc_fxnum& a )
{
    _SC_FXNUM_OBSERVER_READ( a )
    c.set_rep( ::neg__scfx_rep( *a._rep ) );
}

inline
void
neg( sc_fxnum& c, const sc_fxnum& a )
{
    _SC_FXNUM_OBSERVER_READ( a )
    delete c._rep;
    c._rep = ::neg__scfx_rep( *a._rep );
    c.cast();
    _SC_FXNUM_OBSERVER_WRITE( c )
}


// binary operators

#define DEFN_BIN_OP_T(op,fnc,tp)                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    return sc_fxval( ::fnc ## __scfx_rep( *a._rep, *tmp.get_rep() ) );        \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    return sc_fxval( ::fnc ## __scfx_rep( *tmp.get_rep(), *b._rep ) );        \
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
operator op ( const sc_fxnum& a, const sc_fxnum& b )                          \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    return sc_fxval( ::fnc ## __scfx_rep( *a._rep, *b._rep ) );               \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxnum& a, const sc_fxval& b )                          \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    return sc_fxval( ::fnc ## __scfx_rep( *a._rep, *b.get_rep() ) );          \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval                                                                \
operator op ( const sc_fxval& a, const sc_fxnum& b )                          \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    return sc_fxval( ::fnc ## __scfx_rep( *a.get_rep(), *b._rep ) );          \
}                                                                             \
                                                                              \
DEFN_BIN_OP_T(op,fnc,int)                                                     \
DEFN_BIN_OP_T(op,fnc,unsigned int)                                            \
DEFN_BIN_OP_T(op,fnc,long)                                                    \
DEFN_BIN_OP_T(op,fnc,unsigned long)                                           \
DEFN_BIN_OP_T(op,fnc,double)                                                  \
DEFN_BIN_OP_T(op,fnc,const char*)                                             \
DEFN_BIN_OP_T(op,fnc,const sc_fxval_fast&)                                    \
DEFN_BIN_OP_T(op,fnc,const sc_fxnum_fast&)                                    \
DEFN_BIN_OP_OTHER(op,fnc)

DEFN_BIN_OP(*,mult)
DEFN_BIN_OP(+,add)
DEFN_BIN_OP(-,sub)
// don't use macros
//DEFN_BIN_OP(/,div)
inline
const sc_fxval
operator / ( const sc_fxnum& a, const sc_fxnum& b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    _SC_FXNUM_OBSERVER_READ( b )
    return sc_fxval( ::div__scfx_rep( *a._rep, *b._rep ) );
}

inline
const sc_fxval
operator / ( const sc_fxnum& a, const sc_fxval& b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    return sc_fxval( ::div__scfx_rep( *a._rep, *b.get_rep() ) );
}

inline
const sc_fxval
operator / ( const sc_fxval& a, const sc_fxnum& b )
{
    _SC_FXNUM_OBSERVER_READ( b )
    return sc_fxval( ::div__scfx_rep( *a.get_rep(), *b._rep ) );
}                                                                                                                                                           \
DEFN_BIN_OP_T(/,div,int)
DEFN_BIN_OP_T(/,div,unsigned int)
DEFN_BIN_OP_T(/,div,long)
DEFN_BIN_OP_T(/,div,unsigned long)
DEFN_BIN_OP_T(/,div,double)
DEFN_BIN_OP_T(/,div,const char*)
DEFN_BIN_OP_T(/,div,const sc_fxval_fast&)
DEFN_BIN_OP_T(/,div,const sc_fxnum_fast&)
//DEFN_BIN_OP_OTHER(/,div)
#ifndef SC_FX_EXCLUDE_OTHER
DEFN_BIN_OP_T(/,div,int64)
DEFN_BIN_OP_T(/,div,uint64)
DEFN_BIN_OP_T(/,div,const sc_int_base&)
DEFN_BIN_OP_T(/,div,const sc_uint_base&)
DEFN_BIN_OP_T(/,div,const sc_signed&)
DEFN_BIN_OP_T(/,div,const sc_unsigned&)
#endif

#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP_OTHER
#undef DEFN_BIN_OP


inline
const sc_fxval
operator << ( const sc_fxnum& a, int b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    return sc_fxval( ::lsh__scfx_rep( *a._rep, b ) );
}

inline
const sc_fxval
operator >> ( const sc_fxnum& a, int b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    return sc_fxval( ::rsh__scfx_rep( *a._rep, b ) );
}


// binary functions

#define DEFN_BIN_FNC_T(fnc,tp)                                                \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    c.set_rep( ::fnc ## __scfx_rep( *a._rep, *tmp.get_rep() ) );              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    c.set_rep( ::fnc ## __scfx_rep( *tmp.get_rep(), *b._rep ) );              \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, tp b )                                  \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *a._rep, *tmp.get_rep() );                  \
    c.cast();                                                                 \
    _SC_FXNUM_OBSERVER_WRITE( c )                                             \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, tp a, const sc_fxnum& b )                                  \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *tmp.get_rep(), *b._rep );                  \
    c.cast();                                                                 \
    _SC_FXNUM_OBSERVER_WRITE( c )                                             \
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
fnc ( sc_fxval& c, const sc_fxnum& a, const sc_fxnum& b )                     \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    c.set_rep( ::fnc ## __scfx_rep( *a._rep, *b._rep ) );                     \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, const sc_fxnum& b )                     \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *a._rep, *b._rep );                         \
    c.cast();                                                                 \
    _SC_FXNUM_OBSERVER_WRITE( c )                                             \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxnum& a, const sc_fxval& b )                     \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    c.set_rep( ::fnc ## __scfx_rep( *a._rep, *b.get_rep() ) );                \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval& c, const sc_fxval& a, const sc_fxnum& b )                     \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    c.set_rep( ::fnc ## __scfx_rep( *a.get_rep(), *b._rep ) );                \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxnum& a, const sc_fxval& b )                     \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *a._rep, *b.get_rep() );                    \
    c.cast();                                                                 \
    _SC_FXNUM_OBSERVER_WRITE( c )                                             \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum& c, const sc_fxval& a, const sc_fxnum& b )                     \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    delete c._rep;                                                            \
    c._rep = ::fnc ## __scfx_rep( *a.get_rep(), *b._rep );                    \
    c.cast();                                                                 \
    _SC_FXNUM_OBSERVER_WRITE( c )                                             \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T(fnc,int)                                                       \
DEFN_BIN_FNC_T(fnc,unsigned int)                                              \
DEFN_BIN_FNC_T(fnc,long)                                                      \
DEFN_BIN_FNC_T(fnc,unsigned long)                                             \
DEFN_BIN_FNC_T(fnc,double)                                                    \
DEFN_BIN_FNC_T(fnc,const char*)                                               \
DEFN_BIN_FNC_T(fnc,const sc_fxval_fast&)                                      \
DEFN_BIN_FNC_T(fnc,const sc_fxnum_fast&)                                      \
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
lshift( sc_fxval& c, const sc_fxnum& a, int b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    c.set_rep( ::lsh__scfx_rep( *a._rep, b ) );
}

inline
void
rshift( sc_fxval& c, const sc_fxnum& a, int b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    c.set_rep( ::rsh__scfx_rep( *a._rep, b ) );
}

inline
void
lshift( sc_fxnum& c, const sc_fxnum& a, int b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    delete c._rep;
    c._rep = ::lsh__scfx_rep( *a._rep, b );
    c.cast();
    _SC_FXNUM_OBSERVER_WRITE( c )
}

inline
void
rshift( sc_fxnum& c, const sc_fxnum& a, int b )
{
    _SC_FXNUM_OBSERVER_READ( a )
    delete c._rep;
    c._rep = ::rsh__scfx_rep( *a._rep, b );
    c.cast();
    _SC_FXNUM_OBSERVER_WRITE( c )
}


// relational (including equality) operators

#define DEFN_REL_OP_T(op,ret,tp)                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum& a, tp b )                                       \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    sc_fxval tmp( b );                                                        \
    int result = ::cmp__scfx_rep( *a._rep, *tmp.get_rep() );                  \
    return ( ret );                                                           \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum& b )                                       \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    sc_fxval tmp( a );                                                        \
    int result = ::cmp__scfx_rep( *tmp.get_rep(), *b._rep );                  \
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
operator op ( const sc_fxnum& a, const sc_fxnum& b )                          \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    int result = ::cmp__scfx_rep( *a._rep, *b._rep );                         \
    return ( ret );                                                           \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum& a, const sc_fxval& b )                          \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( a )                                              \
    int result = ::cmp__scfx_rep( *a._rep, *b.get_rep() );                    \
    return ( ret );                                                           \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval& a, const sc_fxnum& b )                          \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    int result = ::cmp__scfx_rep( *a.get_rep(), *b._rep );                    \
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
DEFN_REL_OP_T(op,ret,const sc_fxnum_fast&)                                    \
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
sc_fxnum&
sc_fxnum::operator = ( const sc_fxnum& a )
{
    if( &a != this )
    {
        _SC_FXNUM_OBSERVER_READ( a )
	*_rep = *a._rep;
	cast();
	_SC_FXNUM_OBSERVER_WRITE( *this )
    }
    return *this;
}

inline
sc_fxnum&
sc_fxnum::operator = ( const sc_fxval& a )
{
    *_rep = *a.get_rep();
    cast();
    _SC_FXNUM_OBSERVER_WRITE( *this )
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
inline                                                                        \
sc_fxnum&                                                                     \
sc_fxnum::operator = ( tp a )                                                 \
{                                                                             \
    sc_fxval tmp( a );                                                        \
    *_rep = *tmp.get_rep();                                                   \
    cast();                                                                   \
    _SC_FXNUM_OBSERVER_WRITE( *this )                                         \
    return *this;                                                             \
}

DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(double)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const sc_fxval_fast&)
DEFN_ASN_OP_T(const sc_fxnum_fast&)
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
sc_fxnum&                                                                     \
sc_fxnum::operator op ( tp b )                                                \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( *this )                                          \
    sc_fxval tmp( b );                                                        \
    scfx_rep* new_rep = ::fnc ## __scfx_rep( *_rep, *tmp.get_rep() );         \
    delete _rep;                                                              \
    _rep = new_rep;                                                           \
    cast();                                                                   \
    _SC_FXNUM_OBSERVER_WRITE( *this )                                         \
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
sc_fxnum&                                                                     \
sc_fxnum::operator op ( const sc_fxnum& b )                                   \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( *this )                                          \
    _SC_FXNUM_OBSERVER_READ( b )                                              \
    scfx_rep* new_rep = ::fnc ## __scfx_rep( *_rep, *b._rep );                \
    delete _rep;                                                              \
    _rep = new_rep;                                                           \
    cast();                                                                   \
    _SC_FXNUM_OBSERVER_WRITE( *this )                                         \
    return *this;                                                             \
}                                                                             \
                                                                              \
inline                                                                        \
sc_fxnum&                                                                     \
sc_fxnum::operator op ( const sc_fxval& b )                                   \
{                                                                             \
    _SC_FXNUM_OBSERVER_READ( *this )                                          \
    scfx_rep* new_rep = ::fnc ## __scfx_rep( *_rep, *b.get_rep() );           \
    delete _rep;                                                              \
    _rep = new_rep;                                                           \
    cast();                                                                   \
    _SC_FXNUM_OBSERVER_WRITE( *this )                                         \
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
DEFN_ASN_OP_T(op,fnc,const sc_fxnum_fast&)                                    \
DEFN_ASN_OP_OTHER(op,fnc)

DEFN_ASN_OP(*=,mult)
DEFN_ASN_OP(/=,div)
DEFN_ASN_OP(+=,add)
DEFN_ASN_OP(-=,sub)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP_OTHER
#undef DEFN_ASN_OP


inline
sc_fxnum&
sc_fxnum::operator <<= ( int b )
{
    _SC_FXNUM_OBSERVER_READ( *this )
    _rep->lshift( b );
    cast();
    _SC_FXNUM_OBSERVER_WRITE( *this )
    return *this;
}

inline
sc_fxnum&
sc_fxnum::operator >>= ( int b )
{
    _SC_FXNUM_OBSERVER_READ( *this )
    _rep->rshift( b );
    cast();
    _SC_FXNUM_OBSERVER_WRITE( *this )
    return *this;
}


// auto-increment and auto-decrement

inline
const sc_fxval
sc_fxnum::operator ++ ( int )
{
    sc_fxval c( *this );
    (*this) += 1;
    return c;
}

inline
const sc_fxval
sc_fxnum::operator -- ( int )
{
    sc_fxval c( *this );
    (*this) -= 1;
    return c;
}

inline
sc_fxnum&
sc_fxnum::operator ++ ()
{
    (*this) += 1;
    return *this;
}

inline
sc_fxnum&
sc_fxnum::operator -- ()
{
    (*this) -= 1;
    return *this;
}


// bit selection

inline
const sc_fxnum_bitref
sc_fxnum::operator [] ( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl() );
}

inline
sc_fxnum_bitref
sc_fxnum::operator [] ( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( *this, i - _params.fwl() );
}

inline
const sc_fxnum_bitref
sc_fxnum::bit( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl() );
}

inline
sc_fxnum_bitref
sc_fxnum::bit( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_bitref( *this, i - _params.fwl() );
}


// part selection

inline
const sc_fxnum_subref
sc_fxnum::operator () ( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_subref
sc_fxnum::operator () ( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( *this, i - _params.fwl(), j - _params.fwl() );
}

inline
const sc_fxnum_subref
sc_fxnum::range( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( const_cast<sc_fxnum&>( *this ),
			    i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_subref
sc_fxnum::range( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_subref( *this, i - _params.fwl(), j - _params.fwl() );
}


inline
const sc_fxnum_subref
sc_fxnum::operator () () const
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
sc_fxnum_subref
sc_fxnum::operator () ()
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
const sc_fxnum_subref
sc_fxnum::range() const
{
    return this->range( _params.wl() - 1, 0 );
}

inline
sc_fxnum_subref
sc_fxnum::range()
{
    return this->range( _params.wl() - 1, 0 );
}


// implicit conversion

inline
sc_fxnum::operator double() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return _rep->to_double();
}


// explicit conversion to primitive types

inline
short
sc_fxnum::to_short() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<short>( _rep->to_double() );
}

inline
unsigned short
sc_fxnum::to_ushort() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<unsigned short>( _rep->to_double() );
}

inline
int
sc_fxnum::to_int() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<int>( _rep->to_double() );
}

inline
unsigned int
sc_fxnum::to_uint() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<unsigned int>( _rep->to_double() );
}

inline
long
sc_fxnum::to_long() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<long>( _rep->to_double() );
}

inline
unsigned long
sc_fxnum::to_ulong() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<unsigned long>( _rep->to_double() );
}

inline
float
sc_fxnum::to_float() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return static_cast<float>( _rep->to_double() );
}

inline
double
sc_fxnum::to_double() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return _rep->to_double();
}


// query value

inline
bool
sc_fxnum::is_neg() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return _rep->is_neg();
}

inline
bool
sc_fxnum::is_zero() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return _rep->is_zero();
}

// internal use only;
inline
bool
sc_fxnum::is_normal() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return _rep->is_normal();
}

inline
bool
sc_fxnum::quantization_flag() const
{
    return _q_flag;
}

inline
bool
sc_fxnum::overflow_flag() const
{
    return _o_flag;
}


inline
const sc_fxval
sc_fxnum::value() const
{
    _SC_FXNUM_OBSERVER_READ( *this )
    return sc_fxval( new scfx_rep( *_rep ) );
}


// query parameters

inline
int
sc_fxnum::wl() const
{
    return _params.wl();
}

inline
int
sc_fxnum::iwl() const
{
    return _params.iwl();
}

inline
sc_q_mode
sc_fxnum::q_mode() const
{
    return _params.q_mode();
}

inline
sc_o_mode
sc_fxnum::o_mode() const
{
    return _params.o_mode();
}

inline
int
sc_fxnum::n_bits() const
{
    return _params.n_bits();
}


inline
const sc_fxtype_params&
sc_fxnum::type_params() const
{
    return _params.type_params();
}


inline
const sc_fxcast_switch&
sc_fxnum::cast_switch() const
{
    return _params.cast_switch();
}


// internal use only;
inline
void
sc_fxnum::observer_read() const
{
    _SC_FXNUM_OBSERVER_READ( *this );
}


// internal use only;
inline
bool
sc_fxnum::get_bit( int i ) const
{
    return _rep->get_bit( i );
}


// protected methods and friend functions

inline
bool
sc_fxnum::set_bit( int i, bool high )
{
    if( high )
        return _rep->set( i, _params );
    else
        return _rep->clear( i, _params );
}


inline
bool
sc_fxnum::get_slice( int i, int j, sc_bv_base& bv ) const
{
    return _rep->get_slice( i, j, _params, bv );
}

inline
bool
sc_fxnum::set_slice( int i, int j, const sc_bv_base& bv )
{
    return _rep->set_slice( i, j, _params, bv );
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast
//
//  Base class for the fixed-point types; limited precision.
// ----------------------------------------------------------------------------

inline
sc_fxnum_fast_observer*
sc_fxnum_fast::observer() const
{
    return _observer;
}


// constructors

inline
sc_fxnum_fast::sc_fxnum_fast( const sc_fxtype_params& type_params_,
			      sc_enc enc_,
			      const sc_fxcast_switch& cast_sw,
			      sc_fxnum_fast_observer* observer_ )
: _val( 0.0 ),
  _params( type_params_, enc_, cast_sw ),
  _q_flag( false ),
  _o_flag( false ),
  _observer( observer_ )
{
    _SC_FXNUM_FAST_OBSERVER_DEFAULT
    _SC_FXNUM_FAST_OBSERVER_CONSTRUCT(*this)
}

inline
sc_fxnum_fast::sc_fxnum_fast( const sc_fxnum_fast& a,
			      const sc_fxtype_params& type_params_,
			      sc_enc enc_,
			      const sc_fxcast_switch& cast_sw,
			      sc_fxnum_fast_observer* observer_ )
: _val( a._val ),
  _params( type_params_, enc_, cast_sw ),
  _q_flag( false ),
  _o_flag( false ),
  _observer( observer_ )
{
    _SC_FXNUM_FAST_OBSERVER_DEFAULT
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    cast();
    _SC_FXNUM_FAST_OBSERVER_CONSTRUCT( *this )
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
}

#define DEFN_CTOR_T(tp,arg)                                                   \
inline                                                                        \
sc_fxnum_fast::sc_fxnum_fast( tp a,                                           \
		              const sc_fxtype_params& type_params_,           \
		              sc_enc enc_,                                    \
		              const sc_fxcast_switch& cast_sw,                \
		              sc_fxnum_fast_observer* observer_ )             \
: _val( arg ),                                                                \
  _params( type_params_, enc_, cast_sw ),                                     \
  _q_flag( false ),                                                           \
  _o_flag( false ),                                                           \
  _observer( observer_ )                                                      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_DEFAULT                                           \
    cast();                                                                   \
    _SC_FXNUM_FAST_OBSERVER_CONSTRUCT(*this)                                  \
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )                                    \
}

#define DEFN_CTOR_T_A(tp) DEFN_CTOR_T(tp,static_cast<double>( a ))
#define DEFN_CTOR_T_B(tp) DEFN_CTOR_T(tp,sc_fxval_fast::from_string( a ))
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
DEFN_CTOR_T_C(const sc_fxval_fast&)
DEFN_CTOR_T_C(const sc_fxnum&)
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
sc_fxnum_fast::~sc_fxnum_fast()
{
    _SC_FXNUM_FAST_OBSERVER_DESTRUCT( *this )
}


// internal use only;
inline
double
sc_fxnum_fast::get_val() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return _val;
}


// unary operators

inline
const sc_fxval_fast
sc_fxnum_fast::operator - () const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return sc_fxval_fast( - _val );
}

inline
const sc_fxval_fast
sc_fxnum_fast::operator + () const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return sc_fxval_fast( _val );
}


// unary functions

inline
void
neg( sc_fxval_fast& c, const sc_fxnum_fast& a )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    c.set_val( - a._val );
}

inline
void
neg( sc_fxnum_fast& c, const sc_fxnum_fast& a )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    c._val = - a._val;
    c.cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )
}


// binary operators

#define DEFN_BIN_OP_T(op,tp)                                                  \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxnum_fast& a, tp b )                                  \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    return sc_fxval_fast( a._val op tmp.get_val() );                          \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( tp a, const sc_fxnum_fast& b )                                  \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    return sc_fxval_fast( tmp.get_val() op b._val );                          \
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
operator op ( const sc_fxnum_fast& a, const sc_fxnum_fast& b )                \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    return sc_fxval_fast( a._val op b._val );                                 \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxnum_fast& a, const sc_fxval_fast& b )                \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    return sc_fxval_fast( a._val op b.get_val() );                            \
}                                                                             \
                                                                              \
inline                                                                        \
const sc_fxval_fast                                                           \
operator op ( const sc_fxval_fast& a, const sc_fxnum_fast& b )                \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    return sc_fxval_fast( a.get_val() op b._val );                            \
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
operator / ( const sc_fxnum_fast& a, const sc_fxnum_fast& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    _SC_FXNUM_FAST_OBSERVER_READ( b )
    return sc_fxval_fast( a._val / b._val );
}

inline
const sc_fxval_fast
operator / ( const sc_fxnum_fast& a, const sc_fxval_fast& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    return sc_fxval_fast( a._val / b.get_val() );
}

inline
const sc_fxval_fast
operator / ( const sc_fxval_fast& a, const sc_fxnum_fast& b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( b )
    return sc_fxval_fast( a.get_val() / b._val );
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
operator << ( const sc_fxnum_fast& a, int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    return sc_fxval_fast( a._val * scfx_pow2( b ) );
}

inline
const sc_fxval_fast
operator >> ( const sc_fxnum_fast& a, int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    return sc_fxval_fast( a._val * scfx_pow2( -b ) );
}


// binary functions

#define DEFN_BIN_FNC_T(fnc,op,tp)                                             \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxnum_fast& a, tp b )                        \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    c.set_val( a._val op tmp.get_val() );                                     \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, tp a, const sc_fxnum_fast& b )                        \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    c.set_val( tmp.get_val() op b._val );                                     \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxnum_fast& a, tp b )                        \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    c._val = a._val op tmp.get_val();                                         \
    c.cast();                                                                 \
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )                                        \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, tp a, const sc_fxnum_fast& b )                        \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    c._val = tmp.get_val() op b._val;                                         \
    c.cast();                                                                 \
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )                                        \
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
fnc ( sc_fxval_fast& c, const sc_fxnum_fast& a, const sc_fxnum_fast& b )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    c.set_val( a._val op b._val );                                            \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxnum_fast& a, const sc_fxnum_fast& b )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    c._val = a._val op b._val;                                                \
    c.cast();                                                                 \
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )                                        \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxnum_fast& a, const sc_fxval_fast& b )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    c.set_val( a._val op b.get_val() );                                       \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxval_fast& c, const sc_fxval_fast& a, const sc_fxnum_fast& b )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    c.set_val( a.get_val() op b._val );                                       \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxnum_fast& a, const sc_fxval_fast& b )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    c._val = a._val op b.get_val();                                           \
    c.cast();                                                                 \
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )                                        \
}                                                                             \
                                                                              \
inline                                                                        \
void                                                                          \
fnc ( sc_fxnum_fast& c, const sc_fxval_fast& a, const sc_fxnum_fast& b )      \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    c._val = a.get_val() op b._val;                                           \
    c.cast();                                                                 \
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )                                        \
}                                                                             \
                                                                              \
DEFN_BIN_FNC_T(fnc,op,int)                                                    \
DEFN_BIN_FNC_T(fnc,op,unsigned int)                                           \
DEFN_BIN_FNC_T(fnc,op,long)                                                   \
DEFN_BIN_FNC_T(fnc,op,unsigned long)                                          \
DEFN_BIN_FNC_T(fnc,op,double)                                                 \
DEFN_BIN_FNC_T(fnc,op,const char*)                                            \
DEFN_BIN_FNC_T(fnc,op,const sc_fxval&)                                        \
DEFN_BIN_FNC_T(fnc,op,const sc_fxnum&)                                        \
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
lshift( sc_fxval_fast& c, const sc_fxnum_fast& a, int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    c.set_val( a._val * scfx_pow2( b ) );
}

inline
void
rshift( sc_fxval_fast& c, const sc_fxnum_fast& a, int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    c.set_val( a._val * scfx_pow2( -b ) );
}

inline
void
lshift( sc_fxnum_fast& c, const sc_fxnum_fast& a, int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    c._val = a._val * scfx_pow2( b );
    c.cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )
}

inline
void
rshift( sc_fxnum_fast& c, const sc_fxnum_fast& a, int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( a )
    c._val = a._val * scfx_pow2( -b );
    c.cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( c )
}


// relational (including equality) operators

#define DEFN_REL_OP_T(op,tp)                                                  \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast& a, tp b )                                  \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    sc_fxval_fast tmp( b );                                                   \
    return ( a._val op tmp.get_val() );                                       \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( tp a, const sc_fxnum_fast& b )                                  \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    sc_fxval_fast tmp( a );                                                   \
    return ( tmp.get_val() op b._val );                                       \
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
operator op ( const sc_fxnum_fast& a, const sc_fxnum_fast& b )                \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    return ( a._val op b._val );                                              \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxnum_fast& a, const sc_fxval_fast& b )                \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( a )                                         \
    return ( a._val op b.get_val() );                                         \
}                                                                             \
                                                                              \
inline                                                                        \
bool                                                                          \
operator op ( const sc_fxval_fast& a, const sc_fxnum_fast& b )                \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    return ( a.get_val() op b._val );                                         \
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
sc_fxnum_fast&
sc_fxnum_fast::operator = ( const sc_fxnum_fast& a )
{
    if( &a != this )
    {
	_SC_FXNUM_FAST_OBSERVER_READ( a )
	_val = a._val;
	cast();
	_SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    }
    return *this;
}

inline
sc_fxnum_fast&
sc_fxnum_fast::operator = ( const sc_fxval_fast& a )
{
    _val = a.get_val();
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return *this;
}

#define DEFN_ASN_OP_T(tp)                                                     \
inline                                                                        \
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator = ( tp a )                                            \
{                                                                             \
    sc_fxval_fast tmp( a );                                                   \
    _val = tmp.get_val();                                                     \
    cast();                                                                   \
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}

DEFN_ASN_OP_T(int)
DEFN_ASN_OP_T(unsigned int)
DEFN_ASN_OP_T(long)
DEFN_ASN_OP_T(unsigned long)
DEFN_ASN_OP_T(double)
DEFN_ASN_OP_T(const char*)
DEFN_ASN_OP_T(const sc_fxval&)
DEFN_ASN_OP_T(const sc_fxnum&)
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
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator op ( tp b )                                           \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( *this )                                     \
    sc_fxval_fast tmp( b );                                                   \
    _val op tmp.get_val();                                                    \
    cast();                                                                   \
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )                                    \
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
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator op ( const sc_fxnum_fast& b )                         \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( *this )                                     \
    _SC_FXNUM_FAST_OBSERVER_READ( b )                                         \
    _val op b._val;                                                           \
    cast();                                                                   \
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}                                                                             \
                                                                              \
inline                                                                        \
sc_fxnum_fast&                                                                \
sc_fxnum_fast::operator op ( const sc_fxval_fast& b )                         \
{                                                                             \
    _SC_FXNUM_FAST_OBSERVER_READ( *this )                                     \
    _val op b.get_val();                                                      \
    cast();                                                                   \
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,int)                                                         \
DEFN_ASN_OP_T(op,unsigned int)                                                \
DEFN_ASN_OP_T(op,long)                                                        \
DEFN_ASN_OP_T(op,unsigned long)                                               \
DEFN_ASN_OP_T(op,double)                                                      \
DEFN_ASN_OP_T(op,const char*)                                                 \
DEFN_ASN_OP_T(op,const sc_fxval&)                                             \
DEFN_ASN_OP_T(op,const sc_fxnum&)                                             \
DEFN_ASN_OP_OTHER(op)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP_OTHER
#undef DEFN_ASN_OP


inline
sc_fxnum_fast&
sc_fxnum_fast::operator <<= ( int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    _val *= scfx_pow2( b );
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return *this;
}

inline
sc_fxnum_fast&
sc_fxnum_fast::operator >>= ( int b )
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    _val *= scfx_pow2( -b );
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return *this;
}


// auto-increment and auto-decrement

inline
const sc_fxval_fast
sc_fxnum_fast::operator ++ ( int )
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    double c = _val;
    _val = _val + 1;
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return sc_fxval_fast( c );
}

inline
const sc_fxval_fast
sc_fxnum_fast::operator -- ( int )
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    double c = _val;
    _val = _val - 1;
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return sc_fxval_fast( c );
}

inline
sc_fxnum_fast&
sc_fxnum_fast::operator ++ ()
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    _val = _val + 1;
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return *this;
}

inline
sc_fxnum_fast&
sc_fxnum_fast::operator -- ()
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    _val = _val - 1;
    cast();
    _SC_FXNUM_FAST_OBSERVER_WRITE( *this )
    return *this;
}


// bit selection

inline
const sc_fxnum_fast_bitref
sc_fxnum_fast::operator [] ( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl() );
}

inline
sc_fxnum_fast_bitref
sc_fxnum_fast::operator [] ( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( *this, i - _params.fwl() );
}

inline
const sc_fxnum_fast_bitref
sc_fxnum_fast::bit( int i ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl() );
}

inline
sc_fxnum_fast_bitref
sc_fxnum_fast::bit( int i )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    return sc_fxnum_fast_bitref( *this, i - _params.fwl() );
}


// part selection

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::operator () ( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::operator () ( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( *this, i - _params.fwl(), j - _params.fwl() );
}

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::range( int i, int j ) const
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( const_cast<sc_fxnum_fast&>( *this ),
				 i - _params.fwl(), j - _params.fwl() );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::range( int i, int j )
{
    _SC_ERROR_IF( i < 0 || i >= _params.wl(), _SC_ID_OUT_OF_RANGE );
    _SC_ERROR_IF( j < 0 || j >= _params.wl(), _SC_ID_OUT_OF_RANGE );

    return sc_fxnum_fast_subref( *this, i - _params.fwl(), j - _params.fwl() );
}

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::operator () () const
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::operator () ()
{
    return this->operator () ( _params.wl() - 1, 0 );
}

inline
const sc_fxnum_fast_subref
sc_fxnum_fast::range() const
{
    return this->range( _params.wl() - 1, 0 );
}

inline
sc_fxnum_fast_subref
sc_fxnum_fast::range()
{
    return this->range( _params.wl() - 1, 0 );
}


// implicit conversion

inline
sc_fxnum_fast::operator double() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return _val;
}


// explicit conversion to primitive types

inline
short
sc_fxnum_fast::to_short() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<short>( _val );
}

inline
unsigned short
sc_fxnum_fast::to_ushort() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<unsigned short>( _val );
}

inline
int
sc_fxnum_fast::to_int() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<int>( _val );
}

inline
unsigned int
sc_fxnum_fast::to_uint() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<unsigned int>( _val );
}

inline
long
sc_fxnum_fast::to_long() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<long>( _val );
}

inline
unsigned long
sc_fxnum_fast::to_ulong() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<unsigned long>( _val );
}

inline
float
sc_fxnum_fast::to_float() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return static_cast<float>( _val );
}

inline
double
sc_fxnum_fast::to_double() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return _val;
}


// query value

inline
bool
sc_fxnum_fast::is_neg() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return ( id.negative() != 0 );
}

inline
bool
sc_fxnum_fast::is_zero() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return id.is_zero();
}

// internal use only;
inline
bool
sc_fxnum_fast::is_normal() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    scfx_ieee_double id( _val );
    return ( id.is_normal() || id.is_subnormal() || id.is_zero() );
}


inline
bool
sc_fxnum_fast::quantization_flag() const
{
    return _q_flag;
}

inline
bool
sc_fxnum_fast::overflow_flag() const
{
    return _o_flag;
}


inline
const sc_fxval_fast
sc_fxnum_fast::value() const
{
    _SC_FXNUM_FAST_OBSERVER_READ( *this )
    return sc_fxval_fast( _val );
}


// query parameters

inline
int
sc_fxnum_fast::wl() const
{
    return _params.wl();
}

inline
int
sc_fxnum_fast::iwl() const
{
    return _params.iwl();
}

inline
sc_q_mode
sc_fxnum_fast::q_mode() const
{
    return _params.q_mode();
}

inline
sc_o_mode
sc_fxnum_fast::o_mode() const
{
    return _params.o_mode();
}

inline
int
sc_fxnum_fast::n_bits() const
{
    return _params.n_bits();
}


inline
const sc_fxtype_params&
sc_fxnum_fast::type_params() const
{
    return _params.type_params();
}


inline
const sc_fxcast_switch&
sc_fxnum_fast::cast_switch() const
{
    return _params.cast_switch();
}


// internal use only;
inline
void
sc_fxnum_fast::observer_read() const
{
    _SC_FXNUM_OBSERVER_READ( *this );
}


inline
ostream&
operator << ( ostream& os, const sc_fxnum_fast& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval
//
//  Fixed-point value type; arbitrary precision.
// ----------------------------------------------------------------------------

// public constructors

inline
sc_fxval::sc_fxval( const sc_fxnum& a,
		    sc_fxval_observer* observer_ )
: _rep( new scfx_rep( *a.get_rep() ) ),
  _observer( observer_ )
{
    _SC_FXVAL_OBSERVER_DEFAULT
    _SC_FXVAL_OBSERVER_CONSTRUCT( *this )
    _SC_FXVAL_OBSERVER_WRITE( *this )
}

inline
sc_fxval::sc_fxval( const sc_fxnum_fast& a,
		    sc_fxval_observer* observer_ )
: _rep( new scfx_rep( a.to_double() ) ),
  _observer( observer_ )
{
    _SC_FXVAL_OBSERVER_DEFAULT
    _SC_FXVAL_OBSERVER_CONSTRUCT( *this )
    _SC_FXVAL_OBSERVER_WRITE( *this )
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

#define DEFN_BIN_OP(op,fnc)                                                   \
DEFN_BIN_OP_T(op,fnc,const sc_fxnum_fast&)

DEFN_BIN_OP(*,mult)
DEFN_BIN_OP(+,add)
DEFN_BIN_OP(-,sub)
//DEFN_BIN_OP(/,div)
DEFN_BIN_OP_T(/,div,const sc_fxnum_fast&)

#undef DEFN_BIN_OP_T
#undef DEFN_BIN_OP


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

#define DEFN_BIN_FNC(fnc)                                                     \
DEFN_BIN_FNC_T(fnc,const sc_fxnum_fast&)

DEFN_BIN_FNC(mult)
DEFN_BIN_FNC(div)
DEFN_BIN_FNC(add)
DEFN_BIN_FNC(sub)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC


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


#define DEFN_REL_OP(op,ret)                                                   \
DEFN_REL_OP_T(op,ret,const sc_fxnum_fast&)

DEFN_REL_OP(<,result < 0)
DEFN_REL_OP(<=,result <= 0)
DEFN_REL_OP(>,result > 0 && result != 2)
DEFN_REL_OP(>=,result >= 0 && result != 2)
DEFN_REL_OP(==,result == 0)
DEFN_REL_OP(!=,result != 0)

#undef DEFN_REL_OP_T
#undef DEFN_REL_OP


// assignment operators

inline
sc_fxval&
sc_fxval::operator = ( const sc_fxnum& a )
{
    *_rep = *a.get_rep();
    _SC_FXVAL_OBSERVER_WRITE( *this )
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

DEFN_ASN_OP_T(const sc_fxnum_fast&)

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

#define DEFN_ASN_OP(op,fnc)                                                   \
inline                                                                        \
sc_fxval&                                                                     \
sc_fxval::operator op ( const sc_fxnum& b )                                   \
{                                                                             \
    _SC_FXVAL_OBSERVER_READ( *this )                                          \
    scfx_rep* new_rep = ::fnc ## __scfx_rep( *_rep, *b.get_rep() );           \
    delete _rep;                                                              \
    _rep = new_rep;                                                           \
    _SC_FXVAL_OBSERVER_WRITE( *this )                                         \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,fnc,const sc_fxnum_fast&)

DEFN_ASN_OP(*=,mult)
DEFN_ASN_OP(/=,div)
DEFN_ASN_OP(+=,add)
DEFN_ASN_OP(-=,sub)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast
//
//  Fixed-point value types; limited precision.
// ----------------------------------------------------------------------------

// public constructors

inline
sc_fxval_fast::sc_fxval_fast( const sc_fxnum& a,
			      sc_fxval_fast_observer* observer_ )
: _val( a.to_double() ),
  _observer( observer_ )
{
    _SC_FXVAL_FAST_OBSERVER_DEFAULT
    _SC_FXVAL_FAST_OBSERVER_CONSTRUCT( *this )
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
}

inline
sc_fxval_fast::sc_fxval_fast( const sc_fxnum_fast& a,
			      sc_fxval_fast_observer* observer_ )
: _val( a.get_val() ),
  _observer( observer_ )
{
    _SC_FXVAL_FAST_OBSERVER_DEFAULT
    _SC_FXVAL_FAST_OBSERVER_CONSTRUCT( *this )
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
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

#define DEFN_BIN_FNC(fnc,op)                                                  \
DEFN_BIN_FNC_T(fnc,op,const sc_fxval&)                                        \
DEFN_BIN_FNC_T(fnc,op,const sc_fxnum&)

DEFN_BIN_FNC(mult,*)
DEFN_BIN_FNC(div,/)
DEFN_BIN_FNC(add,+)
DEFN_BIN_FNC(sub,-)

#undef DEFN_BIN_FNC_T
#undef DEFN_BIN_FNC


// assignment operators

inline
sc_fxval_fast&
sc_fxval_fast::operator = ( const sc_fxnum_fast& a )
{
    _val = a.get_val();
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )
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

DEFN_ASN_OP_T(const sc_fxnum&)

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

#define DEFN_ASN_OP(op)                                                       \
inline                                                                        \
sc_fxval_fast&                                                                \
sc_fxval_fast::operator op ( const sc_fxnum_fast& b )                         \
{                                                                             \
    _SC_FXVAL_FAST_OBSERVER_READ( *this )                                     \
    _val op b.get_val();                                                      \
    _SC_FXVAL_FAST_OBSERVER_WRITE( *this )                                    \
    return *this;                                                             \
}                                                                             \
                                                                              \
DEFN_ASN_OP_T(op,const sc_fxnum&)

DEFN_ASN_OP(*=)
DEFN_ASN_OP(/=)
DEFN_ASN_OP(+=)
DEFN_ASN_OP(-=)

#undef DEFN_ASN_OP_T
#undef DEFN_ASN_OP


#endif

// Taf!
