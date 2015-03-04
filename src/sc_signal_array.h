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

    sc_signal_array.h -- one-dimensional arrays of signals of type T

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_SIGNAL_ARRAY_H
#define SC_SIGNAL_ARRAY_H

#include "assert.h"
#include "sc_signal.h"
#include "sc_array.h"

/******************************************************************************
    This class is actually managed by sc_signal_array<A>.  It is
    necessary because in sc_async, when we
    compute the dependency graph we need to be able to figure out the
    actual successors of the signals.

    This class keeps track of signals that are concatenated or are
    referenced via a `signal-array reference', i.e. a signal-array
    object that acts as a reference to another signal-array.  This
    class is used by decl_input() and decl_output(), in order that the
    dependencies and topological ordering may be properly computed.

    `original_signals' is an array of pointers to signals that combine
    to form the present signal.  `num_original_signals' is the number
    of elements in the array `original_signals'.  The members of this
    class are managed by sc_signal_array<A>.
    
******************************************************************************/

class sc_signal_array_base : public sc_signal_base {
    friend class sc_simcontext;
    friend class sc_aproc_process;

public:
    static const char* kind_string;
    const char* kind() const { return kind_string; }
    void print(ostream& os) const;
    void dump(ostream& os) const;

    void decl_sensitive_async( sc_async_process_handle async ) const;
    void decl_sensitive_aproc( sc_aproc_process_handle aproc ) const;
    int length() const { return _length; }
    bool event()const;

protected:
    sc_signal_array_base( sc_object* from );
    sc_signal_array_base( const char* nm, sc_object* from );

    /* These should never be called - update() should be called on atomic signals only */
    virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const;
    virtual void update();
    virtual void update_old();

    int _length;
    sc_signal_base** indiv_signals;
    bool free_indiv_signals;

private:
    /* Prohibit the use of copy constructor and assignment */
    sc_signal_array_base( const sc_signal_array_base& );
    void operator=( const sc_signal_array_base& );
};


/******************************************************************************/


/******************************************************************************
    sc_signal_array<A> defines a signal version of A, where A must be
    a sc_array of some type, or a descendent thereof, e.g.,
    sc_logic_vector and sc_bool_vector. The element type is determined
    by A::elemtype
******************************************************************************/

#ifdef USE_SC_SIGNAL_ARRAY_GNU_HACK 

/******************************************************************************
    GNU C++ has trouble with type names in template member function
    definitions, e.g., A::typename causes the parser to choke.  Thus, we
    define an additional level of inheritance to take care of that ...

    sc_signal_array_gnu<A,T> takes two arguments `A' and `T', where
    `A' is either an sc_array of `T', or a subclass of sc_array of
    `T'.  For example, `A' may be sc_array<int> and `T' may be int, or
    `A' may be sc_logic_vector, and `T' is sc_logics.

******************************************************************************/

template< class A, class T > class sc_signal_array_delay_gnu;

