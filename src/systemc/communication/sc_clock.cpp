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

  sc_clock.cpp -- The clock channel.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/


#include "systemc/communication/sc_clock.h"
#include "systemc/communication/sc_communication_ids.h"
#include "systemc/kernel/sc_simcontext.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_clock
//
//  The clock channel.
// ----------------------------------------------------------------------------

const char* const sc_clock::kind_string = "sc_clock";


// constructors

sc_clock::sc_clock()
: sc_module( sc_gen_unique_name( "clock" ) )
{
    init( sc_time( 1.0, true ),
	  0.5,
	  SC_ZERO_TIME,
	  true );

    SC_METHOD( posedge_action );
    sensitive << m_next_posedge_event;
    dont_initialize();

    SC_METHOD( negedge_action );
    sensitive << m_next_negedge_event;
    dont_initialize();

    // posedge first
    m_next_posedge_event.notify_delayed( m_start_time );

    end_module();
}

sc_clock::sc_clock( sc_module_name name_ )
: sc_module( name_ )
{
    init( sc_time( 1.0, true ),
	  0.5,
	  SC_ZERO_TIME,
	  true );

    SC_METHOD( posedge_action );
    sensitive << m_next_posedge_event;
    dont_initialize();

    SC_METHOD( negedge_action );
    sensitive << m_next_negedge_event;
    dont_initialize();

    // posedge first
    m_next_posedge_event.notify_delayed( m_start_time );
}

sc_clock::sc_clock( sc_module_name name_,
		    const sc_time& period_,
		    double         duty_cycle_,
		    const sc_time& start_time_,
		    bool           posedge_first_ )
: sc_module( name_ )
{
    init( period_,
	  duty_cycle_,
	  start_time_,
	  posedge_first_ );

    SC_METHOD( posedge_action );
    sensitive << m_next_posedge_event;
    dont_initialize();

    SC_METHOD( negedge_action );
    sensitive << m_next_negedge_event;
    dont_initialize();
    
    if( posedge_first_ ) {
	// posedge first
	m_next_posedge_event.notify_delayed( m_start_time );
    } else {
	// negedge first
	m_next_negedge_event.notify_delayed( m_start_time );
    }
}

sc_clock::sc_clock( sc_module_name name_,
		    double         period_v_,
		    sc_time_unit   period_tu_,
		    double         duty_cycle_ )
: sc_module( name_ )
{
    init( sc_time( period_v_, period_tu_, simcontext() ),
	  duty_cycle_,
	  SC_ZERO_TIME,
	  true );

    SC_METHOD( posedge_action );
    sensitive << m_next_posedge_event;
    dont_initialize();

    SC_METHOD( negedge_action );
    sensitive << m_next_negedge_event;
    dont_initialize();

    // posedge first
    m_next_posedge_event.notify_delayed( m_start_time );
}

sc_clock::sc_clock( sc_module_name name_,
		    double         period_v_,
		    sc_time_unit   period_tu_,
		    double         duty_cycle_,
		    double         start_time_v_,
		    sc_time_unit   start_time_tu_,
		    bool           posedge_first_ )
: sc_module( name_ )
{
    init( sc_time( period_v_, period_tu_, simcontext() ),
	  duty_cycle_,
	  sc_time( start_time_v_, start_time_tu_, simcontext() ),
	  posedge_first_ );

    SC_METHOD( posedge_action );
    sensitive << m_next_posedge_event;
    dont_initialize();

    SC_METHOD( negedge_action );
    sensitive << m_next_negedge_event;
    dont_initialize();
    
    if( posedge_first_ ) {
	// posedge first
	m_next_posedge_event.notify_delayed( m_start_time );
    } else {
	// negedge first
	m_next_negedge_event.notify_delayed( m_start_time );
    }
}

// for backward compatibility with 1.0
sc_clock::sc_clock( sc_module_name name_,
		    double         period_,      // in default time units
		    double         duty_cycle_,
		    double         start_time_,  // in default time units
		    bool           posedge_first_ )
: sc_module( name_ )
{
    init( sc_time( period_, true ),
	  duty_cycle_,
	  sc_time( start_time_, true ),
	  posedge_first_ );

    SC_METHOD( posedge_action );
    sensitive << m_next_posedge_event;
    dont_initialize();

    SC_METHOD( negedge_action );
    sensitive << m_next_negedge_event;
    dont_initialize();
    
    if( posedge_first_ ) {
	// posedge first
	m_next_posedge_event.notify_delayed( m_start_time );
    } else {
	// negedge first
	m_next_negedge_event.notify_delayed( m_start_time );
    }
}


// destructor (does nothing)

sc_clock::~sc_clock()
{}


// interface methods

// delayed evaluation

const sc_signal_bool_deval&
sc_clock::delayed() const
{
    const sc_signal_in_if<bool>* iface = this;
    return RCAST<const sc_signal_bool_deval&>( *iface );
}


// get the current time

const sc_time&
sc_clock::time_stamp()
{
    return sc_time_stamp();
}


void
sc_clock::print( ostream& os ) const
{
    os << m_cur_val;
}

void
sc_clock::dump( ostream& os ) const
{
    os << " name = " << name() << endl;
    os << "value = " << m_cur_val << endl;
}


// error reporting

void
sc_clock::report_error( int id, const char* add_msg ) const
{
    char msg[BUFSIZ];
    if( add_msg != 0 ) {
	sprintf( msg, "%s: clock '%s'", add_msg, name() );
    } else {
	sprintf( msg, "clock '%s'", name() );
    }
    SC_REPORT_ERROR( id, msg );
}


void
sc_clock::init( const sc_time& period_,
		double         duty_cycle_,
		const sc_time& start_time_,
		bool           posedge_first_ )
{
    if( period_ == SC_ZERO_TIME ) {
	report_error( SC_ID_CLOCK_PERIOD_ZERO_,
		      "increase the period" );
    }
    m_period = period_;
	
    if( duty_cycle_ <= 0.0 || duty_cycle_ >= 1.0 ) {
	m_duty_cycle = 0.5;
    } else {
	m_duty_cycle = duty_cycle_;
    }

    m_negedge_time = m_period * m_duty_cycle;
    m_posedge_time = m_period - m_negedge_time;

    if( m_negedge_time == SC_ZERO_TIME ) {
	report_error( SC_ID_CLOCK_HIGH_TIME_ZERO_,
		      "increase the period or increase the duty cycle" );
    }
    if( m_posedge_time == SC_ZERO_TIME ) {
	report_error( SC_ID_CLOCK_LOW_TIME_ZERO_,
		      "increase the period or decrease the duty cycle" );
    }

    if( posedge_first_ ) {
	m_cur_val = false;
    } else {
	m_cur_val = true;
    }

    m_start_time = start_time_;

    m_delta = ~sc_dt::UINT64_ONE;
}


// Taf!
