#include "stdafx.h"
#include "NormalEffect.h"
#include "Effect_IMP.h"
#include "Scroll_Manager.h"
#include "ScrollMgr.h"


CNormalEffect::CNormalEffect(CEffect_IMP* pMotion)
	:CEffect(pMotion)
{
}


CNormalEffect::~CNormalEffect()
{
	Release_GameObject();
}

HRESULT CNormalEffect::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	return S_OK;
}

int CNormalEffect::Update_GameObject()
{
	return m_pBridge->Update_Effect_IMP();
}

void CNormalEffect::LateUpdate_GameObject()
{
}

void CNormalEffect::Render_GameObject()
{

	
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bDead)
	{
		iScrollX = 0;
		iScrollY = 0;
	}
	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);

	matWorld = matScale * matTrans;
	m_pBridge->Render_Effect_IMP(matWorld);

}

void CNormalEffect::Release_GameObject()
{
}
