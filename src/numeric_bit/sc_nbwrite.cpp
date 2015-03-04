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

    sc_nbwrite.cpp - Definition of the interface between
    sc_signed/sc_unsigned and sc_logic_vector/sc_bool_vector.

    Original Authors: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)
                      Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)

*****************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#define SC_NUMERIC_BIT_IMPLEMENTATION
#include <assert.h>

#include "../sc_iostream.h"
#include "../sc_bool_vector.h"
#include "../sc_logic_vector.h"
#include "sc_signed.h"
#include "sc_unsigned.h"


sc_signal_bool_vector&
sc_signal_bool_vector::write(const sc_unsigned& nv)
{

    int len = length();
    int minlen = min(len, nv.length());

    int i = 0;

    for ( ; i < minlen; ++i)
        write_help(i, nv.test(i));

    for ( ; i < len; ++i)
        write_help(i, 0); /* zero-extend */

    return *this;

}


sc_signal_bool_vector&
sc_signal_bool_vector::write(const sc_signed& nv)
{

    int len = length();
    int minlen = min(len, nv.length());

    int i = 0;
    for ( ; i < minlen; ++i)
        write_help(i, nv.test(i));

    bit sgn = nv.sign();

    for ( ; i < len; ++i)
        write_help(i, sgn); /* sign-extend */

    return *this;

}


sc_signal_logic_vector&
sc_signal_logic_vector::write(const sc_unsigned& nv)
{

    int len = length();
    int minlen = min(len, nv.length());

    int i = 0;
    for ( ; i < minlen; ++i)
        write_help(i, sc_logic(nv.test(i)));

    for ( ; i < len; ++i)
        write_help(i, sc_logic_0); /* zero-extend */

    return *this;

}


sc_signal_logic_vector&
sc_signal_logic_vector::write(const sc_signed& nv)
{

    int len = length();
    int minlen = min(len, nv.length());

    int i = 0;
    for ( ; i < minlen; ++i)
        write_help(i, sc_logic(nv.test(i)));

    bit sgn = nv.sign();

    for ( ; i < len; ++i)
        write_help(i, sc_logic(sgn)); /* sign-extend */

    return *this;

}

// End of file
