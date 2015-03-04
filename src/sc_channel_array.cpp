/******************************************************************************
    Copyright (c) 1996-2001 CoWare, Inc.    ALL RIGHTS RESERVED

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

    sc_channel_array_defs.h_ -- definitions for the template class
                                sc_channel_array<A>

    Original Author: CoWare Inc.

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include "sc_iostream.h"
#include "sc_channel_array.h"

const char* sc_channel_array_base::kind_string = "sc_channel_array";

sc_channel_array_base::sc_channel_array_base( sc_object* from )
    : sc_signal_base( from )
{
    _length = 0;
    indiv_channels = 0;
    free_indiv_channels = false;
}

sc_channel_array_base::sc_channel_array_base( const char* nm, sc_object* from )
    : sc_signal_base( nm, from )
{
    _length = 0;
    indiv_channels = 0;
    free_indiv_channels = false;
}


#ifdef DEBUG_SYSTEMC

void
sc_channel_array_base::print(ostream&) const
{

}

void
sc_channel_array_base::dump(ostream& os) const
{

}

#endif

void
sc_channel_array_base::decl_sensitive_async( sc_async_process_handle async ) const
{
    for (int i = 0; i < _length; ++i) {
        indiv_channels[i]->decl_sensitive_async( async );
    }
}

void
sc_channel_array_base::decl_sensitive_aproc( sc_aproc_process_handle aproc ) const
{
    for (int i = 0; i < _length; ++i) {
        indiv_channels[i]->decl_sensitive_aproc( aproc );
    }
}

sc_signal_base::UPDATE_FUNC*
sc_channel_array_base::update_funcs_array() const
{
    assert( false );
    return 0;
}

void
sc_channel_array_base::update()
{
    for (int i = 0; i < _length; ++i) {
        indiv_channels[i]->update();
    }
}

void
sc_channel_array_base::update_old()
{
    for (int i = 0; i < _length; ++i) {
        indiv_channels[i]->update_old();
    }
}
