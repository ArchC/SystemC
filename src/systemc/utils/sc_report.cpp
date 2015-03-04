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

  sc_report.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/utils/sc_report.h"
#include "systemc/utils/sc_report_handler.h"
#include "systemc/utils/sc_hash.h"
#include "systemc/utils/sc_utils_ids.h"


// ----------------------------------------------------------------------------
//  STRUCT : sc_report_t
//
//  Local struct for holding a report message string and suppressed flag.
// ----------------------------------------------------------------------------

struct sc_report_t
{
    const char* msg;
    bool        suppressed;

    sc_report_t( const char* msg_ )
	: msg( strdup( msg_ ) ), suppressed( false )
	{}

    ~sc_report_t()
	{ delete [] CCAST<char*>( msg ); }
};


// ----------------------------------------------------------------------------
//  CLASS : sc_report
//
//  Report class.
// ----------------------------------------------------------------------------

typedef sc_phash<long,sc_report_t*> report_map_t;


static
report_map_t&
the_report_map()
{
    static report_map_t report_map;
    return report_map;
}

static
int
report_map_insert( int id, const char* msg )
{
    return the_report_map().insert( (long) id, new sc_report_t( msg ) );
}


void
sc_report::register_id( int id, const char* msg )
{
    if( id < 0 ) {
	SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			 "invalid report id" );
    }
    if( msg == 0 ) {
	SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			 "invalid report message" );
    }
    sc_report_t* r = the_report_map()[(long) id];
    if( r != 0 ) {
	if( strcmp( msg, r->msg ) != 0 ) {
	    SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			     "report id already exists" );
	}
	return;
    }
    if( report_map_insert( (long) id, msg ) != 0 ) {
	SC_REPORT_ERROR( SC_ID_REGISTER_ID_FAILED_,
			 "report_map insertion error" );
    }
}

const char*
sc_report::get_message( int id )
{
    sc_report_t* r = the_report_map()[(long) id];
    if( r != 0 ) {
	return r->msg;
    }
    return "unknown id";
}

bool
sc_report::is_suppressed( int id )
{
    sc_report_t* r = the_report_map()[(long) id];
    if( r != 0 ) {
	return r->suppressed;
    }
    return false;
}


// ----------------------------------------------------------------------------

static
sc_report_handler_base*&
the_handler()
{
    static sc_report_handler_base* handler;
    return handler;
}


void
sc_report::report( sc_severity severity,
		   int         id,
		   const char* add_msg,
		   const char* file,
		   int         line )
{
    the_handler()->report( severity, id, add_msg, file, line );
}


void
sc_report::suppress_id( int id, bool b )
{
    sc_report_t* r = the_report_map()[(long) id];
    if( r != 0 ) {
	r->suppressed = b;
    }
}

void
sc_report::suppress_infos( bool b )
{
    the_handler()->suppress_infos( b );
}

void
sc_report::suppress_warnings( bool b )
{
    the_handler()->suppress_warnings( b );
}

void
sc_report::make_warnings_errors( bool b )
{
    the_handler()->make_warnings_errors( b );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_report_handler_base
//
//  Report handler virtual base class.
// ----------------------------------------------------------------------------

sc_report_handler_base::sc_report_handler_base()
: m_suppress_infos( false ),
  m_suppress_warnings( false ),
  m_make_warnings_errors( false ),
  m_old_handler( 0 )
{}

sc_report_handler_base::~sc_report_handler_base()
{}


void
sc_report_handler_base::suppress_infos( bool b )
{
    m_suppress_infos = b;
}

void
sc_report_handler_base::suppress_warnings( bool b )
{
    m_suppress_warnings = b;
}

void
sc_report_handler_base::make_warnings_errors( bool b )
{
    m_make_warnings_errors = b;
}


void
sc_report_handler_base::install_()
{
    if( m_old_handler == 0 && the_handler() != this ) {
	m_old_handler = the_handler();
	the_handler() = this;
    }
}

void
sc_report_handler_base::deinstall_()
{
    if( m_old_handler != 0 && the_handler() == this ) {
	the_handler() = m_old_handler;
	m_old_handler = 0;
    }
}


// ----------------------------------------------------------------------------
//  Report ids (general).
//
//  Report ids in the range of 0-99.
// ----------------------------------------------------------------------------

const int SC_ID_UNKNOWN_ERROR_      =  0;
const int SC_ID_WITHOUT_MESSAGE_    =  1;
const int SC_ID_NOT_IMPLEMENTED_    =  2;
const int SC_ID_INTERNAL_ERROR_     =  3;
const int SC_ID_ASSERTION_FAILED_   =  4;
const int SC_ID_OUT_OF_BOUNDS_      =  5;


// ----------------------------------------------------------------------------

// initialization

static
int
initialize()
{
    // special case
    report_map_insert( SC_ID_REGISTER_ID_FAILED_,
		       "register_id failed" );

    sc_report::register_id( SC_ID_UNKNOWN_ERROR_,
			    "unknown error" );
    sc_report::register_id( SC_ID_WITHOUT_MESSAGE_,
			    "" );
    sc_report::register_id( SC_ID_NOT_IMPLEMENTED_,
			    "not implemented" );
    sc_report::register_id( SC_ID_INTERNAL_ERROR_,
			    "internal error" );
    sc_report::register_id( SC_ID_ASSERTION_FAILED_,
			    "assertion failed" );
    sc_report::register_id( SC_ID_OUT_OF_BOUNDS_,
			    "out of bounds" );

    sc_report_handler::install();

    return 42;
}

static int forty_two = initialize();


// Taf!
