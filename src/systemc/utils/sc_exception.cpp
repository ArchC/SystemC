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

  sc_exception.cpp -- Implementation of the exception classes in SystemC.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
//---------------------------------------------------------------------------
#include "systemc/utils/sc_exception.h"


// ----------------------------------------------------------------------------
//  FUNCTION : sc_stop_here
//
//  Debugging aid for warning, error, and fatal reports.
//  This function *cannot* be inlined.
// ----------------------------------------------------------------------------

void
sc_stop_here()
{}


// ----------------------------------------------------------------------------
// don't use raw data from the ErrRec, it may change in the future
// use GetErrorMsg function instead
struct ErrRecType
{
  int ErrNo;
  const char* ErrMsg; // array of string literals
};
ErrRecType ErrRec[]=
{
  // general
  { 0, "unknown error"},
  { 1, ""},
  { 2, "not implemented"},
  { 1000, "out of bounds"},
  { 1001, "length mismatch in bit/logic vector assignment" },
  { 1002, "incompatible types"},
  { 1003, "cannot perform conversion"},
  { 1004, "fiber doesn''t exist"},
  { 1005, "maximum number of processes per module exceeded"},
  { 1006, "value is not valid"},
  { 1007, "cannot perform requested operation"},
  { 1008, "sc_string doesn''t contain an integer"},
  { 1009, "string is too long"},
  { 1010, "Clock signal has multiple transitions within a delta cycle"},
  { 1011, "dumpfile has already been specified"},
  { 1012, "dumpfile name not specified"},
  { 1013, "object not found"},
  { 1014, "operator does not return boolean"},
  { 1015, "operator does not return int"},
  { 1016, "operator does not return sc_logic"},
  { 1017, "operand is not sc_logic"},
  { 1018, "operand is not bool"},
  { 1019, "Attempt to take front() on an empty list"},
  { 1020, "Attempt to take back() on an empty list"},
  { 1021, "Object already exists"},
  { 1022, "Illegal characters"},
  // datatypes
  { 2000, "incompatible vectors"},
  { 2001, "zero length"},
  { 2002, "vector is too long"},
  { 2003, "vector contains 4-value logic"},
  { 2004, "sc_bv cannot contain values X and Z"},
  { 2005, "wrong base type in sc_string"},
  { 2006, "vector is not resizeable"},
  { 2007, "bool operation with non-bool value"},
  { 2008, "vector is too long: truncated"},
  { 2009, "vector is too short: 0-padded"},
  { 2010, "wrong value"},
  { 2020, "fixed-point value argument is invalid - NaN or Inf" },
  // processes
  { 3001, "internal error: sc_vc6_process_helper"},
  { 3002, "maximum number of processes per module exceeded (VC6)"},
  { 3004, "did you forget to call end_module()"},
  { 3005, "Hierarchical name as shown may be incorrect due to previous errors"},
  { 3006, "You can set the stack size for only synchronous and asynchronous"
          " thread processes"},
  // modules
  { 4001, "incorrect use of sc_module_name"},
  { 4002, "port specified outside of module"},
  { 4003, "An sc_module_name parameter for your constructor is required"},
  { 4004, "Cannot bind an extra signal"},
  { 4005, "Cannot bind an extra port"},
  // signals
  { 5001, "event method unavailable"},
  // other kernel
  { 6001, "set time resolution failed" },
  { 6002, "set default time unit failed" },
  { 6003, "default time unit changed to time resolution" },
  { 6011, "wait(n) is only allowed in SC_CTHREADs" },
  { 6012, "wait_until() is only allowed in SC_CTHREADs" },
  { 6013, "wait() is only allowed in SC_THREADs and SC_CTHREADs" },
  { 6014, "next_trigger() is only allowed in SC_METHODs" },
  { 6015, "immediate notification is not allowed during the update phase" },
  { 6016, "halt() is only allowed in SC_CTHREADs" },
  { 6017, "watching() is only allowed in SC_CTHREADs" },
  { 6018, "dont_initialize() has no effect for SC_CTHREADs" },
  { 6019, "wait(n) is only valid for n > 0" },
  { 6021, "make sensitive failed" },
  { 6022, "make sensitive pos failed" },
  { 6023, "make sensitive neg failed" },
  // other communication
  { 7001, "sc_clock requires a period >= 0" },
  { 7002, "sc_clock requires a start time >= 0" },
  { 7011, "sc_fifo<T> cannot have more than one reader" },
  { 7012, "sc_fifo<T> cannot have more than one writer" },
  { 7013, "sc_fifo<T> must have a size of at least 1" },
  { 7021, "bind interface to port failed" },
  { 7022, "bind parent port to port failed" },
  { 7023, "complete binding failed" },
  { 7024, "insert port failed" },
  { 7025, "remove port failed" },
  { 7026, "get interface failed" },
  { 7031, "insert primitive channel failed" },
  { 7032, "remove primitive channel failed" },
  { 7041, "sc_signal<T> cannot have more than one driver" },
  { 7051, "channel doesn't have a default event" },
  { 7061, "insert module failed" },
  { 7062, "remove module failed" },
  { 7071, "resolved port not bound to resolved signal" },
  // legacy
  { 9001, "SC_ERR_ARRAY_BOUND"},
  { 9002, "SC_ERR_ARRAY_LENGTH"},
  { 9003,  "SC_ERR_PARAMETER"},
  { 9004,  "SC_ERR_LAMBDA"},
  { 9005,  "SC_ERR_SC_LOGIC"},
  { 9006,  "SC_ERR_WAIT"},
  { 9007,  "SC_ERR_WATCHING"}
};
// -------------------------------
static sc_string GetErrorMsg(int N)
// -------------------------------
{
  bool found = false;
  unsigned i=0;
  while(!found && i<sizeof(ErrRec))
  {
    found = ErrRec[i].ErrNo == N;
    i++;
  }
  if(found)
    return sc_string("(E")+sc_string::make_str(N)+") "+ErrRec[i-1].ErrMsg;
  else
    return GetErrorMsg(0);
}
//---------------------------------------------------------------------------
// base exception class
sc_exception::sc_exception () throw()
{
}
sc_exception::sc_exception(const sc_string& What) throw():What(What)
{
}
sc_exception::sc_exception(int Error)throw() :What(GetErrorMsg(Error))
{
}
sc_exception::sc_exception(int Error, const sc_string& Info) throw()
  :What(GetErrorMsg(Error)+" : "+Info)
{
}
sc_exception::sc_exception (const sc_exception& e) throw()
  :What(e.What)
{
}
sc_exception& sc_exception::operator= (const sc_exception& e) throw()
{
  What = e.What;
  return *this;
}
sc_exception::~sc_exception () throw()
{
}
const char* sc_exception::what() const throw()
{
  return str().c_str();
}
const sc_string sc_exception::str() const throw()
{
  return What;
}
// exception class for data types
sc_edata::sc_edata () throw()
{
}
sc_edata::sc_edata(const sc_string& What) throw():sc_exception(What)
{
}
sc_edata::sc_edata(int Error) throw() :sc_exception(Error)
{
}
sc_edata::sc_edata(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_edata::sc_edata (const sc_edata& e) throw()
  :sc_exception(e)
{
}
sc_edata& sc_edata::operator= (const sc_edata& e) throw()
{
  What = e.What;
  return *this;
}
sc_edata::~sc_edata () throw()
{
}
const sc_string sc_edata::str() const throw()
{
  return "ERROR: "+What;
}
// exception class for signals
sc_esignal::sc_esignal () throw()
{
}
sc_esignal::sc_esignal(const sc_string& What) throw():sc_exception(What)
{
}
sc_esignal::sc_esignal(int Error) throw():sc_exception(Error)
{
}
sc_esignal::sc_esignal(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_esignal::sc_esignal (const sc_esignal& e) throw()
  :sc_exception(e)
{
}
sc_esignal& sc_esignal::operator= (const sc_esignal& e) throw()
{
  What = e.What;
  return *this;
}
sc_esignal::~sc_esignal () throw()
{
}
const sc_string sc_esignal::str() const throw()
{
  return "ERROR:"+What;
}
// exception class for processes
sc_eprocess::sc_eprocess () throw()
{
}
sc_eprocess::sc_eprocess(const sc_string& What) throw():sc_exception(What)
{
}
sc_eprocess::sc_eprocess(int Error) throw():sc_exception(Error)
{
}
sc_eprocess::sc_eprocess(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_eprocess::sc_eprocess (const sc_eprocess& e) throw():sc_exception(e)
{
}
sc_eprocess& sc_eprocess::operator= (const sc_eprocess& e) throw()
{
  What = e.What;
  return *this;
}
sc_eprocess::~sc_eprocess () throw()
{
}
const sc_string sc_eprocess::str() const throw()
{
  return "ERROR: "+What;
}
// exception class for modules
sc_emodule::sc_emodule () throw()
{
}
sc_emodule::sc_emodule(const sc_string& What) throw():sc_exception(What)
{
}
sc_emodule::sc_emodule(int Error) throw():sc_exception(Error)
{
}
sc_emodule::sc_emodule(int Error, const sc_string& Info) throw()
  :sc_exception(Error,Info)
{
}
sc_emodule::sc_emodule (const sc_emodule& e) throw()
  :sc_exception(e)
{
}
sc_emodule& sc_emodule::operator= (const sc_emodule& e) throw()
{
  What = e.What;
  return *this;
}
sc_emodule::~sc_emodule () throw()
{
}
const sc_string sc_emodule::str() const throw()
{
  return "ERROR: "+What;
}
