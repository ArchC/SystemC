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

    sc_resolved.h - Resolved signals

    Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_RESOLVED_H
#define SC_RESOLVED_H

#include "sc_reslv.h"
#include "sc_logic.h"
#include "sc_signal.h"

class sc_process_b;

// A resolved signal is a type of signal, and therefore inherits from
// sc_signal<T>. The only specialization we have to do is with the
// write and update methods and with the constructors. 

template <class T, class R>
class sc_signal_resolved_t : public sc_signal<T>
{
public:
  sc_signal_resolved_t();
  explicit sc_signal_resolved_t(char init_value);
  explicit sc_signal_resolved_t(const char* nm);
  
  ~sc_signal_resolved_t();

  // Hides the definition of write in sc_signal<T>
#ifndef _MSC_VER
  virtual sc_signal_resolved_t<T,R>& write( const T& d );
  virtual sc_signal_resolved_t<T,R>& operator=(const T& d) { return write(d); }
  virtual sc_signal_resolved_t<T,R>& operator=(const sc_signal_resolved_t<T,R>& d)
   {return operator=(d.read()); }
#else
  virtual sc_signal<T>& write( const T& d );
  virtual sc_signal<T>& operator=(const T& d) { return write(d); }
  virtual sc_signal<T>& operator=(const sc_signal_resolved_t<T,R>& d)
   {return operator=(d.read()); }
#endif

protected:
  // Hides the definition of update in sc_signal<T>
  void update();

    static void update_prop_none(sc_signal_base*);
    static void update_prop_async(sc_signal_base*);
    static void update_prop_aproc(sc_signal_base*);
    static void update_prop_both(sc_signal_base*);
    virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const;

private:
  // Data members special to this type of signal
  sc_pvector<sc_process_b*>* procs;      // The processes writing to this signal
  sc_pvector<T*>* nvalues;            // The new values written to this signal
};  

// Create type for sc_signal_resolved

typedef sc_signal_resolved_t<sc_logic, sc_logic_resolve> sc_signal_resolved;

#include "sc_resolved_defs.h_"

#endif
