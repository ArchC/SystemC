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

    sc_port_manager.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: 
    - Corrected port resolution so that every port is either bound to
    a signal or is unbounded after the resultion has taken place.
    - Added error handling to resolve_unbound_syncs

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <assert.h>
#include <stdlib.h>

#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_mempool.h"
#include "sc_vector.h"
#include "sc_hash.h"

#include "sc_lambda.h"
#include "sc_module.h"
#include "sc_port.h"
#include "sc_clock.h"
#include "sc_polarity.h"
#include "sc_port_clk.h"
#include "sc_port_manager.h"
#include "sc_process_int.h"

#include "sc_clock_int.h"

struct sc_port_info {
    PORT_STATUS           port_status;
    PORT_TYINFO           port_tyinfo;
    const sc_port_vtable* port_vtable;
    sc_module*  container;  /* the module that contains this port */
    int         ordinal;    /* ordinal of the port in the module's port list */
    const sc_signal_base* to_signal;
    const sc_port_b*      to_port;
    sc_pvector<sc_async_process_handle>* sensitive_asyncs;
    sc_pvector<sc_aproc_process_handle>* sensitive_aprocs;
    sc_pvector<sc_async_process_handle>* sensitive_asyncs_neg;
    sc_pvector<sc_aproc_process_handle>* sensitive_aprocs_neg;
    bool        is_edgy()  const;
    bool        is_clock() const;

    static void* operator new(size_t sz)     { return sc_mempool::allocate(sz); }
    static void operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

    void resolve_sensitives_posneg();
    void resolve_sensitives();

    sc_port_info();
    ~sc_port_info();
};

sc_port_info::sc_port_info()
{
    sensitive_asyncs = 0;
    sensitive_aprocs = 0;
    sensitive_asyncs_neg = 0;
    sensitive_aprocs_neg = 0;
}

sc_port_info::~sc_port_info()
{
    delete sensitive_asyncs;
    delete sensitive_aprocs;
    delete sensitive_asyncs_neg;
    delete sensitive_aprocs_neg;
}

bool
sc_port_info::is_edgy() const
{
    return (PORT_TYINFO_ISEDGY == (port_tyinfo & PORT_TYINFO_ISEDGY));
}

bool
sc_port_info::is_clock() const
{
    return (PORT_TYINFO_ISCLK  == (port_tyinfo & PORT_TYINFO_ISCLK));
}

void
sc_port_info::resolve_sensitives_posneg()
{
    int i;
    const sc_signal_edgy* sb = static_cast<const sc_signal_edgy*>(to_signal);
    if (sensitive_asyncs) {
        for (i = 0; i < sensitive_asyncs->size(); ++i) {
            sc_async_process_handle async_h;
            async_h = sensitive_asyncs->fetch(i);
            async_h->sensitive_pos(*sb);
        }
    }
    if (sensitive_asyncs_neg) {
        for (i = 0; i < sensitive_asyncs_neg->size(); ++i) {
            sc_async_process_handle async_h;
            async_h = sensitive_asyncs_neg->fetch(i);
            async_h->sensitive_neg(*sb);
        }
    }
    if (sensitive_aprocs) {
        for (i = 0; i < sensitive_aprocs->size(); ++i) {
            sc_aproc_process_handle aproc_h;
            aproc_h = sensitive_aprocs->fetch(i);
            aproc_h->sensitive_pos(*sb);
        }
    }
    if (sensitive_aprocs_neg) {
        for (i = 0; i < sensitive_aprocs_neg->size(); ++i) {
            sc_aproc_process_handle aproc_h;
            aproc_h = sensitive_aprocs_neg->fetch(i);
            aproc_h->sensitive_neg(*sb);
        }
    }
}

