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

    sc_array_concat.cpp - Array concatenation manager

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#include <assert.h>
#include <cstring>
#include "sc_iostream.h"
#include "sc_mempool.h"
#include "sc_array_concat.h"
#include "sc_array.h"

struct sc_array_pair {
    static void* operator new(size_t sz)        { return sc_mempool::allocate(sz); }
    static void  operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

    void* a;
    void* b;
    sc_array_pair( void* x, void* y )
        : a(x), b(y)
    {

    }
    static void*    dup(const void*);
    static void     fre(void*);
    static unsigned hsh(const void*);
    static int      cmp(const void*, const void*);
};

void*
sc_array_pair::dup(const void* p)
{
    sc_array_pair* q = (sc_array_pair*) p;
    return new sc_array_pair( q->a, q->b );
}

void
sc_array_pair::fre(void* p)
{
    delete (sc_array_pair*) p;
}

unsigned
sc_array_pair::hsh(const void* p)
{
    sc_array_pair* q = (sc_array_pair*) p;
    unsigned a = (unsigned) q->a;
    unsigned b = (unsigned) q->b;
    /* Make this asymmetric so that (a,b) and (b,a) have different
       hash values */
    return ((a >> 5) | (a << 27)) ^ ((b >> 7) | (b << 25));
}

int
sc_array_pair::cmp(const void* p1, const void* p2)
{
    sc_array_pair* q1 = (sc_array_pair*) p1;
    sc_array_pair* q2 = (sc_array_pair*) p2;
    int diff;
    if ((diff = ((char*)q1->a) - ((char*)q2->a)) != 0)
        return diff;
    else
        return (((char*)q1->b) - ((char*)q2->b));
}

struct constituent_data_struct {
    static void* operator new(size_t sz)        { return sc_mempool::allocate(sz); }
    static void  operator delete(void* p, size_t sz) { sc_mempool::release(p, sz); }

    constituent_data_struct( sc_array_concat_manager::container_t* tbl,
                             sc_array_pair* pair,
                             constituent_data_struct* nx,
                             sc_array_base* oth = 0 )
        : which_table(tbl), which_pair(pair), next(nx), other(oth)
    {
        
    }

    sc_array_concat_manager::container_t* which_table;
    sc_array_pair* which_pair;
    constituent_data_struct* next;
    sc_array_base* other;
};

static constituent_data_struct dummy_cds(0, 0, 0, 0);

sc_array_base*
sc_array_concat_manager::get_concat_array(sc_array_base* x, sc_array_base* y,
                 sc_array_base* (*new_array_fn)(sc_array_base*, sc_array_base*), bool* is_new)
{
    sc_array_pair p(x, y);
    sc_array_base* result;

    if (! table_vv.lookup(&p, &result)) {
        result = (*new_array_fn)(x, y);
        sc_array_pair* q = new sc_array_pair(x, y);
        table_vv.insert(q, result);

        x->in_concat_manager = true;
        y->in_concat_manager = true;

        constituent_data_struct* x_next = cdata_table[x];
        constituent_data_struct* new_x = new constituent_data_struct(&table_vv, q, x_next, y);
        cdata_table.insert(x, new_x);

        if (y != x) {
            constituent_data_struct* y_next = cdata_table[y];
            constituent_data_struct* new_y = new constituent_data_struct(&table_vv, q, y_next, x);
            cdata_table.insert(y, new_y);
        }

        if (is_new) *is_new = true;
    }
    else {
        if (is_new) *is_new = false;
    }
    return result;
}

sc_array_base*
sc_array_concat_manager::get_concat_array(sc_array_base* x, void* y,
                 sc_array_base* (*new_array_fn)(sc_array_base*, void*), bool* is_new)
{
    sc_array_pair p(x, y);
    sc_array_base* result;

    if (! table_vs.lookup(&p, &result)) {
        result = (*new_array_fn)(x, y);
        sc_array_pair* q = new sc_array_pair(x, y);
        table_vs.insert(q, result);

        x->in_concat_manager = true;

        constituent_data_struct* nx = cdata_table[x];
        cdata_table.insert(x, new constituent_data_struct(&table_vs, q, nx));

        if (is_new) *is_new = true;
    }
    else {
        if (is_new) *is_new = false;
    }
    return result;
}

