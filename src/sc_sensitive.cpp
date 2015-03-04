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

    sc_sensitive.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <assert.h>
#include <stdlib.h>

#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_sensitive.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_lambda.h"
#include "sc_module.h"
#include "sc_process_int.h"
#include "sc_port.h"
#include "sc_port_manager.h"

sc_sensitive::sc_sensitive(sc_module* p)
    : module(p)
{
    mode = SC_NONE;
    /* No need to initialize handles */
}

sc_sensitive::~sc_sensitive()
{
    /*EMPTY*/
}

sc_sensitive&
sc_sensitive::operator=(const sc_sensitive&)
{
    assert(false);
	return *this;
}

sc_sensitive::sc_sensitive(const sc_sensitive&)
{
    assert(false);
}

sc_sensitive&
sc_sensitive::operator<<(const sc_async_process_handle& p)
{
    mode = SC_ASYNC;
    process_handle = p;
    return *this;
}

sc_sensitive&
sc_sensitive::operator<<(const sc_aproc_process_handle& p)
{
    mode = SC_APROC;
    process_handle = p;
    return *this;
}

sc_sensitive&
sc_sensitive::operator<<(const sc_signal_base& s)
{
    switch (mode) {
    case SC_NONE:
        /* Issue message */
        break;
        
    case SC_APROC:
        ((sc_aproc_process_handle) process_handle)->sensitive(s);
        break;
        
    case SC_ASYNC:
        ((sc_async_process_handle) process_handle)->sensitive(s);
        break;
    }
    return *this;
}

sc_sensitive&
sc_sensitive::operator()(const sc_signal_base& s)
{
    return operator<<(s);
}

sc_sensitive&
sc_sensitive::operator<<(const sc_port_b& p)
{
    sc_simcontext* simc;
    sc_port_manager* port_manager;
    PORT_STATUS ps;
    PORT_TYINFO ty;
    const sc_port_b* q = &p;
    const sc_signal_base* to_signal = 0;
    const sc_port_b* to_port = 0;

    switch (mode) {
    case SC_NONE:
        /* do nothing */
        break;

    case SC_APROC:
    case SC_ASYNC:
        simc = process_handle->module->simcontext();
        port_manager = simc->get_port_manager();
        ty = port_manager->port_rwmode(q);
        /* Cannot be sensitive to write-only ports. */
        if (PORT_TYINFO_WRITE == ty) {
            cerr << "SystemC warning: Attempt to make process `"
                 << process_handle->name() << "' sensitive to an outport" << endl;
            break;
        }
        ps = port_manager->port_status(q, &to_signal, &to_port);
        switch (ps) {
        case PORT_STATUS_BOUND_SIGNAL:
            (void) operator<<(*to_signal);
            break;
        case PORT_STATUS_BOUND_PORT:
            q = to_port;
            /* FALLTHRU */
        case PORT_STATUS_UNBOUND:
            if (SC_APROC == mode) {
                port_manager->
                    aproc_sensitive_port(*q, (sc_aproc_process_handle)process_handle);
            } else {
                port_manager->
                    async_sensitive_port(*q, (sc_async_process_handle)process_handle);
            }
            break;
        }
        break;
    }
    return *this;
}

sc_sensitive&
sc_sensitive::operator()(const sc_port_b& p)
{
    return operator<<(p);
}

sc_sensitive&
sc_sensitive::operator<<(const sc_signal_edgy& s)
{
    switch (mode) {
    case SC_NONE:
        /* Issue message */
        break;
        
    case SC_APROC:
        ((sc_aproc_process_handle) process_handle)->sensitive(s);
        break;
        
    case SC_ASYNC:
        ((sc_async_process_handle) process_handle)->sensitive(s);
        break;
    }
    return *this;
}

sc_sensitive&
sc_sensitive::operator()(const sc_signal_edgy& p)
{
    return operator<<(p);
}