void
sc_port_info::resolve_sensitives()
{
    int i;
    const sc_signal_base* sb = to_signal;
    if (sensitive_asyncs) {
        for (i = 0; i < sensitive_asyncs->size(); ++i) {
            sc_async_process_handle async_h;
            async_h = sensitive_asyncs->fetch(i);
            async_h->sensitive(*sb);
        }
    }
    if (sensitive_aprocs) {
        for (i = 0; i < sensitive_aprocs->size(); ++i) {
            sc_aproc_process_handle aproc_h;
            aproc_h = sensitive_aprocs->fetch(i);
            aproc_h->sensitive(*sb);
        }
    }
}



sc_port_manager::sc_port_manager(sc_simcontext* simcontext)
{
    simc = simcontext;
    error_occurred = false;
    port_status_table = new sc_phash<sc_port_b*, sc_port_info*>;
    unresolved_lambdas = new sc_pvector<sc_lambda_ptr*>;
    unbound_syncs = 0;
}

sc_port_manager::~sc_port_manager()
{
    delete_unresolved_lambdas();
    delete_unbound_syncs();
    delete_port_status_table();
}

void
sc_port_manager::delete_port_status_table()
{
    if (port_status_table) {
        sc_phash<sc_port_b*, sc_port_info*>::iterator it(port_status_table);
        for (; !it.empty(); it++) {
            delete it.contents();
        }
        delete port_status_table;
    }
    port_status_table = 0;
}

void
sc_port_manager::delete_unresolved_lambdas()
{
    if (unresolved_lambdas) {
        for (int i = 0; i < unresolved_lambdas->size(); ++i) {
            delete unresolved_lambdas->fetch(i);
        }
        delete unresolved_lambdas;
    }
    unresolved_lambdas = 0;
}

void
sc_port_manager::delete_unbound_syncs()
{
    if (unbound_syncs) {
        sc_sync_process_handle curr, next;
        for (curr = unbound_syncs;
             0 != curr;
             curr = next) {
            next = curr->next_handle;
            delete curr;
        }
    }
    unbound_syncs = 0;
}

void
sc_port_manager::add_port( sc_port_b* port,
                           const sc_port_vtable* vt,
                           PORT_TYINFO ty,
                           PORT_STATUS ps,
                           const sc_signal_base* to_signal,
                           const sc_port_b* to_port )
{
    /* Must not add any more ports */
    assert( ! simc->is_running() );

    sc_module* curr_module;
    curr_module = simc->hierarchy_curr();

    sc_port_info* pinfo = new sc_port_info;
    /* Only append unbound ports to module's array */
    if (PORT_STATUS_UNBOUND == ps) {
        /* Note that if we have a port that's unbound and
           that appears outside of a module, then there
           is an error in the user's code. */
        if(curr_module==NULL)
            REPORT_ERROR(4002,"");
        pinfo->container = curr_module;
        pinfo->ordinal = curr_module->append_port(port);
    } else {
        /* Otherwise, we have something like a parameter,
           which must always be bound to another port or
           to a signal. */
        pinfo->container = 0;
        pinfo->ordinal = 0;
    }
    pinfo->port_status = ps;
    pinfo->port_tyinfo = ty;
    pinfo->port_vtable = vt;
    pinfo->to_signal = to_signal;
    pinfo->to_port = to_port;
    (void) port_status_table->insert(port, pinfo);
}

void
sc_port_manager::add_port_unbound( sc_port_b* port,
                                   const sc_port_vtable* vt,
                                   PORT_TYINFO ty )
{
    add_port( port, vt, ty, PORT_STATUS_UNBOUND, 0, port );
}

void
sc_port_manager::add_port_bound_to_signal( sc_port_b* port,
                                           const sc_port_vtable* vt,
                                           PORT_TYINFO ty,
                                           const sc_signal_base* to_signal )
{
    add_port( port, vt, ty, PORT_STATUS_BOUND_SIGNAL,
              to_signal, (sc_port_b*)0 );
}

void
sc_port_manager::add_port_bound_to_port( sc_port_b* port,
                                         const sc_port_vtable* vt,
                                         PORT_TYINFO ty,
                                         const sc_port_b* to_port )
{
    add_port( port, vt, ty, PORT_STATUS_BOUND_PORT,
              (sc_signal_base*)0, to_port );
}

