#include "stdafx.h"
#include "RedHouse.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CRedHouse::CRedHouse()
{
}


CRedHouse::~CRedHouse()
{
}

void CRedHouse::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownHouseRed.bmp", L"TownHouseRed");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroupID = RENDERID::OBJECT;
}

int CRedHouse::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CRedHouse::Late_Update()
{
}

void CRedHouse::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownHouseRed");

	//BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
	//	, m_tInfo.iCX, m_tInfo.iCY
	//	, hMemDC
	//	, 0, 0  // m_iDrawID * 64 �� ���� ������ ������ �ѱ涧 Ÿ��ó��
	//	, SRCCOPY);

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownHouseRed");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
		, 40, 57
		, RGB(255, 0, 255));
}

void CRedHouse::Release()
{
}
