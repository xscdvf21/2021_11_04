#include "stdafx.h"
#include "BandSaw.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "Transform.h"


CBandSaw::CBandSaw(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBandSaw::~CBandSaw(void)
{
}

HRESULT CBandSaw::Ready_Object(void* pArg)
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

Engine::_int CBandSaw::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTransformCom->Move_Pos(&_vec3(0.0f, -1.0f, 0.0f), 3.0f, fTimeDelta);
	if (m_pTransformCom->Get_Position().y <= 7.0f)
		m_pTransformCom->Set_Pos(_vec3(-82.255f, 9.73f, 73.7f));

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CBandSaw::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CBandSaw::Render_Object(const _int& iIndex)
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

CBandSaw* CBandSaw::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CBandSaw*	pInstance = new CBandSaw(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBandSaw::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBandSaw::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BandSaw"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AddAngleY(D3DXToRadian(90.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CBandSaw::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
