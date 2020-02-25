
#include "SkillSystem.h"

u8* UnitGetSkillList(struct Unit* unit)
{
    return unit->supports;
}

int UnitHasSkill(struct Unit* unit, int skillId)
{
    u8* const skills = UnitGetSkillList(unit);

    for (int i = 0; i < UNIT_SKILL_COUNT && skills[i]; ++i)
    {
        if (skills[i] == skillId)
            return TRUE;
    }

    return FALSE;
}

int UnitCountSkills(struct Unit* unit)
{
    u8* const skills = UnitGetSkillList(unit);

    int count = 0;

    for (int i = 0; i < UNIT_SKILL_COUNT && skills[i]; ++i)
        count++;

    return count;
}

int UnitAddSkill(struct Unit* unit, int skillId)
{
    u8* const skills = UnitGetSkillList(unit);

    for (int i = 0; i < UNIT_SKILL_COUNT; ++i)
    {
        if (!skills[i])
        {
            skills[i] = skillId;
            return TRUE;
        }
    }

    return FALSE;
}