sc_array_base*
sc_array_concat_manager::get_concat_array(void* x, sc_array_base* y,
                 sc_array_base* (*new_array_fn)(void*, sc_array_base*), bool* is_new)
{
    sc_array_pair p(x, y);
    sc_array_base* result;

    if (! table_sv.lookup(&p, &result)) {
        result = (*new_array_fn)(x, y);
        sc_array_pair* q = new sc_array_pair(x, y);

        table_sv.insert(q, result);
        y->in_concat_manager = true;

        constituent_data_struct* nx = cdata_table[y];
        cdata_table.insert(y, new constituent_data_struct(&table_sv, q, nx));

        if (is_new) *is_new = true;
    }
    else {
        if (is_new) *is_new = false;
    }
    return result;
}

sc_array_base*
sc_array_concat_manager::get_concat_array(
    void* x,
    void* y,
    sc_array_base* (*new_array_fn)(void*, void*), bool* is_new)
{
    sc_array_pair p(x, y);
    sc_array_base* result;

    if (! table_ss.lookup(&p, &result)) {
        result = (*new_array_fn)(x, y);
        table_ss.insert(&p, result, sc_array_pair::dup);
        if (is_new) *is_new = true;
    }
    else {
        if (is_new) *is_new = false;
    }
    return result;
}


sc_array_base*
sc_array_concat_manager::get_concat_array(
    sc_array_base* x,
    const char* y,
    sc_array_base* (*new_array_fn)(sc_array_base*, const char*), bool* is_new)
{
    sc_array_pair p(x, (void*) y);
    sc_array_base* result;

    if (! table_vc.lookup(&p, &result)) {
        result = (*new_array_fn)(x, y);
        sc_array_pair* q = new sc_array_pair(x, (void*) y);

        table_vc.insert(q, result);
        x->in_concat_manager = true;

        constituent_data_struct* nx = cdata_table[x];
        cdata_table.insert(x, new constituent_data_struct(&table_vs, q, nx));

        if (is_new) *is_new = true;
    }
    else if (result->length() != x->length() + (signed) strlen(y)) {
        delete result;
        result = (*new_array_fn)(x, y);
        table_vc.insert(&p, result);

        x->in_concat_manager = true;

        if (is_new) *is_new = true;
    }
    else {
        if (is_new) *is_new = false;
    }
    return result;
}

sc_array_base*
sc_array_concat_manager::get_concat_array(
    const char* x,
    sc_array_base* y,
    sc_array_base* (*new_array_fn)(const char*, sc_array_base*), bool* is_new)
{
    sc_array_pair p((void*) x, y);
    sc_array_base* result;

    if (! table_cv.lookup(&p, &result)) {
        result = (*new_array_fn)(x, y);
        sc_array_pair* q = new sc_array_pair((void*) x, y);

        table_cv.insert(q, result);

        y->in_concat_manager = true;

        constituent_data_struct* nx = cdata_table[y];
        cdata_table.insert(y, new constituent_data_struct(&table_vs, q, nx));

        if (is_new) *is_new = true;
    }
    else if (result->length() != (signed) strlen(x) + y->length()) {
        delete result;
        result = (*new_array_fn)(x, y);
        table_cv.insert(&p, result);

        y->in_concat_manager = true;

        if (is_new) *is_new = true;
    }
    else {
        if (is_new) *is_new = false;
    }
    return result;
}

