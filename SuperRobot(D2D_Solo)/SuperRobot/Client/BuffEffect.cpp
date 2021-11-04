#include "stdafx.h"
#include "BuffEffect.h"
#include "Player.h"
#include "Effect_IMP.h"
#include "GameObject_Manger.h"
#include "Scroll_Manager.h"
#include "GameObject.h"

CBuffEffect::CBuffEffect(CEffect_IMP* pMotion)
	:CEffect(pMotion)
{
}


CBuffEffect::~CBuffEffect()
{
}

HRESULT CBuffEffect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vPos = { 100.f, 0.f, 0.f };
	m_fSpeed = 5.f;
	return S_OK;
}

int CBuffEffect::Update_GameObject()
{
	m_fAngle += m_fSpeed;

	return m_pBridge->Update_Effect_IMP();
}

void CBuffEffect::LateUpdate_GameObject()
{
}

void CBuffEffect::Render_GameObject()
{
	_matrix matScale, matRotZ, matTrans, matRevRotZ, matParent, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLL_X), m_tInfo.vPos.y + CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLL_Y), 0.f);
	D3DXMatrixRotationZ(&matRevRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	const INFO pParentPos = CGameObject_Manger::Get_Instance()->Get_Player()->Get_Info();
	D3DXMatrixTranslation(&matParent, pParentPos.vPos.x, pParentPos.vPos.y, 0.f);

	matWorld = matScale*matRotZ * matTrans * matRevRotZ * matParent;

	m_pBridge->Render_Effect_IMP(matWorld);
}

void CBuffEffect::Release_GameObject()
{
}
