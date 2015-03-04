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

    sc_templ.h -- A Template Implementation.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_TEMPL_H
#define SC_TEMPL_H

#include <stddef.h>
#include <new.h>

// This struct is provided for primarily two purposes:
// (1) To circumvent g++'s inability to compile complicated template
//     classes/functions (which led to internal errors).
// (2) To avoid the code explosion problem (not yet used).
//
// Later on, depending on what the functionality the users' template
// classes will use of the underlying type, various functions may need
// to be added, e.g., copy constructor, comparison operators, etc.

struct sc_templ_struct {
    int   size;                             // size of object
    void  (*ct)(void*);                     // default constructor
    void* (*ac)(size_t);                    // array new
    void  (*dt)(void*);                     // destructor
    void  (*ad)(void*);                     // array delete
    void  (*as)(void*, const void*);        // assignment operator

    sc_templ_struct( int _size,
                     void  (*_ct)(void*),
                     void* (*_ac)(size_t),
                     void  (*_dt)(void*),
                     void  (*_ad)(void*),
                     void  (*_as)(void*, const void*) )
    {
        size = _size;
        ct = _ct;
        ac = _ac;
        dt = _dt;
        ad = _ad;
        as = _as;
    }
};

template< class T >
class sc_templ_struct_fns {
public:
    static void  ct(void*);
    static void* ac(size_t);
    static void  dt(void*);
    static void  ad(void*);
    static void  as(void*, const void*);
    static sc_templ_struct* get_fns();
};

template< class T >
sc_templ_struct*
sc_templ_struct_fns<T>::get_fns()
{
    static sc_templ_struct fns( sizeof(T), ct, ac, dt, ad, as );
    return &fns;
}

template< class T >
void
sc_templ_struct_fns<T>::ct(void* x)
{
    new (x) T;                  // use placement new to achieve the effect
}

template< class T >
void*
sc_templ_struct_fns<T>::ac(size_t len)
{
    return new T[len];
}

template< class T >
void
sc_templ_struct_fns<T>::ad(void* x)
{
    delete[] ((T*) x);
}

template< class T >
void
sc_templ_struct_fns<T>::dt(void* x)
{
    ((T*) x)->~T();
}

template< class T >
void
sc_templ_struct_fns<T>::as(void* x, const void* y)
{
    *((T*) x) = *((T*) y);
}

#endif
