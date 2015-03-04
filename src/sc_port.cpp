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

    sc_port.cpp -- Port classes.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
        Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)
    Description of Modification:
        resolved ports sc_xxx_rv<> added

    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "sc_iostream.h"
#include "sc_macros_int.h"
#include "sc_bvrep.h"
#include "sc_bit.h"

#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_signal.h"
#include "sc_signal_sc_bit.h"
#include "sc_port.h"
#include "sc_port_sc_bit.h"
#include "sc_port_manager.h"
#include "sc_clock.h"
#include "sc_polarity.h"
#include "sc_port_clk.h"

sc_port_typecheck::~sc_port_typecheck()
{
    /* EMPTY */
}

sc_port_b::sc_port_b(const sc_port_vtable* vt, PORT_TYINFO ty)
{
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();

    /* You are not supposed to understand this. */
    src_signal = reinterpret_cast<sc_signal_base*>(this);

    port_manager->add_port_unbound( this, vt, ty ); /* an unbound port */
}

sc_port_b::sc_port_b(sc_signal_base& sig, const sc_port_vtable* vt, PORT_TYINFO ty)
{
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();

    /* You are not supposed to understand this. */
    src_signal = reinterpret_cast<sc_signal_base*>(this);

    port_manager->add_port_bound_to_signal( this, vt, ty, &sig ); /* bound to signal */
}

sc_port_b::sc_port_b(const sc_port_b& p, const sc_port_vtable* vt, PORT_TYINFO ty)
{
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();

    /* You are not supposed to understand this. */
    src_signal = reinterpret_cast<sc_signal_base*>(this);

    const sc_signal_base* to_signal;
    const sc_port_b*      to_port;

    /* Find status of the port we are connecting this port to. */
    PORT_STATUS ps = port_manager->port_status(&p, &to_signal, &to_port);

    /* Short-circuit the path. */
    switch (ps) {
    case PORT_STATUS_UNBOUND:
        port_manager->add_port_bound_to_port( this, vt, ty, &p );
        break;
    case PORT_STATUS_BOUND_PORT:
        port_manager->add_port_bound_to_port( this, vt, ty, to_port );
        break;
    case PORT_STATUS_BOUND_SIGNAL:
        port_manager->add_port_bound_to_signal( this, vt, ty, to_signal );
        break;
    }
}

sc_port_b::~sc_port_b()
{
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();
    port_manager->remove_port( this );
}

BIND_STATUS
sc_port_b::bind_const(const sc_signal_base& sig)
{
    /* const signals can be bound to inports only */
    return (PORT_TYINFO_READ == rwmode()) ?
        vbind((sc_signal_base&) sig) : BIND_STATUS_NON_IN_TO_CONST_SIG;
}

BIND_STATUS
sc_port_b::vbind(sc_signal_base& sig)
{
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();
    return port_manager->bind_port_to_signal( this, &sig );
}

BIND_STATUS
sc_port_b::vbind(const sc_port_b& p)
{
    BIND_STATUS result = BIND_STATUS_UNKNOWN;
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();

    const sc_signal_base* to_signal = 0;
    const sc_port_b* to_port = 0;
    PORT_STATUS ps = port_manager->port_status(&p, &to_signal, &to_port);

    switch (ps) {
    case PORT_STATUS_UNBOUND:
        result = port_manager->bind_port_to_port( this, &p );
        break;
    case PORT_STATUS_BOUND_PORT:
        result = port_manager->bind_port_to_port( this, to_port );
        break;
    case PORT_STATUS_BOUND_SIGNAL:
        result = port_manager->bind_port_to_signal( this, to_signal );
        break;
    }
    return result;
}

PORT_TYINFO
sc_port_b::rwmode() const
{
    sc_port_manager* port_manager;
    port_manager = sc_get_curr_simcontext()->get_port_manager();
    return port_manager->port_rwmode(this);
}