template< class A, class T >
class sc_signal_array_gnu
    : public sc_signal_array_base,  // Get the properties of a signal array
      public A                      // Get the properties of the underlying array.
{
public:
    typedef T elemtype;
    typedef sc_signal_array_gnu<A,T> this_type;


    /*
     * ANSI allows for what's known as covariance.  That is, the
     * return type of a virtual function in a derived class may be a
     * pointer or a reference to a derived class of the return type
     * declared in the base class.  In other words, if D is a base
     * class of B, and a virtual function B* foo() in my base class, I
     * am allowed to declare D* foo().  This may be nontrivial to
     * implement in compilers, and may incur unnecessary overhead.  A
     * simple way to get around this problem is to declare the desired
     * function as a nonvirtual function and call an auxiliary virtual
     * function, which is what we do here for the [] operator.  The []
     * operator calls the virtual auxiliary function subscript() to
     * get the result, and type-cast it if necessary.  Note that there
     * are two versions of [], one for const.  If the [] is invoked on
     * a const object, then the return type must be const also.
     */

#ifdef ANSI_VIRTUAL_RETURN_INHERITED_TYPE
    virtual sc_signal<T>& operator[](int i);
    virtual const sc_signal<T>& operator[](int i) const
    {
        return ((sc_signal_array_gnu<A,T>*) this)->operator[](i);
    }
#else
    // I suppose this is safer since covariance is difficult to
    // implement.
protected:
    virtual sc_signal<T>& subscript(int i);

public:
    DECLARE_SUBSCRIPT_OP(this_type, sc_signal<T>)

#endif

public:
    /*
     * The read() method returns the present value of the signal
     * array.  Note that given the way signal-arrays are implemented,
     * a simple type-cast will do.
     */
    const A& read() const { return (A&)(*this ); }
    int length() const { return A::length(); }


protected:
    /*
     * Here are the various forms of constructors. (Note that these
     * are declared protected, because sc_signal_array_gnu<A,T> serves
     * as the base class for sc_signal_array<A>.)  These have
     * corresponding counterparts in sc_array<> (q.v.).
     */
    explicit sc_signal_array_gnu( int len );
    sc_signal_array_gnu( const char* nm, int len );

    sc_signal_array_gnu( int len, const sc_signal_array_gnu<A,T>& ref );

    /* These forms of the constructor are used by resolved signal arrays */
    sc_signal_array_gnu( int len, sc_signal<T>* (*create_sig)(void *) );
    sc_signal_array_gnu( const char *nm, int len, sc_signal<T>* (*create_sig)(void *) );

protected:
    /* Prevent use of copy constructor */
    sc_signal_array_gnu( const sc_signal_array_gnu<A,T>& ref );

protected:
    virtual ~sc_signal_array_gnu();

public:
    // See definition of DECL_XWRITE_D() in "sc_signal.h"
    sc_signal_array_gnu<A,T>& write( const sc_array<T>& );
    sc_signal_array_gnu<A,T>& write( const T* );
    sc_signal_array_gnu<A,T>& operator=( const sc_array<T>& nv )
    {
        return write(nv);
    }
    sc_signal_array_gnu<A,T>& operator=( const sc_signal_array_gnu<A,T>& nv )
    {   /* Always remember to overload operator=() for the same type on RHS */
        return write( nv.read() );
    }
    sc_signal_array_gnu<A,T>& operator=( const T* nv )
    {
        return write(nv);
    }

protected:
    const sc_signal_array_gnu<A,T>* parent() const
    {
        return (sc_signal_array_gnu<A,T>*) A::parent();
    }


protected:
    /*
     * xwrite_help() are auxiliary functions for xwrite().  The first
     * form takes a pointer `get_val' to a function, which is called
     * from 0 to (length() - 1) as the third argument.  The first
     * argument to `get_val' is a reference to the location where the
     * result is to be stored.  The second argument to `get_val' is a
     * user-defined argument (that behaves like an array) to be
     * interpreted by `get_val' such that when `get_val' is called
     * with k as the third argument, the kth (from the left) value
     * from that array is retrieved and placed into the result.  The
     * third argument to xwrite_help() is the argument that is passed
     * to `get_val' as the second argument.  The action of
     * xwrite_help() is to write the new value obtained from `ar' into
     * the new-value buffer for the signal array.
     */
    void write_help( void (*get_val)(T& result, const void* ar, int k),
                     const void* ar, bool left_to_right );

    /*
     * The second form of xwrite_help() is `scalar' in that it writes
     * to only an element in the new-value buffer instead of the
     * entire array.  The second argument is the physical index, and
     * the third argument is the new value to be written to the
     * location of the new-value buffer indexed by the physical index.
     */
    void write_help( int phys_index, const T& );

    /*
     * vctor() is the "virtual constructor" that is invoked by the
     * sub() method.  See "sc_array.h" for an explanation of how the
     * subvector methods behave and how they are implemented.
     */
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;
    sc_signal_array_gnu( const sc_array_base* p, int l, int r );

    /*
     * get_signal() returns a _reference_ to a pointer to sc_signal<T>,
     * given a physical index.  An sc_signal_array keeps an
     * array of pointers to individual signals, called
     * `indiv_signals'.
     */
    sc_signal<T>* get_signal( int phys_index );
    const sc_signal<T>* get_signal( int phys_index ) const
    {
        return ((this_type*) this)->get_signal(phys_index);
    }

protected:
    /*
     * This macro provides the subvector methods: sub() and range(), which are
     * synonymous.  Note that there are both the const and non-const
     * versions.
     */
    DECLARE_PARTSEL(this_type)

protected:
    /*
     * These forms of the constructor are used for concatenation of
     * signal arrays.
     */
    sc_signal_array_gnu( sc_signal_array_gnu<A,T>* lhs, sc_signal_array_gnu<A,T>* rhs );
    sc_signal_array_gnu( sc_signal_array_gnu<A,T>* lhs, sc_signal<T>* rhs );
    sc_signal_array_gnu( sc_signal<T>* lhs, sc_signal_array_gnu<A,T>* rhs );
    sc_signal_array_gnu( sc_signal<T>* lhs, sc_signal<T>* rhs );

#ifdef DEBUG_SYSTEMC
public:
    void print(ostream& os) const;
    void dump(ostream& os) const;
#endif

public:
    void trace( sc_trace_file* tf ) const;

private:
    void init(sc_signal<T>* (*create_signal)(void *) = 0, void* arg = 0);
};


