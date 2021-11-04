#include "stdafx.h"
#include "Torch.h"
#include "MainCamera.h"
#include "Export_Function.h"


CTorch::CTorch(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTorch::~CTorch()
{
}

HRESULT CTorch::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);



	m_tShader.m_fRimDist = 0.1f;
	m_tShader.m_iRimPower = 1;
	m_tShader.m_vRimColor = _vec4(0.3f, 0.3f, 0.3f, 0.1f);

	return S_OK;
}

_int CTorch::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CTorch::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CTorch::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 5);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();
}

HRESULT CTorch::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Torch"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Texture
	pComponent = m_pTexture = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(tTemp.eSceneID, L"TorchTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	return S_OK;
}

HRESULT CTorch::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTexture->Set_Texture(pEffect, "g_NormalTexture", 0);
	m_pTexture->Set_Texture(pEffect, "g_Add1Texture", 1);

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);

	return S_OK;
}


CTorch * CTorch::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CTorch*	pInstance = new CTorch(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTorch::Free(void)
{
	CGameObject::Free();
}
