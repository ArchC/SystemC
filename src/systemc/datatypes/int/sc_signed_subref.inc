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

  sc_signed_subref.h -- Proxy class that is declared in sc_signed.h.  

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


sc_signed_subref& 
sc_signed_subref::operator=(const sc_signed_subref& v)
{
  if (this == &v)
    return *this;

  return operator=(static_cast<sc_signed>(v));
}


sc_signed_subref& 
sc_signed_subref::operator=(const sc_signed& v)
{

  int l = sc_min(left, v.nbits - 1 + right);

  for (register int i = right; i <= l; ++i)
    snum->set(i, v.test(i - right));

  return *this;
  
}


sc_signed_subref& 
sc_signed_subref::operator=(const sc_unsigned_subref& v)
{
  return operator=(static_cast<sc_unsigned>(v));
}


sc_signed_subref& 
sc_signed_subref::operator=(const sc_unsigned& v)
{

  int l = sc_min(left, v.nbits - 1 + right);

  for (register int i = right; i <= l; ++i)
    snum->set(i, v.test(i - right));

  return *this;
  
}


sc_signed_subref& 
sc_signed_subref::operator=(const char* v)
{

  int nb = left - right + 1;
  int nd = DIV_CEIL(nb);

#ifdef SC_MAX_NBITS

  unsigned long d[MAX_NDIGITS];

#else

  unsigned long *d = new unsigned long[nd];

#endif

  vec_from_str(nb, nd, d, v);

  unsigned long val = 1;  // Bit value.
  int j = 0;   // Current digit in d.

  register int i = 0;  // Current bit in d.

  while (i < nb) {

    snum->set(i + right, (bool) (d[j] & val));

    ++i;

    if (i % BITS_PER_DIGIT == 0) {
      val = 1;
      ++j;
    }
    else
      val <<= 1;

  }

#ifndef SC_MAX_NBITS

  delete [] d;

#endif

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(int64 v) 
{

  uint64 v2 = (uint64) v;

  for (register int i = right; i <= left; ++i) {
    snum->set(i, static_cast<bool>( v2 & 1 ));
    v2 >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(uint64 v)
{

  for (register int i = right; i <= left; ++i) {
    snum->set(i, static_cast<bool>( v & 1 ));
    v >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(long v) 
{

  unsigned long v2 = (unsigned long) v;

  for (register int i = right; i <= left; ++i) {
    snum->set(i, static_cast<bool>( v2 & 1 ));
    v2 >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(unsigned long v)
{

  for (register int i = right; i <= left; ++i) {
    snum->set(i, static_cast<bool>( v & 1 ));
    v >>= 1;
  }

  return *this;

}


sc_signed_subref& 
sc_signed_subref::operator=(double v)
{

  is_bad_double(v);

  int nb = left - right + 1;
  int nd = DIV_CEIL(nb);

#ifdef SC_MAX_NBITS

  unsigned long d[MAX_NDIGITS];

#else

  unsigned long *d = new unsigned long[nd];

#endif

  if (v < 0)
    v = -v;

  register int i = 0;

  while (floor(v) && (i < nd)) {
#ifndef WIN32
    d[i++] = (unsigned long) floor(remainder(v, DIGIT_RADIX));
#else
    d[i++] = (unsigned long) floor(fmod(v, DIGIT_RADIX));
#endif
    v /= DIGIT_RADIX;
  }

  vec_zero(i, nd, d);

  unsigned long val = 1;  // Bit value.
  int j = 0;   // Current digit in d.

  i = 0;  // Current bit in d.

  while (i < nb) {

    snum->set(i + right, (bool) (d[j] & val));

    ++i;

    if (i % BITS_PER_DIGIT == 0) {
      val = 1;
      ++j;
    }
    else
      val <<= 1;

  }

#ifndef SC_MAX_NBITS

  delete [] d;

#endif

  return *this;
}

sc_signed_subref& 
sc_signed_subref::operator=(const sc_int_base& v)
{ 
  return operator=((int64) v); 
}

sc_signed_subref& 
sc_signed_subref::operator=(const sc_uint_base& v)
{ 
  return operator=((uint64) v); 
}

sc_signed_subref::operator sc_signed() const
{
  return sc_signed(snum, left, right);
}


istream& operator>>(istream& is, sc_signed_subref& u)
{

  sc_signed u2(u.left - u.right + 1);
  is >> u2;
  u = u2;
  return is;
}


ostream& operator<<(ostream& os, const sc_signed_subref& u)
{
  os << static_cast<sc_signed>(u);
  return os;
}


// End of file
