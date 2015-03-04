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

  sc_fork.h -- Fork/Join capability in SystemC and type safe spawning
               of new processes.

  Original Author: Stuart Swan, Cadence Design Systems, Inc., 2001-08-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// Summary:
//  The proposed user-visible constructs for fork/join and dynamic thread
//  creation are:
//    sc_spawn_function(...), sc_spawn_method(...), sc_spawn_options(...),
//    SC_FORK, SC_JOIN, sc_process_join()
//  All other code herein is not intended to be user-visible.
//
// Issue: template specialization to support functions/methods returning 
// "void" works with GNU & HP compilers but not Sun CC 6.1
// Workaround: For Sun CC 6.1 and other compilers that don't properly support
// partial template specialization, uncomment the following line:
//
//    #define SC_FORK_NO_TEMP_SPCL 1
//
// and don't use sc_spawn_method/function for methods/funtions with
// void return types.
//
// Current Limitation: Maximum number of arguments to a function/method
// is currently 4. This can easily be extended, and probably will be in future.
//
//
// Initializing the Global Thread Pool
//
// SystemC 2.0 does not yet natively support dynamic thread creation, so
// we currently use a global thread pool that is allocated before simulation
// starts. The following code outlines how to allocate the global thread pool:
//
// int sc_main (int argc , char *argv[]) 
// {
//   // Create the global thread pool with a limit of 10 threads
//   thread_pool::init(10);
// 
//   top top1("Top1");
//   sc_start(-1);
// 
//   // Optionally delete the thread_pool when done
//   thread_pool::destroy();
// 
//   return 0;
// }
//
// Note that each thread will use 64K of memory for stack space by default,
// so the recommendation is to allocate as many threads as is needed, but
// allocating significantly more than is needed may be wasteful.
// 
//
// Usage Summary for Fork/Join and Dynamic Thread Creation
//
// Given the following function and method declarations:
//
//   returnT my_function( ARGS );
//
//   returnT my_class::my_method ( ARGS );
//
// To spawn in their own thread, use:
//
//   returnT r;
//
//   sc_join_handle sc_spawn_function( &r, &my_function, ARGS );
//
//   my_class* c;
//   c = this; // or point to some other object...
//
//   sc_join_handle sc_spawn_method( &r, c, &my_class::my_method, ARGS );
//
// To specify spawn options (not yet supported), add a sc_spawn_option
// object as the first argument to sc_spawn_function/method .
//
// To use fork/join:
//
// SC_FORK sc_spawn_method/function(...) , ...  SC_JOIN
// 
// Note use of "comma" to separate the sc_spawn_() calls above.
//
// To use sc_process_join() :
//
//   sc_join_handle h1 = sc_spawn_function(...);
//   sc_join_handle h2 = sc_spawn_method(...);
//
//   sc_process_join(h1);
//   sc_process_join(h2);
//
//
// Additional usage notes:
// 
// Note that storage for return values for spawned functions/methods is
// allocated by the caller and must be maintained until the spawned
// function/method completes. If the return value is not needed, pass in
// a NULL pointer casted to the appropriate pointer type.
// 
// For example:
// 
//   int foo() {return 1; }
//   sc_spawn_function((int*)0, &foo); // ignore the return value.
// 
// If spawned function/method returns void, then specify return type as void 
// and pass in NULL ptr for return ptr.
// 
// For example:
// 
//   void foo() {}
//   sc_spawn_function((void*)0, &foo); // function returns void
// 
// Note that the C++ compiler will not perform any implicit conversions when
// invoking functions/methods via sc_spawn_function/method as would be
// the case with a normal function call. Use explicit conversions
// if any are needed.
// 
// Compiler issues:
// 1. Partial template specialization to support return types of
// void works with GNU 2.95.2 C++ and HP aCC A.3.31 compilers 
// but not with Sun CC 6.1 compiler.
// 
// 2. Spawned functions or methods can be overloaded with GNU C++, but
// other compilers can't handle this yet.
// 
//

#include "sc_thread_pool.h"

