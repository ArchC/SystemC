/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_signal_signed.h -- The sc_signal<sc_dt::sc_bigint<W> > definitions.

  Original Author: Andy Goodrich, Forte Design Systems, 2002-10-22

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* 
$Log: scx_signal_signed.h,v $
Revision 1.4  2011/08/15 17:18:21  acg
 Andy Goodrich: fix blown inclusion of Torsten's edit.

Revision 1.3  2011/08/15 16:43:24  acg
 Torsten Maehne: changes to remove unused argument warnings.

Revision 1.2  2011/06/28 21:23:02  acg
 Andy Goodrich: merging of SCV tree.

Revision 1.1.1.1  2006/12/15 20:20:03  acg
SystemC 2.3

Revision 1.4  2006/10/23 19:40:35  acg
 Andy Goodrich: added an explicit dynamic cast to keep gcc 4.x happy.

Revision 1.3  2006/03/21 01:31:48  acg
 Andy Goodrich: changed over to sc_get_current_process_b() from
 sc_get_current_process_base() since the function's name changed.

Revision 1.2  2005/12/26 20:11:14  acg
Fixed up copyright.

Revision 1.1.1.1  2005/12/19 23:16:42  acg
First check in of SystemC 2.1 into its own archive.

Revision 1.29  2005/09/15 23:01:52  acg
Added std:: prefix to appropriate methods and types to get around
issues with the Edison Front End.

Revision 1.28  2005/07/30 03:44:11  acg
Changes from 2.1.

Revision 1.27  2005/05/09 17:17:12  acg
Changes from 2.1.

Revision 1.26  2005/05/08 19:04:06  acg
Fix bug in concat_set(int64,off). Other changes from 2.1 examples usage.

Revision 1.25  2005/05/03 20:05:16  acg
Prefixed with sc_dt some sc_signed instances that were missed.

Revision 1.24  2005/05/03 19:50:20  acg
Name space version.

Revision 1.22  2005/03/21 22:31:32  acg
Changes to sc_core namespace.

Revision 1.21  2005/01/10 17:51:58  acg
Improvements.

Revision 1.20  2004/11/09 00:11:27  acg
Added support for sc_generic_base<T> in place of sc_concatref. sc_concatref
now is derived from sc_generic_base<sc_concatref>.

Revision 1.19  2004/09/27 21:01:59  acg
Andy Goodrich - Forte Design Systems, Inc.
   - This is specialized signal support that allows better use of signals
     and ports whose target value is a SystemC native type.

*/


#if !defined(SC_SIGNAL_SIGNED_H)
#define SC_SIGNAL_SIGNED_H

#if ( !defined(_MSC_VER) || _MSC_VER > 1200 )
#    define SC_TEMPLATE template<int W>
#else
#    define SC_TEMPLATE template<> template<int W>
#endif

// FORWARD REFERENCES AND USINGS:

using sc_dt::int64;

namespace sc_core {

class sc_signed_sigref;

//==============================================================================
// CLASS sc_signed_part_if
//
// This class provides generic access to part selections for signals whose
// data type is sc_dt::sc_bigint<W>. This class serves as the base class for the
// sc_dt::sc_bigint<W> specialization of the sc_signal_in_if<T> class. The methods
// in this class may be over-ridden individually, those that are not overridden
// will produce an error message when called. The methods are used by the 
// sc_bigint_sigref class.
//
// Notes:
//   (1) Descriptions of the methods and operators in this class appear with
//       their implementations in sc_signal<sc_dt::sc_bigint<W> >.
//==============================================================================
class sc_signed_part_if : virtual public sc_interface {
  protected:
	// constructor:
  	sc_signed_part_if() {}

  public:
    // perform a part read.
	virtual sc_dt::sc_signed* part_read_target();
  	virtual sc_dt::sc_signed read_part( int left, int right ) const;

    // perform a part write.
	virtual sc_signed_sigref& select_part( int left, int right );
    virtual void write_part( sc_dt::int64 v, int left, int right );
    virtual void write_part( sc_dt::uint64 v, int left, int right );
    virtual void write_part( const sc_dt::sc_signed& v, int left, int right );
    virtual void write_part( const sc_dt::sc_unsigned& v, int left, int right );


  private:
  	sc_signed_part_if( const sc_signed_part_if& );
  	sc_signed_part_if& operator = ( const sc_signed_part_if& );
};


//==============================================================================
// CLASS sc_signal_in_if<sc_dt::sc_bigint<W> >
//
// This is the class specializations for the sc_signal_in_if<T> class to
// provide additional features for sc_signal instances whose template is
// sc_dt::sc_bigint<W>, including part access. 
//
// Notes:
//   (1) Descriptions of the methods and operators in this class appear with
//       their implementations in sc_signal<sc_dt::sc_bigint<W> >.
//==============================================================================
template< int W >
class sc_signal_in_if<sc_dt::sc_bigint<W> > : public sc_signed_part_if {
	friend class sc_signed_sigref;
  public:
    typedef sc_signal_in_if<sc_dt::sc_bigint<W> > this_type;

    // get the value changed event
    virtual const sc_event& value_changed_event() const = 0;


    // read the current value
    virtual const sc_dt::sc_bigint<W>& read() const = 0;

    // get a reference to the current value (for tracing)
    virtual const sc_dt::sc_bigint<W>& get_data_ref() const = 0;


    // was there a value changed event?
    virtual bool event() const = 0;

  protected: 
    // constructor
    sc_signal_in_if()
    {}

  private: // disabled 
    sc_signal_in_if( const this_type& );
    this_type& operator = ( const this_type& );
};

//=============================================================================
//  CLASS : sc_signed_sigref
//
//  Proxy class for sc_signal_uint bit and part selection.
//=============================================================================
class sc_signed_sigref : public sc_dt::sc_signed_subref_r
{
  public:
    sc_signed_sigref() : sc_dt::sc_signed_subref_r() {}
    virtual ~sc_signed_sigref() {}
    virtual void concat_set(sc_dt::int64 src, int low_i);
    virtual void concat_set(const sc_dt::sc_signed& src, int low_i);
    virtual void concat_set(const sc_dt::sc_unsigned& src, int low_i);
    virtual void concat_set(const sc_dt::sc_lv_base& src, int low_i);
    virtual void concat_set(sc_dt::uint64 src, int low_i);

  public:
    inline void initialize( sc_signed_part_if* if_p, int left_, int right_ );

