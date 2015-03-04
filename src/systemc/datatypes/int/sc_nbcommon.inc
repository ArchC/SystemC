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

  sc_nbcommon.cpp -- Functions common to both sc_signed and sc_unsigned.
                     This file is included in sc_signed.cpp and
                     sc_unsigned.cpp after the macros are defined accordingly.
                     For example, sc_signed.cpp will first define CLASS_TYPE
                     as sc_signed before including this file. This file like
                     sc_nbfriends.cpp and sc_nbexterns.cpp is created in order
                     to ensure only one version of each function, regardless
                     of the class that they interface to.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// SECTION: Public members.
/////////////////////////////////////////////////////////////////////////////

// Create a CLASS_TYPE number with nb bits.
CLASS_TYPE::CLASS_TYPE(int nb)
{

  sgn = default_sign();

  if (nb > 0)
    nbits = num_bits(nb);
  else {
    printf("SystemC error: Zero sized numbers are not allowed.\n");
    abort();
  }
  ndigits = DIV_CEIL(nbits);

#ifdef SC_MAX_NBITS
  test_bound(nb);
#else
  digit = new unsigned long[ndigits];
#endif

}


// Create a CLASS_TYPE number from v. The number can have more bits
// than v implies because it is unnecessarily slower to determine the
// exact number of bits.
CLASS_TYPE::CLASS_TYPE(const char *v)
{

  // Check for errors:
  if (!v) {
    printf("SystemC error: Null char string is not allowed.\n");
    abort();
  }

  // Get the base b and sign s in v.
  small_type b, s;

  // v2 is equal to v without the format specifier if any.
  const char *v2 = get_base_and_sign(v, b, s);

  // Find an upper bound on the number of bits in v. The +1 is to
  // ensure that 2 bits are allocated for a number like '0b1' or 5
  // bits are allocated for a number '0d8'. If the user intends a
  // negative number, then s/he should enter '0b-1'. Also, if s/he
  // intends a 1 bit number, s/he should use the constructor that
  // takes the number of bits as an argument.
  int nb = strlen(v2);

  switch (b) {
  case SC_BIN: nb = nb + 1; break;
  case SC_OCT: nb = 3 * nb + 1; break;
  case SC_DEC: nb = 4 * nb + 1; break;
  case SC_HEX: nb = 4 * nb + 1; break;
  default:
    assert(false); break; // Other cases are not possible.
  }

  // Create a number v_num out of v.
  CLASS_TYPE v_num(nb);

  v_num = v;

  // Copy v_num to 'this' number. If v_num is zero, then this number
  // needs only one bit.

  if (v_num == 0) {
    sgn = SC_ZERO;
    nbits = num_bits(1);
    ndigits = 1;

#ifndef SC_MAX_NBITS
    digit = new unsigned long[1];
#endif

    digit[0] = 0;

  }
  else {

    sgn = v_num.sgn;
    nbits = v_num.nbits;
    ndigits = v_num.ndigits;

#ifndef SC_MAX_NBITS
    digit = new unsigned long[ndigits];
#endif

    vec_copy(ndigits, digit, v_num.digit);

  }

}


// Create a copy of v with sgn s. v is of the same type.
CLASS_TYPE::CLASS_TYPE(const CLASS_TYPE& v)
{

  sgn = v.sgn;
  nbits = v.nbits;
  ndigits = v.ndigits;

#ifndef SC_MAX_NBITS
  digit = new unsigned long[ndigits];
#endif

  vec_copy(ndigits, digit, v.digit);

}


