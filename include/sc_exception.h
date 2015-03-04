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

    sc_exception.h -- exception handling for SystemC classes. All SystemC
    classes are expected to throw only sc_exception (and derived from it)
    objects, standard C++ exceptions, and structured exceptions (platform
    dependent)

    Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
//---------------------------------------------------------------------------

#ifndef sc_exceptionH
#define sc_exceptionH
#include "sc_string.h"
#include <stdio.h>
//---------------------------------------------------------------------------
// base exception class
class sc_exception
{
  protected:
  sc_string What;
  public:
  sc_exception () throw();
  sc_exception(const sc_string& What) throw();
  sc_exception(int Error) throw();
  sc_exception(int Error, const sc_string& Info) throw();
  sc_exception (const sc_exception&) throw();
  sc_exception& operator= (const sc_exception&) throw();
  virtual ~sc_exception () throw();
  const char* what() const throw();
  virtual const sc_string str() const throw();
#if !defined(__SUNPRO_CC)
  template<class T>
  sc_exception& fmt(const T& t) throw()
  { try{ What.fmt(t);} catch(...){}; return *this;}
#endif
};
// exception class for data types
class sc_edata : public sc_exception
{
  public:
  sc_edata () throw();
  sc_edata(const sc_string& What) throw();
  sc_edata(int Error) throw();
  sc_edata(int Error, const sc_string& Info) throw();
  sc_edata (const sc_edata&) throw();
  sc_edata& operator= (const sc_edata&) throw();
  virtual ~sc_edata () throw();
  virtual const sc_string str() const throw();
};
// exception class for signals
class sc_esignal : public sc_exception
{
  public:
  sc_esignal () throw();
  sc_esignal(const sc_string& What) throw();
  sc_esignal(int Error) throw();
  sc_esignal(int Error, const sc_string& Info) throw();
  sc_esignal (const sc_esignal&) throw();
  sc_esignal& operator= (const sc_esignal&) throw();
  virtual ~sc_esignal () throw();
  virtual const sc_string str() const throw();
};
// exception class for processes
class sc_eprocess : public sc_exception
{
  public:
  sc_eprocess () throw();
  sc_eprocess(const sc_string& What) throw();
  sc_eprocess(int Error) throw();
  sc_eprocess(int Error, const sc_string& Info) throw();
  sc_eprocess (const sc_eprocess&) throw();
  sc_eprocess& operator= (const sc_eprocess&) throw();
  virtual ~sc_eprocess () throw();
  virtual const sc_string str() const throw();
};
// exception class for modules
class sc_emodule : public sc_exception
{
  public:
  sc_emodule () throw();
  sc_emodule(const sc_string& What) throw();
  sc_emodule(int Error) throw();
  sc_emodule(int Error, const sc_string& Info) throw();
  sc_emodule (const sc_emodule&) throw();
  sc_emodule& operator= (const sc_emodule&) throw();
  virtual ~sc_emodule () throw();
  virtual const sc_string str() const throw();
};

// convenience error reporting macros
#if defined(__BCPLUSPLUS__)
#define REPORT_ERROR(N,msg) \
    throw sc_exception(N,\
      sc_string::to_string("%s\n - function: %s; %s: %d",msg,__FUNC__,__FILE__,__LINE__))
#else
#define REPORT_ERROR(N,msg) \
    throw sc_exception(N,sc_string::to_string("%s\n - %s: %d",msg,__FILE__,__LINE__))
#endif

#if defined(SC_NO_WARNINGS)
 #define REPORT_WARNING(N,msg)
#else

  #if defined(__BCPLUSPLUS__)
    #if defined(__CONSOLE__)
    #define REPORT_WARNING(N,msg) \
        std::cout<<"\nWARNING: "<<sc_exception(N,\
         sc_string::to_string("%s\n - function: %s; %s: %d",msg,__FUNC__,__FILE__,__LINE__)).str() \
         <<"\n"
    #else
    #define REPORT_WARNING(N,msg) \
        ShowMessage(sc_string("WARNING: ") + sc_exception(N,\
         sc_string::to_string("%s\n - function: %s; %s: %d",msg,__FUNC__,__FILE__,__LINE__)).what())
    #endif
  #else
  #define REPORT_WARNING(N,msg) \
      cout<<"\nWARNING: "<<sc_exception(N,sc_string::to_string("%s\n - %s: %d",msg,__FILE__,__LINE__)).str()<<"\n"
  #endif
#endif //SC_NO_WARNINGS

#endif
