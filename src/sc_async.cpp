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

    sc_async.cpp -- asynchronous blocks

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
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_lambda.h"
#include "sc_async.h"
#include "sc_signal.h"
#include "sc_signal_array.h"

#include "sc_bool_vector.h"
#include "sc_port.h"

/*---------------------------------------------------------------------------*/

sc_async::sc_async( const char* nm )
    : sc_module(nm)
{
    is_aproc = false;
    declare_sc_async_process(sole_async, "entry", sc_module, entry);
    async_handle = sole_async;
    end_module();
}

sc_async::sc_async( const sc_string& nm )
    : sc_module(nm)
{
    is_aproc = false;
    declare_sc_async_process(sole_async, "entry", sc_module, entry);
    async_handle = sole_async;
    end_module();
}

sc_async::~sc_async()
{
    /* EMPTY */
}

void
sc_async::decl_input( const sc_signal_base& sb )
{
    sc_module::sensitive << sb;
}

void
sc_async::decl_input( const sc_signal_array_base& sa )
{
    sc_module::sensitive << sa;
}

void
sc_async::decl_input( const sc_port_b& pb )
{
    sc_module::sensitive << pb;
}

void
sc_async::decl_output( const sc_signal_base& )
{
    /* Intentionally blank */
}

void
sc_async::decl_output( const sc_signal_array_base& )
{
    /* Intentionally blank */
}

bool
sc_async::validate() const
{
    bool valid = true;
    return valid;
}

const char* sc_async::kind_string = "sc_async";

const char*
sc_async::kind() const
{
    return kind_string;
}
