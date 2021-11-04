#include "stdafx.h"
#include "VacuumHeadHoze.h"
#include "Export_Function.h"
#include "Blow.h"


CVacuumHeadHoze::CVacuumHeadHoze(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CVacuumHeadHoze::~CVacuumHeadHoze()
{
}

HRESULT CVacuumHeadHoze::Ready_Object(void* pArg)
{
	return S_OK;
}

HRESULT CVacuumHeadHoze::Ready_Object(_uint iDir, _uint iLength, _float fPower, _float fDuration, void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg, iLength), E_FAIL);
	Set_Rotation(iDir);
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Update_Component(0.f);

	m_fPower = fPower;
	m_fDuration = fDuration;
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform t = PxTransform(vPos.x, vPos.y, vPos.z);
	_int iCount = m_pMeshCom->Get_FrameCount(0);
	auto* pPhys = Engine::Get_Physics();
	m_pHoze = pPhys->createAggregate(iCount, false);
	//0.6 //1칸
	createChain(t, iCount, 0.6f);

	auto* EndHoze = m_vecHozeRigid.back();

	auto HozeEndPos = m_vecHozeRigid.back()->getGlobalPose();
	EndHoze->setMaxLinearVelocity(10.f);
	m_pTransformHeadCom->Set_Pos(HozeEndPos);
	m_pTransformHeadCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	//m_pPlane->
	return S_OK;
}

