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

    sc_module.h -- Base class of all sequential and combinational
                   processes, as well as hierarchical modules

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

    Modifier Name & Affiliation: Gene Bushuyev. Synopsys, Inc.(gbush@synopsys.com)
    Description of Modification:
	- change implementation for VC6.
    
    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/


#ifndef SC_MODULE_H
#define SC_MODULE_H

#include "sc_string.h"
#include "sc_list.h"
#include "sc_object.h"
#include "sc_sensitive.h"
#include "sc_wait.h"
#include "sc_module_prm.h"
#include "sc_process.h"

class sc_lambda_ptr;
class sc_signal_edgy_deval;
class sc_port_b;

class sc_module : public sc_object {
    friend class sc_clock;
    friend class sc_clock_edge;
    friend class sc_simcontext;
    friend class sc_port_manager;
    friend class sc_module_helpers;

public:
        // Returns the unique id associated with the process
    int id() const { return tid; }

    bool validate() const;

    static void dump_modules();
    static void validate_all();

    sc_simcontext* sc_get_curr_simcontext() { return simcontext(); }

    static const char* kind_string;
    virtual const char* kind() const;

	
protected:
    void sc_module_init();

        // constructor
    sc_module( const char* nm );
    sc_module( const sc_string& nm );
    sc_module( const sc_module_name& nm ); /* for those used to old style */
    sc_module();

public:
    virtual void entry();
        // destructor
    virtual ~sc_module();

    sc_module& operator<<(const sc_signal_base&);
    sc_module& operator<<(sc_signal_base&);
    sc_module& operator<<(const sc_port_b&);

    sc_module& operator,(const sc_signal_base& s)
    {
      return operator<<(s);
    }

    sc_module& operator,(sc_signal_base& s)
    {
      return operator<<(s);
    }

    sc_module& operator,(const sc_port_b& p)
    {
      return operator<<(p);
    }

    // operator() is declared at the end of the class.

protected:
        // this must be called by user-defined modules
    void end_module();

    void halt()      { ::halt(simcontext());    }
    void wait()      { ::wait(simcontext());    }
    void wait(int n) { ::wait(n, simcontext()); }
    void wait_until(const sc_lambda_ptr& l)        { ::wait_until(l, simcontext()); }
    void wait_until(const sc_signal_edgy_deval& s) { ::wait_until(s, simcontext()); }
    void watching(const sc_lambda_ptr& l)          { ::watching(l, simcontext());   }
    void watching(const sc_signal_edgy_deval& s)   { ::watching(s, simcontext());   }
    void at_posedge(const sc_signal<sc_logic>& s)  { ::at_posedge(s, simcontext()); }
    void at_posedge(const sc_signal_edgy&  s)      { ::at_posedge(s, simcontext()); }
    void at_negedge(const sc_signal<sc_logic>& s)  { ::at_negedge(s, simcontext()); }
    void at_negedge(const sc_signal_edgy& s)       { ::at_negedge(s, simcontext()); }
    
    // These are protected so that user derived classes can refer to them.
    sc_sensitive     sensitive;
    sc_sensitive_pos sensitive_pos;
    sc_sensitive_neg sensitive_neg;

    // Functions to set the stack size of the current synchronous or
    // asynchronous thread process.
    void set_stack_size(size_t size);

    int append_port(sc_port_b*);

private:
    static int num_modules;     // number of modules instantiated

    bool end_module_called;
    int tid;                    // id of the module
    sc_plist<sc_module*>::handle_t handle;
    sc_pvector<sc_port_b*>* port_vector;
    int port_index;

public:
    void defunct() { }

    void operator()(sc_module_prm& p001,
                    sc_module_prm& p002 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p003 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p004 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p005 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p006 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p007 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p008 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p009 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p010 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p011 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p012 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p013 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p014 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p015 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p016 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p017 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p018 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p019 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p020 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p021 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p022 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p023 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p024 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p025 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p026 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p027 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p028 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p029 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p030 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p031 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p032 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p033 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p034 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p035 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p036 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p037 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p038 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p039 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p040 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p041 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p042 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p043 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p044 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p045 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p046 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p047 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p048 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p049 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p050 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p051 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p052 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p053 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p054 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p055 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p056 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p057 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p058 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p059 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p060 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p061 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p062 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p063 = SC_MODULE_PRM_NULL,
                    sc_module_prm& p064 = SC_MODULE_PRM_NULL);
};

extern sc_module* sc_module_dynalloc(sc_module*);
#define SC_NEW(x)  sc_module_dynalloc(new x);

typedef sc_module sc_process_base;

/* Note that clock_edge here may be an sc_clock_edge& or
   an sc_polarity; the latter is generated when we invoke
   pos() or neg() on an sc_in_clk or an sc_inout_clk */

#define SC_MANGLE_NAME_STRUCT(ptype, func) \
    sc_ ## ptype ## func ## _struct
#define SC_MANGLE_NAME_HELPER(ptype, func) \
    sc_ ## ptype ## func ## _helper

// this must be undefined for VC6 because it messes up stack when
// member functions casts are performed
#ifndef SC_USE_MEMBER_FUNC_PTR

#ifndef SC_USER_DEFINED_MAX_NUMBER_OF_PROCESSES
// the value below defines max number of processes per module
#define SC_VC6_MAX_NUMBER_OF_PROCESSES 10
#endif

