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

  sc_fifo_ifs.h -- The sc_fifo<T> interface classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_FIFO_IFS_H
#define SC_FIFO_IFS_H


#include "systemc/communication/sc_interface.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fifo_in_if<T>
//
//  The sc_fifo<T> input interface class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo_in_if
: virtual public sc_interface
{
public:

    // blocking read
    virtual void read( T& ) = 0;
    virtual T read() = 0;

    // non-blocking read
    virtual bool nb_read( T& ) = 0;

    // get the number of available samples
    virtual int num_available() const = 0;

    // get the data written event
    virtual const sc_event& data_written_event() const = 0;

protected:

    // constructor

    sc_fifo_in_if()
	{}

private:

    // disabled
    sc_fifo_in_if( const sc_fifo_in_if<T>& );
    sc_fifo_in_if<T>& operator = ( const sc_fifo_in_if<T>& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_fifo_out_if<T>
//
//  The sc_fifo<T> output interface class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo_out_if
: virtual public sc_interface
{
public:

    // blocking write
    virtual void write( const T& ) = 0;

    // non-blocking write
    virtual bool nb_write( const T& ) = 0;

    // get the number of free spaces
    virtual int num_free() const = 0;

    // get the data read event
    virtual const sc_event& data_read_event() const = 0;

protected:

    // constructor

    sc_fifo_out_if()
	{}

private:

    // disabled
    sc_fifo_out_if( const sc_fifo_out_if<T>& );
    sc_fifo_out_if<T>& operator = ( const sc_fifo_out_if<T>& );
};


#endif

// Taf!
