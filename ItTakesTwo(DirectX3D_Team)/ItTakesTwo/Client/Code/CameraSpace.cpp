#include "stdafx.h"
#include "CameraSpace.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"
#include "Logo.h"


CCameraSpace::CCameraSpace(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CCameraSpace::~CCameraSpace(void)
{
}

_bool CCameraSpace::Fade_Out(const _float& fTimeDelta)
{
	return false;
}

HRESULT CCameraSpace::Ready_Object(void* pArg)
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

Engine::_int CCameraSpace::Update_Object(const _float& fTimeDelta)
{

	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_vPos, m_vScale.x, m_vScale.y, m_vScale.z, TRIGGER_CAMERA_CHANGE);
		m_bStart = false;
	}
	
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CCameraSpace::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CCameraSpace::Render_Object(const _int& iIndex)
{

}

void* CCameraSpace::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	return nullptr;

}

void CCameraSpace::Set_Fix(_vec3 vEye, _vec3 vAt)
{
	m_eCameraMode = CAMERA_FIX;
	m_vEye = vEye;
	m_vAt = vAt;
}

CCameraSpace* CCameraSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev,  const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg)
{
	CCameraSpace*	pInstance = new CCameraSpace(pGraphicDev);

	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fX, fY, fZ);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CCameraSpace* CCameraSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev,  const _vec3& vPos, const _float& fHalf, void* pArg)
{
	CCameraSpace*	pInstance = new CCameraSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fHalf);
	return pInstance;
}

void CCameraSpace::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CCameraSpace::Add_Component(void* pArg)
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