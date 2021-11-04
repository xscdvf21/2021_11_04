#include "stdafx.h"
#include "FootL.h"
#include "Export_Function.h"
#include "Player.h"


CFootL::CFootL(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CFootL::~CFootL(void)
{

}

HRESULT CFootL::Ready_Object(const _uint& iFlag)
{
	m_iFlag = iFlag;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

Engine::_int CFootL::Update_Object(const _float& fTimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
		return 0;
	}
	if (nullptr == m_pParentBoneMatrix)
	{
		CPlayer* pPlayer = (CPlayer*)Engine::Get_GameObject(L"GameObject", L"Player");
		Engine::CDynamicMesh*	pPlayerMeshCom = (CDynamicMesh*)pPlayer->Get_SpongeBob()->Get_Component(L"Com_Mesh", ID_STATIC);
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("jt_bubbleFoot_L");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pSpongeBobTransformCom = dynamic_cast<CTransform*>(pPlayer->Get_SpongeBob()->Get_Component(L"Com_Transform", ID_DYNAMIC));

		m_pParentWorldMatrix = pSpongeBobTransformCom->Get_WorldMatrix();
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));


	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CFootL::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CFootL::Render_Object(const _int& iIndex = 0)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(0);

	_matrix		matTrans, matWorld;
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	if (m_bActive)
	{
		m_pMeshCom->Render_Meshes(pEffect);
		m_pColliderCom->Render_Collider(matWorld * matTrans, g_bDebug);
	}

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

CFootL* CFootL::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CFootL*	pInstance = new CFootL(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFootL::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CFootL::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STATIC, L"FootL"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 0.5f, _vec4(50.f, 255.f, 255.f, 255.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CFootL::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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