  public:
    inline void operator = ( sc_dt::uint64 v );
    inline void operator = ( const char* v );
    inline void operator = ( unsigned long v );
    inline void operator = ( long v );
    inline void operator = ( unsigned int v );
    inline void operator = ( int v );
    inline void operator = ( sc_dt::int64 v );
    inline void operator = ( double v );
    inline void operator = ( const sc_signed_sigref& v );
    template<typename T>
    inline void operator = ( const sc_dt::sc_generic_base<T>& v );
    inline void operator = ( const sc_dt::sc_signed& v );
    inline void operator = ( const sc_dt::sc_unsigned& v );
    inline void operator = ( const sc_dt::sc_bv_base& v );
    inline void operator = ( const sc_dt::sc_lv_base& v );

  public:
    static sc_vpool<sc_signed_sigref> m_pool; // Pool of objects to use.

  protected:
    sc_signed_part_if*                m_if_p; // Target for selection.

  private:

    // disabled
    sc_signed_sigref( const sc_signed_sigref& a );
};


//==============================================================================
// CLASS sc_signal<sc_dt::sc_bigint<W> > 
//
// This class implements a signal whose value acts like an sc_dt::sc_bigint<W> data
// value. This class is a specialization of the generic sc_signal class to 
// implement tailored support for the sc_dt::sc_bigint<W> class.
//
// Notes:
//   (1) Descriptions of the methods and operators in this class appear with
//       their implementations.
//==============================================================================
SC_TEMPLATE
class sc_signal<sc_dt::sc_bigint<W> > : 
    public sc_dt::sc_bigint<W>, 
	public sc_prim_channel,
    public sc_signal_inout_if<sc_dt::sc_bigint<W> >
{
  public: // typedefs
    typedef sc_signal<sc_dt::sc_bigint<W> > this_type;

  public: // constructors and destructor:
    inline sc_signal();
    explicit inline sc_signal(const char* name_);
    virtual inline ~sc_signal();

  public: // base methods:
    inline bool base_event() const;
    inline const sc_dt::sc_bigint<W>& base_read() const;
    inline const sc_event& base_value_changed_event() const;
    inline void base_write( sc_dt::int64 value );
    inline void base_write( sc_dt::uint64 value );
    inline void base_write( const sc_dt::sc_signed& value );
    inline void base_write( const sc_dt::sc_unsigned& value );

  public: // sc_prim_channel virtual methods:
    virtual inline const char* kind() const;
    virtual inline void update();

  public: // sc_interface virtual methods:
    virtual inline const sc_event& default_event() const;
    virtual inline void register_port( 
		sc_port_base& port_, const char* if_typename_ );

  public: // sc_signed_part_if virtual methods:
    virtual inline sc_dt::sc_signed* part_read_target();
    virtual inline sc_dt::sc_signed read_part(int left, int right) const;
	virtual sc_signed_sigref& select_part( int left, int right );
	virtual inline void write_part( sc_dt::int64 v, int left, int right );
	virtual inline void write_part( sc_dt::uint64 v, int left, int right );
	virtual inline void write_part( const sc_dt::sc_signed& v, int left, int right );
	virtual inline void write_part(const sc_dt::sc_unsigned& v, int left, int right);

  public: // interface virtual methods:
    virtual inline bool event() const;
    virtual inline const sc_dt::sc_bigint<W>& get_data_ref() const;
    virtual inline sc_signal<sc_dt::sc_bigint<W> >& get_signal();
    virtual inline const sc_dt::sc_bigint<W>& read() const;
    virtual inline const sc_event& value_changed_event() const;
    virtual inline void write( const sc_in<sc_dt::sc_bigint<W> >& value );
    virtual inline void write( const sc_inout<sc_dt::sc_bigint<W> >& value );
    virtual inline void write( const sc_dt::sc_bigint<W>& value );

  public: // part selections:
  	inline sc_signed_sigref& operator () ( int left, int right );
	// #### Need to add bit() and range()
  	inline sc_signed_sigref& operator [] ( int bit );

  public: // operators:
    inline void operator = ( const this_type& new_val );
    inline void operator = ( const char* new_val );
    inline void operator = ( sc_dt::uint64 new_val );
    inline void operator = ( sc_dt::int64 new_val );
    inline void operator = ( int new_val );
    inline void operator = ( long new_val );
    inline void operator = ( short new_val );
    inline void operator = ( unsigned int new_val );
    inline void operator = ( unsigned long new_val );
    inline void operator = ( unsigned short new_val );
    template<typename T>
    inline void operator = ( const sc_dt::sc_generic_base<T>& new_val );
    inline void operator = ( const sc_dt::sc_signed& new_val );
    inline void operator = ( const sc_dt::sc_unsigned& new_val );
    inline void operator = ( const sc_dt::sc_bv_base& new_val );
    inline void operator = ( const sc_dt::sc_lv_base& new_val );

    // concatenation methods (we inherit length and gets from sc_dt::sc_bigint<W>):

    virtual inline void concat_set(sc_dt::int64 src, int low_i);
    virtual inline void concat_set(const sc_dt::sc_lv_base& src, int low_i);
    virtual inline void concat_set(const sc_dt::sc_signed& src, int low_i);
    virtual inline void concat_set(const sc_dt::sc_unsigned& src, int low_i);
    virtual inline void concat_set(sc_dt::uint64 src, int low_i);

  protected: // debugging methods:
    // #### void check_port();
	void check_writer();

  private: // Disabled operations that sc_dt::sc_bigint<W> supports:
    sc_dt::sc_signed& operator ++ ();          // prefix
    const sc_dt::sc_signed& operator ++ (int); // postfix
    sc_dt::sc_signed& operator -- ();          // prefix
    const sc_dt::sc_signed& operator -- (int); // postfix
    sc_dt::sc_signed& operator += (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator += (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator += (sc_dt::int64               ); 
    sc_dt::sc_signed& operator += (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator += (long                ); 
    sc_dt::sc_signed& operator += (unsigned long       ); 
    sc_dt::sc_signed& operator += (int                 );
    sc_dt::sc_signed& operator += (unsigned int        );
    sc_dt::sc_signed& operator -= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator -= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator -= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator -= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator -= (long                ); 
    sc_dt::sc_signed& operator -= (unsigned long       ); 
    sc_dt::sc_signed& operator -= (int                 );
    sc_dt::sc_signed& operator -= (unsigned int        );
    sc_dt::sc_signed& operator *= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator *= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator *= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator *= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator *= (long                ); 
    sc_dt::sc_signed& operator *= (unsigned long       ); 
    sc_dt::sc_signed& operator *= (int                 );
    sc_dt::sc_signed& operator *= (unsigned int        );
    sc_dt::sc_signed& operator /= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator /= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator /= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator /= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator /= (long                ); 
    sc_dt::sc_signed& operator /= (unsigned long       ); 
    sc_dt::sc_signed& operator /= (int                 );
    sc_dt::sc_signed& operator /= (unsigned int        );
    sc_dt::sc_signed& operator %= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator %= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator %= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator %= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator %= (long                ); 
    sc_dt::sc_signed& operator %= (unsigned long       ); 
    sc_dt::sc_signed& operator %= (int                 );
    sc_dt::sc_signed& operator %= (unsigned int        );
    sc_dt::sc_signed& operator &= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator &= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator &= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator &= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator &= (long                ); 
    sc_dt::sc_signed& operator &= (unsigned long       ); 
    sc_dt::sc_signed& operator &= (int                 );
    sc_dt::sc_signed& operator &= (unsigned int        );
    sc_dt::sc_signed& operator |= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator |= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator |= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator |= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator |= (long                ); 
    sc_dt::sc_signed& operator |= (unsigned long       ); 
    sc_dt::sc_signed& operator |= (int                 );
    sc_dt::sc_signed& operator |= (unsigned int        );
    sc_dt::sc_signed& operator ^= (const sc_dt::sc_signed&    );
    sc_dt::sc_signed& operator ^= (const sc_dt::sc_unsigned&  );
    sc_dt::sc_signed& operator ^= (sc_dt::int64               ); 
    sc_dt::sc_signed& operator ^= (sc_dt::uint64              ); 
    sc_dt::sc_signed& operator ^= (long                ); 
    sc_dt::sc_signed& operator ^= (unsigned long       ); 
    sc_dt::sc_signed& operator ^= (int                 );
    sc_dt::sc_signed& operator ^= (unsigned int        );

  protected:
    mutable sc_event*  m_changed_event_p; // Value changed event this object.
    sc_dt::uint64      m_event_delta;     // Delta cycle of last event.
    sc_dt::sc_signed   m_new_val;         // New value for this object instance.
    sc_port_base*      m_output_p;        // Single write port verify field.
    sc_process_b*   m_writer_p;        // Single writer verify field.
};


SC_TEMPLATE // Return true if a changed event happened in the last delta cycle.
inline bool sc_signal<sc_dt::sc_bigint<W> >::base_event() const
{
    return simcontext()->delta_count() == m_event_delta + 1;
}


SC_TEMPLATE // Return this object's sc_dt::sc_bigint<W> object instance.
inline const sc_dt::sc_bigint<W>& sc_signal<sc_dt::sc_bigint<W> >::base_read() const
{
	return *this;
}


SC_TEMPLATE // Return the value changed event, allocating it if necessary.
inline const sc_event& sc_signal<sc_dt::sc_bigint<W> >::base_value_changed_event() const
{
    if ( !m_changed_event_p ) m_changed_event_p = new sc_event;
    return *m_changed_event_p;
}


SC_TEMPLATE // Write a const sc_dt::sc_signed& value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::base_write( const sc_dt::sc_signed& value )
{
#   if defined(DEBUG_SYSTEMC)
        check_writer();
#   endif
    m_new_val = value;
    request_update();
}

SC_TEMPLATE // Write a const sc_dt::sc_unsigned& value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::base_write( const sc_dt::sc_unsigned& value )
{
#   if defined(DEBUG_SYSTEMC)
        check_writer();
#   endif
    m_new_val = value;
    request_update();
}

SC_TEMPLATE // Write a sc_dt::int64 value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::base_write( sc_dt::int64 value )
{
#   if defined(DEBUG_SYSTEMC)
        check_writer();
#   endif
    m_new_val = value;
    request_update();
}


SC_TEMPLATE // Write a sc_dt::uint64 value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::base_write( sc_dt::uint64 value )
{
#   if defined(DEBUG_SYSTEMC)
        check_writer();
#   endif
    m_new_val = value;
    request_update();
}


//------------------------------------------------------------------------------
//"sc_signal<sc_dt::sc_bigint<W> >::check_writer"
//
// This method checks to see if there is more than one writer for this 
// object instance by keeping track of the process performing the write.
//------------------------------------------------------------------------------
SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::check_writer()
{
    sc_process_b* writer_p = sc_get_current_process_b();
    if( m_writer_p == 0 ) 
    {   
        m_writer_p = writer_p;
    } 
    else if( m_writer_p != writer_p ) 
    {   
        sc_signal_invalid_writer( name(), kind(),
                                  m_writer_p->name(), writer_p->name() );
    }
}


//------------------------------------------------------------------------------
//"sc_signal<sc_dt::sc_bigint<W> >::concat_set"
//
// These virtual methods allow value assignments to this object instance
// from various sources. The position within the supplied source of the 
// low order bit for this object instance's value is low_i.
//     src   = source value.
//     low_i = bit within src to serve as low order bit of this object 
//             instance's value.
//------------------------------------------------------------------------------
SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::concat_set(sc_dt::int64 src, int low_i)
{
    if ( low_i < 64 )
    {
        base_write(src >> low_i);
    }
    else
    {
        base_write( (sc_dt::int64)((src < 0 ) ?  -1 : 0 ));
    } 
}

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::concat_set(
	const sc_dt::sc_lv_base& src, int low_i)
{
    sc_dt::sc_unsigned tmp(src.length());
    tmp = src >> low_i;
    base_write( tmp );
}

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::concat_set(
	const sc_dt::sc_signed& src, int low_i)
{
    base_write( (src >> low_i) );
}

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::concat_set(	
	const sc_dt::sc_unsigned& src, int low_i)
{
    base_write( (src >> low_i) );
}

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::concat_set(sc_dt::uint64 src, int low_i)
{
	base_write( (sc_dt::uint64)(( low_i < 64 ) ? src >> low_i : 0));
}



SC_TEMPLATE // Return the default event for this object instance.
inline const sc_event& sc_signal<sc_dt::sc_bigint<W> >::default_event() const 
	{ return base_value_changed_event(); }


SC_TEMPLATE // Return true if a changed event happened in the last delta cycle.
inline bool sc_signal<sc_dt::sc_bigint<W> >::event() const
	{ return base_event(); }


SC_TEMPLATE // Return a reference to the value of this object instance.
inline const sc_dt::sc_bigint<W>& sc_signal<sc_dt::sc_bigint<W> >::get_data_ref() const
	{ return *this; }


SC_TEMPLATE // Return a pointer to this object instance.
inline sc_signal<sc_dt::sc_bigint<W> >& sc_signal<sc_dt::sc_bigint<W> >::get_signal() 
	{ return *this; }


SC_TEMPLATE // Return a kind value of "sc_signal".
inline const char* sc_signal<sc_dt::sc_bigint<W> >::kind() const
{
	return "sc_signal";
}


//------------------------------------------------------------------------------
//"sc_signal<sc_dt::sc_bigint<W> >::operator ()
//
// This operator returns a part selection of this object instance.
//     left  = left-hand bit of the selection.
//     right = right-hand bit of the selection.
//------------------------------------------------------------------------------
SC_TEMPLATE
inline sc_signed_sigref& sc_signal<sc_dt::sc_bigint<W> >::operator () (int left, int right)
{
    sc_signed_sigref* result_p;   // Value to return.

	result_p = sc_signed_sigref::m_pool.allocate();
	result_p->initialize(this, left, right);
	return *result_p;
}


//------------------------------------------------------------------------------
//"sc_signal<sc_dt::sc_bigint<W> >::operator []"
//
// This operator returns a bit selection of this object instance.
//     i = bit to be selected.
//------------------------------------------------------------------------------
SC_TEMPLATE
inline sc_signed_sigref& sc_signal<sc_dt::sc_bigint<W> >::operator [] ( int bit )
{
    return operator () (bit,bit);
}


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( const this_type& new_val )
	{ base_write( new_val ); }

SC_TEMPLATE 
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( const char* new_val )
	{ sc_dt::sc_bigint<W> tmp = new_val; m_new_val = tmp; request_update(); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( sc_dt::uint64 new_val )
	{ base_write(new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( sc_dt::int64 new_val )
	{ base_write(new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( int new_val )
	{ base_write((sc_dt::int64)new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( long new_val ) 
	{ base_write((sc_dt::int64)new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( short new_val ) 
	{ base_write((sc_dt::int64)new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( unsigned int new_val ) 
	{ base_write((sc_dt::int64)new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( unsigned long new_val )
	{ base_write((sc_dt::int64)new_val); }


SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( unsigned short new_val )
	{ base_write((sc_dt::int64)new_val); }

SC_TEMPLATE
template<typename T>
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = (
    const sc_dt::sc_generic_base<T>& new_val )
{
    sc_dt::sc_unsigned temp(W);
    new_val->to_sc_unsigned(temp);
    base_write(temp);
}

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( const sc_dt::sc_unsigned& new_val )
	{ base_write(new_val); }

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( const sc_dt::sc_bv_base& new_val )
	{ base_write( (sc_dt::sc_bigint<W>)new_val ); }

SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( const sc_dt::sc_lv_base& new_val )
	{ base_write( (sc_dt::sc_bigint<W>)new_val ); }



SC_TEMPLATE
inline void sc_signal<sc_dt::sc_bigint<W> >::operator = ( const sc_dt::sc_signed& new_val )
	{ base_write(new_val); }


SC_TEMPLATE // Return a pointer to the read target for part selections.
inline sc_dt::sc_signed* sc_signal<sc_dt::sc_bigint<W> >::part_read_target()
	{ return this; }

SC_TEMPLATE // Return this object's base value as a const reference.
inline const sc_dt::sc_bigint<W>& sc_signal<sc_dt::sc_bigint<W> >::read() const
	{ return *this; }


SC_TEMPLATE // Read a portion of a value.
inline sc_dt::sc_signed sc_signal<sc_dt::sc_bigint<W> >::read_part( 
	int left, int right ) const
{
    return (sc_dt::sc_signed)sc_dt::sc_signed::operator () (left, right);
}

SC_TEMPLATE // Register a port for write checking.
inline void sc_signal<sc_dt::sc_bigint<W> >::register_port( 
	__attribute__((unused)) sc_port_base& port_, __attribute__((unused)) const char* if_typename_ )
{
#       ifdef DEBUG_SYSTEMC
		std::string nm( if_typename_ );
		if( nm == typeid( sc_signal_inout_if<sc_dt::sc_bigint<W> > ).name() ) 
		{
			if( m_output_p != 0 ) 
			{
				sc_signal_invalid_writer( name(), kind(),
					 m_output_p->name(), port_.name() );
			}
			m_output_p = &port_;
		}
#       endif
}


SC_TEMPLATE // Autogenerated name object instance constructor.
inline sc_signal<sc_dt::sc_bigint<W> >::sc_signal() : 
	sc_prim_channel(sc_gen_unique_name( "signal" )),
	m_changed_event_p(0),
	m_new_val(W),
	m_output_p(0),
	m_writer_p(0)
{ }


SC_TEMPLATE // Explicitly named object instance constructor.
inline sc_signal<sc_dt::sc_bigint<W> >::sc_signal(const char* name_) : 
	sc_prim_channel(name_),
	m_changed_event_p(0),
	m_new_val(W),
	m_output_p(0),
	m_writer_p(0)
{ }


SC_TEMPLATE // Object instance destructor.
inline sc_signal<sc_dt::sc_bigint<W> >::~sc_signal() 
{
	if ( m_changed_event_p ) delete m_changed_event_p;
}


SC_TEMPLATE // Update the current value from new value.
inline void sc_signal<sc_dt::sc_bigint<W> >::update()
{
    if ( m_changed_event_p )
    {
        if ( m_new_val != *this )
        {
            m_changed_event_p->notify_delayed();
            m_event_delta = simcontext()->delta_count();
        }
    }
	sc_dt::sc_signed::operator = (m_new_val);
}


SC_TEMPLATE // Return the value changed event.
inline const sc_event& sc_signal<sc_dt::sc_bigint<W> >::value_changed_event() const
	{ return base_value_changed_event(); }


SC_TEMPLATE // Write a sc_in<sc_dt::sc_bigint<W> > value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::write( const sc_in<sc_dt::sc_bigint<W> >& value )
	{ base_write( value ); }


SC_TEMPLATE // Write a sc_inout<sc_dt::sc_bigint<W> > value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::write( const sc_inout<sc_dt::sc_bigint<W> >& value ) 
	{ base_write( value ); }


SC_TEMPLATE // Write a sc_dt::sc_bigint<W> value to this object instance.
inline void sc_signal<sc_dt::sc_bigint<W> >::write( const sc_dt::sc_bigint<W>& value ) 
	{ base_write( value); }


SC_TEMPLATE // Select a portion of a value. 
inline sc_signed_sigref& sc_signal<sc_dt::sc_bigint<W> >::select_part(
    int left, int right)
{
    sc_signed_sigref* result_p = sc_signed_sigref::m_pool.allocate();
    result_p->initialize(DCAST<sc_signed_part_if*>(this), left, right);
    return *result_p;
}


SC_TEMPLATE // Write a portion of a value. If this is the first write in 
            // a delta cycle we copy the existing value before setting the bits.
inline void sc_signal<sc_dt::sc_bigint<W> >::write_part(sc_dt::int64 v, int left, int right) 
{
	m_new_val(left, right) = v;
	request_update();
}


SC_TEMPLATE // Write a portion of a value. If this is the first write in 
            // a delta cycle we copy the existing value before setting the bits.
inline void sc_signal<sc_dt::sc_bigint<W> >::write_part(sc_dt::uint64 v, int left, int right)
{
	m_new_val(left, right) = v;
	request_update();
}


SC_TEMPLATE // Write a portion of a value. If this is the first write in 
            // a delta cycle we copy the existing value before setting the bits.
inline void sc_signal<sc_dt::sc_bigint<W> >::write_part( 
	const sc_dt::sc_signed& v, int left, int right ) 
{
	m_new_val(left, right) = v;
	request_update();
}


SC_TEMPLATE // Write a portion of a value. If this is the first write in 
            // a delta cycle we copy the existing value before setting the bits.
inline void sc_signal<sc_dt::sc_bigint<W> >::write_part( 
	const sc_dt::sc_unsigned& v, int left, int right ) 
{
	m_new_val(left, right) = v;
	request_update();
}


//==============================================================================
// CLASS sc_in<sc_dt::sc_bigint<W> >
//
// This class implements an input port whose target acts like an sc_dt::sc_bigint<W> data
// value. This class is a specialization of the generic sc_in class to 
// implement tailored support for the sc_dt::sc_bigint<W> class.
//==============================================================================
SC_TEMPLATE
class sc_in<sc_dt::sc_bigint<W> > : 
    public sc_port<sc_signal_in_if<sc_dt::sc_bigint<W> >, 1, 
                   SC_ONE_OR_MORE_BOUND>,
    public sc_dt::sc_value_base
{
  public:

    // typedefs

    typedef sc_dt::sc_bigint<W>                      data_type;
    typedef sc_signal_in_if<sc_dt::sc_bigint<W> >    if_type;
    typedef sc_port<if_type,1,SC_ONE_OR_MORE_BOUND>  base_type;
    typedef sc_in<sc_dt::sc_bigint<W> >              this_type;

    typedef if_type                                  in_if_type;
    typedef base_type                                in_port_type;
    typedef sc_signal_inout_if<sc_dt::sc_bigint<W> > inout_if_type;
    typedef sc_inout<sc_dt::sc_bigint<W> >           inout_port_type;

  public:

    // bind methods and operators:

    void bind( const in_if_type& interface_ ) 
        { sc_port_base::bind( CCAST<in_if_type&>( interface_) );}
    void operator () ( const in_if_type& interface_ )
        { sc_port_base::bind( CCAST<in_if_type&>( interface_) );}
    void bind( in_port_type& parent_ )
        { sc_port_base::bind(parent_);}
    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind(parent_);}
    void bind( inout_port_type& parent_ )
        { sc_port_base::bind(parent_);}
    void operator () ( inout_port_type& parent_ )
        { sc_port_base::bind(parent_);}

  protected:
    // called by pbind (for internal use only)
    virtual inline int vbind( sc_interface& interface_ )
        {
            return sc_port_b<if_type>::vbind( interface_ );
        }       
    virtual inline int vbind( sc_port_base& parent_ )
        {
            in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );  
            if( in_parent != 0 ) {
                sc_port_base::bind( *in_parent );
                return 0;
            }
            inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
            if( inout_parent != 0 ) {
                sc_port_base::bind( *inout_parent );
                return 0;
            }
            // type mismatch
            return 2;     
        }


    // constructors

  public:
    sc_in()
        : base_type(), m_traces( 0 )
        {}

    explicit sc_in( const char* name_ )
        : base_type( name_ ), m_traces( 0 )
        {}

    explicit sc_in( const in_if_type& interface_ )
        : base_type( CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
        {}

    sc_in( const char* name_, const in_if_type& interface_ )     
        : base_type( name_, CCAST<in_if_type&>( interface_ ) ), m_traces( 0 )
        {}

    explicit sc_in( in_port_type& parent_ )
        : base_type( parent_ ), m_traces( 0 )
        {}

    sc_in( const char* name_, in_port_type& parent_ )
        : base_type( name_, parent_ ), m_traces( 0 )  
        {}

    explicit sc_in( inout_port_type& parent_ )
        : base_type(), m_traces( 0 )
        { sc_port_base::bind( parent_ ); }

    sc_in( const char* name_, inout_port_type& parent_ )
        : base_type( name_ ), m_traces( 0 )
        { sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
        : base_type( parent_ ), m_traces( 0 )
        {}

    sc_in( const char* name_, this_type& parent_ )
        : base_type( name_, parent_ ), m_traces( 0 )
        {}


    // destructor

    virtual inline ~sc_in()
        {
            remove_traces();
        }

    // bit and part selection

    sc_dt::sc_signed_bitref_r operator [] ( int i ) const
        { return (*this)->read()[i]; }
    sc_dt::sc_signed_bitref_r bit( int i ) const
        { return (*this)->read()[i]; }
    sc_dt::sc_signed_subref_r operator () ( int left, int right ) const
        { return (*this)->read()(left,right); }
    sc_dt::sc_signed_subref_r range( int left, int right ) const
        { return (*this)->read()(left,right); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
        { return (*this)->value_changed_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
        { return (*this)->value_changed_event(); }


    // read the current value

    const sc_dt::sc_bigint<W>& read() const
        { return (*this)->read(); }

    operator const sc_dt::sc_bigint<W>& () const
        { return (*this)->read(); }

    // was there a value changed event?

    bool event() const
        { return (*this)->event(); }


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
        {
            return *new sc_event_finder_t<in_if_type>(
                *this, &in_if_type::value_changed_event );
        }



    // reduction methods:

    inline bool and_reduce() const
        { return (*this)->read().and_reduce(); }
    inline bool nand_reduce() const
        { return (*this)->read().nand_reduce(); }
    inline bool nor_reduce() const
        { return (*this)->read().nor_reduce(); }
    inline bool or_reduce() const
        { return (*this)->read().or_reduce(); }
    inline bool xnor_reduce() const
        { return (*this)->read().xnor_reduce(); }
    inline bool xor_reduce() const
        { return (*this)->read().xor_reduce(); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual inline void end_of_elaboration()
        {
            if( m_traces != 0 ) {
                for( unsigned int i = 0; i < m_traces->size(); ++ i ) {
                    sc_trace_params* p = (*m_traces)[i];
                    sc_trace( p->tf, read(), p->name );
                }
                remove_traces();
            }
        }

    virtual inline const char* kind() const 
        { return "sc_in"; }


    // called by sc_trace
    void add_trace( sc_trace_file* tf_, const std::string& name_ ) const
        { 
            if( tf_ != 0 ) {
                if( m_traces == 0 ) {
                    m_traces = new sc_trace_params_vec;
                }
                m_traces->push_back( new sc_trace_params( tf_, name_ ) );
            }
        }


    // concatenation methods

    virtual inline int concat_length(bool* xz_present_p) const
        { return (*this)->read().concat_length( xz_present_p ); }
    virtual inline sc_dt::uint64 concat_get_uint64() const
        { return (*this)->read().concat_get_uint64(); }
    virtual inline bool concat_get_ctrl( sc_dt::sc_digit* dst_p, int low_i ) const
        { return (*this)->read().concat_get_ctrl(dst_p, low_i); }
    virtual inline bool concat_get_data( sc_dt::sc_digit* dst_p, int low_i ) const
        { return (*this)->read().concat_get_data(dst_p, low_i); }

  protected:
    void remove_traces() const
        {
            if( m_traces != 0 ) {
                for( int i = m_traces->size() - 1; i >= 0; -- i ) {
                    delete (*m_traces)[i];
                }
                delete m_traces;
                m_traces = 0;
            }
        }

    mutable sc_trace_params_vec* m_traces;


  private:

    // disabled
    sc_in( const sc_in<sc_dt::sc_bigint<W> >& );
    sc_in<sc_dt::sc_bigint<W> >& operator = ( const sc_in<sc_dt::sc_bigint<W> >& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif

};



SC_TEMPLATE
inline std::ostream& operator << ( 
	std::ostream& os, const sc_in<sc_dt::sc_bigint<W> >& a )
{
    a.read().print( os );
    return os;
}


//==============================================================================
// CLASS sc_inout<sc_dt::sc_bigint<W> >
//
// This class implements an input/output port whose target acts like an 
// sc_dt::sc_bigint<W> data value. It is derived from the sc_signed_in. This class is a 
// specialization of the generic sc_inout class to implement tailored support 
// for the sc_dt::sc_bigint<W> class.
//==============================================================================
SC_TEMPLATE
class sc_inout<sc_dt::sc_bigint<W> > : 
    public sc_port<sc_signal_inout_if<sc_dt::sc_bigint<W> >, 1,
                   SC_ONE_OR_MORE_BOUND>,
    public sc_dt::sc_value_base
{
  public:

    // typedefs

    typedef sc_dt::sc_bigint<W>                      data_type;
    typedef sc_signal_inout_if<sc_dt::sc_bigint<W> > if_type;
    typedef sc_port<if_type,1,SC_ONE_OR_MORE_BOUND>  base_type;
    typedef sc_inout<sc_dt::sc_bigint<W> >           this_type;

    typedef if_type                                  inout_if_type;
    typedef base_type                                inout_port_type;

  public:

    // bind methods and operators:

    void bind( const inout_if_type& interface_ ) 
        { sc_port_base::bind( CCAST<inout_if_type&>( interface_) ); }
    void operator () ( const inout_if_type& interface_ )
        { sc_port_base::bind( CCAST<inout_if_type&>( interface_) ); }
    void bind( inout_port_type& parent_ )
        { sc_port_base::bind(parent_); }
    void operator () ( inout_port_type& parent_ )
        { sc_port_base::bind(parent_); }

  protected:
    // called by pbind (for internal use only)
    virtual inline int vbind( sc_interface& interface_ )
        {
            return sc_port_b<if_type>::vbind( interface_ );
        }       
    virtual inline int vbind( sc_port_base& parent_ )
        {
            inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
            if( inout_parent != 0 ) {
                sc_port_base::bind( *inout_parent );
                return 0;
            }
            // type mismatch
            return 2;     
        }


    // constructors

  public:
    sc_inout()
        : base_type(), m_init_val_p(0), m_traces( 0 )
        {}

    explicit sc_inout( const char* name_ )
        : base_type( name_ ), m_init_val_p(0), m_traces( 0 )
        {}

    explicit sc_inout( inout_if_type& interface_ )
        : base_type( interface_ ), m_init_val_p(0), m_traces( 0 )
        {}

    sc_inout( const char* name_, inout_if_type& interface_ )     
        : base_type( name_, interface_ ), m_init_val_p(0), m_traces( 0 )
        {}

    explicit sc_inout( inout_port_type& parent_ )
        : base_type( parent_ ), m_init_val_p(0), m_traces( 0 )
        {}

    sc_inout( const char* name_, inout_port_type& parent_ )
        : base_type( name_, parent_ ), m_init_val_p(0), m_traces( 0 )  
        {}

    sc_inout( this_type& parent_ )
        : base_type( parent_ ), m_init_val_p(0), m_traces( 0 )
        {}

    sc_inout( const char* name_, this_type& parent_ )
        : base_type( name_, parent_ ), m_init_val_p(0), m_traces( 0 )
        {}


    // destructor

    virtual inline ~sc_inout()
        {
            remove_traces();
        }

    // bit and part selection

    sc_dt::sc_signed_bitref_r operator [] ( int i ) const
        { return (*this)->read()[i]; }
    sc_dt::sc_signed_bitref_r bit( int i ) const
        { return (*this)->read()[i]; }
    sc_signed_sigref& operator [] ( int i ) 
		{ return (*this)->select_part(i,i); }
    sc_signed_sigref& bit( int i ) 
		{ return (*this)->select_part(i,i); }
    sc_dt::sc_signed_subref_r operator () ( int left, int right ) const
        { return (*this)->read()(left,right); }
    sc_dt::sc_signed_subref_r range( int left, int right ) const
        { return (*this)->read()(left,right); }
    sc_signed_sigref& operator () ( int left, int right )
		{ return (*this)->select_part(left,right); }
    sc_signed_sigref& range( int left, int right )
		{ return (*this)->select_part(left,right); }


    // interface access shortcut methods

    // get the default event

    const sc_event& default_event() const
        { return (*this)->value_changed_event(); }


    // get the value changed event

    const sc_event& value_changed_event() const
        { return (*this)->value_changed_event(); }


    // read the current value

    const sc_dt::sc_bigint<W>& read() const
        { return (*this)->read(); }

    operator const sc_dt::sc_bigint<W>& () const
        { return (*this)->read(); }

    // was there a value changed event?

    bool event() const
        { return (*this)->event(); }


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
        {
            return *new sc_event_finder_t<inout_if_type>(
                *this, &inout_if_type::value_changed_event );
        }



    // reduction methods:

    inline bool and_reduce() const
        { return (*this)->read().and_reduce(); }
    inline bool nand_reduce() const
        { return (*this)->read().nand_reduce(); }
    inline bool nor_reduce() const
        { return (*this)->read().nor_reduce(); }
    inline bool or_reduce() const
        { return (*this)->read().or_reduce(); }
    inline bool xnor_reduce() const
        { return (*this)->read().xnor_reduce(); }
    inline bool xor_reduce() const
        { return (*this)->read().xor_reduce(); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual inline void end_of_elaboration()
        {
            if( m_init_val_p != 0 ) {
                (*this)->write( *m_init_val_p );
                delete m_init_val_p;
                m_init_val_p = 0;
            }
            if( m_traces != 0 ) {
                for( unsigned int i = 0; i < m_traces->size(); ++ i ) {
                    sc_trace_params* p = (*m_traces)[i];
                    sc_trace( p->tf, read(), p->name );
                }
                remove_traces();
            }
        }

    virtual inline const char* kind() const 
        { return "sc_inout"; }

    // value initialization

    inline void initialize( const sc_dt::sc_bigint<W>& value_ )
    {
        inout_if_type* iface = DCAST<inout_if_type*>( this->get_interface() );     
        if( iface != 0 ) {
            iface->write( value_ );
        } else {
            if( m_init_val_p == 0 ) {
                m_init_val_p = new sc_dt::sc_bigint<W>;
            }
            *m_init_val_p = value_;
        }
    }


    // called by sc_trace
    void add_trace( sc_trace_file* tf_, const std::string& name_ ) const
        { 
            if( tf_ != 0 ) {
                if( m_traces == 0 ) {
                    m_traces = new sc_trace_params_vec;
                }
                m_traces->push_back( new sc_trace_params( tf_, name_ ) );
            }
        }


    // concatenation methods

    virtual inline int concat_length(bool* xz_present_p) const
        { return (*this)->read().concat_length( xz_present_p ); }
    virtual inline sc_dt::uint64 concat_get_uint64() const
        { return (*this)->read().concat_get_uint64(); }
    virtual inline bool concat_get_ctrl( sc_dt::sc_digit* dst_p, int low_i ) const
        { return (*this)->read().concat_get_ctrl(dst_p, low_i); }
    virtual inline bool concat_get_data( sc_dt::sc_digit* dst_p, int low_i ) const
        { return (*this)->read().concat_get_data(dst_p, low_i); }
    virtual inline void concat_set(sc_dt::int64 src, int low_i)
		{ *this = (src >> ((low_i < 64) ? low_i : 63)); }
#if 0 // ####
    virtual inline void concat_set(const sc_dt::sc_lv_base& src, int low_i)
        { *this = src >> low_i; }
#endif // 0 ####
    virtual inline void concat_set(const sc_dt::sc_signed& src, int low_i)
        { *this = (src >> low_i); }
    virtual inline void concat_set(const sc_dt::sc_unsigned& src, int low_i)
        { *this = (src >> low_i); }
    virtual inline void concat_set(sc_dt::uint64 src, int low_i)
        { *this = ((low_i < 64) ? (src >> low_i) : (sc_dt::uint64)0); }


  public: // assignment operators:
    inline void operator = ( const this_type& new_val )
        { (*this)->write( new_val.read() ); }
    inline void operator = ( const char* new_val )
        { sc_dt::sc_signed aa(W); aa = new_val; (*this)->write( aa ); }
    inline void operator = ( sc_dt::uint64 new_val )
        { (*this)->write(new_val); }
    inline void operator = ( sc_dt::int64 new_val )
        { (*this)->write(new_val); }
    inline void operator = ( int new_val )
        { (*this)->write((sc_dt::int64)new_val); }
    inline void operator = ( long new_val )
        { (*this)->write((sc_dt::int64)new_val); }
    inline void operator = ( short new_val )
        { (*this)->write((sc_dt::int64)new_val); }
    inline void operator = ( unsigned int new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( unsigned long new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( unsigned short new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    template<typename T>
    inline void operator = ( const sc_dt::sc_generic_base<T>& new_val )
        {
            sc_dt::sc_unsigned temp(W);
            new_val->to_sc_unsigned(temp);
            (*this)->write(temp);
        }
    inline void operator = ( const sc_dt::sc_signed& new_val )
        { (*this)->write(new_val); }
    inline void operator = ( const sc_dt::sc_unsigned& new_val )
        { (*this)->write(new_val); }
    inline void operator = ( const sc_dt::sc_bv_base& new_val )
        { (*this)->write((sc_dt::sc_bigint<W>)new_val); }
    inline void operator = ( const sc_dt::sc_lv_base& new_val )
        { (*this)->write((sc_dt::sc_bigint<W>)new_val); }

    inline void write( const sc_in<sc_dt::sc_bigint<W> >& new_val )
        { (*this)->write( new_val.read() ); }
    inline void write( const sc_inout<sc_dt::sc_bigint<W> >& new_val )
        { (*this)->write( new_val.read() ); }
    inline void write( const sc_dt::sc_bigint<W>& new_val )
        { (*this)->write( new_val); }

  protected:
    void remove_traces() const
        {
            if( m_traces != 0 ) {
                for( int i = m_traces->size() - 1; i >= 0; -- i ) {
                    delete (*m_traces)[i];
                }
                delete m_traces;
                m_traces = 0;
            }
        }

    sc_dt::sc_bigint<W>*                m_init_val_p;
    mutable sc_trace_params_vec* m_traces;


  private:

    // disabled
    sc_inout( const sc_inout<sc_dt::sc_bigint<W> >& );

#ifdef __GNUC__
    // Needed to circumvent a problem in the g++-2.95.2 compiler:
    // This unused variable forces the compiler to instantiate
    // an object of T template so an implicit conversion from
    // read() to a C++ intrinsic data type will work.
    static data_type dummy;
#endif

};



SC_TEMPLATE
inline std::ostream& operator << ( 
	std::ostream& os, const sc_inout<sc_dt::sc_bigint<W> >& a )
{
    a.read().print( os );
    return os;
}


//==============================================================================
// CLASS sc_out<sc_dt::sc_bigint<W> >
//
// This class implements an output port whose target acts like an 
// sc_dt::sc_bigint<W> data value. This class is a derivation of sc_inout, since
// output ports are really no different from input/output ports.
//==============================================================================
SC_TEMPLATE
class sc_out<sc_dt::sc_bigint<W> > : public sc_inout<sc_dt::sc_bigint<W> >
{
  public:

    // typedefs

    typedef sc_dt::sc_bigint<W>                          data_type;

    typedef sc_out<data_type>                   this_type;
    typedef sc_inout<data_type>                 base_type;

    typedef typename base_type::inout_if_type   inout_if_type;
    typedef typename base_type::inout_port_type inout_port_type;

    // constructors

    sc_out()
        : base_type()
        {}

    explicit sc_out( const char* name_ )
        : base_type( name_ )
        {}

    explicit sc_out( inout_if_type& interface_ )
        : base_type( interface_ )
        {}

    sc_out( const char* name_, inout_if_type& interface_ )
        : base_type( name_, interface_ )
        {}

    explicit sc_out( inout_port_type& parent_ )
        : base_type( parent_ )
        {}

    sc_out( const char* name_, inout_port_type& parent_ )
        : base_type( name_, parent_ )
        {}

    sc_out( this_type& parent_ )
        : base_type( parent_ )
        {}

    sc_out( const char* name_, this_type& parent_ )
        : base_type( name_, parent_ )
        {}


    // destructor (does nothing)

    virtual inline ~sc_out()
        {}


    // assignment operators:

  public:
    inline void operator = ( const this_type& new_val )
        { (*this)->write( (const sc_dt::sc_signed&)new_val ); }
    inline void operator = ( const char* new_val )
        { sc_dt::sc_signed aa(W); aa = new_val; (*this)->write( aa ); }
    inline void operator = ( sc_dt::uint64 new_val )
        { (*this)->write(new_val); }
    inline void operator = ( sc_dt::int64 new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( int new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( long new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( short new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( unsigned int new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( unsigned long new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    inline void operator = ( unsigned short new_val )
        { (*this)->write((sc_dt::uint64)new_val); }
    template<typename T>
    inline void operator = ( const sc_dt::sc_generic_base<T>& new_val )
        {
            sc_dt::sc_unsigned temp(W);
            new_val->to_sc_unsigned(temp);
            (*this)->write(temp);
        }
    inline void operator = ( const sc_dt::sc_signed& new_val )
        { (*this)->write(new_val); }
    inline void operator = ( const sc_dt::sc_unsigned& new_val )
        { (*this)->write(new_val); }
    inline void operator = ( const sc_dt::sc_bv_base& new_val )
        { (*this)->write((sc_dt::sc_bigint<W>)new_val); }
    inline void operator = ( const sc_dt::sc_lv_base& new_val )
        { (*this)->write((sc_dt::sc_bigint<W>)new_val); }

  private:

    // disabled
    sc_out( const this_type& );
};



//------------------------------------------------------------------------------
//"sc_signed_sigref::initialize"
//
// This method initializes an object instance from the supplied arguments.
//     if_p   -> target of this selection.
//     left_  =  left-most bit in selection.
//     right_ =  right-most bit in selection.
//------------------------------------------------------------------------------
inline void sc_signed_sigref::initialize( 
	sc_signed_part_if* if_p, int left_, int right_ )
{
    m_if_p = if_p;
    m_left = left_;
    m_right = right_;
	m_obj_p = if_p->part_read_target();
}


//------------------------------------------------------------------------------
//"sc_signed_sigref::operator ="
//
// These operators assign a value to the bits associated with this object
// instance within this object instance's target signal.
//------------------------------------------------------------------------------
inline void sc_signed_sigref::operator = ( sc_dt::uint64 v )
{
	m_if_p->write_part( v, m_left, m_right );
}

inline void sc_signed_sigref::operator = ( const char* v )
{
	sc_dt::sc_signed tmp(length()); tmp = v; *this = tmp;
}

inline void sc_signed_sigref:: operator = ( sc_dt::int64 v )
{ 
    *this = (sc_dt::uint64)v; 
}

inline void sc_signed_sigref:: operator = ( int v )
{ 
    *this = (sc_dt::uint64)v; 
}

inline void sc_signed_sigref:: operator = ( long v )
{ 
    *this = (sc_dt::uint64)v; 
}

inline void sc_signed_sigref:: operator = ( unsigned int v )
{ 
    *this = (sc_dt::uint64)v; 
}

inline void sc_signed_sigref:: operator = ( unsigned long v )
{ 
    *this = (sc_dt::uint64)v; 
}

template<typename T>
inline void sc_signed_sigref:: operator = ( const sc_dt::sc_generic_base<T>& v )
{
    sc_dt::sc_unsigned temp(m_left-m_right+1);
    v->to_sc_unsigned(temp);
    m_if_p->write_part( temp, m_left, m_right );
}

inline void sc_signed_sigref:: operator = ( const sc_dt::sc_signed& v )
{ 
    m_if_p->write_part( v, m_left, m_right );
}

inline void sc_signed_sigref:: operator = ( const sc_dt::sc_unsigned& v )
{ 
    m_if_p->write_part( v, m_left, m_right );
}

void sc_signed_sigref::operator = ( const sc_signed_sigref& v )
{
    *this = (sc_dt::sc_unsigned)v;
}


#undef SC_TEMPLATE
} // namespace sc_core
#endif // !defined(SC_SIGNAL_SIGNED_H)
