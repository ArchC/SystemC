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

    sc_object_manager.h -- Manager of objects (naming, &c.)

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_OBJECT_MANAGER_H
#define SC_OBJECT_MANAGER_H

class sc_module_name;

class sc_object_manager {
    friend class sc_simcontext;

public:
    typedef sc_phash<const char*, sc_object*> object_table_type;
    typedef sc_pvector<sc_object*> object_vector_type;
    typedef sc_plist<sc_object*> object_hierarchy_type;

    sc_object_manager();
    ~sc_object_manager();

    sc_object* find_object(const char* name);
    sc_object* first_object();
    sc_object* next_object();

    void hierarchy_push(sc_object* mdl);
    sc_object* hierarchy_pop();
    sc_object* hierarchy_curr();
    int hierarchy_size();

    void push_module_name(sc_module_name* mod_name);
    sc_module_name* pop_module_name();
    sc_module_name* top_of_module_name_stack();

    void insert_object(const char* name, sc_object* obj);
    void remove_object(const char* name);

private:
    object_table_type* object_table;
    object_vector_type* ordered_object_vector;
    bool ordered_object_vector_dirty;
    int next_object_index;
    object_hierarchy_type* object_hierarchy;
    sc_module_name* module_name_stack;
};

#endif
