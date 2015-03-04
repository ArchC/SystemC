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

    sc_signal_rv.h - implementation of sc_signal_rv

    Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/
#ifndef sc_signal_rvH
#define sc_signal_rvH

#include "sc_logic.h"
#include "sc_reslv.h"
#include "sc_resolved.h"
#include "sc_lv.h"
using sc_bv_ns::sc_bv;
using sc_bv_ns::sc_lv;

#ifdef STATIC_MEM_TEMP
#define RESOLVE_CLASS sc_logic_resolve
#else
#define RESOLVE_CLASS sc_lv_resolve<W>
#endif

template<int W>
class sc_signal_rv : public sc_signal_resolved_t<sc_lv<W>,RESOLVE_CLASS>
{
  typedef sc_signal_resolved_t<sc_lv<W>,RESOLVE_CLASS> base;
  public:
  sc_signal_rv():base('X'){};
  explicit sc_signal_rv(char init_value):base(init_value){};
  explicit sc_signal_rv(const char* nm):base(nm){};

#ifndef _MSC_VER
  sc_signal_rv& write( const sc_lv<W>& d )
    {return static_cast<sc_signal_rv&>(base::write(d));};
  sc_signal_rv& operator=(const sc_lv<W>& d) { return write(d); }
  sc_signal_rv& operator=(const sc_signal_rv& d)
    {return operator=(d.read()); }
#else
// requires casts in user code
#endif
};

//---------------------------------------------------------------------------
#endif
