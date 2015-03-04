/******************************************************************************
    Copyright (c) 1996-2001 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Community License Software Download and
  Use License Version 1.2 (the "License"); you may not use this file excep
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    sc_iostream.h - Portable iostream header file wrapper.

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_IOSTREAM_H
#define SC_IOSTREAM_H

#if !defined(_MSC_VER) && !defined(__HP_aCC)

#include <iostream>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;

#include <cstddef>
using std::size_t;

#else

#if defined(SC_VC6_SCL)
// VC6 with standard library
#include <iostream>
#include <cstddef>
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;
#else
// VC6 with old library and HP10.20 aCC
#include <iostream.h>
#endif
// VC6 and HPUX 10.20 aCC scope bug
#define for if(false);else for

#endif

#endif
