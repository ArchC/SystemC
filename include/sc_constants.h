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

    sc_constants.h -- default constants whose values may need to be
    changed depending on the application.

    Original Author: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_CONSTANTS_H
#define SC_CONSTANTS_H

// Maximum number of bits for arbitrary precision arithmetic. If
// defined, the arithmetic becomes faster. If not defined, the
// arithmetic becomes slower and the precision becomes infinite.  It
// is a good idea to define this constant as a multiple of
// BITS_PER_DIGIT, which is defined in numeric_bit/sc_nbdefs.h.
//#define SC_MAX_NBITS    510    // 17 * BITS_PER_DIGIT

const int SC_DEFAULT_STACK_SIZE   = 0x10000;
const int SC_MAX_WATCH_LEVEL      = 16;

#ifdef DEBUG_SYSTEMC
const int SC_MAX_NUM_DELTA_CYCLES = 10000;
#endif

#endif
