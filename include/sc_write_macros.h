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

    sc_write_macros.h - Some useful macros for saving typing (and your wrist)

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_WRITE_MACROS_H
#define SC_WRITE_MACROS_H

/* These #defines are required since the preprocessor chokes on <foo,bar> */
#define TEMPL1(T)      template<class T>
#define TEMPL2(A,T)    template<class A, class T>
#define ITEMPL2(N,A,T) N<A,T>

// Signals need to declare friends to sc_signal_write(); use the following
// macro to get both versions for sc_sync and sc_async
#define DECLARE_FRIEND_SC_WRITE( SIGREF, RV ) \
friend void sc_signal_write( sc_module*, SIGREF, RV ); \
friend void sc_signal_write_d( sc_sync*, SIGREF, RV, int );


#define D_TEMPL_SC_WRITE( TEMPL, SIGREF, RV ) \
TEMPL inline void \
sc_signal_write( sc_module* proc, SIGREF sig, RV nv ) \
{ \
    if (sig.xwrite(proc,nv)) (*(get_curr_submit()))(proc, &sig); \
} \
TEMPL inline void \
sc_signal_write_d( sc_sync* proc, SIGREF sig, RV nv, int delay ) \
{ \
    if (delay <= 0) \
        sc_signal_write( proc, sig, nv ); \
    else \
        proc->clock_edge().submit_update_d(sig.xwrite_d(proc,nv),delay); \
} \
TEMPL inline void \
write( SIGREF sig, RV nv ) \
{ \
    sc_signal_write( get_curr_proc(), sig, nv ); \
} \
TEMPL inline void \
write_d( SIGREF sig, RV nv, int delay ) \
{ \
    sc_signal_write_d( (sc_sync*) get_curr_proc(), sig, nv, delay ); \
}

#define D_SC_WRITE( SIGREF, RV ) \
inline void \
sc_signal_write( sc_module* proc, SIGREF sig, RV nv ) \
{ \
    if (sig.xwrite(proc,nv)) (*(get_curr_submit()))(proc, &sig); \
} \
inline void \
sc_signal_write_d( sc_sync* proc, SIGREF sig, RV nv, int delay ) \
{ \
    if (delay <= 0) \
        sc_signal_write( proc, sig, nv ); \
    else \
        proc->clock_edge().submit_update_d(sig.xwrite_d(proc,nv),delay); \
} \
inline void \
write( SIGREF sig, RV nv ) \
{ \
    sc_signal_write( get_curr_proc(), sig, nv ); \
} \
inline void \
write_d( SIGREF sig, RV nv, int delay ) \
{ \
    sc_signal_write_d( (sc_sync*) get_curr_proc(), sig, nv, delay ); \
}

#endif
