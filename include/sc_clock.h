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

    sc_clock.h -- The clock process for SystemC

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Stan Y. Liao, Synopsys, Inc. 000120
    Description of Modification: Make sc_clock derive from sc_signal<bool>
                                 so that it can be operated on as such,
                                 instead of forwarding to a separate signal.
    
******************************************************************************/

#ifndef SC_CLOCK_H
#define SC_CLOCK_H

#ifdef WIN32
#include "sc_cmnhdr.h"
#include "Windows.h"
#else
#include "qt/qt.h"
#endif

#include "sc_macros.h"
#include "sc_list.h"
#include "sc_logic.h"
#include "sc_object.h"
#include "sc_signal.h"

class sc_signal_base;
template<class T> class sc_signal;
class sc_signal_delay_base;

struct qt_t;
class sc_sync;

enum phase_t { PHASE_NEG = 0, PHASE_POS };

#ifndef WIN32
extern "C" void* sc_clock_edge_yieldhelp( qt_t*, void*, void* );
#endif

//  <summary> The clock process </summary>
//
//  sc_clock is used to instantiate clock objects.  A clock has a period,
//  and the time instant when the first edge occurs (as well as a name).
//  A clock is used to synchronize events among processes.  But there may
//  be multiple non-isochronous clocks as well.
//
//  We derive sc_clock from sc_signal_base, because we want to be able
//  to control it externally as well as automatically generating waveforms.
//
class sc_clock : public sc_signal<bool> {
    friend class sc_clock_helpers;
    friend class sc_sync;
    friend class sc_async;
    friend class sc_aproc;
    friend class sc_clock_edge;
    friend class sc_simcontext;
    friend class sc_signal_optimize;
public:
    static const char* kind_string;
    const char* kind() const { return kind_string; }

    void print(ostream&) const;
    void dump(ostream&) const;
    void trace( sc_trace_file* tf ) const;
    
        // Constructor - user can specify name, period, starting edge
        // and orientation of the starting edge (pos or neg).  No
        // simulation context is specified; use the default global context.
    sc_clock( const char* nm = 0, double p = 1.0, 
              double d = 0.5, double s = 0.0, bool posedge_first = true );

        // Destructor
    ~sc_clock();

        // Returns the unique id of the clock.
    int id() const { return cid; }

    double period() const { return per; }
    double duty_cycle() const { return dut; }

    sc_clock_edge& pos() const { return *edges[1]; }
    sc_clock_edge& neg() const { return *edges[0]; }
    const sc_signal<bool>& signal() const { return *this; }

    // derived from sc_signal<bool>
    // bool read() const          { return sig->read(); }
    // operator bool() const      { return sig->read(); }
    // bool edgy_read() const; /* virtual function for sc_signal_edgy */

    // There's also a function "clock_write()" in sc_clock_int.h
    // If you change any of the following three, please also update
    // sc_clock_helpers::clock_write().
#ifdef _MSC_VER
#define sc_clock_virt_ret_type sc_signal<bool>
#else
#define sc_clock_virt_ret_type sc_clock
#endif

	sc_clock_virt_ret_type& write( bool nv )
    {
        new_value = nv;
        if (cur_value != nv)
            submit_update();
        return *this;
    }
    virtual sc_clock_virt_ret_type& operator=( const sc_signal<bool>& nv )
    {
      sc_signal<bool>::operator=(nv);
      return *this;
    }
    virtual sc_clock_virt_ret_type& operator=( bool nv )
    {
        new_value = nv;
        if (cur_value != nv)
            submit_update();
        return *this;
    }
    virtual sc_clock_virt_ret_type& operator=( const sc_clock& nv )
    {
        bool nn = nv.read();
        new_value = nn;
        if (cur_value != nn)
            submit_update();
        return *this;
    }
#undef sc_clock_virt_ret_type

    /* bool event() const     { return sig->event();   } -- inherited */
    /* bool posedge() const   { return sig->posedge(); } -- inherited */
    /* bool negedge() const   { return sig->negedge(); } -- inherited */

    typedef int (*callback_fn)( const sc_clock_edge& edge, void* arg );
    // Begins simulation.  The duration of the simulation is specified
    // in the first parameter.  At each clock edge, the callback function
    // will be called with an argument if the callback function is not 0.
    static void start( double duration, callback_fn=0, void* =0 );
    static void stop();
    static double time_stamp();
    // For compatibility with old stuff
    static void press_start_button(double duration, callback_fn cb=0, void* ua=0)
      { start(duration, cb, ua); }
    static void press_stop_button() { stop();}


/******************************************************************************
                        NO PEEKING BEYOND THIS POINT
******************************************************************************/

protected:
    virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const;

