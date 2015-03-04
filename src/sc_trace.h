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

     sc_trace.h - Functions for tracing signals and variables

     Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

/****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, the basics are identical to what was
   originally contributed by Infineon.  The contribution of Infineon
   in the development of this tracing technology is hereby
   acknowledged.

*****************************************************************************/

#ifndef SC_TRACE_H
#define SC_TRACE_H

#include <stdio.h>

#include "sc_string.h"

//Some forward declarations
class sc_logic;
class sc_logic_vector;
class sc_bool_vector;
class sc_signed;
class sc_unsigned;
class sc_int_base;
class sc_uint_base;
class sc_bit;
namespace sc_bv_ns
{
  class sc_bv_base;
  class sc_lv_base;
}
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv_base;

//#ifdef SC_INCLUDE_FX
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;
//#endif
class sc_signal_bool_vector;
class sc_signal_logic_vector;
class sc_logic_resolve;
template <class T, class R> class sc_signal_resolved_t;
typedef sc_signal_resolved_t<sc_logic, sc_logic_resolve> sc_signal_resolved;
template <class T> class sc_signal;
template <class A> class sc_signal_array;
template <class T> class sc_channel;
template <class A> class sc_channel_array;
class sc_signal_resolved_vector;

// Base class for all kinds of trace files. 

class sc_trace_file {
    friend class sc_simcontext;
    
public:
    // Constructor
    sc_trace_file();
    
    // All functions are pure virtual because they need to be defined by the
    // particular tracing mechanism
    
    // Trace a boolean object (single bit)
    virtual void trace(const bool& object, const sc_string& name) = 0;

    // Trace a sc_bit object (single bit)
    virtual void trace(const sc_bit& object, const sc_string& name) = 0;

    // Trace a sc_logic object (single bit)
    virtual void trace(const sc_logic& object, const sc_string& name) = 0;

    // Trace a sc_bool_vector
    virtual void trace(const sc_bool_vector& object, const sc_string& name) = 0;

    // Trace a sc_logic_vector
    virtual void trace(const sc_logic_vector& object, const sc_string& name) = 0;
    
    // Trace an unsigned char with the given width
    virtual void trace(const unsigned char& object, const sc_string& name, int width) = 0;

    // Trace an unsigned short with the given width
    virtual void trace(const unsigned short& object, const sc_string& name, int width) = 0;

    // Trace an unsigned int with the given width
    virtual void trace(const unsigned int& object, const sc_string& name, int width) = 0;

    // Trace an unsigned long with the given width
    virtual void trace(const unsigned long& object, const sc_string& name, int width) = 0;

    // Trace a signed char with the given width
    virtual void trace(const char& object, const sc_string& name, int width) = 0;

    // Trace a signed short with the given width
    virtual void trace(const short& object, const sc_string& name, int width) = 0;

    // Trace a signed int with the given width
    virtual void trace(const int& object, const sc_string& name, int width) = 0;

    // Trace a signed long with the given width
    virtual void trace(const long& object, const sc_string& name, int width) = 0;
    
    // Trace a float
    virtual void trace(const float& object, const sc_string& name) = 0;

    // Trace a double
    virtual void trace(const double& object, const sc_string& name) = 0;

    // Trace sc_signal<sc_bool_vector>
    virtual void trace(const sc_signal_bool_vector& object, const sc_string& name) = 0;

    // Trace sc_signal<sc_logic_vector>
    virtual void trace(const sc_signal_logic_vector& object, const sc_string& name) = 0;

    // Trace sc_uint_base
    virtual void trace (const sc_uint_base& object, const sc_string& name) = 0;

    // Trace sc_int_base
    virtual void trace (const sc_int_base& object, const sc_string& name) = 0;

    // Trace sc_unsigned
    virtual void trace (const sc_unsigned& object, const sc_string& name) = 0;

    // Trace sc_signed
    virtual void trace (const sc_signed& object, const sc_string& name) = 0;

#ifdef SC_INCLUDE_FX
    // Trace sc_fxval
    virtual void trace( const sc_fxval& object,
			const sc_string& name ) = 0;

    // Trace sc_fxval_fast
    virtual void trace( const sc_fxval_fast& object,
			const sc_string& name ) = 0;

    // Trace sc_fxnum
    virtual void trace( const sc_fxnum& object,
			const sc_string& name ) = 0;

    // Trace sc_fxnum_fast
    virtual void trace( const sc_fxnum_fast& object,
			const sc_string& name ) = 0;
#endif

    // Trace resolved signal
    virtual void trace(const sc_signal_resolved& object, const sc_string& name)=0;

    // Trace resolved signal vector
    virtual void trace(const sc_signal_resolved_vector& object,
                       const sc_string& name) = 0;

    // Trace sc_bv_base (sc_bv)
    virtual void trace(const sc_bv_ns::sc_bv_base& object, const sc_string& name)=0;

    // Trace sc_lv_base (sc_lv)
    virtual void trace(const sc_bv_ns::sc_lv_base& object, const sc_string& name)=0;

