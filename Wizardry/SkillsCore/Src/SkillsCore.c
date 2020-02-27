
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

// ======================
// = SKILL INFO GETTERS =
// ======================

int GetSkillNameMsg(int skillId)
{
    return SkillInfoTable[skillId]->msgName;
}

int GetSkillDescriptionMsg(int skillId)
{
    return SkillInfoTable[skillId]->msgDesc;
}

const void* GetSkillIconGfx(int skillId)
{
    return SkillInfoTable[skillId]->icon;
}

// =========================
// = SKILL POPR COMPONENTS =
// =========================

#include "PopupRework.h"

static int  SS_PopRSkillIcon_GetLength(struct PopupReworkProc* proc, u32 argument);
static void SS_PopRSkillIcon_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument);

const struct PopupComponentType PopRComponent_SkillIcon =
{
    .getLength = SS_PopRSkillIcon_GetLength,
    .display   = SS_PopRSkillIcon_Display,
};

static int SS_PopRSkillIcon_GetLength(struct PopupReworkProc* proc, u32 argument)
{
    return 16;
}

static void SS_PopRSkillIcon_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument)
{
    proc->addIcon(proc, SKILL_ICON(gPopupItem), Text_GetXCursor(text));
    Text_Advance(text, 16);

    LoadIconPalette(0, proc->pop.iconPalId);
}

static int  SS_PopRSkillName_GetLength(struct PopupReworkProc* proc, u32 argument);
static void SS_PopRSkillName_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument);

const struct PopupComponentType PopRComponent_SkillName =
{
    .getLength = SS_PopRSkillName_GetLength,
    .display   = SS_PopRSkillName_Display,
};

static int  SS_PopRSkillName_GetLength(struct PopupReworkProc* proc, u32 argument)
{
    return Text_GetStringTextWidth(GetStringFromIndex(GetSkillNameMsg(gPopupItem)));
}

static void SS_PopRSkillName_Display(struct PopupReworkProc* proc, struct TextHandle* text, u32 argument)
{
    Text_DrawString(text, GetStringFromIndex(GetSkillNameMsg(gPopupItem)));
}
