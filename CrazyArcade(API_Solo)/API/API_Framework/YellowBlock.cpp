#include "stdafx.h"
#include "YellowBlock.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CYellowBlock::CYellowBlock()
{

}


CYellowBlock::~CYellowBlock()
{
}

void CYellowBlock::Initialize()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownBlockYellow.bmp", L"TownBlockYellow");

	m_tInfo.fX = 500.f;
	m_tInfo.fY = 200.f;

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroupID = RENDERID::OBJECT;
}

int CYellowBlock::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
	
}

void CYellowBlock::Late_Update()
{
}

void CYellowBlock::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownBlockYellow");

	BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, 0  // m_iDrawID * 64 는 사진 여러장 프레임 넘길때 타일처럼
		, SRCCOPY);
}

void CYellowBlock::Release()
{
}
