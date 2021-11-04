#include "stdafx.h"
#include "WindLaser.h"
#include "Export_Function.h"
#include "BaboonGround.h"
#include "SpaceBaboon.h"
#include "SpaceLaser.h"
#include "LaserMirror.h"
#include "SpaceBoss.h"


CWindLaser::CWindLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bMoveDir(false), m_bUp(false), m_fUpDis(25.f), m_bAttUp(false), m_fAttDis(5.f), m_bAttDown(false)
	, m_fAttSpeed(D3DXToRadian(1.f)), m_bAttUpDir(false), m_bAttDownDir(false)
{
}

CWindLaser::~CWindLaser()
{
}

HRESULT CWindLaser::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	return S_OK;
}

_int CWindLaser::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{

		Create_Laser(L"SpaceLaser");
		m_bStart = false;
	}

	m_fTimeDelta += fTimeDelta;

	MoveUp(fTimeDelta);
	AttUp(fTimeDelta);
	AttDown(fTimeDelta);


	Effect_Laser(fTimeDelta);

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	_vec3 vRot = m_pTransformCom->Get_Angle();
	pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	m_pRigidActor->setGlobalPose(pxTransform);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CWindLaser::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CWindLaser::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 8);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CWindLaser::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"WindLaser"));
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
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName("BaboonGround");


	return S_OK;
}

HRESULT CWindLaser::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));


	return S_OK;

}

void CWindLaser::MoveUp(const _float & fTimeDelta)
{
	if (!m_bUp)
		return;

	if (!m_bMoveDir)
	{

		m_vMoveUpStartPos = m_pTransformCom->Get_Position();
		CBaboonGround*  pBaboonGround = (CBaboonGround*)Engine::Get_GameObject(L"GameObject", L"BaboonGround");
		pBaboonGround->Set_MoveUp();

		CLaserMirror*	pLaserMirror = (CLaserMirror*)Engine::Get_GameObject(L"GameObject", L"LaserMirror");
		CLaserMirror*	pLaserMirror2 = (CLaserMirror*)Engine::Get_GameObject(L"GameObject", L"LaserMirror2");
		CLaserMirror*	pLaserMirror3 = (CLaserMirror*)Engine::Get_GameObject(L"GameObject", L"LaserMirror3");

		pLaserMirror->Set_AllMove();
		pLaserMirror2->Set_AllMove();
		pLaserMirror3->Set_AllMove();

		CSpaceBaboon*	pBaboon = (CSpaceBaboon*)Engine::Get_GameObject(L"GameObject", L"SpaceBoss");
		pBaboon->Set_Patturn(Patturn_MoveUp);

		CSpaceBoss* pScene = (CSpaceBoss*)Engine::Get_Scene();
		pScene->Set_LightMove();

		m_bMoveDir = true;

		return;
	}

	_vec3 vDir, vPos;
	_float fDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = m_vMoveUpStartPos - vPos;
	fDir = D3DXVec3Length(&vDir);
	CGameObject* pPlayer = nullptr;
	if (fDir < m_fUpDis)
	{
		m_pLaserEffect = true;
		//////////////////////////////////////////////////////////////////////////
		for (_uint i = 0; i < LaserLookEnd; ++i)
		{
			m_pSpaceLaser[i]->Set_Active(true);
		}
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
		m_pTransformCom->Rotation(ROT_Y, m_fAttSpeed);
		for (_uint i = 0; i < 8; ++i)
		{

			const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
			PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
			PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here
			m_fDistance = 1000.f;


			_vec3 vPos = m_pTransformCom->Get_Position();
			_vec3 vDir;
			m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
			D3DXVec3Normalize(&vDir, &vDir);

			_matrix matRot;
			D3DXMatrixRotationY(&matRot, D3DXToRadian(i * 45.f));
			D3DXVec3TransformNormal(&vDir, &vDir, &matRot);

			PxQueryFilterData filterData = PxQueryFilterData();
			filterData.data.word0 = FilterGroup::eGround | FilterGroup::eCody | FilterGroup::eMay;
			_int iIndex = 0, iCurIndex = 0;
			if (m_pScene->raycast(PxVec3(vPos.x, vPos.y + 1.f, vPos.z),
				PxVec3(vDir.x, vDir.y, vDir.z),
				m_fMaxDistance,
				buf, PxHitFlag::eDEFAULT, filterData))
			{
				for (PxU32 i = 0; i < buf.nbTouches; ++i)
				{
					if (buf.getTouch(i).shape->getSimulationFilterData().word0 &(FilterGroup::eGround))
					{
						iCurIndex = 2;
					}
					if (buf.getTouch(i).shape->getSimulationFilterData().word0 &(FilterGroup::eCody | FilterGroup::eMay))
					{
						pPlayer = ToObj<CGameObject>(buf.getTouch(i).shape->getName());
						iCurIndex = 1;
					}
					if (m_fDistance > buf.getTouch(i).distance)
					{

						iIndex = iCurIndex;
						m_fDistance = buf.getTouch(i).distance;
						m_vUltHitPos = ToVec3(buf.getTouch(i).position);
					}
				}
				switch (iIndex)
				{
				case 1:
				{DEBUGMSG(L"맞음"); }
				pPlayer->Set_Damage(-1);
				break;
				case 2:
					break;
				default:
					break;
				}
				auto* pTransform = m_pSpaceLaser[i]->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
				pTransform->Set_Scale({ m_fDistance*0.01f,0.005f,0.005f });
				pTransform->Set_Pos(vPos);
				Effect_LaserHit(fTimeDelta);
			}
		}
	}
	else
	{
		m_pTransformCom->Set_Pos(m_vMoveUpStartPos.x, m_vMoveUpStartPos.y + m_fUpDis, m_vMoveUpStartPos.z);

		m_bUp = false;
		m_bAttDown = true;
		m_bMoveDir = false;
		m_pLaserEffect = false;

		for (_uint i = 0; i < LaserLookEnd; ++i)
		{
			m_pSpaceLaser[i]->Set_Active(false);
		}
	}
}
void CWindLaser::Effect_LaserHit(const _float & fTimeDelta)
{

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.vEndSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 30.f, 30.f, 30.f, 255.f * 1.5f };
	tParticle2.vEndColor = { 30.f, 30.f, 30.f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 0.5f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 23, 4);


	tParticle2.vStartSize = _vec3(2.f, 2.f, 2.f);
	tParticle2.vEndSize = _vec3(3.2f, 3.2f, 3.2f);
	tParticle2.fLifeTime = 0.4f;
	tParticle2.vStartColor = { 255.f * 3.5f, 125.f* 3.5f, 85.f* 3.5f, 255.f };
	tParticle2.vEndColor = { 255.f, 65.f, 35.f, 0.f };
	tParticle2.vPos = m_vUltHitPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);
	
}

