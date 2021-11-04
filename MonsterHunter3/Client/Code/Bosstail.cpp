#include "stdafx.h"
#include "Bosstail.h"


#include "Export_Fucntion.h"



CBosstail::CBosstail(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBosstail::~CBosstail(void)
{
}

HRESULT CBosstail::Ready_Object(const _uint & iFlag)
{
	m_iFlag = iFlag;


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	/*m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));*/

	return S_OK;
}

_int CBosstail::Update_Object(const _float & fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pBossMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Boss", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pBossMesh, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pBossMesh->Get_FrameByName("TL-04");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pBossTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Boss", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pBossTransformCom, 0);

		m_pParentWorldMatrix = pBossTransformCom->Get_WorldMatrix();
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bTailDestory) //²¿¸® ÆÄ±«.
	{
	
		if (!m_bTailDestoryOne)
		{
			_vec3 vPos;
			m_pTransformCom->Set_Scale(_vec3(0.02f, 0.02f, 0.02f));
			m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
			_vec3 temp = { m_pTransformCom->m_matWorld._41, m_pTransformCom->m_matWorld._42,m_pTransformCom->m_matWorld._43 };
			temp.y = 1.f;
			m_pTransformCom->Set_Pos(temp);

			m_bTailDestoryOne = true;
		}
		
	}
	else
	{
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
		
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return _int();
}

void CBosstail::Render_Object(void)
{
	m_pTransformCom->Set_Rotation(EAxis::X, D3DXToRadian(90));
	m_pTransformCom->Set_Rotation(EAxis::Z, D3DXToRadian(90));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pMeshCom->Render_Meshes();


	m_pTransformCom->Set_Rotation(EAxis::X, D3DXToRadian(-90));
	m_pTransformCom->Set_Rotation(EAxis::Z, D3DXToRadian(-90));

	_matrix		matWorld;

	if (0 == m_iFlag)
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	else
	m_pTransformCom->Get_NRotWorldMatrix(&matWorld);

	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLTYPE(m_bColl), &matWorld);

}

HRESULT CBosstail::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Bosstail"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_VtxCnt(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

_bool CBosstail::Collision_ToObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	Engine::CCollider*		pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	if (0 == m_iFlag)
	{
		return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_ColMatrix(),
			m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_ColMatrix());
	}

	else
		return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_ColMatrix(),
			m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_ColMatrix());
}

CBosstail * CBosstail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint & iFlag)
{
	CBosstail*	pInstance = new CBosstail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBosstail::Free(void)
{
	Engine::CGameObject::Free();
}
