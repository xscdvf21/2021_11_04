#include "stdafx.h"
#include "UnitAttack.h"
#include "Texture_Manager.h"
#include "ScrollMgr.h"
#include "GameObject_Manger.h"


CUnitAttack::CUnitAttack()
{
}


CUnitAttack::~CUnitAttack()
{
}

HRESULT CUnitAttack::Ready_GameObject()
{
	m_pTextureKey = L"UnitAttack";
	return S_OK;
}

int CUnitAttack::Update_GameObject()
{
	return 0;
}

void CUnitAttack::LateUpdate_GameObject()
{
}

void CUnitAttack::Render_GameObject()
{
	if (static_iAllCheck == 5)
	{
		const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", m_pTextureKey, m_iSceneNum);
		if (nullptr == pTexInfo)
		{
			ERR_MSG(L"UnitAttack.Cpp, pTexInfo°¡ ¿À·ù.");
			return;
		}

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		_matrix matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);

		matWorld = matScale * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CUnitAttack::Release_GameObject()
{
}
