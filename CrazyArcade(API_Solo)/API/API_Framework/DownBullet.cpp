#include "stdafx.h"
#include "DownBullet.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CDownBullet::CDownBullet()
{
}


CDownBullet::~CDownBullet()
{
}

void CDownBullet::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/effect/BoomDown.bmp", L"BoomDown");



	m_dwTime = GetTickCount();

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameTime = 200;
	m_tFrame.dwTime = GetTickCount();

	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_fSpeed = 10.f;

	m_eGroupID = RENDERID::OBJECT;

	Update_Rect();
}

int CDownBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CDownBullet::Late_Update()
{
}

void CDownBullet::Render(HDC _DC)
{
	HDC hMemDC_Left = CBmpMgr::Get_Instance()->Find_Bmp(L"BoomDown");

	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC_Left
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
		, 40, 40
		, RGB(0, 0, 0));

	if (m_dwTime + 300 < GetTickCount())
	{

		m_bDead = true;
	}
}

void CDownBullet::Release()
{
}
