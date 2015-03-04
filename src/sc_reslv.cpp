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

    sc_reslv.cpp -- C++ implementation of resolution function class.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "sc_iostream.h"
#include "sc_reslv.h"
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif

// Note that we assume that two drivers driving the resolved signal to a 1 or 0
// is O.K. This might not be true for all technologies, but is certainly true for
// CMOS, the predominant technology in use today.

const sc_logic::Log_enum resolution_table[4][4] =
{
  //  O  l  Z  X
  //------------
    {  sc_logic::Log_0, sc_logic::Log_X, sc_logic::Log_0, sc_logic::Log_X }, // O
    {  sc_logic::Log_X, sc_logic::Log_1, sc_logic::Log_1, sc_logic::Log_X }, // l
    {  sc_logic::Log_0, sc_logic::Log_1, sc_logic::Log_Z, sc_logic::Log_X }, // Z
    {  sc_logic::Log_X, sc_logic::Log_X, sc_logic::Log_X, sc_logic::Log_X }  // X
};

sc_logic
sc_logic_resolve::resolve( const sc_pvector<sc_logic*>& values )
{
    sc_logic result('Z');
    if (values.size() == 1) {
        return *(values[0]);
    }
    else {
        sc_pvector<sc_logic*>::const_iterator it = values.begin();
        sc_pvector<sc_logic*>::const_iterator end = values.end();
        for( ; it != end; it++ ) {
            result = resolution_table[result.val][(*it)->val];
        }
    }
    return result;
}
/*
const long lv_res_table[4][4] =
{
  //  O  l  Z  X
  //------------
    { 0, 3, 0, 3 }, // 0
    { 3, 1, 1, 3 }, // 1
    { 0, 1, 2, 3 }, // Z
    { 3, 3, 3, 3 } // X
};

*/