/*************************************************************************

  The constituent_data_struct and the cdata_table are hacks that take
  care of destruction of concatenated arrays upon the destruction of
  either one of its components.  Basically, the table looks like this:


           cdata_table
           +-----+
           |     |
           +-----+      +----------- other
           |     |      |    +------ which_pair
           +-----+      |    |  +--- next
           |     |      |    |  |
           +-----+   +----+----+-+     +----+----+-+     +----+----+-+
         x |  o----->|  y |  o |o----->|    |    |o----->|  z |  o |/|
           +-----+   +----+--|-+-+     +----+----+-+     +----+--|-+-+
           |     |       (1) |             (2)               (3) | 
           +-----+           +------+                            +-------+
           |     |                  |                                    |
           +-----+   +----+----+-+  |  +----+----+-+     +----+----+-+   |
         y |  o----->|    |    |o---)->|  x |  o |o----->|    |    |/|   |
           +-----+   +----+----+-+  |  +----+--|-+-+     +----+----+-+   |
           |     |       (4)        |      (5) |             (6)         |
           +-----+                  |          +------+                  |
           |     |                  |                 |                  |
           +-----+   +----+----+-+  |  +----+----+-+  |  +----+----+-+   |
         z |  o----->|    |    |o---)->|    |    |o---)->|  x |  o |/|   |
           +-----+   +----+----+-+  |  +----+----+-+  |  +----+--|-+-+   |
                         (7)        |      (8)        |      (9) |       |
                                    |                 |          |       |
                                    |                 |          |       |
                                    +-------+  +------+          |  +----+
                                            V  V                 V  V
                                         +---+---+            +---+---+
                                         | x | y |            | z | x |
                                         +---+---+            +---+---+

           table_vv (indexed by pair)
           +-----+
           |     |
           +-----+
           |     |
           +-----+
           |     |
           +-----+
           |     |
+---+---+  +-----+
| x | y |==|  o-----> (x, y) (let's call it z)
+---+---+  +-----+
           |     |
           +-----+
           |     |
+---+---+  +-----+
| z | x |==|  o-----> (z, x) (let's call it w)
+---+---+  +-----+
           |     |
           +-----+
           |     |
           +-----+

Suppose x is about to be destructed.  We go through the constitutent
data list corresponding to x (see cdata_table[x], above), and try to
remove all concatenated arrays that contains x.  Thus, we find that
(x,y) (which we call z) contains x.  Hence, we will try remove cell
(5) from y's cdata list.  y is found by looking at the `other' field
of cell (1).  Now, this requires looking up cdata_table[y], and modify
that list.

We need to be careful, however, to disassociate x from cdata_table
right after getting its list.  The reason is that when we try to
delete z, during the traversal of x's list, we'll be recursively (via
the destructor of z (see sc_array_base::~sc_array_base())) traversing
the list for z.  Now when we come to cell (9), we would have a
circularity problem because we were still visiting x.  Hence, by
disassociating x from cdata_table first, when we visit cell (9), we
do not have to worry about the list for x (since the list is being
removed).

In this implementation, instead of disassociating x, we set it to point
to a dummy cell.  This is so that we can perform sanity checks (see the
two assert()s below).

*** Actually, the above scenario cannot happen, because when a new
concatenated array is created, the cds corresponding to its two
concatenands are inserted to the *front* of the list.

Sketch of proof: z must appear before y in x's list because z was
created by concatenating x and y.  Thus the order x -> z -> w -> x
is impossible.

**************************************************************************/

