#include "stdafx.h"
#include "Animation_IMP.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"


CAnimation_IMP::CAnimation_IMP()
{
}


CAnimation_IMP::~CAnimation_IMP()
{
}

void CAnimation_IMP::Set_Frame(const wstring & wstrStateKey, const FRAME & tFrame)
{
	m_wstrStateKey = wstrStateKey;
	m_tFrame = tFrame;
}

HRESULT CAnimation_IMP::Ready_Effect_IMP()
{
	return S_OK;
}

int CAnimation_IMP::Update_Effect_IMP()
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * CTime_Manager::Get_Instance()->Get_TimeDelta();
	if (m_tFrame.fFrameStart >= m_tFrame.fFrameEnd)
	{
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}

void CAnimation_IMP::Render_Effect_IMP(const _matrix & matWorld)
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Effect", m_wstrStateKey, DWORD(m_tFrame.fFrameStart));
	if (!pTexInfo)
		return;
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}
