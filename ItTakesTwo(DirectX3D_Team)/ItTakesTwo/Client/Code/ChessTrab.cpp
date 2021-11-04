#include "stdafx.h"
#include "ChessTrab.h"
#include "CollisionMgr.h"
#include "Export_Function.h"

CChessTrab::CChessTrab(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_fMoveSpeed(5.f)
	, m_fTurnSpeed(D3DXToRadian(10))
	, m_fDeltaTime(0.f)
	, m_fCurTime(0.f)
	, m_fCollisionTime(0.f)
{
}

CChessTrab::~CChessTrab()
{
}

HRESULT CChessTrab::Ready_Object(_vec3 vStartPos, _vec3 vEndPos, void * pArg)
{

	m_fColDis = 1.f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, tTemp.vAngle.x);
	m_pTransformCom->Rotation(ROT_Y, tTemp.vAngle.y);
	m_pTransformCom->Rotation(ROT_Z, tTemp.vAngle.z);
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_vMoveStartPos = vStartPos;
	m_vMoveEndPos = vEndPos;

	m_bAttAble = true;

	return S_OK;
}

_int CChessTrab::Update_Object(const _float & fTimeDelta)
{
	m_fCollisionTime += fTimeDelta;
	Move(fTimeDelta);

	if (m_fCollisionTime > 1.f)
	{
		CEnermy::Clear_CollisionList();
		m_fCollisionTime = 0.f;
	}


	CCollisionMgr::GetInstance()->Add_EnermyWeaponList(this);
	
	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);




	return OBJ_NOEVENT;
}

_int CChessTrab::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CChessTrab::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);



	//Spine2
	m_tColInfo.matWorld =  *m_pTransformCom->Get_WorldMatrix();
	m_tColInfo.matWorld._42 += 2.f;
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CChessTrab::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessSpinTrab"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_tColInfo.fRadius, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CChessTrab::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CChessTrab::Move(const _float & fTimeDelta)
{
	//m_fDeltaTime += fTimeDelta / 10.f;
	//if (m_fDeltaTime >= 1.f)
	//{
	//	m_fDeltaTime = 1.0f;
	//}
	if (m_bChangeStart_Pos)
		ChangePos(fTimeDelta, 2.f, m_vMoveStartPos, m_vMoveEndPos);
	else if(!m_bChangeStart_Pos)
		ChangePos(fTimeDelta, 2.f, m_vMoveEndPos, m_vMoveStartPos);

	m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
}

void CChessTrab::ChangePos(const _float& fTimeDelta, const _float& fMoveTime, _vec3 vStartPos, _vec3 vEndPos)
{
	m_fMoveTime = fMoveTime;
	m_vStartPos = vStartPos;
	m_vEndPos = vEndPos;


	m_fCurTime += fTimeDelta ;
	m_pTransformCom->Set_Pos
	(
		(((m_vStartPos.x + ((((m_vEndPos.x) - (m_vStartPos.x)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.y + ((((m_vEndPos.y) - (m_vStartPos.y)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.z + ((((m_vEndPos.z) - (m_vStartPos.z)) / m_fMoveTime)*m_fCurTime))))
	);


	if (m_fMoveTime < m_fCurTime)
	{
		m_bChangeStart_Pos = !m_bChangeStart_Pos;
		m_fCurTime = 0.f;
	}
	
}

CChessTrab * CChessTrab::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vStartPos, _vec3 vEndPos, void * pArg)
{
	CChessTrab*	pInstance = new CChessTrab(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vStartPos, vEndPos,pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChessTrab::Free(void)
{
	CEnermy::Free();
}
