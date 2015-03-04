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

    sc_array_concat.h - Array concatenation manager

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_ARRAY_CONCAT_H
#define SC_ARRAY_CONCAT_H

#include "sc_hash.h"

class sc_array_base;
#ifndef WIN32
class sc_array_pair;
#else
struct sc_array_pair;
#endif

struct constituent_data_struct;

class sc_array_concat_manager {
    friend class sc_array_base;
    friend struct constituent_data_struct;

public:
    typedef sc_phash<sc_array_pair*,sc_array_base*> container_t;
    typedef sc_phash<sc_array_base*,constituent_data_struct*> cdata_table_t;

    sc_array_concat_manager();
    ~sc_array_concat_manager();

    sc_array_base* get_concat_array(sc_array_base* x, sc_array_base* y,
                                    sc_array_base* (*new_array_fn)(sc_array_base*, sc_array_base*), bool* is_new = 0);
    sc_array_base* get_concat_array(sc_array_base* x, void* y,
                                    sc_array_base* (*new_array_fn)(sc_array_base*, void*), bool* is_new = 0);
    sc_array_base* get_concat_array(void* x, sc_array_base* y,
                                    sc_array_base* (*new_array_fn)(void*, sc_array_base*), bool* is_new = 0);
    sc_array_base* get_concat_array(void* x, void* y,
                                    sc_array_base* (*new_array_fn)(void*, void*), bool* is_new = 0);
    sc_array_base* get_concat_array(sc_array_base* x, const char* y,
                                    sc_array_base* (*new_array_fn)(sc_array_base*, const char*), bool* is_new = 0);
    sc_array_base* get_concat_array(const char* x, sc_array_base* y,
                                    sc_array_base* (*new_array_fn)(const char*, sc_array_base*), bool* is_new = 0);

    void remove_constituent(sc_array_base* a);

private:
    void free_constituent_data(cdata_table_t&, container_t*);

    container_t table_vv;
    container_t table_vs;
    container_t table_sv;
    container_t table_ss;

    container_t table_vc;
    container_t table_cv;

    cdata_table_t cdata_table;
};

#endif
