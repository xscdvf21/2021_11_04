#include "stdafx.h"
#include "TownBlock.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CTownBlock::CTownBlock()
{
}


CTownBlock::~CTownBlock()
{
}

void CTownBlock::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownBox.bmp", L"TownBox");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroupID = RENDERID::OBJECT;
}

int CTownBlock::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();



	return OBJ_NOEVENT;

}

void CTownBlock::Late_Update()
{
}

void CTownBlock::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownBox");

	BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, 0  // m_iDrawID * 64 는 사진 여러장 프레임 넘길때 타일처럼
		, SRCCOPY);

	//if (m_tInfo.fX <= 40)
	//{
	//	m_tInfo.fX = 40;
	//}

	//if (m_tInfo.fX >= 760)
	//{
	//	m_tInfo.fX = 760;
	//}

	//if (m_tInfo.fY <= 40)
	//{
	//	m_tInfo.fY = 40;
	//}

	//if (m_tInfo.fY >= 560)
	//{
	//	m_tInfo.fY = 560;
	//}
}

void CTownBlock::Release()
{
}
