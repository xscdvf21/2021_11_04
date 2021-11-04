#include "stdafx.h"
#include "SkyBox.h"
#include "Export_Function.h"


CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bStop(false)
{
}

CSkyBox::~CSkyBox(void)
{
}

HRESULT CSkyBox::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	TRAM temp;
	if (pArg)
	{
		memcpy(&temp, pArg, sizeof(TRAM));
	}

	m_pTransformCom->Set_Pos(_vec3(0.f, -50.f, 0.f));
	m_pTransformCom->Set_Scale(_vec3(0.1f, 0.1f, 0.1f));
	m_pTransformCom->Set_Angle(_vec3(0.f, 0.f, 0.f));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(temp.vPos.x));
	m_fHeight = temp.vPos.y;

	return S_OK;
}

Engine::_int CSkyBox::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(fTimeDelta));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fTimeDelta));


	if (m_bStop)
		return 0;

	//_vec3 vPos;
	//CTransform* pPlayerTransform = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	//pPlayerTransform->Get_Info(INFO_POS, &vPos);

	//m_pTransformCom->Set_Pos(vPos + _vec3(0.f, m_fHeight, 0.f));
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fTimeDelta * 2.f));

	return 0;
}

_int CSkyBox::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSkyBox::Render_Object(const _int& iIndex)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	m_pMeshCom->Render_Meshes(pEffect, 23);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CSkyBox*	pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkyBox::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSkyBox::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"SpaceSkybox"));
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

HRESULT CSkyBox::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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


	return S_OK;
}
