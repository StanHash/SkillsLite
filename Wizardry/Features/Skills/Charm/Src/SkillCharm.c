
#include "SkillSystem.h"

extern u8 const CharmSkillId;

static inline
int CountCharms(int x, int y, int unitId)
{
    int const alliance = unitId & 0x80;

    int const begin = alliance + 1;
    int const end   = alliance + 0x80;

    int count = 0;

    for (int i = begin; i < end; ++i)
    {
        struct Unit* const unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (i == unitId)
            continue;

        if (RECT_DISTANCE(x, y, unit->xPos, unit->yPos) > 2)
            continue;

        if (UnitHasSkill(unit, CharmSkillId))
            count++;
    }

    return count;
}

void BC_Charm(struct BattleUnit* attacker, struct BattleUnit* defender)
{
    int const count = CountCharms(attacker->unit.xPos, attacker->unit.yPos, attacker->unit.index);

    if (count)
    {
        attacker->battleAttack += 2*count;
        attacker->battleDefense += 2*count;
    }
}
