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

    sc_pq.cpp - Simple heap implementation of priority queue

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "sc_pq.h"

sc_ppq_base::sc_ppq_base( int sz, int (*cmp)(const void*, const void*))
    : size_alloc(sz), heap_size(0), compar(cmp)
{
    heap = new void*[sz+1];
}

sc_ppq_base::~sc_ppq_base()
{
    delete[] heap;
}

void*
sc_ppq_base::extract_top()
{
    assert( heap_size > 0 );
    void* topelem = heap[1];
    heap[1] = heap[heap_size];
    heap_size--;
    heapify(1);
    return topelem;
}

void
sc_ppq_base::insert( void* elem )
{
    heap_size++;
    int i = heap_size;

    // Resize the heap in case there's not enough memory
    if (heap_size > size_alloc) {
        size_alloc += size_alloc / 2;
        void** new_heap = new void*[size_alloc+1];
        for (int j = 1; j < heap_size; ++j) {
            new_heap[j] = heap[j];
        }
        delete[] heap;
        heap = new_heap;
    }

    while ((i > 1) && (compar( heap[parent(i)], elem ) < 0)) {
        heap[i] = heap[parent(i)];
        i = parent(i);
    }
    heap[i] = elem;
}

void
sc_ppq_base::heapify( int i )
{
    int l;
    while (l = left(i), l <= heap_size) {
        int largest = (compar( heap[l], heap[i] ) > 0) ? l : i;

        int r = right(i);
        if ((r <= heap_size) && (compar( heap[r], heap[largest] ) > 0))
            largest = r;

        if (largest != i) {
            void* tmp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = tmp;
            i = largest;
        }
        else
            break;
    }
}
