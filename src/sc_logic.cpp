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

    sc_logic.cpp -- C++ implementation of logic type.  Behaves
                     pretty much the same way as HDLs logic type.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "sc_iostream.h"
#include "sc_logic.h"
#include "sc_exception.h"

void
sc_logic::check_01() const
{
    if (! is_01())
        REPORT_WARNING(9005,sc_string::to_string(
			"sc_logic value `%c' cannot be converted to bool", to_char()).c_str());
}

// when unknown symbol is converted to sc_logic it becomes Log_X
const sc_logic::Log_enum sc_logic::char_to_logic[128]=
  {
    Log_0, Log_1, Log_Z, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_0, Log_1,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_Z, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_X, Log_X, Log_X,
    Log_X, Log_X, Log_Z, Log_X, Log_X,
    Log_X, Log_X, Log_X
  };

const char sc_logic::logic_to_char[4]={'0','1','Z','X'};
const sc_logic::Log_enum sc_logic::and_table[4][4] =
{
   {Log_0, Log_0, Log_0, Log_0},
   {Log_0, Log_1, Log_X, Log_X},
   {Log_0, Log_X, Log_X, Log_X},
   {Log_0, Log_X, Log_X, Log_X}
};
const sc_logic::Log_enum sc_logic::or_table[4][4] =
{
   {Log_0, Log_1, Log_X, Log_X},
   {Log_1, Log_1, Log_1, Log_1},
   {Log_X, Log_1, Log_X, Log_X},
   {Log_X, Log_1, Log_X, Log_X}
};
const sc_logic::Log_enum sc_logic::xor_table[4][4] =
{
   {Log_0, Log_1, Log_X, Log_X},
   {Log_1, Log_0, Log_X, Log_X},
   {Log_X, Log_X, Log_X, Log_X},
   {Log_X, Log_X, Log_X, Log_X}
};
const sc_logic::Log_enum sc_logic::not_table[4] =
{   Log_1, Log_0, Log_X, Log_X  };

const sc_logic sc_logic_X(sc_logic::Log_X);
const sc_logic sc_logic_0(sc_logic::Log_0);
const sc_logic sc_logic_1(sc_logic::Log_1);
const sc_logic sc_logic_Z(sc_logic::Log_Z);

