#include "stdafx.h"
#include "Stage2.h"

#include "GameObject_Manger.h"
#include "Tile.h"
#include "MousePointer.h"
#include "ScrollMgr.h"
#include "Texture_Manager.h"

#include "GunDamMK.h"
#include "MazingerZ.h"


#include "GameObject.h"
#include "Monster1.h"
#include "Monster2.h"

#include "UnitMenu.h"
#include "UnitInfo.h"
#include "UnitAttack.h"
#include "UnitWeapon.h"
#include "UnitFight.h"

#include "Background.h"

#include "Astar_manager.h"
#include "Time_Manager.h"
#include "SoundMgr.h"
#include "Scene_Manager.h"
#include "Key_Manager.h"

CStage2::CStage2()
	:m_fTurnDelay(0.f)
{
}


CStage2::~CStage2()
{
}

HRESULT CStage2::Ready_Scene()
{
	float fX = 40.f;
	float fY = 40.f;
	_vec3 vecPos = { fX, fY, 0.f };


	m_pTile = new CTile;
	m_pTile->Ready_GameObject();

	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	{

		GROUND*		m_pTemp = m_pTile->Get_Index(vecPos);
		vecPos = m_pTemp->vPos;
		_rList->Set_UnitMaxHP(_rList->Get_Info());
		_rList->Set_UnitEnergy(_rList->Get_Info());
		_rList->Set_Pos(vecPos);

		fX += 40.f;
		fY += 40.f;
		vecPos = { fX, fY, 0.f };
	}


	bool bUnitCheck = false;  //유닛의 턴이 끝났는지,
	bool bMonsterCheck = false; //몬스터의 턴이 끝났는지,


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
			_rList->Set_monster_iAllCheck(0);
			CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
		}
		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT).front()->Get_Info().vPos);
		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(0);

	return S_OK;
}

_int CStage2::Update_Scene()
{
	Unit_Turn();

	CGameObject_Manger::Get_Instance()->Update_GameObjectManager();
	return OBJ_NOEVENT;
}

void CStage2::LateUpdate_Scene()
{
	CGameObject_Manger::Get_Instance()->Late_Update_GameObjectManager();
}

void CStage2::Render_Scene()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"Stage", 1);
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
}

void CStage2::Release_Scene()
{
}

void CStage2::Unit_Turn()
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
				_rList->Set_monster_iAllCheck(0);
				CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(false);
			}
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
