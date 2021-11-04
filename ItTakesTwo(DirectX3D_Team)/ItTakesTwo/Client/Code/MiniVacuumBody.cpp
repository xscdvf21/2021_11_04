#include "stdafx.h"
#include "MiniVacuumBody.h"

#include "MiniVacuumHub.h"
#include "Export_Function.h"


CMiniVacuumBody::CMiniVacuumBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_fDis(0.8f)
	, m_vDir(0.f, 0.f, 0.f)
{
}

CMiniVacuumBody::~CMiniVacuumBody()
{
}

HRESULT CMiniVacuumBody::Ready_Object(_float fDis, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fDis = m_fDis * fDis;

	if (nullptr != pArg)
	{
		
	}
	m_pTransformCom->Set_Scale(_vec3(0.8f, 0.8f, 0.8f));

	return S_OK;
}

_int CMiniVacuumBody::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pMiniVacuumHub = static_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameObject", L"MiniVacuumHub", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pMiniVacuumHub, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pMiniVacuumHub->Get_FrameByName("Hips");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pMiniVacuumHubTrans = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"MiniVacuumHub", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pMiniVacuumHubTrans, 0);

		m_pParentWorldMatrix = pMiniVacuumHubTrans->Get_WorldMatrix();
	}

	//m_pMeshCom->Play_Animationset(fTimeDelta);

	m_pTransformCom->Set_ParentMatrix((*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	Engine::CTransform*		pMiniVacuumHubTrans = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"MiniVacuumHub", L"Com_Transform", Engine::ID_DYNAMIC));


	//라이트해야함. 매쉬방향이 그럼.
	_vec3 vRight;
	pMiniVacuumHubTrans->Get_Info(INFO_LOOK, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	vRight = vRight * m_fDis;


	_matrix temp;
	m_pTransformCom->Get_WorldMatrix(&temp);
	temp._41 = temp._41 + vRight.x;
	temp._42 = temp._42 + vRight.y;
	temp._43 = temp._43 + vRight.z;

	m_pTransformCom->Set_WorldMatrix(temp);

	_vec3 vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos = vPos + vRight;

	m_pTransformCom->Set_Pos(vPos);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vDir = m_vDir * 2.f;
	m_tInfo.iHp;
	return OBJ_NOEVENT;
}

_int CMiniVacuumBody::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CMiniVacuumBody::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
												//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CMiniVacuumBody::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STATIC, L"MiniVacuumBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CMiniVacuumBody::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CMiniVacuumBody * CMiniVacuumBody::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fDis, void * pArg)
{
	CMiniVacuumBody*	pInstance = new CMiniVacuumBody(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(fDis,pArg)))
		Safe_Release(pInstance);


	return pInstance;
}

void CMiniVacuumBody::Free(void)
{
	Engine::CGameObject::Free();
}