sc_port_info*
sc_port_manager::get_port_info( const sc_port_b* port, bool must_find )
{
    sc_port_info* port_info = 0;
    bool found = port_status_table->lookup((sc_port_b*) port, &port_info);
    assert( !must_find || found );
    return port_info;
}

PORT_STATUS
sc_port_manager::port_status( const sc_port_b* port,
                              const sc_signal_base** p_to_signal,
                              const sc_port_b** p_to_port )
{
    sc_port_info* port_info = get_port_info( port );
    if (p_to_signal) {
        *p_to_signal = port_info->to_signal;
    }
    if (p_to_port) {
        *p_to_port = port_info->to_port;
    }
    return port_info->port_status;
}


PORT_TYINFO
sc_port_manager::port_tyinfo( const sc_port_b* port )
{
    sc_port_info* port_info = get_port_info( port );
    return port_info->port_tyinfo;
}

PORT_TYINFO
sc_port_manager::port_rwmode( const sc_port_b* port )
{
    sc_port_info* port_info = get_port_info( port );
    return (PORT_TYINFO_RWMASK & port_info->port_tyinfo);
}

const sc_port_typecheck*
sc_port_manager::typecheck_object( const sc_port_b* port )
{
    sc_port_info* port_info = get_port_info( port );
    return port_info->port_vtable->typecheck_obj;
}

void
sc_port_manager::remove_port( sc_port_b* port )
{
    sc_port_info* pi;
    sc_port_b* pp;
    bool found;
    if (!port_status_table)
        return;
    found = port_status_table->remove(port, &pp, &pi);
    assert(found);
    assert(pp == port);
    assert(PORT_STATUS_UNBOUND != pi->port_status);
    delete pi;
}

BIND_STATUS
sc_port_manager::bind_port_to_port( sc_port_b* port, const sc_port_b* to_port )
{
    sc_port_info* pi = get_port_info( port );
    if (PORT_STATUS_UNBOUND != pi->port_status) {
        return BIND_STATUS_REBIND;
    }
    pi->to_port = to_port;
    pi->port_status = PORT_STATUS_BOUND_PORT;

    if (! (*pi->port_vtable->type_check_port)( typecheck_object(to_port) )) {
        return BIND_STATUS_WRONG_TYPE;
    } else {
        switch (port->rwmode()) {
        case PORT_TYINFO_READ: /* an inport can be bound to in or inout */
            switch (to_port->rwmode()) {
            case PORT_TYINFO_READ:
            case PORT_TYINFO_READWRITE:
                return BIND_STATUS_OK;
            default:
                return BIND_STATUS_IN_TO_OUT;
            }
            /* NOTREACHED */
        case PORT_TYINFO_WRITE: /* an outport can be bound to an out or inout */
            switch (to_port->rwmode()) {
            case PORT_TYINFO_WRITE:
            case PORT_TYINFO_READWRITE:
                return BIND_STATUS_OK;
            default:
                return BIND_STATUS_OUT_TO_IN;
            }
            /* NOTREACHED */
        case PORT_TYINFO_READWRITE:  /* an inoutport can be bound to an inout only */
            switch (to_port->rwmode()) {
            case PORT_TYINFO_READWRITE:
                return BIND_STATUS_OK;
            default:
                return BIND_STATUS_INOUT_TO_NON_INOUT;
            }
        }
    }
    return BIND_STATUS_UNKNOWN;
}

BIND_STATUS
sc_port_manager::bind_port_to_signal( sc_port_b* port, const sc_signal_base* to_signal )
{
    sc_port_info* pi = get_port_info( port );
    if (PORT_STATUS_UNBOUND != pi->port_status) {
        return BIND_STATUS_REBIND;
    }
    pi->to_signal = to_signal;
    pi->port_status = PORT_STATUS_BOUND_SIGNAL;

    /* This performs type-specific binding (now only validity check) */
    return ((*pi->port_vtable->type_check_sgnl)(to_signal) ?
            BIND_STATUS_OK :
            BIND_STATUS_WRONG_TYPE);
}

