/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2001 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_thread_pool.h -- Dynamic thread support on top of SystemC 2.0
                      using a pool of static threads.

  Original Author: Stuart Swan, Cadence Design Systems, Inc., 2001-08-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


class sc_join_info
{
public:

  friend class sc_join_handle;
  friend class thread_pool;

private:
  sc_join_info() : done_flag(false), _refcnt(0) {}
  void add_ref() { ++ _refcnt; }
  void del_ref() { if ( -- _refcnt == 0) delete this; }

  bool done_flag;
  sc_event done_event;

  unsigned _refcnt;
};

// Note that some sort of smart pointer or reference counting implementation
// for sc_join_handle is necessary since sc_join_info objects may
// need to survive until sc_process_join() is called. Alternatively,
// if sc_process_join() is never called for a spawned process (which is
// perfectly acceptable) then we need to be smart enough to destroy the
// sc_join_info object.
// Using simple integers or pointers for sc_join_handle (ala POSIX pthreads)
// is unacceptable since there may be huge numbers of spawned processes
// in SystemC over the course of a simulation.
// Here I use a simple form of reference counting for sc_join_info.

class sc_join_handle
{
public:
  explicit sc_join_handle(sc_join_info* i) : ptr(i) { ptr->add_ref(); }

  sc_join_handle( const sc_join_handle& h) : ptr(h.ptr) { ptr->add_ref(); }

  sc_join_handle() : ptr(0) {}

  ~sc_join_handle() { ptr->del_ref(); }

  sc_join_handle& operator = ( const sc_join_handle& h)
  {
    if (&h != this)
    {
      if (ptr) ptr->del_ref();

      ptr = h.ptr;

      if (ptr) ptr->add_ref();
    }

    return *this;
  }

  friend class thread_pool;

private:
  sc_join_info* ptr;
};

class thread_pool : public sc_module
{
public:
  SC_HAS_PROCESS(thread_pool);

  thread_pool(sc_module_name name, int max_threads = 100) :
	sc_module(name), _max_threads(max_threads), _cur_id(0)
  {
	_spawn_info_vec = new spawn_info [_max_threads];

	for (int i = 0; i < _max_threads; i++)
	  simcontext()->register_thread_process("", 
	    SC_MAKE_FUNC_PTR(SC_CURRENT_USER_MODULE, thread_loop), this);
  }

  ~thread_pool() { delete [] _spawn_info_vec; }

  typedef void (*func_t)(void *); 
  typedef void* arg_t;

  sc_join_handle spawn(func_t f, arg_t a);

  void join(sc_join_handle h)
  {
     if (h.ptr->done_flag == false)
	wait(h.ptr->done_event);
  }

  static thread_pool* global_thread_pool;

  static void init(int sz = 100)
  {
    assert( global_thread_pool == 0 );
    global_thread_pool = new thread_pool("tp1", sz);
  }

  static void destroy()
  {
    delete global_thread_pool;
    global_thread_pool = 0;
  }

private:
  class spawn_info
  {
  public:
    spawn_info() : fp(0), ap(0), busy(false), join_info(0) {}
    func_t fp;
    arg_t ap;
    bool busy;
    sc_join_info *join_info;
    sc_event start;
  };

  const int _max_threads;
  int _cur_id;
  spawn_info *_spawn_info_vec;

  void thread_loop();
};

