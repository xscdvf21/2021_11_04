#include "stdafx.h"
#include "MiniVacuum.h"
#include "MiniVacuumBody.h"
#include "Export_Function.h"




CMiniVacuum::CMiniVacuum(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMiniVacuum::~CMiniVacuum()
{

}

void CMiniVacuum::Set_Suck(bool bState)
{
	m_bSuck = bState;
}

void* CMiniVacuum::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	m_vecOut.clear();
	m_vecOut.emplace_back(&m_vecIOPath);
	return &m_vecOut;
}

Engine::_mat CMiniVacuum::Get_EventPos()
{
	_mat mat;
	memcpy(&mat, &PxMat44(m_vecHozeRigid.back()->getGlobalPose()), sizeof(_mat));
	return mat;
}

HRESULT CMiniVacuum::Ready_Object(_uint iDir, _uint iLength, void* pArg /*= nullptr*/)
{
	FAILED_CHECK_RETURN(Add_Component(pArg, iLength), E_FAIL);
	Set_Rotation(iDir);
	if (nullptr != pArg)
	{
	}
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Update_Component(0.f);

	_mat matHubBase = m_pMeshHub->Get_FrameByName("Hips")->CombinedTranformationMatrix;
	matHubBase *= *m_pTransformCom->Get_WorldMatrix();

	PxTransform t = PxTransform(matHubBase._41, matHubBase._42, matHubBase._43);
	_int iCount = m_pMeshHoze->Get_FrameCount(0);
	m_pHoze = Engine::Get_Physics()->createAggregate(iCount, false);

	//0.6 //1칸
	createChain(t, iCount, 0.6f);
	//createChain(t, iCount,2.f);
	auto* EndHoze = m_vecHozeRigid[m_vecHozeRigid.size() - m_iHeadPos];

	auto HozeEndPos = (m_vecHozeRigid[m_vecHozeRigid.size() - m_iHeadPos])->getGlobalPose();
	EndHoze->setMaxLinearVelocity(1.f);
	m_pTransformHeadCom->Set_Pos(HozeEndPos);
	m_pTransformHeadCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	return S_OK;
}

_int CMiniVacuum::Update_Object(const _float & fTimeDelta)
{
	_vec3 vPos;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, (char*)this, m_pTransformCom->Get_Position() + ToVec3(-m_vDirForce)*3.f, 2.0f, 1.0f, 1.5f, TRIGGERTYPE::TRIGGER_VACUUM_ENTER, FilterGroup::eHoze);
		m_pTrigger->Set_FloatValue(40);
		m_pTrigger->Set_FloatValue(2);
		m_pTrigger->Set_FloatValue(1.f);

		m_pHeadTrigger = CTrigger::CreateBoxTrigger(m_pScene, (char*)this, m_pTransformHeadCom->Get_Position(), 2.0f, 1.0f, 1.5f, TRIGGERTYPE::TRIGGER_VACUUM_RIDE, FilterGroup::eHoze);

		_mat matTrans, matScale, matRot;
		vPos = m_pTransformCom->Get_Position() + ToVec3(-m_vDirForce)*2.f + _vec3{ 0.f,2.f,0.f };
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
		D3DXMatrixRotationZ(&matRot, D3DXToRadian(90.f));
		m_matBlowWorld = matScale*matRot*matTrans;
		m_bStart = false;
	}

	m_vecIOPath.clear();
	for (auto& pHoze : m_vecHozeRigid)
	{
		vPos = { pHoze->getGlobalPose().p.x, pHoze->getGlobalPose().p.y, pHoze->getGlobalPose().p.z };
		m_vecIOPath.emplace_back(vPos);
	}

	m_vecHozeRigid.back()->addForce(m_vDirForce, PxForceMode::eIMPULSE);

	Engine::CGameObject::Update_Object(fTimeDelta);
	_vec3 vHeadPos = m_pTransformHeadCom->Get_Position();
	auto HozeEndPos = (m_vecHozeRigid[m_vecHozeRigid.size() - m_iHeadPos])->getGlobalPose();
	HozeEndPos.q *= PxQuat(D3DXToRadian(90 * m_iDir), PxVec3(0.f, 1.f, 0.f));

	m_pTransformHeadCom->Set_WorldMatrix(HozeEndPos);
	if (m_pHeadTrigger)
		m_pHeadTrigger->Set_Pos(HozeEndPos);

	m_pMeshHub->Play_Animationset(fTimeDelta);
	m_pMeshHead->Play_Animationset(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	if (m_bBlow)
		m_pBlow->Update_Object(fTimeDelta);
	if (m_bBlow)
		m_pBlow->Set_Parent(m_matBlowWorld);
	return OBJ_NOEVENT;
}