class sc_spawn_options
{
public:
  explicit sc_spawn_options(const char* name = 0, bool orphan = false,
	bool method = false) :
	_name(name), _orphan(orphan), _method(method) {}

  const char* name() const { return _name; }
  bool orphan() const { return _orphan; }
  bool method() const { return _method; }

private:
  const char* 	_name;	 // name of spawned process
  bool 		_orphan; // true iff process will have no parent process
  bool 		_method; // true iff process will be an SC_METHOD, not SC_THREAD
};

inline sc_join_handle sc_spawn_low_level(
  void (*fp)(void *), void *ap, const sc_spawn_options *options)
{
	// (spawn options are not used here yet ...)

	return(thread_pool::global_thread_pool->spawn(fp, ap));
}

// Template for spawning functions and static methods
// that have no arguments in a type safe way.

template <class returnT>
class sc_0_arg_function
{
public:
  typedef returnT (*functionT)();

  static sc_join_handle spawn(returnT* ret, functionT func, 
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    info(returnT* ret, functionT func) :
	_ret(ret), _func(func) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    if (i->_ret == 0)
      (*i->_func)();
    else
      *(i->_ret) = (*i->_func)();
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for "void" return type.
template <>
class sc_0_arg_function <void>
{
public:
  typedef void returnT;
  typedef returnT (*functionT)();

  static sc_join_handle spawn(returnT* ret, functionT func,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    info(returnT* ret, functionT func) :
	_ret(ret), _func(func) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    (*i->_func)();
    delete i;
  }
};
#endif

template <class returnT>
inline sc_join_handle
sc_spawn_function(returnT *ret, returnT (*func)())
{
  return(sc_0_arg_function<returnT>::spawn(ret, func));
}

template <class returnT>
inline sc_join_handle
sc_spawn_function(const sc_spawn_options& opts,
	returnT *ret, returnT (*func)())
{
  return(sc_0_arg_function<returnT>::spawn(ret, func, &opts));
}

// Template for spawning functions and static methods
// that have a single argument in a type safe way.

template <class returnT, class a1T>
class sc_1_arg_function
{
public:
  typedef returnT (*functionT)(a1T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    info(returnT* ret, functionT func, a1T a1) :
	_ret(ret), _func(func), _a1(a1) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    if (i->_ret == 0)
      (*i->_func)(i->_a1);
    else
      *(i->_ret) = (*i->_func)(i->_a1);
    delete i;
  }
};

template <class returnT, class a1T>
inline sc_join_handle
sc_spawn_function(returnT *ret, returnT (*func)(a1T), a1T a1)
{
  return(sc_1_arg_function<returnT, a1T>::spawn(ret, func, a1));
}

template <class returnT, class a1T>
inline sc_join_handle
sc_spawn_function(const sc_spawn_options& opts,
	returnT *ret, returnT (*func)(a1T), a1T a1)
{
  return(sc_1_arg_function<returnT, a1T>::spawn(ret, func, a1, &opts));
}

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for "void" return type.
template <class a1T>
class sc_1_arg_function <void, a1T>
{
public:
  typedef void returnT;
  typedef returnT (*functionT)(a1T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    info(returnT* ret, functionT func, a1T a1) :
	_ret(ret), _func(func), _a1(a1) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    (*i->_func)(i->_a1);
    delete i;
  }
};
#endif

// Template for spawning functions and static methods
// that have two arguments in a type safe way.

template <class returnT, class a1T, class a2T>
class sc_2_arg_function
{
public:
  typedef returnT (*functionT)(a1T, a2T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1,
     a2T a2, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1, a2);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    a2T	_a2;
    info(returnT* ret, functionT func, a1T a1, a2T a2) :
	_ret(ret), _func(func), _a1(a1), _a2(a2) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    if (i->_ret == 0)
      (*i->_func)(i->_a1, i->_a2);
    else
      *(i->_ret) = (*i->_func)(i->_a1, i->_a2);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for "void" return type.
template <class a1T, class a2T>
class sc_2_arg_function <void, a1T, a2T>
{
public:
  typedef void returnT;
  typedef returnT (*functionT)(a1T, a2T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1, a2T a2,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1, a2);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    a2T	_a2;
    info(returnT* ret, functionT func, a1T a1, a2T a2) :
	_ret(ret), _func(func), _a1(a1), _a2(a2) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    (*i->_func)(i->_a1, i->_a2);
    delete i;
  }
};
#endif

template <class returnT, class a1T, class a2T>
inline sc_join_handle
sc_spawn_function(returnT *ret,
	returnT (*func)(a1T, a2T), a1T a1, a2T a2)
{
   return(sc_2_arg_function<returnT, a1T, a2T>
	::spawn(ret, func, a1, a2));
}

template <class returnT, class a1T, class a2T>
inline sc_join_handle
sc_spawn_function(const sc_spawn_options& opts,
	returnT *ret, returnT (*func)(a1T, a2T), a1T a1, a2T a2)
{
  return(sc_2_arg_function<returnT, a1T, a2T>
	::spawn(ret, func, a1, a2, &opts));
}

// Template for spawning functions and static methods
// that have three arguments in a type safe way.

template <class returnT, class a1T, class a2T, class a3T>
class sc_3_arg_function
{
public:
  typedef returnT (*functionT)(a1T, a2T, a3T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1,
     a2T a2, a3T a3, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1, a2, a3);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    info(returnT* ret, functionT func, a1T a1, a2T a2, a3T a3) :
	_ret(ret), _func(func), _a1(a1), _a2(a2), _a3(a3) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    if (i->_ret == 0)
      (*i->_func)(i->_a1, i->_a2, i->_a3);
    else
      *(i->_ret) = (*i->_func)(i->_a1, i->_a2, i->_a3);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for "void" return type.
template <class a1T, class a2T, class a3T>
class sc_3_arg_function <void, a1T, a2T, a3T>
{
public:
  typedef void returnT;
  typedef returnT (*functionT)(a1T, a2T, a3T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1, a2T a2,
	a3T a3, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1, a2, a3);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    info(returnT* ret, functionT func, a1T a1, a2T a2, a3T a3) :
	_ret(ret), _func(func), _a1(a1), _a2(a2), _a3(a3) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    (*i->_func)(i->_a1, i->_a2, i->_a3);
    delete i;
  }
};
#endif

template <class returnT, class a1T, class a2T, class a3T>
inline sc_join_handle
sc_spawn_function(returnT *ret,
	returnT (*func)(a1T, a2T, a3T), a1T a1, a2T a2, a3T a3)
{
   return(sc_3_arg_function<returnT, a1T, a2T, a3T>
	::spawn(ret, func, a1, a2, a3));
}

template <class returnT, class a1T, class a2T, class a3T>
inline sc_join_handle
sc_spawn_function(const sc_spawn_options& opts,
	returnT *ret, returnT (*func)(a1T, a2T, a3T), a1T a1, a2T a2, a3T a3)
{
  return(sc_3_arg_function<returnT, a1T, a2T, a3T>
	::spawn(ret, func, a1, a2, a3, &opts));
}

// Template for spawning functions and static methods
// that have four arguments in a type safe way.

template <class returnT, class a1T, class a2T, class a3T, class a4T>
class sc_4_arg_function
{
public:
  typedef returnT (*functionT)(a1T, a2T, a3T, a4T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1,
     a2T a2, a3T a3, a4T a4, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1, a2, a3, a4);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    a4T	_a4;
    info(returnT* ret, functionT func, a1T a1, a2T a2, a3T a3, a4T a4) :
	_ret(ret), _func(func), _a1(a1), _a2(a2), _a3(a3), _a4(a4) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    if (i->_ret == 0)
      (*i->_func)(i->_a1, i->_a2, i->_a3, i->_a4);
    else
      *(i->_ret) = (*i->_func)(i->_a1, i->_a2, i->_a3, i->_a4);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for "void" return type.
template <class a1T, class a2T, class a3T, class a4T>
class sc_4_arg_function <void, a1T, a2T, a3T, a4T>
{
public:
  typedef void returnT;
  typedef returnT (*functionT)(a1T, a2T, a3T, a4T);

  static sc_join_handle spawn(returnT* ret, functionT func, a1T a1, a2T a2,
	a3T a3, a4T a4, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, func, a1, a2, a3, a4);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    functionT	_func;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    a4T	_a4;
    info(returnT* ret, functionT func, a1T a1, a2T a2, a3T a3, a4T a4) :
	_ret(ret), _func(func), _a1(a1), _a2(a2), _a3(a3), _a4(a4) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    (*i->_func)(i->_a1, i->_a2, i->_a3, i->_a4);
    delete i;
  }
};
#endif

template <class returnT, class a1T, class a2T, class a3T, class a4T>
inline sc_join_handle
sc_spawn_function(returnT *ret,
	returnT (*func)(a1T, a2T, a3T, a4T), a1T a1, a2T a2, a3T a3, a4T a4)
{
   return(sc_4_arg_function<returnT, a1T, a2T, a3T, a4T>
	::spawn(ret, func, a1, a2, a3, a4));
}

template <class returnT, class a1T, class a2T, class a3T, class a4T>
inline sc_join_handle
sc_spawn_function(const sc_spawn_options& opts,
	returnT *ret, returnT (*func)(a1T, a2T, a3T, a4T),
	a1T a1, a2T a2, a3T a3, a4T a4)
{
  return(sc_4_arg_function<returnT, a1T, a2T, a3T, a4T>
	::spawn(ret, func, a1, a2, a3, a4, &opts));
}


// Template for spawning non-static methods within classes
// that have no arguments in a type safe way.

template <class returnT, class objectT>
class sc_0_arg_method
{
public:
  typedef returnT (objectT::*methodT)();

  static sc_join_handle spawn(
      returnT* ret, objectT* obj, methodT method,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    info(returnT* ret, objectT* obj, methodT method) :
	_ret(ret), _obj(obj), _method(method) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    if (i->_ret == 0)
      (i->_obj->*method)();
    else
      *(i->_ret) = (i->_obj->*method)();
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for void return type
template <class objectT>
class sc_0_arg_method <void, objectT>
{
public:
  typedef void returnT;
  typedef returnT (objectT::*methodT)();

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, 
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    info(returnT* ret, objectT* obj, methodT method) :
	_ret(ret), _obj(obj), _method(method) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    (i->_obj->*method)();
    delete i;
  }
};
#endif

template <class returnT, class objectT>
inline sc_join_handle
sc_spawn_method(returnT *ret, objectT* obj,
	returnT (objectT::*func)())
{
	return(sc_0_arg_method<returnT, objectT>
	  ::spawn(ret, obj, func));
}

template <class returnT, class objectT>
inline sc_join_handle
sc_spawn_method(const sc_spawn_options& opts, returnT *ret, objectT* obj,
	returnT (objectT::*func)())
{
	return(sc_0_arg_method<returnT, objectT>
	   ::spawn(ret, obj, func, &opts));
}

// Template for spawning non-static methods within classes
// that have a single argument in a type safe way.

template <class returnT, class objectT, class a1T>
class sc_1_arg_method
{
public:
  typedef returnT (objectT::*methodT)(a1T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    info(returnT* ret, objectT* obj, methodT method, a1T a1) :
	_ret(ret), _obj(obj), _method(method), _a1(a1) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    if (i->_ret == 0)
      (i->_obj->*method)(i->_a1);
    else
      *(i->_ret) = (i->_obj->*method)(i->_a1);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for void return type
template <class objectT, class a1T>
class sc_1_arg_method <void, objectT, a1T>
{
public:
  typedef void returnT;
  typedef returnT (objectT::*methodT)(a1T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    info(returnT* ret, objectT* obj, methodT method, a1T a1) :
	_ret(ret), _obj(obj), _method(method), _a1(a1) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    (i->_obj->*method)(i->_a1);
    delete i;
  }
};
#endif

template <class returnT, class objectT, class a1T>
inline sc_join_handle
sc_spawn_method(returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T), a1T a1)
{
	return(sc_1_arg_method<returnT, objectT, a1T>
	  ::spawn(ret, obj, func, a1));
}

template <class returnT, class objectT, class a1T>
inline sc_join_handle
sc_spawn_method(const sc_spawn_options& opts, returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T), a1T a1)
{
	return(sc_1_arg_method<returnT, objectT, a1T>
	   ::spawn(ret, obj, func, a1, &opts));
}

// Template for spawning non-static methods within classes
// that have two arguments in a type safe way.

template <class returnT, class objectT, class a1T, class a2T>
class sc_2_arg_method
{
public:
  typedef returnT (objectT::*methodT)(a1T, a2T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1, a2);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    a2T	_a2;
    info(returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2) :
	_ret(ret), _obj(obj), _method(method), _a1(a1), _a2(a2) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    if (i->_ret == 0)
      (i->_obj->*method)(i->_a1, i->_a2);
    else
      *(i->_ret) = (i->_obj->*method)(i->_a1, i->_a2);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for void return type
template <class objectT, class a1T, class a2T>
class sc_2_arg_method <void, objectT, a1T, a2T>
{
public:
  typedef void returnT;
  typedef returnT (objectT::*methodT)(a1T, a2T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1, a2);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    a2T	_a2;
    info(returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2) :
	_ret(ret), _obj(obj), _method(method), _a1(a1), _a2(a2) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    (i->_obj->*method)(i->_a1, i->_a2);
    delete i;
  }
};
#endif

template <class returnT, class objectT, class a1T, class a2T>
inline sc_join_handle
sc_spawn_method(returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T, a2T), a1T a1, a2T a2)
{
	return(sc_2_arg_method<returnT, objectT, a1T, a2T>
	  ::spawn(ret, obj, func, a1, a2));
}

template <class returnT, class objectT, class a1T, class a2T>
inline sc_join_handle
sc_spawn_method(const sc_spawn_options& opts, returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T, a2T), a1T a1, a2T a2)
{
	return(sc_2_arg_method<returnT, objectT, a1T, a2T>
	   ::spawn(ret, obj, func, a1, a2, &opts));
}

// Template for spawning non-static methods within classes
// that have 3 arguments in a type safe way.

template <class returnT, class objectT, class a1T, class a2T, class a3T>
class sc_3_arg_method
{
public:
  typedef returnT (objectT::*methodT)(a1T, a2T, a3T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2,
	a3T a3, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1, a2, a3);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    info(returnT* ret, objectT* obj, methodT method,
	a1T a1, a2T a2, a3T a3) :
	_ret(ret), _obj(obj), _method(method), _a1(a1), _a2(a2),
	_a3(a3) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    if (i->_ret == 0)
      (i->_obj->*method)(i->_a1, i->_a2, i->_a3);
    else
      *(i->_ret) = (i->_obj->*method)(i->_a1, i->_a2, i->_a3);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for void return type
template <class objectT, class a1T, class a2T, class a3T>
class sc_3_arg_method <void, objectT, a1T, a2T, a3T>
{
public:
  typedef void returnT;
  typedef returnT (objectT::*methodT)(a1T, a2T, a3T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2, a3T a3,
	const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1, a2, a3);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    info(returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2, a3T a3) :
	_ret(ret), _obj(obj), _method(method), _a1(a1), _a2(a2), _a3(a3) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    (i->_obj->*method)(i->_a1, i->_a2, i->_a3);
    delete i;
  }
};
#endif

template <class returnT, class objectT, class a1T, class a2T, class a3T>
inline sc_join_handle
sc_spawn_method(returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T, a2T, a3T), a1T a1, a2T a2,
	a3T a3)
{
	return(sc_3_arg_method<returnT, objectT, a1T, a2T, a3T>
	  ::spawn(ret, obj, func, a1, a2, a3));
}

