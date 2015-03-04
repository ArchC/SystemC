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

  sc_isdb_trace.h - Implementation of ISDB tracing.

  Original Author: Ulli Holtmann, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#ifndef SC_ISDB_TRACE_H
#define SC_ISDB_TRACE_H

#include <stdio.h>
#include "systemc/tracing/sc_trace.h"
#include "systemc/utils/sc_vector.h"

class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;
class sc_bit;
/********************************************************************

  Simplified version of ISDB Definitions

*********************************************************************/

#if defined(hal) && defined(__WORD64)
typedef int32_t ISDB_Int;
typedef uint32_t ISDB_UInt;
#else
# if WORD_BIT > 24 || INT_MAX > 16777215
#  define	ISDB_Int int
# else
#  define	ISDB_Int long
# endif
typedef unsigned ISDB_Int ISDB_UInt;
#endif

#define ISDB_HIGH               1       /* binary high */
#define ISDB_LOW                0       /* binary low */
#define ISDB_Z                  2       /* binary tri-state */
#define ISDB_X                  3       /* binary undetermined */
#define ISDB_IncludeDeltaCycle  2       /* Include delta cycle information */

typedef struct _ISDB_conn    *ISDB_Conn;     /* Database connection */
typedef union  _ISDB_signal  *ISDB_Signal;   /* Signal identifier */
typedef ISDB_UInt             ISDB_ID;       /* Type of an ISDB identifier */
typedef ISDB_ID               ISDB_Type;     /* Signal type */
typedef ISDB_ID               ISDB_Status;   /* Return type for functions */
typedef struct
  {
    ISDB_UInt low;		  	     /* low order 32 bits of time */
    ISDB_UInt high;			     /* high order 32 bits of time */
  }	ISDB_Time;
typedef enum
  {
    ISDB_DONE,			             /* Close ISDB database */
    ISDB_RESTART	 	             /* Keep database open, discard values */
  }	ISDB_EndMode;
typedef enum
  {
    ISDB_NoError,		             /* No error was recorded */
    ISDB_SystemError,		             /* System error */
    ISDB_BadISDBOpError		             /* Illegal operation requested */
  }	ISDB_ErrorType;
typedef void (*ISDB_ErrorHandler) (ISDB_Conn, ISDB_ErrorType, char *);


extern ISDB_Type ISDB_Binary;		     /* Binary signal */
extern ISDB_Type ISDB_Array;                 /* Array (or bus) of binary signals */
extern ISDB_Type ISDB_Integer;		     /* Integer scalar signal */
extern ISDB_Type ISDB_Float;		     /* Floating-point scalar signal */
extern char *ISDB_TimeUnits;                 /* Holds: "TimeUnits"       */
extern char *ISDB_StartTime;                 /* Holds: "StartTime"       */
extern char *ISDB_PathSeparator;             /* Holds: "PathSeparator"    */
extern char *ISDB_StructSeparator;           /* Holds: "StructSeparator"  */

extern "C" ISDB_Conn ISDB_CreateDB ( char *, ISDB_UInt );
extern "C" ISDB_Signal ISDB_DefineSignal ( ISDB_Conn, ISDB_Type, const char *, ... );
extern "C" ISDB_Status ISDB_SetValue ( ISDB_Conn, ISDB_Signal, ... );
extern "C" void ISDB_InstallErrorHandler ( ISDB_ErrorHandler );
extern "C" ISDB_Status ISDB_Configure ( ISDB_Conn, ... /* NULL */ );
extern "C" ISDB_Status ISDB_SetTime ( ISDB_Conn, ISDB_Time );
extern "C" ISDB_Status ISDB_End ( ISDB_Conn, ISDB_EndMode );


/********************************************************************

  Class isdb_trace

*********************************************************************/

class isdb_trace;  // defined in isdb_trace.cc


class isdb_trace_file : public sc_trace_file {
public:
    void sc_set_isdb_time_unit(int exponent10_seconds); // -7 -> 100ns

