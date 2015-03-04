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

  sc_iostream.h - Portable iostream header file wrapper.

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_IOSTREAM_H
#define SC_IOSTREAM_H


#if !defined( _MSC_VER ) && !defined( __HP_aCC ) && !defined( __BORLANDC__ )

#include <iostream>
#include <strstream>
using std::ios;
using std::streambuf;
using std::streampos;
using std::streamsize;
using std::iostream;
using std::istream;
using std::ostream;
using std::strstream;
using std::strstreambuf;
using std::istrstream;
using std::ostrstream;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::dec;
using std::hex;
using std::oct;

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;

#include <cstddef>
using std::size_t;

#include <cstring>
using std::memchr;
using std::memcmp;
using std::memcpy;
using std::memmove;
using std::memset;
using std::strcat;
using std::strncat;
using std::strchr;
using std::strrchr;
using std::strcmp;
using std::strncmp;
using std::strcpy;
using std::strncpy;
using std::strcspn;
using std::strspn;
using std::strlen;
using std::strpbrk;
using std::strstr;
using std::strtok;

#else

#if defined( _MSC_VER )
// VC++6 with standard library

#include <iostream>
#include <strstream>
using std::ios;
using std::streambuf;
using std::streampos;
using std::streamsize;
using std::iostream;
using std::istream;
using std::ostream;
using std::strstream;
using std::strstreambuf;
using std::istrstream;
using std::ostrstream;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::dec;
using std::hex;
using std::oct;

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;

#include <cstddef>

#else

#if defined( __HP_aCC )
// HP aCC

#include <iostream.h>
#include <strstream.h>
#include <fstream.h>

#endif

#endif
#endif


// shortcuts that save some typing

#ifdef CCAST
#undef CCAST
#endif
#define CCAST       const_cast

#ifdef DCAST
#undef DCAST
#endif
#define DCAST     dynamic_cast

#ifdef RCAST
#undef RCAST
#endif
#define RCAST reinterpret_cast

#ifdef SCAST
#undef SCAST
#endif
#define SCAST      static_cast


#endif
