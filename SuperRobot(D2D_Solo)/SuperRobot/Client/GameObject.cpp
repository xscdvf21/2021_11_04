#include "stdafx.h"
#include "GameObject.h"

//int		CGameObject::static_iAllCheck = 0;
CGameObject::CGameObject()
	:pTexInfo(nullptr)
	, m_pTile(nullptr)
	, m_bSelect(false)
	, m_bCheck(true)
	, m_bDead(false)
	, m_bUnitMenuCheck(true)
	, m_iAlpha(255)
	, m_iSceneNum(0)
	, m_pTextureKey(nullptr)
	, static_iAllCheck(0)
	, m_bUnitSelect(false)
	, m_bTileCheck(false)
	, m_bMoveTile(false)
	, m_bAttaker(false)
	, monster_iAllCheck(0)
	, m_fTurnDelay(0.f)
	, m_bMonsterTurnCheck(false)
	, m_bUnitSkillCheck(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO)); 
	ZeroMemory(&m_tMonInfo, sizeof(MONSTERINFO));
}


CGameObject::~CGameObject()
{
}
