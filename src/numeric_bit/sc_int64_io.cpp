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

    sc_int64_io.cpp                                                                           
    Original Author: Amit Rao. Synopsys, Inc. (arao@synopsys.com)

******************************************************************************/


/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/

#ifdef __BCPLUSPLUS__
#pragma hdrstop
#endif
#include "../sc_iostream.h"
#include "sc_unsigned.h"
#include "sc_signed.h"
#include "sc_int.h"
#include "sc_uint.h"

#if defined(_MSC_VER) && !defined(SC_VC6_SCL)
static void
write_uint64(ostream& os, uint64 val, int sign)
{
    const int WRITE_BUF_SIZE = 10 + sizeof(uint64)*3;
    char buf[WRITE_BUF_SIZE];
    char* buf_ptr = buf + WRITE_BUF_SIZE;
    const char* show_base = "";
    int show_base_len = 0;
    int show_pos = 0;
    fmtflags flags = os.flags();

    if ((flags & ios::basefield) == ios::oct) {
        do {
            *--buf_ptr = (char)((val & 7) + '0');
            val = val >> 3;
        } while (val != 0);
        if ((flags & ios::showbase) && (*buf_ptr != '0'))
            *--buf_ptr = '0';
    } else if ((flags & ios::basefield) == ios::hex) {
        const char* xdigs = (flags & ios::uppercase) ? 
            "0123456789ABCDEF0X" : 
            "0123456789abcdef0x";
        do {
            *--buf_ptr = xdigs[val & 15];
            val = val >> 4;
        } while (val != 0);
        if ((flags & ios::showbase)) {
            show_base = xdigs + 16;
            show_base_len = 2;
        }
    } else {
        while (val > UINT_MAX) {
            *--buf_ptr = (char)((val % 10) + '0');
            val /= 10;
        }
        unsigned ival = (unsigned) val;
        do {
            *--buf_ptr = (ival % 10) + '0';
            ival /= 10;
        } while (ival != 0);
        if (sign > 0 && (flags & ios::showpos))
            show_pos = 1;
    }

    int buf_len = buf + WRITE_BUF_SIZE - buf_ptr;
    int w = os.width(0);

    int len = buf_len + show_pos;
    if (sign < 0) len++;
    len += show_base_len;

    int padding = len > w ? 0 : w - len;
    fmtflags pad_kind = flags & ios::adjustfield;
    char fill_char = os.fill();

    if (padding > 0 &&
        ios::left != pad_kind &&
        ios::internal != pad_kind) {
        for (int i = padding - 1; i >= 0; --i) {
            if (! os.put(fill_char))
                goto fail;
        }
    }
    if (sign < 0 || show_pos) {
        if (! os.put(sign < 0 ? '-' : '+'))
            goto fail;
    }
    if (show_base_len) {
        if (! os.write(show_base, show_base_len))
            goto fail;
    }
    if ((fmtflags)ios::internal == pad_kind && padding > 0) {
        for (int i = padding - 1; i >= 0; --i) {
            if (! os.put(fill_char))
                goto fail;
        }
    }
    if (! os.write(buf_ptr, buf_len))
        goto fail;
    if ((fmtflags)ios::left == pad_kind && padding > 0) {
        for (int i = padding - 1; i >= 0; --i) {
            if (! os.put(fill_char))
                goto fail;
        }
    }
    os.osfx();
    return;
fail:
    //os.set(ios::badbit);
    os.osfx();
}

ostream&
operator<<(ostream& os, int64 n)
{
    if (os.opfx()) {
        int sign = 1;
        uint64 abs_n = (uint64) n;
        if (n < 0 && (os.flags() & (ios::oct|ios::hex)) == 0) {
            abs_n = -1*((uint64) n);
            sign = -1;
        }
        write_uint64(os, abs_n, sign);
    }
    return os;
}

ostream&
operator<<(ostream& os, uint64 n)
{
    if (os.opfx()) {
        write_uint64(os, n, 0);
    }
    return os;
}


#endif
