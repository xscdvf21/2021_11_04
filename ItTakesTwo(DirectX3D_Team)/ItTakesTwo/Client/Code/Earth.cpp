#include "stdafx.h"
#include "Earth.h"
#include "Export_Function.h"


CEarth::CEarth(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bStop(false)
{
}

CEarth::~CEarth(void)
{
}

HRESULT CEarth::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_pTransformCom->Set_Pos(_vec3(0.f, -800.f, 0.f));
	m_pTransformCom->Set_Scale(_vec3(0.00005f, 0.00005f, 0.00005f));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(90.f));

	return S_OK;
}

Engine::_int CEarth::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(fTimeDelta * 0.2f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fTimeDelta * 0.2f));

	if (m_bStop)
		return 0;

	//_vec3 vPos;
	//CTransform* pPlayerTransform = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	//pPlayerTransform->Get_Info(INFO_POS, &vPos);

	//m_pTransformCom->Set_Pos(vPos + _vec3(0.f, m_fHeight, 0.f));
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fTimeDelta * 2.f));

	return 0;
}

_int CEarth::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CEarth::Render_Object(const _int& iIndex)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	m_pMeshCom->Render_Meshes(pEffect, 25);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CEarth* CEarth::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CEarth*	pInstance = new CEarth(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEarth::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CEarth::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO temp;
	if (pArg)
	{
		memcpy(&temp, pArg, sizeof(OBJINFO));
	}

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(temp.eSceneID, L"Earth"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CEarth::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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
	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	pEffect->CommitChanges();

	pEffect->SetFloat("g_DeltaTime", m_fTimeDelta);


	return S_OK;
}
