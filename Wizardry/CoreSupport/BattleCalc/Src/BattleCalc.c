
#include "gbafe.h"

typedef void (*BCFunc) (struct BattleUnit*, struct BattleUnit*);

extern const BCFunc BattleCalcFuncList[];

void NuComputeBattleStats(struct BattleUnit* subject, struct BattleUnit* target)
{
    const BCFunc* it = BattleCalcFuncList;

    while (*it)
        (*it++)(subject, target);
}
