
#include "SkillSystem.h"

extern u8 const WrathSkillId;

void BC_Wrath(struct BattleUnit* attacker, struct BattleUnit* defender)
{
    if (attacker == &gBattleActor)
        return; // Wrath doesn't activate when attacking

    if (!UnitHasSkill(&attacker->unit, WrathSkillId))
        return; // Wrath doesn't active without the Wrath skill, of course

    if (attacker->unit.curHP <= (attacker->unit.maxHP)/2)
    {
        attacker->battleCritRate = 100;
        defender->battleDodgeRate = 0;
    }
}