    // Trace an enumerated object - where possible output the enumeration literals
    // in the trace file. Enum literals is a null terminated array of null
    // terminated char* literal strings.
    virtual void trace(const unsigned int& object, const sc_string& name,
                       const char** enum_literals) = 0;

    // Output a comment to the trace file
    virtual void write_comment(const sc_string& comment) = 0;

    // Set the amount of space before next column
    // (For most formats this does nothing)
    virtual void space(int n);

    // Also trace transitions between delta cycles if flag is true.
    virtual void delta_cycles(bool flag);

protected:
    // Write trace info for cycle.
    virtual void cycle(bool delta_cycle) = 0;

    // Flush results and close file.
    virtual ~sc_trace_file() { /* Intentionally blank */ };
};

/***************************************************************************************************/

// Now comes all the SystemC defined tracing functions
// We define two sc_trace() versions for scalar types; one where the object to be traced 
// is passed as  a reference and the other where a pointer to the tracing object is passed.

// HPUX compiler aCC has a bug in overload resolution which causes
// the ambiguity between pointer-to-class -> pointer-to-base and -> const bool&
// interesting that it doesn't have problems with just bool

#if !defined(__HP_aCC)
inline void sc_trace(sc_trace_file* tf, const bool& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}
#endif

inline void sc_trace(sc_trace_file* tf, const sc_bit& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file* tf, const bool* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}
    
inline void sc_trace(sc_trace_file* tf, const sc_logic& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file* tf, const sc_logic* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_bool_vector& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_bool_vector* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

inline void sc_trace(sc_trace_file* tf, const sc_logic_vector& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file* tf, const sc_logic_vector* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

inline void sc_trace(sc_trace_file *tf, const unsigned char& object, const sc_string& name, int width = 8 * sizeof(char))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const unsigned char* object, const sc_string& name, int width = 8 * sizeof(char))
{
    if (tf) tf->trace(*object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const unsigned short& object, const sc_string& name, int width = 8 * sizeof(short))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const unsigned short* object, const sc_string& name, int width = 8 * sizeof(short))
{
    if (tf) tf->trace(*object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const unsigned int& object, const sc_string& name, int width = 8 * sizeof(int))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const unsigned int* object, const sc_string& name, int width = 8 * sizeof(int))
{
    if (tf) tf->trace(*object, name, width);
}


inline void sc_trace(sc_trace_file *tf, const unsigned long& object, const sc_string& name, int width = 8 * sizeof(long))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const unsigned long* object, const sc_string& name, int width = 8 * sizeof(long))
{
    if (tf) tf->trace(*object, name, width);
}


inline void sc_trace(sc_trace_file *tf, const char& object, const sc_string& name, int width = 8 * sizeof(char))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const char* object, const sc_string& name, int width = 8 * sizeof(char))
{
    if (tf) tf->trace(*object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const short& object, const sc_string& name, int width = 8 * sizeof(short))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const short* object, const sc_string& name, int width = 8 * sizeof(short))
{
    if (tf) tf->trace(*object, name, width);
}


inline void sc_trace(sc_trace_file *tf, const int& object, const sc_string& name, int width = 8 * sizeof(int))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const int* object, const sc_string& name, int width = 8 * sizeof(int))
{
    if (tf) tf->trace(*object, name, width);
}


