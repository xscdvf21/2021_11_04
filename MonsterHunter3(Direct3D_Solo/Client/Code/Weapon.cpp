#include "stdafx.h"
#include "Weapon.h"
#include "Player.h"
#include "Export_Fucntion.h"
#include "SoundMgr.h"
#include "HitPlayer.h"
CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CWeapon::~CWeapon()
{
}

HRESULT CWeapon::Ready_Object(const _uint & iFlag)
{
	m_iFlag = iFlag;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-30.f));
	//m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(180.f));
	//m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(-90.f));

	ZeroMemory(&m_tINFOPLAYER, sizeof(m_tINFOPLAYER));


	for (_uint i = 0; i < WP_END; ++i)
	{
		m_pParentBoneMatrix_Sphere[i] = nullptr;
	}

	for (_uint i = 0; i < WP_END; ++i)
	{
		D3DXMatrixIdentity(&m_pFinalMatrix_Sphere[i]);
	}


	m_tINFOPLAYER.iAttack = 30;

	return S_OK;
}

_int CWeapon::Update_Object(const _float & fTimeDelta)
{

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_fFontTime += fTimeDelta;
	m_fJumpTime += fTimeDelta;

	if (m_fJumpTime > 0.4f)
	{
		m_bTimeStop = false;
	}
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName("AL2D03");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransformCom, 0);


		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix();

	}

	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
	_matrix temp;

	m_pTransformCom->Get_WorldMatrix(&temp);
	//temp._41 = temp._41 - 0.3f;
	temp._42 = temp._42 - 0.1f;
	//temp._43 = temp._43 - 0.3f;

	m_pTransformCom->Set_WorldMatrix(&temp);
	SetUp_Sphere();


	

	
	m_bColl = Collision_ToObject(L"GameLogic", L"Boss", fTimeDelta);

	
	if (m_bColl)
	{
		m_tINFOPLAYER.iAttack = rand() % 20 + 40;

		m_bTimeStop = true;
		
		m_fFontTime = 0.f;
		m_fJumpTime = 0.f;
		m_bFont = true;

		m_pFontMatrix = m_pFinalMatrix_Sphere[WP_00];

		m_vJumpPos = { m_pFontMatrix._41,m_pFontMatrix._42, m_pFontMatrix._43 };


		CManagement*		pManagement = CManagement::GetInstance();
		NULL_CHECK_RETURN(pManagement, );

		CGameObject*			pBossObj = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Boss"));

		if (nullptr == pBossObj)
			return;

		INFOBOSS Temp = pBossObj->Get_INFOBOSS();
		Temp.iHP -= m_tINFOPLAYER.iAttack;

		pBossObj->Set_INFOBOSS(Temp);
		OutputDebugStringW(L"무기와 보스가 충돌했음% \n");

		Engine::CLayer*			pLayer = nullptr;

		pLayer = pManagement->Get_Layer(L"GameLogic");
		NULL_CHECK_RETURN(pLayer, E_FAIL);

		// 오브젝트 추가
		Engine::CGameObject*		pGameObject = nullptr;


		_vec3 vPos = { m_pFinalMatrix_Sphere[WP_00]._41, m_pFinalMatrix_Sphere[WP_00]._42, m_pFinalMatrix_Sphere[WP_00]._43 };
		pGameObject = CHitPlayer::Create(m_pGraphicDev,vPos );
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HitPlayer", pGameObject), E_FAIL);

		

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Hit.wav", CSoundMgr::CHANNELID::PLAYER_EFFECT);
		
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CWeapon::Render_Object(void)
{

	m_pTransformCom->Set_Rotation(EAxis::Y, D3DXToRadian(90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pMeshCom->Render_Meshes();
	if (exColisionSee)
	{
		for (_uint i = 0; i < WP_END; ++i)
		{

			m_pSphere->Render_Buffer(&m_pFinalMatrix_Sphere[i], _vec3(0.7f, 0.7f, 0.7f)/*, _vec3(-1.f,1.f,0.f) */);
		}
	}
	_matrix		matWorld;



	wsprintf(m_szFPS, L"%d", m_tINFOPLAYER.iAttack);

	if (m_fFontTime <= 2.f && m_bFont)
	{
		Engine::Render_Font3D(L"Font_Jinji", m_szFPS, &_vec2(0.f, 0.f), m_pFontMatrix, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
		

		//점프 공식.
		//m_pFontMatrix._41 = m_vJumpPos.x + m_fJumpTime * m_fJumpPowerY;
		//m_pFontMatrix._42 = m_vJumpPos.y + (m_fJumpPowerY * m_fJumpTime - 4.f * m_fJumpTime * m_fJumpTime);
		//m_pFontMatrix._43 = m_vJumpPos.z + m_fJumpTime;


	}
	else if (m_fFontTime < 2.f)
	{
		m_bFont = false;
	}


	if (0 == m_iFlag)
		m_pTransformCom->Get_WorldMatrix(&matWorld);
	else
		m_pTransformCom->Get_NRotWorldMatrix(&matWorld);

	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLTYPE(m_bColl), &matWorld);

	/*m_pTransformCom->Set_Rotation(EAxis::Y, D3DXToRadian(-90.f));*/
}

HRESULT CWeapon::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pSphere = dynamic_cast<Engine::CSphere*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Proto_Buffer_Sphere"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Sphere", pComponent);

	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player_Weapon"));
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

_bool CWeapon::Collision_ToObject(const _tchar * pLayerTag, const _tchar * pObjTag , const _float& fTimeDelta)
{


	CManagement*	pManagement = CManagement::GetInstance();

	CGameObject*	pObj = pManagement->Get_GameObject(pLayerTag, pObjTag);
	CGameObject*	pPlayer = pManagement->Get_GameObject(pLayerTag, L"Player");
	
	_bool PlayerAttackCheck = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(pLayerTag, L"Player"))->Get_AttackCheck();



	if (PlayerAttackCheck) 
	{
		_float iStack = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(pLayerTag, L"Player"))->Get_AttackStack();

		if (iStack == 2)
		{
			m_fDelta += fTimeDelta * 2.5f;
		}
		else
		m_fDelta += fTimeDelta;

		if (m_pCalculatorCom->Collision_Sphere(this, pObj) && m_fDelta >= m_fDel)
		{

			if(m_fDelta >= m_fDel)
			{
				m_fDelta = 0.f;


				if (iStack != m_iAttackStack)
				{
					m_iAttackStack++;
					return true;
				}
			}

		}

	}
	else
	{
		_float iStack = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(pLayerTag, L"Player"))->Get_AttackStack();
		m_iAttackStack = -1;
		m_fDelta = 0.f;
		
	}
	


	return false;

	//Engine::CCollider*		pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	//NULL_CHECK_RETURN(pPlayerColliderCom, false);

	//if (0 == m_iFlag)
	//{
	//	return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_ColMatrix(),
	//		m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_ColMatrix());
	//}

	//else
	//	return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_ColMatrix(),
	//		m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_ColMatrix());

}

void CWeapon::SetUp_Sphere(void)
{
	if (nullptr == m_pParentBoneMatrix_Sphere[WP_00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WP-01");

		m_pParentBoneMatrix_Sphere[WP_00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix_Sphere = m_pTransformCom->Get_WorldMatrix();

		m_pWorld = *m_pParentBoneMatrix_Sphere[WP_00];
		m_pWorld.m[3][1] -= 100.f;
		m_pWorld.m[3][2] -= 100.f;
	}
	m_pFinalMatrix_Sphere[WP_00] = m_pWorld * *m_pParentWorldMatrix_Sphere;


	if (!m_listCollision.empty())
	{
		m_listCollision.clear();
	}

	_vec3 vSpherePos;
	vSpherePos = { m_pFinalMatrix_Sphere[WP_00]._41, m_pFinalMatrix_Sphere[WP_00]._42, m_pFinalMatrix_Sphere[WP_00]._43 };

	/*_float vRadius = 1.5f;*/
	_float vRadius = 3.f;
	m_listCollision.push_back(make_pair(vSpherePos, vRadius));

}

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint & iFlag)
{
	CWeapon*	pInstance = new CWeapon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWeapon::Free(void)
{
	Engine::CGameObject::Free();
}
