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

    scfx_utils.cpp - 

    Original Author: Martin Janssen. Synopsys, Inc. (mjanssen@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include "scfx_utils.h"


void
scfx_tc2csd( scfx_string& s )
{
    _SC_ASSERT( s[0] == '0' && s[1] == 'c' &&
		s[2] == 's' && s[3] == 'd', "invalid prefix" );

    scfx_string csd;

    // copy bits from 's' into 'csd'; skip prefix, point, and exponent
    int i = 0;
    int j = 4;
    while( s[j] )
    {
	if( s[j] == '0' || s[j] == '1' )
	    csd[i ++] = s[j];
	else if( s[j] != '.' )
	    break;
	++ j;
    }
    csd[i] = '\0';

    // convert 'csd' from two's complement to csd
    -- i;
    while( i >= 0 )
    {
	if( csd[i] == '0' )
	    -- i;
	else
	{
	    if( i > 0 && csd[i - 1] == '0' )
		-- i;
	    else if( i == 0 )
		csd[i --] = '-';
	    else
	    {   // i > 0 && csd[i - 1] == '1'
		csd[i --] = '-';
		while( i >= 0 && csd[i] == '1' )
		    csd[i --] = '0';
		if( i > 0 )
		    csd[i] = '1';
		else if( i == 0 )
		    csd[i --] = '1';
	    }
	}
    }

    // copy bits from 'csd' back into 's'
    i = 0;
    j = 4;
    while( csd[i] )
    {
	if( s[j] == '.' )
	    ++ j;
	s[j ++] = csd[i ++];
    }
}


void
scfx_csd2tc( scfx_string& csd )
{
    _SC_ASSERT( csd[0] == '0' && csd[1] == 'c' &&
		csd[2] == 's' && csd[3] == 'd', "invalid prefix" );

    scfx_string s;

    // copy bits from 'csd' into 's'; skip prefix, point, and exponent
    int i = 0;
    s[i ++] = '0';
    int j = 4;
    while( csd[j] )
    {
	if( csd[j] == '-' || csd[j] == '0' || csd[j] == '1' )
	    s[i ++] = csd[j];
	else if( csd[j] != '.' )
	    break;
	++ j;
    }
    s[i] = '\0';

    // convert 's' from csd to two's complement
    int len = i;
    i = 1;
    while( i < len )
    {
        while( i < len && s[i] != '-' )
	    i ++;
	if( i < len )
	{
	    j = i ++;
	    s[j --] = '1';
	    while( j >= 0 && s[j] == '0' )
	        s[j --] = '1';
	    if( j >= 0 )
	        s[j] = '0';
	}
    }

    // copy bits from 's' back into 'csd'
    j = csd.length();
    csd[j + 1] = '\0';
    while( j > 4 )
    {
	csd[j] = csd[j - 1];
	-- j;
    }
        
    i = 0;
    j = 4;
    while( s[i] )
    {
	if( csd[j] == '.' )
	    ++ j;
	csd[j ++] = s[i ++];
    }
}


// Taf!