_int CVacuumHeadHoze::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_bStart)
	{
		_vec3 vEnterTriggerPos = m_pTransformHeadCom->Get_Position();
		_vec3 vEnterBox = { 1.f,1.f,1.f };
		_vec3 vHeadTriggerPos = m_pTransformHeadCom->Get_Position();
		_vec3 vHeadBox = { 1.f,1.f,1.f };
		switch (m_iDir)
		{
		case DIRUP:
			vEnterTriggerPos += {0.f, 0.f, 6.f};
			vEnterBox = { 1.5f,1.f,3.f };
			vHeadBox = { 1.5f,1.f,2.f };
			break;
		case DIRRIGHT:
			vEnterTriggerPos += {6.f, 0.f, 0.f};
			vEnterBox = { 3.f,1.f,1.5f };
			vHeadBox = { 2.f,1.f,1.5f };
			break;
		case DIRDOWN:
			vEnterTriggerPos -= {0.f, 0.f, 6.f};
			vEnterBox = { 1.5f,1.f,3.f };
			vHeadBox = { 1.5f,1.f,2.f };
			break;
		case DIRLEFT:
			vEnterTriggerPos -= {6.f, 0.f, 0.f};
			vEnterBox = { 3.f,1.f,1.5f };
			vHeadBox = { 2.f,1.f,1.5f };
			break;
		default:
			break;
		}
		m_pEnterTrigger = CTrigger::CreateBoxTrigger(m_pScene, (char*)this, vEnterTriggerPos, vEnterBox.x, vEnterBox.y, vEnterBox.z, TRIGGERTYPE::TRIGGER_VACUUM_ENTER, FilterGroup::eHoze);
		m_pEnterTrigger->Set_FloatValue(m_fPower);
		m_pEnterTrigger->Set_FloatValue(m_fDuration);
		m_pEnterTrigger->Set_FloatValue(1.f);
		m_pEnterTrigger->Set_Interactable(m_bSuck);
		m_pHeadTrigger = CTrigger::CreateBoxTrigger(m_pScene, (char*)this, m_pTransformHeadCom->Get_Position(), vHeadBox.x, vHeadBox.y, vHeadBox.z, TRIGGERTYPE::TRIGGER_VACUUM_RIDE, FilterGroup::eHoze);

		m_bStart = false;
		auto* pPhys = Engine::Get_Physics();

		_vec3 vPos = m_pTransformCom->Get_Position();
		PxTransform t = PxTransform(vPos.x, vPos.y, vPos.z);
		t.p += PxVec3{ 4,-0.5f,0.f };
		m_pPlane = pPhys->createRigidDynamic(t);
		PxShape* pShape = pPhys->createShape(PxBoxGeometry(4.f, 0.01f, 1.3f), *pPhys->createMaterial(0.5f, 0.5f, 0.f), true);
		m_pPlane->attachShape(*pShape);
		m_pPlane->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		m_pScene->addActor(*m_pPlane);
	}

	m_vecIOPath.clear();
	_vec3 vPos;
	if (!m_bSuck)
	{
		for (size_t i = 0; i < m_vecHozeRigid.size(); ++i)
		{
			vPos = { m_vecHozeRigid[i]->getGlobalPose().p.x, m_vecHozeRigid[i]->getGlobalPose().p.y, m_vecHozeRigid[i]->getGlobalPose().p.z };
			m_vecIOPath.emplace_back(vPos);
		}
	}
	else
	{



		for (size_t i = m_vecHozeRigid.size() - 1; i > 0; --i)
		{
			vPos = { m_vecHozeRigid[i]->getGlobalPose().p.x, m_vecHozeRigid[i]->getGlobalPose().p.y, m_vecHozeRigid[i]->getGlobalPose().p.z };
			m_vecIOPath.emplace_back(vPos);
		}
	}

	m_vecHozeRigid.back()->addForce(m_vDirForce, PxForceMode::eIMPULSE);

	Engine::CGameObject::Update_Object(fTimeDelta);
	_vec3 vHeadPos = m_pTransformHeadCom->Get_Position();
	auto HozeEndPos = m_vecHozeRigid.back()->getGlobalPose();
	HozeEndPos.q *= PxQuat(D3DXToRadian(90 * m_iDir), PxVec3(0.f, 1.f, 0.f));

	m_pTransformHeadCom->Set_WorldMatrix(HozeEndPos);
	if (m_pHeadTrigger)
	{
		m_pEnterTrigger->Set_Rotation(m_pTransformHeadCom->Get_Angle());
		m_pHeadTrigger->Set_Pos(HozeEndPos.p + PxVec3(0.f, 0.f, 0.f));
	}
	if (m_pEnterTrigger)
	{
		switch (m_iDir)
		{
		case DIRUP:
			m_pEnterTrigger->Set_Pos(HozeEndPos.p + PxVec3(0.f, 0.f, 6.f));

			break;
		case DIRRIGHT:
			m_pEnterTrigger->Set_Pos(HozeEndPos.p + PxVec3(6.f, 0.f, 0.f));
			break;

		case DIRDOWN:
			m_pEnterTrigger->Set_Pos(HozeEndPos.p + PxVec3(0.f, 0.f, -6.f));
			break;
		case DIRLEFT:
			m_pEnterTrigger->Set_Pos(HozeEndPos.p + PxVec3(-6.f, 0.f, 0.f));
			break;
		default:
			break;
		}
		m_pEnterTrigger->Set_Rotation(m_pTransformHeadCom->Get_Angle());
	}

	m_pMeshHead->Play_Animationset(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pBlow->Update_Object(fTimeDelta);
	auto* pFrame = m_pMeshHead->Get_FrameByName("SucktionTubeBase");
	m_pBlow->Set_Parent((pFrame->CombinedTranformationMatrix**m_pTransformHeadCom->Get_WorldMatrix()));

	if (m_bMoveUp)
		MoveUp(fTimeDelta);
	if (m_bMoveUpFront)
		MoveUpFront(fTimeDelta);
	if (m_bMoveDown)
		MoveDown(fTimeDelta);
	if (m_bMoveDownFront)
		MoveDownFront(fTimeDelta);

	if (Engine::Key_Pressing(DIK_G))
	{
		Move_Up();
	}
	if (Engine::Key_Pressing(DIK_F))
	{
		Move_Left();
	}
	if (Engine::Key_Pressing(DIK_H))
	{
		Move_Right();
	}

	return OBJ_NOEVENT;
}

