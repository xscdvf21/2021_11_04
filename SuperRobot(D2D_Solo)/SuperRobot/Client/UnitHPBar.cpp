#include "stdafx.h"
#include "UnitHPBar.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"

CUnitHPBar::CUnitHPBar()
{
}


CUnitHPBar::~CUnitHPBar()
{
}

HRESULT CUnitHPBar::Ready_GameObject()
{
	//m_pTextureKey = L"UnitHPBar";
	return S_OK;
}

int CUnitHPBar::Update_GameObject()
{
	return 0;
}

void CUnitHPBar::LateUpdate_GameObject()
{
}

void CUnitHPBar::Render_GameObject()
{
	//const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"UnitHPBar", m_pTextureKey, m_iSceneNum);

	//if (nullptr == pTexInfo)
	//{
	//	ERR_MSG(L"UnitFight.Cpp, UnitFight.png, 즉 pTexInfo가 오류.");
	//	return;
	//}

	//float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	//float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	//int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//_matrix matScale, matTrans, matWorld;
	//D3DXMatrixScaling(&matScale, 1.0f, 1.f, 0.f);
	//D3DXMatrixTranslation(&matTrans, 400, WINCY >> 1, 0.f);

	///*matWorld = matScale * matTrans;*/

	//CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	//CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(0.f, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CUnitHPBar::Release_GameObject()
{
}
