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

    sc_port.h -- Port classes.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: 
    - Implementation of operator= for sc_out and sc_inout when the
    right side is an sc_signal.
    - Implementation of operator() for binding.
    - Implementation of operator sc_module_prm for positional
    connection method.

    Modifier Name & Affiliation:
        Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)
    Description of Modification:
        resolved ports sc_xxx_rv<> added

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/

#ifndef SC_PORT_H
#define SC_PORT_H

/* Requires sc_signal.h */

#include "sc_iostream.h"
#include "sc_module_prm.h"
#include "sc_lv.h"

enum PORT_STATUS {
    PORT_STATUS_UNBOUND = 0,
    PORT_STATUS_BOUND_SIGNAL = 1,
    PORT_STATUS_BOUND_PORT = 2
};

enum BIND_STATUS {
    BIND_STATUS_OK,
    BIND_STATUS_REBIND,              /* trying to rebind a port */
    BIND_STATUS_WRONG_TYPE,          /* dynamic types of port/signal mismatch */
    BIND_STATUS_IN_TO_OUT,           /* binding an inport to an outport */
    BIND_STATUS_OUT_TO_IN,           /* binding an outport to an inport */
    BIND_STATUS_INOUT_TO_NON_INOUT,  /* binding an inoutport to a non-inoutport */
    BIND_STATUS_NON_IN_TO_CONST_SIG, /* binding a non-inport to a const signal */
    BIND_STATUS_UNKNOWN              /* unknown error */
};

typedef unsigned PORT_TYINFO;
const unsigned PORT_TYINFO_READ       = 0x1;
const unsigned PORT_TYINFO_WRITE      = 0x2;
const unsigned PORT_TYINFO_READWRITE  = (PORT_TYINFO_READ | PORT_TYINFO_WRITE);
const unsigned PORT_TYINFO_RWMASK     = (PORT_TYINFO_READ | PORT_TYINFO_WRITE);
const unsigned PORT_TYINFO_ISEDGY     = 0x4;
const unsigned PORT_TYINFO_ISCLK      = 0x8;
const unsigned PORT_TYINFO_TYMASK     = (PORT_TYINFO_ISEDGY | PORT_TYINFO_ISCLK);

class sc_port_b;

class sc_port_typecheck {
public:
    // declare destructor virtual so as to make this
    // class polymorphic.
    virtual ~sc_port_typecheck()=0;
};

template<class T>
class sc_port_typecheck_T : public sc_port_typecheck {
public:
    sc_port_typecheck_T() { }
  virtual ~sc_port_typecheck_T(){}
};

#if defined(__SUNPRO_CC)&&0
template<>
class sc_port_typecheck_T<sc_bit> : public sc_port_typecheck {
public:
    sc_port_typecheck_T() { }
    ~sc_port_typecheck_T() { }
};
template<>
class sc_port_typecheck_T<sc_clock> : public sc_port_typecheck {
public:
    sc_port_typecheck_T() { }
    ~sc_port_typecheck_T() { }
};
template<>
class sc_port_typecheck_T<bool> : public sc_port_typecheck {
public:
    sc_port_typecheck_T() { }
    ~sc_port_typecheck_T() { }
};
template<>
class sc_port_typecheck_T<int> : public sc_port_typecheck {
public:
    sc_port_typecheck_T() { }
    ~sc_port_typecheck_T() { }
};
#endif
/* "Simulated vtable", used to make ports small */
struct sc_port_vtable {
    bool (*type_check_sgnl)(const sc_signal_base* sig);
    bool (*type_check_port)(const sc_port_typecheck* ptc);
    const sc_port_typecheck* typecheck_obj;
};

class sc_port_b {
    friend class sc_port_manager;
    friend class sc_module;
    friend class sc_sensitive;
    friend class sc_sensitive_pos;
    friend class sc_sensitive_neg;

public:
    PORT_TYINFO rwmode() const;

    operator sc_module_prm&()
    {
      return reinterpret_cast<sc_module_prm&>(*this);
    }

private:
    sc_port_b& operator=(const sc_port_b&);

protected:
    sc_signal_base*  src_signal;

    BIND_STATUS bind_const(const sc_signal_base& sig);
    BIND_STATUS vbind(sc_signal_base& sig);
    BIND_STATUS vbind(const sc_port_b& p);

