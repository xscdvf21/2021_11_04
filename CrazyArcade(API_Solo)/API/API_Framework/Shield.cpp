#include "stdafx.h"
#include "Shield.h"
#include "ObjMgr.h"


CShield::CShield()
	: m_fDis(0.f)
{
}


CShield::~CShield()
{
	Release();
}

void CShield::Initialize()
{
	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;

	m_fSpeed = 10.f;
	m_fDis = 150.f;

	m_eGroupID = RENDERID::OBJECT;
}

int CShield::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pTarget = CObjMgr::Get_Instance()->Get_Player();
		m_bStart = false;
	}

	m_fAngle += m_fSpeed;

	m_tInfo.fX = m_pTarget->Get_Info().fX + cosf(m_fAngle * PI / 180.f) * m_fDis;
	m_tInfo.fY = m_pTarget->Get_Info().fY - sinf(m_fAngle * PI / 180.f) * m_fDis;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CShield::Late_Update()
{
}

void CShield::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CShield::Release()
{
}
