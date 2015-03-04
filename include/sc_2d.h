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

    sc_2d.h -- Two-dimensional arrays

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_2D_H
#define SC_2D_H

#include "sc_macros.h"
#include "sc_signal.h"

template< class A > class sc_signal_2d;

//  <summary> Two Dimensional Array </summary>
//
//  This template class is used to declare 2-dimensional arrays, where the
//  class argument T itslef must be an sc_array, because constructors,
//  destructors, and certain other methods are assumed.  Code will not compile
//  if T is not an sc_array or a derived class thereof.  Examples:
//  <srcblock>
//      sc_2d<sc_logic_vector> foo(16,32);
//      sc_2d<sc_unsigned> bar(8,8);
//  </srcblock>
//  The first declares and array, indexed from 0 to 15, of sc_logic_vector
//  of size 32 bits (indexed from 31 down to 0).  The second declares an array
//  (0,7) of sc_unsigned (which is itself a 1d array) of (7,0).
//
template< class A > class sc_2d : public sc_array_base {
    typedef typename A::elemtype atom_t;

public:
        // Constructor - with the lengths of the two dimensions
    sc_2d( int len0, int len1 );

    void sc_2d_init( int len1 );

        // Destructor
    ~sc_2d();

        // Returns the 1d array indexed by the logical index i
    DECLARE_SUBSCRIPT_OP(sc_2d<A>, A)

public:    
        // Attribute functions which allows for specification of the
	// dimension interested.
    int length(int dim = 0) const;

    sc_2d<A>& operator=(const sc_2d<A>& y);

#ifndef __SUNPRO_CC
    sc_2d<A>& operator=(const A y[]);
#endif

    sc_2d<A>& operator=(const atom_t y[])
    {
        int leng0 = length(0), k = 0;
        for (int i = 0; i < leng0; ++i) {
            for (int j = 0; j < leng0; ++j) {
                (*this)[i][j] = y[k++];
            }
        }
        return *this;
    }

#define SC2D_OPERATOR_EQ(type) \
    sc_2d<A>& operator=(const type y[]) { \
        int leng0 = length(0); \
        for (int i = 0; i < leng0; ++i) { \
            (*this)[i] = y[i]; \
        } \
        return *this; \
    }
    /* added typename qualification */
    SC2D_OPERATOR_EQ(typename A::asgn_type1);
    SC2D_OPERATOR_EQ(typename A::asgn_type2);
    SC2D_OPERATOR_EQ(typename A::asgn_type3);
    SC2D_OPERATOR_EQ(typename A::asgn_type4);
    SC2D_OPERATOR_EQ(typename A::asgn_type5);
#undef SC2D_OPERATOR_EQ

#define SC2D_CTOR(type) \
    sc_2d( int len0, int len1, const type y[] ) \
        : sc_array_base(len0) \
    { \
        sc_2d_init(len1); \
        (*this) = y; \
    }

    /* added typename qualification */
    SC2D_CTOR(typename A::asgn_type1);
    SC2D_CTOR(typename A::asgn_type2);
    SC2D_CTOR(typename A::asgn_type3);
    SC2D_CTOR(typename A::asgn_type4);
    SC2D_CTOR(typename A::asgn_type5);
#undef SC2D_CTOR

    sc_2d( int len0, int len1, const atom_t y[] )
        : sc_array_base(len0)
    {
        sc_2d_init(len1);
        (*this) = y;
    }

private:
    // These functions should never be called; since subvector
    // is reserved for special vectors only

        // Virtual constructor required by sc_array_base.
    sc_array_base* vctor( const sc_array_base* p, int l0, int r0 ) const;

private:
    // bool second_dim_specified;
    A** data;
};

#include "sc_2d_defs.h_"

#endif // SC_2D_H