// Create a copy of v where v is of the different type.
CLASS_TYPE::CLASS_TYPE(const OTHER_CLASS_TYPE& v)
{

  sgn = v.sgn;
  nbits = num_bits(v.nbits);

#if (IF_SC_SIGNED == 1)
  ndigits = v.ndigits;
#else
  ndigits = DIV_CEIL(nbits);
#endif

#ifndef SC_MAX_NBITS
  digit = new unsigned long[ndigits];
#endif

  copy_digits(v.nbits, v.ndigits, v.digit);

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Public members - Assignment operators.
/////////////////////////////////////////////////////////////////////////////

// Assignment from v of the same type.
CLASS_TYPE&
CLASS_TYPE::operator=(const CLASS_TYPE& v)
{

  if (this != &v) {

    sgn = v.sgn;

    if (sgn == SC_ZERO)
      vec_zero(ndigits, digit);

    else
      copy_digits(v.nbits, v.ndigits, v.digit);

  }

  return *this;

}


// Assignment from v of the different type.
CLASS_TYPE&
CLASS_TYPE::operator=(const OTHER_CLASS_TYPE& v)
{

  sgn = v.sgn;

  if (sgn == SC_ZERO)
    vec_zero(ndigits, digit);

  else
    copy_digits(v.nbits, v.ndigits, v.digit);
    
  return *this;

}


// Assignment from a subref v of the same type.
CLASS_TYPE& 
CLASS_TYPE::operator=(const CLASS_TYPE_SUBREF& v)
{
  return operator=(CLASS_TYPE(v));
}


// Assignment from a subref v of the different type.
CLASS_TYPE& 
CLASS_TYPE::operator=(const OTHER_CLASS_TYPE_SUBREF& v)
{
  return operator=(OTHER_CLASS_TYPE(v));
}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Input and output operators
/////////////////////////////////////////////////////////////////////////////

// Input operator. The sign and base of the number must be given in
// the input string itself. This behavior is preferred so that it
// matches the behavior of assignment from a char string. The input
// string should be terminated by a newline char. Note that
// vec_from_str() in sc_nbutils.cpp is similar to this function.
istream&
operator>>(istream& is, CLASS_TYPE& u) 
{

  char c;

  // Skip white space.
  while (is.get(c) && isspace(c))
    ;

  small_type b_cin;  // Base specified using I/O formatting.
  
  // Get the base specified using I/O formatting. Since base 10 is the
  // VSIA default base, there is no way to enter a base 2 number via
  // standard input.

#if 0
  fmtflags flags = is.flags();

  b_cin = NB_DEFAULT_BASE;

  if (flags & ios::hex)
    b_cin = SC_HEX;
  else if (flags & ios::oct)
    b_cin = SC_OCT;
#else
  b_cin = NB_DEFAULT_BASE;
#endif

  // Get the base specified in the input string itself.

  // State info for the FSM in fsm_move():
  const small_type STATE_START = 0;
  const small_type STATE_FINISH = 3;
  
  small_type state = STATE_START;
  small_type nskip = 0; // Read one more char if this is 1.

  // Default sign = SC_POS, default base = 10.
  small_type s = SC_POS;
  small_type b = NB_DEFAULT_BASE;

  while (is.good() && (! is.eof()) && (c != '\n')) {
    if (isspace(c))  // Skip white space.
      is.get(c);
    else {
      // Note that in sc_nbutils.cpp, we have nskip +=. The reason for
      // having nskip = below is that each call to get already
      // consumes one char from the file, so we cannot go back and
      // skip nskip chars again (also we don't want to deal with
      // putback). Instead, if the last value of nskip is 1, we read
      // the char that is going to be processed by the next
      // loop. Otherwise, the required char is already read.
      nskip = fsm_move(c, b, s, state);  
      if (state == STATE_FINISH)
        break;
      else
        is.get(c);
    }
  }

  if (nskip == 1)
    is.get(c);

  if (b == NB_DEFAULT_BASE)
    b = b_cin;
  else if (b != b_cin) {
    printf("SystemC error:\n");
    printf("The base set in the program does not match the base in the input string.\n");
    abort();
  }

  // Zero u.
  vec_zero(u.ndigits, u.digit);

  for ( ; is.good() && (! is.eof()) && (c != '\n'); is.get(c)) {

    if (isalnum(c)) {

      small_type val;

      if (isalpha(c)) // Hex digit.
        val = toupper(c) - 'A' + 10;
      else
        val = c - '0';
      
      if (val >= b) {
        printf("SystemC error: %c is not a valid digit in base %d\n", c, b);
        abort();
      }
      
      // digit = digit * b + val;
      vec_mul_small_on(u.ndigits, u.digit, b);
      
      if (val)
        vec_add_small_on(u.ndigits, u.digit, val);

    }
    else {
      printf("SystemC error: %c is not a valid digit in base %d\n", c, b);
      abort();      
    }
  }

  u.sgn = convert_signed_SM_to_2C_to_SM(s, u.nbits, u.ndigits, u.digit);

#ifdef SC_UNSIGNED
  u.convert_SM_to_2C_to_SM();
#endif

  return is;
  
}


// Output operator. It supports C++-style formatting of integer
// outputs. More specifically, it supports basefield specifier (hex,
// oct, or dec), adjustfield specifier (left, right, or internal), and
// other related specifiers (showbase, showpos, uppercase, width, and
// fill). If the output in binary is desired, the number u should
// first be copied into a string using to_string method and then
// printed. It is also possible to change this operator to print
// binary by default and decimal by explicit basefield setting of dec.
ostream&
operator<<(ostream& os, const CLASS_TYPE& u) 
{

  const char *xdigs = "0123456789abcdefx0123456789ABCDEFX";

  fmtflags flags = os.flags();

  char fill_ch = os.fill();            // Fill char.
  int field_len = os.width();  // Field size.
  int base_field_len = 0;      // Length of the base char(s).
  int sign_len = 0;            // Length of the sign char.
  int up_len = 0;              // Inx into xdigs.
  int fill_len = 0;            // Number of chars for padding/filling.
  small_type b;                        // Base.
  small_type s = u.sgn;                // Sign.

  fmtflags base_field = flags & ios::basefield;

  // Find base.
  if (base_field == ios::hex) {
    b = SC_HEX;

    // Should we output the hex base specifier 0x?
    if (flags & ios::showbase)
      base_field_len = 2;

    // Should we output all uppercase letters for x and a through f.
    if (flags & ios::uppercase)
      up_len = 17;

  }
  else if (base_field == ios::oct)
    b = SC_OCT;

  else
    b = NB_DEFAULT_BASE;  // Default base.

  register int nd = (s == SC_ZERO ? 0 : u.ndigits);

  // num_str will contain the output string prior to formatting.
  // Since there is no way to print the number in binary, we can
  // safely allocate 1/3 of the actual number of bits. 
#ifdef SC_MAX_NBITS
  char num_str[SC_MAX_NBITS / 3];
#else
  char *num_str = new char[(nd * BITS_PER_DIGIT) / 3];
#endif

  register int inx = 0;

  if (s != SC_ZERO) {

#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[nd];
#endif

  vec_copy(nd, d, u.digit);

#if (IF_SC_SIGNED == 1)  // For signed numbers.

    // If u is not initialized yet, determine its sign based on its
    // "random" bits.
    if (s == SC_NOSIGN)
      s = convert_signed_2C_to_SM(u.nbits, nd, d);

#else  // For unsigned numbers.

    s = convert_unsigned_SM_to_2C_to_SM(s, u.nbits, nd, d);

#endif

    nd = vec_skip_leading_zeros(nd, d);

    // An unsigned number with SC_POS or SC_NEG sign can actually be zero.
    if (nd == 0)
      s = SC_ZERO;

    // Get the number in base b from d into num_str.
    while (nd) {
      unsigned long r = vec_rem_on_small(nd, d, b);
      num_str[inx++] = xdigs[r + up_len];
      nd = vec_skip_leading_zeros(nd, d);
    }

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

  }

  // This 'if' is not 'else' of the above if block. Since s can be set
  // to zero inside the above if block, this 'if' has to be separate.
  if (s == SC_ZERO)
    num_str[inx++] = '0';

  // Find the length of the digits in num_str. There is no need to put
  // a null char in the end because num_str is not the output string.
  int num_str_len = inx;

  // Should we output the sign?
  if ((s == SC_NEG) || ((flags & ios::showpos) && (s == SC_POS)))
      sign_len = 1;

  // Should we output 0 for an octal number as its base specifier?
  if ((b == SC_OCT) && (flags & ios::showbase) && (s != SC_ZERO))
      base_field_len = 1;

  // Find the length of the output string.
  int out_str_len = num_str_len + base_field_len + sign_len;

  // Process the field width setting if any. If specified, out_str_len
  // must be updated accordingly. If not specified, no padding is
  // needed, and out_str_len does not change.
  if (field_len) {
    if (field_len > out_str_len) {
      fill_len = field_len - out_str_len;
      out_str_len = field_len;
    }
  }

  // Allocate memory for the output string out_str.
  char *out_str = new char[out_str_len + 1];

  // i will also index out_str, staring from 0.
  inx = 0;

  // Save adjustfield for efficiency.
  fmtflags adjust_flags = flags & ios::adjustfield;

  // For left or internal adjustments, print sign.
  if (adjust_flags != ios::right) {
    if (sign_len) {
      if (s == SC_POS)
        out_str[inx++] = '+';
      else if (s == SC_NEG)
        out_str[inx++] = '-';
    }
  }

  // For right and internal adjustments, perform padding.
  if (adjust_flags != ios::left) {
    while (fill_len--)
      out_str[inx++] = fill_ch;
  }

  // For right adjustment, print sign.
  if (adjust_flags == ios::right) {
    if (sign_len) {
      if (s == SC_POS)
        out_str[inx++] = '+';
      else if (s == SC_NEG)
        out_str[inx++] = '-';
    }    
  }

  // Should we print the basefield?
  if (base_field_len) {
    out_str[inx++] = '0';
    if (base_field_len == 2)
      out_str[inx++] = xdigs[16 + up_len];
  }

  // Print the number itself.
  while (--num_str_len >= 0)
    out_str[inx++] = num_str[num_str_len];

  // For left adjustment, perform padding.
  if (adjust_flags == ios::left) {
    while (fill_len--)
      out_str[inx++] = fill_ch;
  }

#ifndef SC_MAX_NBITS
  delete [] num_str;
#endif

  out_str[inx] = '\0';
 
  // Actual printing of the formatted number.
  os << out_str;

  delete [] out_str;

  return os;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: PLUS operators: +, +=, ++
/////////////////////////////////////////////////////////////////////////////

// Cases to consider when computing u + v:
// 1. 0 + v = v
// 2. u + 0 = u
// 3. if sgn(u) == sgn(v)
//    3.1 u + v = +(u + v) = sgn(u) * (u + v) 
//    3.2 (-u) + (-v) = -(u + v) = sgn(u) * (u + v)
// 4. if sgn(u) != sgn(v)
//    4.1 u + (-v) = u - v = sgn(u) * (u - v)
//    4.2 (-u) + v = -(u - v) ==> sgn(u) * (u - v)
//
// Specialization of above cases for computing ++u or u++: 
// 1. 0 + 1 = 1
// 3. u + 1 = u + 1 = sgn(u) * (u + 1)
// 4. (-u) + 1 = -(u - 1) = sgn(u) * (u - 1)

CLASS_TYPE&
CLASS_TYPE::operator+=(const CLASS_TYPE& v)
{

  // u = *this

  if (sgn == SC_ZERO) // case 1
    return (*this = v);

  if (v.sgn == SC_ZERO) // case 2
    return *this;

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              v.sgn, v.nbits, v.ndigits, v.digit);

  convert_SM_to_2C_to_SM();

  return *this; 

}


CLASS_TYPE&
CLASS_TYPE::operator+=(const OTHER_CLASS_TYPE& v)
{

  // u = *this

  if (sgn == SC_ZERO) // case 1
    return (*this = v);

  if (v.sgn == SC_ZERO) // case 2
    return *this;

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              v.sgn, v.nbits, v.ndigits, v.digit);

  convert_SM_to_2C_to_SM();

  return *this; 

}


CLASS_TYPE&
CLASS_TYPE::operator++() // prefix
{

  // u = *this

  if (sgn == SC_ZERO) { // case 1
    digit[0] = 1;
    sgn = SC_POS;
  }

  else if (sgn == SC_POS)   // case 3
    vec_add_small_on(ndigits, digit, 1);

  else // case 4
    vec_sub_small_on(ndigits, digit, 1);

  sgn = convert_signed_SM_to_2C_to_SM(sgn, nbits, ndigits, digit);

  return *this;   

}


