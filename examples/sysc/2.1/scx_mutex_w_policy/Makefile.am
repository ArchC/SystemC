## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2003 by all Contributors.
##  All Rights reserved.
##
##  The contents of this file are subject to the restrictions and limitations
##  set forth in the SystemC Open Source License Version 2.4 (the "License");
##  You may not use this file except in compliance with such restrictions and
##  limitations. You may obtain instructions on how to receive a copy of the
##  License at http://www.systemc.org/. Software distributed by Contributors
##  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
##  ANY KIND, either express or implied. See the License for the specific
##  language governing rights and limitations under the License.
##
## ****************************************************************************
##
##  examples/sysc/2.1/scx_mutex_w_policy/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Andy Goodrich, Forte Design Systems, 17 June 2003
##
## ****************************************************************************
##
##  MODIFICATION LOG - modifiers, enter your name, affiliation and
##  changes you are making here:
##
##  Modifier Name & Affiliation:
##  Description of Modification:
##
## ****************************************************************************

include $(top_srcdir)/config/Make-rules.examples

CXX_FILES = \
	scx_mutex_w_policy.cpp 

H_FILES = 

check_PROGRAMS = scx_mutex_w_policy
TESTS = $(check_PROGRAMS) 

scx_mutex_w_policy_SOURCES = $(CXX_FILES) $(H_FILES)

scx_mutex_w_policy_LDADD = -L@libdir@ -lsystemc -lm

filesdir = $(datadir)/systemc/examples/sysc/2.1/scx_mutex_w_policy
files_DATA = $(scx_mutex_w_policy_SOURCES)

## Taf!
