#include "stdafx.h"
#include "Sheilder.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "Sheild.h"

#include "Export_Function.h"


//#Sound VacuumMine, ToyMage, ToySheilder��, Toy�� �´¼Ҹ� ?
CSheilder::CSheilder(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_fTimeDelta(0.f), m_bMove(false)
{
}

CSheilder::~CSheilder()
{
}

HRESULT CSheilder::Ready_Object(void * pArg, void* pOBJINFO)
{
	m_fColDis = 1.5f;

	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;
	FAILED_CHECK_RETURN(Add_Component(pOBJINFO), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		m_pSheild = (CSheild*)pArg;
		m_pSheild->AddRef();

		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	OBJINFO tObjINFO;
	ZeroMemory(&tObjINFO, sizeof(OBJINFO));
	if (nullptr != pOBJINFO)
	{
		memcpy(&tObjINFO, pOBJINFO, sizeof(OBJINFO));
	}


	m_pMeshCom->Set_Animationset(animID::Idle);

	Create_DefaultUI(2.f, 2.f);
	m_iHP = 10;
	m_iHPMax = 10;

	return S_OK;
}

_int CSheilder::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Update_DefaultUI(fTimeDelta);

	if (m_iHP <= 0)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		m_pMeshCom->Set_Animationset(Dead);
		m_pMeshCom->Play_Animationset(fTimeDelta);

		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_pSheild->Set_Dead();
			m_bDead = true;
		}

		if (m_bDead)
		{
			Dead_Effect();
			Delete_UI();
			m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
			return OBJ_DEAD;
		}

		return 0;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bHit)
	{
		Hit_State(fTimeDelta);
		m_bMove = true;
	}
	else if(m_bMove)
		Move(fTimeDelta);

	m_pMeshCom->Play_Animationset(fTimeDelta);
	Tick_Damage(fTimeDelta);
	CCollisionMgr::GetInstance()->Add_EnermyList(this);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	_vec3 vPos;

	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	return OBJ_NOEVENT;
}

_int CSheilder::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	m_pTransformCom->Set_Pos(pxTransform);


	return _int();
}

void CSheilder::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Spine2");
	_float4x4 matBoneMatrix = pBone->CombinedTranformationMatrix;

	//Spine2
	m_tColInfo.matWorld = matBoneMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);

	m_pSheild->Update_Object(m_fTimeDelta);
	m_pSheild->Render_Object();
}

void CSheilder::Hit_State(const _float & fTimeDelta)
{
	m_pMeshCom->Set_Animationset(Damage);

	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		m_bHit = false;

	m_pTargetPlayer = nullptr;
}

void CSheilder::Set_Damage(_int iDamage)
{
	CEnermy::Set_Damage(iDamage);

	Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 3.f);
	Hit_Damage(1.5f);
}

HRESULT CSheilder::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;


	OBJINFO tObjINFO;
	ZeroMemory(&tObjINFO, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tObjINFO, pArg, sizeof(OBJINFO));
	}

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToySheilder"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	tObjINFO.vPos.y += 2.f;
	m_pTransformCom->Set_Scale(tObjINFO.vScale);
	m_pTransformCom->Rotation(ROT_X, tObjINFO.vAngle.x);
	m_pTransformCom->Rotation(ROT_Y, tObjINFO.vAngle.y);
	m_pTransformCom->Rotation(ROT_Z, tObjINFO.vAngle.z);
	m_pTransformCom->Set_Pos(tObjINFO.vPos);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y + 0.5f, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *pMaterial, true);
	//�� �ΰ��� �ϳ��� false �Ѵ� true��?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// Ʈ�������� ������������ �������� �ʴ¾ֵ�
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	// �׷���Ƽ�� ����.

	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// ��������
	pBody->setMass(5);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//�̵� ������
	pBody->setLinearDamping(0.05f);
	return S_OK;

}

