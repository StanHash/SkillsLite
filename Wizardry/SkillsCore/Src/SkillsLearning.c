
#include "SkillSystem.h"

extern const u8* const ClassSkillLists[];
extern const u8* const CharacterSkillLists[];

extern const u8 PromotedInternalLevelBonus;

int GetLevelUpSkill(unsigned level, unsigned charId, unsigned classId)
{
    const u8* charSkills = CharacterSkillLists[charId];

    if (charSkills)
    {
        while (charSkills[0])
        {
            int const learnLevel = charSkills[0];
            int const learnClass = charSkills[1];
            int const skillId = charSkills[2];

            if ((learnClass == 0) || (learnClass == classId))
                if (learnLevel == level)
                    return skillId;

            charSkills = charSkills + 3;
        }
    }

    const u8* classSkills = ClassSkillLists[classId];

    if (classSkills)
    {
        level = Mod(level, PromotedInternalLevelBonus);

        while (classSkills[0])
        {
            int const learnLevel = classSkills[0];
            int const skillId = classSkills[1];

            if (level == learnLevel)
                return skillId;

            classSkills = classSkills + 2;
        }
    }

    return 0;
}

int UnitGetLevelUpSkill(struct Unit* unit)
{
    return GetLevelUpSkill(
        (UNIT_CATTRIBUTES(unit) & CA_PROMOTED)
            ? unit->level + PromotedInternalLevelBonus
            : unit->level,
        unit->pCharacterData->number,
        unit->pClassData->number);
}

void UnitAutolevelSkills(struct Unit* unit)
{
    UnitClearSkills(unit);

    const u8* charSkills = CharacterSkillLists[unit->pCharacterData->number];

    if (charSkills)
    {
        int const unitLevel = (UNIT_CATTRIBUTES(unit) & CA_PROMOTED)
            ? unit->level + PromotedInternalLevelBonus
            : unit->level;

        int const unitClass = unit->pCharacterData->number;

        while (charSkills[0])
        {
            int const learnLevel = charSkills[0];
            int const learnClass = charSkills[1];
            int const skillId = charSkills[2];

            if ((learnClass == 0) || (learnClass == unitClass))
                if ((learnLevel == 0xFF) || (learnLevel <= unitLevel))
                    UnitAddSkill(unit, skillId);

            charSkills = charSkills + 3;
        }
    }

    const u8* classSkills = ClassSkillLists[unit->pClassData->number];

    if (classSkills)
    {
        int const unitLevel = unit->level;

        while (classSkills[0])
        {
            int const learnLevel = classSkills[0];
            int const skillId = classSkills[1];

            // Skill is learned if learning level is lower than unit level,
            // Or learning level is 0xFF (used to give promoted generics unpromoted classes' skills)

            if ((learnLevel == 0xFF) || (learnLevel <= unitLevel))
                UnitAddSkill(unit, skillId);

            classSkills = classSkills + 2;
        }
    }
}

void OnLevelUp_TryLearnSkill(struct BattleUnit* bu)
{
    BU_SKILL_LEARNED(bu) = UnitGetLevelUpSkill(&bu->unit);
}

int PopR_InitSkillLearning(void)
{
    if (BU_SKILL_LEARNED(&gBattleActor))
    {
        SetPopupItem(BU_SKILL_LEARNED(&gBattleActor));
        BU_SKILL_LEARNED(&gBattleActor) = 0;

        return TRUE;
    }
    else if (BU_SKILL_LEARNED(&gBattleTarget))
    {
        SetPopupItem(BU_SKILL_LEARNED(&gBattleTarget));
        BU_SKILL_LEARNED(&gBattleTarget) = 0;

        return TRUE;
    }

    return FALSE;
}

void OnUnit2Battle_ClearLearnedSkills(struct BattleUnit* bu, struct Unit* unit)
{
    BU_SKILL_LEARNED(bu) = 0;
    BU_SKILL_FORGET_SLOT(bu) = (-1);
}

void OnBattle2Unit_UpdateSkills(struct Unit* unit, struct BattleUnit* bu)
{
    if (BU_SKILL_FORGET_SLOT(bu) >= 0)
    {
        UnitGetSkillList(unit)[BU_SKILL_FORGET_SLOT(bu)] = 0;
        UnitClearBlankSkills(unit);
    }

    if (BU_SKILL_LEARNED(bu))
    {
        UnitAddSkill(unit, BU_SKILL_LEARNED(bu));
    }
}
