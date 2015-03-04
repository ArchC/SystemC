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

    sc_sensitive.h -- Sensitivity classes.  Requires "sc_process.h"
    for declarations of sc_async_process_handle, &.c.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifndef SC_SENSITIVE_H
#define SC_SENSITIVE_H

class sc_signal_base;
class sc_signal_edgy;
class sc_signal_array_base;
class sc_signal_bool_vector;

class sc_port_b;
class sc_port_edgy;

class sc_sensitive {
    friend class sc_module;

private:
    sc_module* module; // back pointer to the containing module
    enum { SC_NONE, SC_ASYNC, SC_APROC } mode;
    sc_process_b* process_handle;

    sc_sensitive& operator=(const sc_sensitive&);
    sc_sensitive(const sc_sensitive&);

    // Constructor
    sc_sensitive(sc_module* p);
    ~sc_sensitive();

public:
    // Changing between process handles
    sc_sensitive& operator<<(const sc_async_process_handle& p);
    sc_sensitive& operator<<(const sc_aproc_process_handle& p);

    // Declaring signals current process is sensitive to
    sc_sensitive& operator<<(const sc_signal_base& s);
    sc_sensitive& operator()(const sc_signal_base& s);
    sc_sensitive& operator<<(const sc_port_b& s);
    sc_sensitive& operator()(const sc_port_b& s);

    sc_sensitive& operator<<(const sc_signal_array_base& s);
    sc_sensitive& operator()(const sc_signal_array_base& s);
    sc_sensitive& operator<<(const sc_signal_bool_vector& sbv);
    sc_sensitive& operator()(const sc_signal_bool_vector& sbv);

    sc_sensitive& operator<<(const sc_signal_edgy& s);
    sc_sensitive& operator()(const sc_signal_edgy& s);
    sc_sensitive& operator<<(const sc_port_edgy& s);
    sc_sensitive& operator()(const sc_port_edgy& s);
};


class sc_sensitive_pos {
    friend class sc_module;

private:
    sc_module* module;
    enum { SC_NONE, SC_ASYNC, SC_APROC } mode;
    sc_process_b* process_handle;

    sc_sensitive_pos& operator=(const sc_sensitive_pos&);
    sc_sensitive_pos(const sc_sensitive_pos&);

    // Constructor
    sc_sensitive_pos(sc_module* p);
    ~sc_sensitive_pos();

public:
    // Changing between process handles
    sc_sensitive_pos& operator<<(const sc_async_process_handle& p);
    sc_sensitive_pos& operator<<(const sc_aproc_process_handle& p);

    // Declaring
    sc_sensitive_pos& operator<<(const sc_signal_edgy& s);
    sc_sensitive_pos& operator()(const sc_signal_edgy& s);
    sc_sensitive_pos& operator<<(const sc_port_edgy& s);
    sc_sensitive_pos& operator()(const sc_port_edgy& s);
};


class sc_sensitive_neg {
    friend class sc_module;

private:
    sc_module* module;
    enum { SC_NONE, SC_ASYNC, SC_APROC } mode;
    sc_process_b* process_handle;

    sc_sensitive_neg& operator=(const sc_sensitive_neg&);
    sc_sensitive_neg(const sc_sensitive_neg&);

    // Constructor
    sc_sensitive_neg(sc_module* p);
    ~sc_sensitive_neg();

public:
    // Changing between process handles
    sc_sensitive_neg& operator<<(const sc_async_process_handle& p);
    sc_sensitive_neg& operator<<(const sc_aproc_process_handle& p);

    // Declaring
    sc_sensitive_neg& operator<<(const sc_signal_edgy& s);
    sc_sensitive_neg& operator()(const sc_signal_edgy& s);
    sc_sensitive_neg& operator<<(const sc_port_edgy& s);
    sc_sensitive_neg& operator()(const sc_port_edgy& s);
};

#endif
