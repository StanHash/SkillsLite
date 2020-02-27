
#pragma once

#include "gbafe.h"

enum { UNIT_SKILL_COUNT = 6 };

/**
 * Represents misc constant information on a particular skill
 */
struct SkillInfo
{
    u16 msgName;      //* name text id
    u16 msgDesc;      //* description text id
    const void* icon; //* icon gfx address
};

extern const struct SkillInfo* const SkillInfoTable[];

#define SKILL_ICON(aSkillId) ((1 << 8) + (aSkillId))

/**
 * Gets a pointer to the unit's skill list
 * 
 * @param unit the unit
 * @return address of unit's skill list
 */
u8* UnitGetSkillList(struct Unit* unit);

/**
 * Checks whether a unit has a given skill
 * 
 * @param unit the unit
 * @param skillId the id of the skill to check for
 * @return non-zero if the unit has the given skill
 */
int UnitHasSkill(struct Unit* unit, int skillId);

/**
 * Counts the amount of skills the given unit has.
 * 
 * @param unit the unit
 * @return the amount of skills the unit has
 */
int UnitCountSkills(struct Unit* unit);

/**
 * Attempts to add a skill to the given unit.
 * 
 * @param unit the unit
 * @param skillId the id of the skill to add
 * @return zero if no skill could be added (skill list was full)
 */
int UnitAddSkill(struct Unit* unit, int skillId);

/**
 * Get skill name text id by skill id
 * 
 * @param skillId id of skill
 * @return name text id
 */
int GetSkillNameMsg(int skillId);

/**
 * Get skill description text id by skill id
 * 
 * @param skillId id of skill
 * @return description text id
 */
int GetSkillDescriptionMsg(int skillId);

/**
 * Gets address of skill icon image by skill id (for IconDisplay)
 * 
 * @param skillId id of skill
 * @return address of icon gfx
 */
const void* GetSkillIconGfx(int skillId);
