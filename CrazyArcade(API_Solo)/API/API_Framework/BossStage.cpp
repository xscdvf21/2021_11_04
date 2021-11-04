#include "stdafx.h"
#include "BossStage.h"
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

CBossStage::CBossStage()
{
}


CBossStage::~CBossStage()
{
}

void CBossStage::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/TownGround.bmp", L"Tile");
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->PlayBGM(L"Boss.mp3");

	CTileMgr::Get_Instance()->Load_Tile3();
	CBlockMgr::Get_Instance()->Load_RedBlock3();
	CYellowBlockMgr::Get_Instance()->Load_YellowBlock3();
	CSpeedItemMgr::Get_Instance()->Load_SpeedItem3();
	CPadoItemMgr::Get_Instance()->Load_PadoItem3();
	CBoomCountMgr::Get_Instance()->Load_BoomCountItem3();
	CTownBlockMgr::Get_Instance()->Load_Town_Block3();
	CRedHouseMgr::Get_Instance()->Load_Red_House3();
	CBlueHouseMgr::Get_Instance()->Load_Blue_House3();
	CYellowHouseMgr::Get_Instance()->Load_Yellow_House3();
	CTreeMgr::Get_Instance()->Load_Tree3();
	CBushMgr::Get_Instance()->Load_Bush3();

	CObj*	pObj = CAbstractFactory<CPlayer>::Create();									//캐릭터 생성
	CObj*	pObjMoster = CAbstractFactory<CMonster>::Create();							//몬스터 생성

	CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::PLAYER);
	CObjMgr::Get_Instance()->Add_Object(pObjMoster, OBJID::MONSTER);
}

void CBossStage::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CBossStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CBossStage::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC);
}

void CBossStage::Release()
{
}
