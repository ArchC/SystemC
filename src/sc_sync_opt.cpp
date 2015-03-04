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

    sc_sync.cpp -- Base class of all processes

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <errno.h>
#include <stdio.h>
#include <cstring>
#include <assert.h>
#include <cstring>

#include "sc_iostream.h"
#include "sc_sync.h"
#include "sc_clock.h"
#include "sc_except.h"
#include "sc_simcontext.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_lambda.h"
#include "sc_process_int.h"
#include "sc_simcontext.h"

/******************************************************************************
    Constructor for the base class sc_sync.  The user-defined
    process must be publicly derived from this class, and must pass a
    reference to the clock to this constructor in the initializer,
    and, optionally, a user-defined name and/or the size of the stack
    to be allocated for the coroutine.  If `nm' is omitted, a unique
    name is generated.  If `stack_size' is omitted, then default is
    used.
******************************************************************************/
sc_sync::sc_sync( sc_clock_edge& ce, const char* nm, size_t stack_size )
    : sc_module(nm), clk_edge(ce), clk(ce.clock())
{
    init( stack_size, ce );
}

sc_sync::sc_sync( sc_clock& ck, const char* nm, size_t stack_size )
    : sc_module(nm), clk_edge(ck.pos()), clk(ck)
{
    init( stack_size, ck.pos() );
}

sc_sync::sc_sync( const sc_string& nm,
                  sc_clock_edge& ce,
                  size_t stack_size )
    : sc_module(nm), clk_edge(ce), clk(ce.clock())
{
    init( stack_size, ce );
}

sc_sync::sc_sync( const sc_string& nm,
                  sc_clock& ck,
                  size_t stack_size )
    : sc_module(nm), clk_edge(ck.pos()), clk(ck)
{
    init( stack_size, ck.pos() );
}

void
sc_sync::init( size_t stack_size, sc_clock_edge& clock_edge )
{
    declare_sc_sync_process(sole_sync, "entry", clock_edge, sc_module, entry);
    sync_handle = sole_sync;
    sync_handle->set_stack_size( stack_size );
    end_module();
}

/******************************************************************************
    sc_sync::~sc_sync() is the destructor for sc_sync.
    Need to destroy all the lambdas that were added to the watchlist,
    as well as the stack space.
******************************************************************************/
sc_sync::~sc_sync()
{
    /* EMPTY */
}

const char* sc_sync_kind_string = "SC_SYNC";

const char*
sc_sync::kind() const
{
    return sc_sync_kind_string;
}