    sc_port_b(const sc_port_vtable* vt, PORT_TYINFO ty);
    sc_port_b(sc_signal_base& sig, const sc_port_vtable* vt, PORT_TYINFO ty);
    sc_port_b(const sc_port_b& p, const sc_port_vtable* vt, PORT_TYINFO ty);
    ~sc_port_b();
};

// forward declaration necessary for lookup
template<class T> class sc_port;

template<class T>
ostream& operator<<(ostream& os,const sc_port<T>& port);

template<class T>
class sc_port : public sc_port_b {
private:
    sc_port<T>& operator=(const sc_port<T>&); /* never defined */
    static bool type_check_sgnl(const sc_signal_base* sig);
    static bool type_check_port(const sc_port_typecheck* tc);
    static const sc_port_vtable* port_vtfn()
    {
        static sc_port_typecheck_T<T> tc;
        static sc_port_vtable vt = { &type_check_sgnl,
                                     &type_check_port,
                                     &tc };
        return &vt;
    }
#if !defined(_MSC_VER) && !defined(__HP_aCC) && !defined(SC_HDL_COSIM)
protected:
#else
public:
#endif
    const sc_signal<T>* const_signal() const
    {
        return (const sc_signal<T>*) src_signal;
    }
    sc_signal<T>* nonconst_signal() const
    {
        return (sc_signal<T>*) src_signal;
    }
protected:
    BIND_STATUS bind(const sc_signal<T>& sig);
    BIND_STATUS bind(const sc_port<T>& p);

    BIND_STATUS operator()(const sc_signal<T>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_port<T>& p)     { return bind(p); }

    sc_port(PORT_TYINFO ty)
        : sc_port_b(port_vtfn(), ty)
    {
    }
    sc_port(const sc_signal<T>& sig, PORT_TYINFO ty)
        : sc_port_b((sc_signal_base&) sig, port_vtfn(), ty)
    {
    }
    sc_port(const sc_port<T>& p, PORT_TYINFO ty)
        : sc_port_b(p, port_vtfn(), ty)
    {
    }
    sc_port(const sc_signal_base& sig, PORT_TYINFO ty)
        : sc_port_b( sig, port_vtfn(), ty){}
    sc_port(const sc_port_b& p, PORT_TYINFO ty)
        : sc_port_b(p, port_vtfn(), ty){}
    ~sc_port()
    {
    }
#if !defined(_MSC_VER) && !defined(__HP_aCC)
    friend  std::ostream& operator<< <>(std::ostream& os,const sc_port<T>& port);
#endif
};

template<class T>
ostream& operator<<(ostream& os,const sc_port<T>& port)
{ return os<<*port.const_signal(); }

template<class T> class sc_out;
template<class T> class sc_inout;

template<class T>
class sc_in : public sc_port<T> {
private:
    sc_in<T>& operator=(const sc_in<T>&);

public:
    const T& read() const     { return this->const_signal()->read();  }
    operator const T&() const { return this->const_signal()->read();  }
    const bool event() const  { return this->const_signal()->event(); }

    BIND_STATUS bind(const sc_signal<T>& sig) { return sc_port<T>::bind(sig); }
    BIND_STATUS bind(const sc_in<T>& p)       { return sc_port<T>::bind(p); }
    BIND_STATUS bind(const sc_inout<T>& p)    { return sc_port<T>::bind(p); }

    BIND_STATUS operator()(const sc_signal<T>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_in<T>& p)       { return bind(p); }
    BIND_STATUS operator()(const sc_inout<T>& p)    { return bind(p); }

    sc_in();
    sc_in(const sc_signal<T>& sig);
    sc_in(const sc_in<T>& p);
    sc_in(const sc_inout<T>& p);
    ~sc_in();
    protected:
    explicit sc_in(const sc_signal_base& sig)
      : sc_port<T>(sig, PORT_TYINFO_READ){}
    explicit sc_in(const sc_port_b& p): sc_port<T>(p, PORT_TYINFO_READ){}
};

template<class T>
class sc_out : public sc_port<T> {
    private:
    sc_out<T>& operator=(const sc_out<T>& d);

