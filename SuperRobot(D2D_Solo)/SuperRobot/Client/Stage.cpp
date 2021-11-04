#include "stdafx.h"
#include "Stage.h"
#include "GameObject_Manger.h"
#include "Tile.h"
#include "MousePointer.h"
#include "ScrollMgr.h"
#include "Texture_Manager.h"

#include "GunDamMK.h"
#include "MazingerZ.h"
#include "Whitebase.h"
#include "Aphrodite.h"
#include "Getta.h"

#include "GameObject.h"
#include "Monster1.h"
#include "Monster2.h"
#include "Monster3.h"
#include "Monster4.h"
#include "Monster05.h"

#include "UnitMenu.h"
#include "UnitInfo.h"
#include "UnitAttack.h"
#include "UnitWeapon.h"
#include "UnitFight.h"
#include "UnitGold.h"
#include "UnitSkill.h"

#include "Background.h"

#include "Astar_manager.h"
#include "Time_Manager.h"
#include "SoundMgr.h"
#include "Scene_Manager.h"
#include "Key_Manager.h"

CStage::CStage()
	:m_fTurnDelay(0.f)
	, m_bCutScene(false)
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"Stage1.mp3");




	CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(CGameObject_Manger::Get_Instance()->Get_Player()->Get_Info().vPos);

	//CGameObject* pObj = new CMousePointer;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_CURSOR, pObj);

	//pObj = new CTile;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_GROUND, pObj);


	////유닛
	//pObj = new CGunDamMK;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	//pObj = new CMazingerZ;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	//pObj = new CGetta;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	//pObj = new CAphrodite;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	//pObj = new CWhitebase;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);


	////몬스터
	//pObj = new CMonster1;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	//pObj = new CMonster2;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	//pObj = new CMonster3;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	//pObj = new CMonster4;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	//pObj = new CMonster05;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);


	//UI
	//CGameObject* pObj = new CUnitMenu;  	//static_iAllCheck = 1  처음 유닛 선택하고, 유닛이동, 정신, 능력 볼 수 있는 메뉴
	//pObj->Ready_GameObject(); 
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	//pObj = new CUnitSkill;  	//static_iAllCheck = 3  유닛 정신
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	//pObj = new CUnitInfo;	//static_iAllCheck = 4	유닛 능력치 창
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	//pObj = new CUnitAttack;	//static_iAllCheck = 5	유닛 이동 후 타일 검사해서, 몬스터가 있으면 공격, 대기 메뉴창 
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	//pObj = new CUnitWeapon;//static_iAllCheck = 6  유닛이 공격 누르고 커서가 몬스터한테 간후 A키 누르면 무기고 창
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);



	//pObj = new CUnitFight;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);

	//pObj = new CUnitFightReady;
	//pObj->Ready_GameObject();
	//CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj); 
	return S_OK;
}

_int CStage::Update_Scene()
{
	if (1 == CGameObject_Manger::Get_Instance()->Get_list2(CGameObject_Manger::OBJ_MONSTER).size() && exStagePlay == 0)
	{
		CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGEPLAYING);
	}
		
	if (0 == CGameObject_Manger::Get_Instance()->Get_list2(CGameObject_Manger::OBJ_MONSTER).size())
	{
		CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGEEND);
	}


	if (CKey_Manager::Get_Instance()->Key_Down('P'))
	{
		CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_WEAPONSTORE);
	}

	if (CKey_Manager::Get_Instance()->Key_Down('D'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);

		CGameObject*	pObj = new CUnitGold;
		pObj->Ready_GameObject();
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	}

		Unit_Turn();
		CGameObject_Manger::Get_Instance()->Update_GameObjectManager();

	return OBJ_NOEVENT;
}

void CStage::LateUpdate_Scene()
{
	CGameObject_Manger::Get_Instance()->Late_Update_GameObjectManager();
}

void CStage::Render_Scene()
{

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"Stage", 0);
	assert(!(nullptr == pTexInfo));
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f +iScrollX , 0.f + iScrollY, 0.f);

	matWorld = matScale * matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	//if (2 == CGameObject::static_iAllCheck)
	//	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 50, 50, 50));

	CGameObject_Manger::Get_Instance()->Render_GameObjectManager();


}

