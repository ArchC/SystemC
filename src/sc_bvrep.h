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
#ifndef SC_BVREP_H
#define SC_BVREP_H

#if defined(__BCPLUSPLUS__) || defined(__SUNPRO_CC)
#include <cstddef>
using std::size_t;
#endif
#if defined(__HP_aCC)
#include <stddef.h>
#endif

#define WORD_POS(index) (index / (sizeof(unsigned)*8))
#define  BIT_POS(index) (index % (sizeof(unsigned)*8))

typedef unsigned sc_bvrep;

inline void
sc_bvrep_set(sc_bvrep* bv, unsigned index)
{
    bv[WORD_POS(index)] |= (1 << BIT_POS(index));
}

inline void
sc_bvrep_set_wb(sc_bvrep* bv, unsigned wpos, unsigned bpos)
{
    bv[wpos] |= (1 << bpos);
}

inline void
sc_bvrep_set_wb(sc_bvrep* bv, unsigned wpos, unsigned bpos, bool val)
{
    bv[wpos] &= ~(1 << bpos);
    bv[wpos] |= ((unsigned)val) << bpos;
}

inline void
sc_bvrep_set(sc_bvrep* bv, unsigned index, bool val)
{
    unsigned wpos = WORD_POS(index);
    unsigned bpos = BIT_POS(index);
    unsigned w = bv[wpos] & ~(1 << bpos);
    bv[wpos] = w | (((unsigned)val) << bpos);
}

inline void
sc_bvrep_clear(sc_bvrep* bv, unsigned index)
{
    bv[WORD_POS(index)] &= ~(1 << BIT_POS(index));
}

inline void
sc_bvrep_clear_wb(sc_bvrep* bv, unsigned wpos, unsigned bpos)
{
    bv[wpos] &= ~(1 << bpos);
}

inline bool
sc_bvrep_test(const sc_bvrep* bv, unsigned index)
{
    return (0 != (bv[WORD_POS(index)] & (1 << BIT_POS(index))));
}

inline bool
sc_bvrep_test_wb(const sc_bvrep* bv, unsigned wpos, unsigned bpos)
{
    return (0 != (bv[wpos] & (1 << bpos)));
}

inline void
sc_bvrep_bitand(sc_bvrep* bv1, const sc_bvrep* bv2, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv1[i] &= bv2[i];
    }
}

inline void
sc_bvrep_bitand(sc_bvrep* bv0, const sc_bvrep* bv1, const sc_bvrep* bv2, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv0[i] = bv1[i] & bv2[i];
    }
}

inline void
sc_bvrep_bitor(sc_bvrep* bv1, const sc_bvrep* bv2, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv1[i] |= bv2[i];
    }
}

inline void
sc_bvrep_bitor(sc_bvrep* bv0, const sc_bvrep* bv1, const sc_bvrep* bv2, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv0[i] = bv1[i] | bv2[i];
    }
}

inline void
sc_bvrep_bitxor(sc_bvrep* bv1, const sc_bvrep* bv2, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv1[i] ^= bv2[i];
    }
}

inline void
sc_bvrep_bitxor(sc_bvrep* bv0, const sc_bvrep* bv1, const sc_bvrep* bv2, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv0[i] = bv1[i] ^ bv2[i];
    }
}

inline void
sc_bvrep_bitnot(sc_bvrep* bv0, sc_bvrep* bv1, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        bv0[i] = ~ bv1[i];
    }
}

inline bool
sc_bvrep_equal(const sc_bvrep* bv0, const sc_bvrep* bv1, size_t nwords)
{
    for (int i = nwords - 1; i >= 0; --i) {
        if (bv0[i] != bv1[i])
            return false;
    }
    return true;
}


#endif