    // Create a Isdb trace file.
    // `Name' forms the base of the name to which `.isdb' is added.
    isdb_trace_file(const char *name);

    // Flush results and close file.
    ~isdb_trace_file();

protected:
    // These are all virtual functions in sc_trace_file and
    // they need to be defined here.

    // Trace a boolean object (single bit)
     void trace(const bool& object, const sc_string& name);

    // Trace a sc_bit object (single bit)
     void trace(const sc_bit& object, const sc_string& name);

    // Trace a sc_logic object (single bit)
     void trace(const sc_logic& object, const sc_string& name);

    // Trace an unsigned char with the given width
     void trace(const unsigned char& object, const sc_string& name, int width);

    // Trace an unsigned short with the given width
     void trace(const unsigned short& object, const sc_string& name, int width);

    // Trace an unsigned int with the given width
     void trace(const unsigned int& object, const sc_string& name, int width);

    // Trace an unsigned long with the given width
     void trace(const unsigned long& object, const sc_string& name, int width);

    // Trace a signed char with the given width
     void trace(const char& object, const sc_string& name, int width);

    // Trace a signed short with the given width
     void trace(const short& object, const sc_string& name, int width);

    // Trace a signed int with the given width
     void trace(const int& object, const sc_string& name, int width);

    // Trace a signed long with the given width
     void trace(const long& object, const sc_string& name, int width);
    
    // Trace a float
     void trace(const float& object, const sc_string& name);

    // Trace a double
     void trace(const double& object, const sc_string& name);

    // Trace sc_uint_base
     void trace (const sc_uint_base& object, const sc_string& name);

    // Trace sc_int_base
     void trace (const sc_int_base& object, const sc_string& name);

    // Trace sc_unsigned
     void trace (const sc_unsigned& object, const sc_string& name);

    // Trace sc_signed
     void trace (const sc_signed& object, const sc_string& name);

    // Trace sc_fxval
    void trace( const sc_fxval& object, const sc_string& name );

    // Trace sc_fxval_fast
    void trace( const sc_fxval_fast& object, const sc_string& name );

    // Trace sc_fxnum
    void trace( const sc_fxnum& object, const sc_string& name );

    // Trace sc_fxnum_fast
    void trace( const sc_fxnum_fast& object, const sc_string& name );

    // Trace sc_bv_base (sc_bv)
    virtual void trace(const sc_bv_ns::sc_bv_base& object, const sc_string& name);

    // Trace sc_lv_base (sc_lv)
    virtual void trace(const sc_bv_ns::sc_lv_base& object, const sc_string& name);

    // Trace an enumerated object - where possible output the enumeration literals
    // in the trace file. Enum literals is a null terminated array of null
    // terminated char* literal strings.
     void trace(const unsigned& object, const sc_string& name, const char** enum_literals);

    // Output a comment to the trace file
     void write_comment(const sc_string& comment);

    // Also trace transitions between delta cycles if flag is true.
     void delta_cycles(bool flag);

    // Write trace info for cycle.
     void cycle(bool delta_cycle);
    
private:
    // Initialize the tracing
    void initialize();
    // Create ISDB names for each variable
    void create_isdb_name(sc_string* p_destination);
    
    // Array to store the variables traced
    sc_pvector<isdb_trace*> traces;

    // Pointer to the file that needs to be written
    ISDB_Conn database;

    double timescale_unit;      // in seconds
    bool timescale_set_by_user; // = 1 means set by user
    bool trace_delta_cycles;    // = 1 means trace the delta cycles
    bool initialized;           // = 1 means initialized

    unsigned isdb_name_index;    // Number of variables traced

    unsigned previous_time_units_low, previous_time_units_high; // Previous time unit as 64-bit integer
};

// Create ISDB file
extern sc_trace_file *sc_create_isdb_trace_file(const char* name);
//delete the file object
extern void sc_close_isdb_trace_file( sc_trace_file* );
#endif
