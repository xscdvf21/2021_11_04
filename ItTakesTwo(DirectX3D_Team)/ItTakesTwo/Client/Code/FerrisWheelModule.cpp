#include "stdafx.h"
#include "FerrisWheelModule.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "BoundingBox.h"
#include "SmallFan.h"


CFerriswheelModule::CFerriswheelModule(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CFerriswheelModule::~CFerriswheelModule(void)
{
}

HRESULT CFerriswheelModule::Ready_Object(_vec3 vDis, _vec3 vRot, void* pArg)
{
	_matrix matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(vRot.x));
	D3DXVec3TransformNormal(&m_vAddDir, &vDis, &matRotX);

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CFerriswheelModule::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CFerriswheelModule::LateUpdate_Object(const _float & fTimeDelta)
{
	_vec3* vDir = (_vec3*)Engine::Get_GameObject(L"GameObject", m_tObjInfo.tagMesh)->Is<CSmallFan>()->On_Attach(this);
	m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + *vDir);
	m_pTransformCom->Update_Component(fTimeDelta);

	return _int();
}

void CFerriswheelModule::Render_Object(const _int& iIndex)
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

CFerriswheelModule* CFerriswheelModule::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vDis, _vec3 vRot, void* pArg)
{
	CFerriswheelModule*	pInstance = new CFerriswheelModule(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vDis, vRot, pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFerriswheelModule::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CFerriswheelModule::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FerrisModule"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos + m_vAddDir);
	//m_pTransformCom->Set_AddAngleZ(D3DXToRadian(90.0f));
	m_pTransformCom->Set_AddAngleY(D3DXToRadian(180.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CFerriswheelModule::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
