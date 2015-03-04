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

  sc_report.cpp - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc/datatypes/fx/sc_report.h"
#include "systemc/datatypes/fx/sc_report_handler.h"
#include "systemc/utils/sc_hash.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_report
//
//  Report class.
// ----------------------------------------------------------------------------

static
const sc_report_handler_base*&
the_handler()
{
    static const sc_report_handler_base* handler;
    return handler;
}


void
sc_report::report( sc_severity s, const char* id, const char* add_msg )
{
    the_handler()->report( s, id, add_msg );
}


typedef sc_strhash<const char*> dictionary_t;


static
dictionary_t&
the_dictionary()
{
    static dictionary_t dictionary;
    return dictionary;
}


static
int
dictionary_insert( const char* key, const char* value )
{
    return the_dictionary().insert( CCAST<char*>( key ), strdup( value ) );
}


void
sc_report::register_id( const char* id, const char* msg )
{
    if( id == 0 )
    {
	error( "register_id_failed", "invalid report id" );
    }

    if( msg == 0 )
    {
	error( "register_id_failed", "invalid report message" );
    }

    if( the_dictionary()[ id ] != 0 )
    {
	error( "register_id_failed", "report id already exists" );
    }

    if( dictionary_insert( id, msg ) != 0 )
    {
	error( "register_id_failed", "dictionary insertion error" );
    }
}


const char*
sc_report::message( const char* id )
{
    return the_dictionary()[ id ];
}


static
int
initialize()
{
    dictionary_insert( "register_id_failed",
		       "register_id failed" );

    sc_report_handler::install();

    return 42;
}

static int forty_two = initialize();


// ----------------------------------------------------------------------------
//  CLASS : sc_report_handler_base
//
//  Report handler virtual base class.
// ----------------------------------------------------------------------------

sc_report_handler_base::sc_report_handler_base()
: m_old_handler( 0 )
{}

sc_report_handler_base::~sc_report_handler_base()
{}


void
sc_report_handler_base::install_()
{
    if( m_old_handler == 0 && the_handler() != this )
    {
	m_old_handler = the_handler();
	the_handler() = this;
    }
}

void
sc_report_handler_base::deinstall_()
{
    if( m_old_handler != 0 && the_handler() == this )
    {
	the_handler() = m_old_handler;
	m_old_handler = 0;
    }
}


// Taf!