sc_sensitive&
sc_sensitive::operator<<(const sc_port_edgy& p)
{
    sc_simcontext* simc;
    sc_port_manager* port_manager;
    PORT_STATUS ps;
    PORT_TYINFO ty;
    const sc_port_b* q = &p;
    const sc_signal_base* to_signal = 0;
    const sc_port_b* to_port = 0;

    switch (mode) {
    case SC_NONE:
        /* do nothing */
        break;
        
    case SC_APROC:
    case SC_ASYNC:
        simc = process_handle->module->simcontext();
        port_manager = simc->get_port_manager();
        ty =port_manager->port_rwmode(q);
        /* Cannot be sensitive to output ports. */
        if (PORT_TYINFO_WRITE == ty) {
            cerr << "SystemC warning: Attempt to make process `"
                 << process_handle->name() << "' sensitive to an outport" << endl;
            break;
        }
        ps = port_manager->port_status( q, &to_signal, &to_port );
        switch (ps) {
        case PORT_STATUS_BOUND_SIGNAL:
            (void) operator<<( *((const sc_signal_edgy*) to_signal) );
            break;
        case PORT_STATUS_BOUND_PORT:
            q = to_port;
            /* FALLTHRU */
        case PORT_STATUS_UNBOUND:
            if (SC_APROC == mode) {
                port_manager->
                    aproc_sensitive_port_edgy_pos(
                        *q, (sc_aproc_process_handle)process_handle);
                port_manager->
                    aproc_sensitive_port_edgy_neg(
                        *q, (sc_aproc_process_handle)process_handle);
            } else {
                port_manager->
                    async_sensitive_port_edgy_pos(
                        *q, (sc_async_process_handle)process_handle);
                port_manager->
                    async_sensitive_port_edgy_neg(
                        *q, (sc_async_process_handle)process_handle);
            }
            break;
        }
        break;
    }
    return *this;
}

sc_sensitive&
sc_sensitive::operator()(const sc_port_edgy& p)
{
    return operator<<(p);
}


sc_sensitive&
sc_sensitive::operator<<(const sc_signal_array_base& s)
{
    switch (mode) {
    case SC_NONE:
        /* Issue message */
        break;
        
    case SC_APROC:
        ((sc_aproc_process_handle)process_handle)->sensitive(s);
        break;
        
    case SC_ASYNC:
        ((sc_async_process_handle)process_handle)->sensitive(s);
        break;
    }
    return *this;
}

sc_sensitive&
sc_sensitive::operator()(const sc_signal_array_base& s)
{
    return operator<<(s);
}

sc_sensitive&
sc_sensitive::operator<<(const sc_signal_bool_vector& s)
{
    switch (mode) {
    case SC_NONE:
        /* Issue message */
        break;
        
    case SC_APROC:
        ((sc_aproc_process_handle)process_handle)->sensitive(s);
        break;
        
    case SC_ASYNC:
        ((sc_async_process_handle)process_handle)->sensitive(s);
        break;
    }
    return *this;
}

sc_sensitive&
sc_sensitive::operator()(const sc_signal_bool_vector& s)
{
    return operator<<(s);
}



sc_sensitive_pos&
sc_sensitive_pos::operator=(const sc_sensitive_pos&)
{
    assert(false);
	return *this;
}

sc_sensitive_pos::sc_sensitive_pos(const sc_sensitive_pos&)
{
    assert(false);
}

sc_sensitive_pos::sc_sensitive_pos(sc_module* p)
    : module(p)
{
    mode = SC_NONE;
    /* No need to initialize handles */
}

sc_sensitive_pos::~sc_sensitive_pos()
{
    /*EMPTY*/
}

sc_sensitive_pos&
sc_sensitive_pos::operator<<(const sc_async_process_handle& p)
{
    mode = SC_ASYNC;
    process_handle = p;
    return *this;
}

sc_sensitive_pos&
sc_sensitive_pos::operator<<(const sc_aproc_process_handle& p)
{
    mode = SC_APROC;
    process_handle = p;
    return *this;
}


sc_sensitive_pos&
sc_sensitive_pos::operator<<(const sc_signal_edgy& s)
{
    switch (mode) {
    case SC_NONE:
        /* Issue message */
        break;
        
    case SC_APROC:
        ((sc_aproc_process_handle)process_handle)->sensitive_pos(s);
        break;
        
    case SC_ASYNC:
        ((sc_async_process_handle)process_handle)->sensitive_pos(s);
        break;
    }
    return *this;
}

sc_sensitive_pos&
sc_sensitive_pos::operator()(const sc_signal_edgy& s)
{
    return operator<<(s);
}


