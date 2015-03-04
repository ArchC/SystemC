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

  demo.cpp -- Demo fork/join and dynamic thread creation within SystemC 2.0.

              The proposed public API for fork/join and dynamic thread
              creation in SystemC are:

                  sc_spawn_function(...)
                  sc_spawn_method(...)
                  sc_spawn_options(...)
                  SC_FORK
		  SC_JOIN
                  sc_process_join()

              The use of all of these constructs is demonstrated below.

  Original Author: Stuart Swan, Cadence Design Systems, Inc., 2001-08-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <systemc.h>
#include "sc_fork.h"

int test_function(double d)
{
  cout << endl << "Test_function sees " << d << endl;
  return int(d);
}

class top : public sc_module
{
public:
  SC_HAS_PROCESS(top);

  top(sc_module_name name) : sc_module(name) 
  {
     SC_THREAD(main);
  }

  void main()
  {
    int r;
    sc_event e1, e2, e3, e4;

    cout << endl;

    e1.notify(100, SC_NS);

    // Spawn several threads that co-operatively execute in round robin order

    SC_FORK
      sc_spawn_method(&r, this, &top::round_robin, "1", &e1, &e2, 3),
      sc_spawn_method(&r, this, &top::round_robin, "2", &e2, &e3, 3),
      sc_spawn_method(&r, this, &top::round_robin, "3", &e3, &e4, 3),
      sc_spawn_method(&r, this, &top::round_robin, "4", &e4, &e1, 3),
    SC_JOIN

    cout << "Returned int is " << r << endl;
    cout << endl << endl;

    // Test that threads in thread pool are successfully reused ...

    for (int i = 0 ; i < 10; i++)
      sc_spawn_method(&r, this, &top::wait_and_end, i);

    wait(20, SC_NS);

    // Test thread reuse, & show how to use sc_spawn_options 

    for (int i = 0 ; i < 10; i++)
      sc_spawn_method(sc_spawn_options("nm"), &r, this, &top::wait_and_end, i);

    wait(20, SC_NS);

    // Demo of a function rather than method call, & use return value ...
    
    sc_join_handle h = sc_spawn_function(&r, &test_function, 3.1459);
    sc_process_join(h);

    cout << "Returned int is " << r << endl;

    cout << endl << "Done." << endl;
  }

  int round_robin(const char *str, sc_event* receive, sc_event* send, int cnt)
  {
    while (--cnt >= 0)
    {
      wait(*receive);
      cout << "Round robin thread " << str <<
		" at time " << sc_time_stamp() << endl;
      wait(10, SC_NS);
      send->notify();
    }

    return 0;
  }

  int wait_and_end(int i)
  {
    wait(10, SC_NS);
    cout << "Thread " << i << " ending." << endl;
    return 0;
  }
};

int sc_main (int argc , char *argv[]) 
{
  // Create the global thread pool with a limit of 10 threads

  thread_pool::init(10);

  top top1("Top1");
  sc_start(-1);

  // To be tidy, let's delete the thread_pool when done

  thread_pool::destroy();

  return 0;
}
