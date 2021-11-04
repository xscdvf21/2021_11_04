#include "stdafx.h"
#include "StageEnd.h"
#include "ScrollMgr.h"
#include "Texture_Manager.h"
#include "GameObject.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"
#include "Scene_Manager.h"
#include "Background.h"
#include "StartBeforeKeyMgr.h"
#include "SoundMgr.h"

CStageEnd::CStageEnd()
	:m_fTurnDelay(0.f)
{
}


CStageEnd::~CStageEnd()
{
}

HRESULT CStageEnd::Ready_Scene()
{
	return S_OK;
}

_int CStageEnd::Update_Scene()
{
	Key_Check();

	if (iSceneNum == 5)
	{
		CGameObject* pObj = new CBackground;
		pObj->Ready_GameObject();
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_BACKGROUND, pObj);

		if (FAILED(CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_WEAPONSTORE)))
		{
			ERR_MSG(L"Create Failed SceneChange_SceneManager(SCENE_STAGE) - StageEnd.cpp");
			return E_FAIL;
		}
	}


	CGameObject_Manger::Get_Instance()->Update_GameObjectManager();

	return OBJ_NOEVENT;
}

void CStageEnd::LateUpdate_Scene()
{
}

void CStageEnd::Render_Scene()
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
		swprintf_s(szChar, L"박병건쌤 「너희 공부 열심히 했구나???」");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
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
		swprintf_s(szChar, L"신우재쌤 「내가 가르칠 때랑 많이 달라졌구나」");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		//TCHAR szChar2[64] = L"";
		//swprintf_s(szChar, L"이정도 였어??」");
		//D3DXMatrixTranslation(&matTrans, 290.f, 60.f, 0.f);
		//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		//CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
		//	szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
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
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", L"Monster00", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 25.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"박병건쌤 「우리가 졌다. 오늘 하루 쉬어라」");
		D3DXMatrixTranslation(&matTrans, 150.f, 30.f, 0.f);
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
		pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Whitebase", 2);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 25.f, 475.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 반장 「우리가 이겼다!! 오늘은 ");
		D3DXMatrixTranslation(&matTrans, 150.f, 475.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"쉬고 내일을 위해 정비 하자.」");
		D3DXMatrixTranslation(&matTrans, 310.f, 505.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CStageEnd::Release_Scene()
{
}

void CStageEnd::Key_Check()
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
