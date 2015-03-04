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

    sc_signal_array.cpp -- one-dimensional arrays of signals of type T

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "sc_iostream.h"
#include "sc_signal_array.h"

const char* sc_signal_array_base::kind_string = "sc_signal_array";

// Signal arrays are themselves not added to the simulation context
// because they are treated as the collection of underlying signals.
// When writing to signal arrays, it's the underlying individual
// signals that are to be updated, not the signal array.  Thus, we
// should assert when update() is called on signal array.

sc_signal_array_base::sc_signal_array_base( sc_object* from )
    : sc_signal_base( from, /*add_to_simcontext*/ false )
{
    _length = 0;
    indiv_signals = 0;
    free_indiv_signals = false;
}

sc_signal_array_base::sc_signal_array_base( const char* nm, sc_object* from )
    : sc_signal_base( nm, from, /*add_to_simcontext*/ false )
{
    _length = 0;
    indiv_signals = 0;
    free_indiv_signals = false;
}

void
sc_signal_array_base::decl_sensitive_async( sc_async_process_handle async ) const
{
    for (int i = 0; i < _length; ++i) {
        indiv_signals[i]->decl_sensitive_async( async );
    }
}

void
sc_signal_array_base::decl_sensitive_aproc( sc_aproc_process_handle aproc ) const
{
    for (int i = 0; i < _length; ++i) {
        indiv_signals[i]->decl_sensitive_aproc( aproc );
    }
}


void
sc_signal_array_base::print(ostream&) const
{
    /*EMPTY*/
}

void
sc_signal_array_base::dump(ostream& os) const
{
    sc_signal_base::dump(os);
}

sc_signal_base::UPDATE_FUNC*
sc_signal_array_base::update_funcs_array() const
{
    assert( false );
    return 0;
}

void
sc_signal_array_base::update()
{
    // See comments at the top of the file.
    assert( false );

    for (int i = _length; i >= 0; --i) {
        indiv_signals[i]->update();
    }
}

void
sc_signal_array_base::update_old()
{
    // See comments at the top of the file.
    assert( false );

    for (int i = _length; i >= 0; --i) {
        indiv_signals[i]->update_old();
    }
}

#if defined(__BCPLUSPLUS__)
#pragma warn -8060
#endif
bool sc_signal_array_base::event()const
{
  int i=0;
  bool b = true;
  while(i<_length && (b=indiv_signals[i]->event())) i++;
  return b;
}
#if defined(__BCPLUSPLUS__)
#pragma warn .8060
#endif