const CLASS_TYPE
CLASS_TYPE::operator++(int) // postfix
{

  // Copy digit into d.

#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[ndigits];
#endif

  small_type s = sgn;

  vec_copy(ndigits, d, digit);

  ++(*this);

  return CLASS_TYPE(s, nbits, ndigits, d);

}


CLASS_TYPE&
CLASS_TYPE::operator+=(int64 v)
{

  // u = *this

  if (sgn == SC_ZERO)  // case 1
    return (*this = v);

  if (v == 0) // case 2
    return *this;

  CONVERT_INT64(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator+=(uint64 v)
{

  // u = *this

  if (sgn == SC_ZERO)  // case 1
    return (*this = v);

  if (v == 0)  // case 2
    return *this;

  CONVERT_INT64(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator+=(long v)
{

  // u = *this

  if (sgn == SC_ZERO)  // case 1
    return (*this = v);

  if (v == 0) // case 2
    return *this;

  CONVERT_LONG(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator+=(unsigned long v)
{

  // u = *this

  if (sgn == SC_ZERO)  // case 1
    return (*this = v);

  if (v == 0)  // case 2
    return *this;

  CONVERT_LONG(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: MINUS operators: -, -=, --
/////////////////////////////////////////////////////////////////////////////

// Cases to consider when computing u + v:
// 1. u - 0 = u 
// 2. 0 - v = -v
// 3. if sgn(u) != sgn(v)
//    3.1 u - (-v) = u + v = sgn(u) * (u + v)
//    3.2 (-u) - v = -(u + v) ==> sgn(u) * (u + v)
// 4. if sgn(u) == sgn(v)
//    4.1 u - v = +(u - v) = sgn(u) * (u - v) 
//    4.2 (-u) - (-v) = -(u - v) = sgn(u) * (u - v)
//
// Specialization of above cases for computing --u or u--: 
// 1. 0 - 1 = -1
// 3. (-u) - 1 = -(u + 1) = sgn(u) * (u + 1)
// 4. u - 1 = u - 1 = sgn(u) * (u - 1)

CLASS_TYPE&
CLASS_TYPE::operator-=(const CLASS_TYPE& v)
{

  // u = *this

  if (v.sgn == SC_ZERO)  // case 1
    return *this;

  if (sgn == SC_ZERO) { // case 2

    sgn = -v.sgn;
    copy_digits(v.nbits, v.ndigits, v.digit);   

  }
  else {
    
    // cases 3 and 4
    add_on_help(sgn, nbits, ndigits, digit, 
                -v.sgn, v.nbits, v.ndigits, v.digit);

    convert_SM_to_2C_to_SM();

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator-=(const OTHER_CLASS_TYPE& v)
{

  // u = *this

  if (v.sgn == SC_ZERO)  // case 1
    return *this;

  if (sgn == SC_ZERO) { // case 2

    sgn = -v.sgn;
    copy_digits(v.nbits, v.ndigits, v.digit);   

  }
  else {

    // cases 3 and 4
    add_on_help(sgn, nbits, ndigits, digit, 
                -v.sgn, v.nbits, v.ndigits, v.digit);

    convert_SM_to_2C_to_SM();
    
  }
  
  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator--() // prefix
{

  if (sgn == SC_ZERO) { // case 1
    digit[0] = 1;
    sgn = SC_NEG;
  }

  else if (sgn == SC_POS)   // case 3
    vec_sub_small_on(ndigits, digit, 1);

  else // case 4
    vec_add_small_on(ndigits, digit, 1);

  sgn = convert_signed_SM_to_2C_to_SM(sgn, nbits, ndigits, digit);

  return *this;   

}


const CLASS_TYPE
CLASS_TYPE::operator--(int) // postfix
{

  // Copy digit into d.

#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[ndigits];
#endif

  small_type s = sgn;

  vec_copy(ndigits, d, digit);

  --(*this);

  return CLASS_TYPE(s, nbits, ndigits, d);

}


CLASS_TYPE&
CLASS_TYPE::operator-=(int64 v)
{

  // u = *this

  if (v == 0) // case 1
    return *this;

  if (sgn == SC_ZERO) // case 2
    return (*this = -v);

  CONVERT_INT64(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              -vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator-=(uint64 v)
{

  // u = *this

  if (v == 0) // case 1
    return *this;

  int64 v2 = (int64) v;

  if (sgn == SC_ZERO) // case 2
    return (*this = -v2);

  CONVERT_INT64(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              -vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator-=(long v)
{

  // u = *this

  if (v == 0) // case 1
    return *this;

  if (sgn == SC_ZERO) // case 2
    return (*this = -v);

  CONVERT_LONG(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              -vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator-=(unsigned long v)
{

  // u = *this

  if (v == 0) // case 1
    return *this;

  long v2 = (long) v;

  if (sgn == SC_ZERO) // case 2
    return (*this = -v2);

  CONVERT_LONG(v);

  // cases 3 and 4
  add_on_help(sgn, nbits, ndigits, digit,
              -vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_SM_to_2C_to_SM();

  return *this;

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: MULTIPLICATION operators: *, *=
/////////////////////////////////////////////////////////////////////////////

// Cases to consider when computing u * v:
// 1. u * 0 = 0 * v = 0
// 2. 1 * v = v and -1 * v = -v
// 3. u * 1 = u and u * -1 = -u
// 4. u * v = u * v

CLASS_TYPE&
CLASS_TYPE::operator*=(const CLASS_TYPE& v)
{

  // u = *this

  sgn = mul_signs(sgn, v.sgn);

  if (sgn == SC_ZERO) // case 1
    vec_zero(ndigits, digit);

  else
    // cases 2-4
    MUL_ON_HELPER(sgn,  nbits, ndigits, digit,
                  v.nbits, v.ndigits, v.digit);

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator*=(const OTHER_CLASS_TYPE& v)
{

  // u = *this

  sgn = mul_signs(sgn, v.sgn);

  if (sgn == SC_ZERO) // case 1
    vec_zero(ndigits, digit);

  else 
    // cases 2-4
    MUL_ON_HELPER(sgn,  nbits, ndigits, digit,
                  v.nbits, v.ndigits, v.digit);

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator*=(int64 v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) // case 1
    vec_zero(ndigits, digit);

  else {  // cases 2-4

    CONVERT_INT64_2(v);

    MUL_ON_HELPER(sgn,  nbits, ndigits, digit,
                  BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator*=(uint64 v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) // case 1
    vec_zero(ndigits, digit);

  else { // cases 2-4

    CONVERT_INT64_2(v);

    MUL_ON_HELPER(sgn,  nbits, ndigits, digit,
                  BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator*=(long v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) // case 1
    vec_zero(ndigits, digit);

  else {   // cases 2-4

    CONVERT_LONG_2(v);

    MUL_ON_HELPER(sgn,  nbits, ndigits, digit,
                  BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator*=(unsigned long v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) // case 1
    vec_zero(ndigits, digit);

  else {  // cases 2-4

    CONVERT_LONG_2(v);

    MUL_ON_HELPER(sgn,  nbits, ndigits, digit,
                  BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  }

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: DIVISION operators: /, /=
/////////////////////////////////////////////////////////////////////////////

// Cases to consider when finding the quotient q = floor(u/v):
// Note that u = q * v + r for r < q.
// 1. 0 / 0 or u / 0 => error
// 2. 0 / v => 0 = 0 * v + 0
// 3. u / v && u = v => u = 1 * u + 0  - u or v can be 1 or -1
// 4. u / v && u < v => u = 0 * v + u  - u can be 1 or -1
// 5. u / v && u > v => u = q * v + r  - v can be 1 or -1

CLASS_TYPE&
CLASS_TYPE::operator/=(const CLASS_TYPE& v)
{

  sgn = mul_signs(sgn, v.sgn);

  if (sgn == SC_ZERO) {

    div_by_zero(v.sgn); // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else  // other cases
    DIV_ON_HELPER(sgn, nbits, ndigits, digit,
                  v.nbits, v.ndigits, v.digit);

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator/=(const OTHER_CLASS_TYPE& v)
{

  sgn = mul_signs(sgn, v.sgn);

  if (sgn == SC_ZERO) {

    div_by_zero(v.sgn); // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else  // other cases
    DIV_ON_HELPER(sgn, nbits, ndigits, digit,
                  v.nbits, v.ndigits, v.digit);

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator/=(int64 v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit); // case 2

  }
  else {

    CONVERT_INT64_2(v);

    // other cases
    DIV_ON_HELPER(sgn, nbits, ndigits, digit,
                  BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator/=(uint64 v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else {

    CONVERT_INT64_2(v);

    // other cases
    DIV_ON_HELPER(sgn, nbits, ndigits, digit,
                  BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator/=(long v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit); // case 2

  }
  else {

    CONVERT_LONG_2(v);

    // other cases
    DIV_ON_HELPER(sgn,  nbits, ndigits, digit,
                  BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator/=(unsigned long v)
{

  // u = *this

  sgn = mul_signs(sgn, get_sign(v));

  if (sgn == SC_ZERO) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else {
    
    CONVERT_LONG_2(v);

    // other cases
    DIV_ON_HELPER(sgn,  nbits, ndigits, digit,
                  BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  }

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: MOD operators: %, %=.
/////////////////////////////////////////////////////////////////////////////

// Cases to consider when finding the remainder r = u % v:
// Note that u = q * v + r for r < q.
// 1. 0 % 0 or u % 0 => error
// 2. 0 % v => 0 = 0 * v + 0
// 3. u % v && u = v => u = 1 * u + 0  - u or v can be 1 or -1
// 4. u % v && u < v => u = 0 * v + u  - u can be 1 or -1
// 5. u % v && u > v => u = q * v + r  - v can be 1 or -1

CLASS_TYPE&
CLASS_TYPE::operator%=(const CLASS_TYPE& v)
{

  if ((sgn == SC_ZERO) || (v.sgn == SC_ZERO)) {

    div_by_zero(v.sgn);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else // other cases
    MOD_ON_HELPER(sgn, nbits, ndigits, digit,
                  v.nbits, v.ndigits, v.digit);

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator%=(const OTHER_CLASS_TYPE& v)
{

  if ((sgn == SC_ZERO) || (v.sgn == SC_ZERO)) {

    div_by_zero(v.sgn);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else  // other cases
    MOD_ON_HELPER(sgn, nbits, ndigits, digit,
                  v.nbits, v.ndigits, v.digit);

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator%=(int64 v)
{

  small_type vs = get_sign(v);

  if ((sgn == SC_ZERO) || (vs == SC_ZERO)) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else {
    
    CONVERT_INT64_2(v);

    // other cases
    MOD_ON_HELPER(sgn, nbits, ndigits, digit,
                  BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator%=(uint64 v)
{

  if ((sgn == SC_ZERO) || (v == 0)) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else {
    
    CONVERT_INT64_2(v);

    // other cases
    MOD_ON_HELPER(sgn, nbits, ndigits, digit,
                  BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator%=(long v)
{

  small_type vs = get_sign(v);

  if ((sgn == SC_ZERO) || (vs == SC_ZERO)) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else {
    
    CONVERT_LONG_2(v);

    // other cases
    MOD_ON_HELPER(sgn, nbits, ndigits, digit,
                  BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator%=(unsigned long v)
{

  if ((sgn == SC_ZERO) || (v == 0)) {

    div_by_zero(v);  // case 1
    vec_zero(ndigits, digit);  // case 2

  }
  else {
    
    CONVERT_LONG_2(v);

    // other cases
    MOD_ON_HELPER(sgn, nbits, ndigits, digit,
                  BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  }

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Bitwise AND operators: &, &=
/////////////////////////////////////////////////////////////////////////////

// Cases to consider when computing u & v:
// 1. u & 0 = 0 & v = 0
// 2. u & v => sgn = +
// 3. (-u) & (-v) => sgn = -
// 4. u & (-v) => sgn = +
// 5. (-u) & v => sgn = +

CLASS_TYPE&
CLASS_TYPE::operator&=(const CLASS_TYPE& v)
{

  // u = *this

  if ((sgn == SC_ZERO) || (v.sgn == SC_ZERO)) // case 1
    makezero();

  else  {  // other cases

    and_on_help(sgn, nbits, ndigits, digit,
                v.sgn, v.nbits, v.ndigits, v.digit);

    convert_2C_to_SM();

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator&=(const OTHER_CLASS_TYPE& v)
{

  // u = *this

  if ((sgn == SC_ZERO) || (v.sgn == SC_ZERO)) // case 1
    makezero();

  else {  // other cases
    
    and_on_help(sgn, nbits, ndigits, digit,
                v.sgn, v.nbits, v.ndigits, v.digit);

    convert_2C_to_SM();

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator&=(int64 v)
{
  // u = *this

  if ((sgn == SC_ZERO) || (v == 0)) // case 1
    makezero();

  else {    // other cases

    CONVERT_INT64(v);

    and_on_help(sgn, nbits, ndigits, digit,
                vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

    convert_2C_to_SM();

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator&=(uint64 v)
{
  // u = *this

  if ((sgn == SC_ZERO) || (v == 0))  // case 1
    makezero();

  else {  // other cases

    CONVERT_INT64(v);

    and_on_help(sgn, nbits, ndigits, digit,
                vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

    convert_2C_to_SM();
    
  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator&=(long v)
{
  // u = *this

  if ((sgn == SC_ZERO) || (v == 0))  // case 1
    makezero();

  else {      // other cases

    CONVERT_LONG(v);

    and_on_help(sgn, nbits, ndigits, digit,
                vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

    convert_2C_to_SM();

  }

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator&=(unsigned long v)
{
  // u = *this

  if ((sgn == SC_ZERO) || (v == 0))  // case 1
    makezero();

  else {  // other cases

    CONVERT_LONG(v);

    and_on_help(sgn, nbits, ndigits, digit,
                vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

    convert_2C_to_SM();

  }

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Bitwise OR operators: |, |=
/////////////////////////////////////////////////////////////////////////////
// Cases to consider when computing u | v:
// 1. u | 0 = u
// 2. 0 | v = v
// 3. u | v => sgn = +
// 4. (-u) | (-v) => sgn = -
// 5. u | (-v) => sgn = -
// 6. (-u) | v => sgn = -

CLASS_TYPE&
CLASS_TYPE::operator|=(const CLASS_TYPE& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases
  or_on_help(sgn, nbits, ndigits, digit,
             v.sgn, v.nbits, v.ndigits, v.digit);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator|=(const OTHER_CLASS_TYPE& v)
{

  if (v.sgn == SC_ZERO)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases
  or_on_help(sgn, nbits, ndigits, digit,
             v.sgn, v.nbits, v.ndigits, v.digit);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator|=(int64 v)
{

  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_INT64(v);

  or_on_help(sgn, nbits, ndigits, digit,
             vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator|=(uint64 v)
{

  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_INT64(v);

  or_on_help(sgn, nbits, ndigits, digit,
             vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator|=(long v)
{

  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_LONG(v);

  or_on_help(sgn, nbits, ndigits, digit,
             vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator|=(unsigned long v)
{

  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_LONG(v);

  or_on_help(sgn, nbits, ndigits, digit,
             vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_2C_to_SM();

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Bitwise XOR operators: ^, ^=
/////////////////////////////////////////////////////////////////////////////
// Cases to consider when computing u ^ v:
// Note that  u ^ v = (~u & v) | (u & ~v).
// 1. u ^ 0 = u
// 2. 0 ^ v = v
// 3. u ^ v => sgn = +
// 4. (-u) ^ (-v) => sgn = -
// 5. u ^ (-v) => sgn = -
// 6. (-u) ^ v => sgn = +

CLASS_TYPE&
CLASS_TYPE::operator^=(const CLASS_TYPE& v)
{

  // u = *this

  if (v.sgn == SC_ZERO)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases
  xor_on_help(sgn, nbits, ndigits, digit,
              v.sgn, v.nbits, v.ndigits, v.digit);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator^=(const OTHER_CLASS_TYPE& v)
{

  // u = *this

  if (v.sgn == SC_ZERO)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases
  xor_on_help(sgn, nbits, ndigits, digit,
              v.sgn, v.nbits, v.ndigits, v.digit);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator^=(int64 v)
{

  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_INT64(v);

  xor_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator^=(uint64 v)
{
  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_INT64(v);

  xor_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_UINT64, DIGITS_PER_UINT64, vd);
  
  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator^=(long v)
{

  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_LONG(v);

  xor_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);

  convert_2C_to_SM();

  return *this;

}


CLASS_TYPE&
CLASS_TYPE::operator^=(unsigned long v)
{
  if (v == 0)  // case 1
    return *this;

  if (sgn == SC_ZERO)  // case 2
    return (*this = v);

  // other cases

  CONVERT_LONG(v);

  xor_on_help(sgn, nbits, ndigits, digit,
              vs, BITS_PER_ULONG, DIGITS_PER_ULONG, vd);
  
  convert_2C_to_SM();

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Bitwise NOT operator: ~
/////////////////////////////////////////////////////////////////////////////

CLASS_TYPE
operator~(const CLASS_TYPE& u)
{

  small_type s = u.sgn;

  if (s == SC_ZERO) {

    unsigned long d = 1;
    return CLASS_TYPE(SC_NEG, u.nbits, 1, &d, false);

  }

  int nd = u.ndigits;

#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[nd];
#endif

  vec_copy(nd, d, u.digit);

  if (s == SC_POS) {

    s = SC_NEG;
    vec_add_small_on(nd, d, 1);

  }
  else {

    s = SC_POS;
    vec_sub_small_on(nd, d, 1);

    if (check_for_zero(nd, d))
      s = SC_ZERO;

  }

  return CLASS_TYPE(s, u.nbits, nd, d);

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: LEFT SHIFT operators: <<, <<=
/////////////////////////////////////////////////////////////////////////////

CLASS_TYPE
operator<<(const CLASS_TYPE& u, const CLASS_TYPE& v)
{
  if (v.sgn == SC_ZERO) 
    return CLASS_TYPE(u);

#ifdef SC_SIGNED
  if (v.sgn == SC_NEG)
    return CLASS_TYPE(u);
#endif

  return operator<<(u, v.to_ulong());
}


CLASS_TYPE&
CLASS_TYPE::operator<<=(const CLASS_TYPE& v)
{
  if (v.sgn == SC_ZERO)
    return *this;

#ifdef SC_SIGNED
  if (v.sgn == SC_NEG)
    return *this;
#endif 

  return operator<<=(v.to_ulong());
}


CLASS_TYPE&
CLASS_TYPE::operator<<=(const OTHER_CLASS_TYPE& v)
{
  if (v.sgn == SC_ZERO)
    return *this;

#ifdef SC_UNSIGNED
  if (v.sgn == SC_NEG)
    return *this;
#endif

  return operator<<=(v.to_ulong());
}


CLASS_TYPE
operator<<(const CLASS_TYPE& u, int64 v)
{

  if (v <= 0)
    return CLASS_TYPE(u);

  return operator<<(u, (unsigned long) v);

}


CLASS_TYPE
operator<<(const CLASS_TYPE& u, uint64 v)
{

  if (v == 0)
    return CLASS_TYPE(u);

  return operator<<(u, (unsigned long) v);

}


CLASS_TYPE&
CLASS_TYPE::operator<<=(int64 v)
{

  if (v <= 0)
    return *this;

  return operator<<=((unsigned long) v);

}


CLASS_TYPE&
CLASS_TYPE::operator<<=(uint64 v)
{

  if (v == 0)
    return *this;

  return operator<<=((unsigned long) v);

}


CLASS_TYPE
operator<<(const CLASS_TYPE& u, long v)
{

  if (v <= 0)
    return CLASS_TYPE(u);

  return operator<<(u, (unsigned long) v);

}

CLASS_TYPE
operator<<(const CLASS_TYPE& u, unsigned long v)
{

  if (v == 0)
    return CLASS_TYPE(u);

  if (u.sgn == SC_ZERO)
    return CLASS_TYPE(u);

  int nb = u.nbits + v;
  int nd = DIV_CEIL(nb);

#ifdef SC_MAX_NBITS
  test_bound(nb);
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[nd];
#endif  

  vec_copy_and_zero(nd, d, u.ndigits, u.digit);

  convert_SM_to_2C(u.sgn, nd, d);

  vec_shift_left(nd, d, v);

  small_type s = convert_signed_2C_to_SM(nb, nd, d);

  return CLASS_TYPE(s, nb, nd, d);

}


CLASS_TYPE&
CLASS_TYPE::operator<<=(long v)
{

  if (v <= 0)
    return *this;

  return operator<<=((unsigned long) v);

}


CLASS_TYPE&
CLASS_TYPE::operator<<=(unsigned long v)
{

  if (v == 0)
    return *this;

  if (sgn == SC_ZERO)
    return *this;

  convert_SM_to_2C();

  vec_shift_left(ndigits, digit, v);

  convert_2C_to_SM();

  return *this;

}


/////////////////////////////////////////////////////////////////////////////
// SECTION: RIGHT SHIFT operators: >>, >>=
/////////////////////////////////////////////////////////////////////////////


CLASS_TYPE
operator>>(const CLASS_TYPE& u, const CLASS_TYPE& v)
{
  if (v.sgn == SC_ZERO)
    return CLASS_TYPE(u);

#ifdef SC_SIGNED
  if (v.sgn == SC_NEG)
    return CLASS_TYPE(u);
#endif

  return operator>>(u, v.to_long());
}


CLASS_TYPE&
CLASS_TYPE::operator>>=(const CLASS_TYPE& v)
{
  if (v.sgn == SC_ZERO)
    return *this;

#ifdef SC_SIGNED
  if (v.sgn == SC_NEG)
    return *this;
#endif

  return operator>>=(v.to_long());
}


CLASS_TYPE&
CLASS_TYPE::operator>>=(const OTHER_CLASS_TYPE& v)
{
  if (v.sgn == SC_ZERO)
    return *this;

#ifdef SC_UNSIGNED
  if (v.sgn == SC_NEG)
    return *this;
#endif

  return operator>>=(v.to_ulong());
}


CLASS_TYPE
operator>>(const CLASS_TYPE& u, int64 v)
{

  if (v <= 0)
    return CLASS_TYPE(u);

  return operator>>(u, (unsigned long) v);

}


CLASS_TYPE
operator>>(const CLASS_TYPE& u, uint64 v)
{

  if (v == 0)
    return CLASS_TYPE(u);

  return operator>>(u, (unsigned long) v);

}

CLASS_TYPE&
CLASS_TYPE::operator>>=(int64 v)
{

  if (v <= 0)
    return *this;

  return operator>>=((unsigned long) v);

}


CLASS_TYPE&
CLASS_TYPE::operator>>=(uint64 v)
{

  if (v == 0)
    return *this;

  return operator>>=((unsigned long) v);

}


CLASS_TYPE
operator>>(const CLASS_TYPE& u, long v)
{

  if (v <= 0)
    return CLASS_TYPE(u);

  return operator>>(u, (unsigned long) v);

}


CLASS_TYPE
operator>>(const CLASS_TYPE& u, unsigned long v)
{

  if (v == 0)
    return CLASS_TYPE(u);

  if (u.sgn == SC_ZERO)
    return CLASS_TYPE(u);

  int nb = u.nbits;
  int nd = u.ndigits;

#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[nd];
#endif  

  vec_copy(nd, d, u.digit);

  convert_SM_to_2C(u.sgn, nd, d);

  if (u.sgn == SC_NEG)
    vec_shift_right(nd, d, v, DIGIT_MASK);
  else
    vec_shift_right(nd, d, v, 0);
    
  small_type s = convert_signed_2C_to_SM(nb, nd, d);

  return CLASS_TYPE(s, nb, nd, d);

}


CLASS_TYPE&
CLASS_TYPE::operator>>=(long v)
{

  if (v <= 0)
    return *this;

  return operator>>=((unsigned long) v);

}


CLASS_TYPE&
CLASS_TYPE::operator>>=(unsigned long v)
{

  if (v == 0)
    return *this;

  if (sgn == SC_ZERO)
    return *this;

  convert_SM_to_2C();

  if (sgn == SC_NEG)
    vec_shift_right(ndigits, digit, v, DIGIT_MASK);
  else 
    vec_shift_right(ndigits, digit, v, 0);

  convert_2C_to_SM();

  return *this;

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: EQUAL TO operator: ==
/////////////////////////////////////////////////////////////////////////////

// Defined in the sc_signed.cpp and sc_unsigned.cpp.

/////////////////////////////////////////////////////////////////////////////
// SECTION: NOT_EQUAL operator: !=
/////////////////////////////////////////////////////////////////////////////

bool
operator!=(const CLASS_TYPE& u, const CLASS_TYPE& v)
{
  return (! operator==(u, v));
}


bool
operator!=(const CLASS_TYPE& u, int64 v)
{
  return (! operator==(u, v));
}


bool
operator!=(int64 u, const CLASS_TYPE& v)
{
  return (! operator==(u, v));  
}


bool
operator!=(const CLASS_TYPE& u, uint64 v)
{
  return (! operator==(u, v));  
}


bool
operator!=(uint64 u, const CLASS_TYPE& v)
{
  return (! operator==(u, v));  
}


bool
operator!=(const CLASS_TYPE& u, long v)
{
  return (! operator==(u, v));
}


bool
operator!=(long u, const CLASS_TYPE& v)
{
  return (! operator==(u, v));  
}


bool
operator!=(const CLASS_TYPE& u, unsigned long v)
{
  return (! operator==(u, v));  
}


bool
operator!=(unsigned long u, const CLASS_TYPE& v)
{
  return (! operator==(u, v));  
}

/////////////////////////////////////////////////////////////////////////////
// SECTION: LESS THAN operator: <
/////////////////////////////////////////////////////////////////////////////

// Defined in the sc_signed.cpp and sc_unsigned.cpp.

/////////////////////////////////////////////////////////////////////////////
// SECTION: LESS THAN or EQUAL operator: <=
/////////////////////////////////////////////////////////////////////////////

bool
operator<=(const CLASS_TYPE& u, const CLASS_TYPE& v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(const CLASS_TYPE& u, int64 v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(int64 u, const CLASS_TYPE& v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(const CLASS_TYPE& u, uint64 v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(uint64 u, const CLASS_TYPE& v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(const CLASS_TYPE& u, long v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(long u, const CLASS_TYPE& v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(const CLASS_TYPE& u, unsigned long v)
{
  return (operator<(u, v) || operator==(u, v));
}


bool
operator<=(unsigned long u, const CLASS_TYPE& v)
{
  return (operator<(u, v) || operator==(u, v));
}

/////////////////////////////////////////////////////////////////////////////
// SECTION: GREATER THAN operator: >
/////////////////////////////////////////////////////////////////////////////

bool
operator>(const CLASS_TYPE& u, const CLASS_TYPE& v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(const CLASS_TYPE& u, int64 v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(int64 u, const CLASS_TYPE& v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(const CLASS_TYPE& u, uint64 v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(uint64 u, const CLASS_TYPE& v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(const CLASS_TYPE& u, long v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(long u, const CLASS_TYPE& v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(const CLASS_TYPE& u, unsigned long v)
{
  return (! (operator<=(u, v)));
}


bool
operator>(unsigned long u, const CLASS_TYPE& v)
{
  return (! (operator<=(u, v)));
}

/////////////////////////////////////////////////////////////////////////////
// SECTION: GREATER THAN or EQUAL operator: >=
/////////////////////////////////////////////////////////////////////////////

bool
operator>=(const CLASS_TYPE& u, const CLASS_TYPE& v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(const CLASS_TYPE& u, int64 v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(int64 u, const CLASS_TYPE& v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(const CLASS_TYPE& u, uint64 v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(uint64 u, const CLASS_TYPE& v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(const CLASS_TYPE& u, long v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(long u, const CLASS_TYPE& v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(const CLASS_TYPE& u, unsigned long v)
{
  return (! (operator<(u, v)));
}


bool
operator>=(unsigned long u, const CLASS_TYPE& v)
{
  return (! (operator<(u, v)));
}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Public members - Other utils.
/////////////////////////////////////////////////////////////////////////////

void 
CLASS_TYPE::print(ostream& os) const
{
  os << *this;
}


// Convert the number to an sc_string in base b. Note that the bit
// string is actually the mirror image of the number's bits, and that
// the output is binary.
sc_string
CLASS_TYPE::to_string(sc_numrep base, bool formatted) const
{

  is_valid_base(base);

  small_type s = sgn;
  // num_str will hold the string representation of the number until
  // the result is copied into an sc_string object. Since the number
  // can be requested in binary, we have to allocate at least nbits
  // bits in num_str to hold the string representation. 
#ifdef SC_MAX_NBITS
  char num_str[SC_MAX_NBITS];
#else
  char *num_str = new char[ndigits * BITS_PER_DIGIT];
#endif

  char base_ch;
  int out_nd;
  int out_nb;
  // out_nd is the number of digits needed to print the number in the
  // required base. It is equal to ceil(nbits / lg(base)) or
  // DIV_CEIL2(nbits, lg(base)).

  // out_nb is the number of bits needed to print the number in the
  // required base. For example, if this number is equal to '11001',
  // then out_nb must be set to 8 since in hex this number must look
  // like '11111001' or 'F9' rather than '19'. This will ensure that
  // the user will know whether or not the number was negative or
  // positive just by looking at the number. But the user should also
  // know whether s/he is printing a signed or an unsigned number
  // because an unsigned number can look like a negative number in 2's
  // complement. out_nb is equal to out_nd * lg(base).

#if (IF_SC_SIGNED == 1)
  out_nd = nbits;
#else
  out_nd = nbits - 1;
#endif

  switch(base) {
  case SC_BIN: 
    base_ch = 'b'; 
    out_nb = out_nd; 
    break;
  case SC_OCT: 
    base_ch = 'o'; 
    out_nd = DIV_CEIL2(out_nd, 3); 
    out_nb = 3 * out_nd;
    break;
  case SC_DEC: 
    base_ch = 'd'; 
    out_nd = DIV_CEIL2(out_nd, 3); 
    out_nb = 3 * out_nd;
    break;
  case SC_HEX: 
    base_ch = 'x'; 
    out_nd = DIV_CEIL2(out_nd, 4); 
    out_nb = 4 * out_nd;
    break;
  default:
    printf("SystemC error: The base arg is invalid.\n");
    abort();
  }

  register int len = 0;

  if (s != SC_ZERO) {

    register int nd = DIV_CEIL(out_nb);

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[nd];
#endif

    vec_copy_and_zero(nd, d, ndigits, digit);

    // Below the first call is needed to determine the actual sign of
    // the number. After that call, we get the number in
    // sign-magnitude. We then convert it to 2's complement, which
    // won't affect non-negative numbers. Note the use of nbits and
    // ndigits in the first call and out_nb and nd in the second call
    // for d's length. The reason is that the first call refers to the
    // digit as stored in d and the second call refers to d itself.
#if (IF_SC_SIGNED == 1)
    s = convert_signed_SM_to_2C_to_SM(s, nbits, ndigits, d);
    convert_signed_SM_to_2C_trimmed(s, out_nb, nd, d);
#else
    s = convert_unsigned_SM_to_2C_to_SM(s, nbits, ndigits, d);
    convert_unsigned_SM_to_2C_trimmed(s, out_nb, nd, d);
#endif

    nd = vec_skip_leading_zeros(nd, d);
  
    if (nd == 0)
      s = SC_ZERO;
  
    // Get the number in base b from d into num_str.
    while (nd) {
      unsigned long r = vec_rem_on_small(nd, d, base);
      num_str[len++] = "0123456789ABCDEF"[r];
      nd = vec_skip_leading_zeros(nd, d);
    }

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

  }

  // This 'if' is not 'else' of the above if block. Since s can be set
  // to zero inside the above if block, this 'if' has to be separate.
  if (s == SC_ZERO)
    num_str[len++] = '0';

#ifdef DEBUG_SYSTEMC
  assert(len <= out_nd);
#endif

  int ulen = (formatted ? out_nd + 2 : out_nd );
  sc_string u(ulen + 1);

  register int i = 0;

  if (formatted) {
    u.set(0, '0');
    u.set(1, base_ch);
    i = 2;
  }

  register int j;

  for (j = out_nd - 1; j >= len; ++i, --j)
    u.set(i, '0');

  for (j = len - 1; j >= 0; ++i, --j)
    u.set(i, num_str[j]);

  u.set(ulen, '\0');

#ifndef SC_MAX_NBITS
  delete [] num_str;
#endif

  return u;

}


// Convert to int64, long, or int.
#define TO_INTX(RET_TYPE, UP_RET_TYPE)   \
                                                             \
if (sgn == SC_ZERO)                                          \
return 0;                                                    \
                                                             \
int vnd = sc_min(DIGITS_PER_ ## UP_RET_TYPE, ndigits); \
                                                             \
RET_TYPE v = 0;                                              \
while (--vnd >= 0)                                           \
v = (v << BITS_PER_DIGIT) + digit[vnd];                      \
                                                             \
if (sgn == SC_NEG)                                           \
return -v;                                                   \
else                                                         \
return v;                                                    


int64
CLASS_TYPE::to_int64() const
{
  TO_INTX(int64, INT64);
}


long
CLASS_TYPE::to_long() const
{
  TO_INTX(long, LONG);
}


int
CLASS_TYPE::to_int() const
{
  TO_INTX(int, INT);
}


// Convert to unsigned int64, unsigned long or unsigned
// int. to_uint64, to_ulong, and to_uint have the same body except for
// the type of v defined inside.
uint64
CLASS_TYPE::to_uint64() const
{

  if (sgn == SC_ZERO)
    return 0;

  int vnd = sc_min(DIGITS_PER_INT64, ndigits);

  uint64 v = 0;

  if (sgn == SC_NEG) {

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[ndigits];
#endif

    vec_copy(ndigits, d, digit);

    convert_SM_to_2C_trimmed(IF_SC_SIGNED, sgn, nbits, ndigits, d);

    while (--vnd >= 0)
      v = (v << BITS_PER_DIGIT) + d[vnd];

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

  }
  else {

    while (--vnd >= 0)
      v = (v << BITS_PER_DIGIT) + digit[vnd];

  }

  return v;

}


unsigned long
CLASS_TYPE::to_ulong() const
{

  if (sgn == SC_ZERO)
    return 0;

  int vnd = sc_min(DIGITS_PER_LONG, ndigits);

  unsigned long v = 0;

  if (sgn == SC_NEG) {

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[ndigits];
#endif

    vec_copy(ndigits, d, digit);

    convert_SM_to_2C_trimmed(IF_SC_SIGNED, sgn, nbits, ndigits, d);

    while (--vnd >= 0)
      v = (v << BITS_PER_DIGIT) + d[vnd];

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

  }
  else {

    while (--vnd >= 0)
      v = (v << BITS_PER_DIGIT) + digit[vnd];

  }

  return v;

}


unsigned int
CLASS_TYPE::to_uint() const
{

  if (sgn == SC_ZERO)
    return 0;

  int vnd = sc_min(DIGITS_PER_INT, ndigits);

  unsigned int v = 0;

  if (sgn == SC_NEG) {

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[ndigits];
#endif

    vec_copy(ndigits, d, digit);

    convert_SM_to_2C_trimmed(IF_SC_SIGNED, sgn, nbits, ndigits, d);

    while (--vnd >= 0)
      v = (v << BITS_PER_DIGIT) + d[vnd];

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

  }
  else {

    while (--vnd >= 0)
      v = (v << BITS_PER_DIGIT) + digit[vnd];

  }

  return v;

}


// Convert to double.
double
CLASS_TYPE::to_double() const
{

  if (sgn == SC_ZERO)
    return (double) 0.0;

  int vnd = ndigits;

  double v = 0.0;
  while (--vnd >= 0)
    v = v * DIGIT_RADIX + digit[vnd];

  if (sgn == SC_NEG)
    return -v;
  else
    return v;

}


// Return true if the bit i is 1, false otherwise. If i is outside the
// bounds, return 1/0 according to the sign of the number by assuming
// that the number has infinite length.
bit 
CLASS_TYPE::test(int i) const
{

#ifdef SC_SIGNED
  if (check_if_outside(i)) {
    if (sgn == SC_NEG)
      return 1;
    else
      return 0;
  }
#else
  if (check_if_outside(i))
    return 0;
#endif  

  int bit_num = bit_ord(i);
  int digit_num = digit_ord(i);

  if (sgn == SC_NEG) {

#ifdef SC_MAX_NBITS
    unsigned long d[MAX_NDIGITS];
#else
    unsigned long *d = new unsigned long[ndigits];
#endif

    vec_copy(ndigits, d, digit);
    vec_complement(ndigits, d);
    bit val = ((d[digit_num] & one_and_zeros(bit_num)) != 0);

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

    return val;

  }
  else 
    return ((digit[digit_num] & one_and_zeros(bit_num)) != 0);

}


// Set the ith bit with 1.
void 
CLASS_TYPE::set(int i) 
{

  if (check_if_outside(i))
    return;

  int bit_num = bit_ord(i);
  int digit_num = digit_ord(i);

  convert_SM_to_2C();
  digit[digit_num] |= one_and_zeros(bit_num);    
  digit[digit_num] &= DIGIT_MASK; // Needed to zero the overflow bits.
  convert_2C_to_SM();

}


// Set the ith bit with 0, i.e., clear the ith bit.
void 
CLASS_TYPE::clear(int i)
{

  if (check_if_outside(i))
    return;

  int bit_num = bit_ord(i);
  int digit_num = digit_ord(i);

  convert_SM_to_2C();
  digit[digit_num] &= ~(one_and_zeros(bit_num));
  digit[digit_num] &= DIGIT_MASK; // Needed to zero the overflow bits.
  convert_2C_to_SM();

}


// Create a mirror image of the number.
void
CLASS_TYPE::reverse()
{

  convert_SM_to_2C();
  vec_reverse(length(), ndigits, digit, length() - 1);
  convert_2C_to_SM();

}


// Get a packed bit representation of the number.
void 
CLASS_TYPE::get_packed_rep(unsigned long *buf) const
{

  int buf_ndigits = (length() - 1) / BITS_PER_DIGIT_TYPE + 1;

  // Initialize buf to zero.
  vec_zero(buf_ndigits, buf);

  if (sgn == SC_ZERO)
    return;

  const unsigned long *digit_or_d;
#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  unsigned long *d = new unsigned long[ndigits];
#endif

  if (sgn == SC_POS)
    digit_or_d = digit;

  else 
  {
    // If sgn is negative, we have to convert digit to its 2's
    // complement. Since this function is const, we can not do it on
    // digit. Since buf doesn't have overflow bits, we cannot also do
    // it on buf. Thus, we have to do the complementation on a copy of
    // digit, i.e., on d.

    vec_copy(ndigits, d, digit);
    vec_complement(ndigits, d);

    buf[buf_ndigits - 1] = ~((unsigned long) 0);

    digit_or_d = d;

  }

  // Copy the bits from digit to buf. The division and mod operations
  // below can be converted to addition/subtraction and comparison
  // operations at the expense of complicating the code. We can do it
  // if we see any performance problems.

  for (register int i = length() - 1; i >= 0; --i) {

    if ((digit_or_d[digit_ord(i)] & one_and_zeros(bit_ord(i))) != 0) // Test.

      buf[i / BITS_PER_DIGIT_TYPE] |= 
        one_and_zeros(i % BITS_PER_DIGIT_TYPE); // Set.

    else  

      buf[i / BITS_PER_DIGIT_TYPE] &= 
        ~(one_and_zeros(i % BITS_PER_DIGIT_TYPE));  // Clear.

  }

#ifndef SC_MAX_NBITS
    delete[] d;
#endif
}


// Set a packed bit representation of the number.
void 
CLASS_TYPE::set_packed_rep(unsigned long *buf)
{

  // Initialize digit to zero.
  vec_zero(ndigits, digit);

  // Copy the bits from buf to digit.
  for (register int i = length() - 1; i >= 0; --i) {

    if ((buf[i / BITS_PER_DIGIT_TYPE] & 
         one_and_zeros(i % BITS_PER_DIGIT_TYPE)) != 0) // Test.

      digit[digit_ord(i)] |= one_and_zeros(bit_ord(i));     // Set.

    else  

      digit[digit_ord(i)] &= ~(one_and_zeros(bit_ord(i)));  // Clear

  }

  convert_2C_to_SM();

}

/////////////////////////////////////////////////////////////////////////////
// SECTION: Private members.
/////////////////////////////////////////////////////////////////////////////


// Create zero.
CLASS_TYPE::CLASS_TYPE()
{

  sgn = SC_ZERO;
  nbits = num_bits(1);
  ndigits = 1;

#ifndef SC_MAX_NBITS
  digit = new unsigned long[1];
#endif

  digit[0] = 0;

}

// Create a copy of v with sgn s.
CLASS_TYPE::CLASS_TYPE(const CLASS_TYPE& v, small_type s)
{

  sgn = s;
  nbits = v.nbits;
  ndigits = v.ndigits;

#ifndef SC_MAX_NBITS
  digit = new unsigned long[ndigits];
#endif

  vec_copy(ndigits, digit, v.digit);

}


// Create a copy of v where v is of the different type.
CLASS_TYPE::CLASS_TYPE(const OTHER_CLASS_TYPE& v, small_type s)
{

  sgn = s;
  nbits = num_bits(v.nbits);

#if (IF_SC_SIGNED == 1)
  ndigits = v.ndigits;
#else
  ndigits = DIV_CEIL(nbits);
#endif

#ifndef SC_MAX_NBITS
  digit = new unsigned long[ndigits];
#endif

  copy_digits(v.nbits, v.ndigits, v.digit);

}


// Create a signed number with (s, nb, nd, d) as its attributes (as
// defined in class CLASS_TYPE). If alloc is set, delete d.
CLASS_TYPE::CLASS_TYPE(small_type s, int nb, 
                       int nd, unsigned long *d, 
                       bool alloc)
{

  sgn = s;
  nbits = nb;
  ndigits = DIV_CEIL(nbits);

#ifndef SC_MAX_NBITS
  digit = new unsigned long[ndigits];
#endif

  if (ndigits <= nd)
    vec_copy(ndigits, digit, d);
  else
    vec_copy_and_zero(ndigits, digit, nd, d);

#ifndef SC_MAX_NBITS
  if (alloc)
    delete [] d;
#endif

}


// This constructor is mainly used in finding a "range" of bits from a
// number of type CLASS_TYPE. The function range(l, r) can have
// arbitrary precedence between l and r. If l is smaller than r, then
// the output is the reverse of range(r, l). 
CLASS_TYPE::CLASS_TYPE(const CLASS_TYPE* u, int l, int r)
{

  bool reversed = false;

  // This test is for efficiency only.
  if (u->sgn != SC_ZERO) {

    if (l < r) {
      reversed = true;
      int tmp = l;
      l = r;
      r = tmp;

    }

    // At this point, l >= r.

    // Make sure that l and r point to the bits of u.
    r = sc_max(r, 0);
    l = sc_min(l, u->nbits - 1);
    
    nbits = num_bits(l - r + 1);

    // nbits can still be <= 0 because l and r have just been updated
    // with the bounds of u.

  }

  // If u == 0 or the range is out of bounds, return 0.
  if ((u->sgn == SC_ZERO) || (nbits <= num_bits(0))) {

    sgn = SC_ZERO;
    nbits = 1;
    ndigits = 1;

#ifndef SC_MAX_NBITS
    digit = new unsigned long[1];
#endif

    digit[0] = 0;

    return;

  }

  // The rest will be executed if u is not zero.

  ndigits = DIV_CEIL(nbits);
  
  // The number of bits up to and including l and r, respectively.
  int nl = l + 1; 
  int nr = r + 1; 
  
  // The indices of the digits that have lth and rth bits, respectively.
  int left_digit = DIV_CEIL(nl) - 1;
  int right_digit = DIV_CEIL(nr) - 1;
  
  int nd;

  // The range is performed on the 2's complement representation, so
  // first get the indices for that.
  if (u->sgn == SC_NEG)
    nd = left_digit + 1;
  else
    nd = left_digit - right_digit + 1;

  // Allocate memory for the range.
#ifdef SC_MAX_NBITS
  unsigned long d[MAX_NDIGITS];
#else
  digit = new unsigned long[ndigits];
  unsigned long *d = new unsigned long[nd];
#endif
  
  // Getting the range on the 2's complement representation.
  if (u->sgn == SC_NEG) {
    
    vec_copy(nd, d, u->digit);
    vec_complement(nd, d);
    vec_shift_right(nd, d, r - right_digit * BITS_PER_DIGIT, DIGIT_MASK);
    
  }
  else {
    
    for (register int i = right_digit; i <= left_digit; ++i)
      d[i - right_digit] = u->digit[i];
    
    vec_shift_right(nd, d, r - right_digit * BITS_PER_DIGIT, 0);
    
  }
  
  vec_zero(ndigits, digit);

  if (! reversed)
    vec_copy(sc_min(nd, ndigits), digit, d);
  
  else {

    // If l < r, i.e., reversed is set, reverse the bits of digit.  d
    // will be used as a temporary store. The following code tries to
    // minimize the use of bit_ord and digit_ord, which use mod and
    // div operators. Since these operators are function calls to
    // standard library routines, they are slow. The main idea in
    // reversing is "read bits out of d from left to right and push
    // them into digit using right shifting."

    // Take care of the last digit.
    int nd_less_1 = nd - 1;

    // Deletions will start from the left end and move one position
    // after each deletion.
    register unsigned long del_mask = one_and_zeros(bit_ord(l - r));
      
    while (del_mask) {
      vec_shift_right(ndigits, digit, 1, ((d[nd_less_1] & del_mask) != 0));
      del_mask >>= 1;
    }

    // Take care of the other digits if any.

    // Insertion to digit will always occur at the left end.
    unsigned long ins_mask = one_and_zeros(BITS_PER_DIGIT - 1);

    for (register int j = nd - 2; j >= 0; --j) { // j = nd - 2

      // Deletions will start from the left end and move one position
      // after each deletion.
      del_mask = ins_mask;

      while (del_mask) {
        vec_shift_right(ndigits, digit, 1, ((d[j] & del_mask) != 0));
        del_mask >>= 1;
      }
    }

    if (u->sgn == SC_NEG)
      vec_shift_right(ndigits, digit, 
                      ndigits * BITS_PER_DIGIT - length(), DIGIT_MASK);
    else
      vec_shift_right(ndigits, digit, 
                      ndigits * BITS_PER_DIGIT - length(), 0);


  }  // if reversed.

  convert_2C_to_SM();
  
#ifndef SC_MAX_NBITS
  delete [] d;
#endif
  
}


// Print out all the physical attributes.
void
CLASS_TYPE::dump(ostream& os) const
{

  // Save the current setting, and set the base to decimal.
  fmtflags old_flags = os.setf(ios::dec, ios::basefield);

  os << "width = " << length() << endl;
  os << "value = " << *this << endl;
  os << "bits  = ";

  int len = length();

  for (int i = len - 1; i >= 0; --i) {

    os << "01"[test(i)];
    if (--len % 4 == 0)
      os << " ";

  }

  os << endl;

  // Restore old_flags.
  os.setf(old_flags, ios::basefield);

}


// Checks to see if bit_num is out of bounds.
bool
CLASS_TYPE::check_if_outside(int bit_num) const
{
  if ((bit_num < 0) || (num_bits(bit_num) >= nbits)) {

#ifdef DEBUG_SYSTEMC
    warn((bit_num >= 0) && (bit_num < nbits), "Bit index is out of bounds.\n");
#endif

    return true;
  }

  return false;
}

// End of file.

