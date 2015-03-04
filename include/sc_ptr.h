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
  sc_ptr.cpp -- smart pointer class

  Original Author: Gene Bushuyev. Synopsys, Inc. (gbush@synopsys.com)

******************************************************************************/



/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:


******************************************************************************/

#ifndef sc_ptrH
#define sc_ptrH

#if !defined(_MSC_VER) && !defined(__SUNPRO_CC)

template<class X> class sc_ptr
{
  X* object;
  public:
  explicit sc_ptr(X* p=0)throw():object(p){}
  sc_ptr(sc_ptr& p) throw():object(p.release()){}
  template<class Y> sc_ptr(sc_ptr<Y>& p) throw()
  {
     object=static_cast<X*>(p.get());
     p.release();
  }
  sc_ptr& operator= (sc_ptr& p) throw()
  {
     delete object;
     object = p.get();
     p.release();
  }
  template<class Y> sc_ptr& operator= (sc_ptr<Y>& p) throw()
  {
    reset(p.release());
    return *this;
  }
  ~sc_ptr() throw()
  {
      delete object;
  }

  // members
  X& operator* () const throw() { return *object;}
  X* operator-> () const throw() { return object; }
  X* get() const throw() { return object;}
  X* release() throw()
  {
    X* tmp = object;
    object =0;
    return tmp;
  }
  void reset(X* p=0) throw()
  {
    if (object!=p)
    {
      delete object;
      object = p;
    }
  }
  // conversions
  template<class Y> operator sc_ptr<Y>() throw(){return sc_ptr<Y>(*this);};
};

//---------------------------------------------------------------------------

#endif
#endif

