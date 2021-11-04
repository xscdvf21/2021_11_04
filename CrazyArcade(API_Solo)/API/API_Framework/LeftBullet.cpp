#include "stdafx.h"
#include "LeftBullet.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"




CLeftBullet::CLeftBullet()
{
}


CLeftBullet::~CLeftBullet()
{
}

void CLeftBullet::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/effect/BoomLeft.bmp", L"Boom_Left");



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

int CLeftBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CLeftBullet::Late_Update()
{

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();


}

void CLeftBullet::Render(HDC _DC)
{

	HDC hMemDC_Left = CBmpMgr::Get_Instance()->Find_Bmp(L"Boom_Left");

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

void CLeftBullet::Release()
{
}
