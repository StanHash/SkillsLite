
#include "gbafe.h"

int MLUStandardGetMaxLevel(struct Unit* unit)
{
	return (UNIT_CATTRIBUTES(unit) & CA_MAXLEVEL10) ? 10 : 20;
}

void MLUPromStandardStatGains(struct Unit* unit, u8 classId)
{
	const struct ClassData* pc = GetClassData(classId);

	unit->maxHP += pc->promotionHp;
	if (unit->maxHP > pc->maxHP)
		unit->maxHP = pc->maxHP;

	unit->pow += pc->promotionPow;
	if (unit->pow > pc->maxPow)
		unit->pow = pc->maxPow;

	unit->skl += pc->promotionSkl;
	if (unit->skl > pc->maxSkl)
		unit->skl = pc->maxSkl;

	unit->spd += pc->promotionSpd;
	if (unit->spd > pc->maxSpd)
		unit->spd = pc->maxSpd;

	unit->def += pc->promotionDef;
	if (unit->def > pc->maxDef)
		unit->def = pc->maxDef;

	unit->res += pc->promotionRes;
	if (unit->res > pc->maxRes)
		unit->res = pc->maxRes;

	unit->curHP += pc->promotionHp;
}

void MLUPromStandardWRankGains(struct Unit* unit, u8 classId)
{
	const struct ClassData* pc = GetClassData(classId);
	const struct ClassData* bc = unit->pClassData;

	for (unsigned i = 0; i < 8; ++i)
	{
		if (pc->baseRanks[i] == 0)
		{
			// If promoted class cannot use weapon, set wexp to 0 (generalization of the pupil -> shaman thing)

			unit->ranks[i] = WPN_EXP_0;
			continue;
		}

		int wexp = unit->ranks[i] - bc->baseRanks[i] + pc->baseRanks[i];

		if (wexp > WPN_EXP_S)
			wexp = WPN_EXP_S;

		if (wexp < WPN_EXP_E)
			wexp = WPN_EXP_E;

		unit->ranks[i] = wexp;
	}
}

void MLUPromStandardExpLevel(struct Unit* unit, u8 classId)
{
	unit->level = 1;

	unit->exp = UNIT_FACTION(unit) == FACTION_BLUE
		? 0
		: UNIT_EXP_DISABLED;
}

void MLUStandardGainDiff(struct BattleUnit* bu, struct Unit* unit)
{
	bu->changeHP  = bu->unit.maxHP    - unit->maxHP;
	bu->changePow = bu->unit.pow      - unit->pow;
	bu->changeSkl = bu->unit.skl      - unit->skl;
	bu->changeSpd = bu->unit.spd      - unit->spd;
	bu->changeDef = bu->unit.def      - unit->def;
	bu->changeRes = bu->unit.res      - unit->res;
	bu->changeLck = bu->unit.lck      - unit->lck;
	bu->changeCon = bu->unit.conBonus - unit->conBonus;
}
