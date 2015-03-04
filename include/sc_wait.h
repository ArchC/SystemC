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

    sc_wait.h -- Wait() and related functions. 

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: Added error messages to clarify the
    following: wait() has no effect for SC_METHODs. wait_until(),
    wait(n), and watching work only for SC_CTHREADS.

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_WAIT_H
#define SC_WAIT_H

extern void halt( sc_simcontext* = sc_get_curr_simcontext() );

extern void wait( sc_simcontext* = sc_get_curr_simcontext() );

inline void next( sc_simcontext* simc = sc_get_curr_simcontext() )
{
    wait(simc);
}

extern void wait( int,
                  sc_simcontext* = sc_get_curr_simcontext());

inline void next( int n,
                  sc_simcontext* simc = sc_get_curr_simcontext() )
{
    wait(n, simc);
}

extern void wait_until( const sc_lambda_ptr&,
                        sc_simcontext* = sc_get_curr_simcontext() );

inline void wait_until( const sc_signal_edgy_deval& s,
                       sc_simcontext* simc = sc_get_curr_simcontext() )
{
    wait_until( sc_lambda_ptr(s), simc );
}

extern void at_posedge( const sc_signal<sc_logic>&,
                        sc_simcontext* = sc_get_curr_simcontext() );

extern void at_posedge( const sc_signal_edgy&,
                        sc_simcontext* = sc_get_curr_simcontext() );

extern void at_negedge( const sc_signal<sc_logic>&,
                        sc_simcontext* = sc_get_curr_simcontext() );

extern void at_negedge( const sc_signal_edgy&,
                        sc_simcontext* = sc_get_curr_simcontext() );

inline void watching( const sc_lambda_ptr& lambda,
                      sc_simcontext* simc = sc_get_curr_simcontext() )
{
    (*simc->watching_fn)(lambda, simc);
}

inline void watching( const sc_signal_edgy_deval& s,
                      sc_simcontext* simc = sc_get_curr_simcontext() )
{
    (*simc->watching_fn)( sc_lambda_ptr(s), simc );
}

extern void __open_watching( sc_sync_process_handle );
extern void __close_watching( sc_sync_process_handle );
extern int  __watch_level( sc_sync_process_handle );
extern void __watching_first( sc_sync_process_handle );
extern void __sanitycheck_watchlists( sc_sync_process_handle );

class sc_watch {
public:
    sc_sync_process_handle sync_handle;
    sc_watch(sc_simcontext* simc)  {
        const sc_curr_proc_info* cpi = simc->get_curr_proc_info();
        assert(SC_CURR_PROC_SYNC == cpi->kind);
        sync_handle = reinterpret_cast<sc_sync_process_handle>(cpi->process_handle);
        __open_watching(sync_handle);
    }
    ~sc_watch() {
        __close_watching(sync_handle);
    }
};

extern void sc_set_location( const char*, int,
                             sc_simcontext* = sc_get_curr_simcontext() );

#endif
