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

    sc_array.h -- Base class of all arrays, and 1d arrays

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_ARRAY_H
#define SC_ARRAY_H

#include "sc_macros.h"
#include "sc_logic.h"
#include "sc_hash.h"
#include "sc_list.h"
#include "sc_exception.h"

#ifdef WIN32
#include "sc_cmnhdr.h"
#endif

#define DECLARE_SUBSCRIPT_OP(type, elemtype) \
    elemtype& operator[](int i); \
    const elemtype& operator[](int i) const { \
        return (const_cast<type*>(this)->operator[](i)); \
    }

#define DECLARE_PARTSEL(type)   \
    type& sub( int i, int j );  \
    type& range( int i, int j ) \
    {                           \
       if(i>=this->length() || j>=this->length() || i<0 || j<0)      \
         REPORT_ERROR(1000,"");   \
       return sub(i, j);        \
    }                           \
    const type& sub( int i, int j ) const { \
        return const_cast<type*>(this)->sub(i, j); \
    } \
    const type& range( int i, int j ) const { \
       if(i>=this->length() || j>=this->length() || i<0 || j<0)      \
         REPORT_ERROR(1000,"");   \
        return const_cast<type*>(this)->sub(i, j); \
    }


class sc_array_concat_manager;
struct sc_subarray_struct;

/*---------------------------------------------------------------------------*/


//
// `sc_array_base' provides the basic functionalities of all SystemC
// arrays and signal arrays.  These include: using logical indices
// (i.e. arbitrary array bounds rather than 0..(len-1)), query methods
// (left(), right(), length(), low(), high(), ascending(), and
// direction()), and subarray method (range()).  These are the public
// methods available for users of any classes derived from `sc_array_base'.
// `sc_array_base' also provides protected methods for derived classes
// to operate efficiently, mainly those that work with translation of
// logical indices to physical indices.  Logical indices are what the user
// of sc_array uses when accessing elements.  Physical indices are the actual
// offset from the data arrays.  In addition, `sc_array_base' manages
// the subarrays created by the range() method. 
//
class sc_array_base {
    friend class sc_array_concat_manager;

public:
    static void* operator new(size_t sz)             { return sc_mempool::allocate(sz); }
    static void  operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

        // Returns the length of the array
    int length() const;

        // Returns the subarray indexed by `i' and `j'.  `i' and `j'
        // may be arbitrary values within the bounds; i.e., the
        // direction of the subarray need not be the same as that of
        // the parent array.  The logical indices remain the same as
        // the parent.  Note that both the const and non-const versions.
    sc_array_base& sub( int i, int j );
    sc_array_base& range( int i, int j )
    {
        if(i>=length() || j>=length() || i<0 || j<0)
          REPORT_ERROR(1000,"");
        return sub(i,j);
    }
    const sc_array_base& sub( int i, int j ) const
    {
        return ((sc_array_base*) this)->sub(i,j);
    }
    const sc_array_base& range( int i, int j ) const
    {
        if(i>=length() || j>=length() || i<0 || j<0)
          REPORT_ERROR(1000,"");
        return ((sc_array_base*) this)->sub(i,j);
    }

        // Destructor - make it virtual, since there are virtual functions.
    virtual ~sc_array_base();

    void print() const;
    virtual void print(ostream& os) const;

    void dump() const;
    virtual void dump(ostream& os) const;

    void check_length(int len) const;

protected:
        // This does range check against "ref"
    sc_array_base( int len, const sc_array_base& ref );

        // Constructor - argument: length
    explicit sc_array_base( int len );  /* prevent implicit conversion */

        // Copy constructor
    sc_array_base( const sc_array_base& );

        // Assignment operator
    sc_array_base& operator=( const sc_array_base& );

        /* Some helper functions for use by subclasses */

protected:
        // Returns true if being a subarray, i.e. generated by a call
        // to sub()
    bool is_subarray() const;
        // The "virtual constructor" is used by sub() to generate a
        // subarray of the correct type.  All subtypes of
        // sc_array_base must provide this member function.  Here, the
        // first argument is the array from which a subarray is to be created.
    virtual sc_array_base* vctor(const sc_array_base*, int, int) const = 0;

        // The actual constructor used by the virtual constructor.
    sc_array_base( const sc_array_base* p, int l, int r );

    const sc_array_base* parent() const { return m_parent; }

    void check_bounds(int i) const;
    void check_bounds(int i, int j) const;

protected:
    sc_array_concat_manager* get_concat_manager() const { return concat_manager; }

    /* The following are simply wrappers for the functions in sc_array_concat.h */
    static sc_array_base* get_concat_array(sc_array_base* x, sc_array_base* y,
                                           sc_array_base* (*new_array_fn)(sc_array_base*, sc_array_base*),
                                           bool* is_new = 0);
    static sc_array_base* get_concat_array(sc_array_base* x, void* y,
                                           sc_array_base* (*new_array_fn)(sc_array_base*, void*),
                                           bool* is_new = 0);
    static sc_array_base* get_concat_array(void* x, sc_array_base* y,
                                           sc_array_base* (*new_array_fn)(void*, sc_array_base*),
                                           bool* is_new = 0);
    static sc_array_base* get_concat_array(void* x, void* y,
                                           sc_array_base* (*new_array_fn)(void*, void*),
                                           bool* is_new = 0);
    static sc_array_base* get_concat_array(sc_array_base* x, const char* y,
                                           sc_array_base* (*new_array_fn)(sc_array_base*, const char*),
                                           bool* is_new = 0);
    static sc_array_base* get_concat_array(const char* x, sc_array_base* y,
                                           sc_array_base* (*new_array_fn)(const char*, sc_array_base*),
                                           bool* is_new = 0);

private:
    const sc_array_base* m_parent; // parent of the array, 0 if not created from sub()
    sc_array_concat_manager* concat_manager;
    mutable sc_plist< sc_subarray_struct* >* kids; // subarrays of this array.
    int length_;
    bool in_concat_manager;
}; /* class sc_array_base */

