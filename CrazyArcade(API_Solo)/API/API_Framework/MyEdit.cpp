#include "stdafx.h"
#include "MyEdit.h"
#include "TileMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "TileBlock.h"
#include "BlockMgr.h"
#include "YellowBlockMgr.h"
#include "SpeedItemMgr.h"
#include "PadoItemMgr.h"
#include "BoomCountMgr.h"
#include "TownBlockMgr.h"
#include "RedHouseMgr.h"
#include "YellowHouseMgr.h"
#include "BlueHouseMgr.h"
#include "TreeMgr.h"
#include "BushMgr.h"


CMyEdit::CMyEdit()
{
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/mainBack.bmp", L"mainback");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/TownGround.bmp", L"Tile");
	CTileMgr::Get_Instance()->Initialize();

}

void CMyEdit::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(1);
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(0);
	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
		CTileMgr::Get_Instance()->Picking_Tile(2);
	if (CKeyMgr::Get_Instance()->Key_Down('W'))
		CTileMgr::Get_Instance()->Picking_Tile(3);
	if (CKeyMgr::Get_Instance()->Key_Down('E'))
		CTileMgr::Get_Instance()->Picking_Tile(4);
	if (CKeyMgr::Get_Instance()->Key_Down('R'))
		CTileMgr::Get_Instance()->Picking_Tile(5);
	if (CKeyMgr::Get_Instance()->Key_Down('T'))
		CTileMgr::Get_Instance()->Picking_Tile(6);

	if (CKeyMgr::Get_Instance()->Key_Down('O'))
		CBlockMgr::Get_Instance()->Red_Block();
	if (CKeyMgr::Get_Instance()->Key_Down('P'))
		CYellowBlockMgr::Get_Instance()->Yellow_Block();
	if (CKeyMgr::Get_Instance()->Key_Down('L'))
		CSpeedItemMgr::Get_Instance()->Speed_Item();
	if (CKeyMgr::Get_Instance()->Key_Down('K'))
		CPadoItemMgr::Get_Instance()->Pado_Item();
	if (CKeyMgr::Get_Instance()->Key_Down('J'))
		CBoomCountMgr::Get_Instance()->BoomCount_Item();
	if (CKeyMgr::Get_Instance()->Key_Down('H'))
		CTownBlockMgr::Get_Instance()->Town_Block();
	if (CKeyMgr::Get_Instance()->Key_Down('G'))
		CRedHouseMgr::Get_Instance()->Red_House();
	if (CKeyMgr::Get_Instance()->Key_Down('F'))
		CBlueHouseMgr::Get_Instance()->Blue_House();
	if (CKeyMgr::Get_Instance()->Key_Down('D'))
		CYellowHouseMgr::Get_Instance()->Yellow_House();
	if (CKeyMgr::Get_Instance()->Key_Down('M'))
		CTreeMgr::Get_Instance()->Tree();
	if (CKeyMgr::Get_Instance()->Key_Down('B'))
		CBushMgr::Get_Instance()->Bush();

	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		CTileMgr::Get_Instance()->Save_Tile2();
		CBlockMgr::Get_Instance()->Save_RedBlock2();
		CYellowBlockMgr::Get_Instance()->Save_YellowBlock2();
		CSpeedItemMgr::Get_Instance()->Save_SpeedItem2();
		CPadoItemMgr::Get_Instance()->Save_PadoItem2();
		CBoomCountMgr::Get_Instance()->Save_BoomCountItem2();
		CTownBlockMgr::Get_Instance()->Save_Town_Block2();
		CRedHouseMgr::Get_Instance()->Save_Red_House2();
		CBlueHouseMgr::Get_Instance()->Save_Blue_House2();
		CYellowHouseMgr::Get_Instance()->Save_Yellow_House2();
		CTreeMgr::Get_Instance()->Save_Tree2();
		CBushMgr::Get_Instance()->Save_Bush2();

	}
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		CTileMgr::Get_Instance()->Load_Tile2();
		CBlockMgr::Get_Instance()->Load_RedBlock2();
		CYellowBlockMgr::Get_Instance()->Load_YellowBlock2();
		CSpeedItemMgr::Get_Instance()->Load_SpeedItem2();
		CPadoItemMgr::Get_Instance()->Load_PadoItem2();
		CBoomCountMgr::Get_Instance()->Load_BoomCountItem2();
		CTownBlockMgr::Get_Instance()->Load_Town_Block2();
		CRedHouseMgr::Get_Instance()->Load_Red_House2();
		CBlueHouseMgr::Get_Instance()->Load_Blue_House2();
		CYellowHouseMgr::Get_Instance()->Load_Yellow_House2();
		CTreeMgr::Get_Instance()->Load_Tree2();
		CBushMgr::Get_Instance()->Load_Bush2();
	}
}



void CMyEdit::Late_Update()
{
}

void CMyEdit::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"mainback");

	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(_DC);
	CBlockMgr::Get_Instance()->Render(_DC);
	CYellowBlockMgr::Get_Instance()->Render(_DC);
	CSpeedItemMgr::Get_Instance()->Render(_DC);
	CPadoItemMgr::Get_Instance()->Render(_DC);
	CBoomCountMgr::Get_Instance()->Render(_DC);
	CTownBlockMgr::Get_Instance()->Render(_DC);
	CRedHouseMgr::Get_Instance()->Render(_DC);
	CBlueHouseMgr::Get_Instance()->Render(_DC);
	CYellowHouseMgr::Get_Instance()->Render(_DC);
	CTreeMgr::Get_Instance()->Render(_DC);
	CBushMgr::Get_Instance()->Render(_DC);


	
}


void CMyEdit::Release()
{
	CTileMgr::Destroy_Instance();
	CBlockMgr::Destroy_Instance();
	CYellowBlockMgr::Destroy_Instance();
	CSpeedItemMgr::Destroy_Instance();
	CPadoItemMgr::Destroy_Instance();
	CBoomCountMgr::Destroy_Instance();
	CTownBlockMgr::Destroy_Instance();
	CRedHouseMgr::Destroy_Instance();
	CBlueHouseMgr::Destroy_Instance();
	CYellowHouseMgr::Destroy_Instance();
	CTreeMgr::Destroy_Instance();
	CBushMgr::Destroy_Instance();
}
