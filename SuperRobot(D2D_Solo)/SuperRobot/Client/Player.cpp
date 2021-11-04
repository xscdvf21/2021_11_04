#include "stdafx.h"
#include "Player.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"


CPlayer::CPlayer()
{
	ZeroMemory(&m_tFrame, sizeof(FRAME)); 
}


CPlayer::~CPlayer()
{
}

void CPlayer::FrameMove(float fSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd * CTime_Manager::Get_Instance()->Get_TimeDelta() * fSpeed;
	if (m_tFrame.fFrameStart >= m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f; 

	}

}

HRESULT CPlayer::Ready_GameObject()
{
	m_tInfo.vPos = _vec3(float(WINCX >> 1), float(WINCY >> 1), 0.f);
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tFrame = {0.f, 5.f};
	return S_OK; 
}
int CPlayer::Update_GameObject()
{
	return 0; 
}
void CPlayer::LateUpdate_GameObject()
{
	FrameMove(0.1f); 
	// 6 * 
}
void CPlayer::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Player", L"Attack", DWORD(m_tFrame.fFrameStart));
	if (nullptr == pTexInfo)
		return; 

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1); 
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1); // ¾ÆÁÖ ±ñ°£ÇØ. 

	_matrix matScale, matTrans, matWorld ; 
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x , m_tInfo.vPos.y, 0.f); 

	matWorld = matScale * matTrans; 

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld); 
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


}
void CPlayer::Release_GameObject()
{

}



