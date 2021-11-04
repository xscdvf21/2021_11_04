#include "stdafx.h"
#include "SpaceLaser.h"
#include "CollisionMgr.h"
#include "WindLaser.h"
#include "Export_Function.h"

CSpaceLaser::CSpaceLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev)
{
}

CSpaceLaser::~CSpaceLaser()
{
}

HRESULT CSpaceLaser::Ready_Object(void * pArg, _float fAngle)
{
	m_fColDis = 1.0f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_fAngle = fAngle;

	return S_OK;
}

_int CSpaceLaser::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_b3DMap)
	{
		CWindLaser* pWindLaser = (CWindLaser*)Engine::Get_GameObject(L"GameObject", L"WindLaser");
		CTransform* pWindLaserTrans = (CTransform*)pWindLaser->Get_Component(L"Com_Transform", ID_DYNAMIC);
		m_vPos = pWindLaserTrans->Get_Position();
		m_pTransformCom->Set_Pos(m_vPos + _vec3(0.f, 1.f, 0.f));
		m_pTransformCom->Set_Angle(pWindLaserTrans->Get_Angle() + _vec3(0.f, D3DXToRadian(m_fAngle), 0.f));

		CCollisionMgr::GetInstance()->Add_EnermyBulletList(this);
	}
	else
	{
		if (true == m_bStart)
		{
			m_bStart = false;
			m_pTransformCom->Set_Pos(m_tObjInfo.vPos);
			_vec3 vAngle = _vec3(D3DXToRadian(m_tObjInfo.vAngle.x), D3DXToRadian(m_tObjInfo.vAngle.y), D3DXToRadian(m_tObjInfo.vAngle.z));
			m_pTransformCom->Set_Angle(vAngle);
		}
	}

	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CSpaceLaser::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceLaser::Render_Object(const _int & iIndex)
{
	if (!m_bActive)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	m_pMeshCom->Render_Meshes(pEffect, 21);

	pEffect->End();

	Safe_Release(pEffect);


	_float4x4 matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 80.0f, 0.0f);

	m_tColInfo.matWorld = matWorld * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CSpaceLaser::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = tTemp;

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"SpaceLaser"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 1.0f, _vec4(100.f, 255.f, 0.0f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_vPos = tTemp.vPos;

	return S_OK;
}

HRESULT CSpaceLaser::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_DeltaTime", m_fTimeDelta);
	pEffect->SetFloat("g_fDistance", m_fAmount);
	m_pTextureCom->Set_Texture(pEffect, "g_DistortionTexture", 21);


	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
	//m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 39);
	pEffect->SetVector("g_vColor", &_vec4{ 255.f * 3.5f, 125.f * 3.5f, 85.f * 3.5f, 255.f });


	pEffect->CommitChanges();

	return S_OK;
}

CSpaceLaser * CSpaceLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _float fAngle)
{
	CSpaceLaser*	pInstance = new CSpaceLaser(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, fAngle)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpaceLaser::Free(void)
{
	CEnermy::Free();
}
