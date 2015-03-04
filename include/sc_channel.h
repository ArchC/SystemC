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

    sc_channel.h - Channels of any scalar or structs 

    Original Author: Abhijit Ghosh. Synopsys, Inc. (ghosh@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

// Based on sc_signal.h

#ifndef SC_CHANNEL_H
#define SC_CHANNEL_H

#include "sc_logic.h"
#include "sc_object.h"
#include "sc_signal.h"
#include "sc_simcontext.h"
#include "sc_trace.h"

class sc_clock;
class sc_sync;
class sc_async;
class sc_aproc;
class sc_module;
class sc_trace_file;
class sc_string;

/*---------------------------------------------------------------------------*/

//
// <summary> Channel of a scalar type </summary>
//
// sc_channel&lt;T&gt; is used to create a channel-type of underlying
// type T.  Here, T must be a scalar.  If a channel array is desired,
// use <b>sc_channel_array</b> with an <b>sc_array</b> as argument.
//
// We have to use sc_signal_base because simcontext requires a pointer
// to sc_signal_base to store this channel in update queue. Note that
// sc_signal_base has more features than we need.


template<class T> 
class sc_channel_friend;

template< class T > class sc_channel : public sc_signal_base
{
  friend class sc_clock;
  friend class sc_clock_edge;
  friend class sc_channel_friend<T>;

public:
  // Constructors
  sc_channel( const int bsz = 0 );
  sc_channel( const char *nm, const int bsz = 0 );
  sc_channel( sc_object* created_by, const int bsz = 0 );
  sc_channel( sc_object* created_by, const char *nm, const int bsz = 0);

public:
  // Destructor
  virtual ~sc_channel();

  const T& read();
  operator T();
  T* get_data_ptr(int);
  const T* get_data_ptr(int) const;
  T* get_cur_val_ptr();
  const T* get_cur_val_ptr() const;

  sc_channel<T>& write( const T& d );       // Blocking write
  sc_channel<T>& operator=( const T& d );   // Blocking write

  sc_channel<T>& nb_write( const T& d);     // Non-blocking write

  const int bufsize() const { return buf_size; }
  int bufsize() { return buf_size; }

protected: 
  // For sc_channel_array to write into individual channels
  void backdoor_write(const T& d);

private:
  // Need to define this
  sc_channel<T>& operator=(sc_channel<T>&) { return *this; }

public:

#ifdef DEBUG_SYSTEMC
  // These routines should not be used unless debugging because they 
  // require users to define the << operator for user-defined structs.
  void print(ostream& os) const;
  void dump(ostream& os) const;
#endif

  void trace( sc_trace_file* tf ) const;

protected:
  // Stores the value in the channel buffers
  static void update_prop_none(sc_signal_base*);
  static void update_prop_async(sc_signal_base*);
  static void update_prop_aproc(sc_signal_base*);
  static void update_prop_both(sc_signal_base*);

  virtual sc_signal_base::UPDATE_FUNC* update_funcs_array() const;
  virtual void update();
  void update_nonv();
  virtual void update_old();

private:
  // Got to have this function here so that we assert
  // if a channel is used with an sc_async
  void decl_sensitive_async( sc_async_process_handle async ) const;
  // don't need decl_sensitiv_aproc because we can leverage
  // the one in sc_signal_base

  inline void sc_channel_init(const int size);

public:
  // Function returns true if the channel can be written, else false
  bool space_available() const
  { return (buf_size? (buf_size > (num_readable + num_written)) : (num_readable == 0)); }

  bool data_available() const
  { return (num_readable > num_read); }

  // The functions below are anachronisms that should go away. Use the 
  // functions above instead.

  // is_empty() is going to be called by user to determine if there is
  // anything in the channel to read from
  bool is_empty() const { return (num_readable == 0); }

  // is_full() is going to be called by the user to determine if there
  // is space to write into. Note that is_empty() and is_full() can be
  // true simultaneously, i.e. you have nothing to read from the
  // channel.  but there are enough values stuffed in the channel
  // (that will appear to be readable in the next cycle) so that any
  // more writes to the channel will overflow it.
  bool is_full() const { return ((num_readable + num_written) == buf_size); }
  
private:
  // A channel has buf_size number of buffers (for a channel of size
  // 0, it still has 1 buffer). The actual buffer is a circular array
  // (buf) with insertion and extraction pointers(indices), ip and ep
  // respectively.  The number of readable samples in the buffers is
  // num_readable. As one reads from a channel, this does not
  // change. It only changes at the end of the delta cycle (during
  // update). The number of samples read during the delta cycle is
  // stored in num_read. The number of samples written during the
  // delta cycle is stored in num_written. The current value read from
  // the channel is stored in the cur_value variable. This is required
  // for channel arrays (they use the the cur_value variable to create
  // an sc_array that is returned as a result of a read operation).
  // Note that by using num_readable, num_read and num_written, we are
  // guaranteeing order independence (i.e. simulation results will not
  // depend on the order in which processes are getting executed). To
  // see how this happens, note that at every update (end of delta
  // cycle), num_readable += (num_written - num_read), and num_read
  // and num_written are set to 0. Also, num_readable does not change
  // during a delta cycle.  During delta cycle evals, everytime
  // something is written into the channel, num_written is
  // incremented; and everytime something is read from the channel,
  // num_read is incremented. Values can only be read if num_readable
  // > num_read.  A channel is full for writing when (num_readable +
  // num_written) = buf_size; A channel is empty for reading when
  // (num_readable) = 0

  int buf_size;
  mutable T *buf;
  mutable int ip, ep;
  mutable int num_readable;
  mutable int num_read;
  mutable int num_written;
  mutable T cur_value;
};


template<class T>
class sc_channel_friend
{
public:
  static bool space_available(sc_channel<T> *chn) { return chn->space_available(); }
  static void backdoor_write(sc_channel<T>* chn, const T& nv) { chn->backdoor_write(nv); }
  static int bufsize(sc_channel<T>* chn) { return chn->bufsize();}
  static int data_available(sc_channel<T>* chn) { return chn->data_available(); }
};

/*---------------------------------------------------------------------------*/

#include "sc_channel_defs.h_"

/*---------------------------------------------------------------------------*/

#endif