HRESULT CSheilder::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CSheilder::Move(const _float & fTimeDelta)
{
	Engine::CTransform*		pMayTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", Engine::ID_DYNAMIC));
	Engine::CTransform*		pCodyTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", Engine::ID_DYNAMIC));

	_vec3 vMayPos; //���� ��ġ
	_vec3 vCodyPos; //�ڵ� ��ġ
	_vec3 vPos; //���� ����(�ڽ�)	��ġ.
	_vec3 vLook; //���� ����(�ڽ�)	�����
	_vec3 vRight; //���� ����(�ڽ�) ����Ʈ
	_vec3 vLeft; //���� ����(�ڽ�) ����Ʈ

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	vMayPos = pMayTransformCom->Get_Position();
	vCodyPos = pCodyTransformCom->Get_Position();
	vPos = m_pTransformCom->Get_Position();

	_vec3 vMayDir;	//���̿��� �Ÿ�
	_vec3 vCodyDir; //�ڵ���� �Ÿ�

	_float fMayDir;
	_float fCodyDir;

	vMayDir = vMayPos - vPos;
	vCodyDir = vCodyPos - vPos;

	fMayDir = D3DXVec3Length(&vMayDir);
	fCodyDir = D3DXVec3Length(&vCodyDir);


	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	//�Ÿ��� ���߿� �ϳ��� 10.f ���� ������, �÷��̾� ����.
	if (fMayDir <= 10.f || fCodyDir <= 10.f)
	{
		if (fMayDir < fCodyDir && m_pTargetPlayer == nullptr)
		{
			m_pTargetPlayer = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"May"));
		}
		else if (fCodyDir < fMayDir && m_pTargetPlayer == nullptr)
		{
			m_pTargetPlayer = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"GameObject", L"Cody"));
		}
	}


	if (m_pTargetPlayer != nullptr)
	{

		//Ÿ���� Ʈ������ ������.
		CTransform*		pTargetTransCom = dynamic_cast<CTransform*>(m_pTargetPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));

		_vec3	vTargetPos;
		_vec3	vTargetDir;
		_float	 fTargetDir;


		vTargetPos = pTargetTransCom->Get_Position();
		vTargetDir = vTargetPos - vPos;

		fTargetDir = D3DXVec3Length(&vTargetDir);
		vTargetDir.y = 0.f;
		D3DXVec3Normalize(&vTargetDir, &vTargetDir);

		m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vTargetDir)));

		_float fRightAngle = D3DXVec3Dot(&vTargetDir, &vRight);
		_float fLeftAngle = D3DXVec3Dot(&vTargetDir, &vLeft);

		if (m_fAngle >= D3DXToDegree(m_fTurnSpeed))
		{
			if (fRightAngle < 0 && fLeftAngle > 0)
			{
				m_pTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
			}
			else if (fRightAngle > 0 && fLeftAngle < 0)
				m_pTransformCom->Rotation(ROT_Y, m_fTurnSpeed);
		}

		if (fTargetDir >= 2.f) //�־����� �߰�.
		{
			m_pMeshCom->Set_Animationset(animID::Walk);
			m_pTransformCom->Move_Pos(&vLook, 2.f, fTimeDelta);
		}
		else if (fTargetDir <= 2.f) //��������� ����
		{
			m_pMeshCom->Set_Animationset(animID::Attack);
			m_pSheild->Set_AttAble(true);

			//Engine::StopSound(CSoundMgr::CHANNELID::ToyKnight);
			//Engine::PlaySoundW(L"ToyKnightDeath1.ogg", CSoundMgr::CHANNELID::ToyKnight);

			if (true == m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{

			}

		}

		if (fTargetDir >= 11.f) //�Ÿ��� �־����� Ÿ���� ������� �ٽ� ���̵� ����
		{
			m_pMeshCom->Set_Animationset(animID::Idle);
			m_pTargetPlayer = nullptr;
		}


	}
}

CSheilder * CSheilder::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg,void* pOBJINFO)
{
	CSheilder*	pInstance = new CSheilder(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pOBJINFO)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSheilder::Free(void)
{
	Safe_Release(m_pSheild);
	CEnermy::Free();
}
