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

    sc_unsigned_bitref.h -- Proxy class that is declared in sc_unsigned.h.  

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
 
******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


sc_unsigned_bitref&
sc_unsigned_bitref::operator=(const sc_unsigned_bitref& v)
{
  snum->set(index, v.snum->test(v.index));
  return *this;
}


sc_unsigned_bitref&
sc_unsigned_bitref::operator=(bool v)
{
  snum->set(index, v);
  return *this;
}


sc_unsigned_bitref&
sc_unsigned_bitref::operator&=(bool v)
{
  if (! v)
    snum->clear(index);
  return *this;
}


sc_unsigned_bitref&
sc_unsigned_bitref::operator|=(bool v)
{
  if (v)
    snum->set(index);
  return *this;
}


sc_unsigned_bitref&
sc_unsigned_bitref::operator^=(bool v)
{
  if (v)
    snum->invert(index);
  return *this;
}

sc_unsigned_bitref::operator bool() const
{
  return snum->test(index);
}


bool
sc_unsigned_bitref::operator~() const
{
  return (! snum->test(index));
}


istream&
operator>>(istream& is, sc_unsigned_bitref& u)
{

  char c;
  is >> c;

  if ((c == '0') || (c == '1'))
#ifndef WIN32
    u = (c - '0');
#else
    u = (c != '0');
#endif
  else
    u = 0;

  return is;

}

ostream&
operator<<(ostream& os, const sc_unsigned_bitref& u)
{
  os << "01"[u.snum->test(u.index)];
  return os;
}

// End of file
