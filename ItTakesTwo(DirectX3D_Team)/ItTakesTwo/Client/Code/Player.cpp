#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

void CPlayer::Add_CollisionList(CGameObject * pGameObject)
{
	m_vecCollisionList.emplace_back(pGameObject);
}

_bool CPlayer::Check_CollisionList(CGameObject * pGameObject)
{
	for (auto& iter : m_vecCollisionList)
	{
		if (iter == pGameObject)
			return true;
	}
	return false;
}

void CPlayer::Clear_CollisionList()
{
	m_vecCollisionList.clear();
}

void CPlayer::Set_Hit(HITTYPE eType)
{
	switch (eType)
	{
	case DEFAULT:
		m_bHit = true;
		break;
	case CHESSBOSS:
		m_bHit = true;
		m_bKnockback = true;
		break;
	}
}

void CPlayer::Set_Damage(_int iHp)
{
	m_bHitAble = true;
	m_iHp += iHp;
}

void CPlayer::Clear_HitAble(const _float fTimeDelta)
{
	if (true == m_bHitAble)
	{
		m_fHitClearTime += fTimeDelta;
		if (m_fHitClearTime >= 1.5f)
		{
			m_bHitAble = false;
			m_fHitClearTime = 0.f;
		}
	}
}

void CPlayer::Free()
{
	CGameObject::Free();
}
