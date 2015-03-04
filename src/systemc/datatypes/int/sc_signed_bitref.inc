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

  sc_signed_bitref.h -- Proxy class that is declared in sc_signed.h.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


sc_signed_bitref&
sc_signed_bitref::operator=(const sc_signed_bitref& v)
{
  snum->set(index, v.snum->test(v.index));
  return *this;
}


sc_signed_bitref&
sc_signed_bitref::operator=(bool v)
{
  snum->set(index, v);
  return *this;
}


sc_signed_bitref&
sc_signed_bitref::operator&=(bool v)
{
  if (! v)
    snum->clear(index);
  return *this;
}


sc_signed_bitref&
sc_signed_bitref::operator|=(bool v)
{
  if (v)
    snum->set(index);
  return *this;
}


sc_signed_bitref&
sc_signed_bitref::operator^=(bool v)
{
  if (v)
    snum->invert(index);
  return *this;
}

sc_signed_bitref::operator bool() const
{
  return snum->test(index);
}


bool
sc_signed_bitref::operator~() const
{
  return (! snum->test(index));
}


istream&
operator>>(istream& is, sc_signed_bitref& u)
{

  char c;
  is >> c;

  if ((c == '0') || (c == '1'))
#ifndef WIN32
    u = static_cast<bool>( c - '0' );
#else
    u = (c != '0');
#endif
  else
    u = 0;

  return is;

}

ostream&
operator<<(ostream& os, const sc_signed_bitref& u)
{
  os << "01"[u.snum->test(u.index)];
  return os;
}

// End of file
