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

    sc_module.cpp -- Base class of all sequential and combinational
    processes

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)  
    Description of Modification: 
    - Implementation of operator() and operator, positional connection
    method.
    - Implementation of error checking in operator<<'s.
    - Implementation of the function test_module_prm.
    - Implementation of set_stack_size().
    - Added SC_INCLUDE_FX into end_module() to control the call of
      reset_curr_proc.

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <stdio.h>
#include <assert.h>
#include <cstring>

#include "sc_iostream.h"
#include "sc_macros_int.h"
#include "sc_bvrep.h"
#include "sc_bit.h"

#include "sc_lambda.h"
#include "sc_process.h"
#include "sc_module.h"
#include "sc_simcontext.h"
#include "sc_process_int.h"
#include "sc_port.h"
#include "sc_port_manager.h"
#include "sc_object_manager.h"
#include "sc_module_int.h"
#include "sc_module_name.h"
#include "sc_simcontext_int.h"


int sc_module::num_modules = 0;

/* This should be moved to sc_simcontext */
static sc_plist<sc_module*>&
get_list_of_modules()
{
    static sc_plist<sc_module*> list_of_modules;
    return list_of_modules;
}


class sc_module_dynalloc_list {
public:
    sc_module_dynalloc_list()
    {

    }
    ~sc_module_dynalloc_list();
    void add(sc_module* p)
    {
        m_list.push_back(p);
    }
private:
    sc_plist<sc_module*> m_list;
};

sc_module_dynalloc_list::~sc_module_dynalloc_list()
{
    sc_plist<sc_module*>::iterator it(m_list);
    while (! it.empty()) {
        delete *it;
        it++;
    }
}

sc_module*
sc_module_dynalloc(sc_module* m)
{
    static sc_module_dynalloc_list dynalloc_list;
    dynalloc_list.add(m);
    return m;
}



void
sc_module::sc_module_init()
{
    tid = num_modules++;
    end_module_called = false;
    simcontext()->hierarchy_push(this);
    handle = get_list_of_modules().push_back(this);
    port_vector = new sc_pvector<sc_port_b*>(16);
    port_index = 0;
}

sc_module::sc_module( const char* nm )
    : sc_object(nm),
      sensitive(this),
      sensitive_pos(this),
      sensitive_neg(this)
{
    sc_module_init();
}

/*
 *  This form of the constructor assumes that the user has
 *  used an sc_module_name parameter for his/her constructor.
 *  That parameter object has been pushed onto the stack,
 *  and can be looked up by calling the 
 *  top_of_module_name_stack() function of the object manager.
 *  This technique has two advantages:
 *
 *  1) The user no longer has to write sc_module(name) in the
 *     constructor initializer.
 *  2) The user no longer has to call end_module() at the end
 *     of the constructor -- a common negligence.
 *
 *  But it is important to note that sc_module_name may be used
 *  in the user's constructor's parameter.  If it is used anywhere
 *  else, unexpected things will happen.  The error-checking
 *  mechanism builtin here cannot hope to catch all misuses.
 *
 */
sc_module::sc_module()
    : sc_object(::sc_get_curr_simcontext()
                ->get_object_manager()
                ->top_of_module_name_stack()
                ->operator const char*()),
      sensitive(this),
      sensitive_pos(this),
      sensitive_neg(this)
{
    /* When this form is used, we better have a fresh sc_module_name
       on the top of the stack */
    sc_module_name* mod_name = 
        simcontext()->get_object_manager()->top_of_module_name_stack();
    if (0 == mod_name || 0 != mod_name->module)
        REPORT_ERROR(4003,"");
    mod_name->module = this;
    sc_module_init();
}

sc_module::sc_module(const sc_module_name& mn)
    : sc_object(::sc_get_curr_simcontext()
                ->get_object_manager()
                ->top_of_module_name_stack()
                ->operator const char*()),
      sensitive(this),
      sensitive_pos(this),
      sensitive_neg(this)
{
    /* For those used to the old style of passing a name to sc_module,
       this constructor will reduce the chance of making a mistake */

    /* When this form is used, we better have a fresh sc_module_name
       on the top of the stack */
    sc_module_name* mod_name = 
        simcontext()->get_object_manager()->top_of_module_name_stack();
    if (0 == mod_name || 0 != mod_name->module)
      REPORT_ERROR(4003,"");
    mod_name->module = this;
    sc_module_init();
}

