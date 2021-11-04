#include "stdafx.h"
#include "BossTimeBullet.h"
#include "Boss.h"
#include "BulletBust.h"
#include "Blood.h"
#include "DynamicCamera.h"
#include "SoundMgr.h"
#include "BulletWind.h"
#include "Export_Fucntion.h"



CBossTimeBullet::CBossTimeBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CBossTimeBullet::~CBossTimeBullet()
{
}

HRESULT CBossTimeBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.02f, 0.02f, 0.02f));
	m_pTransformCom->Set_Pos(_vec3(0.f, 2.f, 5.f));


	m_pParentBoneMatrix_Sphere = nullptr;
	D3DXMatrixIdentity(&m_pFinalMatrix_Sphere);

	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, );

	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));



	_vec3 vPlayerPos;

	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	//도착지에서 출발지를 빼야 우리가원하는게나옴
	m_vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	D3DXVec3Normalize(&m_vDir, &m_vDir);


	m_tINFOBOSS.iAttack = 3;


	return S_OK;
}

HRESULT CBossTimeBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.02f, 0.02f, 0.02f));
	m_pTransformCom->Set_Pos(vPos);

	m_pParentBoneMatrix_Sphere = nullptr;

	D3DXMatrixIdentity(&m_pFinalMatrix_Sphere);

	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, );

	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	m_vWindPos = vPos;

	m_tINFOBOSS.iAttack = 3;

	return S_OK;
}

_int CBossTimeBullet::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (fDelta > 2.f)
	{
		m_pTransformCom->Move_Pos(&m_vDir, 30.f, fTimeDelta);

		if (false == m_bBulletWind)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
			CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Bite.wav", CSoundMgr::CHANNELID::MONSTER);

			CManagement*		pManagement = CManagement::GetInstance();
			NULL_CHECK_RETURN(pManagement, );

			CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));


			_vec3 vPlayerPos;

			pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
			//도착지에서 출발지를 빼야 우리가원하는게나옴
			m_vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

			D3DXVec3Normalize(&m_vDir, &m_vDir);

			CLayer*			pLayer = nullptr;

			pLayer = pManagement->Get_Layer(L"GameLogic");

	/*		_vec3 vTempPos = vPos;*/

			// 오브젝트 추가
			Engine::CGameObject*		pGameObject = nullptr;

			pGameObject = CBulletWind::Create(m_pGraphicDev, m_vDir, m_vWindPos, 2.f);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect_BulletWind", pGameObject), E_FAIL);

			pGameObject = CBulletWind::Create(m_pGraphicDev, m_vDir, m_vWindPos, 3.5f);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect_BulletWind2", pGameObject), E_FAIL);

			pGameObject = CBulletWind::Create(m_pGraphicDev, m_vDir, m_vWindPos, 5.f);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect_BulletWind3", pGameObject), E_FAIL);

			m_bBulletWind = true;
		}
	}
	SetUp_Sphere();

	m_pMeshCom->Play_Animationset((fTimeDelta * 3));

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	fDelta += fTimeDelta;



	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_bColl = Collision_ToObject(L"GameLogic", L"Player", fTimeDelta);


	if (true == m_bColl && false == m_bCollOne)
	{
		CManagement*		pManagement = CManagement::GetInstance();
		NULL_CHECK_RETURN(pManagement, );

		CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

		if (nullptr == pPlayer)
			return;


		_bool		bPlayerDash = pPlayer->Get_Dash();

		if (!bPlayerDash)
		{
			INFOPLAYER Temp = pPlayer->Get_INFOPLAYER();
			Temp.iHP -= m_tINFOBOSS.iAttack;

			pPlayer->Set_INFOPLAYER(Temp);
			OutputDebugStringW(L"총알이 플레이어 공격했음 \n");


			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER_EFFECT);
			CSoundMgr::Get_Instance()->PlaySoundW(L"Hit_Player_Blood.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);


			m_bCollOne = true;

			Engine::CLayer*			pLayer = nullptr;

			pLayer = pManagement->Get_Layer(L"GameLogic");
			NULL_CHECK_RETURN(pLayer, E_FAIL);

			// 오브젝트 추가
			Engine::CGameObject*		pGameObject = nullptr;

			pGameObject = CBlood::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Blood", pGameObject), E_FAIL);


			CDynamicCamera*		pCamera = dynamic_cast<CDynamicCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));

			if (nullptr == pCamera)
				return;

			pCamera->Set_Shake(true);
		}
	}

	if (fDelta >= 5.f || vPos.y <= 0.f)
	{
		CManagement*		pManagement = CManagement::GetInstance();
		NULL_CHECK_RETURN(pManagement, );

		CGameObject*			pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

		if (nullptr == pPlayer)
			return;

		Engine::CLayer*			pLayer = nullptr;

		pLayer = pManagement->Get_Layer(L"GameLogic");
		NULL_CHECK_RETURN(pLayer, E_FAIL);

		// 오브젝트 추가
		Engine::CGameObject*		pGameObject = nullptr;

		_vec3 vPos;
		
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y += 5.f;

		pGameObject = CBulletBust::Create(m_pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect_Bust", pGameObject), E_FAIL);

		
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;


	return 0;
}

void CBossTimeBullet::Render_Object(void)
{
	m_pTransformCom->Set_Rotation(EAxis::Z, D3DXToRadian(-90.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);


	m_pMeshCom->Render_Meshes();
	m_pSphere->Render_Buffer(&m_pFinalMatrix_Sphere);


	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pTransformCom->Set_Rotation(EAxis::Z, D3DXToRadian(90.f));


}

HRESULT CBossTimeBullet::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);


	pComponent = m_pSphere = dynamic_cast<Engine::CSphere*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Proto_Buffer_Sphere"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Sphere", pComponent);


	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Bossbullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// NaviMesh
	//pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

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
	//pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

void CBossTimeBullet::SetUp_Sphere(void)
{
	if (nullptr == m_pParentBoneMatrix_Sphere)
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("E_EM001_100_00");

		m_pParentBoneMatrix_Sphere = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix_Sphere = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix_Sphere = *m_pParentBoneMatrix_Sphere * *m_pParentWorldMatrix_Sphere;


	if (!m_listCollision.empty())
	{
		m_listCollision.clear();
	}
	_vec3 vSpherePos;
	vSpherePos = { m_pFinalMatrix_Sphere._41, m_pFinalMatrix_Sphere._42, m_pFinalMatrix_Sphere._43 };

	_float vRadius = 3.f;
	m_listCollision.push_back(make_pair(vSpherePos, vRadius));
}

_bool CBossTimeBullet::Collision_ToObject(const _tchar * pLayerTag, const _tchar * pObjTag, const _float & fTimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();

	CGameObject*	pPlayer = pManagement->Get_GameObject(pLayerTag, pObjTag);

	_bool PlayerAttackCheck = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(pLayerTag, pObjTag))->Get_AttackCheck();


	if (m_pCalculatorCom->Collision_Sphere(this, pPlayer))
	{
		return true;
	}

	return false;

}

CBossTimeBullet * CBossTimeBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBossTimeBullet*	pInstance = new CBossTimeBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBossTimeBullet::Free(void)
{
	Engine::CGameObject::Free();
}
