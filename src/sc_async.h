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

    sc_async.h -- combinational processes

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_ASYNC_H
#define SC_ASYNC_H

#include "sc_macros.h"
#include "sc_module.h"
#include "sc_sensitive.h"

class sc_signal_base;
class sc_signal_array_base;
class sc_signal_edgy;

template<class T> class sc_in;
template<class T> class sc_inout;

/*---------------------------------------------------------------------------*/

class sc_async : public sc_module {
    friend class sc_simcontext;
    friend class sc_signal_base;
    friend class sc_signal_edgy;

public:
    const char* kind() const;
    static const char* kind_string;

    bool validate() const;
    virtual void entry() = 0;

protected:
    sc_async( const char* nm = 0 );
    sc_async( const sc_string& nm );
    virtual ~sc_async();

    // Old names - should not be used anymore
    void decl_input ( const sc_signal_base& );
    void decl_input ( const sc_signal_array_base& );
    void decl_input ( const sc_port_b& );

    // Dummy functions - not needed anymore
    void decl_output( const sc_signal_base& );
    void decl_output( const sc_signal_array_base& );

private:
    sc_async_process_handle async_handle;
    bool is_aproc;

    /* These are forbidden for sc_async. */
    void wait_until(); /* shadow sc_module::wait_until() */
    void wait()   ;    /* shadow sc_module::wait() */
    void watching();   /* shadow sc_module::watching() */
#ifndef SC_USE_MEMBER_FUNC_PTR
    public:
    sc_vc6_process_helper_class<sc_async> sc_vc6_process_helper;
#endif
};

typedef sc_async sc_combo;

/*---------------------------------------------------------------------------*/

#endif
