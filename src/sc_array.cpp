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

    sc_array.cpp -- Definitions for sc_array_base

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <stdio.h>
#include <assert.h>

#include "sc_iostream.h"
#include "sc_array.h"
#include "sc_simcontext.h"
#include "sc_array_concat.h"
#include "sc_externs.h"
#include "sc_exception.h"

struct sc_subarray_struct {
    int left;
    int right;
    sc_array_base* ar;
};

/****************************************************************************
    sc_array_base::sc_array_base( p, l, r )

    This form of the constructor is used for creating subarrays from
    parent parent arrays.  `p' is a pointer to the parent array, and
    `l' and `r' are the left- and right- bounds of the child array.
*****************************************************************************/
sc_array_base::sc_array_base( const sc_array_base* p, int l, int r )
    : m_parent(p)
{
    concat_manager = sc_get_curr_simcontext()->concat_manager;;

    if (l > r) {
        length_ = l - r + 1;
    } else {
        length_ = r - l + 1;
    }

    kids = new sc_plist< sc_subarray_struct* >;
    in_concat_manager = false;
}

sc_array_base::sc_array_base( int len, const sc_array_base& ref )
    : m_parent(0)
{
    concat_manager = sc_get_curr_simcontext()->concat_manager;
    length_ = len;

    check_length(ref.length());

    // This is a new array, so we'll keep a list of subarrays created
    // herefrom.
    kids = new sc_plist< sc_subarray_struct* >;
    in_concat_manager = false;
}

/****************************************************************************
    sc_array_base::sc_array_base( len )

    This is a constructor familiar to C programmers, viz. only the
    length is specified (`len') and the bounds are inferred as (0,
    len-1).
****************************************************************************/
sc_array_base::sc_array_base( int len )
    : m_parent(0)
{
    concat_manager = sc_get_curr_simcontext()->concat_manager;

    if (len < 0) len = 0;
    length_ = len;

    // No need to allocate kids if length is 0
    kids = len ? new sc_plist< sc_subarray_struct* > : 0;
    in_concat_manager = false;
}

/****************************************************************************
    sc_array_base::sc_array_base(y) : copy constructor

    Copy the basic information from one array to another.
****************************************************************************/
sc_array_base::sc_array_base( const sc_array_base& y )
    : m_parent(0)
{
    /* Note that we use the concat manager for the current simulation
       context.  This is so that we can transfer data between different
       simulation contexts */
    concat_manager = sc_get_curr_simcontext()->concat_manager;
    length_ = y.length_;
    kids = new sc_plist< sc_subarray_struct* >;
    in_concat_manager = false;
}

/****************************************************************************
    sc_array_base::operator=(y) : assignment operator

    The only thing we need to ensure is that the lengths are equal.
    Since no data exists in the abstract class sc_array_base, no
    copying actually takes place.
****************************************************************************/
sc_array_base&
sc_array_base::operator=( const sc_array_base& y )
{
    check_length(y.length());
    return *this;
}


/****************************************************************************
    sc_array_base::~sc_array_base() : destructor

    The predicate is_subarray() returns true if the array was created
    via the sub() or range() (synonymous) methods.  If that's the
    case, we don't need to do anything in the destructor since the
    subarray itself never allocated any data.  Otherwise, we need to
    walk through the list of kids and delete each subarray.  Then we
    delete the `kids' list.
****************************************************************************/
sc_array_base::~sc_array_base()
{
    if (kids) {
        // Go through the list of kids, and destroy each subarray.
        sc_plist<sc_subarray_struct*>::iterator it(kids);
        while( ! it.empty() ) {
            sc_subarray_struct* q = it.get();
            delete q->ar;
            delete q;         // Note the virtual destructor is invoked here
            it++;
        }
        delete kids;          // Infanticide
    }

    if (in_concat_manager && concat_manager)
        concat_manager->remove_constituent(this);
}

void
sc_array_base::check_bounds(int i) const
{
    if (i < 0 || i >= length()) {
        char msg[128];
        sprintf( msg, "array index %d out of bounds, valid bounds are (0,%d)",
                 i, length() - 1 );
        // sc_call_handler( SC_ERR_ARRAY_BOUND, msg );
        REPORT_ERROR(9001,msg);
    }
}

void
sc_array_base::check_bounds(int i, int j) const
{
    if (i < 0 || i >= length() || j < 0 || j >= length()) {
        char msg[128];
        sprintf( msg, "array indices (%d,%d) out of bounds, valid bounds are (0,%d)",
                 i, j, length() - 1 );
        //sc_call_handler( SC_ERR_ARRAY_BOUND, msg );
        REPORT_ERROR(9001,msg);
    }
}

void
sc_array_base::check_length(int len) const
{
    if (len != length())
        //sc_call_handler( SC_ERR_ARRAY_LENGTH, "array length mismatch" );
        REPORT_ERROR(9002,"array length mismatch");
}