sc_module::sc_module( const sc_string& s )
    : sc_object(s.operator const char*()),
      sensitive(this),
      sensitive_pos(this),
      sensitive_neg(this)
{
    sc_module_init();
}

sc_module::~sc_module()
{
    get_list_of_modules().remove(handle);
    delete port_vector;
}

void
sc_module::end_module()
{
    if (! end_module_called) {
        /* TBD: Can check here to alert the user that
           end_module was not called for a previous
           module. */
        (void)simcontext()->hierarchy_pop();
        // If the old syntax is used and the process constructor
        // contains watching(), then calling reset_curr_proc here
        // resets the curr_proc_info. On the other hand, watching()
        // needs the curr_proc_info. This means that the old syntax
        // should not be used with watching() or the following line
        // must be commented out. A temporary solution is: Since this
        // call is needed for FX, make this call be controlled by the
        // SC_INCLUDE_FX preprocessor directive.
#ifdef SC_INCLUDE_FX
	simcontext()->reset_curr_proc(); 
#endif
        end_module_called = true;
    }
}

bool
sc_module::validate() const
{
    return end_module_called;
}

void
sc_module::dump_modules()
{ // cannot be used with VCL
    sc_plist<sc_module*>::iterator it(get_list_of_modules());
    for ( ; ! it.empty(); it++) {
        sc_module* p = *it;
        const char* q = p->kind();
        cerr << p->id() << "\t" << q << "\t"
             << p->name() << endl;
    }
}

void
sc_module::validate_all()
{
    bool error = false;
    sc_plist<sc_module*>::iterator it(get_list_of_modules());
    for ( ; ! it.empty(); it++) {
        sc_module* p = *it;
        if (! p->validate()) {
          REPORT_WARNING(3004,sc_string::to_string("for %s ?",p->name()).c_str());
          if (error)
            REPORT_WARNING(3005,"");
          error = true;
        }
    }
}

const char* sc_module::kind_string = "sc_module";

const char*
sc_module::kind() const
{
    return kind_string;
}

void
sc_module::entry()
{
    assert( false );
}

void
sc_module::set_stack_size(size_t size)
{
  const sc_curr_proc_info* cpi = simcontext()->get_curr_proc_info();

  if (SC_CURR_PROC_SYNC == cpi->kind) {
    sc_sync_process_handle handle_ = (sc_sync_process_handle) cpi->process_handle;
    handle_->set_stack_size(size);
  }
  else if (SC_CURR_PROC_APROC == cpi->kind) {
    sc_aproc_process_handle handle_ = (sc_aproc_process_handle) cpi->process_handle;
    handle_->set_stack_size(size);
  }
  else
    REPORT_WARNING(3006,"");
}

int
sc_module::append_port( sc_port_b* port )
{
    int ordinal = port_vector->size();
    port_vector->push_back(port);
    return ordinal;
}

sc_module&
sc_module::operator<<(const sc_signal_base& s)
{
    if (port_vector->size() <= port_index) {
      REPORT_WARNING(4004,sc_string::to_string("%s, to module %s."
                          " The extra is ignored",s.name(),name()).c_str());
      return *this;
    }

    BIND_STATUS bs = (*port_vector)[port_index]->bind_const(s);
    if (BIND_STATUS_OK != bs) {
        sc_port_errmsg(bs, 1 + port_index, name());
        simcontext()->get_port_manager()->flag_error();
    }
    ++port_index;
    return *this;
}

sc_module&
sc_module::operator<<(sc_signal_base& s)
{
    if (port_vector->size() <= port_index) {
      REPORT_WARNING(4004,sc_string::to_string("%s, to module %s."
                          " The extra is ignored",s.name(),name()).c_str());
      return *this;
    }

    BIND_STATUS bs = (*port_vector)[port_index]->vbind(s);
    if (BIND_STATUS_OK != bs) {
        sc_port_errmsg(bs, 1 + port_index, name());
        simcontext()->get_port_manager()->flag_error();
    }
    ++port_index;
    return *this;
}

