//#line 1 "-"
  
//#line 1 "sc_logic_vector_bitop.m4"
///* -*- c++ -*- */

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

    sc_logic_vector_bitop.m4 - m4 template for bitwise operators for
                               sc_logic_vector

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


/*
 * Use process_m4.sh to generate this file
 *  AUTOMATICALLY GENERATED -- DO NOT EDIT
 */

/*
 *  USE m4 to process this file.
 *
 *  _define_ ^ to           &           |           ^ 
 *  _define_ xor_reduce to    and_reduce  or_reduce   xor_reduce
 *  _define_ '0' to     1         0         0
 */

sc_logic_vector&
sc_logic_vector::operator ^= ( const sc_logic_vector& rhs )
{
    if (&rhs == this)
        return *this;
    int leng = rhs.length();
    check_length(leng);

    if (overlapp(rhs)) {
        sc_logic* tmp_rhs = new sc_logic[leng];
        rhs.to_cpp_array(tmp_rhs);
        for (int i = leng - 1; i >= 0; --i) {
            *get_data_ptr(i) ^= tmp_rhs[i];
        }
        delete[] tmp_rhs;
    } else {
        for (int i = leng - 1; i >= 0; --i) {
            *get_data_ptr(i) ^= *rhs.get_data_ptr(i);
        }
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator ^= ( const sc_bool_vector& rhs )
{
    int leng = rhs.length();
    check_length(leng);

    for (int i = leng - 1; i >= 0; --i) {
        *get_data_ptr(i) ^= sc_logic((char)(*rhs.get_data_ptr(i)));
    }
    return *this;
}

sc_logic_vector&
sc_logic_vector::operator ^= ( const char* rhs )
{
    int leng = length();
    rhs += (strlen(rhs) - leng);
    for (int i = leng - 1; i >= 0; --i) {
        *get_data_ptr(i) ^= sc_logic(*rhs);
        ++rhs;
    }
    return *this;
}

sc_logic_vector
operator ^ ( const sc_logic_vector& a, const sc_logic_vector& b )
{
    int leng = b.length();
    a.check_length(leng);
    sc_logic* c = new sc_logic[leng];
    for (int i = leng - 1; i >= 0; --i) {
        c[i] = *a.get_data_ptr(i) ^ *b.get_data_ptr(i);
    }
    return sc_logic_vector( c, leng );
}

sc_logic_vector
operator ^ ( const sc_logic_vector& a, const sc_bool_vector& b )
{
    int leng = b.length();
    a.check_length(leng);
    sc_logic* c = new sc_logic[leng];
    for (int i = leng - 1; i >= 0; --i) {
        c[i] = *a.get_data_ptr(i) ^ sc_logic((char)(*b.get_data_ptr(i)));
    }
    return sc_logic_vector( c, leng );
}

sc_logic_vector
operator ^ ( const sc_logic_vector& a, const char* b )
{
    int leng = a.length();
    b += (strlen(b) - leng);
    sc_logic* c = new sc_logic[leng];
    for (int i = leng - 1; i >= 0; --i) {
        c[i] = *a.get_data_ptr(i) ^ sc_logic(*b);
        ++b;
    }
    return sc_logic_vector( c, leng );
}

sc_logic
sc_logic_vector::xor_reduce () const
{
    sc_logic result('0');
    for (int i = length() - 1; i >= 0; --i) {
        result = result ^ *get_data_ptr(i);
    }
    return result;
}
