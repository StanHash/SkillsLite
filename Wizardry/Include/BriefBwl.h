
#pragma once

#include "gbafe.h"

enum
{
    MAX_BWL_CHAR = 0x40
};

struct NewBwlData
{
    /* 00+00bit */ unsigned losses     : 8;
    /* 00+08bit */ unsigned wins       : 10;
    /* 00+18bit */ unsigned battles    : 12;
    /* 00+30bit */ unsigned deathSkirm : 1;
    /* 00+31bit */ unsigned : 0; /* needs to stay 0 for logic to work */

    /* 04+00bit */ unsigned deathLoc   : 6;

    /* USER DATA STARTS HERE at 04+06bit*/

    /* 04+06bit */ unsigned : 0; // pad to +08

    /* BrieferBwl only: end of saved data at 08+00bit, rest is padding for compat */

    /* 08 */ u8 supports[8];
};

extern struct NewBwlData gBWLDataStorage[];

/**
 * Gets Bwl entry for given character
 * 
 * @param charId character id
 * @return address of Bwl entry
 */
struct NewBwlData* BBwl_GetBwlData(unsigned charId);

/**
 * Returns non-zero if given character has a Bwl entry
 * 
 * @param charId character id
 * @return non-zero if character has Bwl entry
 */
int BBwl_HasBwl(unsigned charId);