_int CVacuumHeadHoze::LateUpdate_Object(const _float & fTimeDelta)
{
	//_vec3 vPos;
	//vPos = m_pTransformCom->Get_Position();
	//m_pRigidActor->setGlobalPose({ vPos.x,vPos.y,vPos.z });
	if (m_bSuck)
	{
		m_pBlow->LateUpdate_Object(fTimeDelta);

		PARTICLE tParticle;
		_float fRand = 0.f;
		if (CRandoms()(0, 1) == 1)
			fRand = 0.f;
		else
			fRand = 0.5f;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 2, 2, fRand);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.15f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 30.f;

		if (CRandoms()(0, 1) == 1)
			tParticle.fAmount = 0.f;
		else
			tParticle.fAmount = 0.5f;

		_vec3 vLook;
		m_pTransformHeadCom->Get_Info(INFO_LOOK, &vLook);

		tParticle.vPos = ToVec3(m_vecHozeRigid.back()->getGlobalPose().p) + _vec3(CRandoms()(-2.f, 2.f), CRandoms()(-2.f, 2.f), CRandoms()(-2.f, 2.f)) + (vLook * (800.f + CRandoms()(0.f, 200.f)));
		_vec3 vDir = ToVec3(m_vecHozeRigid.back()->getGlobalPose().p) - tParticle.vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		tParticle.vDir = vDir;
		tParticle.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
		tParticle.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
		tParticle.vStartColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 250.f };
		tParticle.vEndColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 150.f };

		tParticle.bBill = false;
		tParticle.vRot = { CRandoms()(0.f, 360.f), CRandoms()(0.f, 360.f), CRandoms()(0.f, 360.f) };

		Engine::Emit_ImageParticle(tParticle, 43, 23);
	}
	m_fEffectTime += fTimeDelta;

	Effect(fTimeDelta);

	return _int();
}

void CVacuumHeadHoze::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	_matrix			matWorld, matView, matProj;

	D3DXMatrixScaling(&matWorld, 0.01f, 0.01f, 0.01f);
	ZeroMemory(&matWorld._41, sizeof(_vec3));
	D3DXMatrixRotationY(&matView, 90);
	//matWorld = matWorld*matView;
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->CommitChanges();

	m_pMeshCom->Render_PhysMeshes(pEffect, 4, m_vecHozeRigid, m_iDir);

	m_pTransformHeadCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	m_pMeshHead->Render_Meshes_VTF(pEffect, 0);

	pEffect->End();

	Safe_Release(pEffect);
}

void* CVacuumHeadHoze::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	if (m_pParent)
		return static_cast<CGameObject*>(m_pParent)->On_Trigger_Active(pTrigger, pCaller);
	return &m_vecIOPath;
}

Engine::_mat CVacuumHeadHoze::Get_EventPos()
{
	_mat mat;
	memcpy(&mat, &PxMat44(m_vecHozeRigid.back()->getGlobalPose()), sizeof(_mat));
	return mat;
}

HRESULT CVacuumHeadHoze::Add_Component(void* pArg, _uint iLength)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	switch (iLength)
	{
	case 0:
	case 1:
		pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze"));
		break;
	case 2:
		pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze2"));
		break;
	case 3:
		pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze3"));
		break;
	case 4:
		pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHoze4"));
		break;
	default:
		break;
	}
	// CDynamicMesh
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHoze", pComponent);
	// CDynamicMesh
	pComponent = m_pMeshHead = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshHead", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTransformHeadCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformHead", pComponent);

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

	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_pBlow = CBlow::Create(m_pGraphicDev, pArg);

	return S_OK;
}

HRESULT CVacuumHeadHoze::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	pEffect->CommitChanges();

	return S_OK;
}
void CVacuumHeadHoze::Set_Rotation(const _int& iDir)
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
void CVacuumHeadHoze::MoveDown(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);
	m_bActive = false;

	if (vPos.y > -1.5f)
	{
		_float fSpeed = -2.f;
		m_pTransformCom->Move_Pos(&vUp, fSpeed, fTimeDelta);
		for (size_t i = 0; i < m_vecHozeRigid.size(); ++i)
		{
			auto tPos = m_vecHozeRigid[i]->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_vecHozeRigid[i]->setGlobalPose(tPos);

			tPos = m_pPlane->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_pPlane->setKinematicTarget(tPos);

			if (i == 0)
			{
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR0, m_vecHozeRigid[0]->getGlobalPose());
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR1, PxTransform(PxVec3(0, 0, 0)));
				//m_vecHozeJoint[i]->setLocalPose(PxJointActorIndex::eACTOR1, m_vecHozeRigid[i]->getGlobalPose());
				//PxVec3(-0.6f/ 2, 0, 0);
			}
		}
	}
	else
	{
		m_bMoveDown = false;
	}
}