sc_module&
sc_module::operator<<(const sc_port_b& p)
{
    if (port_vector->size() <= port_index) {
      REPORT_WARNING(4005,sc_string::to_string("to module %s."
                          " The extra is ignored",name()).c_str());
      return *this;
    }

    BIND_STATUS bs = (*port_vector)[port_index]->vbind(p);
    if (BIND_STATUS_OK != bs) {
        sc_port_errmsg(bs, 1 + port_index, name());
        simcontext()->get_port_manager()->flag_error();
    }
    ++port_index;
    return *this;
}

void
sc_module_helpers::end_module(sc_module* mod)
{
    mod->end_module();
}

// Checks to see if pxxx is a port or a signal. In either case, their
// binding is done. Returns true if pxxx is null, indicating the end
// of the argument list.
bool
sc_module_helpers::test_module_prm(sc_module* mod, sc_module_prm& pxxx)
{
  // If pxxx is a port or a signal, i.e., not null - the default.
  if (&pxxx != &SC_MODULE_PRM_NULL) {
    // Check to see if pxxx is a port.
    sc_port_b *port = &(reinterpret_cast<sc_port_b&>(pxxx)); 

    if (mod->sc_get_curr_simcontext()->get_port_manager()->port_exists(port)) 
      (void) mod->operator<<(*port); 
    else // If not a port, it must then be a signal.
      (void) mod->operator<<(reinterpret_cast<sc_signal_base&>(pxxx)); 

    return false; // There may be more arguments.
  } 

  return true; // No more arguments, exit.
}

