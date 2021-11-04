#include "stdafx.h"
#include "Stage.h"
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
#include "Monster2.h"
#include "SceneMgr.h"
#include "KeyMgr.h"


CStage::CStage()
	:m_bOne(true)
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/TownGround.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/mainBack.bmp", L"mainBack");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/start.bmp", L"start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/YouWin.bmp", L"YouWin");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/background/YouLose.bmp", L"YouLose");
	/*CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);*/
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->Initialize();

	CSoundMgr::Get_Instance()->PlaySoundW(L"GameStart.mp3", CSoundMgr::START);
	CSoundMgr::Get_Instance()->PlayBGM(L"InGameBGM.mp3");
	//CTileMgr::Get_Instance()->Load_Tile();
	//CBlockMgr::Get_Instance()->Load_RedBlock();
	//CYellowBlockMgr::Get_Instance()->Load_YellowBlock();
	//CSpeedItemMgr::Get_Instance()->Load_SpeedItem();

	CTileMgr::Get_Instance()->Load_Tile();
	CBlockMgr::Get_Instance()->Load_RedBlock();
	CYellowBlockMgr::Get_Instance()->Load_YellowBlock();
	CSpeedItemMgr::Get_Instance()->Load_SpeedItem();
	CPadoItemMgr::Get_Instance()->Load_PadoItem();
	CBoomCountMgr::Get_Instance()->Load_BoomCountItem();
	CTownBlockMgr::Get_Instance()->Load_Town_Block();
	CRedHouseMgr::Get_Instance()->Load_Red_House();
	CBlueHouseMgr::Get_Instance()->Load_Blue_House();
	CYellowHouseMgr::Get_Instance()->Load_Yellow_House();
	CTreeMgr::Get_Instance()->Load_Tree();
	CBushMgr::Get_Instance()->Load_Bush();

	CObj*	pObj = CAbstractFactory<CPlayer>::Create();									//캐릭터 생성
	CObj*	pObjMoster = CAbstractFactory<CMonster>::Create();							//몬스터 생성
	CObj*	pObjMonster2 = CAbstractFactory<CMonster2>::Create(60.f, 500.f);			//슬라임 생성
	//CObj*	pObjItem = CAbstractFactory<CItem>::Create(400.f, 400.f);					//신발 아이템 생성
	//CObj*	pObjBoomItem = CAbstractFactory<CBoomItem>::Create(400.f, 200.f);			// 폭탄 아이템 생성
	//CObj*	pObjBoomSizeItem = CAbstractFactory<CBoomSizeItem>::Create(400.f, 300.f);	// 폭탄사이즈 아이템
	////CObj*	pObjTileBlock = CAbstractFactory<CTileBlock>::Create(20.f, 20.f);			//빨간 블록 생성
	/*CObj*	pObjTownBlock = CAbstractFactory<CTownBlock>::Create(500.f, 300.f);*/


	//CObjMgr::Get_Instance()->Add_Object(pObjItem, OBJID::SPEEDITEM);
	//CObjMgr::Get_Instance()->Add_Object(pObjBoomItem, OBJID::BOOMITEM);
	//CObjMgr::Get_Instance()->Add_Object(pObjBoomSizeItem, OBJID::BOOMSIZEITEM);
	/*CObjMgr::Get_Instance()->Add_Object(pObjTownBlock, OBJID::TOWNBLOCK);*/

	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	CObjMgr::Get_Instance()->Add_Object(pObjMoster, OBJID::MONSTER);
	CObjMgr::Get_Instance()->Add_Object(pObjMonster2, OBJID::MONSTER);
	/*CObjMgr::Get_Instance()->Add_Object(pObjTileBlock, OBJID::REDBLOCK);*/

	//CObj*	pObj = CAbstractFactory<CMonster>::Create(400.f, 200.f);
	//CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::MONSTER);

	
	m_dwtime = GetTickCount();
}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();

	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::PLAYER).size() && m_bOne)
	{
		m_dwtime2 = GetTickCount();
		m_bOne = false;
	}

	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::MONSTER).size())
	{

		CObjMgr::Destroy_Instance();

		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE2);
	}
	if (CKeyMgr::Get_Instance()->Key_Down('9'))
	{

		CObjMgr::Destroy_Instance();
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE2);
		
	}


}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC _DC)
{

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"mainBack");

	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
	//CBlockMgr::Get_Instance()->Render(_DC);  //쓰면은 아이템을 먹거나 블럭이 사라지면 터짐
	//CYellowBlockMgr::Get_Instance()->Render(_DC);


	if (m_dwtime + 3000 > GetTickCount())
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"start");

		GdiTransparentBlt(_DC
			, 160,200
			, 320, 80
			, hMemDC
			, 0, 0
			, 400, 120
			, RGB(255, 255, 255));
	}

	if (0 >= CObjMgr::Get_Instance()->Get_list(OBJID::MONSTER).size())
	{
		if (m_dwtime2 + 3000 > GetTickCount())
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

		if (m_dwtime2 + 1000 > GetTickCount())
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

void CStage::Release()
{
}
