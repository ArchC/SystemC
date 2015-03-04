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

    sc_async_process.cpp --

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <assert.h>
#include <cstring>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_lambda.h"
#include "sc_process_int.h"
#include "sc_signal.h"
#include "sc_signal_array.h"

#include "sc_bool_vector.h"

sc_async_process::sc_async_process(const char* nm,
                                   SC_ENTRY_FUNC fn,
                                   sc_module* mod)
    : sc_process_b(nm, fn, mod)
{
    static int next_proc_id = 0;

    /* Initialize member variables */
    proc_id         = ++next_proc_id;
    m_is_in_updateq = false;
    next_handle     = 0;
}


sc_async_process::~sc_async_process()
{
    /* EMPTY */
}


void
sc_async_process::sensitive( const sc_signal_base& s )
{
    s.decl_sensitive_async(this);
}

void
sc_async_process::sensitive( const sc_signal_array_base& sa )
{
    sa.decl_sensitive_async(this);
}

void
sc_async_process::sensitive( const sc_signal_bool_vector& sbv )
{
    sbv.decl_sensitive_async(this);
}

void
sc_async_process::sensitive_pos( const sc_signal_edgy& s )
{
    s.decl_sensitive_async_pos(this);
}

void
sc_async_process::sensitive_neg( const sc_signal_edgy& s )
{
    s.decl_sensitive_async_neg(this);
}

void
sc_async_process::sensitive( const sc_signal_edgy& s )
{
    s.decl_sensitive_async_neg(this);
    s.decl_sensitive_async_pos(this);
}
