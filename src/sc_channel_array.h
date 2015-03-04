/******************************************************************************
    Copyright (c) 1996-2001 CoWare, Inc.    ALL RIGHTS RESERVED

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

    sc_channel_array_defs.h_ -- definitions for the template class
                                sc_channel_array<A>

    Original Author: CoWare Inc.

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

// Based on sc_signal_array.h
// However, to keep matters simple in the first implementation, we are 
// not allowing [] and range() operation on channels. (It is not clear
// that such operations make sense anyway). Therefore, this is a simpler
// implementation than sc_signal_array

#ifndef SC_CHANNEL_ARRAY_H
#define SC_CHANNEL_ARRAY_H

#include "assert.h"
#include "sc_signal.h"
#include "sc_array.h"
#include "sc_channel.h"

// sc_signal_base is the updateable object, so we inherit from it

class sc_channel_array_base : public sc_signal_base {
    friend class sc_simcontext;

public:
    static const char* kind_string;
    const char* kind() const { return kind_string; }

#ifdef DEBUG_SYSTEMC
    void print(ostream& os) const;
    void dump(ostream& os) const;
#endif

    void decl_sensitive_async( sc_async_process_handle async ) const;
    void decl_sensitive_aproc( sc_aproc_process_handle aproc ) const;
    int length() const { return _length; }

protected:
    sc_channel_array_base( sc_object* from );
    sc_channel_array_base( const char* nm, sc_object* from );

    // These should never be called - update() should be called on 
    // atomic channels only
    virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const;
    virtual void update();
    virtual void update_old();

    int _length;
    sc_signal_base** indiv_channels;
    bool free_indiv_channels;
};


/******************************************************************************
    sc_channel_array<A> defines a channel version of A, where A must be
    a sc_array of some type, or a descendent thereof, e.g.,
    sc_logic_vector or sc_bool_vector. The element type is determined
    by A::elemtype
******************************************************************************/


/******************************************************************************
    sc_channel_array_gnu<A,T> takes two arguments `A' and `T', where
    `A' is either an sc_array of `T', or a subclass of sc_array of
    `T'.  For example, `A' may be sc_array<int> and `T' may be int, or
    `A' may be sc_logic_vector, and `T' is sc_logic.
******************************************************************************/

template <class A> class sc_channel_array;

template< class A, class T >
class sc_channel_array_gnu : public sc_channel_array_base, // Get the properties of a channel array base
			 public sc_channel_friend<T> // Get protected member functions of scalar channel
{
public:
    typedef sc_channel_array_gnu<A,T> this_type;

public:
    /*
     *
     * The read() method returns the present value of the channel array
     */
    const A& read() const;
    operator A() const {return read();}

    // Checks for empty or full condition
    bool is_empty() const ;
    bool is_full() const ;
    
public:
    sc_channel_array_gnu( int len, int bsz );
    sc_channel_array_gnu( const char* nm, int len, int bsz );

protected:
    /* Prevent use of copy constructor */
    sc_channel_array_gnu( const sc_channel_array_gnu<A,T>& y );

protected:
    virtual ~sc_channel_array_gnu();

public:
    // The write methods
    sc_channel_array_gnu<A,T>& write( const sc_array<T>& );
    sc_channel_array_gnu<A,T>& write( const T* );
    sc_channel_array_gnu<A,T>& nb_write( const sc_array<T>& );
    sc_channel_array_gnu<A,T>& nb_write( const T* );
    sc_channel_array_gnu<A,T>& operator=( const sc_array<T>& v) { return write(v); }
    sc_channel_array_gnu<A,T>& operator=( const T* v) { return write(v); }

private:
    sc_channel_array_gnu<A,T>& operator=( const sc_channel_array_gnu<A, T>& v) { return *this; }

public:
    /*
     * get_channel() returns a pointer to sc_channel<T>,
     * given a physical index.  An sc_channel_array_gnu keeps an
     * array of pointers to individual channels, called
     * `indiv_channels'.
     */
    sc_channel<T>* get_channel( int phys_index ) const; 
    int length() const { return _length; }

public:
    void trace( sc_trace_file* tf ) const;

private:
    void init(const char *nm, const int bsz);
    A cur_val_array;
};

/*******************************************************************************/

/* 
 * sc_channel_array<A> is the real class that the user sees. `A' must be an
 * sc_array of some kind, say T. Note that sc_array<T> defines the typedef
 * elemtype to be T
 */

template< class A >
class sc_channel_array : public sc_channel_array_gnu<A, typename A::elemtype>
{
public:
  typedef typename A::elemtype T;
  
    /*
     * The read() methods are inherited as is from sc_channel_array_gnu
     */

public:
    sc_channel_array( int len, int bsz = 0 )
      : sc_channel_array_gnu<A,T>(len, bsz)
    {
      /* All work done in the sc_channel_array_gnu constructor */
    }
    sc_channel_array( const char* nm, int len, int bsz = 0 )
      : sc_channel_array_gnu<A,T>(nm, len, bsz)
    {
      /* All work done in the sc_channel_array_gnu constructor */
    }

protected:
    /* Prevent use of copy constructor */
    sc_channel_array( const sc_channel_array<A>& y ) 
      : sc_channel_array_gnu<A,T>( y )
    {
      assert(false);
    }

public:
    virtual ~sc_channel_array()
    {
      /* Intentionally blank, all work done in ~sc_channel_array_gnu */
    }

public:
    // The write methods
    sc_channel_array<A>& write( const sc_array<T>& nv) 
    {
      return (sc_channel_array<A>&) sc_channel_array_gnu<A,T>::write(nv);
    }
    sc_channel_array<A>& write( const T* nv)
    {
      return (sc_channel_array<A>&) sc_channel_array_gnu<A,T>::write(nv);
    }

    sc_channel_array<A>& operator=( const sc_array<T>& nv) 
    { 
      return (sc_channel_array<A>&) sc_channel_array_gnu<A,T>::write(nv); 
    }
    sc_channel_array<A>& operator=( const T* nv) 
    { 
      return (sc_channel_array<A>&) sc_channel_array_gnu<A,T>::write(nv); 
    }

    sc_channel_array<A>& nb_write( const sc_array<T>& nv) 
    {
      return (sc_channel_array<A>&) sc_channel_array_gnu<A,T>::nb_write(nv);
    }
    sc_channel_array<A>& nb_write( const T* nv)
    {
      return (sc_channel_array<A>&) sc_channel_array_gnu<A,T>::nb_write(nv);
    }

private:
    sc_channel_array<A>& operator=( const sc_channel_array<A>& nv) 
    { 
      return *this;
    }
    
};


/*---------------------------------------------------------------------------*/

#include "sc_channel_array_defs.h_"

#endif
