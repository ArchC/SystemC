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

    sc_aproc.cpp -- asynchronous processes

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
#include <cstddef>
#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "sc_iostream.h"
#include "sc_process.h"
#include "sc_simcontext.h"
#include "sc_lambda.h"
#include "sc_module.h"
#include "sc_aproc.h"
#include "sc_signal.h"
#include "sc_signal_array.h"

#include "sc_bool_vector.h"
#include "sc_process_int.h"
#include "sc_port.h"

sc_aproc::sc_aproc( const char* nm, size_t stack_size )
    : sc_module(nm)
{
    declare_sc_aproc_process(sole_aproc, "entry", sc_module, entry);
    aproc_handle = sole_aproc;
    aproc_handle->set_stack_size( stack_size );
    end_module();
}

sc_aproc::~sc_aproc()
{
    /* EMPTY */
}

void 
sc_aproc::decl_input( const sc_signal_base& s )
{
    sc_module::sensitive << s;
}

void
sc_aproc::decl_input( const sc_signal_array_base& sa )
{
    sc_module::sensitive << sa;
}

void
sc_aproc::decl_input( const sc_signal_bool_vector& sbv )
{
    sc_module::sensitive << sbv;
}

void
sc_aproc::decl_input( const sc_port_b& p )
{
    sc_module::sensitive << p;
}

void
sc_aproc::decl_output( const sc_signal_base& )
{
    /* Intentionally blank */
}

void
sc_aproc::decl_output( const sc_signal_array_base& )
{
    /* Intentionally blank */
}
