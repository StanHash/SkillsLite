
#include "BriefBwl.h"

struct NewBwlData* BBwl_GetBwlData(unsigned charId)
{
    charId = charId - 1;
    return gBWLDataStorage + charId;
}

int BBwl_HasBwl(unsigned charId)
{
    return (charId < MAX_BWL_CHAR) && GetCharacterData(charId)->affinity;
}

void BBwl_NuSetDeathStats(int charId, int killerId, int cause)
{
    if (BBwl_HasBwl(charId))
    {
        struct NewBwlData* bwl = BBwl_GetBwlData(charId);

        if (GetBattleMapType() == 2) // skirmish
        {
            bwl->deathSkirm = TRUE;
            bwl->deathLoc = gGMData.units[0].location;
        }
        else
        {
            bwl->deathSkirm = FALSE;
            bwl->deathLoc = gChapterData.chapterIndex;
        }
    }
}

int BBwl_NuggetTotalBattles(void)
{
    int result = 0;

    for (unsigned i = 1; i < MAX_BWL_CHAR; ++i)
        result += BBwl_GetBwlData(i)->battles;

    return result;
}

int BBwl_NuggetTotalWins(void)
{
    int result = 0;

    for (unsigned i = 1; i < MAX_BWL_CHAR; ++i)
        result += BBwl_GetBwlData(i)->wins;

    return result;
}

int BBwl_NuggetTotalLosses(void)
{
    int result = 0;

    for (unsigned i = 1; i < MAX_BWL_CHAR; ++i)
        result += BBwl_GetBwlData(i)->losses;

    return result;
}

int BBwl_NuggetFavor(int charId)
{
    if (BBwl_HasBwl(charId))
    {
        struct NewBwlData* bwl = BBwl_GetBwlData(charId);
        int result = bwl->battles * 4 + bwl->wins * 0x10 - bwl->losses * 0x80;

        if (result > 4000)
            return 4000;

        if (result < 0)
            return 0;

        return result;
    }

    return 0;
}