sc_sensitive_pos&
sc_sensitive_pos::operator<<(const sc_port_edgy& p)
{
    sc_simcontext* simc;
    sc_port_manager* port_manager;
    PORT_STATUS ps;
    PORT_TYINFO ty;
    const sc_port_b* q = &p;
    const sc_signal_base* to_signal = 0;
    const sc_port_b* to_port = 0;

    switch (mode) {
    case SC_NONE:
        /* do nothing */
        break;
        
    case SC_APROC:
    case SC_ASYNC:
        simc = process_handle->module->simcontext();
        port_manager = simc->get_port_manager();
        ty = port_manager->port_rwmode(q);
        /* Cannot be sensitive to write-only ports. */
        if (PORT_TYINFO_WRITE == ty) {
            cerr << "SystemC warning: Attempt to make process `"
                 << process_handle->name() << "' sensitive to an outport" << endl;
            break;
        }
        ps = port_manager->port_status(q, &to_signal, &to_port);
        switch (ps) {
        case PORT_STATUS_BOUND_SIGNAL:
            (void) operator<<( *((const sc_signal_edgy*)to_signal) );
            break;
        case PORT_STATUS_BOUND_PORT:
            q = to_port;
            /* FALLTHRU */
        case PORT_STATUS_UNBOUND:
            if (SC_APROC == mode) {
                port_manager->
                    aproc_sensitive_port_edgy_pos(
                        *q, (sc_aproc_process_handle)process_handle);
            } else {
                port_manager->
                    async_sensitive_port_edgy_pos(
                        *q, (sc_async_process_handle)process_handle);
            }
            break;
        }
        break;
    }
    return *this;
}

sc_sensitive_pos&
sc_sensitive_pos::operator()(const sc_port_edgy& p)
{
    return operator<<(p);
}




sc_sensitive_neg&
sc_sensitive_neg::operator=(const sc_sensitive_neg&)
{
    assert(false);
	return *this;
}

sc_sensitive_neg::sc_sensitive_neg(const sc_sensitive_neg&)
{
    assert(false);
}

sc_sensitive_neg::sc_sensitive_neg(sc_module* p)
    : module(p)
{
    mode = SC_NONE;
    /* No need to initialize handles */
}

sc_sensitive_neg::~sc_sensitive_neg()
{
    /*EMPTY*/
}

sc_sensitive_neg&
sc_sensitive_neg::operator<<(const sc_async_process_handle& p)
{
    mode = SC_ASYNC;
    process_handle = p;
    return *this;
}

sc_sensitive_neg&
sc_sensitive_neg::operator<<(const sc_aproc_process_handle& p)
{
    mode = SC_APROC;
    process_handle = p;
    return *this;
}

sc_sensitive_neg&
sc_sensitive_neg::operator<<(const sc_signal_edgy& s)
{
    switch (mode) {
    case SC_NONE:
        /* Issue message */
        break;
        
    case SC_APROC:
        ((sc_aproc_process_handle)process_handle)->sensitive_neg(s);
        break;
        
    case SC_ASYNC:
        ((sc_async_process_handle)process_handle)->sensitive_neg(s);
        break;
    }
    return *this;
}

sc_sensitive_neg&
sc_sensitive_neg::operator()(const sc_signal_edgy& s)
{
    return operator<<(s);
}


sc_sensitive_neg&
sc_sensitive_neg::operator<<(const sc_port_edgy& p)
{
    sc_simcontext* simc;
    sc_port_manager* port_manager;
    PORT_STATUS ps;
    PORT_TYINFO ty;
    const sc_port_b* q = &p;
    const sc_signal_base* to_signal = 0;
    const sc_port_b* to_port = 0;

    switch (mode) {
    case SC_NONE:
        /* do nothing */
        break;
        
    case SC_APROC:
    case SC_ASYNC:
        simc = process_handle->module->simcontext();
        port_manager = simc->get_port_manager();
        ty = port_manager->port_rwmode(q);
        /* Cannot be sensitive to write-only ports. */
        if (PORT_TYINFO_WRITE == ty) {
            cerr << "SystemC warning: Attempt to make process `"
                 << process_handle->name() << "' sensitive to an outport" << endl;
            break;
        }
        ps = port_manager->port_status(q, &to_signal, &to_port);
        switch (ps) {
        case PORT_STATUS_BOUND_SIGNAL:
            (void) operator<<( *((const sc_signal_edgy*)to_signal) );
            break;
        case PORT_STATUS_BOUND_PORT:
            q = to_port;
            /* FALLTHRU */
        case PORT_STATUS_UNBOUND:
            if (SC_APROC == mode) {
                port_manager->
                    aproc_sensitive_port_edgy_neg(
                        *q, (sc_aproc_process_handle)process_handle);
            } else {
                port_manager->
                    async_sensitive_port_edgy_neg(
                        *q, (sc_async_process_handle)process_handle);
            }
            break;
        }
        break;
    }
    return *this;
}

sc_sensitive_neg&
sc_sensitive_neg::operator()(const sc_port_edgy& p)
{
    return operator<<(p);
}
