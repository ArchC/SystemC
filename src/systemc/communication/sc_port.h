/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_port.h -- Base classes of all port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_PORT_H
#define SC_PORT_H


#include "systemc/communication/sc_communication_ids.h"
#include "systemc/communication/sc_interface.h"
#include "systemc/kernel/sc_event.h"
#include "systemc/kernel/sc_object.h"
#include "systemc/kernel/sc_process.h"
#include "systemc/utils/sc_vector.h"
#include <typeinfo>

class sc_event_finder;
class sc_lambda_rand;
class sc_lambda_ptr;

struct sc_bind_info;


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  BEWARE: Ports can only be created and bound during elaboration.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// ----------------------------------------------------------------------------
//  CLASS : sc_port_base
//
//  Abstract base class for class sc_port_b.
// ----------------------------------------------------------------------------

class sc_port_base
: public sc_object
{
    friend class sc_module;
    friend class sc_port_registry;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;

public:

    // typedefs

    typedef sc_port_base this_type;

public:

    // get the first interface without checking for nil
    virtual       sc_interface* get_interface()       = 0;
    virtual const sc_interface* get_interface() const = 0;


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    // constructors
    explicit sc_port_base( int max_size_ );
    sc_port_base( const char* name_, int max_size_ );

    // destructor
    virtual ~sc_port_base();

    // bind interface to this port
    void bind( sc_interface& interface_ );

    // bind parent port to this port
    void bind( this_type& parent_ );

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& ) = 0;
    virtual int vbind( sc_port_base& ) = 0;

    // called by complete_binding (for internal use only)
    virtual void add_interface( sc_interface* ) = 0;
    virtual const char* if_typename() const = 0;

    // called by elaboration_done (does nothing)
    virtual void end_of_elaboration();

    // error reporting
    void report_error( int id, const char* add_msg = 0 ) const;

private:

    // called by class sc_module for positional binding
    int pbind( sc_interface& );
    int pbind( sc_port_base& );

    // called by the sc_sensitive* classes
    void make_sensitive( sc_thread_handle, sc_event_finder* = 0 ) const;
    void make_sensitive( sc_method_handle, sc_event_finder* = 0 ) const;

    // support methods
    int first_parent();
    void insert_parent( int );

    // called when elaboration is done
    void complete_binding();
    void elaboration_done();

private:

    sc_bind_info* m_bind_info;