template <class returnT, class objectT, class a1T, class a2T, class a3T>
inline sc_join_handle
sc_spawn_method(const sc_spawn_options& opts, returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T, a2T, a3T),
	a1T a1, a2T a2, a3T a3)
{
	return(sc_3_arg_method<returnT, objectT, a1T, a2T, a3T>
	   ::spawn(ret, obj, func, a1, a2, a3, &opts));
}

// Template for spawning non-static methods within classes
// that have 4 arguments in a type safe way.

template <class returnT, class objectT,
	class a1T, class a2T, class a3T, class a4T>
class sc_4_arg_method
{
public:
  typedef returnT (objectT::*methodT)(a1T, a2T, a3T, a4T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2,
	a3T a3, a4T a4, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1, a2, a3, a4);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    a4T	_a4;
    info(returnT* ret, objectT* obj, methodT method,
	a1T a1, a2T a2, a3T a3, a4T a4) :
	_ret(ret), _obj(obj), _method(method), _a1(a1), _a2(a2),
	_a3(a3), _a4(a4) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    if (i->_ret == 0)
      (i->_obj->*method)(i->_a1, i->_a2, i->_a3, i->_a4);
    else
      *(i->_ret) = (i->_obj->*method)(i->_a1, i->_a2, i->_a3, i->_a4);
    delete i;
  }
};

