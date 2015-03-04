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

  sc_nbdefs.h -- Top level header file for arbitrary precision signed/unsigned
                 arithmetic. This file defines all the constants needed.

  Original Author: Ali Dasdan, Synopsys, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/datatypes/int/sc_nbdefs.h"

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

const small_type NB_DEFAULT_BASE = SC_DEC;

#ifndef _32BIT_
const uint64 const_one_uint = const_one_ull;
const uint64 const_zero_uint = const_zero_ull;
#else
const unsigned int const_one_uint = 1U;
const unsigned int const_zero_uint = 0U;
#endif

