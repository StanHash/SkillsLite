
#include "BriefBwl.h"

enum { MAX_SUPPORT_COUNT = 8 };

static const
int sSupportMaxExpLut[] =
{
    [SUPPORT_LEVEL_NONE] = SUPPORT_EXP_C-1,
    [SUPPORT_LEVEL_C]    = SUPPORT_EXP_B-1,
    [SUPPORT_LEVEL_B]    = SUPPORT_EXP_A-1,
    [SUPPORT_LEVEL_A]    = SUPPORT_EXP_A
};

static inline
int HasCharacterSupportData(int characterId)
{
    return characterId && characterId < MAX_BWL_CHAR;
}

static inline
u8* GetCharacterSupportData(int characterId)
{
    u8* const base = (u8*) (gBWLDataStorage + characterId - 1);
    return base + sizeof(gBWLDataStorage[0]) - MAX_SUPPORT_COUNT;
}

static inline
int HasUnitSupportData(struct Unit* unit)
{
    return HasCharacterSupportData(unit->pCharacterData->number);
}

static inline
u8* GetUnitSuportData(struct Unit* unit)
{
    return GetCharacterSupportData(unit->pCharacterData->number);
}

int NuggetUnitSupportLevel(struct Unit* unit, int num)
{
    int supportExp = HasUnitSupportData(unit)
        ? GetUnitSuportData(unit)[num]
        : UNIT_SUPPORT_DATA(unit)->supportExpBase[num];

    if (supportExp > 240)
        return SUPPORT_LEVEL_A;

    if (supportExp > 160)
        return SUPPORT_LEVEL_B;

    if (supportExp > 80)
        return SUPPORT_LEVEL_C;

    return SUPPORT_LEVEL_NONE;
}

void NuUnitGainSupportExp(struct Unit* unit, int num)
{
    if (HasUnitSupportData(unit) && UNIT_SUPPORT_DATA(unit))
    {
        u8* const data = GetUnitSuportData(unit);

        int gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
        int currentExp = data[num];
        int maxExp = sSupportMaxExpLut[NuggetUnitSupportLevel(unit, num)];

        if (currentExp + gain > maxExp)
            gain = maxExp - currentExp;

        data[num] = currentExp + gain;
        gChapterData.supportGainTotal += gain;
    }
}

void NuUnitGainSupportLevel(struct Unit* unit, int num)
{
    void (* const SetSupportLevelGained)(u8 charA, u8 charB) = (void*) (0x0802873C+1); // TODO: add to CLib reference and headers

    if (!HasUnitSupportData(unit))
        return;

    GetUnitSuportData(unit)[num]++;
    gChapterData.supportGainTotal++;

    SetSupportLevelGained(unit->pCharacterData->number, GetROMUnitSupportingId(unit, num));
}

s8 NuCanUnitSupportNow_Abridged(struct Unit* unit, int num)
{
    if (!HasUnitSupportData(unit))
        return FALSE;

    int const exp    = GetUnitSuportData(unit)[num];
    int const maxExp = sSupportMaxExpLut[NuggetUnitSupportLevel(unit, num)];

    if (exp == SUPPORT_EXP_A)
        return FALSE;

    return (exp == maxExp);
}

void NuClearUnitSupports(struct Unit* unit)
{
    int const count = GetROMUnitSupportCount(unit);
    int const charId = unit->pCharacterData->number;

    if (HasUnitSupportData(unit))
    {
        u8* const data = GetUnitSuportData(unit);

        for (int i = 0; i < count; ++i)
            data[i] = 0;
    }

    for (int i = 0; i < count; ++i)
    {
        struct Unit* const other = GetUnitSupportingUnit(unit, i);

        if (!other)
            continue;

        GetUnitSuportData(other)[GetSupportDataIdForOtherUnit(other, charId)] = 0;
    }
}
