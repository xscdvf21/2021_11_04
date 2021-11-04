#include "stdafx.h"
#include "YellowHouse.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CYellowHouse::CYellowHouse()
{
}


CYellowHouse::~CYellowHouse()
{
}

void CYellowHouse::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownHouseYellow.bmp", L"TownHouseYellow");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroupID = RENDERID::OBJECT;
}

int CYellowHouse::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CYellowHouse::Late_Update()
{
}

void CYellowHouse::Render(HDC _DC)
{

	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownHouseYellow");

	//BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
	//	, m_tInfo.iCX, m_tInfo.iCY
	//	, hMemDC
	//	, 0, 0  // m_iDrawID * 64 는 사진 여러장 프레임 넘길때 타일처럼
	//	, SRCCOPY);


		GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
		, 40, 57
		, RGB(255, 0, 255));
}

void CYellowHouse::Release()
{
}
