#include "stdafx.h"
#include "StageStart.h"
#include "ScrollMgr.h"
#include "Texture_Manager.h"
#include "GameObject.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"
#include "Scene_Manager.h"
#include "Background.h"

#include "Tile.h"
#include "MousePointer.h"
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

#include "StartBeforeKeyMgr.h"
#include "SoundMgr.h"

CStageStart::CStageStart()
	:m_fTurnDelay(0.f)
{
}


CStageStart::~CStageStart()
{
}

HRESULT CStageStart::Ready_Scene()
{
	CGameObject* pObj = new CMousePointer;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_CURSOR, pObj);

	pObj = new CTile;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_GROUND, pObj);


	//유닛
	pObj = new CGunDamMK;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	pObj = new CMazingerZ;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	pObj = new CGetta;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	pObj = new CAphrodite;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);
	pObj = new CWhitebase;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UNIT, pObj);


	//몬스터
	pObj = new CMonster1;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster2;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster3;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster4;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster05;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);


	pObj = new CUnitMenu;  	//static_iAllCheck = 1  처음 유닛 선택하고, 유닛이동, 정신, 능력 볼 수 있는 메뉴
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	pObj = new CUnitSkill;  	//static_iAllCheck = 3  유닛 정신
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	pObj = new CUnitInfo;	//static_iAllCheck = 4	유닛 능력치 창
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	pObj = new CUnitAttack;	//static_iAllCheck = 5	유닛 이동 후 타일 검사해서, 몬스터가 있으면 공격, 대기 메뉴창 
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	pObj = new CUnitWeapon;//static_iAllCheck = 6  유닛이 공격 누르고 커서가 몬스터한테 간후 A키 누르면 무기고 창
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);

	return S_OK;
}

_int CStageStart::Update_Scene()
{

	Key_Check();
	//병건쌤
	if (iSceneNum == 1)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster00"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	} 

	//반장
	if (iSceneNum == 2)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (!lstrcmp(_rList->Get_TextureUnitKey(), L"Whitebase"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_Info().vPos);
				break;
			}
		}
		
	} 

	//우재쌤
	if (iSceneNum == 3)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster01"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	} 

	//부반장
	if (iSceneNum == 4)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (!lstrcmp(_rList->Get_TextureUnitKey(), L"Aphrodite"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_Info().vPos);
				break;
			}
		}

	}

	//병건쌤
	if (iSceneNum == 5)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster00"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	} 

	//반장
	if (iSceneNum == 6)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (!lstrcmp(_rList->Get_TextureUnitKey(), L"Whitebase"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_Info().vPos);
				break;
			}
		}

	}

	//병건쌤
	if (iSceneNum == 7)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster00"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	}

	if (iSceneNum == 8)// 원래 8임
	{
		CGameObject* pObj = new CBackground;
		pObj->Ready_GameObject();
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_BACKGROUND, pObj);

		if (FAILED(CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGE)))
		{
			ERR_MSG(L"Create Failed SceneChange_SceneManager(SCENE_STAGE) - MainApp.cpp");
			return E_FAIL;
		}
	}
	CGameObject_Manger::Get_Instance()->Update_GameObjectManager();
	
	return OBJ_NOEVENT;
}

void CStageStart::LateUpdate_Scene()
{
}

void CStageStart::Render_Scene()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"Stage", 0);
	const TEXINFO*	pTexInfo1 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitFightReady", 1); //유닛 대사 할곳
	const TEXINFO*	pTexInfo2; //각각 담을 작은 사진
	assert(!(nullptr == pTexInfo));
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f + iScrollX, 0.f + iScrollY, 0.f);

	matWorld = matScale * matTrans;


	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//if (2 == CGameObject::static_iAllCheck)
	//	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 50, 50, 50));

	CGameObject_Manger::Get_Instance()->Render_GameObjectManager();

	//대사 씬
	if (iSceneNum == 1)
	{
		//몬스터 대사 먼저 시작.
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", L"Monster00", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"박병건쌤 「너희 오늘 9시까지 안남았더라?");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"그게 최선이야?」");
		D3DXMatrixTranslation(&matTrans, 290.f, 60.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 2)
	{
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 450.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Whitebase", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f , 475.f , 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 반장 「네. 112기 모두 휴식을 원합니다");
		D3DXMatrixTranslation(&matTrans, 150.f, 475.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"오늘 하루 쉬고 싶습니다.」");
		D3DXMatrixTranslation(&matTrans, 310.f, 505.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 3)
	{
		//몬스터 대사 먼저 시작.
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", L"Monster01", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"신우재쌤 「112기 인성은 알고 있었지만...");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"이정도 였어??」");
		D3DXMatrixTranslation(&matTrans, 290.f, 60.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 4)
	{
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 450.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Aphrodite", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 475.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 부반장 「오늘 하루만 쉬겠다는 거에요");
		D3DXMatrixTranslation(&matTrans, 150.f, 475.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"오늘 하루도 안돼요? 시연회 오늘");
		D3DXMatrixTranslation(&matTrans, 350.f, 505.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar3[64] = L"";
		swprintf_s(szChar, L"끝났는데 ?」");
		D3DXMatrixTranslation(&matTrans, 350.f, 535.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 5)
	{
		//몬스터 대사 먼저 시작.
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", L"Monster00", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"박병건쌤 「우리들은 오늘 하루도 용납");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"할 수 없다. 오늘도 9시 까지");
		D3DXMatrixTranslation(&matTrans, 290.f, 60.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar3[64] = L"";
		swprintf_s(szChar, L"남아 알았어?!」");
		D3DXMatrixTranslation(&matTrans, 290.f, 90.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 6)
	{
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 450.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Whitebase", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 475.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 반장 「쌤들의 뜻이 그렇다면... 저희도");
		D3DXMatrixTranslation(&matTrans, 150.f, 475.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"어쩔 수 없습니다. 」");
		D3DXMatrixTranslation(&matTrans, 310.f, 505.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 7)
	{
		//몬스터 대사 먼저 시작.
		//대사 를 쓸곳이므로, 제일 먼저 출력
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo1->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//작은 사진
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", L"Monster00", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"박병건쌤 「전쟁이다! 우리들의 힘을 보여줍시다」");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CStageStart::Release_Scene()
{
}

void CStageStart::Key_Check()
{

	m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();
	if (m_fTurnDelay >= 3.0f)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);

		iSceneNum++;
		m_fTurnDelay = 0.f;
	}
}