_int CMiniVacuum::LateUpdate_Object(const _float & fTimeDelta)
{
	//m_pArticulation->get
	/*m_pRigidActor->setGlobalPose(m_vecHozeRigid.back()->getGlobalPose());*/
	if (m_bBlow)
		m_pBlow->LateUpdate_Object(fTimeDelta);

	m_fEffectTime += fTimeDelta;

	if (m_fEffectTime > 0.12f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 2.f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 1.3f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 15.f;

		_vec3 vLook;
		m_pTransformHeadCom->Get_Info(INFO_LOOK, &vLook);
		tParticle.vDir = vLook;
		tParticle.vPos = ToVec3(m_vecHozeRigid.back()->getGlobalPose().p) + (vLook * 200.f);
		tParticle.vStartSize = _vec3(2.f, 2.f, 2.f);
		tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
		tParticle.vStartColor = { 255.f * 2.5f, 255.f * 2.5f, 255.f * 2.5f, 150.f };
		tParticle.vEndColor = { 150.f, 150.f, 150.f, 0.f };

		Engine::Emit_ImageParticle(tParticle, 1, 4);

		m_fEffectTime = 0.f;
	}

	return _int();
}


void CMiniVacuum::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;
	_matrix			matWorld, matView, matProj;


	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->SetFloat("g_fMaterialIndex", 1.f); //pEffect->BeginPass(0);
	m_pMeshHub->Render_Meshes_VTF(pEffect, 0);


	D3DXMatrixScaling(&matWorld, 0.01f, 0.01f, 0.01f);
	ZeroMemory(&matWorld._41, sizeof(_vec3));
	D3DXMatrixRotationY(&matView, 90);
	//matWorld = matWorld*matView;
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->CommitChanges();

	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	m_pMeshHoze->Render_PhysMeshes(pEffect, 4, m_vecHozeRigid, m_iDir);




	m_pTransformHeadCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	m_pMeshHead->Render_Meshes_VTF(pEffect, 0);



	//D3DXMatrixRotationAxis(&matView, (_vec3*)&(matWorld._11), D3DXToRadian(-90));
	//matWorld = matView*matWorld;
	//pEffect->SetMatrix("g_matWorld", &matWorld);


	//_TCHAR szBuf[256] = L"";
	//_vec3 vPos;
	//OutputDebugString(L"=================================\n");
	//for (size_t i = 0; i < m_vecHozeRigid.size(); ++i)
	//{
	//	PxTransform tTransform = m_vecHozeRigid[i]->getGlobalPose();
	//	vPos = { tTransform.p.x,tTransform.p.y,tTransform.p.z };
	//	swprintf_s(szBuf, L"%d번째\t%f\t%f\t%f\n", (_int)i, vPos.x, vPos.y, vPos.z);
	//	OutputDebugString(szBuf);
	//}

	pEffect->End();

	Safe_Release(pEffect);
}



void CMiniVacuum::Set_Rotation(const _int& iDir)
{
	switch (iDir)
	{
	case DIRUP:
		m_pTransformCom->Set_AngleY(0);
		m_iDir = DIRUP;
		m_vDirForce = { 0.f,0.f,1.f };
		m_vRight = { 1.f,0.f,0.f };
		break;
	case DIRRIGHT:
		m_pTransformCom->Set_AngleY(D3DXToRadian(90));
		m_iDir = DIRRIGHT;
		m_vDirForce = { 1.f,0.f,0.f };
		m_vRight = { 0.f,0.f,-1.f };
		break;
	case DIRDOWN:
		m_pTransformCom->Set_AngleY(D3DXToRadian(180));
		m_iDir = DIRDOWN;
		m_vDirForce = { 0.f,0.f,-1.f };
		m_vRight = { -1.f,0.f,0.f };
		break;
	case DIRLEFT:
		m_pTransformCom->Set_AngleY(D3DXToRadian(270));
		m_iDir = DIRLEFT;
		m_vDirForce = { -1.f,0.f,0.f };
		m_vRight = { 0.f,0.f,1.f };
		break;
	default:
		break;
	}
}


HRESULT CMiniVacuum::Add_Component(void* pArg, _uint iLength)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CDynamicMesh
	pComponent = m_pMeshHub = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHub"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHub", pComponent);

	// CDynamicMesh
	pComponent = m_pMeshHead = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHead", pComponent);

	switch (iLength)
	{
	case 0:
	case 1:
		pComponent = m_pMeshHoze = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze"));
		m_iHeadPos = 2;
		break;
	case 2:
		pComponent = m_pMeshHoze = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze2"));
		break;
	case 3:
		pComponent = m_pMeshHoze = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze3"));
		break;
	case 4:
		pComponent = m_pMeshHoze = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze4"));
		m_iHeadPos = 2;
		break;
	default:
		break;
	}
	// CDynamicMesh
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHoze", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(tTemp.vPos);

	pComponent = m_pTransformHeadCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformHead", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	if (tTemp.eSceneID == RESOURCE_VF3)
		m_bBlow = false;

	if (m_bBlow)
		m_pBlow = CBlow::Create(m_pGraphicDev, pArg);

	//auto* pPhysics = Engine::Get_Physics();
	//_vec3 vPos = m_pTransformHeadCom->Get_Position();

	//PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	//PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	//
	//PxShape* pTrigger = pPhysics->createShape(PxBoxGeometry(1.5f, 1.5f, 1.5f), *pMaterial, true);

	//pTrigger->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	//pTrigger->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	//pTrigger->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	//m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pTrigger, 1);

	//auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	//pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//// 그래비티를 끈다.
	//pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//// 질량설정
	//pBody->setMass(0);
	////이동 제동력

	//pBody->setName((char*)this);
	//pBody->attachShape(*pTrigger);

	return S_OK;
}

