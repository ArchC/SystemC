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

#ifndef SC_PORT_MANAGER_H
#define SC_PORT_MANAGER_H

/* Requires "sc_port.h" */

template<class T> class sc_pvector;
template<class K, class C> class sc_phash;

class sc_port_b;
struct sc_port_info;
struct sc_port_vtable;
class sc_lambda_rand;
class sc_lambda_ptr;

class sc_port_manager {
    friend class sc_simcontext;

public:
    PORT_STATUS port_status( const sc_port_b* port,
                             const sc_signal_base** to_signal = 0, 
                             const sc_port_b** to_port = 0 );
    PORT_TYINFO port_rwmode( const sc_port_b* port );
    PORT_TYINFO port_tyinfo( const sc_port_b* port );
    const sc_port_typecheck* typecheck_object( const sc_port_b* port );

    void add_port_unbound( sc_port_b* port, const sc_port_vtable* vt, PORT_TYINFO ty );
    void add_port_bound_to_signal( sc_port_b* port, const sc_port_vtable* vt, 
                                   PORT_TYINFO ty, const sc_signal_base* to_signal );
    void add_port_bound_to_port( sc_port_b* port, const sc_port_vtable* vt,
                                 PORT_TYINFO ty, const sc_port_b* to_port );

    void remove_port( sc_port_b* port );
    BIND_STATUS bind_port_to_port( sc_port_b* port, const sc_port_b* );
    BIND_STATUS bind_port_to_signal( sc_port_b* port, const sc_signal_base* );

    void aproc_sensitive_port(const sc_port_b&, sc_aproc_process_handle);
    void async_sensitive_port(const sc_port_b&, sc_async_process_handle);
    void async_sensitive_port_edgy_pos(const sc_port_b&, sc_async_process_handle);
    void aproc_sensitive_port_edgy_pos(const sc_port_b&, sc_aproc_process_handle);
    void async_sensitive_port_edgy_neg(const sc_port_b&, sc_async_process_handle);
    void aproc_sensitive_port_edgy_neg(const sc_port_b&, sc_aproc_process_handle);

    void add_unbound_sync_handle(sc_sync_process_handle);

    void flag_error();
    bool resolve_all_ports();
    void add_lambda_for_resolution(const sc_lambda_ptr& lambda);
    bool port_exists( const sc_port_b* p ) { return (0 != get_port_info(p, false)); }

private:
    bool error_occurred;
    sc_simcontext* simc; /* back pointer to the simulation context */
    sc_phash<sc_port_b*, sc_port_info*>* port_status_table;
    sc_pvector<sc_lambda_ptr*>* unresolved_lambdas;
    sc_sync_process_handle unbound_syncs;

    sc_port_manager( sc_simcontext* );
    ~sc_port_manager();

    void delete_port_status_table();
    void delete_unbound_syncs();
    void delete_unresolved_lambdas();

    void add_port( sc_port_b* port, const sc_port_vtable*,
                   PORT_TYINFO, PORT_STATUS,
                   const sc_signal_base*, const sc_port_b* );
    sc_port_info* get_port_info( const sc_port_b*, bool must_find = true );
    void resolve_lambdas();
    void resolve_unbound_syncs();

    static void replace_port( sc_port_manager*, sc_lambda_rand* );
};

extern void sc_port_errmsg(BIND_STATUS bs, int port_index,
                           const char* module_name);

#endif
