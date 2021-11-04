#include "stdafx.h"
#include "Tree.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CTree::CTree()
{
}


CTree::~CTree()
{
}

void CTree::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/map/TownTree.bmp", L"TownTree");

	m_tInfo.iCX = 40;
	m_tInfo.iCY = 40;

	m_eGroupID = RENDERID::OBJECT;
}

int CTree::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CTree::Late_Update()
{
}

void CTree::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownHouseRed");

	//BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top
	//	, m_tInfo.iCX, m_tInfo.iCY
	//	, hMemDC
	//	, 0, 0  // m_iDrawID * 64 는 사진 여러장 프레임 넘길때 타일처럼
	//	, SRCCOPY);

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"TownTree");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 40, m_tFrame.iFrameScene * 0
		, 40, 67
		, RGB(255, 0, 255));
}

void CTree::Release()
{
}
