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

  sc_module.h -- Base class of all hierarchical modules and channels.

  Original Author: Stan Y. Liao, Synopsys, Inc.
                   Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Ali Dasdan, Synopsys, Inc.
  Description of Modification: - Implementation of operator() and operator,
                                 positional connection method.
                               - Implementation of error checking in
                                 operator<<'s.
                               - Implementation of the function test_module_prm.
                               - Implementation of set_stack_size().

      Name, Affiliation, Date: Gene Bushuyev, Synopsys, Inc.
  Description of Modification: - Change implementation for VC6.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_MODULE_H
#define SC_MODULE_H


#include "systemc/kernel/sc_kernel_ids.h"
#include "systemc/kernel/sc_lambda.h"
#include "systemc/kernel/sc_module_name.h"
#include "systemc/kernel/sc_object.h"
#include "systemc/kernel/sc_process.h"
#include "systemc/kernel/sc_sensitive.h"
#include "systemc/kernel/sc_time.h"
#include "systemc/kernel/sc_wait.h"
#include "systemc/kernel/sc_wait_cthread.h"
#include "systemc/utils/sc_list.h"
#include "systemc/utils/sc_string.h"

class sc_name_gen;


// ----------------------------------------------------------------------------
//  STRUCT : sc_bind_proxy
//
//  Struct for temporarily storing a pointer to an interface or port.
//  Used for positional binding.
// ----------------------------------------------------------------------------

struct sc_bind_proxy
{
    sc_interface* iface;
    sc_port_base* port;
    
    sc_bind_proxy();
    sc_bind_proxy( sc_interface& );
    sc_bind_proxy( sc_port_base& );
};


extern const sc_bind_proxy SC_BIND_PROXY_NIL;


// ----------------------------------------------------------------------------
//  CLASS : sc_module
//
//  Base class for all structural entities.
// ----------------------------------------------------------------------------

