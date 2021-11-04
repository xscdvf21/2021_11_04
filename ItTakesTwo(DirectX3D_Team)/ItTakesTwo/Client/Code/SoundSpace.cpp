#include "stdafx.h"
#include "SoundSpace.h"
#include "Export_Function.h"


CSoundSpace::CSoundSpace(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSoundSpace::~CSoundSpace(void)
{
}

_bool CSoundSpace::Fade_Out(const _float& fTimeDelta)
{
	if (m_pUI->Is_ColorEnd())
		return true;
	m_pUI->Update_Object(fTimeDelta);
	m_pUI->LateUpdate_Object(fTimeDelta);
	return false;
}

HRESULT CSoundSpace::Ready_Object(void* pArg)
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

Engine::_int CSoundSpace::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_vPos, m_vScale.x, m_vScale.y, m_vScale.z, TRIGGER_SOUND);
		//m_pTrigger->Set_Interactable(false);
		m_bStart = false;
	}

	return OBJ_NOEVENT;
}

_int CSoundSpace::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CSoundSpace::Render_Object(const _int& iIndex)
{
}


void* CSoundSpace::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	return nullptr;

}

void* CSoundSpace::On_Active(void* pCaller)
{
	if (m_bActive)
		static_cast<CGameObject*>(pCaller)->Set_ReservePos(m_vTargetPos);
	//else if ()
	//{

	//}
	return nullptr;
}

CSoundSpace* CSoundSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg /*= nullptr*/)
{


	CSoundSpace*	pInstance = new CSoundSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fX, fY, fZ);
	return pInstance;
}

CSoundSpace* CSoundSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _float& fHalf, void* pArg /*= nullptr*/)
{
	CSoundSpace*	pInstance = new CSoundSpace(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_Pos(vPos);
	pInstance->Set_Scale(fHalf);
	return pInstance;
}

void CSoundSpace::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSoundSpace::Add_Component(void* pArg)
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