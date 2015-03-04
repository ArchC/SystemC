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

  sc_thread_pool.cpp -- Dynamic thread support on top of SystemC 2.0
                        using a pool of static threads.

  Original Author: Stuart Swan, Cadence Design Systems, Inc., 2001-08-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include <systemc.h>
#include "sc_thread_pool.h"

thread_pool* thread_pool::global_thread_pool = 0;

void thread_pool::thread_loop()
{	
        int id = _cur_id ++ ;
	spawn_info* sip = &_spawn_info_vec[id];

	while (true)
	{
	  if (sip->busy == false) // may not be false at start of sim
	     wait(sip->start);

	  (*sip->fp)(sip->ap);

	  sip->join_info->done_flag = true;
	  sip->join_info->done_event.notify();
	  sip->join_info->del_ref();
	  sip->join_info = 0;
	  sip->busy = false;
	}
}

sc_join_handle thread_pool::spawn(func_t f, arg_t a)
{
     int id;

     for (id = 0; id < _max_threads; id ++ )
	if (_spawn_info_vec[id].busy == false)
	  break;

     if (id == _max_threads)
     {
	cout << "thread_pool: No more threads available. Limit of " <<
		_max_threads << " should be increased." << endl;
	assert(false);
     }

     spawn_info* sip = &_spawn_info_vec[id];
     sip->fp = f;
     sip->ap = a;
     sip->busy = true;
     sip->join_info = new sc_join_info();
     sip->join_info->add_ref();
     sip->start.notify();

     return sc_join_handle(sip->join_info);
}