void
sc_port_manager::flag_error()
{
    error_occurred = true;
}

void
sc_port_manager::async_sensitive_port(const sc_port_b& port,
                                      sc_async_process_handle async_h)
{
    sc_port_info* pi = get_port_info( &port );
    /* Cannot be sensitive to output ports */
    assert(PORT_TYINFO_WRITE != (pi->port_tyinfo & PORT_TYINFO_RWMASK));
    if (!pi->sensitive_asyncs)
        pi->sensitive_asyncs = new sc_pvector<sc_async_process_handle>;
    pi->sensitive_asyncs->push_back(async_h);
}

void
sc_port_manager::aproc_sensitive_port(const sc_port_b& port,
                                      sc_aproc_process_handle aproc_h)
{
    sc_port_info* pi = get_port_info( &port );
    /* Cannot be sensitive to output ports */
    assert(PORT_TYINFO_WRITE != (pi->port_tyinfo & PORT_TYINFO_RWMASK));
    if (!pi->sensitive_aprocs)
        pi->sensitive_aprocs = new sc_pvector<sc_aproc_process_handle>;
    pi->sensitive_aprocs->push_back(aproc_h);
}

void
sc_port_manager::async_sensitive_port_edgy_pos(const sc_port_b& port,
                                               sc_async_process_handle async_h)
{
    sc_port_info* pi = get_port_info( &port );
    assert( pi->is_edgy() );
    if (!pi->sensitive_asyncs)
        pi->sensitive_asyncs = new sc_pvector<sc_async_process_handle>;
    pi->sensitive_asyncs->push_back(async_h);
}

void
sc_port_manager::aproc_sensitive_port_edgy_pos(const sc_port_b& port,
                                               sc_aproc_process_handle aproc_h)
{
    sc_port_info* pi = get_port_info( &port );
    assert( pi->is_edgy() );
    if (!pi->sensitive_aprocs)
        pi->sensitive_aprocs = new sc_pvector<sc_aproc_process_handle>;
    pi->sensitive_aprocs->push_back(aproc_h);
}

void
sc_port_manager::async_sensitive_port_edgy_neg(const sc_port_b& port,
                                               sc_async_process_handle async_h)
{
    sc_port_info* pi = get_port_info( &port );
    assert( pi->is_edgy() );
    if (!pi->sensitive_asyncs_neg)
        pi->sensitive_asyncs_neg = new sc_pvector<sc_async_process_handle>;
    pi->sensitive_asyncs_neg->push_back(async_h);
}

void
sc_port_manager::aproc_sensitive_port_edgy_neg(const sc_port_b& port,
                                               sc_aproc_process_handle aproc_h)
{
    sc_port_info* pi = get_port_info( &port );
    assert( pi->is_edgy() );
    if (!pi->sensitive_aprocs_neg)
        pi->sensitive_aprocs_neg = new sc_pvector<sc_aproc_process_handle>;
    pi->sensitive_aprocs_neg->push_back(aproc_h);
}

void
sc_port_manager::add_unbound_sync_handle(sc_sync_process_handle sync_h)
{
    sync_h->next_handle = unbound_syncs;
    unbound_syncs = sync_h;
}

