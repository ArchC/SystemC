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

  sc_sensitive.h -- Sensitivity classes. Requires "sc_process.h"
  for declarations of sc_method_handle, &.c.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_SENSITIVE_H
#define SC_SENSITIVE_H


#include "systemc/kernel/sc_process.h"


class sc_event;
class sc_event_finder;
class sc_interface;
class sc_port_base;
template <class T> class sc_in;
template <class T> class sc_inout;
template <class T> class sc_signal_in_if;
template <class T> class sc_signal_inout_if;


// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive
//
//  Static sensitivity class for events.
// ----------------------------------------------------------------------------

class sc_sensitive
{
    friend class sc_module;

public:

    // typedefs
    typedef sc_signal_in_if<bool>    in_if_type;
    typedef sc_signal_inout_if<bool> inout_if_type;
    typedef sc_in<bool>              in_port_type;
    typedef sc_inout<bool>           inout_port_type;

private:

    // constructor
    explicit sc_sensitive( sc_module* );

    // destructor
    ~sc_sensitive();

public:

    // changing between process handles
    sc_sensitive& operator << ( sc_method_handle );
    sc_sensitive& operator << ( sc_thread_handle );

    sc_sensitive& operator () ( const sc_event& );
    sc_sensitive& operator () ( const sc_interface& );
    sc_sensitive& operator () ( const sc_port_base& );
    sc_sensitive& operator () ( sc_event_finder& );

    sc_sensitive& operator << ( const sc_event& );
    sc_sensitive& operator << ( const sc_interface& );
    sc_sensitive& operator << ( const sc_port_base& );
    sc_sensitive& operator << ( sc_event_finder& );

    sc_sensitive& operator () ( sc_cthread_handle, sc_event_finder& );
    sc_sensitive& operator () ( sc_cthread_handle, const in_if_type& );
    sc_sensitive& operator () ( sc_cthread_handle, const in_port_type& );
    sc_sensitive& operator () ( sc_cthread_handle, const inout_port_type& );

private:

    sc_module*                                m_module;
    enum { SC_NONE_, SC_METHOD_, SC_THREAD_ } m_mode;
    sc_process_b*                             m_handle;

private:

    // disabled

    sc_sensitive();
    sc_sensitive( const sc_sensitive& );
    sc_sensitive& operator = ( const sc_sensitive& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive_pos
//
//  Static sensitivity class for positive edge events.
// ----------------------------------------------------------------------------

class sc_sensitive_pos
{
    friend class sc_module;

public:

    // typedefs
    typedef sc_signal_in_if<bool>    in_if_type;
    typedef sc_signal_inout_if<bool> inout_if_type;
    typedef sc_in<bool>              in_port_type;
    typedef sc_inout<bool>           inout_port_type;

private:

    // constructor
    explicit sc_sensitive_pos( sc_module* );

    // destructor
    ~sc_sensitive_pos();

public:

    // changing between process handles
    sc_sensitive_pos& operator << ( sc_method_handle );
    sc_sensitive_pos& operator << ( sc_thread_handle );

    sc_sensitive_pos& operator () ( const in_if_type& );
    sc_sensitive_pos& operator () ( const in_port_type& );
    sc_sensitive_pos& operator () ( const inout_port_type& );

    sc_sensitive_pos& operator << ( const in_if_type& );
    sc_sensitive_pos& operator << ( const in_port_type& );
    sc_sensitive_pos& operator << ( const inout_port_type& );

private:

    sc_module*                                m_module;
    enum { SC_NONE_, SC_METHOD_, SC_THREAD_ } m_mode;
    sc_process_b*                             m_handle;

private:

    // disabled
    sc_sensitive_pos();
    sc_sensitive_pos( const sc_sensitive_pos& );
    sc_sensitive_pos& operator = ( const sc_sensitive_pos& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_sensitive_neg
//
//  Static sensitivity class for negative edge events.
// ----------------------------------------------------------------------------

class sc_sensitive_neg
{
    friend class sc_module;

public:

    // typedefs
    typedef sc_signal_in_if<bool>    in_if_type;
    typedef sc_signal_inout_if<bool> inout_if_type;
    typedef sc_in<bool>              in_port_type;
    typedef sc_inout<bool>           inout_port_type;

private:

    // constructor
    explicit sc_sensitive_neg( sc_module* );

    // destructor
    ~sc_sensitive_neg();

public:

    // changing between process handles
    sc_sensitive_neg& operator << ( sc_method_handle );
    sc_sensitive_neg& operator << ( sc_thread_handle );

    sc_sensitive_neg& operator () ( const in_if_type& );
    sc_sensitive_neg& operator () ( const in_port_type& );
    sc_sensitive_neg& operator () ( const inout_port_type& );

    sc_sensitive_neg& operator << ( const in_if_type& );
    sc_sensitive_neg& operator << ( const in_port_type& );
    sc_sensitive_neg& operator << ( const inout_port_type& );

private:

    sc_module*                                m_module;
    enum { SC_NONE_, SC_METHOD_, SC_THREAD_ } m_mode;
    sc_process_b*                             m_handle;

private:

    // disabled
    sc_sensitive_neg();
    sc_sensitive_neg( const sc_sensitive_neg& );
    sc_sensitive_neg& operator = ( const sc_sensitive_neg& );
};


#endif

// Taf!