#ifndef SC_FORK_NO_TEMP_SPCL
// Template specialization for void return type
template <class objectT, class a1T, class a2T, class a3T, class a4T>
class sc_4_arg_method <void, objectT, a1T, a2T, a3T, a4T>
{
public:
  typedef void returnT;
  typedef returnT (objectT::*methodT)(a1T, a2T, a3T, a4T);

  static sc_join_handle spawn(
	returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2, a3T a3,
	a4T a4, const sc_spawn_options * options=0)
  {
    info* i = new info(ret, obj, method, a1, a2, a3, a4);
    return sc_spawn_low_level(exec, i, options);
  }

private:
  struct info {
    returnT*	_ret;
    objectT*	_obj;
    methodT	_method;
    a1T	_a1;
    a2T	_a2;
    a3T	_a3;
    a4T	_a4;
    info(returnT* ret, objectT* obj, methodT method, a1T a1, a2T a2, a3T a3,
	a4T a4) :
	_ret(ret), _obj(obj), _method(method), _a1(a1), _a2(a2), _a3(a3),
	_a4(a4) {}
  };

  static void exec(void* a) {
    info* i = (info*) a;
    methodT method = i->_method;
    (i->_obj->*method)(i->_a1, i->_a2, i->_a3, i->_a4);
    delete i;
  }
};
#endif

