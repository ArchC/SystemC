/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_module_registry.h -- Registry for all modules.
                          FOR INTERNAL USE ONLY.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_MODULE_REGISTRY_H
#define SC_MODULE_REGISTRY_H


#include "systemc/utils/sc_vector.h"

class sc_module;
class sc_simcontext;


// ----------------------------------------------------------------------------
//  CLASS : sc_module_registry
//
//  Registry for all modules.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_module_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_module& );
    void remove( sc_module& );

    int size() const
        { return m_module_vec.size(); }

private:

    // constructor
    explicit sc_module_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_module_registry();

    // called when elaboration is done
    void elaboration_done();

private:

    sc_simcontext*         m_simc;
    sc_pvector<sc_module*> m_module_vec;

private:

    // disabled
    sc_module_registry();
    sc_module_registry( const sc_module_registry& );
    sc_module_registry& operator = ( const sc_module_registry& );
};


#endif

// Taf!
