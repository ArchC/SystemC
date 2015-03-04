/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_report.h - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_REPORT_H
#define SC_REPORT_H


// classes defined in this module
class sc_report;
class sc_report_handler_base;


// ----------------------------------------------------------------------------
//  ENUM : sc_severity
//
//  Enumeration of report severities.
// ----------------------------------------------------------------------------

enum sc_severity
{
    SC_INFO = 0,
    SC_WARNING,
    SC_ERROR,
    SC_FATAL
};


// ----------------------------------------------------------------------------
//  CLASS : sc_report
//
//  Report class.
// ----------------------------------------------------------------------------

class sc_report
{
public:

    static void register_id( int id, const char* msg );
    static const char* get_message( int id );
    static bool is_suppressed( int id );

    static void report( sc_severity severity,
			int         id,
			const char* add_msg,
			const char* file,
			int         line );

    static void suppress_id( int id, bool ); // only for info or warning
    static void suppress_infos( bool );
    static void suppress_warnings( bool );
    static void make_warnings_errors( bool );

private:

    sc_report() {}
};


// ----------------------------------------------------------------------------
//  CLASS : sc_report_handler_base
//
//  Report handler pure virtual base class.
// ----------------------------------------------------------------------------

class sc_report_handler_base
{
    friend class sc_report;

protected:

    sc_report_handler_base();
    virtual ~sc_report_handler_base();

    virtual void report( sc_severity severity,
			 int         id,
			 const char* add_msg,
			 const char* file,
			 int         line ) = 0;

    void suppress_infos( bool );
    void suppress_warnings( bool );
    void make_warnings_errors( bool );

    void install_();
    void deinstall_();

protected:

    bool m_suppress_infos;
    bool m_suppress_warnings;
    bool m_make_warnings_errors;

private:

    sc_report_handler_base* m_old_handler;
};


// ----------------------------------------------------------------------------
//  Report macros.
//
//  Use these macros to report an info, warning, error, or fatal.
// ----------------------------------------------------------------------------

#define SC_REPORT_INFO(id,msg)                                                \
    sc_report::report( SC_INFO, id, msg, __FILE__, __LINE__ )

#define SC_REPORT_WARNING(id,msg)                                             \
    sc_report::report( SC_WARNING, id, msg, __FILE__, __LINE__ )

#define SC_REPORT_ERROR(id,msg)                                               \
    sc_report::report( SC_ERROR, id, msg, __FILE__, __LINE__ )

#define SC_REPORT_FATAL(id,msg)                                               \
    sc_report::report( SC_FATAL, id, msg, __FILE__, __LINE__ )


// ----------------------------------------------------------------------------
//  Allocation of report ids:
//
//    0-99     general
//    100-199  communication
//    200-299  datatypes/bit
//    300-399  datatypes/fx
//    400-499  datatypes/int
//    500-599  kernel
//    600-699  qt
//    700-799  tracing
//    800-899  utils
//    900-999  reserved
//    1000-    libraries, user code, etc.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  Report ids (general).
//
//  Report ids in the range of 0-99.
// ----------------------------------------------------------------------------

extern const int SC_ID_UNKNOWN_ERROR_;
extern const int SC_ID_WITHOUT_MESSAGE_;
extern const int SC_ID_NOT_IMPLEMENTED_;
extern const int SC_ID_INTERNAL_ERROR_;
extern const int SC_ID_ASSERTION_FAILED_;
extern const int SC_ID_OUT_OF_BOUNDS_;


// ----------------------------------------------------------------------------
//  MACRO : sc_assert(expr)
//
//  Like assert(), but additionally prints the current process name
//  and simulation time, if the simulation is running.
// ----------------------------------------------------------------------------

#ifdef NDEBUG

#define sc_assert(expr)                                                       \
 ((void) 0)

#else

#define sc_assert(expr)                                                       \
 ((void) ((expr) ? 0 : (SC_REPORT_FATAL( SC_ID_ASSERTION_FAILED_ , #expr ), 0)))

#endif


#endif

// Taf!
