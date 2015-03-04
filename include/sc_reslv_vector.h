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

    sc_reslv_vector.h -- implementation of types sc_signal_resolved_vector.

    Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
      Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)
    Description of Modification:
      added sc_signal_resolved_vector::write(int) and
      sc_signal_resolved_vector::operator=(int)
      because sc_logic_vector constructor cannot intialize vector with int

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/

#ifndef SC_RESLV_VECTOR_H
#define SC_RESLV_VECTOR_H

#include "sc_resolved_array.h"
#include "sc_reslv.h"
#include "sc_logic_vector.h"

class sc_signal_resolved_vector;

typedef sc_signal_resolved_vector signal_rlv;

class sc_signal_resolved_vector :
    public sc_signal_resolved_array<sc_signal_logic_vector, sc_logic_resolve>
{
    friend class sc_clock;

public:
    typedef sc_signal_resolved_array<sc_signal_logic_vector, sc_logic_resolve> super;

public:
    explicit sc_signal_resolved_vector( int len );
    sc_signal_resolved_vector( const char* nm, int len );
    sc_signal_resolved_vector( int len, const sc_signal_resolved_vector& ref );

private:
    sc_signal_resolved_vector( const sc_signal_resolved_vector& ref );

public:
    virtual ~sc_signal_resolved_vector();
    bool is_resolved() const;

    /* operator[](int) inherited from superclass */

    DECLARE_PARTSEL(sc_signal_resolved_vector)

public:
    // void update(); inherited from superclass
    // read method inherited from sc_signal_array<sc_logic>
    // Now the write methods
    sc_signal_resolved_vector& write(const sc_array<sc_logic>& nv) 
    {
        return (sc_signal_resolved_vector&) super::write(nv);
    }
    sc_signal_resolved_vector& operator=(const sc_array<sc_logic>& nv)
      { return write(nv); }
    // Always a good idea to have XYZ& operator=(const XYZ&)
    sc_signal_resolved_vector& operator=(const sc_signal_resolved_vector& nv)
      { return write(nv.read()); }
    sc_signal_resolved_vector& write(const sc_logic* nv)
      { return (sc_signal_resolved_vector&) super::write(nv); }
    sc_signal_resolved_vector& operator=(const sc_logic* nv) { return write(nv); }
    sc_signal_resolved_vector& write(const char*);
    sc_signal_resolved_vector& operator=(const char *nv) { return write(nv); }
    sc_signal_resolved_vector& write(const sc_unsigned&);
    sc_signal_resolved_vector& operator=(const sc_unsigned& nv) { return write(nv); }
    sc_signal_resolved_vector& write(const sc_signed&);
    sc_signal_resolved_vector& operator=(const sc_signed& nv) { return write(nv); }
    sc_signal_resolved_vector& write(const sc_bool_vector&);
    sc_signal_resolved_vector& operator=(const sc_bool_vector& nv) { return write(nv); }
    sc_signal_resolved_vector& write(int i)
      {sc_signed s(length()); s=i; return write(s);}
    sc_signal_resolved_vector& operator=(int i) { return write(i); }


    // The virtual constructor that will be used for creating subarrays by sc_array_base
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;
    // The actual constructor that will be called by the virtual constructor
    sc_signal_resolved_vector( const sc_array_base* p, int l, int r );
    signal_rlv& operator,( signal_rlv& );
    signal_rlv& operator,( sc_signal_resolved& );
    const signal_rlv& operator,( const signal_rlv& ) const;
    const signal_rlv& operator,(const sc_signal_resolved& ) const;

protected:
    // Constructors for concatenation
    sc_signal_resolved_vector( signal_rlv*, signal_rlv* );
    sc_signal_resolved_vector( signal_rlv*, sc_signal_resolved* );
    sc_signal_resolved_vector( sc_signal_resolved*, signal_rlv* );
    sc_signal_resolved_vector( sc_signal_resolved*, sc_signal_resolved* );

public:
    static signal_rlv* get_concat_array( signal_rlv*, signal_rlv* );
    static signal_rlv* get_concat_array( signal_rlv*, sc_signal_resolved* );
    static signal_rlv* get_concat_array( sc_signal_resolved*, signal_rlv* );
    static signal_rlv* get_concat_array( sc_signal_resolved*, sc_signal_resolved* );

protected:
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );
};

#if !defined(_MSC_VER)
extern signal_rlv& operator,( sc_signal_resolved&, signal_rlv& );
extern const signal_rlv& operator,( const sc_signal_resolved&, const signal_rlv& );
#endif

#endif