/*
 * sc_signal_array<A> is the real class that the user sees.  `A' must
 * be an sc_array of some kind, say T.  Note that sc_array<T> defines
 * the typedef `elemtype' to be T.
 */
template< class A >
class sc_signal_array : public sc_signal_array_gnu< A, typename A::elemtype >
{
public:
    /*
     *  Some useful typedefs
     */
#ifndef WIN32
    typedef typename A::elemtype T;
#else
    typedef sc_signal_array_gnu<A, typename A::elemtype>::elemtype T;
#endif

    typedef sc_signal_array<A> templ_type, this_type;

public:
    /*
     *  These are the usual forms of the constructor.
     */
    explicit sc_signal_array( int len )
        : sc_signal_array_gnu<A,T>( len )
    {
        /*EMPTY*/
    }
    sc_signal_array( const char* nm, int len )
        : sc_signal_array_gnu<A,T>( nm, len )
    {
        /*EMPTY*/
    }

    sc_signal_array( int len, const sc_signal_array<A>& ref )
        : sc_signal_array_gnu<A,T>( len, (const sc_signal_array_gnu<A,T>&) ref )
    {
        /*EMPTY*/
    }

    /* This form of the constructor is used by resolved signal arrays */
    sc_signal_array( int len, sc_signal<T>* (*create_sig)(void *) )
      : sc_signal_array_gnu<A,T>(len, create_sig)
    {
        /*EMPTY*/
    }

    /* This form of the constructor is used by resolved signal arrays */
    sc_signal_array( const char *nm, int len, sc_signal<T>* (*create_sig)(void *) )
      : sc_signal_array_gnu<A,T>(nm, len, create_sig)
    {
        /*EMPTY*/
    }

protected:
    /*
     * These shouldn't be called.  Declare them as protected and
     * assert if ever called.
     */
    sc_signal_array( const sc_signal_array<A>& ref )
        : sc_signal_array_gnu<A,T>( ref )
    {
        assert(false);
    }

public:
    virtual ~sc_signal_array()
    {
        /*EMPTY*/
    }

    sc_signal_array<A>& write( const sc_array<T>& nv )
    {
        return (sc_signal_array<A>&) sc_signal_array_gnu<A,T>::write(nv);
    }
    sc_signal_array<A>& write( const T* nv )
    {
        return (sc_signal_array<A>&) sc_signal_array_gnu<A,T>::write(nv);
    }
    sc_signal_array<A>& operator=( const sc_array<T>& nv )
    {
        return (sc_signal_array<A>&) sc_signal_array_gnu<A,T>::write(nv);
    }
    sc_signal_array<A>& operator=( const sc_signal_array<A>& nv )
    {
        return (sc_signal_array<A>&) sc_signal_array_gnu<A,T>::write( nv.read() );
    }
    sc_signal_array<A>& operator=( const T* nv )
    {
        return (sc_signal_array<A>&) sc_signal_array_gnu<A,T>::write(nv);
    }

private:
    /*
     * The write() methods and the operator[]() methods are inherited
     * from sc_signal_array_gnu<A,T>.  We need only redefine the
     * subvector methods.
     */

