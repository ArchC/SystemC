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
  sc_bv.cpp (.h) -- packed bit vector class

  Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/



/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
#include "sc_bool_vector.h"
#include "sc_logic_vector.h"
#include "sc_bv.h"

namespace sc_bv_ns
{
//-------------------- sc_bv_base implementation ------------------------------
//-----------------------------------------------------------------------------
void sc_bv_base::clean_tail()
{
   const unsigned reserved = (N-1)/ul_size +1;
   const unsigned tail = ul_size-N % ul_size;
   if(tail<ul_size)
     data[reserved-1] &= ~0ul<<tail>>tail;
}

void sc_bv_base::init(unsigned long length)
{
   N=0;
   if(length==0)
     REPORT_ERROR(2001,"");
   const unsigned reserved = (length-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   N=length;
   for(unsigned i=0; i<reserved; i++)
     data[i]=0ul;
}

sc_bv_base::sc_bv_base(const sc_bool_vector& v)
{
  init(v.length());
  for(unsigned i=0; i<N; i++)
    set_bit(i, v[i]);
  clean_tail();
}

sc_bv_base& sc_bv_base::operator=(const sc_bool_vector& t)
{
  return assign_(t);
}

void sc_bv_base::set_packed_rep(const unsigned long* new_buffer)
{ // no range check
  const unsigned reserved = (length()-1)/ul_size +1;
  for(register unsigned i=0; i<reserved; i++)
    data[i]=new_buffer[i];
  clean_tail();
}

sc_bv_base::sc_bv_base(unsigned length, bool init_value):N(0)
{ // slow
   if(length==0)
     REPORT_ERROR(2001,"");
   const unsigned reserved = (length-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   N=length;
   for(register unsigned i =0; i<reserved; i++)
     data[i] = init_value? 0ul:~0ul;
   clean_tail();
}
sc_bv_base::sc_bv_base(const sc_bv_base& b):N(0)
{  // fast
   const unsigned reserved = (b.N-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   N=b.N;
   sc_bv_ns::assign_((*this),b);
   clean_tail();
}
sc_bv_base::sc_bv_base(const char* ss):N(0)
{ // slow
   const sc_string s = convert_to_bin(ss);
   const unsigned s_length = s.length();
   if(s_length==0)
     REPORT_ERROR(2001,"");
   sc_string fs = convert_to_bin(s);
   assign_(fs.c_str());
   clean_tail();
}

void sc_bv_base::set(unsigned long v) // fill with v
{
  const unsigned reserved =  N/ul_size;
  for(unsigned i=0; i<reserved; i++)
    data[i] = v;
}

// if the vector shrinks extra data are discarded
void sc_bv_base::resize(unsigned long new_size)
{
  if(new_size==N) return;
  if(new_size==0)
   REPORT_ERROR(2001,"");
  int reserved = (new_size-1)/ul_size +1;
  int len = ((int)N-1)/ul_size +1;
  // no reallocation necessary if the reserved sizes are the same
  if(reserved==len)
  {
    unsigned long* new_data = new unsigned long[reserved];
    while(--reserved>=0)
      if(reserved<len)
        new_data[reserved] = data[reserved];
      else
        new_data[reserved] = 0ul;
    if(N) delete[] data;
    data = new_data;
  }
  N = new_size;
  clean_tail();
}


// non-template non-member functions
// string conversions
// there is time and memory penalty if one uses formated strings
sc_string convert_to_bin(const sc_string& s)
{
#if defined(FORMATTED_STRING_CONVERSION)
  if(s.length()>2 && s[0]=='0' &&
     (s[1]=='b' || s[1]=='B' || s[1]=='d' || s[1]=='D' || s[1]=='o' || s[1]=='O'
      || s[1]=='x' || s[1]=='X')) // formated string
  {
    sc_unsigned u(s.c_str());
    return u.to_string(SC_BIN_BASE, false); // no prefix
  }
  else // must be bin by default
#endif
    return s;
}
sc_string convert_to_logic(const sc_string& s)
{
#if defined(FORMATTED_STRING_CONVERSION)
  if(s.length()>2 && s[0]=='0' &&
     (s[1]=='b' || s[1]=='B' || s[1]=='d' || s[1]=='D' || s[1]=='o' || s[1]=='O'
      )) // formated string, notice X is a valid logic value
  {
    sc_unsigned u(s.c_str());
    return u.to_string(SC_BIN_BASE, false); // no prefix
  }
  else // must be logic by default
#endif
    return s;
}
} // namespace
