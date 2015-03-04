/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2001 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_lv.cpp -- Packed logic vector class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc/datatypes/bit/sc_lv.h"

namespace sc_bv_ns
{
//-------------------- sc_bv_base implementation ------------------------------

sc_lv_base::sc_lv_base( const sc_logic& y )
{
    init( 1 );
    set_bit( 0, y.to_long() );
}

void sc_lv_base::clean_tail()
{
   const unsigned reserved = (length()-1)/ul_size +1;
   const unsigned tail = ul_size-length() % ul_size;
   if(tail<ul_size)
   {
     data[reserved-1] &= ~0ul<<tail>>tail;
     control[reserved-1] &= ~0ul<<tail>>tail;
   }
}

void sc_lv_base::init(unsigned long length)
{
   N=0;
   if(length==0)
     REPORT_ERROR(2001,"");
   const unsigned reserved = (length-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   control = new unsigned long[reserved]; // can throw
   N=length;
   for(register unsigned i =0; i<length; i++)
     set_bit(i,sc_logic::Log_X);
   clean_tail();
}

sc_lv_base::sc_lv_base(unsigned length, long init_value):N(0)
{ // slow
   if(length==0)
     REPORT_ERROR(2001,"");
   const unsigned reserved = (length-1)/ul_size +1;
   data = new unsigned long[reserved]; // can throw
   control = new unsigned long[reserved]; // can throw
   N=length;
   for(register unsigned i =0; i<length; i++)
     set_bit(i,init_value);
   clean_tail();
}

void sc_lv_base::set_packed_rep(const unsigned long* new_buffer)
{ // no range check
  const unsigned reserved = (length()-1)/ul_size +1;
  for(register unsigned i=0; i<reserved; i++)
    data[i]=new_buffer[i];
   clean_tail();
}

void sc_lv_base::set_control_rep(const unsigned long* new_buffer)
{ // no range check
  const unsigned reserved = (length()-1)/ul_size +1;
  for(register unsigned i=0; i<reserved; i++)
    control[i]=new_buffer[i];
  clean_tail();
}

// if the vector shrinks extra data are discarded
void sc_lv_base::resize(unsigned long new_size)
{
  if(new_size==N) return;
  if(new_size==0)
   REPORT_ERROR(2001,"");
  int reserved = (new_size-1)/ul_size +1;
  int len = ((int)N-1)/ul_size +1;
  // no reallocation necessary if the previous size is big enough
  if(reserved > len)
  {
    unsigned long* new_data = new unsigned long[reserved];
    unsigned long* new_control = new unsigned long[reserved];
    while(--reserved>=0)
      if(reserved<len)
      {
        new_data[reserved] = data[reserved];
        new_control[reserved] = control[reserved];
      }
      else
      {
        new_data[reserved] = 0ul;
        new_control[reserved] = 0ul;
      }
    if( N ) {
      delete[] data;
      delete[] control;
    }
    data = new_data;
    control = new_control;
  }
  N = new_size;
  clean_tail();
}

// returns true if logic vector contains only 0's and 1's
bool sc_lv_base::is_01()
{
  bool result=true;
  unsigned i=0;
  const unsigned reserved = (N-1)/ul_size+1;
  while(result && i<reserved)
    result = get_cword(i++)==0;
  return result;
}

// ----------------------------
char long_buffer[33];
char* long_to_char(unsigned long l)
{
   for(int i=0; i<32; i++)
     long_buffer[31-i]= (l>>i)&1?'1':'0';
   long_buffer[32]=0;
   return long_buffer;
}

// non-template non-member functions

}
