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

    sc_port_clk.h -- Clock port class. Requires sc_polarity.h.

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

#ifndef SC_PORT_CLK_H
#define SC_PORT_CLK_H

template<>
class sc_port<sc_clock> : public sc_port_edgy {
private:
    sc_port& operator=(const sc_port&); /* never defined */
    static bool type_check_sgnl(const sc_signal_base* sig);
    static bool type_check_port(const sc_port_typecheck* tc);
    static const sc_port_vtable* port_vtfn();

protected:
    const sc_clock* const_signal() const
    {
        return static_cast<sc_clock*>(src_signal);
    }
    sc_clock* nonconst_signal() const
    {
        return static_cast<sc_clock*>(src_signal);
    }

    BIND_STATUS bind(const sc_clock& clk);
    BIND_STATUS bind(const sc_port<sc_clock>& p);

    BIND_STATUS operator()(const sc_clock& clk)        { return bind(clk); }
    BIND_STATUS operator()(const sc_port<sc_clock>& p) { return bind(p); }

    sc_port(PORT_TYINFO ty)
        : sc_port_edgy(port_vtfn(), (ty | PORT_TYINFO_ISCLK))
    {
    }
    sc_port(const sc_clock& clk, PORT_TYINFO ty)
        : sc_port_edgy((sc_signal_base&) clk, port_vtfn(),
                       (ty | PORT_TYINFO_ISCLK))
    {
    }
    sc_port(const sc_port<sc_clock>& p, PORT_TYINFO ty)
        : sc_port_edgy(p, port_vtfn(), (ty | PORT_TYINFO_ISCLK))
    {
    }
    ~sc_port()
    {
    }
};

class sc_readable_clk_port : public sc_port<sc_clock> {
private:
    sc_readable_clk_port& operator=(const sc_readable_clk_port&);

protected:
    sc_readable_clk_port(PORT_TYINFO rw) : sc_port<sc_clock>(rw) { }
    sc_readable_clk_port(const sc_clock& clk, PORT_TYINFO rw) : sc_port<sc_clock>(clk, rw) { }
    sc_readable_clk_port(const sc_readable_clk_port& p, PORT_TYINFO rw)
        : sc_port<sc_clock>(p, rw) { }
    ~sc_readable_clk_port() { }

public:
    bool read()     const { return const_signal()->read();    }
    operator bool() const { return const_signal()->read();    }
    bool event()    const { return const_signal()->event();   }
    bool posedge()  const { return const_signal()->posedge(); }
    bool negedge()  const { return const_signal()->negedge(); }
    sc_polarity pos() const { return sc_polarity(this, true);  }
    sc_polarity neg() const { return sc_polarity(this, false); }
    operator sc_polarity() const { return sc_polarity(this, true); }
    const sc_signal_edgy_deval& delayed() const { return const_signal()->delayed(); }
};

template<>
class sc_inout<sc_clock> : public sc_readable_clk_port {
public:
    sc_inout<sc_clock>& write(const bool d)
    {
        nonconst_signal()->write(d);
        return *this;
    }
    sc_inout<sc_clock>& operator=(const bool d)
    {
        nonconst_signal()->write(d);
        return *this;
    }
    sc_inout<sc_clock>& operator=(const sc_inout<sc_clock>& d)
    {
        nonconst_signal()->write(d.read());
        return *this;
    }

    BIND_STATUS bind(const sc_clock& sig)         { return sc_port<sc_clock>::bind(sig); }
    BIND_STATUS bind(const sc_inout<sc_clock>& p) { return sc_port<sc_clock>::bind(p);   }

    BIND_STATUS operator()(const sc_clock& sig)         { return bind(sig); }
    BIND_STATUS operator()(const sc_inout<sc_clock>& p) { return bind(p); }

    sc_inout();
    sc_inout(sc_clock& sig);
    sc_inout(const sc_inout<sc_clock>& p);
    ~sc_inout();
};

template<>
class sc_in<sc_clock> : public sc_readable_clk_port {
private:
    sc_in<sc_clock>& operator=(const sc_in<sc_clock>&);

public:
    BIND_STATUS bind(const sc_clock& sig)         { return sc_port<sc_clock>::bind(sig); }
    BIND_STATUS bind(const sc_in<sc_clock>& p)    { return sc_port<sc_clock>::bind(p); }
    BIND_STATUS bind(const sc_inout<sc_clock>& p) { return sc_port<sc_clock>::bind(p); }

    BIND_STATUS operator()(const sc_clock& sig)         { return bind(sig); }
    BIND_STATUS operator()(const sc_in<sc_clock>& p)    { return bind(p); }
    BIND_STATUS operator()(const sc_inout<sc_clock>& p) { return bind(p); }

    sc_in();
    sc_in(const sc_clock& sig);
    sc_in(const sc_in<sc_clock>& p);
    sc_in(const sc_inout<sc_clock>& p);
    ~sc_in();
};

template<>
class sc_out<sc_clock> : public sc_port<sc_clock> {
private:
    sc_out<sc_clock>& operator=(const sc_out<sc_clock>&);

public:
    sc_out<sc_clock>& write(const bool d)
    {
        nonconst_signal()->write(d);
        return *this;
    }
    sc_out<sc_clock>& operator=(const bool d)
    {
        nonconst_signal()->write(d);
        return *this;
    }

    BIND_STATUS bind(const sc_clock& sig)         { return sc_port<sc_clock>::bind(sig); }
    BIND_STATUS bind(const sc_out<sc_clock>&  p)  { return sc_port<sc_clock>::bind(p); }
    BIND_STATUS bind(const sc_inout<sc_clock>& p) { return sc_port<sc_clock>::bind(p); }

    BIND_STATUS operator()(const sc_clock& sig)         { return bind(sig); }
    BIND_STATUS operator()(const sc_out<sc_clock>&  p)  { return bind(p); }
    BIND_STATUS operator()(const sc_inout<sc_clock>& p) { return bind(p); }

    sc_out();
    sc_out(sc_clock& sig);
    sc_out(const sc_out<sc_clock>& p);
    sc_out(const sc_inout<sc_clock>& p);
    ~sc_out();
};

typedef sc_inout<sc_clock> sc_inout_clk;
typedef sc_in<sc_clock> sc_in_clk;
typedef sc_out<sc_clock> sc_out_clk;

#endif