bool
sc_port_manager::resolve_all_ports()
{
    /* Ensure that all ports have been bound */

    /* Loop through port_status_table to update the signals */
    sc_phash<sc_port_b*, sc_port_info*>::iterator it(port_status_table);
    for ( ; !it.empty(); it++) {
        sc_port_b* port = it.key();
        sc_port_info* port_info = it.contents();

        switch (port_info->port_status) {
        case PORT_STATUS_UNBOUND: { 
            cerr << "SystemC error: Port ";
            if (port_info->container) {
                cerr << (port_info->ordinal + 1)
                     << " of module `" << port_info->container->name() << "'";
            } else {
                cerr << port;
            }
            cerr << " is unbound" << endl;
            flag_error();
            break;
        }

        case PORT_STATUS_BOUND_PORT: {
            /* In case to_port is not bound to a signal at all or is
               not bound to a signal yet, walk thru the list of ports
               using to_port pointers to find either a port bound to a
               signal or a port unbounded. While walking thru the
               list, we can also bind the ports that we visit, but the
               outer for loop already handles that, so there is no
               need to introduce an extra level of complexity here. */

            const sc_port_b* to_port = port_info->to_port;
            sc_port_info* to_port_info = get_port_info( to_port );

            while (PORT_STATUS_BOUND_PORT == to_port_info->port_status) {
              to_port = to_port_info->to_port;
              to_port_info = get_port_info( to_port );
            }

            /* TBD: We'll have to figure out how to do bit-select of BV ports */

            if (PORT_STATUS_BOUND_SIGNAL == to_port_info->port_status) {
                /* Set current port's status to that signal */
                port_info->to_signal   = to_port_info->to_signal;
                port_info->port_status = PORT_STATUS_BOUND_SIGNAL;
            }
            else /* if (PORT_STATUS_UNBOUND == to_port_info->port_status) */ {
                /* Set current port's status to unbound because
                   to_port is unbound */
                port_info->port_status = PORT_STATUS_UNBOUND;

                cerr << "SystemC error: Port ";
                if (port_info->container) {
                  cerr << (port_info->ordinal + 1)
                       << " of module `" << port_info->container->name() << "'";
                } else {
                  cerr << port;
                }
                cerr << " is unbound because port ";
                if (to_port_info->container) {
                  cerr << (to_port_info->ordinal + 1)
                       << " of module `" << to_port_info->container->name() << "'";
                } else {
                  cerr << to_port;
                }
                cerr << " is unbound" << endl;
                flag_error();

                break;  /* Because of the error found, DO NOT FALL
                           THRU to the following case but break to the
                           end of switch. */
            }
        }

        /* We need to fall through here because after changing a port's status
           to PORT_STATUS_BOUND_SIGNAL, we need to hook up processes that were
           declared sensitive to this port, to the actual signal. */
        /* FALLTHRU */

        case PORT_STATUS_BOUND_SIGNAL: {
            /* Skip outports */
            if (PORT_TYINFO_WRITE ==
                (PORT_TYINFO_RWMASK & port_info->port_tyinfo))
                break;

            /* Update the sensitive lists */
            if (port_info->is_edgy()) {
                /* Special-case bool signals */
                port_info->resolve_sensitives_posneg();
            } else {
                port_info->resolve_sensitives();
            }
            break;
        }
        }  // switch
    }  // for 

    if (!error_occurred) {
        /* Now, let's update the lambdas */
        resolve_lambdas();
        resolve_unbound_syncs();
    }

    /* Then resolve the src_signal field of the ports. */
    for (it.reset(port_status_table); !it.empty(); it++) {
        sc_port_b* port = it.key();
        sc_port_info* port_info = it.contents();
        switch (port_info->port_status) {
        case PORT_STATUS_UNBOUND:
            /* This case has signaled a user error already */
            break;
        case PORT_STATUS_BOUND_PORT:
            /* This case cannot happen because the status of every
                port is either set to a signal or is unbound. */
            assert(false);
            break;
        case PORT_STATUS_BOUND_SIGNAL:
            port->src_signal = const_cast<sc_signal_base*>(port_info->to_signal);
            break;
        }
    }

    /* No longer needed. */
    delete_port_status_table();

    return !error_occurred;
}

void
sc_port_manager::add_lambda_for_resolution(const sc_lambda_ptr& lambda)
{
    sc_lambda_ptr* lambda_copy = new sc_lambda_ptr(lambda);
    unresolved_lambdas->push_back(lambda_copy);
}

