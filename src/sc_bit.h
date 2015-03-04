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

    sc_bit.h -- Bit class.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/

#ifndef SC_BIT_H
#define SC_BIT_H

#include "sc_signal.h"
#include "sc_bvrep.h"
#include "sc_logic.h"

class sc_bit {
    friend class sc_bv_b;
    friend class sc_signal<sc_bit>;
    friend class sc_signal_optimize;
private:
    sc_bit(sc_bvrep* re, unsigned po)
    {
        rep = (void*)re;
        val_or_index = po;
    }
    void* rep;
    unsigned val_or_index;

public:
    sc_bit()
    {
        rep = (void*)this;
        val_or_index = 0; // must be initialized
    }
    sc_bit(const sc_bit& r)
    {
        rep = (void*)this;
        val_or_index = r.to_bool();
    }
    explicit sc_bit(bool r)
    {
        rep = (void*)this;
        val_or_index = r;
    }
    explicit sc_bit(const sc_logic& r)
    {
        rep = (void*)this;
        val_or_index = r.to_bool();
    }
    sc_bit& operator=(const sc_bit& r)
    {
        if ((void*)this == rep) { /* raw */
            val_or_index = r.to_bool();
        } else {
            sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, bool(r));
        }
        return *this;
    }
    sc_bit& operator=(bool r)
    {
        if ((void*)this == rep) { /* raw */
            val_or_index = r;
        } else {
            sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, r);
        }
        return *this;
    }
    sc_bit& operator=(const sc_logic& r)
    {
      return (*this)=r.to_bool();
    }
    operator bool() const
    {
        if ((void*)this == rep) { /* raw */
            return (bool)val_or_index;
        } else {
            return sc_bvrep_test_wb((sc_bvrep*)rep, 0, val_or_index);
        }
    }
    bool to_bool() const
    {
        if ((void*)this == rep) { /* raw */
            return (bool)val_or_index;
        } else {
            return sc_bvrep_test_wb((sc_bvrep*)rep, 0, val_or_index);
        }
    }
    sc_bit& operator&=(bool r)
    {
        if ((void*)this == rep) {
            val_or_index &= (unsigned)r;
        } else {
            bool q = sc_bvrep_test_wb((sc_bvrep*)rep, 0, val_or_index);
            q = q & r;
            sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, q);
        }
        return *this;
    }
    sc_bit& operator|=(bool r)
    {
        if ((void*)this == rep) {
            val_or_index |= (unsigned)r;
        } else {
            bool q = sc_bvrep_test_wb((sc_bvrep*)rep, 0, val_or_index);
            q = q | r;
            sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, q);
        }
        return *this;
    }
    sc_bit& operator^=(bool r)
    {
        if ((void*)this == rep) {
            val_or_index ^= (unsigned)r;
        } else {
            bool q = sc_bvrep_test_wb((sc_bvrep*)rep, 0, val_or_index);
            q = q ^ r;
            sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, q);
        }
        return *this;
    }
    bool is_bitref() const { return ((void*) this != rep); }

//------------------------------------------------------------
// VSIA compatibility issues
//------------------------------------------------------------
 public:

  // constructors
  // MANDATORY
  //  sc_bit ();
  sc_bit (int);
  //  sc_bit (bool);
  sc_bit (char);

  // copy constructor
  // MANDATORY
  //  sc_bit (const sc_bit& );


  // destructor
  // MANDATORY
  //  ~sc_bit ();

  // assignments

  // MANDATORY
  //  sc_bit& operator = (const sc_bit&);
  sc_bit& operator = (int);
  //  sc_bit& operator = (bool);
  sc_bit& operator = (char);

  // MANDATORY
  sc_bit& operator &= (const sc_bit&);
  sc_bit& operator &= (int);
  //  sc_bit& operator &= (bool);
  sc_bit& operator &= (char);

  // MANDATORY
  sc_bit& operator |= (const sc_bit&);
  sc_bit& operator |= (int);
  //  sc_bit& operator |= (bool);
  sc_bit& operator |= (char);

  // MANDATORY
  sc_bit& operator ^= (const sc_bit&);
  sc_bit& operator ^= (int);
  //  sc_bit& operator ^= (bool);
  sc_bit& operator ^= (char);

  // conversions
  // MANDATORY
  char to_char () const;
  // OPEN
  // decided that operator bool will suffice
  // bool to_bool () const;

  // operators

  // relational operators

  // gcc 2.95.2 has a bug that causes ambiguity with many types
#if !defined(__GNUC__)
  // MANDATORY
  friend bool operator == (const sc_bit&, const sc_bit&);
  friend bool operator == (const sc_bit&, int);
  friend bool operator == (const sc_bit&, bool);
  friend bool operator == (const sc_bit&, char);
  friend bool operator == (int, const sc_bit&);
  friend bool operator == (bool, const sc_bit&);
  friend bool operator == (char, const sc_bit&);
#endif
  // OPTIONAL
  friend bool equal (const sc_bit&, const sc_bit&);
  friend bool equal (const sc_bit&, int);
  friend bool equal (const sc_bit&, bool);
  friend bool equal (const sc_bit&, char);
  friend bool equal (int, const sc_bit&);
  friend bool equal (bool, const sc_bit&);
  friend bool equal (char, const sc_bit&);

  // MANDATORY
#if !defined(__GNUC__)
  friend bool operator != (const sc_bit&, const sc_bit&);
  friend bool operator != (const sc_bit&, int);
  friend bool operator != (const sc_bit&, bool);
  friend bool operator != (const sc_bit&, char);
  friend bool operator != (int, const sc_bit&);
  friend bool operator != (bool, const sc_bit&);
  friend bool operator != (char, const sc_bit&);