void CWindLaser::AttUp(const _float & fTimeDelta)
{
	if (!m_bAttUp)
		return;

	if (!m_bAttUpDir)
	{
		m_vAttUpStartPos = m_pTransformCom->Get_Position();
		m_bAttUpDir = true;
		return;
	}

	_vec3 vDir, vPos;
	_float fDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = m_vAttUpStartPos - vPos;
	fDir = D3DXVec3Length(&vDir);

	if (fDir < m_fAttDis)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 2.f, fTimeDelta);
	}
	else
	{
		m_pTransformCom->Set_Pos(m_vAttUpStartPos.x, m_vAttUpStartPos.y + m_fAttDis, m_vAttUpStartPos.z);

		m_bAttUp = false;
		m_bAttUpDir = false;
		m_bUp = true;
	}
}

void CWindLaser::AttDown(const _float & fTimeDelta)
{
	if (!m_bAttDown)
		return;

	if (!m_bAttDownDir)
	{
		m_vAttDownStartPos = m_pTransformCom->Get_Position();
		m_bAttDownDir = true;
		return;
	}

	_vec3 vDir, vPos;
	_float fDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = m_vAttDownStartPos - vPos;
	fDir = D3DXVec3Length(&vDir);

	if (fDir < m_fAttDis)
	{
		m_pTransformCom->Move_Pos(&_vec3(0.f, -1.f, 0.f), 2.f, fTimeDelta);
	}
	else if (fDir > m_fAttDis)
	{
		m_pTransformCom->Set_Pos(m_vAttDownStartPos.x, m_vAttDownStartPos.y - m_fAttDis, m_vAttDownStartPos.z);

		m_bAttDown = false;
		m_bAttDownDir = false;
		CSpaceBaboon*	pBaboon = (CSpaceBaboon*)Engine::Get_GameObject(L"GameObject", L"SpaceBoss");
		int iHp = pBaboon->Get_Hp();
		if (iHp == 6)
		{
			pBaboon->Set_NextPatturn(true);
		}
		else if (iHp == 5)
		{
			pBaboon->Set_NextPatturn2(true);
		}
	}
}

void CWindLaser::Create_Laser(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = m_tObjInfo.eSceneID;
	tObj.vScale = { 1.f, 0.005f, 0.005f };

	//-90이 룩방향임.
	for (_uint i = 0; i < LaserLookEnd; ++i)
	{
		_tchar tagTemp[MAX_PATH];
		wsprintf(tagTemp, L"SpaceLaser %d", i);

		_float fAngle = -90.f;
		fAngle += i * 45.f;

		pGameObject = CSpaceLaser::Create(m_pGraphicDev, &tObj, fAngle);
		m_pSpaceLaser[i] = pGameObject->Is<CSpaceLaser>();
		m_pSpaceLaser[i]->Set_Index(0);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject), );
		m_pSpaceLaser[i]->AddRef();
		m_pSpaceLaser[i]->Set_Amount(1);
	}
	Engine::Add_Layer(pLayerTag, pLayer);
}

void CWindLaser::Effect_Laser(const _float & fTimeDelta)
{
	if (!m_pLaserEffect)
		return;




}

CWindLaser * CWindLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CWindLaser*	pInstance = new CWindLaser(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWindLaser::Free(void)
{
	Engine::CGameObject::Free();

	for (_uint i = 0; i < LaserLookEnd; ++i)
	{
		m_pSpaceLaser[i]->Release();
	}

}