// Performs the positional connection method for modules. If the
// module has more than 64 ports for binding, this function can be
// called more than once or this function can be made to work with
// more arguments. The latter can easily be done in sc_module.h and
// this file.
void 
sc_module::operator()(sc_module_prm& p001,
                      sc_module_prm& p002,
                      sc_module_prm& p003,
                      sc_module_prm& p004,
                      sc_module_prm& p005,
                      sc_module_prm& p006,
                      sc_module_prm& p007,
                      sc_module_prm& p008,
                      sc_module_prm& p009,
                      sc_module_prm& p010,
                      sc_module_prm& p011,
                      sc_module_prm& p012,
                      sc_module_prm& p013,
                      sc_module_prm& p014,
                      sc_module_prm& p015,
                      sc_module_prm& p016,
                      sc_module_prm& p017,
                      sc_module_prm& p018,
                      sc_module_prm& p019,
                      sc_module_prm& p020,
                      sc_module_prm& p021,
                      sc_module_prm& p022,
                      sc_module_prm& p023,
                      sc_module_prm& p024,
                      sc_module_prm& p025,
                      sc_module_prm& p026,
                      sc_module_prm& p027,
                      sc_module_prm& p028,
                      sc_module_prm& p029,
                      sc_module_prm& p030,
                      sc_module_prm& p031,
                      sc_module_prm& p032,
                      sc_module_prm& p033,
                      sc_module_prm& p034,
                      sc_module_prm& p035,
                      sc_module_prm& p036,
                      sc_module_prm& p037,
                      sc_module_prm& p038,
                      sc_module_prm& p039,
                      sc_module_prm& p040,
                      sc_module_prm& p041,
                      sc_module_prm& p042,
                      sc_module_prm& p043,
                      sc_module_prm& p044,
                      sc_module_prm& p045,
                      sc_module_prm& p046,
                      sc_module_prm& p047,
                      sc_module_prm& p048,
                      sc_module_prm& p049,
                      sc_module_prm& p050,
                      sc_module_prm& p051,
                      sc_module_prm& p052,
                      sc_module_prm& p053,
                      sc_module_prm& p054,
                      sc_module_prm& p055,
                      sc_module_prm& p056,
                      sc_module_prm& p057,
                      sc_module_prm& p058,
                      sc_module_prm& p059,
                      sc_module_prm& p060,
                      sc_module_prm& p061,
                      sc_module_prm& p062,
                      sc_module_prm& p063,
                      sc_module_prm& p064)
{
  if (sc_module_helpers::test_module_prm(this, p001)) return;
  if (sc_module_helpers::test_module_prm(this, p002)) return;
  if (sc_module_helpers::test_module_prm(this, p003)) return;
  if (sc_module_helpers::test_module_prm(this, p004)) return;
  if (sc_module_helpers::test_module_prm(this, p005)) return;
  if (sc_module_helpers::test_module_prm(this, p006)) return;
  if (sc_module_helpers::test_module_prm(this, p007)) return;
  if (sc_module_helpers::test_module_prm(this, p008)) return;
  if (sc_module_helpers::test_module_prm(this, p009)) return;
  if (sc_module_helpers::test_module_prm(this, p010)) return;
  if (sc_module_helpers::test_module_prm(this, p011)) return;
  if (sc_module_helpers::test_module_prm(this, p012)) return;
  if (sc_module_helpers::test_module_prm(this, p013)) return;
  if (sc_module_helpers::test_module_prm(this, p014)) return;
  if (sc_module_helpers::test_module_prm(this, p015)) return;
  if (sc_module_helpers::test_module_prm(this, p016)) return;
  if (sc_module_helpers::test_module_prm(this, p017)) return;
  if (sc_module_helpers::test_module_prm(this, p018)) return;
  if (sc_module_helpers::test_module_prm(this, p019)) return;
  if (sc_module_helpers::test_module_prm(this, p020)) return;
  if (sc_module_helpers::test_module_prm(this, p021)) return;
  if (sc_module_helpers::test_module_prm(this, p022)) return;
  if (sc_module_helpers::test_module_prm(this, p023)) return;
  if (sc_module_helpers::test_module_prm(this, p024)) return;
  if (sc_module_helpers::test_module_prm(this, p025)) return;
  if (sc_module_helpers::test_module_prm(this, p026)) return;
  if (sc_module_helpers::test_module_prm(this, p027)) return;
  if (sc_module_helpers::test_module_prm(this, p028)) return;
  if (sc_module_helpers::test_module_prm(this, p029)) return;
  if (sc_module_helpers::test_module_prm(this, p030)) return;
  if (sc_module_helpers::test_module_prm(this, p031)) return;
  if (sc_module_helpers::test_module_prm(this, p032)) return;
  if (sc_module_helpers::test_module_prm(this, p033)) return;
  if (sc_module_helpers::test_module_prm(this, p034)) return;
  if (sc_module_helpers::test_module_prm(this, p035)) return;
  if (sc_module_helpers::test_module_prm(this, p036)) return;
  if (sc_module_helpers::test_module_prm(this, p037)) return;
  if (sc_module_helpers::test_module_prm(this, p038)) return;
  if (sc_module_helpers::test_module_prm(this, p039)) return;
  if (sc_module_helpers::test_module_prm(this, p040)) return;
  if (sc_module_helpers::test_module_prm(this, p041)) return;
  if (sc_module_helpers::test_module_prm(this, p042)) return;
  if (sc_module_helpers::test_module_prm(this, p043)) return;
  if (sc_module_helpers::test_module_prm(this, p044)) return;
  if (sc_module_helpers::test_module_prm(this, p045)) return;
  if (sc_module_helpers::test_module_prm(this, p046)) return;
  if (sc_module_helpers::test_module_prm(this, p047)) return;
  if (sc_module_helpers::test_module_prm(this, p048)) return;
  if (sc_module_helpers::test_module_prm(this, p049)) return;
  if (sc_module_helpers::test_module_prm(this, p050)) return;
  if (sc_module_helpers::test_module_prm(this, p051)) return;
  if (sc_module_helpers::test_module_prm(this, p052)) return;
  if (sc_module_helpers::test_module_prm(this, p053)) return;
  if (sc_module_helpers::test_module_prm(this, p054)) return;
  if (sc_module_helpers::test_module_prm(this, p055)) return;
  if (sc_module_helpers::test_module_prm(this, p056)) return;
  if (sc_module_helpers::test_module_prm(this, p057)) return;
  if (sc_module_helpers::test_module_prm(this, p058)) return;
  if (sc_module_helpers::test_module_prm(this, p059)) return;
  if (sc_module_helpers::test_module_prm(this, p060)) return;
  if (sc_module_helpers::test_module_prm(this, p061)) return;
  if (sc_module_helpers::test_module_prm(this, p062)) return;
  if (sc_module_helpers::test_module_prm(this, p063)) return;
  if (sc_module_helpers::test_module_prm(this, p064)) return;
}

// End of file
