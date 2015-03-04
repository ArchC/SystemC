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

    sc_signal_sc_bit.h -- Specialization of signals for sc_bit.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_SIGNAL_SC_BIT_H
#define SC_SIGNAL_SC_BIT_H


template<>
class sc_signal<sc_bit> : public sc_signal_edgy
{
    friend class sc_clock;
    friend class sc_clock_edge;
    friend class sc_signal_bv_b;
    friend class sc_signal_optimize;

public:
    sc_signal();
    explicit sc_signal( const char* nm );
    explicit sc_signal( sc_object* created_by );
    ~sc_signal() {  }

    bool read()     const  { return cur_value.operator bool(); }
    operator bool() const  { return cur_value.operator bool(); }
    sc_bit* get_data_ptr() { return &cur_value; }
    const sc_bit* get_data_ptr() const { return &cur_value; }
    bool get_new_value()   {
        return (cur_value.is_bitref() ? new_value.value : 
                sc_bvrep_test_wb((const sc_bvrep*) new_value.rep, 0, cur_value.val_or_index));
    }
    bool get_old_value()   {
        return (cur_value.is_bitref() ? old_value.value :
                sc_bvrep_test_wb((const sc_bvrep*) old_value.rep, 0, cur_value.val_or_index));
    }
    sc_signal<sc_bit>& write( bool d );
    sc_signal<sc_bit>& operator=( bool d )  { return write(d); }
    sc_signal<sc_bit>& operator=( const sc_signal<sc_bit>& d ) { return write(d); }
    bool event() const;
    bool posedge() const;
    bool negedge() const;
    void print(ostream& os) const;
    void dump(ostream& os) const;
    void trace( sc_trace_file* tf ) const;

protected:
    virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const;
    void update();
    void update_old();
    void set_cur_value(bool v);
    void set_old_value(bool v);
    sc_bit& get_cur_value() { return cur_value; }
    bool edgy_read() const;

private:
    sc_signal( sc_signal_bv_b*, unsigned index );
    sc_bit          cur_value;
    union ptr_or_unsigned {
        void*    rep;
        unsigned value;
    } new_value, old_value;
};
#endif
