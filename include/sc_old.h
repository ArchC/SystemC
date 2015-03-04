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

    sc_old.h -- Compatability with old names in SystemC
  
    Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_OLD_H
#define SC_OLD_H

inline void co_initialize()             { sc_initialize();}
inline void co_cycle(double t)          { sc_cycle(t); }

typedef sc_logic                  std_ulogic;
typedef sc_logic_vector           std_ulogic_vector;
typedef sc_signal_logic_vector    signal_std_ulogic_vector;

typedef sc_signal_resolved        signal_std_logic;
typedef sc_signal_resolved_vector signal_std_logic_vector;

typedef sc_bool_vector            bool_vector;
typedef sc_signal_bool_vector     signal_bool_vector;

#endif  
