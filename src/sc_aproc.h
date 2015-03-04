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

    sc_aproc.h -- asynchronous processes

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_APROC_H
#define SC_APROC_H

class sc_signal_base;
class sc_signal_array_base;
class sc_port_b;

class sc_aproc : public sc_module {
    friend class sc_simcontext;
    friend class sc_signal_base;
    friend class sc_signal_edgy;

protected:
    sc_aproc( const char* nm, size_t stack_size = SC_DEFAULT_STACK_SIZE );
    ~sc_aproc();
    virtual void entry() = 0;

    /* The following shadows those declared in sc_async */
    // Old names - should not be used
    void decl_input( const sc_signal_base& );
    void decl_input( const sc_signal_array_base& );
    void decl_input( const sc_signal_bool_vector& );
    void decl_input( const sc_port_b& );

    // Dummy functions - not needed anymore
    void decl_output( const sc_signal_base& );
    void decl_output( const sc_signal_array_base& );

    void wait()       { sc_module::wait(); }

private:
    void init( size_t stack_size );
    sc_aproc_process_handle aproc_handle;

    void wait_until(); /* shadow sc_module::wait_until() */
    void wait(int);    /* shadow sc_module::wait(int) */
    void watching();   /* shadow sc_module::watching() */
#ifndef SC_USE_MEMBER_FUNC_PTR
    public:
    sc_vc6_process_helper_class<sc_aproc> sc_vc6_process_helper;
#endif
};

#endif
