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

    Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
#include "sc_bit.h"
#include "sc_exception.h"

// constructors
// MANDATORY
//  sc_bit ();
sc_bit::sc_bit (int i)
{
  if(i!=0 && i!=1)
    REPORT_ERROR(1006,"sc_bit");
  rep = (void*)this;
  val_or_index = i;
}
//  sc_bit (bool);
sc_bit::sc_bit (char c)
{
  if(c!='0' && c!='1')
    REPORT_ERROR(1006,"sc_bit");
  rep = (void*)this;
  val_or_index = c-'0';
}
// copy constructor
// MANDATORY
//  sc_bit (const sc_bit& );


// destructor
// MANDATORY
//  ~sc_bit ();

// assignments

// MANDATORY
//  sc_bit& operator = (const sc_bit&);
sc_bit& sc_bit::operator = (int i)
{
  if(i!=0 && i!=1)
    REPORT_ERROR(1006,"sc_bit");
  if ((void*)this == rep) { /* raw */
      val_or_index = i;
  } else {
      sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, i);
  }
  return *this;
}
//  sc_bit& operator = (bool);
sc_bit& sc_bit::operator = (char c)
{
  if(c!='0' && c!='1')
    REPORT_ERROR(1006,"sc_bit");
  if ((void*)this == rep) { /* raw */
      val_or_index = c-'0';
  } else {
      sc_bvrep_set_wb((sc_bvrep*)rep, 0, val_or_index, c-'0');
  }
  return *this;
}
// MANDATORY
sc_bit& sc_bit::operator &= (const sc_bit& s)
{
  return  operator &= ((bool)s);
}
sc_bit& sc_bit::operator &= (int i)
{
  if(i!=0 && i!=1)
    REPORT_ERROR(1006,"sc_bit");
  return operator &= ((bool)i);
}
//  sc_bit& operator &= (bool);
sc_bit& sc_bit::operator &= (char c)
{
  if(c!='0' && c!='1')
    REPORT_ERROR(1006,"sc_bit");
  return operator &= ((bool)(c-'0'));
}

// MANDATORY
sc_bit& sc_bit::operator |= (const sc_bit& s)
{
  return  operator |= ((bool)s);
}
sc_bit& sc_bit::operator |= (int i)
{
  if(i!=0 && i!=1)
    REPORT_ERROR(1006,"sc_bit");
  return operator |= ((bool)i);
}
//  sc_bit& operator |= (bool);
sc_bit& sc_bit::operator |= (char c)
{
  if(c!='0' && c!='1')
    REPORT_ERROR(1006,"sc_bit");
  return operator |= ((bool)(c-'0'));
}

// MANDATORY
sc_bit& sc_bit::operator ^= (const sc_bit& s)
{
  return  operator ^= ((bool)s);
}
sc_bit& sc_bit::operator ^= (int i)
{
  if(i!=0 && i!=1)
    REPORT_ERROR(1006,"sc_bit");
  return operator ^= ((bool)i);
}
//  sc_bit& operator ^= (bool);
sc_bit& sc_bit::operator ^= (char c)
{
  if(c!='0' && c!='1')
    REPORT_ERROR(1006,"sc_bit");
  return operator ^= ((bool)(c-'0'));
}

// conversions
// MANDATORY
char sc_bit::to_char () const
{
  return (bool)(*this);
}
// OPEN
// decided that operator bool will suffice
// bool to_bool () const;

// operators

// relational operators

// MANDATORY
#if !defined(__GNUC__)
bool operator == (const sc_bit& a, const sc_bit& b){return (bool)a==(bool)b;}
bool operator == (const sc_bit& a, int i){return (int)a==i;}
bool operator == (const sc_bit& a, bool b){return (bool)a==b;}
bool operator == (const sc_bit& a, char c){return (int)a==(int)(c-'0');}
bool operator == (int i, const sc_bit& a){return a==i;}
bool operator == (bool b, const sc_bit& a){return a==b;}
bool operator == (char c, const sc_bit& a){return a==c;}
// MANDATORY
bool operator != (const sc_bit& a, const sc_bit& b){return (bool)a!=(bool)b;}
bool operator != (const sc_bit& a, int i){return (int)a!=i;}
bool operator != (const sc_bit& a, bool b){return (bool)a!=b;}
bool operator != (const sc_bit& a, char c){return (int)a!=(int)(c-'0');}
bool operator != (int i, const sc_bit& a){return a!=i;}
bool operator != (bool b, const sc_bit& a){return a!=b;}
bool operator != (char c, const sc_bit& a){return a!=c;}
// bitwise operators and functions

// bitwise complement

// MANDATORY
const sc_bit operator ~ (const sc_bit& a)
{
  sc_bit result(a);
  result.val_or_index = !result.val_or_index;
  return result;
}



// bitwise or
// MANDATORY
const sc_bit operator | (const sc_bit& a, const sc_bit& b)
{
  sc_bit result(a);
  return result|=b;
}
const sc_bit operator | (const sc_bit& a, int i)
{
  sc_bit result(a);
  return result|=i;
}
const sc_bit operator | (const sc_bit& a, bool b)
{
  sc_bit result(a);
  return result|=b;
}
const sc_bit operator | (const sc_bit& a, char c)
{
  sc_bit result(a);
  return result|=c;
}
const sc_bit operator | (int i, const sc_bit& a)
{
  sc_bit result(a);
  return result|=i;
}
const sc_bit operator | (bool b, const sc_bit& a)
{
  sc_bit result(a);
  return result|=b;
}
const sc_bit operator | (char c, const sc_bit& a)
{
  sc_bit result(a);
  return result|=c;
}

// bitwise and

// MANDATORY
const sc_bit operator & (const sc_bit& a, const sc_bit& b)
{
  sc_bit result(a);
  return result&=b;
}
const sc_bit operator & (const sc_bit& a, int i)
{
  sc_bit result(a);
  return result&=i;
}
const sc_bit operator & (const sc_bit& a, bool b)
{
  sc_bit result(a);
  return result&=b;
}
const sc_bit operator & (const sc_bit& a, char c)
{
  sc_bit result(a);
  return result&=c;
}
const sc_bit operator & (int i, const sc_bit& a)
{
  sc_bit result(a);
  return result&=i;
}
const sc_bit operator & (bool b, const sc_bit& a)
{
  sc_bit result(a);
  return result&=b;
}
const sc_bit operator & (char c, const sc_bit& a)
{
  sc_bit result(a);
  return result&=c;
}

// bitwise exor

// MANDATORY
const sc_bit operator ^ (const sc_bit& a, const sc_bit& b)
{
  sc_bit result(a);
  return result^=b;
}
const sc_bit operator ^ (const sc_bit& a, int i)
{
  sc_bit result(a);
  return result&=i;
}
const sc_bit operator ^ (const sc_bit& a, bool b)
{
  sc_bit result(a);
  return result^=b;
}
const sc_bit operator ^ (const sc_bit& a, char c)
{
  sc_bit result(a);
  return result^=c;
}
const sc_bit operator ^ (int i, const sc_bit& a)
{
  sc_bit result(a);
  return result^=i;
}
const sc_bit operator ^ (bool b, const sc_bit& a)
{
  sc_bit result(a);
  return result^=b;
}
const sc_bit operator ^ (char c, const sc_bit& a)
{
  sc_bit result(a);
  return result^=c;
}
#endif
