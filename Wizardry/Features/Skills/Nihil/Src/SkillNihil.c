
#include "SkillSystem.h"

extern u8 const NihilSkillId;

void BC_Nihil(struct BattleUnit* attacker, struct BattleUnit* defender)
{
    // We're handling both attacker and defender here to avoid cases where
    // Nihil kicks in after the other unit has had its skills applied

    int const attackerHasNihil = UnitHasSkill(&attacker->unit, NihilSkillId);
    int const defenderHasNihil = UnitHasSkill(&defender->unit, NihilSkillId);

    if (attackerHasNihil)
        UnitClearSkills(&defender->unit);

    if (defenderHasNihil)
        UnitClearSkills(&attacker->unit);
}
