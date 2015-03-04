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

    sc_reslv_vector.cpp - implementation of sc_signal_resolved_vector

    Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_array.h"

#include "sc_bool_vector.h"
// #include "numeric_bit/numeric_bit.h"
#include "sc_reslv_vector.h"

static sc_signal<sc_logic>*
create_resolved_signal(void *)
{
    return (sc_signal<sc_logic> *) new sc_signal_resolved_t<sc_logic, sc_logic_resolve>;
}

/******************************************************************************
            DEFINITION OF PRIVATE AND PROTECTED MEMBER FUNCTIONS
******************************************************************************/

sc_array_base*
sc_signal_resolved_vector::vctor( const sc_array_base* p, int l, int r ) const
{
    return new sc_signal_resolved_vector( p, l, r );
}

sc_signal_resolved_vector::sc_signal_resolved_vector( const sc_array_base* p, int l, int r )
    : super( p, l, r )
{
    /* Intentionally Blank - all work done in super */
}

/******************************************************************************
               DEFINITIONS OF PUBLIC MEMBER FUNCTIONS
******************************************************************************/

sc_signal_resolved_vector::sc_signal_resolved_vector( int len )
    : super( len, create_resolved_signal)
{
    /* Intentionally Blank - all work done in super */
}

sc_signal_resolved_vector::sc_signal_resolved_vector( const char* nm, int len )
    : super( nm, len, create_resolved_signal)
{
    /* Intentionally Blank - all work done in super */
}

sc_signal_resolved_vector::sc_signal_resolved_vector( int len, const sc_signal_resolved_vector& ref )
    : super( len, (const super&) ref )
{
    /* Intentionally Blank - all work done in super */
}

sc_signal_resolved_vector::sc_signal_resolved_vector( const sc_signal_resolved_vector& ref )
    : super(ref)
{
    assert(false);
}

sc_signal_resolved_vector::~sc_signal_resolved_vector()
{
    /* Nothing to do - all work done in base class' destructor */
}

bool
sc_signal_resolved_vector::is_resolved() const
{
    return true;
}

sc_signal_resolved_vector&
sc_signal_resolved_vector::sub( int i, int j )
{
    return dynamic_cast<sc_signal_resolved_vector&>(sc_array_base::sub(i,j));
}

/******************************************************************************
      The WRITE methods
*******************************************************************************/

sc_signal_resolved_vector& 
sc_signal_resolved_vector::write(const char* nv)
{
    sc_logic tmp;
    int len = length();
    nv += (strlen(nv) - len);
    for (int i = len - 1; i >= 0; --i) {
        tmp = *nv;  
        ((sc_signal_resolved*) get_signal(i))->write(tmp);
        nv++;
    }
    return *this;
}

sc_signal_resolved_vector& 
sc_signal_resolved_vector::write(const sc_unsigned& nv) 
{
  // This could be made a little more efficient by writing individual signals
  // of the sc_signal_resolved_vector
    sc_logic_vector a(nv.length());
    a = nv;
    return write(a);
}

sc_signal_resolved_vector& 
sc_signal_resolved_vector::write(const sc_signed& nv) 
{
  // This could be made a little more efficient by writing individual signals
  // of the sc_signal_resolved_vector
    sc_logic_vector a(nv.length());
    a = nv;
    return write(a);
}

sc_signal_resolved_vector& 
sc_signal_resolved_vector::write(const sc_bool_vector& nv)
{
    int len = nv.length();
    check_length(len);
    sc_logic tmp;
    for (int i = length() - 1; i >= 0; --i) {
        tmp = char(*(nv.get_data_ptr(i))); 
        ((sc_signal_resolved*) get_signal(i))->write(tmp);
    }
    return *this;
}


/*****************************************************************************/

sc_signal_resolved_vector::sc_signal_resolved_vector( signal_rlv* x, signal_rlv* y )
    : super( x, y )
{
    /* Intentionally Blank - all work done in super */
}

sc_signal_resolved_vector::sc_signal_resolved_vector( signal_rlv* x, sc_signal_resolved* y )
    : super( x, y )
{
    /* Intentionally Blank - all work done in super */
}

sc_signal_resolved_vector::sc_signal_resolved_vector( sc_signal_resolved* x, signal_rlv* y )
    : super( x, y )
{
    /* Intentionally Blank - all work done in super */
}

sc_signal_resolved_vector::sc_signal_resolved_vector( sc_signal_resolved* x, sc_signal_resolved* y )
    : super( x, y )
{
    /* Intentionally Blank - all work done in super */
}

signal_rlv*
sc_signal_resolved_vector::get_concat_array( signal_rlv* x, signal_rlv* y )
{
    return (signal_rlv*) sc_array_base::get_concat_array( x, y, new_array_vv );
}

signal_rlv*
sc_signal_resolved_vector::get_concat_array( signal_rlv* x, sc_signal_resolved* y )
{
    return (signal_rlv*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
}

signal_rlv*
sc_signal_resolved_vector::get_concat_array( sc_signal_resolved* x, signal_rlv* y )
{
    return (signal_rlv*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
}

signal_rlv*
sc_signal_resolved_vector::get_concat_array( sc_signal_resolved* x, sc_signal_resolved* y )
{
    return (signal_rlv*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
}

sc_array_base*
sc_signal_resolved_vector::new_array_vv( sc_array_base* x, sc_array_base* y )
{
    return new signal_rlv( (signal_rlv*) x, (signal_rlv*) y );
}

sc_array_base*
sc_signal_resolved_vector::new_array_vs( sc_array_base* x, void* y )
{
    return new signal_rlv( (signal_rlv*) x, (sc_signal_resolved*) y );
}

sc_array_base*
sc_signal_resolved_vector::new_array_sv( void* x, sc_array_base* y )
{
    return new signal_rlv( (sc_signal_resolved*) x, (signal_rlv*) y );
}

sc_array_base*
sc_signal_resolved_vector::new_array_ss( void* x, void* y )
{
    return new signal_rlv( (sc_signal_resolved*) x, (sc_signal_resolved*) y );
}

signal_rlv&
sc_signal_resolved_vector::operator,(signal_rlv& y )
{
    return *(signal_rlv::get_concat_array( this, &y ));
}

signal_rlv&
sc_signal_resolved_vector::operator,(sc_signal_resolved& y )
{
    return *(signal_rlv::get_concat_array( this, &y ));
}


const signal_rlv&
sc_signal_resolved_vector::operator,(const signal_rlv& y ) const
{
    return *(signal_rlv::get_concat_array( (signal_rlv*)this, (signal_rlv*) &y ));
}

const signal_rlv&
sc_signal_resolved_vector::operator,(const sc_signal_resolved& y ) const
{
    return *(signal_rlv::get_concat_array( (signal_rlv*)this, (sc_signal_resolved*) &y ));
}

#if !defined(_MSC_VER)
const signal_rlv&
operator,( const sc_signal_resolved& x, const signal_rlv& y )
{
    return *(signal_rlv::get_concat_array( (sc_signal_resolved*) &x, (signal_rlv*) &y ));
}

signal_rlv&
operator,( sc_signal_resolved& x, signal_rlv& y )
{
    return *(signal_rlv::get_concat_array( &x, &y ));
}
#endif
