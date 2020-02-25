
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

    /* BrieferBwl only: end of saved data at 08+00bit, rest is padding for compat */

    /* 08 */ u32 pad[2];
};

extern struct NewBwlData gBWLDataStorage[];