    public:
    sc_out<T>& write(const T& d)
    {
        this->nonconst_signal()->write(d);
        return *this;
    }
    sc_out<T>& operator=(const T& d)
    {
        this->nonconst_signal()->write(d);
        return *this;
    }
    sc_out<T>& operator=(sc_signal<T>& sig)
    {
        this->nonconst_signal()->write(sig.read());
        return *this;
    }

    BIND_STATUS bind(const sc_signal<T>& sig) { return sc_port<T>::bind(sig); }
    BIND_STATUS bind(const sc_out<T>& p)      { return sc_port<T>::bind(p); }
    BIND_STATUS bind(const sc_inout<T>& p)    { return sc_port<T>::bind(p); }

    BIND_STATUS operator()(const sc_signal<T>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_out<T>& p)      { return bind(p); }
    BIND_STATUS operator()(const sc_inout<T>& p)    { return bind(p); }

    sc_out();
    sc_out(const sc_signal<T>& sig);
    sc_out(const sc_out<T>& p);
    sc_out(const sc_inout<T>& p);
    ~sc_out();
    protected:
    explicit sc_out(const sc_signal_base& sig): sc_port<T>( sig, PORT_TYINFO_WRITE){}
    explicit sc_out(const sc_port_b& p): sc_port<T>(p,PORT_TYINFO_WRITE){}
};

template<class T>
class sc_inout : public sc_port<T> {
public:
    const T& read() const     { return this->const_signal()->read(); }
    operator const T&() const { return this->const_signal()->read(); }
    sc_inout<T>& write(const T& d)
    {
        this->nonconst_signal()->write(d);
        return *this;
    }
    sc_inout<T>& operator=(const T& d)
    {
        this->nonconst_signal()->write(d);
        return *this;
    }
    sc_inout<T>& operator=(sc_signal<T>& sig)
    {
        this->nonconst_signal()->write(sig.read());
        return *this;
    }
    sc_inout<T>& operator=(const sc_inout<T>& d)
    {
        this->nonconst_signal()->write(d.read());
        return *this;
    }

    BIND_STATUS bind(const sc_signal<T>& sig) { return sc_port<T>::bind(sig); }
    BIND_STATUS bind(const sc_inout<T>& p)    { return sc_port<T>::bind(p); }

    BIND_STATUS operator()(const sc_signal<T>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_inout<T>& p)    { return bind(p); }

    sc_inout();
    sc_inout(const sc_signal<T>& sig);
    sc_inout(const sc_inout<T>& p);
    ~sc_inout();
    protected:
    explicit sc_inout(const sc_signal_base& sig): sc_port<T>( sig, PORT_TYINFO_READWRITE){}
    explicit sc_inout(const sc_port_b& p): sc_port<T>(p,PORT_TYINFO_READWRITE){}
};

// resolved ports
template<int W> class sc_in_rv;
template<int W> class sc_inout_rv;
template<int W> class sc_out_rv;
template<int W> class sc_signal_rv;

template<int W> class sc_in_rv: public sc_in<sc_bv_ns::sc_lv<W> >
{
    sc_in_rv<W>& operator=(const sc_in_rv<W>&);
    typedef sc_in<sc_bv_ns::sc_lv<W> > inherited;
    typedef sc_bv_ns::sc_lv<W> data_type;
public:
    const data_type& read() const     { return this->const_signal()->read();  }
    operator const data_type&() const { return this->const_signal()->read();  }
    const bool event() const  { return this->const_signal()->event(); }

    BIND_STATUS bind(const sc_signal_rv<W>& sig){ return inherited::bind(sig); }
    BIND_STATUS bind(const sc_signal<data_type>& sig)
      { return inherited::bind(sig); }
    BIND_STATUS bind(const sc_in_rv<W>& p)      { return inherited::bind(p); }
    BIND_STATUS bind(const sc_in<data_type>& p) { return inherited::bind(p); }
    BIND_STATUS bind(const sc_inout_rv<W>& p)   { return inherited::bind(p); }

    BIND_STATUS operator()(const sc_signal_rv<W>& sig)     { return bind(sig); }
    BIND_STATUS operator()(const sc_signal<data_type>& sig){ return bind(sig); }
    BIND_STATUS operator()(const sc_in_rv<W>& p)   { return bind(p); }
    BIND_STATUS operator()(const inherited& p)     { return bind(p); }
    BIND_STATUS operator()(const sc_inout_rv<W>& p){ return bind(p); }

    sc_in_rv(){}
    sc_in_rv(const sc_signal_rv<W>& sig):inherited(sig){}
    sc_in_rv(const sc_signal<data_type>& sig):inherited(sig){}
    sc_in_rv(const sc_in<data_type>& p):inherited(p){}
    sc_in_rv(const sc_inout<data_type>& p):inherited(p){}
    sc_in_rv(const sc_in_rv<W>& p):inherited(p){}
    sc_in_rv(const sc_inout_rv<W>& p):inherited(p){}
};