bool
sc_port<bool>::type_check_sgnl(const sc_signal_base* sig)
{
    /* Make sure we can dynamically cast to sc_signal<T>* */
    const sc_signal<bool>* q = dynamic_cast< const sc_signal<bool>* >(sig);
    return (0 != q);
}

bool
sc_port<bool>::type_check_port(const sc_port_typecheck* tc)
{
    const sc_port_typecheck_T<bool>* tcb;
    tcb = dynamic_cast< const sc_port_typecheck_T<bool>* >(tc);
    return (0 != tcb);
}

const sc_port_vtable*
sc_port<bool>::port_vtfn()
{
    static sc_port_typecheck_T<bool> tc;
    static sc_port_vtable vt = { &type_check_sgnl,
                                 &type_check_port,
                                 &tc };
    return &vt;
}

BIND_STATUS
sc_port<bool>::bind(const sc_signal<bool>& sig)
{
    const sc_signal_base& sb = sig;
    return sc_port_b::vbind(const_cast<sc_signal_base&>(sb));
}

BIND_STATUS
sc_port<bool>::bind(const sc_port<bool>& p)
{
    const sc_port_b& pb = p;
    return sc_port_b::vbind(const_cast<sc_port_b&>(pb));
}

/* Specialization of sc_in<bool>, &c. */

