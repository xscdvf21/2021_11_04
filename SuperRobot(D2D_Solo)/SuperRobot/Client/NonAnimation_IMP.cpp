#include "stdafx.h"
#include "NonAnimation_IMP.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"


CNonAnimation_IMP::CNonAnimation_IMP()
{
}


CNonAnimation_IMP::~CNonAnimation_IMP()
{
}

void CNonAnimation_IMP::Set_StateKey(const wstring & wstrStateKey, float fLifeTime)
{
	m_wstrStateKey = wstrStateKey;
	m_fLifeTime = fLifeTime;
}

HRESULT CNonAnimation_IMP::Ready_Effect_IMP()
{
	m_fCountTime = 0.f;
	return S_OK;
}

int CNonAnimation_IMP::Update_Effect_IMP()
{
	m_fCountTime += CTime_Manager::Get_Instance()->Get_TimeDelta();
	if (m_fCountTime >= m_fLifeTime)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CNonAnimation_IMP::Render_Effect_IMP(const _matrix & matWorld)
{

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Effect", m_wstrStateKey, 0);
	if (!pTexInfo)
		return;
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