void
sc_array_concat_manager::remove_constituent(sc_array_base* a)
{
    constituent_data_struct* cds = cdata_table[a];
    if (! cds) return;

    /* Mark it as dummy node */
    cdata_table.insert(a, &dummy_cds);

    constituent_data_struct* safe_next = 0;
    for ( ; cds != 0; cds = safe_next) {
        safe_next = cds->next;
        sc_array_pair* pair = cds->which_pair;
        container_t* tbl = cds->which_table;

        /* Now if the table is that of two SystemC arrays, then
           we need to remove the corresponding constituent structure
           for the other array as well. */
        if (tbl == &table_vv) {
            sc_array_base* other = cds->other;
            assert(other != 0);
            if (other != a) {

                constituent_data_struct* cds_other = cdata_table[other];
                assert(cds_other);

                if (cds_other != &dummy_cds) {
                    /* Now go through the list.  If the first element of the list
                       is to be deleted, then we need to make sure that
                       cdata_table[other] is also updated correctly */
                    if (cds_other->which_pair == pair) {
                        if (cds_other->next) {
                            cdata_table.insert(other, cds_other->next);
                        } else {
                            cdata_table.remove(other);
                        }
                        // Get rid of this element.
                        delete cds_other;
                    } else {
                        constituent_data_struct* lookahead;
                        while (lookahead = cds_other->next, lookahead != 0) {
                            if (lookahead->which_pair == pair) {
                                /* Perform the bypass */
                                cds_other->next = lookahead->next;
                                delete lookahead;
                                break;
                            } else {
                                cds_other = lookahead;
                            }
                        }
                    }
                }
            }
        }

        /* Get rid of the concatenated array */
        delete (*tbl)[pair];
        tbl->remove(pair, sc_array_pair::fre);
        delete cds;
    }

    cdata_table.remove(a);
}

sc_array_concat_manager::sc_array_concat_manager()
    : table_vv(0, 31, 5, 1.5, true, sc_array_pair::hsh, sc_array_pair::cmp),
      table_vs(0, 31, 5, 1.5, true, sc_array_pair::hsh, sc_array_pair::cmp),
      table_sv(0, 31, 5, 1.5, true, sc_array_pair::hsh, sc_array_pair::cmp),
      table_ss(0, 31, 5, 1.5, true, sc_array_pair::hsh, sc_array_pair::cmp),
      table_vc(0, 31, 5, 1.5, true, sc_array_pair::hsh, sc_array_pair::cmp),
      table_cv(0, 31, 5, 1.5, true, sc_array_pair::hsh, sc_array_pair::cmp)
{


}

static void
free_concatenated_arrays( sc_array_concat_manager::container_t& table )
{
    typedef sc_array_concat_manager::container_t container_t;
    container_t::iterator it(table);
    for ( ; ! it.empty(); it++) {
        // This should invoke the appropriate destructor for the concatenated
        // array, since the destructor is declared virtual in sc_array_base.
        delete it.contents();
    }
}

void
sc_array_concat_manager::free_constituent_data
    ( sc_array_concat_manager::cdata_table_t& cdata_table_,
      sc_array_concat_manager::container_t* table_vv_ )
{
    typedef sc_array_concat_manager::cdata_table_t cdata_table_t;
    cdata_table_t::iterator it(cdata_table_);
    for ( ; ! it.empty(); it++) {
        constituent_data_struct* safe_next = 0;
        constituent_data_struct* cds = it.contents();
        sc_array_base* me = it.key();

        /* mark the concat_manager field as 0, just in
           case the array has not been destructed yet (e.g.,
           a global. */
        me->concat_manager = 0;
        me->in_concat_manager = false;

        it.set_contents(0);
        for ( ; cds != 0; cds = safe_next) {
            safe_next = cds->next;
            delete cds;
        }
    }
}

sc_array_concat_manager::~sc_array_concat_manager()
{
    free_constituent_data(cdata_table, &table_vv);

    free_concatenated_arrays(table_vv);
    free_concatenated_arrays(table_vs);
    free_concatenated_arrays(table_sv);
    free_concatenated_arrays(table_ss);
    free_concatenated_arrays(table_vc);
    free_concatenated_arrays(table_cv);

    table_vv.erase(sc_array_pair::fre);
    table_vs.erase(sc_array_pair::fre);
    table_sv.erase(sc_array_pair::fre);
    table_ss.erase(sc_array_pair::fre);
    table_vc.erase(sc_array_pair::fre);
    table_cv.erase(sc_array_pair::fre);
}