sc_in<bool>::sc_in()
    : sc_readable_bool_port(PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<bool>::sc_in(const sc_signal<bool>& sig)
    : sc_readable_bool_port(sig, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<bool>::sc_in(const sc_in<bool>& p)
    : sc_readable_bool_port(p, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<bool>::sc_in(const sc_inout<bool>& p)
    : sc_readable_bool_port(p, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<bool>::~sc_in()
{
    /*EMPTY*/
}


sc_inout<bool>::sc_inout()
    : sc_readable_bool_port(PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<bool>::sc_inout(sc_signal<bool>& sig)
    : sc_readable_bool_port(sig, PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<bool>::sc_inout(const sc_inout<bool>& p)
    : sc_readable_bool_port(p, PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<bool>::~sc_inout()
{
    /*EMPTY*/
}



/* SPECIALIZATIONS FOR SC_BIT */

bool
sc_port<sc_bit>::type_check_sgnl(const sc_signal_base* sig)
{
    /* Make sure we can dynamically cast to sc_signal<T>* */
    const sc_signal<sc_bit>* q = dynamic_cast< const sc_signal<sc_bit>* >(sig);
    return (0 != q);
}

bool
sc_port<sc_bit>::type_check_port(const sc_port_typecheck* tc)
{
    const sc_port_typecheck_T<sc_bit>* tcb;
    tcb = dynamic_cast< const sc_port_typecheck_T<sc_bit>* >(tc);
    return (0 != tcb);
}

const sc_port_vtable*
sc_port<sc_bit>::port_vtfn()
{
    static sc_port_typecheck_T<sc_bit> tc;
    static sc_port_vtable vt = { &type_check_sgnl,
                                 &type_check_port,
                                 &tc };
    return &vt;
}

BIND_STATUS
sc_port<sc_bit>::bind(const sc_signal<sc_bit>& sig)
{
    const sc_signal_base& sb = sig;
    return sc_port_b::vbind(const_cast<sc_signal_base&>(sb));
}

BIND_STATUS
sc_port<sc_bit>::bind(const sc_port<sc_bit>& p)
{
    const sc_port_b& pb = p;
    return sc_port_b::vbind(const_cast<sc_port_b&>(pb));
}

/* Specialization of sc_in<sc_bit>, &c. */

sc_in<sc_bit>::sc_in()
    : sc_readable_sc_bit_port(PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_bit>::sc_in(const sc_signal<sc_bit>& sig)
    : sc_readable_sc_bit_port(sig, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_bit>::sc_in(const sc_in<sc_bit>& p)
    : sc_readable_sc_bit_port(p, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_bit>::sc_in(const sc_inout<sc_bit>& p)
    : sc_readable_sc_bit_port(p, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_bit>::~sc_in()
{
    /*EMPTY*/
}


sc_inout<sc_bit>::sc_inout()
    : sc_readable_sc_bit_port(PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<sc_bit>::sc_inout(sc_signal<sc_bit>& sig)
    : sc_readable_sc_bit_port(sig, PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<sc_bit>::sc_inout(const sc_inout<sc_bit>& p)
    : sc_readable_sc_bit_port(p, PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<sc_bit>::~sc_inout()
{
    /*EMPTY*/
}



/* SPECIALIZATION FOR SC_CLOCK */

bool
sc_port<sc_clock>::type_check_sgnl(const sc_signal_base* sig)
{
    /* Make sure we can dynamically cast to sc_clock* */
    const sc_clock* q = dynamic_cast<const sc_clock*>(sig);
    return (0 != q);
}

bool
sc_port<sc_clock>::type_check_port(const sc_port_typecheck* tc)
{
    const sc_port_typecheck_T<sc_clock>* tcb;
    tcb = dynamic_cast< const sc_port_typecheck_T<sc_clock>* >(tc);
    return (0 != tcb);
}

const sc_port_vtable*
sc_port<sc_clock>::port_vtfn()
{
    static sc_port_typecheck_T<sc_clock> tc;
    static sc_port_vtable vt = { &type_check_sgnl,
                                 &type_check_port,
                                 &tc };
    return &vt;
}

BIND_STATUS
sc_port<sc_clock>::bind(const sc_clock& clk)
{
    const sc_signal_base& sb = clk;
    return sc_port_b::vbind(const_cast<sc_signal_base&>(sb));
}

BIND_STATUS
sc_port<sc_clock>::bind(const sc_port<sc_clock>& p)
{
    const sc_port_b& pb = p;
    return sc_port_b::vbind(const_cast<sc_port_b&>(pb));
}

/* Specialization of sc_in<sc_clock>, &c. */

sc_in<sc_clock>::sc_in()
    : sc_readable_clk_port(PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_clock>::sc_in(const sc_clock& sig)
    : sc_readable_clk_port(sig, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_clock>::sc_in(const sc_in<sc_clock>& p)
    : sc_readable_clk_port(p, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_clock>::sc_in(const sc_inout<sc_clock>& p)
    : sc_readable_clk_port(p, PORT_TYINFO_READ)
{
    /*EMPTY*/
}

sc_in<sc_clock>::~sc_in()
{
    /*EMPTY*/
}


sc_inout<sc_clock>::sc_inout()
    : sc_readable_clk_port(PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<sc_clock>::sc_inout(sc_clock& sig)
    : sc_readable_clk_port(sig, PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<sc_clock>::sc_inout(const sc_inout<sc_clock>& p)
    : sc_readable_clk_port(p, PORT_TYINFO_READWRITE)
{
    /*EMPTY*/
}

sc_inout<sc_clock>::~sc_inout()
{
    /*EMPTY*/
}


sc_out<sc_clock>::sc_out()
    : sc_port<sc_clock>(PORT_TYINFO_WRITE)
{
    /*EMPTY*/
}

sc_out<sc_clock>::sc_out(sc_clock& sig)
    : sc_port<sc_clock>(sig, PORT_TYINFO_WRITE)
{
    /*EMPTY*/
}

sc_out<sc_clock>::sc_out(const sc_out<sc_clock>& p)
    : sc_port<sc_clock>(p, PORT_TYINFO_WRITE)
{
    /*EMPTY*/
}

sc_out<sc_clock>::sc_out(const sc_inout<sc_clock>& p)
    : sc_port<sc_clock>(p, PORT_TYINFO_WRITE)
{
    /*EMPTY*/
}

sc_out<sc_clock>::~sc_out()
{
    /*EMPTY*/
}
