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

    sc_nbdefs.h -- Top level header file for arbitrary precision
    signed/unsigned arithmetic. This file defines all the constants
    needed.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
  
******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_nbdefs.h"
// Attributes of a byte.
//const uchar      BITS_PER_BYTE = CHAR_BIT;
//const unsigned long BYTE_RADIX    = ((unsigned long) 1) << BITS_PER_BYTE;
//const unsigned long BYTE_MASK     = BYTE_RADIX - 1;

// LOG2_BITS_PER_BYTE = log2(BITS_PER_BYTE), assuming that
// BITS_PER_BYTE is a power of 2.
//const uchar      LOG2_BITS_PER_BYTE = 3;

// Attributes of the unsigned long. These definitions are used mainly in
// the functions that are aware of the internal representation of
// digits, e.g., get/set_packed_rep().
//const unsigned long BYTES_PER_DIGIT_TYPE = sizeof(unsigned long);
//const unsigned long BITS_PER_DIGIT_TYPE  = BYTES_PER_DIGIT_TYPE * BITS_PER_BYTE;

// Attributes of a digit, i.e., unsigned long less the overflow bits.
//const unsigned long BYTES_PER_DIGIT = BYTES_PER_DIGIT_TYPE;
//const unsigned long BITS_PER_DIGIT  = BITS_PER_DIGIT_TYPE - 2;
//const unsigned long DIGIT_RADIX     = ((unsigned long) 1) << BITS_PER_DIGIT;
//const unsigned long DIGIT_MASK      = DIGIT_RADIX - 1;
// Make sure that BYTES_PER_DIGIT = ceil(BITS_PER_DIGIT / BITS_PER_BYTE).

// Similar attributes for the half of a digit. Note that
// HALF_DIGIT_RADIX is equal to the square root of DIGIT_RADIX. These
// definitions are used mainly in the multiplication routines.
//const unsigned long BITS_PER_HALF_DIGIT = BITS_PER_DIGIT / 2;
//const unsigned long HALF_DIGIT_RADIX    = ((unsigned long) 1) << BITS_PER_HALF_DIGIT;
//const unsigned long HALF_DIGIT_MASK     = HALF_DIGIT_RADIX - 1;


#ifdef SC_MAX_NBITS
const int MAX_NDIGITS      = DIV_CEIL(SC_MAX_NBITS) + 2;
// Consider a number with x bits another with y bits. The maximum
// number of bits happens when we multiply them. The result will have
// (x + y) bits. Assume that x + y <= SC_MAX_NBITS. Then, DIV_CEIL(x) +
// DIV_CEIL(y) <= DIV_CEIL(SC_MAX_NBITS) + 2. This is the reason for +2
// above. With this change, MAX_NDIGITS must be enough to hold the
// result of any operation.
#endif

// Support for the long long type. This type is not in the standard
// but is usually supported by compilers.
#ifndef WIN32
const uint64 const_zero_ull = 0ULL;
const uint64 const_one_ull = 1ULL;
const uint64 const_32ones_ull = 0x00000000ffffffffULL;
#else
const uint64 const_zero_ull = 0i64;
const uint64 const_one_ull = 1i64;
const uint64 const_32ones_ull = 0x00000000ffffffffi64;
#endif

// Bits per ...
/*
const int BITS_PER_CHAR   = BITS_PER_BYTE;
const int BITS_PER_INT    = BITS_PER_BYTE * sizeof(int);
const int BITS_PER_LONG   = BITS_PER_BYTE * sizeof(long);
const int BITS_PER_INT64  = BITS_PER_BYTE * sizeof(int64);
const int BITS_PER_UINT   = BITS_PER_INT + 1;
const int BITS_PER_ULONG  = BITS_PER_LONG + 1;
const int BITS_PER_UINT64 = BITS_PER_INT64 + 1;
*/
// Digits per ...
//const int DIGITS_PER_CHAR   = DIV_CEIL(BITS_PER_CHAR);
//const int DIGITS_PER_INT    = DIV_CEIL(BITS_PER_INT);
//const int DIGITS_PER_LONG   = DIV_CEIL(BITS_PER_LONG);
//const int DIGITS_PER_INT64  = DIV_CEIL(BITS_PER_INT64);
//const int DIGITS_PER_UINT   = DIV_CEIL(BITS_PER_UINT);
//const int DIGITS_PER_ULONG  = DIV_CEIL(BITS_PER_ULONG);
//const int DIGITS_PER_UINT64 = DIV_CEIL(BITS_PER_UINT64);

// Above, BITS_PER_X is mainly used for sc_signed, and BITS_PER_UX is
// mainly used for sc_unsigned.

//const char       NEW_LINE = '\n';
const small_type NB_DEFAULT_BASE = SC_DEC;

#ifndef _32BIT_
//const int SC_INTWIDTH = BITS_PER_INT64;
const uint64 const_one_uint = const_one_ull;
const uint64 const_zero_uint = const_zero_ull;
#else
//const int SC_INTWIDTH = BITS_PER_INT;
const unsigned int const_one_uint = 1U;
const unsigned int const_zero_uint = 0U;
#endif

