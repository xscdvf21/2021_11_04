#include "stdafx.h"
#include "TriggerView.h"
#include "BoundingBox.h"

CTriggerView::CTriggerView(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}


CTriggerView::~CTriggerView()
{
}

HRESULT CTriggerView::Ready_Object(TRIGGERTYPE eType, void * pArg)
{
	m_eTriggerType = eType;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = tTemp;

	Engine::CComponent*		pComponent = nullptr;

	return S_OK;
}

Engine::_int CTriggerView::Update_Object(const _float& fTimeDelta)
{

	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, m_eTriggerType);
		m_pTrigger->Set_Interactable(m_bTriggerActive);
		m_pTrigger->Set_Rotation(m_tObjInfo.vTriggerAngle);
		m_bStart = false;
	}

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

Engine::_int CTriggerView::LateUpdate_Object(const _float & fTimeDelta)
{
	return 0;
}

void CTriggerView::Render_Object(const _int& iIndex /*= 0*/)
{
}

void * CTriggerView::On_Active(void * pCaller)
{
	_bool bActive = *(_bool*)pCaller;
	m_pTrigger->Set_Interactable(bActive);
	return nullptr;
}

CGameObject * CTriggerView::Create(LPDIRECT3DDEVICE9 pGraphicDev ,TRIGGERTYPE eType, void * pArg, _vec3 vCodyPos, _vec3 vMayPos, _vec3 vMayUp)
{
	CTriggerView* pInstance = new CTriggerView(pGraphicDev);
	if (eType == TRIGGER_PLATFORMRESPWAN)
	{
		pInstance->m_vCodyRespwanPoint = vCodyPos;
		pInstance->m_vMayRespwanPoint = vMayPos;
		pInstance->m_vMayRespwanUp = vMayUp;
	}

	if (FAILED(pInstance->Ready_Object(eType, pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTriggerView::Free()
{
	Engine::CGameObject::Free();
}
