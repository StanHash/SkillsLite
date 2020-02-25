
#pragma once

#include "gbafe.h"

enum { UNIT_SKILL_COUNT = 6 };

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
