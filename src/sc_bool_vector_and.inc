//#line 1 "-"
   
//#line 1 "sc_bool_vector_bitop.m4"
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

    sc_bool_vector_bitop.m4 - m4 template for bitwise operators for bool_vector.

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
 *  _define_ & to           &           |           ^ 
 *  _define_ and_reduce to    and_reduce  or_reduce   xor_reduce
 *  _define_ true to     true        false       false
 *  _define_ and  to     and         or          xor
 */

sc_bool_vector&
sc_bool_vector::operator &= ( const sc_bool_vector& rhs )
{
    if (&rhs == this)
        return *this;
    int leng = rhs.length();
    check_length(leng);

    if (overlapp(rhs)) {
        const int tmp_alloc = leng * sizeof(bool);
        bool* tmp_rhs = (bool*) sc_mempool::allocate(tmp_alloc);
        rhs.to_cpp_array(tmp_rhs);
        for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
            // MSVC++ miscompiles the original; so we'll spell it out.
            bool* tmp = get_data_ptr(i);
            int tmp2 = *tmp;
            tmp2 &= int(tmp_rhs[i]);
            *tmp = (0 != tmp2);
#else
            *(get_data_ptr(i)) &= tmp_rhs[i];
#endif
        }
        sc_mempool::release(tmp_rhs, tmp_alloc);
    } else {
        for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
            // MSVC++ miscompiles the original; so we'll spell it out.
            bool* tmp = get_data_ptr(i);
            int tmp2 = *tmp;
            tmp2 &= int(*rhs.get_data_ptr(i));
            *tmp = (0 != tmp2);
#else
            *(get_data_ptr(i)) &= *(rhs.get_data_ptr(i));
#endif
        }
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator &= ( const sc_logic_vector& rhs )
{
    int leng = rhs.length();
    check_length(leng);

    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        // MSVC++ miscompiles the original; so we'll spell it out.
        bool* tmp = get_data_ptr(i);
        int tmp2 = *tmp;
        tmp2 &= int(rhs.get_data_ptr(i)->to_bool());
        *tmp = (0 != tmp2);
#else
        *(get_data_ptr(i)) &= rhs.get_data_ptr(i)->to_bool();
#endif
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::operator &= ( const char* rhs )
{
    int leng = length();
    rhs += (strlen(rhs) - leng);
    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        // MSVC++ miscompiles the original; so we'll spell it out.
        bool* tmp = get_data_ptr(i);
        int tmp2 = *tmp;
        tmp2 &= int(*rhs != '0');
        *tmp = (0 != tmp2);
#else
        *(get_data_ptr(i)) &= (*rhs != '0');
#endif
        ++rhs;
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::asgn_andop1( const sc_bool_vector* a, const sc_bool_vector* b )
{
    int leng = length();
    a->check_length(leng);
    b->check_length(leng);
    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        assign_help(i, (0 != (int(*(a->get_data_ptr(i))) & int(*(b->get_data_ptr(i))))));
#else
        assign_help(i, *(a->get_data_ptr(i)) & *(b->get_data_ptr(i)));
#endif
    }
    return *this;
}

sc_bool_vector&
sc_bool_vector::asgn_andop2( const sc_bool_vector* a, const char* b )
{
    int leng = length();
    a->check_length(leng);
    b += (strlen(b) - leng);
    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        assign_help(i, (0 != (int(*(a->get_data_ptr(i))) & int(*b != '0'))));
#else
        assign_help(i, *(a->get_data_ptr(i)) & (*b != '0'));
#endif
        ++b;
    }
    return *this;
}

bool
sc_bool_vector::and_reduce () const
{
    bool result = true;
    for (int i = length() - 1; i >= 0; --i) {
#ifdef WIN32
        result = (0 != (int(result) & int(*(get_data_ptr(i)))));
#else
        result = result & *(get_data_ptr(i));
#endif
    }
    return result;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const sc_bool_vector_andop1& x )
{
    const sc_bool_vector* a = x.a;
    const sc_bool_vector* b = x.b;
    for (int i = length() - 1; i >= 0; --i) {
#ifdef WIN32
        super::write_help( i, (0 != (int(*(a->get_data_ptr(i))) & int(*(b->get_data_ptr(i))))));
#else
        super::write_help( i, *(a->get_data_ptr(i)) & *(b->get_data_ptr(i)));
#endif
    }
    return *this;
}

sc_signal_bool_vector&
sc_signal_bool_vector::write( const sc_bool_vector_andop2& x )
{
    const sc_bool_vector* a = x.a;
    const char* b = x.b;
    int leng = length();
    a->check_length(leng);
    b += (strlen(b) - leng);

    for (int i = leng - 1; i >= 0; --i) {
#ifdef WIN32
        super::write_help( i, (0 != (int(*(a->get_data_ptr(i))) & int(*b != '0'))));
#else
        super::write_help( i, *(a->get_data_ptr(i)) & (*b != '0'));
#endif
        ++b;
    }
    return *this;
}
