#include "stdafx.h"
#include "BlueHouse.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CBlueHouse::CBlueHouse()
{
}


CBlueHouse::~CBlueHouse()
{
}

void CBlueHouse::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownHouseBlue.bmp", L"TownHouseBlue");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroupID = RENDERID::OBJECT;
}

int CBlueHouse::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBlueHouse::Late_Update()
{
}

void CBlueHouse::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	//BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
	//	, m_tInfo.iCX, m_tInfo.iCY
	//	, hMemDC
	//	, 0, 0  // m_iDrawID * 64 는 사진 여러장 프레임 넘길때 타일처럼
	//	, SRCCOPY);
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownHouseBlue");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
		, 40, 57
		, RGB(255, 0, 255));

}

void CBlueHouse::Release()
{
}
