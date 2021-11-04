#include "stdafx.h"
#include "CodyDoll.h"
#include "Export_Function.h"
#include "MainCamera.h"

CCodyDoll::CCodyDoll(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CCodyDoll::~CCodyDoll()
{
}

HRESULT CCodyDoll::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_Pos(_vec3(2.f, 0.f, 0.f));
	m_pTransformCom->Set_Angle(_vec3(-0.5159f, -2.2f, -1.2529f));

	m_eAnim = CodyDoll_Idle;

	return S_OK;
}

_int CCodyDoll::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pRoseR = static_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameObject", L"RoseSelectR", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pRoseR, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pRoseR->Get_FrameByName("RightAttach");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pRoseRTrans = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"RoseSelectR", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pRoseR, 0);

		m_pParentWorldMatrix = pRoseRTrans->Get_WorldMatrix();
	}

	m_pTransformCom->Set_ParentMatrix((*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	m_pMeshCom->Play_Animationset(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	m_pMeshCom->Set_Animationset(m_eAnim);

	return OBJ_NOEVENT;
}

_int CCodyDoll::LateUpdate_Object(const _float & fTimeDelta)
{


	

	return _int();
}

void CCodyDoll::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;
	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 4);
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CCodyDoll::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_MENUPLAYERSELECT, L"CodyDoll"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CCodyDoll::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->CommitChanges();

	return S_OK;
}

CCodyDoll * CCodyDoll::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CCodyDoll*	pInstance = new CCodyDoll(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCodyDoll::Free(void)
{
	CGameObject::Free();
}