void CVacuumHeadHoze::MoveUp(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);

	if (vPos.y < 9.7f)
	{
		_float fSpeed = 2.f;
		m_pTransformCom->Move_Pos(&vUp, fSpeed, fTimeDelta);
		for (size_t i = 0; i < m_vecHozeRigid.size(); ++i)
		{
			auto tPos = m_vecHozeRigid[i]->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_vecHozeRigid[i]->setGlobalPose(tPos);
			m_vecHozeRigid[i]->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

			tPos = m_pPlane->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_pPlane->setKinematicTarget(tPos);
			if (i == 0)
			{
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR0, m_vecHozeRigid[0]->getGlobalPose());
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR1, PxTransform(PxVec3(0, 0, 0)));
			}
		}
	}
	else
	{
		for (auto& pHoze : m_vecHozeRigid)
		{
			pHoze->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
		}
		m_bActive = true;
		m_bMoveUp = false;
	}



}
void CVacuumHeadHoze::MoveUpFront(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);

	if (vPos.y < 1.f)
	{
		_float fSpeed = 1.4f;
		m_pTransformCom->Move_Pos(&vUp, fSpeed, fTimeDelta);
		for (size_t i = 0; i < m_vecHozeRigid.size(); ++i)
		{
			auto tPos = m_vecHozeRigid[i]->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_vecHozeRigid[i]->setGlobalPose(tPos);
			m_vecHozeRigid[i]->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

			tPos = m_pPlane->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_pPlane->setKinematicTarget(tPos);


			if (i == 0)
			{
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR0, m_vecHozeRigid[0]->getGlobalPose());
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR1, PxTransform(PxVec3(0, 0, 0)));
			}


		}
	}
	else
	{
		for (auto& pHoze : m_vecHozeRigid)
		{
			pHoze->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
		}
		m_bActive = true;
		m_bMoveUpFront = false;
	}



}

void CVacuumHeadHoze::MoveDownFront(const _float & fTimeDelta)
{
	_vec3 vPos;
	_vec3 vUp;
	vPos = m_pTransformCom->Get_Position();
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);
	m_bActive = false;

	if (vPos.y > -1.5f)
	{
		_float fSpeed = -2.f;
		m_pTransformCom->Move_Pos(&vUp, fSpeed, fTimeDelta);
		for (size_t i = 0; i < m_vecHozeRigid.size(); ++i)
		{
			auto tPos = m_vecHozeRigid[i]->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_vecHozeRigid[i]->setGlobalPose(tPos);

			tPos = m_pPlane->getGlobalPose();
			tPos.p += PxVec3{ 0.f, 1.f, 0.f }*fSpeed*fTimeDelta;
			m_pPlane->setKinematicTarget(tPos);
			//m_vecHozeRigid[i]->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

			if (i == 0)
			{
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR0, m_vecHozeRigid[0]->getGlobalPose());
				m_vecHozeJoint[0]->setLocalPose(PxJointActorIndex::eACTOR1, PxTransform(PxVec3(0, 0, 0)));
			}
		}
	}
	else
	{

		m_bMoveDownFront = false;
	}

}


void CVacuumHeadHoze::Effect(const _float& fTimeDelta)
{
	if (m_bSuck)
		return;

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
}

