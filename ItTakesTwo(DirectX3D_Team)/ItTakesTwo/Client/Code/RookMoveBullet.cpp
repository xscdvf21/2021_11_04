#include "stdafx.h"
#include "RookMoveBullet.h"
#include "ChessTile.h"
#include "RookStopBullet.h"
#include "Export_Function.h"


CRookMoveBullet::CRookMoveBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iBulletCount(0)
	, m_iBulletName(0)
{
}

CRookMoveBullet::~CRookMoveBullet()
{
}

HRESULT CRookMoveBullet::Ready_Object(_vec3 vPos, _vec3 vDir, _uint iCount)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vCurrentPos = vPos;

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(vPos);

	m_iBulletName = iCount;
	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir); //혹시 몰라서 한번더해줌, 애초에 인자로 던질때 노말라이즈 해주긴할것.


	return S_OK;
}

_int CRookMoveBullet::Update_Object(const _float & fTimeDelta)
{
	m_fLifeTime += fTimeDelta;
	m_fEffecTime += fTimeDelta;

	_vec3	vPos;
	_vec3	vDir;
	_float	fDir;
	vPos = m_pTransformCom->Get_Position();

	vDir = vPos - m_vCurrentPos;
	fDir = D3DXVec3Length(&vDir);

	if (fDir >= 2.7f)
	{

		CHESSTILE	tTemp;
		ZeroMemory(&tTemp, sizeof(CHESSTILE));

		tTemp = CChessTile::GetInstance()->Get_Index(vPos);

		_tchar tagTemp[MAX_PATH];

		wsprintf(tagTemp, L"RookStopBullet %d %d", m_iBulletCount, m_iBulletName);

		CLayer*				pLayer = nullptr;
		pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;

		//pGameObject = CBiShopStopBullet::Create(m_pGraphicDev, tTemp.vPos);
		//pLayer->Add_GameObject(tagTemp, pGameObject);
		//m_iBulletCount++;

		//m_vCurrentPos = m_pTransformCom->Get_Position();


		pGameObject = CRookStopBullet::Create(m_pGraphicDev, vPos);
		pLayer->Add_GameObject(tagTemp, pGameObject);
		m_iBulletCount++;

		m_vCurrentPos = m_pTransformCom->Get_Position();
	}
	if (vPos.x <= -10.f || vPos.z <= -10.f || vPos.x > 10.f || vPos.z > 10.f)
	{
		m_bDead = true;
	}


	if (m_fLifeTime > 5.f) // 현재는 라이프타임  2초로 설정해둠. 나중에 플레이어나 벽충돌하면 없어지게해야할듯
	{
		m_bDead = true;
	}

	if (m_bDead)
		return OBJ_DEAD;


	m_pTransformCom->Move_Pos(&m_vDir, 10.f, fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	//Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return OBJ_NOEVENT;
}

_int CRookMoveBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	Effect_Walk();
	return _int();
}

void CRookMoveBullet::Render_Object(const _int & iIndex)
{
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//pEffect->AddRef();

	//FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	//_uint	iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	//									//pEffect->BeginPass(0);
	//m_pMeshCom->Render_Meshes(pEffect, 8);

	//pEffect->End();

	//Safe_Release(pEffect);


}

HRESULT CRookMoveBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"IceOrb"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CRookMoveBullet::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CRookMoveBullet::Move(const _float & fTimeDelta)
{

}

void CRookMoveBullet::Effect_Walk()
{
	if (m_fEffecTime > 0.04f)
	{
		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		Engine::Set_AnimState(tParticle3, 8, 8);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		_float fSize = CRandoms()(10, 70) * 0.01f + 1.5f;
		tParticle3.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle3.vEndSize = _vec3(0.4f, 0.4f, 0.4f);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.fVelocity = 1.f;
		tParticle3.fLifeTime = 0.8f;
		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.vStartColor = { 30.f, 30.f, 30.f, 255.f * 3.5f };
		tParticle3.vEndColor = { 30.f, 30.f, 30.f, 255.f* 3.5f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-4, 4) * 0.1f, 0.7f, CRandoms()(-4, 4) * 0.1f);
		Engine::Emit_ImageParticle(tParticle3, 5, 19);

		tParticle3.vStartSize = _vec3(fSize - 0.2f, fSize - 0.2f, fSize - 0.2f);
		tParticle3.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
		Engine::Emit_ImageParticle(tParticle3, 5, 4);
		m_fEffecTime = 0.f;
	}
}

CRookMoveBullet * CRookMoveBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir, _uint iCount)
{
	CRookMoveBullet*	pInstance = new CRookMoveBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, iCount)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRookMoveBullet::Free(void)
{
	Engine::CGameObject::Free();
}