template<int W> class sc_out_rv: public sc_out<sc_bv_ns::sc_lv<W> >
{
    sc_out_rv<W>& operator=(const sc_out_rv<W>&);
    typedef sc_out<sc_bv_ns::sc_lv<W> > inherited;
    typedef sc_bv_ns::sc_lv<W> data_type;
public:
    const data_type& read() const     { return this->const_signal()->read();  }
    operator const data_type&() const { return this->const_signal()->read();  }
    const bool event() const  { return this->const_signal()->event(); }

    sc_out_rv& write(const data_type& d)
    {
        inherited::write(d);
        return *this;
    }
    sc_out_rv& operator=(const data_type& d)
    {
        return static_cast<sc_out_rv<W>& >(inherited::operator=(d));
    }
    sc_out_rv& operator=(sc_signal_rv<W>& sig)
    {
        return  static_cast<sc_out_rv<W>& >(inherited::operator=(sig));
    }

    BIND_STATUS bind(const sc_signal_rv<W>& sig){ return inherited::bind(sig); }
    BIND_STATUS bind(const sc_out_rv<W>& p)     { return inherited::bind(p); }
    BIND_STATUS bind(const sc_inout_rv<W>& p)   { return inherited::bind(p); }

    BIND_STATUS operator()(const sc_signal_rv<W>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_out_rv<W>& p)      { return bind(p); }
    BIND_STATUS operator()(const sc_inout_rv<W>& p)    { return bind(p); }

    sc_out_rv(){}
    sc_out_rv(const sc_signal_rv<W>& sig):inherited(sig){}
    sc_out_rv(const sc_out_rv<W>& p):inherited(p){}
    sc_out_rv(const sc_inout_rv<W>& p):inherited(p){}
};

template<int W> class sc_inout_rv: public sc_inout<sc_bv_ns::sc_lv<W> >
{
    typedef sc_inout<sc_bv_ns::sc_lv<W> > inherited;
    typedef sc_bv_ns::sc_lv<W> data_type;
public:
    const data_type& read() const     { return this->const_signal()->read();  }
    operator const data_type&() const { return this->const_signal()->read();  }
    const bool event() const  { return this->const_signal()->event(); }

    sc_inout_rv& write(const data_type& d)
    {
        inherited::write(d);
        return *this;
    }
    sc_inout_rv& operator=(const data_type& d)
    {
        return static_cast<sc_inout_rv<W>&>(inherited::operator=(d));
    }
    sc_inout_rv& operator=(sc_signal_rv<W>& sig)
    {
        return static_cast<sc_inout_rv<W>&>(inherited::operator=(sig));
    }
    sc_inout_rv& operator=(const sc_inout_rv<W>& d)
    {
        return static_cast<sc_inout_rv<W>&>(inherited::operator=(d));
    }

    BIND_STATUS bind(const sc_signal_rv<W>& sig){ return inherited::bind(sig); }
    BIND_STATUS bind(const sc_out_rv<W>& p)     { return inherited::bind(p); }
    BIND_STATUS bind(const sc_inout_rv<W>& p)   { return inherited::bind(p); }

    BIND_STATUS operator()(const sc_signal_rv<W>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_out_rv<W>& p)      { return bind(p); }
    BIND_STATUS operator()(const sc_inout_rv<W>& p)    { return bind(p); }

    sc_inout_rv(){}
    sc_inout_rv(const sc_signal_rv<W>& sig):inherited(sig){}
    sc_inout_rv(const sc_out_rv<W>& p):inherited(p){}
    sc_inout_rv(const sc_inout_rv<W>& p):inherited(p){}
};

#include "sc_port_defs.h_"
#include "sc_port_spec.h_"

#endif
