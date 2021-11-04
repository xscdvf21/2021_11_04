#include "stdafx.h"
#include "ScrewBullet.h"


CScrewBullet::CScrewBullet()
	: m_fCenterAngle(0.f), m_fCenterSpeed(0.f), m_fCenterDis(0.f)
{
	ZeroMemory(&m_tCenterPos, sizeof(m_tCenterPos));
}


CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Initialize()
{
	m_tInfo.iCX = 20;
	m_tInfo.iCY = 20;

	// 공전 속도
	m_fSpeed = 30.f;

	// 중점의 이동 속도
	m_fCenterSpeed = 5.f;

	// 중점으로부터 미사일의 거리
	m_fCenterDis = 30.f;

	m_eGroupID = RENDERID::OBJECT;
}

int CScrewBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_tCenterPos.x = (LONG)m_tInfo.fX;
		m_tCenterPos.y = (LONG)m_tInfo.fY;
		m_fCenterAngle = m_fAngle;
		m_bStart = false;
	}

	m_tCenterPos.x += (LONG)(cosf(m_fCenterAngle * PI / 180.f) * m_fCenterSpeed);
	m_tCenterPos.y -= (LONG)(sinf(m_fCenterAngle * PI / 180.f) * m_fCenterSpeed);


	m_fAngle += m_fSpeed;
	m_tInfo.fX = m_tCenterPos.x + cosf(m_fAngle * PI / 180.f) * m_fCenterDis;
	m_tInfo.fY = m_tCenterPos.y - sinf(m_fAngle * PI / 180.f) * m_fCenterDis;


	Update_Rect();
	return OBJ_NOEVENT;
}

void CScrewBullet::Late_Update()
{
	if (m_tRect.top <= 100 || m_tRect.left <= 100
		|| m_tRect.bottom >= WINCY - 100 || m_tRect.right >= WINCX - 100)
		m_bDead = true;
}

void CScrewBullet::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CScrewBullet::Release()
{
}