#endif
  // OPTIONAL
  friend bool not_equal (const sc_bit&, const sc_bit&);
  friend bool not_equal (const sc_bit&, int);
  friend bool not_equal (const sc_bit&, bool);
  friend bool not_equal (const sc_bit&, char);
  friend bool not_equal (int, const sc_bit&);
  friend bool not_equal (bool, const sc_bit&);
  friend bool not_equal (char, const sc_bit&);

  // bitwise operators and functions

  // bitwise complement

#if !defined(__GNUC__)
  // MANDATORY
  friend const sc_bit operator ~ (const sc_bit&);
#endif
  // OPTIONAL (RECOMMENDED?)
  sc_bit& b_not ();

  // OPTIONAL
  friend const sc_bit b_not (const sc_bit&);
  // RECOMMENDED
  friend void b_not (sc_bit& r, const sc_bit&);


  // bitwise or
  // MANDATORY
#if !defined(__GNUC__)
  friend const sc_bit operator | (const sc_bit&, const sc_bit&);
  friend const sc_bit operator | (const sc_bit&, int);
  friend const sc_bit operator | (const sc_bit&, bool);
  friend const sc_bit operator | (const sc_bit&, char);
  friend const sc_bit operator | (int, const sc_bit&);
  friend const sc_bit operator | (bool, const sc_bit&);
  friend const sc_bit operator | (char, const sc_bit&);
#endif
  // OPTIONAL
  friend const sc_bit b_or (const sc_bit&, const sc_bit&);
  friend const sc_bit b_or (const sc_bit&, int);
  friend const sc_bit b_or (const sc_bit&, bool);
  friend const sc_bit b_or (const sc_bit&, char);
  friend const sc_bit b_or (int, const sc_bit&);
  friend const sc_bit b_or (bool, const sc_bit&);
  friend const sc_bit b_or (char, const sc_bit&);

  // RECOMMENDED
  friend void b_or (sc_bit& r, const sc_bit&, const sc_bit&);
  friend void b_or (sc_bit& r, const sc_bit&, int);
  friend void b_or (sc_bit& r, const sc_bit&, bool);
  friend void b_or (sc_bit& r, const sc_bit&, char);
  friend void b_or (sc_bit& r, int, const sc_bit&);
  friend void b_or (sc_bit& r, bool, const sc_bit&);
  friend void b_or (sc_bit& r, char, const sc_bit&);


  // bitwise and

  // MANDATORY
#if !defined(__GNUC__)
  friend const sc_bit operator & (const sc_bit&, const sc_bit&);
  friend const sc_bit operator & (const sc_bit&, int);
  friend const sc_bit operator & (const sc_bit&, bool);
  friend const sc_bit operator & (const sc_bit&, char);
  friend const sc_bit operator & (int, const sc_bit&);
  friend const sc_bit operator & (bool, const sc_bit&);
  friend const sc_bit operator & (char, const sc_bit&);
#endif

  // OPTIONAL
  friend const sc_bit b_and (const sc_bit&, const sc_bit&);
  friend const sc_bit b_and (const sc_bit&, int);
  friend const sc_bit b_and (const sc_bit&, bool);
  friend const sc_bit b_and (const sc_bit&, char);
  friend const sc_bit b_and (int, const sc_bit&);
  friend const sc_bit b_and (bool, const sc_bit&);
  friend const sc_bit b_and (char, const sc_bit&);
  // RECOMMENDED
  friend void b_and (sc_bit& r, const sc_bit&, const sc_bit&);
  friend void b_and (sc_bit& r, const sc_bit&, int);
  friend void b_and (sc_bit& r, const sc_bit&, bool);
  friend void b_and (sc_bit& r, const sc_bit&, char);
  friend void b_and (sc_bit& r, int, const sc_bit&);
  friend void b_and (sc_bit& r, bool, const sc_bit&);
  friend void b_and (sc_bit& r, char, const sc_bit&);

  // bitwise exor

  // MANDATORY
#if !defined(__GNUC__)
  friend const sc_bit operator ^ (const sc_bit&, const sc_bit&);
  friend const sc_bit operator ^ (const sc_bit&, int);
  friend const sc_bit operator ^ (const sc_bit&, bool);
  friend const sc_bit operator ^ (const sc_bit&, char);
  friend const sc_bit operator ^ (int, const sc_bit&);
  friend const sc_bit operator ^ (bool, const sc_bit&);
  friend const sc_bit operator ^ (char, const sc_bit&);
#endif

  // OPTIONAL
  friend const sc_bit b_xor (const sc_bit&, const sc_bit&);
  friend const sc_bit b_xor (const sc_bit&, int);
  friend const sc_bit b_xor (const sc_bit&, bool);
  friend const sc_bit b_xor (const sc_bit&, char);
  friend const sc_bit b_xor (int, const sc_bit&);
  friend const sc_bit b_xor (bool, const sc_bit&);
  friend const sc_bit b_xor (char, const sc_bit&);


  // RECOMMENDED
  friend void b_xor (sc_bit& r, const sc_bit&, const sc_bit&);
  friend void b_xor (sc_bit& r, const sc_bit&, int);
  friend void b_xor (sc_bit& r, const sc_bit&, bool);
  friend void b_xor (sc_bit& r, const sc_bit&, char);
  friend void b_xor (sc_bit& r, int, const sc_bit&);
  friend void b_xor (sc_bit& r, bool, const sc_bit&);
  friend void b_xor (sc_bit& r, char, const sc_bit&);
//------------------------------------------------------------
};

#endif
