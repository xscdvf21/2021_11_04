#include "stdafx.h"
#include "VacuumFieldPart5.h"
#include "Export_Function.h"
#include "Blow.h"


CVacuumFieldPart5::CVacuumFieldPart5(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CVacuumFieldPart5::~CVacuumFieldPart5()
{
}

HRESULT CVacuumFieldPart5::Ready_Object(void* pArg)
{
	return S_OK;
}

HRESULT CVacuumFieldPart5::Ready_Object(_uint iDir, _uint iLength, void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg, iLength), E_FAIL);
	Set_Rotation(iDir);
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform t = PxTransform(vPos.x - 8.f, vPos.y, vPos.z);
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

_int CVacuumFieldPart5::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_bStart)
	{
		_vec3 vEnterTriggerPos = m_pTransformHeadCom->Get_Position();
		_vec3 vEnterBox = {};
		_vec3 vHeadTriggerPos = m_pTransformHeadCom->Get_Position();
		_vec3 vHeadBox = {};
		switch (m_iDir)
		{
		case DIRUP:
			vEnterTriggerPos += {0.f, 0.f, 6.f};
			vEnterBox = { 1.5f,1.f,4.f };
			vHeadBox = { 1.5f,1.f,2.f };
			break;
		case DIRRIGHT:
			vEnterTriggerPos += {6.f, 0.f, 0.f};
			vEnterBox = { 4.f,1.f,1.5f };
			vHeadBox = { 2.f,1.f,1.5f };
			break;
		case DIRDOWN:
			vEnterTriggerPos -= {0.f, 0.f, 6.f};
			vEnterBox = { 1.5f,1.f,4.f };
			vHeadBox = { 1.5f,1.f,2.f };
			break;
		case DIRLEFT:
			vEnterTriggerPos -= {6.f, 0.f, 0.f};
			vEnterBox = { 4.f,1.f,1.5f };
			vHeadBox = { 2.f,1.f,1.5f };
			break;
		default:
			break;
		}
		m_pEnterTrigger = CTrigger::CreateBoxTrigger(m_pScene, (char*)this, vEnterTriggerPos, vEnterBox.x, vEnterBox.y, vEnterBox.z, TRIGGERTYPE::TRIGGER_VACUUM_ENTER, FilterGroup::eHoze);
		m_pEnterTrigger->Set_Interactable(m_bSuck);
		m_pEnterTrigger->Set_FloatValue(150.f);
		m_pEnterTrigger->Set_FloatValue(2);
		m_pEnterTrigger->Set_FloatValue(1.f);
		m_pHeadTrigger = CTrigger::CreateBoxTrigger(m_pScene, (char*)this, m_pTransformHeadCom->Get_Position(), vHeadBox.x, vHeadBox.y, vHeadBox.z, TRIGGERTYPE::TRIGGER_VACUUM_RIDE, FilterGroup::eHoze);

		m_bStart = false;
		auto* pPhys = Engine::Get_Physics();

		_vec3 vPos = m_pTransformCom->Get_Position();
		PxTransform t = PxTransform(vPos.x - 8.f, vPos.y, vPos.z);
		t.p += PxVec3{ 4,-1.0f,0.f };
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

	if (m_pHeadTrigger)
	{
		m_pHeadTrigger->Set_Pos(HozeEndPos.p + PxVec3(0.f, 0.f, 0.f));
		m_pHeadTrigger->Set_Rotation(HozeEndPos.q);
	}
	if (m_pEnterTrigger)
	{

		m_pEnterTrigger->Set_Pos(HozeEndPos.p + PxVec3(6.f, 0.f, 0.f));
		m_pEnterTrigger->Set_Rotation(HozeEndPos.q);

	}
	HozeEndPos.q *= PxQuat(D3DXToRadian(90 * m_iDir), PxVec3(0.f, 1.f, 0.f));
	m_pTransformHeadCom->Set_WorldMatrix(HozeEndPos);

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
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuumFieldPart5::LateUpdate_Object(const _float & fTimeDelta)
{
	//_vec3 vPos;
	//vPos = m_pTransformCom->Get_Position();
	//m_pRigidActor->setGlobalPose({ vPos.x,vPos.y,vPos.z });
	if (m_bActive)
		m_pBlow->LateUpdate_Object(fTimeDelta);

	return _int();
}

void CVacuumFieldPart5::Render_Object(const _int & iIndex)
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

void* CVacuumFieldPart5::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	if (!m_bActive)
		return nullptr;

	if (m_pParent)
		return static_cast<CGameObject*>(m_pParent)->On_Trigger_Active(pTrigger,pCaller);
	m_vecOut.clear();
	m_vecOut.emplace_back(&m_vecIOPath);
	return &m_vecOut;
}