// the following code is designed to circumvent the following VC6 bugs:
// 1) violation of 9.8/1 about type names lookup
// 2) the linkage of the local class static f-n is made external
// 3) lack of partial specialization
// 4) member functions casts screw up the stack
// this helper class has two sets of pointers: one for member functions
// and the other for static functions. For each member function a static
// function is created that performs conversion and calls member function.

template<class T>
struct sc_vc6_process_helper_class
{
	int counter;
	
    // array of member functions
    typedef void (T::*mf_type)();
	mf_type mfa[SC_VC6_MAX_NUMBER_OF_PROCESSES];
    typedef void (*sf_type)(sc_module* b);
	// array of static member functions
	sf_type sfa[SC_VC6_MAX_NUMBER_OF_PROCESSES];
	// VC6 lacks partial specialization support
	// using explicit specialization instead
    template<int W>
	struct process_helper
	{
		static void smf(sc_module* b)
		{ (static_cast<T*>(b)->*(static_cast<T*>(b)->sc_vc6_process_helper.mfa)[W-1])();}
		// recursively generate other functions
		process_helper<W-1> hlp;
		process_helper(sf_type* sfa_):hlp(sfa_)
		{ sfa_[W-1] = smf; }
	};
	template<>
	struct process_helper<0>
	{
		process_helper<0>(sf_type* sfa){};
	};
    process_helper<SC_VC6_MAX_NUMBER_OF_PROCESSES> helper;
	sc_vc6_process_helper_class():helper(sfa){counter = -1;}
    sc_vc6_process_helper_class& operator = (const sc_vc6_process_helper_class&);
	sc_vc6_process_helper_class& operator = (mf_type f) 
	{
	  counter++;
	  if(counter>=SC_VC6_MAX_NUMBER_OF_PROCESSES)
		 REPORT_ERROR(3002,sc_string::to_string("%d( max %d)\n"
		       "you can increase this number. For example, to set it to 20 do this\n"
		       "before including ""systemc.h"" in your code include these two lines:\n"
                       "#define SC_USER_DEFINED_MAX_NUMBER_OF_PROCESSES\n"
		       "#define SC_VC6_MAX_NUMBER_OF_PROCESSES 20\n",
                       counter+1, SC_VC6_MAX_NUMBER_OF_PROCESSES).c_str());
	  mfa[counter]=f;
	  return *this;
	}
	operator sf_type () 
	{
		if(counter<0)
                  REPORT_ERROR(3001,"Reading before initialization.");
		return sfa[counter];
	}
};
   

#define SC_DECL_HELPER_STRUCT(ptype, module_tag, func) \
    sc_vc6_process_helper = &module_tag::func

#define SC_MAKE_FUNC_PTR(ptype, module_tag, func) sc_vc6_process_helper
#else
#define SC_DECL_HELPER_STRUCT(ptype, module_tag, func) /*EMPTY*/

/* Converting `void (module_tag::*)()' to `void (sc_module::*)()' is OK as
   long as the dynamic type is correct.  C++ Standard 5.4 "Explicit type
   conversion", clause 7: a pointer to member of derived class type may be
   explicitly converted to a pointer to member of an unambiguous non-virtual
   base class type. */
#define SC_MAKE_FUNC_PTR(ptype, module_tag, func) \
    static_cast<SC_ENTRY_FUNC>(&module_tag::func)
#endif

#define declare_sc_sync_process(handle, name, clock_edge, module_tag, func) \
    sc_sync_process_handle handle; \
    {   \
        SC_DECL_HELPER_STRUCT(sync, module_tag, func); \
        handle = simcontext()->register_sc_sync_process(name, \
                     SC_MAKE_FUNC_PTR(sync, module_tag, func), \
                     this, clock_edge); \
    }

#define declare_sc_async_process(handle, name, module_tag, func) \
    sc_async_process_handle handle; \
    {   \
        SC_DECL_HELPER_STRUCT(async, module_tag, func); \
        handle = simcontext()->register_sc_async_process(name, \
                     SC_MAKE_FUNC_PTR(async, module_tag, func), \
                     this); \
        sc_module::sensitive << handle; \
        sc_module::sensitive_pos << handle; \
        sc_module::sensitive_neg << handle; \
    }

#define declare_sc_aproc_process(handle, name, module_tag, func) \
    sc_aproc_process_handle handle; \
    {   \
        SC_DECL_HELPER_STRUCT(aproc, module_tag, func); \
        handle = simcontext()->register_sc_aproc_process(name, \
                     SC_MAKE_FUNC_PTR(aproc, module_tag, func), \
                     this); \
        sc_module::sensitive << handle; \
        sc_module::sensitive_pos << handle; \
        sc_module::sensitive_neg << handle; \
    }


#define sc_sync_tprocess(handle, string_name, class_name, func, clock_edge) \
    declare_sc_sync_process(handle, string_name, clock_edge, class_name, func)

#define sc_async_tprocess(handle, string_name, class_name, func) \
    declare_sc_aproc_process(handle, string_name, class_name, func)

#define sc_async_fprocess(handle, string_name, class_name, func) \
    declare_sc_async_process(handle, string_name, class_name, func)

#endif

