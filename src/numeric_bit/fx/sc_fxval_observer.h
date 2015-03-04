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

    sc_fxval_observer.h - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_FXVAL_OBSERVER_H
#define SC_FXVAL_OBSERVER_H


#include "sc_fxdefs.h"


#ifdef SC_ENABLE_OBSERVERS

#define _SC_FXVAL_OBSERVER_CONSTRUCT(object)                                  \
    _SC_OBSERVER(object,sc_fxval_observer*,construct)
#define _SC_FXVAL_OBSERVER_DESTRUCT(object)                                   \
    _SC_OBSERVER(object,sc_fxval_observer*,destruct)
#define _SC_FXVAL_OBSERVER_READ(object)                                       \
    _SC_OBSERVER(object,sc_fxval_observer*,read)
#define _SC_FXVAL_OBSERVER_WRITE(object)                                      \
    _SC_OBSERVER(object,sc_fxval_observer*,write)
#define _SC_FXVAL_OBSERVER_DEFAULT                                            \
    _SC_OBSERVER_DEFAULT(sc_fxval_observer)

#define _SC_FXVAL_FAST_OBSERVER_CONSTRUCT(object)                             \
    _SC_OBSERVER(object,sc_fxval_fast_observer*,construct)
#define _SC_FXVAL_FAST_OBSERVER_DESTRUCT(object)                              \
    _SC_OBSERVER(object,sc_fxval_fast_observer*,destruct)
#define _SC_FXVAL_FAST_OBSERVER_READ(object)                                  \
    _SC_OBSERVER(object,sc_fxval_fast_observer*,read)
#define _SC_FXVAL_FAST_OBSERVER_WRITE(object)                                 \
    _SC_OBSERVER(object,sc_fxval_fast_observer*,write)
#define _SC_FXVAL_FAST_OBSERVER_DEFAULT                                       \
    _SC_OBSERVER_DEFAULT(sc_fxval_fast_observer)

#else

#define _SC_FXVAL_OBSERVER_CONSTRUCT(object)
#define _SC_FXVAL_OBSERVER_DESTRUCT(object)
#define _SC_FXVAL_OBSERVER_READ(object)
#define _SC_FXVAL_OBSERVER_WRITE(object)
#define _SC_FXVAL_OBSERVER_DEFAULT

#define _SC_FXVAL_FAST_OBSERVER_CONSTRUCT(object)
#define _SC_FXVAL_FAST_OBSERVER_DESTRUCT(object)
#define _SC_FXVAL_FAST_OBSERVER_READ(object)
#define _SC_FXVAL_FAST_OBSERVER_WRITE(object)
#define _SC_FXVAL_FAST_OBSERVER_DEFAULT

#endif


class sc_fxval;
class sc_fxval_fast;


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_observer
//
//  Abstract base class for fixed-point value type observers;
//  arbitrary precision.
// ----------------------------------------------------------------------------

class sc_fxval_observer
{

protected:
    
    sc_fxval_observer() {}
    virtual ~sc_fxval_observer() {}

public:

    virtual void construct( const sc_fxval& );
    virtual void  destruct( const sc_fxval& );
    virtual void      read( const sc_fxval& );
    virtual void     write( const sc_fxval& );

    static sc_fxval_observer* (*default_observer) ();

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast_observer
//
//  Abstract base class for fixed-point value type observers;
//  limited precision.
// ----------------------------------------------------------------------------

class sc_fxval_fast_observer
{

protected:
    
    sc_fxval_fast_observer() {}
    virtual ~sc_fxval_fast_observer() {}

public:

    virtual void construct( const sc_fxval_fast& );
    virtual void  destruct( const sc_fxval_fast& );
    virtual void      read( const sc_fxval_fast& );
    virtual void     write( const sc_fxval_fast& );

    static sc_fxval_fast_observer* (*default_observer) ();

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_observer
//
//  Abstract base class for fixed-point value type observers;
//  arbitrary precision.
// ----------------------------------------------------------------------------

inline
void
sc_fxval_observer::construct( const sc_fxval& )
{}

inline
void
sc_fxval_observer::destruct( const sc_fxval& )
{}

inline
void
sc_fxval_observer::read( const sc_fxval& )
{}

inline
void
sc_fxval_observer::write( const sc_fxval& )
{}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxval_fast_observer
//
//  Abstract base class for fixed-point value type observers;
//  limited precision.
// ----------------------------------------------------------------------------

inline
void
sc_fxval_fast_observer::construct( const sc_fxval_fast& )
{}

inline
void
sc_fxval_fast_observer::destruct( const sc_fxval_fast& )
{}

inline
void
sc_fxval_fast_observer::read( const sc_fxval_fast& )
{}

inline
void
sc_fxval_fast_observer::write( const sc_fxval_fast& )
{}


#endif

// Taf!
