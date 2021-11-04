#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace BULLET
{
	enum DIR { LEFT, RIGHT, UP, DOWN, END };
}

namespace OBJID
{
	enum ID { PLAYER, MONSTER,BOSSMONSTER,MONSTERBULLET, BULLET,LEFTPADO,RIGHTPADO,UPPADO,DOWNPADO
		, MOUSE, SHIELD, UI,SPEEDITEM,BOOMITEM,BOOMSIZEITEM, REDBLOCK, YELLOWBLOCK
		, TOWNBLOCK , HOUSE, TREE,BUSH ,MONSTERLEFTPADO, MONSTERRIGHTPADO, MONSTERUPPADO,MONSTERDOWNPADO
		,END };
}

namespace RENDERID
{
	enum ID { BACKGROUND, ITEM, OBJECT,BUSH, BOSS, EFFECT, UI, END };
}

#endif // !__ENUM_H__