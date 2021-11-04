#include "stdafx.h"
#include "ValveHandle.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "PlayerValue.h"
#include "SmallFan.h"


CValveHandle::CValveHandle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CValveHandle::~CValveHandle(void)
{
}

HRESULT CValveHandle::Ready_Object(void* pArg)
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

Engine::_int CValveHandle::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGERTYPE::TRIGGER_HANDLE);
		//m_pTrigger->Set_Rotation()
		m_bStart = false;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CValveHandle::LateUpdate_Object(const _float & fTimeDelta)
{
	m_pTransformCom->Set_AddAngleZ(-m_fAddAngle);
	m_pTransformCom->Update_Component(fTimeDelta);
	//Engine::Get_GameObject(L"GameObject", m_tObjInfo.tagMesh)->Is<CSmallFan>()->Set_AddAngle(-m_fAddAngle);

	if(m_bMove)
		Engine::Get_GameObject(L"GameObject", m_tObjInfo.tagMesh)->Is<CSmallFan>()->Set_Move(true, m_fDir);
	else
		Engine::Get_GameObject(L"GameObject", m_tObjInfo.tagMesh)->Is<CSmallFan>()->Set_Move(false, m_fDir);

	m_fAddAngle = 0.0f;

	//PxTransform cTrans = m_pRigidActor->getGlobalPose();
	//_vec3 vRot = m_pTransformCom->Get_Angle();

	//auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	//cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	//pBody->setKinematicTarget(cTrans);
	return _int();
}

void CValveHandle::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

void* CValveHandle::On_Active(void * pCaller)
{
	return nullptr;
}

void * CValveHandle::On_Attach(void * pCaller)
{
	return nullptr;
}

CValveHandle* CValveHandle::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CValveHandle*	pInstance = new CValveHandle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CValveHandle::Free(void)
{
	Engine::CGameObject::Free();
}

void CValveHandle::Set_DefaultSetting(wstring ObjTag, _vec3 vTargetPos, _vec3 vNormal)
{
	m_ObjTag = ObjTag;
	m_vTargetPos = vTargetPos;
	m_vNormal = vNormal;
}

HRESULT CValveHandle::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Valve_Handle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AddAngleX(D3DXToRadian(-90.0f));
	m_pTransformCom->Set_Mode(CTransform::MODE_AXIS);
	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CValveHandle::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);
	pEffect->CommitChanges();

	return S_OK;
}