    DECLARE_PARTSEL(this_type)

protected:
    /*
     * The virtual constructor and the actual constructor called by
     * the virtual.  See "sc_array.h" for an explanation of this concept.
     */
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;
    sc_signal_array( const sc_array_base* p, int l, int r )
        : sc_signal_array_gnu<A,T>( p, l, r )
    {
        /* EMPTY */
    }

protected:
    /*
     * These forms of the constructor are used for concatenation.  The
     * actual work is done, in this case, in sc_signal_array_gnu.
     */
    sc_signal_array( sc_signal_array<A>* lhs, sc_signal_array<A>* rhs )
        : sc_signal_array_gnu<A,T>( lhs, rhs )
    {
        /*EMPTY*/
    }
    sc_signal_array( sc_signal_array<A>* lhs, sc_signal<T>* rhs )
        : sc_signal_array_gnu<A,T>( lhs, rhs )
    {
        /*EMPTY*/
    }
    sc_signal_array( sc_signal<T>* lhs, sc_signal_array<A>* rhs )
        : sc_signal_array_gnu<A,T>( lhs, rhs )
    {
        /*EMPTY*/
    }
    sc_signal_array( sc_signal<T>* lhs, sc_signal<T>* rhs )
        : sc_signal_array_gnu<A,T>( lhs, rhs )
    {
        /*EMPTY*/
    }

protected:
    /*
     * These are the auxiliary functions that dynamically create
     * concatenated signal-array objects.
     */
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );

public:
    /*
     * These are auxiliary functions that operator,() and cat() call
     * to obtain concatenated signal-arrays.
     */
    static sc_signal_array<A>* get_concat_array( sc_signal_array<A>* x, sc_signal_array<A>* y )
    {
        return (sc_signal_array<A>*) sc_array_base::get_concat_array( x, y, new_array_vv );
    }
    static sc_signal_array<A>* get_concat_array( sc_signal_array<A>* x, sc_signal<T>* y )
    {
        return (sc_signal_array<A>*) sc_array_base::get_concat_array( x, (void*) y, new_array_vs );
    }
    static sc_signal_array<A>* get_concat_array( sc_signal<T>* x, sc_signal_array<A>* y )
    {
        return (sc_signal_array<A>*) sc_array_base::get_concat_array( (void*) x, y, new_array_sv );
    }
    static sc_signal_array<A>* get_concat_array( sc_signal<T>* x, sc_signal<T>* y )
    {
        return (sc_signal_array<A>*) sc_array_base::get_concat_array( (void*) x, (void*) y, new_array_ss );
    }
};


// __GNUC__
#else
// ! __GNUC__

