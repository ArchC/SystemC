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

    sc_object_manager.cpp -- Manager of objects (naming, &c.)

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <cstring>

#include "sc_iostream.h"
#include "sc_object.h"
#include "sc_hash.h"
#include "sc_list.h"
#include "sc_vector.h"
#include "sc_mempool.h"
#include "sc_object_manager.h"
#include "sc_module_name.h"

int strcmp_void(const void* a, const void* b)
{
  return strcmp(static_cast<const char*>(a),static_cast<const char*>(b));
}
sc_object_manager::sc_object_manager()
{
    ordered_object_vector = new object_vector_type;

    object_table = new object_table_type;
    object_table->set_hash_fn(default_str_hash_fn);
    object_table->set_cmpr_fn(strcmp_void);

    object_hierarchy = new object_hierarchy_type;
    ordered_object_vector_dirty = true;
    next_object_index = 0;
    module_name_stack = 0;
}

sc_object_manager::~sc_object_manager()
{
    delete object_hierarchy;

    /* Go through each object in the table, and
       mark the simc field of the object NULL */
    object_table_type::iterator it(object_table);
    for ( ; !it.empty(); it++) {
        sc_object* obj = it.contents();
        obj->simc = 0;
    }
    delete object_table;
    delete ordered_object_vector;
}

sc_object*
sc_object_manager::find_object(const char* name)
{
    return (*object_table)[name];
}

#if !defined(__HP_aCC)
extern "C" {
#endif
  static int
    object_name_compare(const void* o1, const void* o2)
    {
      const sc_object* obj1 = *(const sc_object**) o1;
      const sc_object* obj2 = *(const sc_object**) o2;
      return strcmp(obj1->name(), obj2->name());
    }
#if !defined(__HP_aCC)
}
#endif

sc_object*
sc_object_manager::first_object()
{
    if (ordered_object_vector_dirty) {
        ordered_object_vector->erase_all();
        object_table_type::iterator it(object_table);
        while (! it.empty()) {
            sc_object* obj = it.contents();
            ordered_object_vector->push_back(obj);
            it++;
        }
        ordered_object_vector->sort(object_name_compare);
        ordered_object_vector_dirty = false;
    }
    next_object_index = 0;
    return next_object();
}

sc_object*
sc_object_manager::next_object()
{
    assert( ! ordered_object_vector_dirty );
    if ( next_object_index >= ordered_object_vector->size() )
        return 0;
    else
        return (*ordered_object_vector)[next_object_index++];

}

void
sc_object_manager::hierarchy_push(sc_object* mdl)
{
    object_hierarchy->push_front(mdl);
}

sc_object*
sc_object_manager::hierarchy_pop()
{
    return object_hierarchy->pop_front();
}

sc_object*
sc_object_manager::hierarchy_curr()
{
    return object_hierarchy->empty() ? 0 : object_hierarchy->front();
}

int
sc_object_manager::hierarchy_size()
{
    return object_hierarchy->size();
}

void
sc_object_manager::push_module_name(sc_module_name* mod_name)
{
    mod_name->next = module_name_stack;
    module_name_stack = mod_name;
}

sc_module_name*
sc_object_manager::pop_module_name()
{
    sc_module_name* mod_name = module_name_stack;
    module_name_stack = module_name_stack->next;
    mod_name->next = 0;
    return mod_name;
}

sc_module_name*
sc_object_manager::top_of_module_name_stack()
{
    return module_name_stack;
}

void
sc_object_manager::insert_object(const char* name, sc_object* obj)
{
    object_table->insert(name, obj);
    ordered_object_vector_dirty = true;
}

void
sc_object_manager::remove_object(const char* name)
{
    object_table->remove(name);
    ordered_object_vector_dirty = true;
}
