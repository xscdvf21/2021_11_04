#include "stdafx.h"
#include "SaveSpace.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "..\Engine\Resources\Code\PhysXMesh.h"


CSaveSpace::CSaveSpace(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_bTex(false)
{

}

CSaveSpace::~CSaveSpace(void)
{
}

HRESULT CSaveSpace::Ready_Object(void* pArg)
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

HRESULT CSaveSpace::Ready_Object(_vec3 vSavePoint, _tchar* tTexName, _bool bTex /*= false*/, void* pArg /*= nullptr*/)
{

	m_bTex = bTex;
	m_tTexName = tTexName;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;
#ifdef _DEBUG
	//if (m_bTex)
	//{
		m_pTransformCom->Set_Scale(m_tObjInfo.vScale);
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(m_tObjInfo.vAngle.x));
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(m_tObjInfo.vAngle.y));
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(m_tObjInfo.vAngle.z));
		m_pTransformCom->Set_Pos(m_tObjInfo.vPos);
	//}
#endif // _DEBUG
	m_vSavePoint = vSavePoint;
	return S_OK;
}

Engine::_int CSaveSpace::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_bTex = g_bRenderBox;
	if (m_bStart)
	{
		m_pTrigger=  CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vPos, m_tObjInfo.vScale.x, m_tObjInfo.vScale.y, m_tObjInfo.vScale.z,TRIGGERTYPE::TRIGGER_SAVESPACE);
		m_bStart = false;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CSaveSpace::LateUpdate_Object(const _float & fTimeDelta)
{
	
	return _int();
}

void CSaveSpace::Render_Object(const _int& iIndex)
{
}





CSaveSpace* CSaveSpace::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vSavePoint, _tchar* tTexName	/*= L""*/, _bool bTex /*= false*/, void* pArg /*= nullptr*/)
{
	CSaveSpace*	pInstance = new CSaveSpace(pGraphicDev);
	
	if (FAILED(pInstance->Ready_Object(vSavePoint, tTexName,bTex,pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSaveSpace::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSaveSpace::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO Temp;

	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	
#ifdef _DEBUG

		// Transform
		pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
#endif
	return S_OK;
}

HRESULT CSaveSpace::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}
