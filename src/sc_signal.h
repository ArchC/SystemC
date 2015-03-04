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

    sc_signal.h - Signals of any scalar type

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com) 
    Description of Modification: Implementation of operator
    sc_module_prm for positional connection method.

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_SIGNAL_H
#define SC_SIGNAL_H

#include "sc_logic.h"
#include "sc_object.h"
#include "sc_simcontext.h"
#include "sc_vector.h"
#include "sc_trace.h"
#include "sc_module_prm.h"

class sc_clock;
class sc_sync;
class sc_async;
class sc_aproc;
class sc_module;

#define SC_SIGNAL_PROP_NONE      0
#define SC_SIGNAL_PROP_ASYNC     (1U<<0)
#define SC_SIGNAL_PROP_APROC     (1U<<1)
#define SC_SIGNAL_PROP_ASYNC_NEG (1U<<2)
#define SC_SIGNAL_PROP_APROC_NEG (1U<<3)

namespace sc_bv_ns
{
	class sc_bv_base;
	class sc_lv_base;
}
//
// Base class for all types of signals.
//
// A signal is an entity that has three values: an old value, 
// a present value  and a future value. When one writes 
// to a signal, one writes to its future value; the present value will 
// become updated when the method update() is invoked; and the 
// old value is recorded when the method update_old() is invoked.
//
class sc_signal_base : public sc_object
{
    friend class sc_aproc_process;
    friend class sc_simcontext;
    friend class sc_signal_optimize;

public:
    typedef void (*UPDATE_FUNC)(sc_signal_base*);

    operator sc_module_prm&()
    {
      return reinterpret_cast<sc_module_prm&>(*this);
    }

    // Updates the current value of the signal to its new value
    virtual void update() = 0;
    virtual void update_old() = 0;

    static const char* kind_string;
    virtual const char* kind() const;

    // void print(ostream& os) const; -- Use sc_object::print(os)
    void dump(ostream& os) const;

    /* Virtual because subtypes may have different handling - e.g.,
       separation of posedge and negedge for sc_signal_edgy. */
    virtual void decl_sensitive_async( sc_async_process_handle async ) const;
    virtual void decl_sensitive_aproc( sc_aproc_process_handle aproc ) const;
    virtual void remove_sensitive_aproc(sc_aproc_process_handle aproc) const;
#if defined(__BCPLUSPLUS__)
#pragma warn -inl
#endif
    virtual bool event()const
    {
      REPORT_ERROR(5001,"");
#if defined(__HP_aCC)
      // some compilers are not very smart
      return false;
#endif
    }
#if defined(__BCPLUSPLUS__)
#pragma warn .inl
#endif
protected:
    virtual UPDATE_FUNC* update_funcs_array() const = 0;
    UPDATE_FUNC update_func;
    unsigned prop_which; /* 0 = none, 1 = asyncs, 2 = aprocs, 3 = both */

        // Constructors
    sc_signal_base( sc_object* from, bool add_to_simcontext = true );
    sc_signal_base( const char* nm, sc_object* from, bool add_to_simcontext = true );

    void add_dependent_procs_to_queue_default();
    void prop_asyncs_only();
    void prop_aprocs_only();
    void prop_asyncs_aprocs();

public:
        // Virtual destructor - does nothing
    virtual ~sc_signal_base();

protected:
    bool submitted;
    //bool is_submitted()        const { return submitted; }
    //void set_submitted(bool s) { submitted = s; }

    void submit_update()
    {
        if (! submitted) {
            simcontext()->submit_update(this);
            submitted=true;
        }
    }

    sc_object* created_by;

    mutable sc_pvector<sc_async_process_handle> sensitive_asyncs;
    mutable sc_pvector<sc_aproc_process_handle> sensitive_aprocs;

private:
        // Prevent use of copy constructor.  This constructor has no
        // implementation.
    sc_signal_base( const sc_signal_base& );
    void operator=( const sc_signal_base& );
};

/*---------------------------------------------------------------------------*/

//
// Signal of a scalar type 
//
// sc_signal<T> is used to create a signal-type of underlying
// type T.  Here, T must be a scalar.  If a signal array is desired,
// use sc_signal_array with an sc_array as argument.
//

template< class T > class sc_signal : public sc_signal_base
{
    friend class sc_clock;
    friend class sc_clock_edge;
public:
        // Constructors
    sc_signal();
    explicit sc_signal(char init_value);
    explicit sc_signal(const char* nm);
    sc_signal( sc_object* created_by );

private:
        // Prevent use of copy constructor; this constructor has no
        // implementation
    sc_signal( const sc_signal<T>& );
    void init();

public:
        // Destructor
    virtual ~sc_signal();

        // Retrieves the current value of the signal
        // <group>
    const T& read() const;
    operator const T&() const;
    T* get_data_ptr();
    const T* get_data_ptr() const;

    const T& get_new_value() const { return new_value; }
    const T& get_old_value() const { return old_value; }
        // </group>

    virtual sc_signal<T>& write( const T& d );
    virtual sc_signal<T>& operator=( const T& d );
    virtual sc_signal<T>& operator=( const sc_signal<T>& d ) { return operator=(d.read()); }
// remove this conditional when VC++ fixes these bugs
#if !defined(_MSC_VER) && !defined(__SUNPRO_CC)
    template<class X>  sc_signal<T>& operator=(const X&);
    template<class X>  sc_signal<T>& write(const X&);
#else
  //	sc_signal<T>& operator=(const sc_bv_ns::sc_bv_base&);
  //    sc_signal<T>& write(const sc_bv_ns::sc_bv_base&);
  //	sc_signal<T>& operator=(const sc_bv_ns::sc_lv_base&);
  //  sc_signal<T>& write(const sc_bv_ns::sc_lv_base&);
#endif

    bool event() const;

#ifdef DEBUG_SYSTEMC
    void print(ostream& os) const;
    void dump(ostream& os) const;
#endif

    void trace( sc_trace_file* tf ) const;

protected:
        // Updates the current value of the signal with the new value.
    virtual void update();
    virtual void update_old();
    // The following needed for resolved signals
    void set_cur_value(const T& nv) { cur_value = nv; }
    void set_new_value(const T& nv) { new_value = nv; }
    const T& get_cur_value() const { return cur_value; }

    static void update_prop_none(sc_signal_base*);
    static void update_prop_async(sc_signal_base*);
    static void update_prop_aproc(sc_signal_base*);
    static void update_prop_both(sc_signal_base*);
    virtual UPDATE_FUNC* update_funcs_array() const;

private:
    T cur_value;
    T new_value;
    T old_value;
};

/*---------------------------------------------------------------------------*/

#include "sc_signal_defs.h_"
#include "sc_signal_spec.h_"

/*---------------------------------------------------------------------------*/

#endif