HRESULT CMiniVacuum::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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



PxJoint* CMiniVacuum::createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
{
	//PxD6Joint* j = PxD6JointCreate(*Engine::Get_Physics(), a0, t0, a1, t1);
	//j->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	//j->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLIMITED);
	//j->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLIMITED);

	//j->setPyramidSwingLimit(PxJointLimitPyramid(D3DXToRadian(-30), D3DXToRadian(30), D3DXToRadian(-30), D3DXToRadian(30)));
	//j->setConstraintFlag(PxConstraintFlag::e);
	//j->setTwistLimit(PxJointAngularLimitPair(0.1, 0.1, -1.f));
	//j->setSwingLimit(PxJointLimitCone(D3DXToRadian(0.5f), D3DXToRadian(0.0), 0.f));
	//j->setDrive(PxD6Drive::eSLERP, PxD6JointDrive(0, 30, FLT_MAX, true));
	//j->setDistanceLimit(PxJointLinearLimit(1.0f, PxSpring(0.1f, 0.05f)));

	//PxSphericalJoint* j = PxSphericalJointCreate(*Engine::Get_Physics(), a0, t0, a1, t1);
	//j->setLimitCone(PxJointLimitCone(0, PxPi / 4, 0.05f));
	//j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
	PxFixedJoint* j = PxFixedJointCreate(*Engine::Get_Physics(), a0, t0, a1, t1);

	return j;
}

void CMiniVacuum::createChain(const PxTransform& t, PxU32 length, PxReal separation)
{
	PxVec3 offset;
	PxRigidDynamic* prev = NULL;
	PxBoxGeometry g;
	if (m_iDir == DIRUP || m_iDir == DIRDOWN)
	{
		if (m_iDir == DIRDOWN)
			separation *= -1;
		offset = PxVec3(0, 0, separation / 2);
		g = PxBoxGeometry(0.5f, 0.8f, 0.1f);
	}
	else
	{
		if (m_iDir == DIRLEFT)
			separation *= -1;
		offset = PxVec3(separation / 2, 0, 0);
		g = PxBoxGeometry(0.1f, 0.8f, 0.5f);
	}
	PxTransform localTm(offset);
	PxJoint* j = nullptr;
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*Engine::Get_Physics(), t*localTm, g, *Engine::Get_Physics()->createMaterial(0.5f, 0.5f, 0.f), 1.0f);
		m_pHoze->addActor(*current);
		//current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X /*| PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y*/ /*| PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z*/);
		//current->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		//current->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		//PxRigidBodyExt::setMassAndUpdateInertia(*current, 0);
		current->setMass(0.1f);
		if (i == 0)
		{
			//헤드부분에 연결
			j = PxFixedJointCreate(*Engine::Get_Physics(), prev, t, current, PxTransform(-offset));

		}
		else if (i == length - 1)
		{
			j = createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		}
		else
		{

			j = createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		}
		m_vecHozeRigid.emplace_back(current);
		prev = current;
		switch (m_iDir)
		{
		case DIRUP:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
			if (j->is<PxD6Joint>())
			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eX, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.z += separation;
			break;
		case DIRRIGHT:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
			if (j->is<PxD6Joint>())

			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.x += separation;
			break;
		case DIRLEFT:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
			if (j->is<PxD6Joint>())

			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.x += separation;
			break;
		case DIRDOWN:
			//if (i == length - 1)
			current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
			if (j->is<PxD6Joint>())
			{

				j->is<PxD6Joint>()->setMotion(PxD6Axis::eX, PxD6Motion::eLIMITED);
				j->is<PxD6Joint>()->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
			}
			localTm.p.z += separation;
			break;
		default:
			break;
		}
	}
}

std::vector<Engine::_vec3>* CMiniVacuum::Get_IOPath()
{
	return &m_vecIOPath;
}

void CMiniVacuum::Move_Right()
{
	m_vecHozeRigid.back()->addForce(m_vRight, PxForceMode::eIMPULSE);
}

void CMiniVacuum::Move_Left()
{
	m_vecHozeRigid.back()->addForce(-m_vRight, PxForceMode::eIMPULSE);
}

void CMiniVacuum::Move_Up()
{
	m_vecHozeRigid.back()->addForce(m_vUpForce, PxForceMode::eIMPULSE);
}


CMiniVacuum* CMiniVacuum::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iDir, void* pArg /*= nullptr*/, _uint iLength /*= 0*/)
{
	CMiniVacuum*	pInstance = new CMiniVacuum(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(iDir, iLength, pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMiniVacuum::Free(void)
{
	for (auto iter : m_vecHozeRigid)
		iter->release();
	m_pHoze->release();
	Safe_Release(m_pBlow);
	Engine::CGameObject::Free();
}