private:

    // disabled
    sc_port_base();
    sc_port_base( const this_type& );
    this_type& operator = ( const this_type& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_port_registry
//
//  Registry for all ports.
//  FOR INTERNAL USE ONLY!
// ----------------------------------------------------------------------------

class sc_port_registry
{
    friend class sc_simcontext;

public:

    void insert( sc_port_base* );
    void remove( sc_port_base* );

    int size() const
        { return m_port_vec.size(); }

    void add_lambda_for_resolution( const sc_lambda_ptr& );

private:

    // constructor
    explicit sc_port_registry( sc_simcontext& simc_ );

    // destructor
    ~sc_port_registry();

    // called when elaboration is done
    void elaboration_done();

    void resolve_lambdas();
    void delete_unresolved_lambdas();

    static void replace_port( sc_port_registry*, sc_lambda_rand* );

private:

    sc_simcontext*              m_simc;
    sc_pvector<sc_port_base*>   m_port_vec;
    sc_pvector<sc_lambda_ptr*>* m_unresolved_lambdas;

private:

    // disabled
    sc_port_registry();
    sc_port_registry( const sc_port_registry& );
    sc_port_registry& operator = ( const sc_port_registry& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_port_b
//
//  Abstract base class for class sc_port.
// ----------------------------------------------------------------------------

template <class IF>
class sc_port_b
: public sc_port_base
{
public:

    // typedefs

    typedef sc_port_base  base_type;
    typedef sc_port_b<IF> this_type;

public:

    // bind an interface of type IF to this port

    void bind( IF& interface_ )
	{ base_type::bind( interface_ ); }

    void operator () ( IF& interface_ )
	{ base_type::bind( interface_ ); }


    // bind a parent port with type IF to this port

    void bind( this_type& parent_ )
	{ base_type::bind( parent_ ); }

    void operator () ( this_type& parent_ )
	{ base_type::bind( parent_ ); }


    // number of connected interfaces

    int size() const
	{ return m_interface_vec.size(); }


    // allow to call methods provided by the first interface
    IF* operator -> ();
    const IF* operator -> () const;


    // allow to call methods provided by interface at index
    IF* operator [] ( int index_ );
    const IF* operator [] ( int index_ ) const;


    // get the first interface without checking for nil

    virtual sc_interface* get_interface()
        { return m_interface; }

    virtual const sc_interface* get_interface() const
        { return m_interface; }

protected:

    // constructors

    explicit sc_port_b( int max_size_ )
	: base_type( max_size_ ), m_interface( 0 )
	{}

    sc_port_b( const char* name_, int max_size_ )
	: base_type( name_, max_size_ ), m_interface( 0 )
	{}


    // destructor (does nothing)

    virtual ~sc_port_b()
	{}


    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // called by complete_binding (for internal use only)
    virtual void add_interface( sc_interface* );
    virtual const char* if_typename() const;

    // disabled
    sc_port_b();
    sc_port_b( const this_type& );
    this_type& operator = ( const this_type& );

private:

    IF*             m_interface;	// first interface in interface vec
    sc_pvector<IF*> m_interface_vec;
};


// ----------------------------------------------------------------------------
//  CLASS : sc_port
//
//  Generic port class and base class for other port classes.
//  N is the maximum number of channels (with interface IF) that can be bound
//  to this port. N <= 0 means no maximum.
// ----------------------------------------------------------------------------

template <class IF, int N = 1>
class sc_port
: public sc_port_b<IF>
{
    // typdefs

    typedef sc_port_b<IF> base_type;
    typedef sc_port<IF,N> this_type;

public:

    // constructors

    sc_port()
	: base_type( N )
	{}

    explicit sc_port( const char* name_ )
	: base_type( name_, N )
	{}

    explicit sc_port( IF& interface_ )
	: base_type( N )
	{ base_type::bind( interface_ ); }

    sc_port( const char* name_, IF& interface_ )
	: base_type( name_, N )
	{ base_type::bind( interface_ ); }

    explicit sc_port( base_type& parent_ )
	: base_type( N )
	{ base_type::bind( parent_ ); }

    sc_port( const char* name_, base_type& parent_ )
	: base_type( name_, N )
	{ base_type::bind( parent_ ); }

    sc_port( this_type& parent_ )
	: base_type( N )
	{ base_type::bind( parent_ ); }

    sc_port( const char* name_, this_type& parent_ )
	: base_type( name_, N )
	{ base_type::bind( parent_ ); }


    // destructor (does nothing)

    virtual ~sc_port()
	{}


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

private:

    // disabled
    sc_port( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_port_b
//
//  Abstract base class for class sc_port.
// ----------------------------------------------------------------------------

// allow to call methods provided by the first interface

template <class IF>
inline
IF*
sc_port_b<IF>::operator -> ()
{
    if( m_interface == 0 ) {
	report_error( SC_ID_GET_IF_, "port is not bound" );
    }
    return m_interface;
}

template <class IF>
inline
const IF*
sc_port_b<IF>::operator -> () const
{
    if( m_interface == 0 ) {
	report_error( SC_ID_GET_IF_, "port is not bound" );
    }
    return m_interface;
}


// allow to call methods provided by interface at index

template <class IF>
inline
IF*
sc_port_b<IF>::operator [] ( int index_ )
{
    if( index_ < 0 || index_ >= size() ) {
	report_error( SC_ID_GET_IF_, "index out of range" );
    }
    return m_interface_vec[index_];
}

template <class IF>
inline
const IF*
sc_port_b<IF>::operator [] ( int index_ ) const
{
    if( index_ < 0 || index_ >= size() ) {
	report_error( SC_ID_GET_IF_, "index out of range" );
    }
    return m_interface_vec[index_];
}


// called by pbind (for internal use only)

template <class IF>
inline
int
sc_port_b<IF>::vbind( sc_interface& interface_ )
{
    IF* iface = DCAST<IF*>( &interface_ );
    if( iface == 0 ) {
	// type mismatch
	return 2;
    }
    base_type::bind( *iface );
    return 0;
}

template <class IF>
inline
int
sc_port_b<IF>::vbind( sc_port_base& parent_ )
{
    this_type* parent = DCAST<this_type*>( &parent_ );
    if( parent == 0 ) {
	// type mismatch
	return 2;
    }
    base_type::bind( *parent );
    return 0;
}


// called by complete_binding (for internal use only)

template <class IF>
inline
void
sc_port_b<IF>::add_interface( sc_interface* interface_ )
{
    IF* iface = dynamic_cast<IF*>( interface_ );
    assert( iface != 0 );
    m_interface_vec.push_back( iface );

    m_interface = m_interface_vec[0];
}

template <class IF>
inline
const char*
sc_port_b<IF>::if_typename() const
{
    return typeid( IF ).name();
}


// ----------------------------------------------------------------------------
//  CLASS : sc_port
//
//  Generic port class and base class for other port classes.
//  N is the maximum number of channels (with interface IF) that can be bound
//  to this port. N <= 0 means no maximum.
// ----------------------------------------------------------------------------

template <class IF, int N>
const char* const sc_port<IF,N>::kind_string = "sc_port";


#endif

// Taf!
