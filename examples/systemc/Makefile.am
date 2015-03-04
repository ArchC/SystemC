## ****************************************************************************
##
##  The following code is derived, directly or indirectly, from the SystemC
##  source code Copyright (c) 1996-2002 by all Contributors.
##  All Rights reserved.
##
##  The contents of this file are subject to the restrictions and limitations
##  set forth in the SystemC Open Source License Version 2.3 (the "License");
##  You may not use this file except in compliance with such restrictions and
##  limitations. You may obtain instructions on how to receive a copy of the
##  License at http://www.systemc.org/. Software distributed by Contributors
##  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
##  ANY KIND, either express or implied. See the License for the specific
##  language governing rights and limitations under the License.
##
## ****************************************************************************
##
##  examples/systemc/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##
##  Original Author: Martin Janssen, Synopsys, Inc.
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

SUBDIRS =

INST_FILES = \
	Makefile.defs \
	fft \
	fir \
	forkjoin \
	pipe \
	pkt_switch \
	risc_cpu \
	rsa \
	simple_bus \
	simple_fifo \
	simple_perf

EXAMPLES_DIR = $(prefix)/examples/systemc

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(EXAMPLES_DIR)
	for file in $(INST_FILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    test -d $(EXAMPLES_DIR)/$$file \
	    || cp -pr $$d/$$file $(EXAMPLES_DIR) || :; \
	  else \
	    test -f $(EXAMPLES_DIR)/$$file \
	    || cp -p $$d/$$file $(EXAMPLES_DIR)/$$file || :; \
	  fi; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	w=`pwd`; cd $(srcdir); s=`pwd`; cd $$w; \
	if test $(EXAMPLES_DIR) != $$s; then \
	  for file in $(INST_FILES); do \
	    d=$(srcdir); \
	    if test -d $$d/$$file; then \
	      rm -rf $(EXAMPLES_DIR)/$$file; \
	    else \
	      rm -f $(EXAMPLES_DIR)/$$file; \
	    fi; \
	  done; \
	fi
	-rmdir $(EXAMPLES_DIR)

## Taf!
