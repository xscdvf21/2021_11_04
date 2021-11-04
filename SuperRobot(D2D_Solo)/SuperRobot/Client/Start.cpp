#include "stdafx.h"
#include "Start.h"
#include "Texture.h"
#include "Texture_Manager.h"
#include "Key_Manager.h"
#include "Scene_Manager.h"
#include "SoundMgr.h"


CStart::CStart()
{
}


CStart::~CStart()
{
}

HRESULT CStart::Ready_Scene()
{
	/*CSoundMgr::Get_Instance()->Initialize();*/
	CSoundMgr::Get_Instance()->PlayBGM(L"Start.mp3");
	return S_OK;
}

_int CStart::Update_Scene()
{

	Key_Check();

	
	return OBJ_NOEVENT;
}

void CStart::LateUpdate_Scene()
{
}

void CStart::Render_Scene()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"MenuSelect", L"StartMenu", iSceneNum);
	assert(!(nullptr == pTexInfo));
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY>> 1, 0.f);

	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CStart::Release_Scene()
{
}

void CStart::Key_Check()
{
	//스타트 씬에서 메뉴 선택 /////////////////////////////////////
	if (CKey_Manager::Get_Instance()->Key_Down(VK_LEFT))  
	{
		iSceneNum--;
		if (iSceneNum < 0)
		{
			iSceneNum = 2;
		}

	}
	else if (CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT))
	{
		iSceneNum++;

		if (iSceneNum > 2)
		{
			iSceneNum = 0;
		}
	}
	///////////////////////////////////////////////////////////



	//메뉴 선택 후 엔터 씬 전환 ///////////////////////////
	if (0 == iSceneNum && CKey_Manager::Get_Instance()->Key_Down(VK_RETURN))
	{
		CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGEBRFORE);
	}


	//////////////////////////////////////////////////////////

}