inline void sc_trace(sc_trace_file *tf, const long& object, const sc_string& name, int width = 8 * sizeof(long))
{
    if (tf) tf->trace(object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const long* object, const sc_string& name, int width = 8 * sizeof(long))
{
    if (tf) tf->trace(*object, name, width);
}

inline void sc_trace(sc_trace_file *tf, const float& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const float* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

inline void sc_trace(sc_trace_file *tf, const double& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const double* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

template< class T > 
inline void sc_trace(sc_trace_file *tf, const sc_signal<T>& object, const sc_string& name)
{
    sc_trace(tf, *(object.get_data_ptr()), name);
}

// Just the above template function will not work unless the last argument happens to be 
// an sc_string type. Assume you call this function with a char * as the last argument. 
// Remember each signal of type T has an implicit convert to variable of type T. Therefore, 
// what gcc does is uses the implicit convert and converts the signal to a variable (in effect
// reading it) and then calls the trace function for the appropriate data type. Therefore,
// if you call sc_trace(tf, sc_signal<int>, char *), gcc will end up calling 
// sc_trace(tf, int, char *). Obviously this is the wrong thing. 
// The workaround, and a nasty one at that, is to have the following function too. In this case, 
// gcc finds an exact match if you call sc_trace(tf, sc_signal<int>, char *) and therefore, no
// implicit converts are used.

template< class T >
inline void sc_trace(sc_trace_file *tf, const sc_signal<T>& object, const char* name)
{
    sc_trace(tf, *(object.get_data_ptr()), name);
}

// Specializations for signals of type int, char, short, long
void sc_trace(sc_trace_file *tf, const sc_signal<char>& object, const sc_string& name, int width);


void sc_trace(sc_trace_file *tf, const sc_signal<short>& object, const sc_string& name, int width);


void sc_trace(sc_trace_file *tf, const sc_signal<int>& object, const sc_string& name, int width);


void sc_trace(sc_trace_file *tf, const sc_signal<long>& object, const sc_string& name, int width);


inline void sc_trace(sc_trace_file *tf, const sc_signal_bool_vector& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}


inline void sc_trace(sc_trace_file *tf, const sc_signal_logic_vector& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_int_base& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_int_base* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_uint_base& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_uint_base* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_signed& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_signed* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}


inline void sc_trace(sc_trace_file *tf, const sc_unsigned& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_unsigned* object, const sc_string& name)
{
    if (tf) tf->trace(*object, name);
}

#if !defined(__HP_aCC)
inline void sc_trace(sc_trace_file *tf, const sc_signal_resolved& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}
#endif

inline void sc_trace(sc_trace_file *tf, const sc_signal_resolved_vector& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}

inline void sc_trace(sc_trace_file *tf, const sc_bv_base& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}
inline void sc_trace(sc_trace_file *tf, const sc_lv_base& object, const sc_string& name)
{
    if (tf) tf->trace(object, name);
}
// 1. non-template function is better than template
// 2. more-specialized template is better than less-specialized
// 3. no partial specialization for template functions
/*
// doesn't work because unknown types will be instantiated (design defect)
template<class T>
class sc_trace_helper
{
  public:
  void trace(sc_trace_file *tf, const T& object, const sc_string& name)
  { if (tf) tf->trace(object, name); }
};
template<class T>
class sc_trace_helper<const T*>
{
  public:
  void trace(sc_trace_file *tf, const T* object, const sc_string& name)
  { if (tf) tf->trace(*object, name); }
};
template<class T>
void sc_trace(sc_trace_file *tf, T& object, const sc_string& name)
{ sc_trace_helper<T>().trace(tf,object,name); }
*/
// Trace an enumerated object - where possible output the enumeration literals
// in the trace file. Enum literals is a null terminated array of null
// terminated char* literal strings.
inline void sc_trace(sc_trace_file* tf, const unsigned int& object, const sc_string& name, const char** enum_literals)
{
    if (tf) tf->trace(object, name, enum_literals);
}


#if defined(__BCPLUSPLUS__)
#pragma warn -8027
// inline will not be effective for functions with loops
#endif
// Functions for tracing channels
template< class T >
inline
void sc_trace(sc_trace_file *tf, const sc_channel<T>& object, const sc_string& name, int nbuf = 1)
{
  char strbuf[100];
  for (int i = 0; i < object.bufsize(); i++) {
    if (i >= nbuf) break;
    sprintf(strbuf, "_%d", i);
    sc_trace(tf, (T&) *(object.get_data_ptr(i)), name + strbuf);
  }
}

// For the same nasty reason as for signals, we need the following for channels too.
template< class T >
inline
void sc_trace(sc_trace_file *tf, const sc_channel<T>& object, const char* name, int nbuf = 1)
{
  char strbuf[100];
  sc_string namestr = name;
  for (int i = 0; i < object.bufsize(); i++) {
    if (i >= nbuf) break;
    sprintf(strbuf, "_%d", i);
    sc_trace(tf, (T&) *(object.get_data_ptr(i)), namestr + strbuf);
  }
}


// Function for tracing channel arrays, which makes use of the previous function
template< class A >
inline
void sc_trace(sc_trace_file *tf, const sc_channel_array<A>& object, const sc_string& name, int nbuf = 1)
{
  char strbuf[100];
  for (int i = 0; i < object.length(); i++) {
    sprintf(strbuf, "_%d", i);
    sc_trace( tf, *(object.get_channel(i)), name + strbuf, nbuf );
  }
}

// For the same nasty reason as for signals, we need the following for channel arrayss too.
template< class A >
inline
void sc_trace(sc_trace_file *tf, const sc_channel_array<A>& object, const char* name, int nbuf = 1)
{
  char strbuf[100];
  sc_string namestr = name;
  for (int i = 0; i < object.length(); i++) {
    sprintf(strbuf, "_%d", i);
    sc_trace( tf, *(object.get_channel(i)), namestr + strbuf, nbuf );
  }
}

#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif

// Dummy function for arbitrary types of value, does nothing
extern void sc_trace(sc_trace_file* tf, const void* object, const sc_string& name);

// Turn on/off delta cycle tracing on trace file `tf'.
// Default is to turn on delta cycle tracing.
inline void sc_trace_delta_cycles(sc_trace_file *tf, bool on = true)
{
    if (tf) tf->delta_cycles(on);
}

// Output a comment to the trace file
inline void sc_write_comment(sc_trace_file *tf, const sc_string& comment)
{
    if (tf) tf->write_comment(comment);
}

// Equivalent of fprintf for trace files!
#ifdef __GNUC__
void tprintf(sc_trace_file *tf,  const char* format, ...)
    __attribute__ ((format (printf,2,3)));
#else
void tprintf(sc_trace_file* tf,  const char* format, ...);
#endif    

// Convert double time to 64-bit integer
extern void double_to_special_int64(double in, unsigned* high, unsigned* low);

#endif
