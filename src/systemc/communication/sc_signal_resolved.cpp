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

  sc_signal_resolved.cpp -- The resolved signal class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include "systemc/communication/sc_signal_resolved.h"


// Note that we assume that two drivers driving the resolved signal to a 1 or
// 0 is O.K. This might not be true for all technologies, but is certainly
// true for CMOS, the predominant technology in use today.

#define LOG_0 sc_logic::Log_0
#define LOG_1 sc_logic::Log_1
#define LOG_Z sc_logic::Log_Z
#define LOG_X sc_logic::Log_X

const sc_logic::Log_enum
sc_logic_resolution_tbl[4][4] =
{   //    0      1      Z      X
    { LOG_0, LOG_X, LOG_0, LOG_X }, // 0
    { LOG_X, LOG_1, LOG_1, LOG_X }, // 1
    { LOG_0, LOG_1, LOG_Z, LOG_X }, // Z
    { LOG_X, LOG_X, LOG_X, LOG_X }  // X
};

#undef LOG_0
#undef LOG_1
#undef LOG_Z
#undef LOG_X


// ----------------------------------------------------------------------------
//  CLASS : sc_logic_resolve
//
//  Resolution function for sc_logic.
// ----------------------------------------------------------------------------

// resolves sc_logic values and returns the resolved value

void
sc_logic_resolve::resolve( sc_logic& result_,
			    const sc_pvector<sc_logic*>& values_ )
{
    int sz = values_.size();

    assert( sz != 0 );

    if( sz == 1 ) {
	result_ = *values_[0];
	return;
    }

    long res = values_[0]->to_long();
    for( int i = sz - 1; i > 0 && res != 3; -- i ) {
	res = sc_logic_resolution_tbl[res][values_[i]->to_long()];
    }
    result_ = static_cast<sc_logic::Log_enum>( res );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal_resolved
//
//  The resolved signal class.
// ----------------------------------------------------------------------------

const char* const sc_signal_resolved::kind_string = "sc_signal_resolved";


// destructor

sc_signal_resolved::~sc_signal_resolved()
{
    for( int i = m_val_vec.size() - 1; i >= 0; -- i ) {
	delete m_val_vec[i];
    }
}


// write the new value

void
sc_signal_resolved::write( const data_type& value_ )
{
    sc_process_b* cur_proc = sc_get_curr_process_handle();

    bool value_changed = false;
    bool found = false;
    
    for( int i = m_proc_vec.size() - 1; i >= 0; -- i ) {
	if( cur_proc == m_proc_vec[i] ) {
	    if( value_ != *m_val_vec[i] ) {
		*m_val_vec[i] = value_;
		value_changed = true;
	    }
	    found = true;
	    break;
	}
    }
    
    if( ! found ) {
	m_proc_vec.push_back( cur_proc );
	m_val_vec.push_back( new data_type( value_ ) );
	value_changed = true;
    }
    
    if( value_changed ) {
	request_update();
    }
}


void
sc_signal_resolved::update()
{
    sc_logic_resolve::resolve( m_new_val, m_val_vec );
    base_type::update();
}


// Taf!
