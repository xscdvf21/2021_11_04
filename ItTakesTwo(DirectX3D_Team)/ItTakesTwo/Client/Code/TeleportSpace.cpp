#include "stdafx.h"
#include "TeleportSpace.h"
#include "Export_Function.h"


CTeleportSpace::CTeleportSpace(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTeleportSpace::~CTeleportSpace(void)
{
}

_bool CTeleportSpace::Fade_Out(const _float& fTimeDelta)
{
	if (m_pUI->Is_ColorEnd())
		return true;
	m_pUI->Update_Object(fTimeDelta);
	m_pUI->LateUpdate_Object(fTimeDelta);
	return false;
}

HRESULT CTeleportSpace::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;
	return S_OK;
}

Engine::_int CTeleportSpace::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_vPos, m_vScale.x, m_vScale.y, m_vScale.z, TRIGGER_TELEPORT);
		//m_pTrigger->Set_Interactable(false);
		m_bStart = false;
	}

	return OBJ_NOEVENT;
}

_int CTeleportSpace::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CTeleportSpace::Render_Object(const _int& iIndex)
{
}


void* CTeleportSpace::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	return nullptr;

}

void* CTeleportSpace::On_Active(void* pCaller)
{
	if (m_bActive)
		static_cast<CGameObject*>(pCaller)->Set_ReservePos(m_vTargetPos);
	Engine::StopNPlaySound(L"Teleport.ogg", CSoundMgr::CHANNELID::ChessQueen);
	return nullptr;
}

CTeleportSpace* CTeleportSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vTargetPos, const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg /*= nullptr*/)
{


	CTeleportSpace*	pInstance = new CTeleportSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(vPos);
	pInstance->Set_TargetPos(vTargetPos);
	pInstance->Set_Scale(fX, fY, fZ);
	return pInstance;
}

CTeleportSpace* CTeleportSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vTargetPos, const _vec3& vPos, const _float& fHalf, void* pArg /*= nullptr*/)
{
	CTeleportSpace*	pInstance = new CTeleportSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_TargetPos(vTargetPos);
	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fHalf);
	return pInstance;
}

void CTeleportSpace::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTeleportSpace::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;

	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	//Temp.eSceneID = RESOURCE_STATIC;
	//m_pUI = CImageUI::Create(m_pGraphicDev,&Temp,false);
	//NULL_CHECK_RETURN(m_pUI, E_FAIL);
	//m_pUI->Set_RenderID(RENDER_UI);
	//m_pUI->Set_Image(L"White_Bg");
	return S_OK;
}