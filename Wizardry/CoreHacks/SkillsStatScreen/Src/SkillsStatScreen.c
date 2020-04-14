
#include "gbafe.h"
#include "SkillSystem.h"

// TODO: add all that to CLib

struct StatScreenSt
{
    /* 00 */ u8 page;
    /* 01 */ u8 pageAmt;
    /* 02 */ u16 pageSlideKey;
    /* 04 */ short xDispOff;
    /* 06 */ short yDispOff;
    /* 08 */ s8 inTransition;
    /* 0C */ struct Unit* unit;
    /* 10 */ void* mu;
    /* 14 */ const void* help;
    /* 18 */ struct TextHandle text[];
};

struct HelpBoxProc
{
    /* 00 */ PROC_HEADER;

    /* 2C */ const struct HelpBoxInfo* info;

    /* 30 */ short xBox;
    /* 32 */ short yBox;
    /* 34 */ short wBox;
    /* 36 */ short hBox;
    /* 38 */ short xBoxInit;
    /* 3A */ short yBoxInit;
    /* 3C */ short xBoxFinal;
    /* 3E */ short yBoxFinal;
    /* 40 */ short wBoxInit;
    /* 42 */ short hBoxInit;
    /* 44 */ short wBoxFinal;
    /* 46 */ short hBoxFinal;
    /* 48 */ short timer;
    /* 4A */ short timerMax;

    /* 4C */ u16 mid;
    /* 4E */ u16 item;

    /* 50 */ u16 moveKey; // move ctrl proc only

    /* 52 */ u8 unk52;

    // NOTE: there's likely more, need to decompile more files
};

struct HelpBoxInfo
{
    /* 00 */ const struct HelpBoxInfo* adjUp;
    /* 04 */ const struct HelpBoxInfo* adjDown;
    /* 08 */ const struct HelpBoxInfo* adjLeft;
    /* 0C */ const struct HelpBoxInfo* adjRight;
    /* 10 */ u8 xDisplay;
    /* 11 */ u8 yDisplay;
    /* 12 */ u16 mid;
    /* 14 */ void(*redirect)(struct HelpBoxProc* proc);
    /* 18 */ void(*populate)(struct HelpBoxProc* proc);
};

enum { STATSCREEN_TEXT_BWL = 34 };

static struct StatScreenSt* const STATSCREEN = (struct StatScreenSt*) 0x02003BFC;

static u16* const TM_PAGEFRAME = (u16*) 0x02003D2C;

static struct HelpBoxInfo const* const HI_SS0_HP  = (struct HelpBoxInfo const*) 0x08A00BA8;
static struct HelpBoxInfo const* const HI_SS0_RES = (struct HelpBoxInfo const*) 0x08A00C50;
static struct HelpBoxInfo const* const HI_SS0_CND = (struct HelpBoxInfo const*) 0x08A00CF8;

static int (* const TryRelocateHbUp)(struct HelpBoxProc* proc) = (void const*) 0x08089355;
static int (* const TryRelocateHbDown)(struct HelpBoxProc* proc) = (void const*) 0x08089385;
static int (* const TryRelocateHbLeft)(struct HelpBoxProc* proc) = (void const*) 0x080893B5;
static int (* const TryRelocateHbRight)(struct HelpBoxProc* proc) = (void const*) 0x080893E5;

void NuDrawStatScreenBwl(void)
{
    static int const X = 1;
    static int const Y = 15;

    struct TextHandle* const text = STATSCREEN->text + STATSCREEN_TEXT_BWL;

    Text_Clear(text);

    Text_SetColorId(text, TEXT_COLOR_GOLD);
    Text_DrawString(text, "Skills");

    Text_Display(text, TM_PAGEFRAME + TILEMAP_INDEX(X, Y));

    u8 const* const skills = UnitGetSkillList(STATSCREEN->unit);

    for (int i = 0; i < UNIT_SKILL_COUNT; ++i)
    {
        if (IsSkill(skills[i]))
            DrawIcon(TM_PAGEFRAME + TILEMAP_INDEX(X + 4 + 2*i, Y), SKILL_ICON(skills[i]), TILEREF(0, 4));
    }
}

#define DefSkillHbFuncs(skillnum) \
static void HbRedirect_Skill ## skillnum (struct HelpBoxProc* proc) \
{ \
    u8 const* const skills = UnitGetSkillList(STATSCREEN->unit); \
    if (!IsSkill(skills[skillnum])) \
        TryRelocateHbUp(proc); \
} \
static void HbPopuplate_Skill ## skillnum (struct HelpBoxProc* proc) \
{ \
    u8 const* const skills = UnitGetSkillList(STATSCREEN->unit); \
    proc->mid = GetSkillDescriptionMsg(skills[skillnum]); \
}

DefSkillHbFuncs(0)
DefSkillHbFuncs(1)
DefSkillHbFuncs(2)
DefSkillHbFuncs(3)
DefSkillHbFuncs(4)
DefSkillHbFuncs(5)

#undef DefSkillHbFuncs

const struct HelpBoxInfo HelpInfo_Ss0Skill0;
const struct HelpBoxInfo HelpInfo_Ss0Skill1;
const struct HelpBoxInfo HelpInfo_Ss0Skill2;
const struct HelpBoxInfo HelpInfo_Ss0Skill3;
const struct HelpBoxInfo HelpInfo_Ss0Skill4;
const struct HelpBoxInfo HelpInfo_Ss0Skill5;

const struct HelpBoxInfo HelpInfo_Ss0Skill0 =
{
    HI_SS0_RES, NULL, HI_SS0_HP, &HelpInfo_Ss0Skill1,
    136, 136, 0, HbRedirect_Skill0, HbPopuplate_Skill0,
};

const struct HelpBoxInfo HelpInfo_Ss0Skill1 =
{
    HI_SS0_RES, NULL, &HelpInfo_Ss0Skill0, &HelpInfo_Ss0Skill2,
    152, 136, 0, HbRedirect_Skill1, HbPopuplate_Skill1,
};

const struct HelpBoxInfo HelpInfo_Ss0Skill2 =
{
    HI_SS0_CND, NULL, &HelpInfo_Ss0Skill1, &HelpInfo_Ss0Skill3,
    168, 136, 0, HbRedirect_Skill2, HbPopuplate_Skill2,
};

const struct HelpBoxInfo HelpInfo_Ss0Skill3 =
{
    HI_SS0_CND, NULL, &HelpInfo_Ss0Skill2, &HelpInfo_Ss0Skill4,
    184, 136, 0, HbRedirect_Skill3, HbPopuplate_Skill3,
};

const struct HelpBoxInfo HelpInfo_Ss0Skill4 =
{
    HI_SS0_CND, NULL, &HelpInfo_Ss0Skill3, &HelpInfo_Ss0Skill5,
    200, 136, 0, HbRedirect_Skill4, HbPopuplate_Skill4,
};

const struct HelpBoxInfo HelpInfo_Ss0Skill5 =
{
    HI_SS0_CND, NULL, &HelpInfo_Ss0Skill4, NULL,
    216, 136, 0, HbRedirect_Skill5, HbPopuplate_Skill5,
};