template< class A >
class sc_signal_array
    : public sc_signal_array_base,  // Get the properties of a signal array
      public A                      // Get the properties of the underlying array.
{
public:

    typedef typename A::elemtype T;
    typedef sc_signal_array<A> this_type;


    /*
     * ANSI allows for what's known as covariance.  That is, the
     * return type of a virtual function in a derived class may be a
     * pointer or a reference to a derived class of the return type
     * declared in the base class.  In other words, if D is a base
     * class of B, and a virtual function B* foo() in my base class, I
     * am allowed to declare D* foo().  This may be nontrivial to
     * implement in compilers, and may incur unnecessary overhead.  A
     * simple way to get around this problem is to declare the desired
     * function as a nonvirtual function and call an auxiliary virtual
     * function, which is what we do here for the [] operator.  The []
     * operator calls the virtual auxiliary function subscript() to
     * get the result, and type-cast it if necessary.  Note that there
     * are two versions of [], one for const.  If the [] is invoked on
     * a const object, then the return type must be const also.
     */

#ifdef ANSI_VIRTUAL_RETURN_INHERITED_TYPE
    virtual sc_signal<T>& operator[](int i);
    virtual const sc_signal<T>& operator[](int i) const
    {
        return ((this_type*) this)->operator[](i);
    }
#else
    // I suppose this is safer since covariance is difficult to
    // implement.
protected:
    virtual sc_signal<T>& subscript(int i);

public:
    DECLARE_SUBSCRIPT_OP(this_type, sc_signal<T>)

#endif

public:
    /*
     * The read() method returns the present value of the signal
     * array.  Note that given the way signal-arrays are implemented,
     * a simple type-cast will do.
     */
    const A& read() const { return static_cast<A&>(*this ); }


public:
    explicit sc_signal_array( int len );
    sc_signal_array( const char* nm, int len );

    sc_signal_array( int len, const this_type& ref );

    /* These form of the constructor are used by resolved signal arrays */
    sc_signal_array( int len, sc_signal<T>* (*create_sig)(void *) );
    sc_signal_array( const char *nm, int len, sc_signal<T>* (*create_sig)(void *) );

protected:
    /* Prevent use of copy constructor */
    sc_signal_array( const this_type& ref );

public:
    virtual ~sc_signal_array();

public:
    // See definition of DECL_XWRITE_D() in "sc_signal.h"
    this_type& write( const sc_array<T>& );
    this_type& write( const T* );
    this_type& operator=( const sc_array<T>& );
    this_type& operator=( const this_type& v )
    {
        return operator=( v.read() );
    }
    this_type& operator=( const T* );

protected:
    const this_type* parent() const
    {
        return (this_type*) A::parent();
    }

protected:
    /*
     * xwrite_help() are auxiliary functions for xwrite().  The first
     * form takes a pointer `get_val' to a function, which is called
     * from 0 to (length() - 1) as the third argument.  The first
     * argument to `get_val' is a reference to the location where the
     * result is to be stored.  The second argument to `get_val' is a
     * user-defined argument (that behaves like an array) to be
     * interpreted by `get_val' such that when `get_val' is called
     * with k as the third argument, the kth (from the left) value
     * from that array is retrieved and placed into the result.  The
     * third argument to xwrite_help() is the argument that is passed
     * to `get_val' as the second argument.  The action of
     * xwrite_help() is to write the new value obtained from `ar' into
     * the new-value buffer for the signal array.
     */
    void write_help( void (*get_val)(T& result, const void* ar, int k),
                     const void* ar, bool left_to_right );

    /*
     * The second form of xwrite_help() is `scalar' in that it writes
     * to only an element in the new-value buffer instead of the
     * entire array.  The second argument is the physical index, and
     * the third argument is the new value to be written to the
     * location of the new-value buffer indexed by the physical index.
     */
    void write_help( int phys_index, const T& );

    /*
     * vctor() is the "virtual constructor" that is invoked by the
     * sub() method.  See "sc_array.h" for an explanation of how the
     * subvector methods behave and how they are implemented.
     */
    virtual sc_array_base* vctor( const sc_array_base* p, int l, int r ) const;
    sc_signal_array( const sc_array_base* p, int l, int r );

    /*
     * get_signal() returns a _reference_ to a pointer to sc_signal<T>,
     * given a physical index.  An sc_signal_array keeps an
     * array of pointers to individual signals, called
     * `indiv_signals'.
     */
    sc_signal<T>* get_signal( int phys_index );
    const sc_signal<T>* get_signal( int phys_index ) const
    {
        return ((this_type*) this)->get_signal(phys_index);  // cast away constness first
    }

private:
    /*
     * This macro provides the subvector methods: sub() and range(), which are
     * synonymous.  Note that there are both the const and non-const
     * versions.
     */
    DECLARE_PARTSEL(this_type)

protected:
    /*
     * These forms of the constructor are used for concatenation of
     * signal arrays.
     */
    sc_signal_array( this_type* lhs, this_type* rhs );
    sc_signal_array( this_type* lhs, sc_signal<T>* rhs );
    sc_signal_array( sc_signal<T>* lhs, this_type* rhs );
    sc_signal_array( sc_signal<T>* lhs, sc_signal<T>* rhs );

private:
    void init(sc_signal<T>* (*create_signal)(void *) = 0, void* arg = 0);

#ifdef DEBUG_SYSTEMC
public:
    void print(ostream& os) const;
    void dump(ostream& os) const;
#endif
};

#endif  // __GNUC__

/*---------------------------------------------------------------------------*/

#include "sc_signal_array_defs.h_"

#endif



