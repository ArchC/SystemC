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

    sc_vector.cpp -- Simple implementation of a vector class

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <string.h>
#include <stdlib.h>
#if defined(__BCPLUSPLUS__)
#include <mem>
#pragma hdrstop
using std::memcpy;
#endif
#include "sc_vector.h"



sc_pvector_base::sc_pvector_base(int alloc)
{
    n_alloc = alloc;
    sz = 0;
    data = new void*[n_alloc];
}

sc_pvector_base::sc_pvector_base(const sc_pvector_base& rhs)
{
    n_alloc = rhs.n_alloc;
    sz = rhs.sz;
    data = new void*[n_alloc];
    memcpy( data, rhs.data, rhs.sz * sizeof(void*) );
}

sc_pvector_base::~sc_pvector_base()
{
    delete[] data;
}

sc_pvector_base&
sc_pvector_base::operator=(const sc_pvector_base& rhs)
{
    if (&rhs == this)
        return *this;
    if (n_alloc < rhs.n_alloc) {
        delete[] data;
        data = new void*[rhs.n_alloc];
        n_alloc = rhs.n_alloc;
    }
    sz = rhs.sz;
    memcpy( data, rhs.data, rhs.sz * sizeof(void*) );
    return *this;
}

void*&
sc_pvector_base::operator[](int i)
{
    if (i >= sz) {
        if (i + 1 > n_alloc)
            resize(i + 11);
        sz = i + 1;
    }
    return data[i];
}

const void*&
sc_pvector_base::operator[](int i) const
{
   return (const void*&)(((sc_pvector_base*) this)->operator[](i));
}

void
sc_pvector_base::push_back(void* item)
{
    if (sz + 1 > n_alloc) {
        resize(sz + 11);
    }
    sz += 1;
    data[sz - 1] = item;
}

void
sc_pvector_base::resize(int new_sz)
{
    if (new_sz <= n_alloc) return;
    n_alloc = new_sz;
    void** new_data = new void*[new_sz];
    memcpy( new_data, data, sz * sizeof(void*) );
    delete[] data;
    data = new_data;
}

void
sc_pvector_base::sort( CFT compar )
{
    qsort( data, sz, sizeof(void*), compar );
}

void
sc_pvector_base::erase_all()
{
    sz = 0;
}
