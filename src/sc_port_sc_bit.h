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

    sc_port_sc_bit.h -- Bit port class.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation: Ali Dasdan. Synopsys, Inc. (dasdan@synopsys.com)
    Description of Modification: Implementation of operator() for binding.
    
    Modifier Name & Affiliation:
    Description of Modification:

******************************************************************************/

#ifndef SC_PORT_SC_BIT_H
#define SC_PORT_SC_BIT_H

template<>
class sc_port<sc_bit> : public sc_port_edgy {
private:
    sc_port<sc_bit>& operator=(const sc_port<sc_bit>&); /* never defined */
    static bool type_check_sgnl(const sc_signal_base* sig);
    static bool type_check_port(const sc_port_typecheck* tc);
    static const sc_port_vtable* port_vtfn();

protected:
    const sc_signal<sc_bit>* const_signal() const
    {
        return (const sc_signal<sc_bit>*) src_signal;
    }
    sc_signal<sc_bit>* nonconst_signal() const
    {
        return (sc_signal<sc_bit>*) src_signal;
    }
    
    BIND_STATUS bind(const sc_signal<sc_bit>& sig);
    BIND_STATUS bind(const sc_port<sc_bit>& p);

    BIND_STATUS operator()(const sc_signal<sc_bit>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_port<sc_bit>& p)     { return bind(p); }

    sc_port(PORT_TYINFO ty)
        : sc_port_edgy(port_vtfn(), ty)
    {
    }
    sc_port(const sc_signal<sc_bit>& sig, PORT_TYINFO ty)
        : sc_port_edgy((sc_signal_base&) sig, port_vtfn(), ty)
    {
    }
    sc_port(const sc_port<sc_bit>& p, PORT_TYINFO ty)
        : sc_port_edgy(p, port_vtfn(), ty)
    {
    }
    ~sc_port()
    {
    }
};

class sc_readable_sc_bit_port : public sc_port<sc_bit> {
private:
    sc_readable_sc_bit_port& operator=(const sc_readable_sc_bit_port&);

protected:
    /* IMPORTANT NOTE:
       The ISBOOL bit in PORT_TYINFO is not exactly right.  It is
       set for sc_in<sc_bit> and sc_inout<sc_bit>.  This should be
       sufficient for our purpose because we can't be sensitive
       to outports, nor can we do watching on outports. */

    sc_readable_sc_bit_port(PORT_TYINFO rw)
        : sc_port<sc_bit>(rw) { }
    sc_readable_sc_bit_port(const sc_signal<sc_bit>& sig, PORT_TYINFO rw)
        : sc_port<sc_bit>(sig, rw) { }
    sc_readable_sc_bit_port(const sc_readable_sc_bit_port& p, PORT_TYINFO rw)
        : sc_port<sc_bit>(p, rw) { }
    ~sc_readable_sc_bit_port() { }

public:
    bool read() const     { return const_signal()->read();    }
    operator bool() const { return const_signal()->read();    }
    bool event()   const  { return const_signal()->event();   }
    bool posedge() const  { return const_signal()->posedge(); }
    bool negedge() const  { return const_signal()->negedge(); }
    const sc_signal_edgy_deval& delayed() const { return const_signal()->delayed(); }
};

template<>
class sc_inout<sc_bit> : public sc_readable_sc_bit_port {
public:
    sc_inout<sc_bit>& write(const bool d)
    {
        nonconst_signal()->write(d);
        return *this;
    }
    sc_inout<sc_bit>& operator=(const bool d)
    {
        nonconst_signal()->write(d);
        return *this;
    }
    sc_inout<sc_bit>& operator=(const sc_inout<sc_bit>& d)
    {
        nonconst_signal()->write(d.read());
        return *this;
    }

    BIND_STATUS bind(sc_signal<sc_bit>& sig)    { return sc_port<sc_bit>::bind(sig); }
    BIND_STATUS bind(const sc_inout<sc_bit>& p) { return sc_port<sc_bit>::bind(p); }

    BIND_STATUS operator()(sc_signal<sc_bit>& sig)    { return bind(sig); }
    BIND_STATUS operator()(const sc_inout<sc_bit>& p) { return bind(p); }

    sc_inout();
    sc_inout(sc_signal<sc_bit>& sig);
    sc_inout(const sc_inout<sc_bit>& p);
    ~sc_inout();
};


template<>
class sc_in<sc_bit> : public sc_readable_sc_bit_port {
private:
    sc_in<sc_bit>& operator=(const sc_in<sc_bit>&);

public:
    BIND_STATUS bind(const sc_signal<sc_bit>& sig) { return sc_port<sc_bit>::bind(sig); }
    BIND_STATUS bind(const sc_in<sc_bit>& p)       { return sc_port<sc_bit>::bind(p); }
    BIND_STATUS bind(const sc_inout<sc_bit>& p)    { return sc_port<sc_bit>::bind(p); }

    BIND_STATUS operator()(const sc_signal<sc_bit>& sig) { return bind(sig); }
    BIND_STATUS operator()(const sc_in<sc_bit>& p)       { return bind(p); }
    BIND_STATUS operator()(const sc_inout<sc_bit>& p)    { return bind(p); }

    sc_in();
    sc_in(const sc_signal<sc_bit>& sig);
    sc_in(const sc_in<sc_bit>& p);
    sc_in(const sc_inout<sc_bit>& p);
    ~sc_in();
};

#endif
