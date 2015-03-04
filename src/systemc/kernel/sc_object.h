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

  sc_object.h -- Abstract base class of all SystemC `simulation' objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_OBJECT_H
#define SC_OBJECT_H


#include "systemc/utils/sc_iostream.h"
#include "systemc/kernel/sc_attribute.h"

class sc_trace_file;
class sc_simcontext;


// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
//  Abstract base class of all SystemC `simulation' objects.
// ----------------------------------------------------------------------------

class sc_object
{
    friend class sc_object_manager;

public:

    const char* name() const
        { return m_name; }
    const char* basename() const;

    void print() const;
    virtual void print(ostream& os) const;

    // dump() is more detailed than print()
    void dump() const;
    virtual void dump(ostream& os) const;

    virtual void trace( sc_trace_file* tf ) const;

    static const char* kind_string;
    virtual const char* kind() const;

    sc_simcontext* simcontext() const
        { return m_simc; }

    // add attribute
    bool add_attribute( sc_attr_base& );

    // get attribute by name
          sc_attr_base* get_attribute( const sc_string& name_ );
    const sc_attr_base* get_attribute( const sc_string& name_ ) const;

    // remove attribute by name
    sc_attr_base* remove_attribute( const sc_string& name_ );

    // remove all attributes
    void remove_all_attributes();

    // get the number of attributes
    int num_attributes() const;

    // get the attribute collection
          sc_attr_cltn& attr_cltn();
    const sc_attr_cltn& attr_cltn() const;

protected:

    sc_object();
    sc_object(const char* nm);
    virtual ~sc_object();

private:

    void sc_object_init(const char* nm);

private:

    /* Each simulation object is associated with a simulation context */ 
    sc_simcontext* m_simc;
    char*          m_ptr;
    char*          m_name;
    sc_attr_cltn   m_attr_cltn;
};


// ----------------------------------------------------------------------------

extern const char SC_HIERARCHY_CHAR;
extern bool sc_enable_name_checking;


#endif
