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

  sc_report_handler.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/utils/sc_exception.h"
#include "systemc/utils/sc_iostream.h"
#include "systemc/utils/sc_report_handler.h"
#include "systemc/utils/sc_stop_here.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/kernel/sc_process_b.h"


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
    if( the_handler() == 0 ) {
	the_handler() = new HANDLER_NAME;
    }
    the_handler()->install_();
}

void
HANDLER_NAME::deinstall()
{
    if( the_handler() != 0 ) {
	the_handler()->deinstall_();
    }
}


HANDLER_NAME::HANDLER_NAME()
{}

HANDLER_NAME::~HANDLER_NAME()
{}


static
const sc_string
compose_message( sc_severity severity,
		 int         id,
		 const char* add_msg,
		 const char* file,
		 int         line )
{
    sc_string str;

    const char* severity_names[] = { "Info", "Warning", "Error", "Fatal" };
    str += severity_names[severity];
    str += ": ";
    str += "(";
    str += "IWEF"[severity];
    str += sc_string::to_string( "%d", id );
    str += ")";
    const char* msg = sc_report::get_message( id );
    if( msg != 0 && *msg != 0 ) {
	str += " ";
	str += msg;
    }
    if( add_msg != 0 && *add_msg != 0 ) {
	str += ": ";
	str += add_msg;
    }
    if( severity != SC_INFO ) {
	str += "\nIn file: ";
	str += file;
	str += ":";
	str += sc_string::to_string( "%d", line );
	sc_simcontext* simc = sc_get_curr_simcontext();
	if( simc->is_running() ) {
	    sc_process_b* p = simc->get_curr_proc_info()->process_handle;
	    if( p != 0 ) {
		str += "\nIn process: ";
		str += p->name();
		str += " @ ";
		str += sc_time_stamp().to_string();
	    }
	}
    }

    return str;
}


// for your own handler, change the body of this function

void
HANDLER_NAME::report( sc_severity severity,
		      int         id,
		      const char* add_msg,
		      const char* file,
		      int         line )
{
    switch( severity ) {
    case SC_INFO: {
	if( m_suppress_infos ) {
	    return;
	}
	if( sc_report::is_suppressed( id ) ) {
	    return;
	}
	cout << "\n";
	cout << compose_message( severity, id, add_msg, file, line ) << endl;
	break;
    }
    case SC_WARNING: {
	if( m_make_warnings_errors ) {
	    severity = SC_ERROR;
	    // fall through to SC_ERROR
	} else {
	    if( m_suppress_warnings ) {
		return;
	    }
	    if( sc_report::is_suppressed( id ) ) {
		return;
	    }
	    sc_stop_here( id, severity );
	    cout << "\n";
	    cout << compose_message( severity, id, add_msg, file, line )
		 << endl;
	    break;
	}
    }
    case SC_ERROR: {
	sc_stop_here( id, severity );
	throw sc_exception( compose_message( severity, id, add_msg,
					     file, line ) );
    }
    case SC_FATAL: {
	sc_stop_here( id, severity );
	cout << "\n";
	cout << compose_message( severity, id, add_msg, file, line ) << endl;
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
