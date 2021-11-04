#include "stdafx.h"
#include "BoomPopo.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CBoomPopo::CBoomPopo()
{
}


CBoomPopo::~CBoomPopo()
{
	Release();
}

void CBoomPopo::Scene_Change()
{

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 4;
		m_tFrame.dwFrameTime = 1000;
		m_tFrame.dwTime = GetTickCount();

}

void CBoomPopo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/effect/Popo.bmp", L"Boom_Popo");

	m_pFramekey = L"Boom_Popo";
}

int CBoomPopo::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	Scene_Change();

	Update_Frame();
	return OBJ_NOEVENT;
}


void CBoomPopo::Late_Update()
{
}

void CBoomPopo::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFramekey);

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 50, m_tFrame.iFrameScene * 0
		, 50, 60
		, RGB(255, 255, 255));
}

void CBoomPopo::Release()
{
}
