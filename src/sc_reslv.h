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

    sc_reslv.h -- C++ implementation of resolution function class.  
    Behaves pretty much the same way as VHDL and Verilog.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_RESLV_H
#define SC_RESLV_H

#include "sc_vector.h"
#include "sc_logic.h"
#include "sc_lv.h"
using sc_bv_ns::sc_lv;

//
// <summary> Resolution function for sc_logic </summary>
//
// This class provides a static member function, resolve, as
// required by sc_resolved and sc_resolved_array.  A vector of
// pointers to sc_logic values is passed to resolve, which
// returns the resolved value of these sc_logic values.
//
class sc_logic_resolve
{
public:
        // Resolves sc_logic values and return the resolved value
        // according to a table (q.v.).
    static sc_logic resolve( const sc_pvector<sc_logic*>& );
#ifdef STATIC_MEM_TEMP
    template<int W>
    static sc_lv<W> resolve( const sc_pvector<sc_lv<W>*>& values);
#endif
};

#ifndef STATIC_MEM_TEMP
template<int W>
class sc_lv_resolve
{
  public:
  static sc_lv<W> resolve( const sc_pvector<sc_lv<W>*>& values);
};
#endif

extern const sc_logic::Log_enum resolution_table[4][4];

template<int W>
sc_lv<W>
#ifdef STATIC_MEM_TEMP
sc_logic_resolve::resolve( const sc_pvector<sc_lv<W>*>& values)
#else
sc_lv_resolve<W>::resolve( const sc_pvector<sc_lv<W>*>& values)
#endif
{
  if(values.size()==0)
    return sc_lv<W>(3); //'X'
  if(values.size() == 1)
    return *(values[0]);
  else
  {
    int size = values.size();
    sc_lv<W> result; // rely on RVO to remove temporaries
    for(int i=0; i<W; i++)
    {
      long res = (*values[0])[i].to_long();
      int j=1;
      while(j<size && res!=3)
      { // "X"(3) supresses all other values
        res = resolution_table[(*values[j])[i].to_long()][res];
        j++;
      }
      result[i]=res;
    }
    return result;
  }
}

#endif
