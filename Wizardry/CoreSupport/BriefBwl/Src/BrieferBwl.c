
#include "BriefBwl.h"

void BBwl_SaveBwl(void* target, unsigned size)
{
    const unsigned amt = size/8 > MAX_BWL_CHAR ? MAX_BWL_CHAR : size/8;

    for (unsigned i = 0; i < amt; ++i)
        WriteSramFast(&gBWLDataStorage[i], target + i*8, 8);
}

void BBwl_LoadBwl(void* source, unsigned size)
{
    const unsigned amt = size/8 > MAX_BWL_CHAR ? MAX_BWL_CHAR : size/8;

    for (unsigned i = 0; i < amt; ++i)
        ReadSramFast(source + i*8, &gBWLDataStorage[i], 8);
}
