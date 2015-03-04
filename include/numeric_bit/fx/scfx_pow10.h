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

    scfx_pow10.h - 

    Original Author: Robert Graulich. Synopsys, Inc. (graulich@synopsys.com)
                     Martin Janssen.  Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SCFX_POW10_H
#define SCFX_POW10_H


#include "scfx_rep.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  CLASS : scfx_pow10
//
//  Class to compute (and cache) powers of 10 in arbitrary precision.
// ----------------------------------------------------------------------------

const int SCFX_POW10_TABLE_SIZE = 32;


class scfx_pow10
{

public:

    scfx_pow10();
    ~scfx_pow10();

    const scfx_rep operator() ( int );

private:

    scfx_rep* pos( int );
    scfx_rep* neg( int );

    scfx_rep _pos[SCFX_POW10_TABLE_SIZE];
    scfx_rep _neg[SCFX_POW10_TABLE_SIZE];

};

#endif

// Taf!
