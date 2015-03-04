/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_bit_ids.h -- Report ids for the datatypes/bit code.

  Original Author: Martin Janssen, Synopsys, Inc., 2002-01-17

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_BIT_IDS_H
#define SC_BIT_IDS_H


#include "systemc/utils/sc_report.h"


// ----------------------------------------------------------------------------
//  Report ids (datatypes/bit)
//
//  Report ids in the range of 200-299.
// ----------------------------------------------------------------------------

extern const int SC_ID_LENGTH_MISMATCH_;
extern const int SC_ID_INCOMPATIBLE_TYPES_;
extern const int SC_ID_CANNOT_CONVERT_;
extern const int SC_ID_INCOMPATIBLE_VECTORS_;
extern const int SC_ID_VALUE_NOT_VALID_;
extern const int SC_ID_ZERO_LENGTH_;
extern const int SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_;
extern const int SC_ID_SC_BV_CANNOT_CONTAIN_X_AND_Z_;
extern const int SC_ID_VECTOR_TOO_LONG_;
extern const int SC_ID_VECTOR_TOO_SHORT_;
extern const int SC_ID_WRONG_VALUE_;
extern const int SC_ID_LOGIC_Z_TO_BOOL_;
extern const int SC_ID_LOGIC_X_TO_BOOL_;


#endif

// Taf!
