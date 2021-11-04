#include "stdafx.h"
#include "StageBefore.h"
#include "Texture.h"
#include "Texture_Manager.h"
#include "StartBeforeKeyMgr.h"
#include "Scene_Manager.h"
#include "SoundMgr.h"
#include "GameObject.h"
#include "Background.h"
#include "GameObject_Manger.h"
#include "Time_Manager.h"

CStageBefore::CStageBefore()
{
}


CStageBefore::~CStageBefore()
{
}

HRESULT CStageBefore::Ready_Scene()
{
	m_fTurnDelay = 0.f;
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"StageBefore.mp3");

	return S_OK;
}

_int CStageBefore::Update_Scene()
{
	if (iSceneNum == 5)
	{

			CGameObject* pObj = new CBackground;
			pObj->Ready_GameObject();
			CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_BACKGROUND, pObj);

			if (FAILED(CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGESTART)))
			{
				ERR_MSG(L"Create Failed SceneChange_SceneManager(SCENE_STAGE) - MainApp.cpp");
				return E_FAIL;
			}
		
	}


	Key_Check();
	return _int();
}

void CStageBefore::LateUpdate_Scene()
{
}

void CStageBefore::Render_Scene()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Map", L"StageBefore", 0);

	assert(!(nullptr == pTexInfo));
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);

	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (iSceneNum == 0 )
	{

		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Whitebase", 3);

		assert(!(nullptr == pTexInfo));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 77.f, 333.f, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 반장 「코딩과의 전쟁을 시작한다. 112기 모두");
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"준비 되었는가 ?」");
		D3DXMatrixTranslation(&matTrans, 200.f, 480.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 1)
	{

		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Aphrodite", 3);

		assert(!(nullptr == pTexInfo));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 77.f, 333.f, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 부반장 「이번 전투에 승리하고 112기 모두");
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"휴식을 차지하자!」");
		D3DXMatrixTranslation(&matTrans, 240.f, 480.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 2)
	{

		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"GunDamMK", 3);

		assert(!(nullptr == pTexInfo));

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 77.f, 333.f, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 일원 「전투에서 이기면 모두들 편하게 휴식 ");
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar2[64] = L"";
		swprintf_s(szChar, L"할 수 있는거야 ?」");
		D3DXMatrixTranslation(&matTrans, 200.f, 480.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (iSceneNum == 3)
	{

		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"MazingerZ", 3);

		assert(!(nullptr == pTexInfo));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 77.f, 333.f, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 일원 「휴식?! 가자 싸우자 이기자 ");
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	if (iSceneNum == 4)
	{

		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Whitebase", 3);

		assert(!(nullptr == pTexInfo));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 77.f, 333.f, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 반장 「모두 힘차게 가보자! 출동!");
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


	}

	if (iSceneNum == 5)
	{

		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"Whitebase", 3);

		assert(!(nullptr == pTexInfo));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, 77.f, 333.f, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szChar[64] = L"";
		swprintf_s(szChar, L"112기 반장 「모두 힘차게 가보자! 출동!");
		D3DXMatrixTranslation(&matTrans, 30.f, 430.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szChar, lstrlen(szChar), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CStageBefore::Release_Scene()
{
}

void CStageBefore::Key_Check()
{
	if (CStartBeforeKeyMgr::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);

		iSceneNum++;
	}
}