/* This is a static member function. */
void
sc_port_manager::replace_port( sc_port_manager* port_manager, sc_lambda_rand* rand )
{
    switch (rand->rand_ty) {
    case SC_LAMBDA_RAND_SIGNAL_BOOL: {
        /* Now the fun part */
        /* The signal pointer we are looking at is an sc_signal_edgy,
           so let's first cast it to sc_signal_base.  Note that this
           cast is static, implicit, because sc_signal_base is a base class
           of sc_signal_edgy */
        const sc_signal_base* sb = rand->edgy_sig;

        /* Undo the reinterpret_cast in the constructor of sc_port_b */
        const sc_port_b* pb = reinterpret_cast<const sc_port_b*>(sb);

        /* Look up the port's status */
        sc_port_info* port_info = port_manager->get_port_info( pb, /*must_find?*/ false );

        /* If the lambda had been created with a port, we need to now replace
           it with the real signal. */
        if (port_info) {
            /* At this point the port must have been bound to a sc_signal_edgy */
            assert( PORT_STATUS_BOUND_SIGNAL == port_info->port_status );
            assert( port_info->is_edgy() );

            /* Now replace rand's bool_sig with the real one */
            rand->edgy_sig = static_cast<const sc_signal_edgy*>( port_info->to_signal );
        }
        break;
    }

    default:
        break;
    }
}

void
sc_port_manager::resolve_lambdas()
{
    for (int i = 0; i < unresolved_lambdas->size(); ++i) {
        sc_lambda_ptr* lambda_copy = unresolved_lambdas->fetch(i);
        (*lambda_copy)->replace_ports( &replace_port, this );
    }
    delete_unresolved_lambdas();
}

void
sc_port_manager::resolve_unbound_syncs()
{
    sc_sync_process_handle curr, next;
    const sc_readable_clk_port* clk_port;

    for (curr = unbound_syncs;
         0 != curr;
         curr = next) {
        next = curr->next_handle; /* safe operation! */

        curr->next_handle = 0;
        curr->prev_handle = 0;
        clk_port = curr->polarity.clk_port;

        /* Find the real clock object */
        sc_port_info* port_info = get_port_info( clk_port, /*must_find?*/true );

        /* The clock should have been bound to a signal */
        assert(PORT_STATUS_BOUND_SIGNAL == port_info->port_status);
        assert(port_info->is_clock());

        const sc_signal_base* sig = port_info->to_signal;
        const sc_clock* clock = dynamic_cast<const sc_clock*>(sig);

        if ( 0 == clock ) {
            cerr << "SystemC error: Port ";
            if (port_info->container) {
                cerr << (port_info->ordinal + 1)
                     << " (a clock port) of module `" 
                     << port_info->container->name() << "'";
            } else {
                cerr << clk_port;
            }
            cerr << " must eventually be bound to a clock signal" << endl;
            assert(false);
        }

        curr->clock_edge = (curr->polarity.positive) ? &(clock->pos()) : &(clock->neg());

        /* Because we had placed the temporary list in reverse order,
           we will put them onto the real list at the front so that
           the order is recovered. */
        sc_clock_helpers::bind_sync_to_clock_edge_front(curr->clock_edge, curr);
    }
    unbound_syncs = 0;
}

void
sc_port_errmsg(BIND_STATUS bs, int port_index, const char* module_name)
{
    cerr << "SystemC error: Port " << port_index
         << " of module `" << module_name << "'";
    switch (bs) {
    case BIND_STATUS_OK:
        break;

    case BIND_STATUS_REBIND:
        cerr << " has already been bound, cannot be bound again" << endl;
        break;

    case BIND_STATUS_WRONG_TYPE:
        cerr << " is being bound to a signal/port of incompatible type" << endl;
        break;

    case BIND_STATUS_IN_TO_OUT:
        cerr << " is an in-port, but being bound to an out-port" << endl;
        break;

    case BIND_STATUS_OUT_TO_IN:
        cerr << " is an out-port, but being bound to an in-port" << endl;
        break;

    case BIND_STATUS_INOUT_TO_NON_INOUT:
        cerr << " is an inout-port, but being bound to an in-port or out-port" << endl;
        break;

    case BIND_STATUS_NON_IN_TO_CONST_SIG:
        cerr << " is an out- or inout-port, but being bound to a const signal" << endl;
        break;

    case BIND_STATUS_UNKNOWN:
        cerr << " is causing an unknown error while being bound" << endl;
        break;
    }
}
