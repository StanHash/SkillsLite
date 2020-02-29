
#include "SkillSystem.h"

extern u8 CharmSkillId;

static inline
int CountCharms(int x, int y, int faction)
{
    static s8 const coordsLut[][2] =
    {
        // Every coord offsets for 'within 2 tiles' except (0, 0)

        { -2,  0 },
        { -1, -1 },
        { -1,  0 },
        { -1, +1 },
        {  0, -2 },
        {  0, -1 },
        {  0, +1 },
        {  0, +2 },
        { +1, -1 },
        { +1,  0 },
        { +1, +1 },
        { +2,  0 },
    };

    int count = 0;

    for (int i = 0; i < (sizeof(coordsLut)/sizeof(coordsLut[0])); ++i)
    {
        int const ix = x + coordsLut[i][0];
        int const iy = y + coordsLut[i][1];

        struct Unit* const unit = GetUnit(gMapUnit[iy][ix]);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (!AreAllegiancesAllied(UNIT_FACTION(unit), faction))
            continue;

        if (UnitHasSkill(unit, CharmSkillId))
            count++;
    }

    return count;
}

void BC_Charm(struct BattleUnit* attacker, struct BattleUnit* defender)
{
    int const count = CountCharms(attacker->unit.xPos, attacker->unit.yPos, UNIT_FACTION(&attacker->unit));

    if (count)
    {
        attacker->battleAttack += 2*count;
        attacker->battleDefense += 2*count;
    }
}
