#include "stdafx.h"
#include "Stage2.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TileMgr.h"
#include "Monster.h"
#include "Item.h"
#include "TileBlock.h"
#include "BoomItem.h"
#include "BoomSizeItem.h"
#include "BlockMgr.h"
#include "YellowBlockMgr.h"
#include "SpeedItemMgr.h"
#include "TownBlockMgr.h"
#include "TownBlock.h"
#include "PadoItemMgr.h"
#include "BoomCountMgr.h"
#include "RedHouseMgr.h"
#include "BlueHouseMgr.h"
#include "YellowHouseMgr.h"
#include "TreeMgr.h"
#include "BushMgr.h"
#include "SoundMgr.h"
#include "BossMonster.h"


CStage2::CStage2()
	:m_life(10), m_bOne(true)
{
}


CStage2::~CStage2()
{
}

void CStage2::Initialize()
{

	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/TownGround.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/role/Bosslife.bmp", L"Bosslife");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/YouWin.bmp", L"YouWin");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/YouLose.bmp", L"YouLose");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/mainBack.bmp", L"mainBack");

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->Initialize();

	CSoundMgr::Get_Instance()->PlaySoundW(L"bossstage.mp3", CSoundMgr::START);
	CSoundMgr::Get_Instance()->PlayBGM(L"Boss.mp3");

	CTileMgr::Get_Instance()->Load_Tile2();
	/*CBlockMgr::Get_Instance()->Load_RedBlock2();*/
	/*CYellowBlockMgr::Get_Instance()->Load_YellowBlock2();*/
	//CSpeedItemMgr::Get_Instance()->Load_SpeedItem2();
	//CPadoItemMgr::Get_Instance()->Load_PadoItem2();
	//CBoomCountMgr::Get_Instance()->Load_BoomCountItem2();
	/*CTownBlockMgr::Get_Instance()->Load_Town_Block2();*/
	CRedHouseMgr::Get_Instance()->Load_Red_House2();
	CBlueHouseMgr::Get_Instance()->Load_Blue_House2();
	CYellowHouseMgr::Get_Instance()->Load_Yellow_House2();
	CTreeMgr::Get_Instance()->Load_Tree2();
	CBushMgr::Get_Instance()->Load_Bush2();


	CObj*	pObj = CAbstractFactory<CPlayer>::Create();									//캐릭터 생성
	CObj*	pObjMoster = CAbstractFactory<CBossMonster>::Create(300.f, 260.f);			//보스 생성

	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	CObjMgr::Get_Instance()->Add_Object(pObjMoster, OBJID::BOSSMONSTER);
}

void CStage2::Update()
{


	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::BOSSMONSTER).size() && m_bOne)
	{
		m_dwtime = GetTickCount();
		m_bOne = false;
	}

	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::PLAYER).size() && m_bOne)
	{
		m_dwtime = GetTickCount();
		m_bOne = false;
	}
	CObjMgr::Get_Instance()->Update();
}

void CStage2::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage2::Render(HDC _DC)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"mainBack");

	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);



	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::BOSSMONSTER).size())
	{
		if (m_dwtime + 3000 > GetTickCount())
		{
			CSoundMgr::Get_Instance()->PlaySound(L"WinnerSound.mp3", CSoundMgr::PLAYER);
		}
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"YouWin");

		GdiTransparentBlt(_DC
			, 160, 200
			, 320, 80
			, hMemDC
			, 0, 0
			, 400, 120
			, RGB(255, 255, 255));

	}
	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::PLAYER).size())
	{

		if (m_dwtime + 1000 > GetTickCount())
		{
			CSoundMgr::Get_Instance()->PlaySound(L"LoseSound.mp3", CSoundMgr::PLAYER);
		}

		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"YouLose");

		GdiTransparentBlt(_DC
			, 160, 200
			, 320, 80
			, hMemDC
			, 0, 0
			, 400, 120
			, RGB(255, 255, 255));
	}

}

void CStage2::Release()
{
}
