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

    sc_pq.h -- A simple priority queue (which can be used to model multiple 
                 clocks).  From Cormen-Leiserson-Rivest, Ch.7.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_PQ_H
#define SC_PQ_H

#include <assert.h>

class sc_ppq_base {
public:
    typedef int (*compare_fn_t)( const void*, const void* );
    sc_ppq_base( int sz, compare_fn_t cmp );
    ~sc_ppq_base();
    void* top() const { return heap[1]; }
    void* extract_top();
    void insert( void* elem );
    int size() const { return heap_size; }
    bool empty() const { return (heap_size == 0); }

protected:
    int parent(int i) const { return i >> 1; }
    int left(int i) const { return i << 1; }
    int right(int i) const { return (i << 1) + 1; }
    void heapify(int i);

private:
    void** heap;
    int size_alloc;
    int heap_size;
    compare_fn_t compar;
};

//
//  <summary> Priority Queue </summary>
//
//  This class is a simple implementation of a priority queue based on
//  binary heaps.  The class is templatized on its data type.  A comparison
//  function needs to be supplied.
//
template< class T > class sc_ppq : public sc_ppq_base {
public:
        // Constructor - specify the maximum size of the queue and
        // give a comparison function.
    sc_ppq( int sz, compare_fn_t cmp )
        : sc_ppq_base(sz, cmp)
    {

    }
    ~sc_ppq() { }

        // Returns the value of the top element in the priority queue.
    T top() const { return (T) sc_ppq_base::top(); }
        // Pops the first element of the priority queue.
    T extract_top() { return (T) sc_ppq_base::extract_top(); }
        // Insert a new element to the priority queue.
    void insert( T elem ) { sc_ppq_base::insert((void*) elem); }

    // size() and empty() are inherited.
};

#endif