/*---------------------------------------------------------------------------*/

//
//  <summary> One dimensional array of a scalar type </summary>
//
//  The class sc_array templatized on class T (a scalar type, i.e., one
//  that is not itself an sc_array) is a type representing
//  one-dimensional arrays.  
//

template< class A > class sc_2d;
template<class A, class T> class sc_channel_array_gnu;

template< class T > class sc_array : public sc_array_base
{
    friend class sc_array_concat_manager;
    friend class sc_2d< sc_array<T> >;
    friend class sc_channel_array_gnu<sc_array<T>, T>;

public:
         // A::elemtype will give the type of the elements in A
    typedef T elemtype;

        // Constructors
    explicit sc_array( int len );
    sc_array( int len, const sc_array<T>& ref );

        // Copy constructor
    sc_array( const sc_array<T>& y );

public:
        // Destructor.
    virtual ~sc_array();

        // Assigns a new value to the array from another sc_array<T> or from
        // a const T*.  Note that the first element of nv in either case
        // will correspond to the logical leftmost index of this array.
        // <group>
    sc_array<T>& operator=( const sc_array<T>& nv );
    sc_array<T>& operator=( const T* nv );
        // </group>

        // Returns true if this array is equal to rhs
        // <group>
    bool operator==( const sc_array<T>& rhs ) const;
    bool operator==( const T* rhs ) const;
        // </group>

        // Returns true if this array is not equal to rhs
        // <group>
    bool operator!=( const sc_array<T>& rhs ) const;
    bool operator!=( const T* rhs ) const;
        // </group>

        // Returns the element indexed by logical index i
    DECLARE_SUBSCRIPT_OP(sc_array<T>, T)


#ifdef DEBUG_SYSTEMC
    void print(ostream& os) const;
    void dump(ostream& os) const;
#endif

protected:

        // This form of the constructor is used by subclasses to supply
        // an array of pointers to data.  This is necessary, e.g., for
        // sc_signal_arrays.  The drawback is that accesses may be slower.
        // But hey - with concatenations, things will be slower anyways.
        // data_ptrs is created with new by the caller, and is deleted
        // by `this'.
    sc_array( T** data_ptrs, int len );
    void set_data_ptrs( T** data_ptrs );

        // Assigns a new value to the element with physical index phys_index
    void assign_help( int phys_index, const T& nv );

        // Assigns the data from new_value_array to this array's data array.
        // Physical indices are used for both arrays.
    void set_data_raw( const T* new_value_array );

public:
        // Returns a pointer to the data field indexed by physical index
    const T* get_data_ptr( int phys_index ) const;
    T* get_data_ptr( int phys_index );

protected:
        // The "virtual constructor" required by the base class.
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;

        // The actual constructor called by the virtual constructor.
    sc_array( const sc_array_base* p, int l, int r );

        // Store the values into a C++ array
    void to_cpp_array( T dst[] ) const;

        // This constructor is used when the underlying data array has
        // been allocated and initialized elsewhere, e.g.,
        // sc_bool_vector::operator&(sc_bool_vector&, sc_bool_vector&)

    sc_array( T* initialized_data, int len );
    
        // These constructors are used for concatenation
    sc_array( sc_array<T>* a1, sc_array<T>* a2 );
    sc_array( sc_array<T>* a1, T* a2 );
    sc_array( T* a1, sc_array<T>* a2 );
    sc_array( T* a1, T* a2 );

/******************************************************************************
                     NO PEEKING BEYOND THIS POINT
******************************************************************************/

protected:
    T* data;           // data array
    T** pdata;         // array of pointers to individual data elements
    bool use_pdata() const { return (data == 0); }
    bool overlapp(const sc_array<T>& other) const;

public:
    // Auxiliary functions for the concatenation operators
    static sc_array<T>* get_concat_array( sc_array<T>*, sc_array<T>* );
    static sc_array<T>* get_concat_array( sc_array<T>*, T* );
    static sc_array<T>* get_concat_array( T*, sc_array<T>* );
    static sc_array<T>* get_concat_array( T*, T* );

protected:
    // Auxiliary functions for get_concat_array()
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );

private:
    typedef const elemtype* asgn_type1;
    class asgn_type2{ };  sc_array<T>& operator=(const asgn_type2&) { return *this; }
    class asgn_type3{ };  sc_array<T>& operator=(const asgn_type3&) { return *this; }
    class asgn_type4{ };  sc_array<T>& operator=(const asgn_type4&) { return *this; }
    class asgn_type5{ };  sc_array<T>& operator=(const asgn_type5&) { return *this; }
    
    // Returns the subarray indexed by logical indices i and j.  Note that
    // i becomes the leftmost logical index of the subarray.
    DECLARE_PARTSEL(sc_array<T>)
};

/*---------------------------------------------------------------------------*/

template< class T >
extern ostream& operator<<(ostream& os, const sc_array<T>& ar);

#include "sc_array_defs.h_"

#endif