CVacuumHeadHoze* CVacuumHeadHoze::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iDir, _uint iLength /*= 0*/, _float fPower /*= 80.f */, _float fDuration/*=2.f*/, void* pArg /*= nullptr*/, void* Parent /*= nullptr*/)
{
	CVacuumHeadHoze*	pInstance = new CVacuumHeadHoze(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iDir, iLength, fPower, fDuration, pArg)))
	{
		Safe_Release(pInstance);
	}
	pInstance->Set_Parent(Parent);
	return pInstance;
}

void CVacuumHeadHoze::Free(void)
{
	for (auto iter : m_vecHozeRigid)
		iter->release();
	m_pHoze->release();
	Safe_Release(m_pBlow);
	Engine::CGameObject::Free();
}

physx::PxJoint* CVacuumHeadHoze::createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
{
	PxFixedJoint* j = PxFixedJointCreate(*Engine::Get_Physics(), a0, t0, a1, t1);
	return j;
}

void CVacuumHeadHoze::createChain(const PxTransform& t, PxU32 length, PxReal separation)
{
	PxVec3 offset;
	PxRigidDynamic* prev = NULL;
	PxBoxGeometry g;
	if (m_iDir == DIRUP || m_iDir == DIRDOWN)
	{
		if (m_iDir == DIRDOWN)
			separation *= -1;
		offset = PxVec3(0, 0, separation / 2);
		g = PxBoxGeometry(0.5f, 0.5f, 0.1f);
	}
	else
	{
		if (m_iDir == DIRLEFT)
			separation *= -1;
		offset = PxVec3(separation / 2, 0, 0);
		g = PxBoxGeometry(0.1f, 0.5f, 0.5f);
	}
	PxTransform localTm(offset);
	PxJoint* j = nullptr;
	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*Engine::Get_Physics(), t*localTm, g, *Engine::Get_Physics()->createMaterial(0.5f, 0.5f, 0.f), 1.0f);
		if (i == 0)
		{

			//current->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
			//current->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		}
		m_pHoze->addActor(*current);

		current->setMass(0.1f);
		//if (i == 0)
		//{
		//	//헤드부분에 연결
		//	j = PxFixedJointCreate(*Engine::Get_Physics(), prev, t, current, PxTransform(-offset));
		//}
		//else if (i == length - 1)
		//{
		//	j = createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		//}
		//else
		//{
		j = createDampedD6(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		//}
		m_vecHozeJoint.emplace_back(j);
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

void CVacuumHeadHoze::Set_MoveUpCheck(_bool bMove)
{
	//for (auto& pHoze : m_vecHozeRigid)
	//{
	//	pHoze->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	//}
	m_bMoveUp = bMove;
}

void CVacuumHeadHoze::Set_MoveUpCheckFront(_bool bMove)
{
	m_bMoveUpFront = bMove;
}

void CVacuumHeadHoze::Set_MoveDownCheck(_bool bMove)
{
	//for (auto& pHoze : m_vecHozeRigid)
	//{
	//	pHoze->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	//}
	m_bMoveDown = bMove;
}

void CVacuumHeadHoze::Set_MoveDownCheckFront(_bool bMove)
{
	m_bMoveDownFront = bMove;

}

void CVacuumHeadHoze::Set_Suck(_bool bState)
{
	m_bSuck = bState;
	if (m_pEnterTrigger)
		m_pEnterTrigger->Set_Interactable(m_bSuck);
}

std::vector<Engine::_vec3>* CVacuumHeadHoze::Get_IOPath()
{
	return &m_vecIOPath;
}
void CVacuumHeadHoze::Move_Right()
{
	if (!(m_iMoveLock & LOCK_RIGHT))
		m_vecHozeRigid.back()->addForce(m_vRight, PxForceMode::eIMPULSE);
}

void CVacuumHeadHoze::Move_Left()
{
	if (!(m_iMoveLock & LOCK_LEFT))
		m_vecHozeRigid.back()->addForce(-m_vRight, PxForceMode::eIMPULSE);

}

void CVacuumHeadHoze::Move_Up()
{
	if (!(m_iMoveLock & LOCK_UP))
		m_vecHozeRigid.back()->addForce(m_vUpForce, PxForceMode::eIMPULSE);
}