template <class returnT, class objectT,
	class a1T, class a2T, class a3T, class a4T>
inline sc_join_handle
sc_spawn_method(returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T, a2T, a3T, a4T),
	a1T a1, a2T a2, a3T a3, a4T a4)
{
	return(sc_4_arg_method<returnT, objectT, a1T, a2T, a3T, a4T>
	  ::spawn(ret, obj, func, a1, a2, a3, a4));
}

template <class returnT, class objectT,
	class a1T, class a2T, class a3T, class a4T>
inline sc_join_handle
sc_spawn_method(const sc_spawn_options& opts, returnT *ret, objectT* obj,
	returnT (objectT::*func)(a1T, a2T, a3T, a4T),
	a1T a1, a2T a2, a3T a3, a4T a4)
{
	return(sc_4_arg_method<returnT, objectT, a1T, a2T, a3T, a4T>
	   ::spawn(ret, obj, func, a1, a2, a3, a4, &opts));
}

// Public function to allow one process to wait for another to complete:

inline void sc_process_join(sc_join_handle p)
{
  thread_pool::global_thread_pool->join(p);
}

#define SC_FORK	  { sc_join_handle sc_fork_V[] = {
#define SC_JOIN	  }; for (unsigned sc_fork_I=0; \
	sc_fork_I < sizeof(sc_fork_V)/sizeof(sc_fork_V[0]); sc_fork_I++) \
				sc_process_join(sc_fork_V[sc_fork_I]); }