/****************************************************************************
    sc_array_base::sub( i, j ) : subarray method

    Returns a reference to the subarray indexed by (i,j).  Note that a
    "virtual constructor" (vctor) is called to obtain an object of the
    desired type.  Hence, each subclass of `sc_array_base' must
    provide a constructor of the form:

        foo::foo( const sc_array_base* p, int i, int j )

    and a "virtual constructor" like this:

        sc_array_base*
        foo::vctor( const sc_array_base* p, int i, int j ) const
        {
            return new foo( p, i, j );
        }

    This ensures that the consistency of types.
****************************************************************************/
sc_array_base&
sc_array_base::sub( int i, int j )
{
    /* Sanity check */
    check_bounds( i, j );

    /* Trivial case: if the subarray is just me, return myself */
    if ((0 == j) && ((length() - 1) == i))
        return const_cast<sc_array_base&>(*this);

    // First, go through the kids list to see if the subarray has
    // already been created.  The linear search may not be the fastest
    // way, but it's much easier to implement.  Later on we may need
    // to do performance tuning to make things faster.
    sc_plist< sc_subarray_struct* >::iterator it(kids);
    while (! it.empty()) {
        sc_subarray_struct* q = it.get();
        if ((q->left == i) && (q->right == j))
            return *(q->ar);
        it++;
    }

    // Call "virtual constructor" to get the subarray of the right type
    sc_array_base* nar = vctor( this, i, j );
    sc_subarray_struct* suba = new sc_subarray_struct;
    suba->left = i;
    suba->right = j;
    suba->ar = nar;
    kids->push_front(suba);
    return *nar;
}

void
sc_array_base::print() const
{
    print(cout);
}

void
sc_array_base::print(ostream& os) const
{
    os << "[sc_array_base]";
}

void
sc_array_base::dump() const
{
    dump(cout);
}

void
sc_array_base::dump(ostream& os) const
{
    os << "length = " << length() << endl;
    if (m_parent) {
        os << "parent = " << m_parent << endl;
    }
    if (kids) {
        os << "subarrays = { ";
        sc_plist< sc_subarray_struct* >::iterator it(kids);
        while (! it.empty()) {
            sc_subarray_struct* k = *it;
            os << "(" << k->left << "," << k->right << ")" << endl;
            it++;
            if (! it.empty()) {
                os << ", ";
            }
        }
        os << "}" << endl;
    }
}

sc_array_base*
sc_array_base::get_concat_array(sc_array_base* x, sc_array_base* y,
                                sc_array_base* (*new_array_fn)(sc_array_base*, sc_array_base*), bool* is_new )
{
    assert( x->get_concat_manager() == y->get_concat_manager() );
    return x->get_concat_manager()->get_concat_array( x, y, new_array_fn, is_new );
}

sc_array_base*
sc_array_base::get_concat_array(sc_array_base* x, void* y,
                                sc_array_base* (*new_array_fn)(sc_array_base*, void*), bool* is_new )
{
    return x->get_concat_manager()->get_concat_array( x, y, new_array_fn, is_new );
}

sc_array_base*
sc_array_base::get_concat_array(void* x, sc_array_base* y,
                                sc_array_base* (*new_array_fn)(void*, sc_array_base*), bool* is_new )
{
    return y->get_concat_manager()->get_concat_array( x, y, new_array_fn, is_new );
}

sc_array_base* sc_array_base::get_concat_array(void* x, void* y,
                                               sc_array_base* (*new_array_fn)(void*, void*), bool* is_new )
{
    assert( false );
    return sc_get_curr_simcontext()->get_concat_manager()->get_concat_array( x, y, new_array_fn, is_new );
}

sc_array_base*
sc_array_base::get_concat_array(sc_array_base* x, const char* y,
                                sc_array_base* (*new_array_fn)(sc_array_base*, const char*), bool* is_new )
{
    return x->get_concat_manager()->get_concat_array( x, y, new_array_fn, is_new );
}

sc_array_base*
sc_array_base::get_concat_array(const char* x, sc_array_base* y,
                                sc_array_base* (*new_array_fn)(const char*, sc_array_base*), bool* is_new )
{
    return y->get_concat_manager()->get_concat_array( x, y, new_array_fn, is_new );
}


/****************************************************************************
    sc_array_init( vdata_p, vpdata_p, len, vinit, sizeT, ac_fn, as_fn )

    An auxiliary function for the initialization of sc_array<T>.  Why
    do we have this silly piece of code here?  Because g++ chokes
    (internal error) if we just put it in the template function
    definition.

    `vdata_p' is a pointer to the array of data elements.  `vpdata_p'
    is the pointer to the array of pointers to data elements.  `len'
    is the length of the array.  `vinit' is the pointer to
    initialization elements.  `sizeT' is the size of the element of
    type T.  ac_fn is a function that takes an argument `len' and then that
    performs the following operation:

        new T[len];

    as_fn takes two arguments u and v, and does the following:

        *((T*) u) = *((T*) v);

    See "sc_templ.h".
****************************************************************************/
void
sc_array_init(void** vdata_p, void*** vpdata_p, int len, const void* vinit, int sizeT,
              void* (*ac_fn)(size_t), void (*as_fn)(void*, const void*))
{
    *vdata_p = (*ac_fn)(len);
    if (vinit) {
        for (int i = 0; i < len; ++i)
            (*as_fn)( (void*) (((char*) *vdata_p) + i * sizeT),
                      (const void*) (((const char*) vinit) + i * sizeT) );
    }

    *vpdata_p = new void*[len];
    for (int i = 0; i < len; ++i)
        (*vpdata_p)[i] = (char*) *vdata_p + i * sizeT;
}
