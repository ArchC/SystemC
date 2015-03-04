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

  sc_clock.h -- The clock channel.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_CLOCK_H
#define SC_CLOCK_H


#include "systemc/kernel/sc_module.h"
#include "systemc/communication/sc_signal_ifs.h"
#include "systemc/tracing/sc_trace.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_clock
//
//  The clock channel.
// ----------------------------------------------------------------------------

class sc_clock
: public sc_signal_in_if<bool>,
  public sc_module
{
public:

    SC_HAS_PROCESS( sc_clock );


    // constructors

    sc_clock();

    explicit sc_clock( sc_module_name name_ );

    sc_clock( sc_module_name name_,
	      const sc_time& period_,
	      double         duty_cycle_ = 0.5,
	      const sc_time& start_time_ = SC_ZERO_TIME,
	      bool           posedge_first_ = true );

    sc_clock( sc_module_name name_,
	      double         period_v_,
	      sc_time_unit   period_tu_,
	      double         duty_cycle_ = 0.5 );

    sc_clock( sc_module_name name_,
	      double         period_v_,
	      sc_time_unit   period_tu_,
	      double         duty_cycle_,
	      double         start_time_v_,
	      sc_time_unit   start_time_tu_,
	      bool           posedge_first_ = true );

    // for backward compatibility with 1.0
    sc_clock( sc_module_name name_,
	      double         period_,            // in default time units
	      double         duty_cycle_ = 0.5,
	      double         start_time_ = 0.0,  // in default time units
	      bool           posedge_first_ = true );


    // destructor (does nothing)
    virtual ~sc_clock();


    // interface methods

    // get the default event
    virtual const sc_event& default_event() const;


    // get the value changed event
    virtual const sc_event& value_changed_event() const;

    // get the positive edge event
    virtual const sc_event& posedge_event() const;

    // get the negative edge event
    virtual const sc_event& negedge_event() const;


    // read the current value
    virtual const bool& read() const;

    // get a reference to the current value (for tracing)
    virtual const bool& get_data_ref() const
        { return m_cur_val; }


    // was there a value changed event?
    virtual bool event() const;

    // was there a positive edge event?
    virtual bool posedge() const;

    // was there a negative edge event?
    virtual bool negedge() const;


    // delayed evaluation
     virtual const sc_signal_bool_deval& delayed() const;


    // other methods

    operator const bool& () const
	{ return read(); }


    // get the period
    const sc_time& period() const
	{ return m_period; }

    // get the duty cycle
    double duty_cycle() const
	{ return m_duty_cycle; }


    // get the current time
    static const sc_time& time_stamp();


    void trace( sc_trace_file* tf ) const
#ifdef DEBUG_SYSTEMC
	{ ::sc_trace( tf, get_data_ref(), name() ); }
#else
	{}
#endif


    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }


    // for backward compatibility with 1.0

    sc_signal_in_if<bool>& signal()
	{ return *this; }

    const sc_signal_in_if<bool>& signal() const
	{ return *this; }

    static void start( const sc_time& duration )
	{ sc_start( duration ); }

    static void start( double v, sc_time_unit tu )
	{ sc_start( v, tu ); }

    static void start( double duration = -1 )
	{ sc_start( duration ); }

    static void stop()
	{ sc_stop(); }

protected:

    // processes
    void posedge_action();
    void negedge_action();


    // error reporting
    void report_error( int id, const char* add_msg = 0 ) const;


    void init( const sc_time&, double, const sc_time&, bool );

protected:

    sc_time  m_period;		// the period of this clock
    double   m_duty_cycle;	// the duty cycle (fraction of period)
    sc_time  m_start_time;	// the start time of the first edge
    sc_time  m_posedge_time;	// time till next positive edge
    sc_time  m_negedge_time;	// time till next negative edge

    bool     m_cur_val;

    sc_event m_value_changed_event;
    sc_event m_posedge_event;
    sc_event m_negedge_event;

    sc_event m_next_posedge_event;
    sc_event m_next_negedge_event;

    uint64   m_delta; // delta of last event

private:

    // disabled
    sc_clock( const sc_clock& );
    sc_clock& operator = ( const sc_clock& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// interface methods

// get the default event

inline
const sc_event&
sc_clock::default_event() const
{
    return m_value_changed_event;
}


// get the value changed event

inline
const sc_event&
sc_clock::value_changed_event() const
{
    return m_value_changed_event;
}

// get the positive edge event

inline
const sc_event&
sc_clock::posedge_event() const
{
    return m_posedge_event;
}

// get the negative edge event

inline
const sc_event&
sc_clock::negedge_event() const
{
    return m_negedge_event;
}


// read the current value

inline
const bool&
sc_clock::read() const
{
    return m_cur_val;
}


// was there a value changed event?

inline
bool
sc_clock::event() const
{
    return ( simcontext()->delta_count() == m_delta + 1 );
}

// was there a positive edge event?

inline
bool
sc_clock::posedge() const
{
    return ( event() && m_cur_val );
}

// was there a negative edge event?

inline
bool
sc_clock::negedge() const
{
    return ( event() && ! m_cur_val );
}


// processes

inline
void
sc_clock::posedge_action()
{
    m_cur_val = true;
    m_posedge_event.notify_delayed();
    m_value_changed_event.notify_delayed();
    m_next_negedge_event.notify_delayed( m_negedge_time );
    m_delta = simcontext()->delta_count();
}

inline
void
sc_clock::negedge_action()
{
    m_cur_val = false;
    m_negedge_event.notify_delayed();
    m_value_changed_event.notify_delayed();
    m_next_posedge_event.notify_delayed( m_posedge_time );
    m_delta = simcontext()->delta_count();
}


// ----------------------------------------------------------------------------

// for backward compatibility with 1.0

inline
void
sc_start( sc_clock& clock, const sc_time& duration )
{
    clock.start( duration );
}

inline
void
sc_start( sc_clock& clock, double v, sc_time_unit tu )
{
    clock.start( v, tu );
}

inline
void
sc_start( sc_clock& clock, double duration = -1 )
{
    clock.start( duration );
}


#endif

// Taf!
