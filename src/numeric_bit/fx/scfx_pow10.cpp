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

    scfx_pow10.cpp - 

    Original Author: Robert Graulich. Synopsys, Inc. (graulich@synopsys.com)
                     Martin Janssen.  Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "scfx_pow10.h"


// ----------------------------------------------------------------------------
//  CLASS : scfx_pow10
//
//  Class to compute (and cache) powers of 10 in arbitrary precision.
// ----------------------------------------------------------------------------

scfx_pow10::scfx_pow10()
{
    _pos[0] = scfx_rep( 10.0 );
    _neg[0] = scfx_rep( 0.1 );

    for( int i = 1; i < SCFX_POW10_TABLE_SIZE; i ++ )
    {
        _pos[i].set_nan();
	_neg[i].set_nan();
    }
}

scfx_pow10::~scfx_pow10()
{}


const scfx_rep
scfx_pow10::operator() ( int i )
{
    if( i == 0 ) {
        return scfx_rep( 1.0 );
    }

    if( i > 0 )
    {
        int bit = scfx_find_msb( i );
	scfx_rep result = *pos( bit );
	if( bit )
	{
	    while( -- bit >= 0 )
	    {
	        if( ( 1 << bit ) & i )
		{
		    scfx_rep* tmp = mult__scfx_rep( result, *pos( bit ) );
		    result = *tmp;
		    delete tmp;
		}
	    }
	}
	return result;
    }
    else
    {
        i = -i;
	int bit = scfx_find_msb( i );
	scfx_rep result = *neg( bit );
	if( bit )
	{
	    while( -- bit >= 0 )
	    {
	        if( ( 1 << bit ) & i )
		{
		    scfx_rep* tmp = mult__scfx_rep( result, *neg( bit ) );
		    result = *tmp;
		    delete tmp;
		}
	    }
	}
	return result;
    }
}


scfx_rep*
scfx_pow10::pos( int i )
{
    if( ! _pos[i].is_normal() )
    {
        multiply( _pos[i], *pos( i - 1 ), *pos( i - 1 ) );
    }
    return &_pos[i];
}

scfx_rep*
scfx_pow10::neg( int i )
{
    if( ! _neg[i].is_normal() )
    {
	multiply( _neg[i], *neg( i - 1 ), *neg( i - 1 ) );
    }
    return &_neg[i];
}


// Taf!
