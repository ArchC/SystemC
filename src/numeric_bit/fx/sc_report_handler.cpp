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

    sc_report_handler.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_report_handler.h"

#include <stdlib.h>
#include "../../sc_iostream.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_report_handler
//
//  Default report handler class.
// ----------------------------------------------------------------------------

// for your own handler, change the handler name

#define HANDLER_NAME sc_report_handler


static
HANDLER_NAME*&
the_handler()
{
    static HANDLER_NAME* handler = 0;
    return handler;
}


void
HANDLER_NAME::install()
{
    if( the_handler() == 0 )
    {
	the_handler() = new HANDLER_NAME;
    }
    the_handler()->install_();
}

void
HANDLER_NAME::deinstall()
{
    if( the_handler() != 0 )
    {
	the_handler()->deinstall_();
    }
}


HANDLER_NAME::HANDLER_NAME()
{}

HANDLER_NAME::~HANDLER_NAME()
{}


// for your own handler, change the body of this function

void
HANDLER_NAME::report( sc_severity s,
		      const char* id,
		      const char* add_msg ) const
{
    const char* msg = sc_report::message( id );

    if( s == SC_INFO )
    {
	cout << "SystemC INFORMATION";

	if( msg != 0 )
	{
	    cout << ": " << msg;
	}

	if( add_msg != 0 )
	{
	    cout << ": " << add_msg;
	}

	cout << " <" << id << ">" << endl;
    }
    else
    {
	cerr << "SystemC ";

	switch( s )
	{
	    case SC_WARNING:
	        cerr << "WARNING";
		break;
	    case SC_ERROR:
		cerr << "ERROR";
		break;
	    case SC_FATAL:
		cerr << "FATAL";
		break;
	    default:
		cerr << "UNKNOWN";
	}

	if( msg != 0 )
	{
	    cerr << ": " << msg;
	}

	if( add_msg != 0 )
	{
	    cerr << ": " << add_msg;
	}

	cerr << " <" << id << ">" << endl;

	sc_stop_here();

	if( s == SC_ERROR )
	{
	    throw id;
	}
	else if( s == SC_FATAL )
	{
	    abort();
	}
    }
}


// for your own handler, change the handler garbage collector name

#define HANDLER_GC_NAME sc_report_handler_gc


class HANDLER_GC_NAME
{

public:
    
    ~HANDLER_GC_NAME()
	{ if( the_handler() != 0 ) { delete the_handler(); } }

};


static HANDLER_GC_NAME gc;


// Taf!
