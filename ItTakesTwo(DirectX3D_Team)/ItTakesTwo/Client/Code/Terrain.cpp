#include "stdafx.h"
#include "Terrain.h"
#include "Export_Function.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fAddUV(0.f)
{

}

CTerrain::~CTerrain(void)
{
}

HRESULT CTerrain::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_pTransformCom->Set_Pos(_vec3(0.f, -4.66f, 68.4f));
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180));

	return S_OK;
}

Engine::_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CTerrain::LateUpdate_Object(const _float & fTimeDelta)
{
	m_fAddUV += fTimeDelta * 0.05f;

	if (Engine::Key_Pressing(DIK_1))
		m_pTransformCom->Move_Pos(&vRIGHT, 5.f, -fTimeDelta);
	if (Engine::Key_Pressing(DIK_2))
		m_pTransformCom->Move_Pos(&vRIGHT, 5.f, fTimeDelta);
	if (Engine::Key_Pressing(DIK_3))
		m_pTransformCom->Move_Pos(&vLOOK, 5.f, -fTimeDelta);
	if (Engine::Key_Pressing(DIK_4))
		m_pTransformCom->Move_Pos(&vLOOK, 5.f, fTimeDelta);
	if (Engine::Key_Pressing(DIK_5))
		m_pTransformCom->Move_Pos(&vUP, 5.f, -fTimeDelta);
	if (Engine::Key_Pressing(DIK_6))
		m_pTransformCom->Move_Pos(&vUP, 5.f, fTimeDelta);
	return _int();
}

void CTerrain::Render_Object(const _int& iIndex )
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();



	Safe_Release(pEffect);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTerrain::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(0, L"Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticObject"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(tMtrlInfo));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_AddUV", m_fAddUV);

	pEffect->CommitChanges();

	return S_OK;
}