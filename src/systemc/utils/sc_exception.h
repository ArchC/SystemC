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

  sc_exception.h -- Exception handling for SystemC classes. All SystemC
                    classes are expected to throw only sc_exception (and
                    derived from it) objects, standard C++ exceptions, and
                    structured exceptions (platform dependent).

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_EXCEPTION_H
#define SC_EXCEPTION_H


#include <stdio.h>

#include "systemc/utils/sc_string.h"


// ----------------------------------------------------------------------------
//  FUNCTION : sc_stop_here
//
//  Debugging aid for warning, error, and fatal reports.
// ----------------------------------------------------------------------------

extern
void
sc_stop_here();


// ----------------------------------------------------------------------------
//  CLASS : sc_exception
//
//  Exception base class.
// ----------------------------------------------------------------------------

class sc_exception
{
public:

    sc_exception() throw();
    sc_exception( const sc_string& What ) throw();
    sc_exception( int Error ) throw();
    sc_exception( int Error, const sc_string& Info ) throw();
    sc_exception( const sc_exception& ) throw();

    sc_exception& operator = ( const sc_exception& ) throw();

    virtual ~sc_exception() throw();

    const char* what() const throw();
    virtual const sc_string str() const throw();

protected:

    sc_string What;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_edata
//
//  Exception class for data types.
// ----------------------------------------------------------------------------

class sc_edata
: public sc_exception
{
public:

    sc_edata() throw();
    sc_edata( const sc_string& What ) throw();
    sc_edata( int Error ) throw();
    sc_edata( int Error, const sc_string& Info ) throw();
    sc_edata( const sc_edata& ) throw();

    sc_edata& operator = ( const sc_edata& ) throw();

    virtual ~sc_edata () throw();

    virtual const sc_string str() const throw();
};


// ----------------------------------------------------------------------------
//  CLASS : sc_esignal
//
//  Exception class for signals.
// ----------------------------------------------------------------------------

class sc_esignal
: public sc_exception
{
public:

    sc_esignal() throw();
    sc_esignal( const sc_string& What ) throw();
    sc_esignal( int Error ) throw();
    sc_esignal( int Error, const sc_string& Info ) throw();
    sc_esignal( const sc_esignal& ) throw();

    sc_esignal& operator = ( const sc_esignal& ) throw();

    virtual ~sc_esignal () throw();

    virtual const sc_string str() const throw();
};


// ----------------------------------------------------------------------------
//  CLASS : sc_eprocess
//
//  Exception class for processes.
// ----------------------------------------------------------------------------

class sc_eprocess
: public sc_exception
{
public:

    sc_eprocess() throw();
    sc_eprocess( const sc_string& What ) throw();
    sc_eprocess( int Error ) throw();
    sc_eprocess( int Error, const sc_string& Info ) throw();
    sc_eprocess( const sc_eprocess& ) throw();

    sc_eprocess& operator = ( const sc_eprocess& ) throw();

    virtual ~sc_eprocess () throw();

    virtual const sc_string str() const throw();
};


// ----------------------------------------------------------------------------
//  CLASS : sc_emodule
//
//  Exception class for modules.
// ----------------------------------------------------------------------------

class sc_emodule
: public sc_exception
{
public:

    sc_emodule() throw();
    sc_emodule( const sc_string& What ) throw();
    sc_emodule( int Error ) throw();
    sc_emodule( int Error, const sc_string& Info ) throw();
    sc_emodule( const sc_emodule& ) throw();

    sc_emodule& operator = ( const sc_emodule& ) throw();

    virtual ~sc_emodule () throw();

    virtual const sc_string str() const throw();
};


// convenience error reporting macros

#define REPORT_ERROR(N,msg)                                                   \
{                                                                             \
    sc_stop_here();                                                           \
    throw sc_exception( N, sc_string::to_string( "%s\n - %s: %d",             \
						 msg,                         \
						 __FILE__,                    \
						 __LINE__ ) );                \
}

#if defined( SC_NO_WARNINGS )

#define REPORT_WARNING(N,msg)

#else

#define REPORT_WARNING(N,msg)                                                 \
{                                                                             \
    sc_stop_here();                                                           \
    cout << "\nWARNING: "                                                     \
         << sc_exception( N, sc_string::to_string( "%s\n - %s: %d",           \
						   msg,                       \
						   __FILE__,                  \
						   __LINE__ ) ).str()         \
         << "\n";                                                             \
}

#endif


#endif
