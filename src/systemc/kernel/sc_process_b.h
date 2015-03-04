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

  sc_process_b.h -- Process base class.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_PROCESS_B_H
#define SC_PROCESS_B_H


#include "systemc/kernel/sc_event.h"
#include "systemc/kernel/sc_object.h"
#include "systemc/kernel/sc_process.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_process_b
//
//  Process base class.
// ----------------------------------------------------------------------------

class sc_process_b
: public sc_object
{
    friend class sc_port_base;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;

    friend bool timed_out( sc_simcontext* );

public:

    SC_ENTRY_FUNC entry_fn;
    sc_module*    module;
    int           proc_id;
    const char*   file;
    int           lineno;


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    sc_process_b( const char*   nm,
		  SC_ENTRY_FUNC fn,
		  sc_module*    mod );
    virtual ~sc_process_b();

    bool do_initialize() const;
    void do_initialize( bool );

    void add_static_event( const sc_event& );
    void remove_static_events();

    bool trigger_static();
    
    void is_runnable( bool );

    void execute();

    bool timed_out() const;

protected:

    enum trigger_t
    {
	STATIC,
	EVENT,
	OR_LIST,
	AND_LIST,
	TIMEOUT,
	EVENT_TIMEOUT,
	OR_LIST_TIMEOUT,
	AND_LIST_TIMEOUT
    };

    bool                        m_do_initialize;
    sc_pvector<const sc_event*> m_static_events;
    bool                        m_is_runnable;
    trigger_t                   m_trigger_type;
    const sc_event*             m_event;
    sc_event_list*              m_event_list;
    int                         m_event_count;
    sc_event                    m_timeout_event;
    bool                        m_timed_out;
};


#endif

// Taf!