Engine::_mat CVacuumFieldPart5::Get_EventPos()
{
	_mat mat;
	memcpy(&mat, &PxMat44(m_vecHozeRigid.back()->getGlobalPose()), sizeof(_mat));
	return mat;
}

HRESULT CVacuumFieldPart5::Add_Component(void* pArg, _uint iLength)
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

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_pBlow = CBlow::Create(m_pGraphicDev, pArg);

	return S_OK;
}

HRESULT CVacuumFieldPart5::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
void CVacuumFieldPart5::Set_Rotation(const _int& iDir)
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
void CVacuumFieldPart5::MoveDown(const _float & fTimeDelta)
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

void CVacuumFieldPart5::MoveUp(const _float & fTimeDelta)
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
void CVacuumFieldPart5::MoveUpFront(const _float & fTimeDelta)
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

void CVacuumFieldPart5::MoveDownFront(const _float & fTimeDelta)
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


CVacuumFieldPart5* CVacuumFieldPart5::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iDir, _uint iLength /*=0*/, void* pArg /*=nullptr*/, void* Parent /*= nullptr*/)
{
	CVacuumFieldPart5*	pInstance = new CVacuumFieldPart5(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iDir, iLength, pArg)))
	{
		Safe_Release(pInstance);
	}
	pInstance->Set_Parent(Parent);
	return pInstance;
}

void CVacuumFieldPart5::Free(void)
{
	for (auto iter : m_vecHozeRigid)
		iter->release();
	m_pHoze->release();
	Safe_Release(m_pBlow);
	Engine::CGameObject::Free();
}

physx::PxJoint* CVacuumFieldPart5::createDampedD6(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1)
{
	PxFixedJoint* j = PxFixedJointCreate(*Engine::Get_Physics(), a0, t0, a1, t1);
	return j;
}

void CVacuumFieldPart5::createChain(const PxTransform& t, PxU32 length, PxReal separation)
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

void CVacuumFieldPart5::Set_MoveUpCheck(_bool bMove)
{
	//for (auto& pHoze : m_vecHozeRigid)
	//{
	//	pHoze->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	//}
	m_bMoveUp = bMove;
}

void CVacuumFieldPart5::Set_MoveUpCheckFront(_bool bMove)
{
	m_bMoveUpFront = bMove;
}

void CVacuumFieldPart5::Set_MoveDownCheck(_bool bMove)
{
	//for (auto& pHoze : m_vecHozeRigid)
	//{
	//	pHoze->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	//}
	m_bMoveDown = bMove;
}

void CVacuumFieldPart5::Set_MoveDownCheckFront(_bool bMove)
{
	m_bMoveDownFront = bMove;

}

void CVacuumFieldPart5::Set_Suck(_bool bState)
{
	m_bSuck = bState;
	if (m_pEnterTrigger)
		m_pEnterTrigger->Set_Interactable(m_bSuck);
}

std::vector<Engine::_vec3>* CVacuumFieldPart5::Get_IOPath()
{
	return &m_vecIOPath;
}
void CVacuumFieldPart5::Move_Right()
{
	if (!(m_iMoveLock & LOCK_RIGHT))
		m_vecHozeRigid.back()->addForce(m_vRight, PxForceMode::eIMPULSE);
}

void CVacuumFieldPart5::Move_Left()
{
	if (!(m_iMoveLock & LOCK_LEFT))
		m_vecHozeRigid.back()->addForce(-m_vRight, PxForceMode::eIMPULSE);
}

void CVacuumFieldPart5::Move_Up()
{
	if (!(m_iMoveLock & LOCK_UP))
		m_vecHozeRigid.back()->addForce(PxVec3{ 0.f,1.f,0.f }, PxForceMode::eIMPULSE);
}
