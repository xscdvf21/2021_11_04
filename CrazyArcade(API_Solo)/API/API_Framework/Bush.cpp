#include "stdafx.h"
#include "Bush.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CBush::CBush()
{
}


CBush::~CBush()
{
}

void CBush::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownBush.bmp", L"TownBush");

	m_tInfo.iCX = 50;
	m_tInfo.iCY = 55;

	m_eGroupID = RENDERID::BUSH;
}

int CBush::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBush::Late_Update()
{
}

void CBush::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	//BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
	//	, m_tInfo.iCX, m_tInfo.iCY
	//	, hMemDC
	//	, 0, 0  // m_iDrawID * 64 는 사진 여러장 프레임 넘길때 타일처럼
	//	, SRCCOPY);
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownBush");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
		, 40, 57
		, RGB(255, 0, 255));
}

void CBush::Release()
{
}