    virtual void update();
    // void update_old(); -- inherited from sc_signal<bool>

    /* The decl_sensitive_* functions are all virtual in sc_signal_edgy. */
    // no longer necessary - because we derive from sc_signal<bool>
    // instead of contain it.

private:
    void init( double p, double d, double s, bool posedge_first );

private:
    static int num_clocks;

private:
    int cid;                    // id of this clock
    double per;                 // the period of this clock
    double dut;                 // the duty cycle (fraction of period)
    bool optimized;             // true iff the clock has been optimized
                                // (reduced) to sc_signal<bool> whereby
                                // bookkeeping for sc_syncs isn't necessary

    sc_clock_edge* edges[2];    // 0 = neg_edge, 1 = pos_edge
    // We no longer use sig
    // sc_signal<bool>* sig;       // signal corresponding to this clock
                                // use negative period to indicate that
                                // clock was created from a signal

    bool m_edge_seen_in_delta_cycle;
    void set_edge_seen_in_delta_cycle(bool v) { m_edge_seen_in_delta_cycle = v; }
    bool edge_seen_in_delta_cycle() const { return m_edge_seen_in_delta_cycle; }
};

/*****************************************************************************/

struct da_elem {
    int delay;
    sc_signal_delay_base* first;
};

class sc_clock_edge {
    /* Note: a part of the interface (private) for sc_clock_edge is
       defined in sc_clock_helpers, in sc_clock_int.h, which
       should not be included by the user ... */
    friend class sc_clock_helpers;
    friend class sc_clock;
    friend class sc_simcontext;
    friend class sc_sync;
    friend class sc_sync_process;
public:
        // Constructor - public used as an implicit type convertor
    sc_clock_edge( sc_clock& );
        // Destructor
    ~sc_clock_edge();

    int id() const { return eid; }
    bool is_implicitly_converted() const { return eid == -1; }

        // Returns the underlying clock for this clock edge
    sc_clock& clock() const { return clk; }
    phase_t phase() const { return faze; }

    const char* name() const { return clk.name(); }
        // Returns the period of the clock.
    double period() const { return per; }
        // Returns the time instant of the edge that just occured.
    double prev_edge() const { return next_time - per; }
        // Returns the time instant of the next edge.
    double next_edge() const { return next_time; }
    double time_stamp() const { return next_time - per; }

        // Update the value of the clock signal, as well as advancing
        // time.
    void tick();

        // Sets the period of the clock.  Used if period not specified during
        // clock creation.
    void set_period( double p ) { per = p; }
        // Set the start time of the clock. Used if start time not
        // specified during clock creation.
    void set_start_time( double s ) { next_time = s; }

        // Advances the clock by switching into the first ready process
        // in the clock's process-list.  Returns false if there are no
        // more processes in the clock's process-list.
    void advance();

        // Submits a signal update request to the clock's update-list.
#if 0
    void submit_update( sc_signal_base* sig );
    void submit_update_d( sc_signal_delay_base* sd, int delay );
#endif

/******************************************************************************
                        NO PEEKING BEYOND THIS POINT
******************************************************************************/

private:
    static int num_clock_edges;
    static sc_plist<sc_clock_edge*> list_of_clock_edges;

private:
    sc_clock_edge( sc_clock&, phase_t, double p, double s );
#ifndef WIN32
    qt_t* qt() const { return sp; }
    friend void* sc_clock_edge_yieldhelp( qt_t*, void*, void* );
#else
    PVOID fiber() { return pFiber; }
#endif

    void reset();

public:
    sc_sync_process_handle register_sc_sync_process(const char* name,
                                                    SC_ENTRY_FUNC entry_fn,
                                                    sc_module*);
private:
    int eid;
    sc_clock& clk;
    const phase_t faze;
#ifndef WIN32
    qt_t* sp;                   // QuickThreads handle
#else
    PVOID pFiber;               // Fibers handle
#endif
    double per;                 // the period of this clock
    const double init_time;     // this should never change
    double next_time;           // the time when the next edge will occur

    /* Signals tied to this edge -- updated a delta cycle after the edge */
    sc_signal_base** signals_to_update;
    int              lastof_signals_to_update;

    sc_plist<sc_clock_edge*>::handle_t handle;

    int  total_number_of_syncs;
    sc_sync_process_handle sync_runnable;
    sc_sync_process_handle sync_defunct;
};

/*****************************************************************************/

inline ostream&
operator<<( ostream& str, const sc_clock_edge& edge )
{
    return (str << edge.name() << (edge.phase() == PHASE_POS ? " UP" : " DN"));
}

/*****************************************************************************/


#endif // SC_CLOCK_H
