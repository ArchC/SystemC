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

    sc_main.cpp - wrapper around user's toplevel routine `sc_main'

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#if defined _MSC_VER
   #include "sc_iostream.h"
   #define _WIN32_WINNT 0x0400
#endif

#ifdef WIN32
#include <Windows.h>
 #if defined(__BCPLUSPLUS__)
   #if !defined(__CONSOLE__)
   #include <vcl>
   #else
   #include <iostream>
    using std::ostream;
    using std::istream;
    using std::cout;
    using std::cin;
    using std::cerr;
    using std::endl;
   #endif
 #pragma hdrstop
 #pragma -warn 8004
 #endif
#include "sc_cmnhdr.h"
#else
#include "sc_iostream.h"
#endif
#include "sc_externs.h"
#include "sc_exception.h"

#if defined(__BCPLUSPLUS__) && !defined(__CONSOLE__)
  #define MAIN_FUNCTION_NAME sc_win_main
inline void Message_Function(const char* s) {ShowMessage(s);}
#else
  #define MAIN_FUNCTION_NAME main
inline void Message_Function(const char* s) {cout<<"\n"<<s<<"\n";}
#endif

bool sc_in_action = false;
extern void pln();

int MAIN_FUNCTION_NAME(int argc, char* argv[])
#undef  MAIN_FUNCTION_NAME
{
    int status=0;
    try
    {
      pln();
      /* Perform initialization here */
      sc_in_action = true;

#ifdef WIN32
      PVOID mainfiber = ConvertThreadToFiber(NULL);
#endif
      status = sc_main(argc, argv);

      /* Perform cleanup here */
      sc_in_action = false;
    }
    catch(const sc_exception& x)
    {
      Message_Function(x.str());
    }
#if defined(__BCPLUSPLUS__) && !defined(__CONSOLE__)
    catch(const Exception &E)
    {
      ShowMessage(AnsiString(E.ClassName())+ E.Message);
    }
#endif
    catch(const char* s)
    {
      Message_Function(s);
    }
    catch(...)
    {
      Message_Function("UNKNOWN EXCEPTION OCCURED");
    }

    return status;
}

#undef  MESSAGE_FUNCTION

