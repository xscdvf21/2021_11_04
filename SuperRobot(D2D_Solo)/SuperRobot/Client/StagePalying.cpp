#include "stdafx.h"
#include "StagePalying.h"
#include "ScrollMgr.h"
#include "Texture_Manager.h"
#include "GameObject.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"
#include "Scene_Manager.h"
#include "Background.h"

#include "Monster6.h"
#include "Monster7.h"
#include "Monster8.h"
#include "Monster9.h"
#include "Monster10.h"

#include "StartBeforeKeyMgr.h"
#include "SoundMgr.h"

CStagePalying::CStagePalying()
	:m_fTurnDelay(0.f)
{
}


CStagePalying::~CStagePalying()
{
}

HRESULT CStagePalying::Ready_Scene()
{
	CGameObject* pObj = new CMonster6;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster7;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster8;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster9;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);
	pObj = new CMonster10;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_MONSTER, pObj);

	return S_OK;
}

_int CStagePalying::Update_Scene()
{

	Key_Check();

	if (iSceneNum == 1)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster05"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	}

	if (iSceneNum == 2)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster07"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	}

	//반장
	if (iSceneNum == 3)
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


	if (iSceneNum == 5)
	{
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (!lstrcmp(_rList->Get_TextureMonsterKey(), L"Monster05"))
			{
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(_rList->Get_MonsterInfo().vPos);
				break;
			}
		}

	}

	if (iSceneNum == 6)
	{
		exStagePlay++;
		if (FAILED(CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGE)))
		{
			ERR_MSG(L"Create Failed SceneChange_SceneManager(SCENE_STAGE) - MainApp.cpp");
			return E_FAIL;
		}
	}
	

	CGameObject_Manger::Get_Instance()->Update_GameObjectManager();
	return OBJ_NOEVENT;


}

void CStagePalying::LateUpdate_Scene()
{
}

void CStagePalying::Render_Scene()
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
		swprintf_s(szChar, L"박병건쌤 「이게 끝인줄 알았지 ??");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"아직 한참남았어 ^^」");
		D3DXMatrixTranslation(&matTrans, 290.f, 60.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 2)
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
		swprintf_s(szChar, L"신우재쌤 「아직 부족해 애들아 ^^");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"더 열심히 해」");
		D3DXMatrixTranslation(&matTrans, 290.f, 60.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 3)
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
		swprintf_s(szChar, L"112기 반장 「이렇게까지 하셔야 합니까?");
		D3DXMatrixTranslation(&matTrans, 150.f, 475.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"우리를 왜 괴롭히세요」");
		D3DXMatrixTranslation(&matTrans, 310.f, 505.f, 0.f);
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
		swprintf_s(szChar, L"112기 부반장 「비겁해요... 쌤...");
		D3DXMatrixTranslation(&matTrans, 150.f, 475.f, 0.f);
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
		swprintf_s(szChar, L"박병건쌤 「더 열심히 하란말이야! ");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
}

void CStagePalying::Release_Scene()
{
}

void CStagePalying::Key_Check()
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
