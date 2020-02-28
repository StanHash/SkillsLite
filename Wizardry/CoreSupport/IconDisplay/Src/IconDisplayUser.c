
#include "IconDisplay.h"

#define ICON_SHEET(icon) (((icon) >> 8) & 0xFF)
#define ICON_INDEX(icon) ((icon) & 0xFF)

typedef const void* (*IDRGetIconFunc) (int id);

extern IDRGetIconFunc const IDRIconGetters[];

const void* GetIconGfx(unsigned icon)
{
	return IDRIconGetters[ICON_SHEET(icon)](ICON_INDEX(icon));
}

const void* GetVanillaIconGfx(unsigned id)
{
	extern const u8 gIconGraphics[];
	return gIconGraphics + 0x80*id;
}