void CStage::Release_Scene()
{
}

void CStage::Unit_Turn()
{
	bool bUnitCheck = false;  //유닛의 턴이 끝났는지,
	bool bMonsterCheck = false; //몬스터의 턴이 끝났는지,

	//유닛 턴 체크. 
	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	{
		if (!_rList->Get_Info().bTurn)//false 가 풀어주는것, ture가 턴을 쓴것.
		{
			bUnitCheck = true; //여길 들어
			break;
		}

	}
	if (bUnitCheck)
		return;

	CGameObject_Manger::Get_Instance()->Get_Monster()->Set_MonsterTurnCheck(true);

	//몬스터 턴 체크.
	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
	{
		if (!_rList->Get_MonsterInfo().bTurn)
		{
			bMonsterCheck = true; //false 면 유닛에게 넘겨서, 모든 애들 턴 풀어줌.
			break;
		}

	}
	if (bMonsterCheck)
		return;
		

	if (!bUnitCheck && !bMonsterCheck)
	{
			if (m_fTurnDelay <= 0.f)
			{
				CGameObject* pObj = new CBackground;
				pObj->Ready_GameObject();
				CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_BACKGROUND, pObj);
			
			}
			m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();
			
			if (m_fTurnDelay >= 3.f)
			{
				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
				{

					_rList->Set_UnitCheck(bUnitCheck);
					_rList->Set_iSceneNum(0);
					_rList->Set_static_iAllCheck(0);
				}

				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				{
					_rList->Set_MonsterCheck(bUnitCheck);
					_rList->Set_static_iAllCheck(0);
					_rList->Set_MonsterTurnCheck(false);
					CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
				}

				/*CGameObject_Manger::Get_Instance()->Get_Monster()->Set_MonsterTurnCheck(true);*/
				

				//if (!lstrcmp(L"Monster00", CGameObject_Manger::Get_Instance()->Get_Monster()->Get_TextureMonsterKey()))
				//{
				//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				//	{
				//		_rList->Set_MonsterCheck(bUnitCheck);
				//		_rList->Set_static_iAllCheck(0);
				//		_rList->Set_monster_iAllCheck(0);
				//		CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
				//	}
				//}
				//if (!lstrcmp(L"Monster01", CGameObject_Manger::Get_Instance()->Get_Monster()->Get_TextureMonsterKey()))
				//{
				//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				//	{
				//		_rList->Set_MonsterCheck(bUnitCheck);
				//		_rList->Set_static_iAllCheck(0);
				//		_rList->Set_monster_iAllCheck(1);
				//		CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
				//	}
				//}
				//if (!lstrcmp(L"Monster02", CGameObject_Manger::Get_Instance()->Get_Monster()->Get_TextureMonsterKey()))
				//{
				//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				//	{
				//		_rList->Set_MonsterCheck(bUnitCheck);
				//		_rList->Set_static_iAllCheck(0);
				//		_rList->Set_monster_iAllCheck(2);
				//		CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
				//	}
				//}
				//if (!lstrcmp(L"Monster03", CGameObject_Manger::Get_Instance()->Get_Monster()->Get_TextureMonsterKey()))
				//{
				//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				//	{
				//		_rList->Set_MonsterCheck(bUnitCheck);
				//		_rList->Set_static_iAllCheck(0);
				//		_rList->Set_monster_iAllCheck(3);
				//		CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
				//	}
				//}
				//if (!lstrcmp(L"Monster04", CGameObject_Manger::Get_Instance()->Get_Monster()->Get_TextureMonsterKey()))
				//{
				//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				//	{
				//		_rList->Set_MonsterCheck(bUnitCheck);
				//		_rList->Set_static_iAllCheck(0);
				//		_rList->Set_monster_iAllCheck(4);
				//		CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
				//	}
				//}
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT).front()->Get_Info().vPos);
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);
				m_fTurnDelay = 0.f;
			}
			
	}
	//if (bUnitCheck) // 한명이라도 유닛이 턴 있으면, 이함수를 끝냄.
	//	return;

	//if (!bUnitCheck)
	//{
	//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
	//	{
	//		_rList->Set_MonsterCheck(bUnitCheck);
	//	}
	//}

}
