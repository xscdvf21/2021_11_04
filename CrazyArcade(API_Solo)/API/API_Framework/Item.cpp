#include "stdafx.h"
#include "Item.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CItem::CItem()
{
}


CItem::~CItem()
{
}

void CItem::Initialize()
{

	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/item/Gift2.bmp", L"Gift2");



	m_tInfo.iCX = 35;
	m_tInfo.iCY = 35;

	m_eGroupID = RENDERID::ITEM;
}

int CItem::Update()
{

	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CItem::Late_Update()
{
}

void CItem::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Gift2");

	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart * 42, m_tFrame.iFrameScene * 0
		, 42, 45
		, RGB(255, 0, 255));
}

void CItem::Release()
{
}