class sc_module
: public sc_object
{
    friend class sc_module_name;
    friend class sc_module_registry;
    friend class sc_object;
    friend class sc_port_registry;
    friend class sc_simcontext;

public:

    sc_simcontext* sc_get_curr_simcontext()
	{ return simcontext(); }

    // to generate unique names for objects in an MT-Safe way
    const char* gen_unique_name( const char* basename_ );

    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    // called by elaboration_done (does nothing by default)
    virtual void end_of_elaboration();

    void elaboration_done( bool& );

    void sc_module_init();

    // constructor
    sc_module( const char* nm );
    sc_module( const sc_string& nm );
    sc_module( const sc_module_name& nm ); /* for those used to old style */
    sc_module();

public:

    // destructor
    virtual ~sc_module();

    // positional binding methods

    sc_module& operator << ( sc_interface& );
    sc_module& operator << ( sc_port_base& );

    sc_module& operator , ( sc_interface& interface_ )
        { return operator << ( interface_ ); }

    sc_module& operator , ( sc_port_base& port_ )
        { return operator << ( port_ ); }

    // operator() is declared at the end of the class.

    const sc_pvector<sc_object*>& get_child_objects() const;

protected:

    void add_child_object( sc_object* );
    void remove_child_object( sc_object* );

    // this must be called by user-defined modules
    void end_module();


    // to prevent initialization for SC_METHODs and SC_THREADs
    void dont_initialize();


    // static sensitivity for SC_THREADs and SC_CTHREADs

    void wait()
        { ::wait( simcontext() ); }


    // dynamic sensitivity for SC_THREADs and SC_CTHREADs

    void wait( const sc_event& e )
        { ::wait( e, simcontext() ); }

    void wait( sc_event_or_list& el )
	{ ::wait( el, simcontext() ); }

    void wait( sc_event_and_list& el )
	{ ::wait( el, simcontext() ); }

    void wait( const sc_time& t )
        { ::wait( t, simcontext() ); }

    void wait( double v, sc_time_unit tu )
        { ::wait( sc_time( v, tu, simcontext() ), simcontext() ); }

    void wait( const sc_time& t, const sc_event& e )
        { ::wait( t, e, simcontext() ); }

    void wait( double v, sc_time_unit tu, const sc_event& e )
        { ::wait( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void wait( const sc_time& t, sc_event_or_list& el )
        { ::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_or_list& el )
        { ::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void wait( const sc_time& t, sc_event_and_list& el )
        { ::wait( t, el, simcontext() ); }

    void wait( double v, sc_time_unit tu, sc_event_and_list& el )
        { ::wait( sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // static sensitivity for SC_METHODs

    void next_trigger()
	{ ::next_trigger( simcontext() ); }


    // dynamic sensitivty for SC_METHODs

    void next_trigger( const sc_event& e )
        { ::next_trigger( e, simcontext() ); }

    void next_trigger( sc_event_or_list& el )
        { ::next_trigger( el, simcontext() ); }

    void next_trigger( sc_event_and_list& el )
        { ::next_trigger( el, simcontext() ); }

    void next_trigger( const sc_time& t )
        { ::next_trigger( t, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu )
        { ::next_trigger( sc_time( v, tu, simcontext() ), simcontext() ); }

    void next_trigger( const sc_time& t, const sc_event& e )
        { ::next_trigger( t, e, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, const sc_event& e )
        { ::next_trigger( sc_time( v, tu, simcontext() ), e, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_or_list& el )
        { ::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_or_list& el )
        { ::next_trigger( sc_time( v, tu, simcontext() ), el, simcontext() ); }

    void next_trigger( const sc_time& t, sc_event_and_list& el )
        { ::next_trigger( t, el, simcontext() ); }

    void next_trigger( double v, sc_time_unit tu, sc_event_and_list& el )
        { ::next_trigger( sc_time( v, tu, simcontext() ), el, simcontext() ); }


    // for SC_METHODs and SC_THREADs and SC_CTHREADs

    bool timed_out()
	{ return ::timed_out( simcontext() ); }


    // for SC_CTHREADs

    void halt()
        { ::halt( simcontext() ); }

    void wait( int n )
        { ::wait( n, simcontext() ); }

    void wait_until( const sc_lambda_ptr& l )
        { ::wait_until( l, simcontext() ); }

    void wait_until( const sc_signal_bool_deval& s )
        { ::wait_until( s, simcontext() ); }

    void at_posedge( const sc_signal_in_if<bool>& s )
	{ ::at_posedge( s, simcontext() ); }

    void at_posedge( const sc_signal_in_if<sc_logic>& s )
	{ ::at_posedge( s, simcontext() ); }

    void at_negedge( const sc_signal_in_if<bool>& s )
	{ ::at_negedge( s, simcontext() ); }

    void at_negedge( const sc_signal_in_if<sc_logic>& s )
	{ ::at_negedge( s, simcontext() ); }

    void watching( const sc_lambda_ptr& l )
        { ::watching( l, simcontext() ); }

    void watching( const sc_signal_bool_deval& s )
        { ::watching( s, simcontext() ); }


    // These are protected so that user derived classes can refer to them.
    sc_sensitive     sensitive;
    sc_sensitive_pos sensitive_pos;
    sc_sensitive_neg sensitive_neg;

    // Function to set the stack size of the current (c)thread process.
    void set_stack_size( size_t );

    int append_port( sc_port_base* );

private:

    bool                       m_end_module_called;
    sc_pvector<sc_port_base*>* m_port_vec;
    int                        m_port_index;
    sc_name_gen*               m_name_gen;
    sc_pvector<sc_object*>     m_child_objects;

public:

    void defunct() { }

    // positional binding methods (cont'd)

    void operator () ( const sc_bind_proxy& p001,
		       const sc_bind_proxy& p002 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p003 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p004 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p005 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p006 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p007 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p008 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p009 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p010 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p011 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p012 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p013 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p014 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p015 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p016 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p017 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p018 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p019 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p020 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p021 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p022 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p023 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p024 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p025 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p026 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p027 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p028 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p029 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p030 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p031 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p032 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p033 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p034 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p035 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p036 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p037 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p038 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p039 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p040 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p041 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p042 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p043 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p044 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p045 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p046 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p047 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p048 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p049 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p050 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p051 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p052 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p053 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p054 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p055 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p056 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p057 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p058 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p059 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p060 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p061 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p062 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p063 = SC_BIND_PROXY_NIL,
		       const sc_bind_proxy& p064 = SC_BIND_PROXY_NIL );
};


extern sc_module* sc_module_dynalloc(sc_module*);
#define SC_NEW(x)  sc_module_dynalloc(new x);


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
		 SC_REPORT_ERROR( SC_ID_VC6_MAX_PROCESSES_EXCEEDED_,
		       sc_string::to_string("%d( max %d)\n"
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
                  SC_REPORT_ERROR( SC_ID_VC6_PROCESS_HELPER_,
				   "Reading before initialization.");
		return sfa[counter];
	}
};
   

#define SC_DECL_HELPER_STRUCT(module_tag, func) \
    sc_vc6_process_helper = &module_tag::func

#define SC_MAKE_FUNC_PTR(module_tag, func) sc_vc6_process_helper
#else
#define SC_DECL_HELPER_STRUCT(module_tag, func) /*EMPTY*/

/* Converting `void (module_tag::*)()' to `void (sc_module::*)()' is OK as
   long as the dynamic type is correct.  C++ Standard 5.4 "Explicit type
   conversion", clause 7: a pointer to member of derived class type may be
   explicitly converted to a pointer to member of an unambiguous non-virtual
   base class type. */
#define SC_MAKE_FUNC_PTR(module_tag, func) \
    static_cast<SC_ENTRY_FUNC>(&module_tag::func)
#endif


// To further simplify the syntax, consider the following.

#define SC_MODULE(user_module_name)                                           \
    struct user_module_name : sc_module

#ifdef SC_USE_MEMBER_FUNC_PTR

#define SC_CTOR(user_module_name)                                             \
    typedef user_module_name SC_CURRENT_USER_MODULE;                          \
    user_module_name( sc_module_name )

// the SC_HAS_PROCESS macro call must be followed by a ;
#define SC_HAS_PROCESS(user_module_name)                                      \
    typedef user_module_name SC_CURRENT_USER_MODULE

#else

#define SC_CTOR(user_module_name)                                             \
    typedef user_module_name SC_CURRENT_USER_MODULE;                          \
    sc_vc6_process_helper_class<user_module_name> sc_vc6_process_helper;      \
    user_module_name( sc_module_name )

// the SC_HAS_PROCESS macro call must be followed by a ;
#define SC_HAS_PROCESS(user_module_name)                                      \
    typedef user_module_name SC_CURRENT_USER_MODULE;                          \
    sc_vc6_process_helper_class<user_module_name> sc_vc6_process_helper

#endif


#define declare_thread_process(handle, name, module_tag, func)                \
    sc_thread_handle handle;                                                  \
    {                                                                         \
        SC_DECL_HELPER_STRUCT( module_tag, func );                            \
        handle = simcontext()->register_thread_process( name,                 \
                     SC_MAKE_FUNC_PTR( module_tag, func ), this );            \
        sc_module::sensitive << handle;                                       \
        sc_module::sensitive_pos << handle;                                   \
        sc_module::sensitive_neg << handle;                                   \
    }

#define declare_method_process(handle, name, module_tag, func)                \
    sc_method_handle handle;                                                  \
    {                                                                         \
        SC_DECL_HELPER_STRUCT( module_tag, func );                            \
        handle = simcontext()->register_method_process( name,                 \
                     SC_MAKE_FUNC_PTR( module_tag, func ), this );            \
        sc_module::sensitive << handle;                                       \
        sc_module::sensitive_pos << handle;                                   \
        sc_module::sensitive_neg << handle;                                   \
    }

#define declare_cthread_process(handle, name, module_tag, func, edge)         \
    sc_cthread_handle handle;                                                 \
    {                                                                         \
        SC_DECL_HELPER_STRUCT( module_tag, func );                            \
        handle = simcontext()->register_cthread_process( name,                \
                     SC_MAKE_FUNC_PTR( module_tag, func ), this );            \
        sc_module::sensitive.operator() ( handle, edge );                     \
    }


#define SC_THREAD(func)                                                       \
    declare_thread_process( func ## _handle,                                  \
                            #func,                                            \
                            SC_CURRENT_USER_MODULE,                           \
                            func )

#define SC_METHOD(func)                                                       \
    declare_method_process( func ## _handle,                                  \
                            #func,                                            \
                            SC_CURRENT_USER_MODULE,                           \
                            func )

#define SC_CTHREAD(func, edge)                                                \
    declare_cthread_process( func ## _handle,                                 \
                             #func,                                           \
                             SC_CURRENT_USER_MODULE,                          \
                             func,                                            \
                             edge )


// ----------------------------------------------------------------------------
//  TYPEDEFS
// ----------------------------------------------------------------------------

typedef sc_module sc_channel;
typedef sc_module sc_behavior;


#endif
