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

    sc_sync.h -- Base class of all processes (sequential)

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_SYNC_H
#define SC_SYNC_H

#include "sc_list.h"
#include "sc_vector.h"

#ifdef WIN32
#include "sc_cmnhdr.h"
#include <Windows.h>
#else
#include "qt/qt.h"
#endif
#include "sc_constants.h"
#include "sc_macros.h"
#include "sc_lambda.h"
#include "sc_module.h"

class sc_clock;
class sc_clock_edge;
class sc_lambda;

//
//  <summary> Base class for user-defined sequential processes </summary>
//
//  sc_sync is the base class for all user-defined
//  processes in the SystemC environment.  The user defines a
//  process by publicly deriving from sc_sync, thereby
//  inheriting all the fundamental capabilities described
//  herein; in particular, reactivity.  The constructor for
//  sc_sync requires a reference to the clock (type
//  sc_clock) against which the process will be synchronized,
//  and optionally a name (char*) and a stack size.
//
class sc_sync : public sc_module 
{
    friend class sc_clock;
    friend class sc_clock_edge;

public:
    const char* kind() const;
    sc_clock&      clock()      const { return clk;      }
    sc_clock_edge& clock_edge() const { return clk_edge; } 

protected:
        // Constructor - the user must supply the reference to the
        // clock that drives it in its initializer for the base class.
        // Optionally, a name and a stack size may be specified.
    sc_sync( sc_clock_edge& ce,
             const char* nm = 0,
             size_t stack_size = SC_DEFAULT_STACK_SIZE );

    sc_sync( sc_clock& ck,
             const char* nm = 0,
             size_t stack_size = SC_DEFAULT_STACK_SIZE );
    sc_sync( const sc_string& nm,
             sc_clock_edge& ce,
             size_t stack_size = SC_DEFAULT_STACK_SIZE );

    sc_sync( const sc_string& nm,
             sc_clock& ck,
             size_t stack_size = SC_DEFAULT_STACK_SIZE );

        // Destructor
    virtual ~sc_sync();
    virtual void entry() = 0;

private:
    void init( size_t size, sc_clock_edge& edge );
    sc_sync_process_handle sync_handle;
    sc_clock_edge& clk_edge;
    sc_clock&      clk;
#ifndef SC_USE_MEMBER_FUNC_PTR
    public:
    sc_vc6_process_helper_class<sc_sync> sc_vc6_process_helper;
#endif
};

extern const char* sc_sync_kind_string;

typedef sc_sync sc_process;

/*****************************************************************************/

#endif
