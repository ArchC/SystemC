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

    sc_vector.h -- Simple implementation of a vector class

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_VECTOR_H
#define SC_VECTOR_H

extern "C" {
  typedef int (*CFT)(const void*, const void*);
} 

class sc_pvector_base {
protected:
    typedef void** iterator;
    typedef const void** const_iterator;

    sc_pvector_base(int alloc = 10);
    sc_pvector_base(const sc_pvector_base& rhs);
    ~sc_pvector_base();

    int size() const { return sz; }
    iterator begin() const { return data; }
    iterator end() const { return data + sz; }

    sc_pvector_base& operator=(const sc_pvector_base& rhs);

    void** raw_data() { return data; }
    void* const* raw_data() const { return data; }

    void*& operator[](int i);
    const void*& operator[](int i) const;
    void*& fetch(int i) { return data[i]; }
    const void*& fetch(int i) const { return (const void*&)data[i]; }
    void push_back(void* item);

    void erase_all();

    void sort( CFT compar );

    
    /* Change the ith item to new_item */
    void put(void* new_item, int i) { data[i] = new_item; }
    void decr_count() { sz ? sz-- : 0; }
    void decr_count(int k) {sz -= k; if(sz < 0) sz = 0; }

private:
    void resize(int new_sz);

    int n_alloc;
    int sz;
    void** data;
};

template< class T >
class sc_pvector : public sc_pvector_base {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    sc_pvector(int alloc = 10) : sc_pvector_base(alloc) { }
    sc_pvector(const sc_pvector<T>& rhs) : sc_pvector_base(rhs) { }
    ~sc_pvector() { }

    int size() const { return sc_pvector_base::size(); }
    iterator begin() const { return (iterator) sc_pvector_base::begin(); }
    iterator end() const { return (iterator) sc_pvector_base::end(); }

    sc_pvector<T>& operator=(const sc_pvector<T>& rhs)
    {
        return (sc_pvector<T>&) sc_pvector_base::operator=(rhs);
    }

    T& operator[](int i) { return (T&) sc_pvector_base::operator[](i); }
    const T& operator[](int i) const { return (const T&) sc_pvector_base::operator[](i); }
    T& fetch(int i) { return (T&) sc_pvector_base::fetch(i); }
    const T& fetch(int i) const { return (const T&) sc_pvector_base::fetch(i); }

    T* raw_data()             { return (T*) sc_pvector_base::raw_data(); }
    const T* raw_data() const { return (const T*) sc_pvector_base::raw_data(); }

    void push_back(T item) { sc_pvector_base::push_back((void*) item); }

    void erase_all() { sc_pvector_base::erase_all(); }

    void sort(CFT compar) { sc_pvector_base::sort(compar); }
    
    /* These methods have been added from Ptr_Array */
    
    void put(T item, int i) { sc_pvector_base::put((void*)item,i); }
    void decr_count() { sc_pvector_base::decr_count(); }
    void decr_count(int k) { sc_pvector_base::decr_count(k); }
};

#endif
