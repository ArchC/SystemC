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

    sc_cmnhdr.h - Common header file containing handy pragmas, macros and
                  definitions common to all SystemC source files.

    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_CMNHDR_H
#define SC_CMNHDR_H

/* Disable VC++ warnings 4355 and 4291 that are harmless */

/* this : used in base member initializer list */
#pragma warning(disable: 4355)

/* new and delete warning when exception handling is turned on */
#pragma warning(disable: 4291)

/* in many places implicit conversion to bool from other integral types is performed */
#pragma warning(disable: 4800)
/* unary minus operator applied to unsigned */
#pragma warning(disable: 4146)

/*  Windows Version Build Option */

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0400
#endif

#endif

