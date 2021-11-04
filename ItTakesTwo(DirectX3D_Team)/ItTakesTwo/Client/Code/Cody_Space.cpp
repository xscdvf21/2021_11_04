#include "stdafx.h"
#include "Cody_Space.h"
#include "Export_Function.h"
#include "CodyCamera.h"
#include "FuseNPC.h"
#include "MiniVacuum.h"
#include "MainCamera.h"
#include "May.h"
#include "ImageUI.h"
#include "Lever.h"
#include "Fan.h"
#include "PowerButton.h"
#include "Dumbbell.h"
#include "HangLever.h"
#include "BoundingBox.h"
#include "GeneratorBattery.h"
#include "ValveHandle.h"
#include "SpaceWall.h"
#include "SpaceValve.h"
#include "SpaceLever.h"
#include "may_space.h"
#include "BaboonMissile.h"


void CCody_Space::Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody)
{
	pActor->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);

	// 그래비티를 끈다.
	//pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	m_pController->setStepOffset(0);

	// 질량설정
	pActor->setMass(5);
	pActor->setName((char*)this);
	m_pRigidBody->setName((char*)this);
	m_pController->setContactOffset(0.001f);
	m_pController->setSlopeLimit(D3DXToRadian(50.f));
	m_pController->setNonWalkableMode(PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING);
}

physx::PxControllerBehaviorFlags CCody_Space::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	PxControllerBehaviorFlags flags;
	if (shape.getSimulationFilterData().word0 & (FilterGroup::eRideObj))
	{
		flags = PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	if (shape.getSimulationFilterData().word0 & (FilterGroup::eBigFan))
	{
		CGameObject* pGameObjectt = (CGameObject*)actor.getName();
		m_pAttachTarget = pGameObjectt;
		m_bRideObj = true;
	}
	else if (shape.getSimulationFilterData().word0 & FilterGroup::eStone)
	{
		CGameObject* pGameObjectt = (CGameObject*)actor.getName();
		m_pAttachTarget = pGameObjectt;
		m_bAttach = true;
	}

	else if (shape.getSimulationFilterData().word0 &FilterGroup::eOgre)
	{
		CGameObject* pGameObjectt = (CGameObject*)actor.getName();
		m_pAttachTarget = pGameObjectt;
	}

	return flags;
}

physx::PxControllerBehaviorFlags CCody_Space::getBehaviorFlags(const PxController& controller)
{
	OutputDebugString(L"컨트롤러랑 충돌\n");

	PxControllerBehaviorFlags flags;
	flags = PxControllerBehaviorFlag::eCCT_SLIDE;
	return flags;
}

physx::PxControllerBehaviorFlags CCody_Space::getBehaviorFlags(const PxObstacle& obstacle)
{
	OutputDebugString(L"옵스타클이랑 충돌\n");
	PxControllerBehaviorFlags flags;

	return flags;
}

void CCody_Space::onShapeHit(const PxControllerShapeHit& hit)
{

	if (m_bJump[PL_GROUNDPOUND] && hit.shape->getSimulationFilterData().word0 & (FilterGroup::eButton | FilterGroup::eCrusher))
	{
		ToObj<CGameObject>(hit.actor->getName())->On_Active(this);
	}

	if (hit.shape->getSimulationFilterData().word0 & (FilterGroup::eOgre))
	{
		ToObj<CGameObject>(hit.actor->getName())->On_Active(this);
	}


	if (hit.shape->getSimulationFilterData().word0 & FilterGroup::eWall &&hit.worldNormal.y > 0.99)
	{

		m_pController->setFootPosition(hit.worldPos);
	}
	if (hit.shape->getSimulationFilterData().word0 & FilterGroup::eWall && (hit.worldNormal.y < 0.09) && m_iSize != 2)
	{
		if (m_fJumpCurTime < m_fJumpCoolTime)
			return;
		m_fJumpCurTime = 0.f;
		//벽에 붙음
		if (hit.shape->getTriangleMeshGeometry(hit.shape->getGeometry().triangleMesh()))
		{

			const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
			PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
			PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

			PxExtendedVec3 vPos = m_pController->getPosition();
			vPos.y += m_fHalfHeight;

			if (!m_pScene->raycast(PxVec3((_float)vPos.x, (_float)vPos.y, (_float)vPos.z),
				-PxVec3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z),
				4.F,
				buf))
			{
				vPos.y -= 0.1f;
				if (m_pScene->raycast(PxVec3((_float)vPos.x, (_float)vPos.y, (_float)vPos.z),
					-PxVec3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z),
					2.F,
					buf))
				{
					if ((PL_JUMP == m_ePL_STATE || PL_CLIMBING == m_ePL_STATE) && !m_bGround)
					{
						m_bHang = true;
						m_vClimbingDir = -_float3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z);	
					}
				}
			}
			else
			{
				if (PL_JUMP == m_ePL_STATE && !m_bGround)
				{
					m_ePL_STATE = PL_CLIMBING;
					m_ePre_State = PL_JUMP;
					Clear(PL_JUMP);

					m_vClimbingDir = -_float3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z);
					return;
				}
				if (PL_CLIMBING == m_ePL_STATE)
				{
					m_vClimbingDir = -_float3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z);
					m_bClimbing = true;
				}
			}
		}
	}
	return;
}

void CCody_Space::onControllerHit(const PxControllersHit& hit)
{
	OutputDebugString(L"다른 컨트롤러랑 충돌\n");
	return;
}

void CCody_Space::onObstacleHit(const PxControllerObstacleHit& hit)
{
	OutputDebugString(L"옵스타클이랑 충돌\n");
	return;
}

bool CCody_Space::filter(const PxController& a, const PxController& b)
{
	PxShape* pShape1;
	PxShape* pShape2;
	pShape1 = nullptr;
	pShape2 = nullptr;
	a.getActor()->getShapes(&pShape1, 1);
	b.getActor()->getShapes(&pShape2, 1);
	if ((pShape1->getSimulationFilterData().word0 | pShape2->getSimulationFilterData().word0) & (FilterGroup::eInteractItem | FilterGroup::eCody))
		return false;

	return true;
}

physx::PxQueryHitType::Enum CCody_Space::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	return PxQueryHitType::eBLOCK;

}

physx::PxQueryHitType::Enum CCody_Space::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	return PxQueryHitType::eBLOCK;

}

void CCody_Space::Set_ChangeAble(const _bool& bState)
{
	m_bChangeAble = bState;
}

void CCody_Space::SpaceField3_Clear_SetPos(const _float & fTimeDelta)
{
	//m_pController->setUpDirection(PxVec3(-1.f, 0.f, 0.f));
	//CCodyCamera* pCamera = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");
	//pCamera->Set_TargetUp(ToVec3(m_pController->getUpDirection()));
	m_ePre_State = m_ePre_State;
	m_ePL_STATE = PL_IDLE;
	Clear(PL_DEAD);
	InteractionSetPos(_vec3(5.f, 8.f, 1.f), fTimeDelta);
}

void CCody_Space::SpaceField2_Clear_SetPos(const _float & fTimeDelta)
{
	m_ePre_State = m_ePre_State;
	m_ePL_STATE = PL_IDLE;
	Clear(PL_DEAD);
	InteractionSetPos(_vec3(4.f, 8.f, 1.f), fTimeDelta);
	m_fZ = -1.516f;
}

Engine::_mat CCody_Space::Get_EventPos()
{
	return *m_pTransformCom->Get_WorldMatrix();
}

void CCody_Space::Clear_Jump()
{
	CPlayerValue::Clear_Jump();

	switch (m_iSize)
	{
	case 0:
		m_fJumpPower = 0.13f;
		m_fEventJumpPower = 0.13f;
		break;
	case 1:
	case 2:
		m_fJumpPower = 1.3f;
		m_fEventJumpPower = 1.3f;
		break;
	}
}

void CCody_Space::Respawn(const _float & fTimeDelta)
{
	m_pController->move({ 0.0f, 0.0f, 0.0f }, 0, fTimeDelta, m_ControllFilter);
	InteractionSetPos(m_vSavePoint, fTimeDelta);
}

void CCody_Space::Late_Dead(const _float & fTimeDelta)
{
	if (m_tObjInfo.eSceneID != RESOURCE_SF2 && !m_bSceneWait)
		return;

	if (m_eCameraMode == CPlayerValue::Camera_2D && m_bSceneWait)
	{
		if (m_bFade && !m_bRespawn &&
			Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Get_Fade())
		{
			m_bRespawn = true;
			return;
		}
		else if (m_bFade)
		{
			_vec3 vPos = Engine::Get_GameObject(L"GameObject", L"MainCamera")->Is<CMainCamera>()->Get_CameraAt();
			InteractionSetPos(vPos, fTimeDelta);
		}

		if (m_bRespawn)
		{
			// 리스폰 시작지점 한번만 탐
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_DEAD;
			Clear(PL_DEAD);
			Set_ClearJump();
			m_bRespawnEffect = true;
			m_bFade = false;
			m_bDeadUI = false;

			Respawn(fTimeDelta);
		}
	}
	else
	{
		if (m_bFade)
		{
			// 리스폰 시작지점 한번만 탐
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_DEAD;
			Clear(PL_DEAD);
			Set_ClearJump();
			m_bRespawnEffect = true;
			m_bFade = false;
			m_bDeadUI = false;

			Respawn(fTimeDelta);
		}
	}
}

void CCody_Space::Late_Missile_Ride(const _float & fTimeDelta)
{
	if (m_pPickUpTarget&&m_pPickUpTarget->Get_Dead())
	{
		//Safe_Release(m_pPickUpTarget);
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_MISSILE;
		Clear(PL_MISSILE);
		m_pTransformCom->Set_AngleZ(0.0f);
		m_pTransformCom->Set_AngleX(0.0f);

		return;
	}

	if (false == m_bMoveTo)
		return;

	_matrix matWorld;
	_vec3 vMissileUp;
	D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)m_pMeshCom->Get_FrameByName("Hips");
	_float4x4 matBone = pBone->CombinedTranformationMatrix;
	matWorld = *m_pTransformCom->Get_WorldMatrix();
	matBone *= matWorld;
	_vec3 vBonePos = _vec3(matBone._41, matBone._42, matBone._43);

	_matrix matMissileWorld = m_pPickUpTarget->Is<CBaboonMissile>()->Get_WorldMatrix();
	memcpy(&m_vTargetPos, (_vec3*)&matMissileWorld._41, sizeof(_vec3));
	memcpy(&m_vPushDIr, (_vec3*)&matMissileWorld._31, sizeof(_vec3));
	memcpy(&vMissileUp, (_vec3*)&matMissileWorld._21, sizeof(_vec3));
	D3DXVec3Normalize(&m_vPushDIr, &m_vPushDIr);
	D3DXVec3Normalize(&vMissileUp, &vMissileUp);
	m_vTargetPos += m_vPushDIr * 0.5F;
	m_vTargetPos += vMissileUp * 0.3F;

	_vec3 vDir = m_vTargetPos - vBonePos;
	InteractionMove(vDir, fTimeDelta);

	_vec3 vAngle = m_pPickUpTarget->Is<CBaboonMissile>()->Get_Angle();
	m_pTransformCom->Set_Angle(vAngle);
}

void CCody_Space::Insert_Fuse_End()
{
	ToObj<CFuseNPC>(m_pPickUpTarget)->Set_TriggerOff();
	m_pPickUpTarget->Set_Dead();
	if (!m_pInteractActor)
		return;
	ToTrigger(m_pInteractActor)->Set_Interactable(false);
	ToTrigger(m_pInteractActor)->Activate(this);
	//m_bInsert = true;
}

Engine::_bool CCody_Space::Check_Insert()
{
	m_bInsert = false;
	return m_bInsert;
}

CCody_Space::CCody_Space(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayerValue(pGraphicDev)
{
}

CCody_Space::~CCody_Space()
{
}

void CCody_Space::Set_SizeChange(const _float& Size)
{
	if (m_bSizeChange)
		return;

	m_bSizeChange = true;
	m_fTargetSize = Size;
	m_fCurSize = m_pTransformCom->Get_Scale().x;
	m_fChangeTime = 0.f;

}

void CCody_Space::Change_Size(const _float& fTimeDelta)
{
	if (!m_bSizeChange)
		return;
	_float fSize;
	_float TotalTime = 0.2f;
	m_fChangeTime += fTimeDelta;
	fSize = (m_pTransformCom->Get_Scale().x + ((((m_fTargetSize)-(m_fCurSize)) / TotalTime)*fTimeDelta));
	//fSize= (max(m_fTargetSize,m_fCurSize)-min(m_fTargetSize,m_fCurSize)) *m_fChangeTime / TotalTime;

	if (m_fChangeTime > TotalTime)
	{
		fSize = m_fTargetSize;
		m_pTransformCom->Set_Scale(fSize);
		_vec3 vPos = m_pTransformCom->Get_Position();

		if (m_pAttachTarget)
		{
			_vec3* pPos = (_vec3*)m_pAttachTarget->On_Attach(this);
			if (pPos)
				vPos.y = pPos->y + 1.F;
		}
		PxExtendedVec3 vExPos = { vPos.x,vPos.y,vPos.z };
		m_pController->setFootPosition(vExPos);
		m_pTransformCom->Set_Pos(vPos);


		m_bSizeChange = false;
		return;
	}
	m_pTransformCom->Set_Scale(fSize);
	_vec3 vPos = m_pTransformCom->Get_Position();
	if (m_pAttachTarget)
	{
		_vec3* pPos = (_vec3*)m_pAttachTarget->On_Attach(this);
		if (pPos)
			vPos.y = pPos->y + 1.F;
	}

	PxExtendedVec3 vExPos = { vPos.x,vPos.y,vPos.z };
	m_pController->setFootPosition(vExPos);
	m_pTransformCom->Set_Pos(vPos);
}

HRESULT CCody_Space::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	// 애니메이션
	ZeroMemory(&m_bJump, sizeof(m_bJump));
	ZeroMemory(&m_bDash, sizeof(m_bDash));
	ZeroMemory(&m_bPlaySound, sizeof(m_bPlaySound));

	m_eCodyAnim = Cody_Space_Idle;
	m_ePreCodyAnim = Cody_Space_Idle;
	m_iMaxHp = 120;
	m_iHp = m_iMaxHp;

	m_fZ = -1.516f; // 시작~
	//m_fZ = -1.12f; // 바운스

	//m_ePL_STATE = PL_HANDLE;
	//m_vSavePoint = { 55.008f, 19.601f, -1.516f };
	return S_OK;
}

Engine::_int CCody_Space::Update_Object(const _float& fTimeDelta)
{
	if (!m_bKeyOff)
	{
		m_fTimeDelta = fTimeDelta;
		m_bEveentKey = false;


		Change_Size(fTimeDelta);
		Engine::CGameObject::Update_Object(fTimeDelta);

		Key_Reset();
		Key_Input(fTimeDelta);
		Event(fTimeDelta);
		Set_State(fTimeDelta);
		m_bEventJump = false;
		Update_Time(fTimeDelta);

		// 애니메이션
		/*if (!m_bPlayerDead)
			PlayAnimation(this, m_pMeshCom, fTimeDelta, m_ePL_STATE, &m_ePre_State);*/
		Effect(fTimeDelta);
		Update_UI(fTimeDelta);
	}
	if (!m_bDeadUI && !m_bRenderOff)
	{
		if (!m_bPlayerDead)
			PlayAnimation(this, m_pMeshCom, fTimeDelta, m_ePL_STATE, &m_ePre_State);

		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		if (m_bStencil)
		{
			Engine::Add_RenderGroup(Engine::RENDER_STENCIL, this);
			if (m_tObjInfo.eSceneID != RESOURCE_SF2)
				Engine::Add_RenderGroup(Engine::RENDER_SHADOW, this);
		}
	}
	if (D3DXVec3Length(&vAccDir) > 0.f)
	{

		vAccDir -= vAccDir*0.01f;
	}
	else
	{
		vAccDir = { 0.f,0.f,0.f };
	}

	return OBJ_NOEVENT;
}

_int CCody_Space::LateUpdate_Object(const _float & fTimeDelta)
{
	if (m_bKeyOff)
		return 0;

	m_pAttachTarget = nullptr;
	Late_Dead(fTimeDelta);
	Gravity(fTimeDelta);
	Set_Late_State(fTimeDelta);
	_vec3 vOrgPos = m_pTransformCom->Get_Position();
	m_bRideObj = false;
	_vec3 vPos;
	PxTransform Trnasform;
	PxExtendedVec3 pxPos = m_pController->getFootPosition();
	vPos.x = (_float)pxPos.x;
	vPos.y = (_float)pxPos.y;
	vPos.z = (_float)pxPos.z;

	ChangeZ(fTimeDelta);

	if (m_ePL_STATE != PL_DEAD)
		m_vDeadSlide = vPos - vOrgPos;

	if (m_eCameraMode == Camera_2D)
	{
		pxPos.z = m_fZ;
		m_pController->setFootPosition(pxPos);
		//m_pTransformCom->Set_PosZ(m_fZ);
		//vPos.z = m_fZ;
	}

	if (m_bPhys)
		m_pTransformCom->Set_Pos(vPos);

	if (m_bStart)
	{
		Create_UI(L"Cody_UI");
		m_bStart = false;
	}

	return _int();
}

void CCody_Space::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	//pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CCody_Space::Render_Shadow(_vec3 * vEye, _vec3 * vAt, const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix matView, matInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	CMay_Space* pMay = (CMay_Space*)Engine::Get_GameObject(L"GameObject", L"May");
	CTransform* pMayTrans = (CTransform*)pMay->Get_Component(L"Com_Transform", ID_DYNAMIC);

	*vAt = (m_pTransformCom->Get_Position() + pMayTrans->Get_Position()) * 0.5f;
	//*vAt = _vec3(0.f, 0.f, 0.f);
	*vEye = *vAt + SHADOWLIGHT;

	_matrix		LightViewMatrix, LightProjMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, vEye, vAt, &vUP);
	pEffect->SetMatrix("g_matView", &LightViewMatrix);

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 500.f);
	pEffect->SetMatrix("g_matProj", &LightProjMatrix);

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 2);

	pEffect->End();

	Safe_Release(pEffect);
}

void CCody_Space::Render_Stencil(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 3);

	pEffect->End();

	Safe_Release(pEffect);

}

void CCody_Space::Effect(const _float & fTimeDelta)
{
	Effect_Walk(fTimeDelta);
	Effect_Dead(fTimeDelta);
	Effect_Respawn(fTimeDelta);
	Effect_Star(fTimeDelta);
	Effect_Hit(fTimeDelta);
}

void CCody_Space::Effect_Walk(const _float & fTimeDelta)
{
	m_fMoveEffectTime += fTimeDelta;

	if (m_ePL_STATE == PL_RUN || m_ePL_STATE == PL_MOVE)
	{
		if (m_fMoveEffectTime > 0.2f)
		{
			PARTICLE tParticle;
			Engine::Reset_Particle(tParticle);
			Engine::Set_AnimState(tParticle, 8, 8, 2.f);

			tParticle.vPos = m_pTransformCom->Get_Position();
			tParticle.fAcc = 1.f;
			tParticle.fCurrentTime = 0.f;
			tParticle.fLifeTime = 1.0f;
			tParticle.fRotateSpeed = 0.f;
			tParticle.fVelocity = 1.f;
			tParticle.vDir = _vec3(0.f, 0.5f, 0.f);
			tParticle.vStartSize = _vec3(1.f, 1.f, 1.f);
			tParticle.vEndSize = _vec3(2.f, 2.f, 2.f);
			tParticle.vStartColor = { 255.f, 255.f, 255.f, 150.f };
			tParticle.vEndColor = { 150.f, 150.f, 150.f, 0.f };

			Engine::Emit_ImageParticle(tParticle, 1, 4);
			m_fMoveEffectTime = 0.f;
		}
	}
}

void CCody_Space::Effect_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadEffect)
		return;

	m_fAmount += fTimeDelta;
	if (m_fAmount >= 1.f)
	{

		if (m_tObjInfo.eSceneID != RESOURCE_SB)
		{
			m_bDeadEffect = false;
			m_fFade = 1.f;
			m_bFade = true;
		}
		else
		{
			m_bDeadEffect = false;
			m_bBossMapDead = true;
			m_fFade = 80.f;
		}

		m_bDeadUI = true;
		m_fAmount = 1.f;
	}
	else if (m_fAmount < 0.02f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 3.f;
		tParticle.fLifeTime = 0.8f;
		tParticle.vDir = { 0.f, 1.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.5f, 0.f);

		tParticle.vStartColor = { 200.f, 200.f, 150.f, 255.f * 2.f };
		tParticle.vEndColor = { 200.f, 200.f, 150.f, 255.f };
		Engine::Emit_ImageParticle(tParticle, 30, 4);

		tParticle.vStartColor = { 40.f, 40.f, 10.f, 255.f * 4.5f };
		tParticle.vEndColor = { 40.f, 40.f, 10.f, 255.f };
		Engine::Emit_ImageParticle(tParticle, 29, 4);
	}
	else if (m_fAmount < 0.5f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 4, 4, 0.5f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.3f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 3.f;
		tParticle.vStartSize = _vec3(0.2f, 0.2f, 0.2f);
		tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f };
		tParticle.vEndColor = { 0.f, 0.f, 0.f, 255.f };

		for (_int i = 0; i < 5; ++i)
		{
			tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f };
			tParticle.vEndColor = { 0.f, 0.f, 0.f, 255.f };
			tParticle.vStartSize = _vec3(0.2f, 0.2f, 0.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.7f), CRandoms()(-0.3f, 0.3f));
			tParticle.vDir = _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.f), CRandoms()(-0.3f, 0.3f));

			Engine::Emit_ImageParticle(tParticle, 8, 9);

			tParticle.vStartSize = _vec3(0.1f, 0.1f, 0.1f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.7f), CRandoms()(-0.3f, 0.3f));
			tParticle.vDir = _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.f), CRandoms()(-0.3f, 0.3f));
			Engine::Emit_ImageParticle(tParticle, 15, 11);

			tParticle.vStartColor = { 200.f, 200.f, 150.f, 255.f };
			tParticle.vEndColor = { 200.f, 200.f, 150.f, 255.f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.7f), CRandoms()(-0.3f, 0.3f));
			tParticle.vDir = _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.f), CRandoms()(-0.3f, 0.3f));
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
	}
}

void CCody_Space::Effect_Respawn(const _float & fTimedelta)
{
	if (!m_bRespawnEffect)
		return;

	m_fAmount -= fTimedelta;
	if (m_fAmount <= 0.f)
	{
		m_bRespawnEffect = false;
		m_bStencil = true;
		m_fAmount = 0.f;
		if (m_eCameraMode != CPlayerValue::Camera_2D)
			Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>()->Set_RayCheck(true);
	}
	else if (m_fAmount > 0.4f)
	{
		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);


		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.fLifeTime = 0.3f;
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.fVelocity = 3.f;
		tParticle2.vStartSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle2.vEndSize = _vec3(0.05f, 0.05f, 0.05f);


		for (_int i = 0; i < 20; ++i)
		{
			tParticle2.vStartColor = { 0.f, 0.f, 0.f, 255.f };
			tParticle2.vEndColor = { 0.f, 0.f, 0.f, 255.f };
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.f, 1.f), CRandoms()(-0.8f, 2.5f), CRandoms()(-1.f, 1.f));
			_vec3 vDir = (m_pTransformCom->Get_Position() + _vec3(0.f, CRandoms()(0.f, 2.f), 0.f)) - tParticle2.vPos;
			D3DXVec3Normalize(&vDir, &vDir);
			tParticle2.vDir = vDir;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);

			tParticle2.vStartColor = { 200.f, 200.f, 150.f, 255.f };
			tParticle2.vEndColor = { 200.f, 200.f, 150.f, 255.f };
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.f, 1.f), CRandoms()(-0.8f, 2.5f), CRandoms()(-1.f, 1.f));
			vDir = (m_pTransformCom->Get_Position() + _vec3(0.f, CRandoms()(0.f, 2.f), 0.f)) - tParticle2.vPos;
			D3DXVec3Normalize(&vDir, &vDir);
			tParticle2.vDir = vDir;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);
		}
	}


	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(2.5f, 2.5f, 2.5f);
	tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
	tParticle.fRotateSpeed = 8.f;
	tParticle.fVelocity = 4.f;
	tParticle.fLifeTime = 1.5f;
	tParticle.vDir = { 0.f, -1.f, 0.f };
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.7f, 0.f);

	//tParticle.bRotAdd = true;
	tParticle.bBill = false;
	tParticle.vRot = { D3DXToRadian(90.f), 0.f,  0.f };

	tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f * 5.f };
	tParticle.vEndColor = { 0.f, 0.f, 0.f, 255.f* 5.f };
	Engine::Emit_ImageParticle(tParticle, 45, 24);

	//tParticle.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
	tParticle.vStartColor = { 200.f, 200.f, 150.f, 255.f * 3.f };
	tParticle.vEndColor = { 200.f, 200.f, 150.f, 255.f* 1.5f };
	Engine::Emit_ImageParticle(tParticle, 45, 24);
}

void CCody_Space::Effect_SizeUp(const _float & fTimeDelta, _int iSizeIndex)
{
	switch (iSizeIndex)
	{
	case 0://
	{
		_vec3 vLook, vUp, vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXVec3Normalize(&vDir, &(vLook + vUp));

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;

		tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.bGravity = true;

		for (_int i = 0; i < 100; ++i)
		{
			tParticle.fGravityForce = CRandoms()(1.f, 2.5f);
			_vec3 vAddDir = { CRandoms()(-0.2f, 0.2f), CRandoms()(0.f, 0.5f) , CRandoms()(-0.2f, 0.2f) };
			D3DXVec3Normalize(&vAddDir, &vAddDir);
			tParticle.vDir = vDir + vAddDir;
			tParticle.fLifeTime = CRandoms()(0.3f, 0.7f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.05f, 0.05f), CRandoms()(-0.2f, 0.5f), CRandoms()(-0.05f, 0.05f));
			_float fSize = CRandoms()(0.1f, 0.4f);
			tParticle.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle.fVelocity = CRandoms()(5.f, 7.f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
	}
	break;

	case 1://
	{
		_vec3 vLook, vUp, vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXVec3Normalize(&vDir, &(vLook + vUp));

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;

		tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.bGravity = true;


		for (_int i = 0; i < 100; ++i)
		{
			tParticle.fGravityForce = CRandoms()(1.f, 2.5f);
			_vec3 vAddDir = { CRandoms()(-1.f, 1.f), CRandoms()(0.f, 1.f) , CRandoms()(-1.f, 1.f) };
			D3DXVec3Normalize(&vAddDir, &vAddDir);
			tParticle.vDir = vDir + vAddDir;
			tParticle.fLifeTime = CRandoms()(0.3f, 0.7f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.5f, 1.5f), CRandoms()(0.f, 7.f), CRandoms()(-1.5f, 1.5f));
			_float fSize = CRandoms()(0.3f, 0.7f);
			tParticle.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle.fVelocity = CRandoms()(10.f, 15.f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
	}

	break;

	case 2:
		break;

	default:
		return;
	}
}

void CCody_Space::Effect_SizeDown(const _float & fTimeDelta, _int iSizeIndex)
{
	switch (iSizeIndex)
	{
	case 0:
		break;

	case 1: //
	{
		_vec3 vLook, vUp, vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXVec3Normalize(&vDir, &(vLook + vUp));

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;

		tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.bGravity = true;

		for (_int i = 0; i < 100; ++i)
		{
			tParticle.fGravityForce = CRandoms()(1.f, 2.5f);
			_vec3 vAddDir = { CRandoms()(-0.2f, 0.2f), CRandoms()(0.f, 0.5f) , CRandoms()(-0.2f, 0.2f) };
			D3DXVec3Normalize(&vAddDir, &vAddDir);
			tParticle.vDir = vDir + vAddDir;
			tParticle.fLifeTime = CRandoms()(0.3f, 0.7f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.05f, 0.05f), CRandoms()(-0.2f, 0.5f), CRandoms()(-0.05f, 0.05f));
			_float fSize = CRandoms()(0.1f, 0.4f);
			tParticle.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle.fVelocity = CRandoms()(5.f, 7.f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
	}
	break;

	case 2: //
	{
		_vec3 vLook, vUp, vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXVec3Normalize(&vDir, &(vLook + vUp));

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;

		tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.vEndColor = { 120.f * 3.5f, 200.f * 3.5f, 255.f * 3.5f, 255.f * 2.f };
		tParticle.bGravity = true;


		for (_int i = 0; i < 100; ++i)
		{
			tParticle.fGravityForce = CRandoms()(1.f, 2.5f);
			_vec3 vAddDir = { CRandoms()(-1.f, 1.f), CRandoms()(0.f, 1.f) , CRandoms()(-1.f, 1.f) };
			D3DXVec3Normalize(&vAddDir, &vAddDir);
			tParticle.vDir = vDir + vAddDir;
			tParticle.fLifeTime = CRandoms()(0.3f, 0.7f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.5f, 1.5f), CRandoms()(0.f, 7.f), CRandoms()(-1.5f, 1.5f));
			_float fSize = CRandoms()(0.3f, 0.7f);
			tParticle.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle.fVelocity = CRandoms()(10.f, 15.f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
	}
	break;

	default:
		return;
	}
}

void CCody_Space::Effect_Star(const _float & fTimeDelta)
{
	m_fStarEffectTime += fTimeDelta;

	if (m_fStarEffectTime > 0.4f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vEndSize = _vec3(0.f, 0.f, 0.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-30.f, 30.f), CRandoms()(-30.f, 30.f), CRandoms()(-30.f, 30.f));
		tParticle.vStartColor = { 255.f * 5.5f, 255.f * 5.5f, 25.f * 5.5f, 255.f };
		tParticle.vEndColor = { 255.f * 5.5f, 255.f * 5.5f, 25.f * 5.5f, 255.f };

		tParticle.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
		tParticle.vDir = { CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.fVelocity = CRandoms()(1.f, 2.f);
		_float fSize = CRandoms()(0.8f, 1.2f);
		tParticle.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle.fLifeTime = CRandoms()(7.f, 12.f);;

		Engine::Emit_ImageParticle(tParticle, 26, 31);

		m_fStarEffectTime = 0.f;
	}

}

void CCody_Space::Effect_Hit(const _float & fTimeDelta)
{
	if (!m_bHitEffect)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
	tParticle.bGravity = true;
	tParticle.vStartColor = { 255.f * 2.5f, 255.f * 2.5f, 25.f * 2.5f, 255.f };
	tParticle.vEndColor = { 255.f * 2.5f, 255.f * 2.5f, 25.f * 2.5f, 255.f };

	for (_int i = 0; i < 17; ++i)
	{
		tParticle.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
		tParticle.vDir = { CRandoms()(-1.f, 1.f), CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.fVelocity = CRandoms()(8.f, 10.f);
		_float fSize = CRandoms()(0.5f, 0.8f);
		tParticle.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle.fLifeTime = CRandoms()(0.5f, 0.8f);;

		tParticle.fGravityForce = CRandoms()(1.f, 2.f);
		Engine::Emit_ImageParticle(tParticle, 26, 31);
	}

	m_bHitEffect = false;
}

void CCody_Space::Create_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_STATIC;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pFade = pGameObject->Is<CImageUI>();
	m_pFade->Set_Image(L"White_BG");
	m_pFade->Set_Size(_vec3{ 960.f, 1080.f, 0.f });
	m_pFade->Set_Pos(_vec3{ 960.f, 0.f, 0.f });
	m_pFade->Set_RenderID(RENDER_FINALUI);
	m_pFade->Set_PassIndex(2);
	m_pFade->Set_Color(_vec4(0.f, 0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Fade", pGameObject), );
	m_pFade->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pProfile = pGameObject->Is<CImageUI>();
	m_pProfile->Set_Image(L"CW_ImageUI");
	m_pProfile->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pProfile->Set_Pos(_vec3{ 1732.f, -880.f, 0.f });
	m_pProfile->Set_RenderID(RENDER_UI);
	m_pProfile->Set_PassIndex(2);
	m_pProfile->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Profile", pGameObject), );
	m_pProfile->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pHp = pGameObject->Is<CImageUI>();
	m_pHp->Set_Image(L"CW_ImageUI");
	m_pHp->Set_Size(_vec3{ 160.f, 160.f, 0.f });
	m_pHp->Set_Pos(_vec3{ 1716.f, -864.f, 0.f });
	m_pHp->Set_RenderID(RENDER_UI);
	m_pHp->Set_PassIndex(14);
	m_pHp->Set_TextureIndex(1);
	m_pHp->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Hp", pGameObject), );
	m_pHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[0] = pGameObject->Is<CImageUI>();
	m_pDeadUI[0]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[0]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[0]->Set_Pos(_vec3{ 1240.f, -330.f, 0.f });
	m_pDeadUI[0]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[0]->Set_PassIndex(11);
	m_pDeadUI[0]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Dead", pGameObject), );
	m_pDeadUI[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[1] = pGameObject->Is<CImageUI>();
	m_pDeadUI[1]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[1]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[1]->Set_Pos(_vec3{ 1240.f, -330.f, 0.f });
	m_pDeadUI[1]->Set_RenderID(RENDER_UI);
	m_pDeadUI[1]->Set_PassIndex(12);
	m_pDeadUI[1]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Dead2", pGameObject), );
	m_pDeadUI[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[2] = pGameObject->Is<CImageUI>();
	m_pDeadUI[2]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[2]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[2]->Set_Pos(_vec3{ 1240.f, -330.f, 0.f });
	m_pDeadUI[2]->Set_RenderID(RENDER_UI);
	m_pDeadUI[2]->Set_PassIndex(13);
	m_pDeadUI[2]->Set_TextureIndex(14);
	m_pDeadUI[2]->Set_Color(_vec4(231.f, 76.f, 60.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Dead3", pGameObject), );
	m_pDeadUI[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[3] = pGameObject->Is<CImageUI>();
	m_pDeadUI[3]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pDeadUI[3]->Set_Pos(_vec3{ 1410.f, -650.f, 0.f });
	m_pDeadUI[3]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[3]->Set_PassIndex(2);
	m_pDeadUI[3]->Set_TextureIndex(13);
	m_pDeadUI[3]->Set_Color(236.f, 216.f, 162.f, 255.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Dead4", pGameObject), );
	m_pDeadUI[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[4] = pGameObject->Is<CImageUI>();
	m_pDeadUI[4]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[4]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pDeadUI[4]->Set_Pos(_vec3{ 1420.f, -660.f, 0.f });
	m_pDeadUI[4]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[4]->Set_PassIndex(2);
	m_pDeadUI[4]->Set_TextureIndex(18);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Dead5", pGameObject), );
	m_pDeadUI[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[5] = pGameObject->Is<CImageUI>();
	m_pDeadUI[5]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[5]->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pDeadUI[5]->Set_Pos(_vec3{ 1376.f, -610.f, 0.f });
	m_pDeadUI[5]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[5]->Set_PassIndex(2);
	m_pDeadUI[5]->Set_TextureIndex(15);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_Dead6", pGameObject), );
	m_pDeadUI[5]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pMaySign = pGameObject->Is<CImageUI>();
	m_pMaySign->Set_Image(L"CW_ImageUI");
	m_pMaySign->Set_Size(_vec3{ 48.f, 48.f, 0.f });
	m_pMaySign->Set_Pos(_vec3{ 1716.f, -864.f, 0.f });
	m_pMaySign->Set_RenderID(RENDER_FINALUI);
	m_pMaySign->Set_PassIndex(2);
	m_pMaySign->Set_TextureIndex(16);
	m_pMaySign->Set_Color(_vec4(75.f, 175.f, 255.f, 255.f));
	m_pMaySign->Set_RoT(ROT_Z, D3DXToRadian(-90.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cody_MaySign", pGameObject), );
	m_pMaySign->AddRef();

	for (_int i = 0; i < 6; ++i)
	{
		m_pDeadUI[i]->Set_Visible(false);
		m_pDeadUI[i]->Change_Size_From_To(0.1f, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f));
	}

	if (m_tObjInfo.eSceneID == RESOURCE_SF || m_tObjInfo.eSceneID == RESOURCE_SF2 || m_tObjInfo.eSceneID == RESOURCE_SF3)
	{
		m_pProfile->Set_Visible(false);
		m_pHp->Set_Visible(false);
	}

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CCody_Space::Update_UI(const _float & fTimeDelta)
{
	HP_UI(fTimeDelta);
	Sign_UI(fTimeDelta);
	NormalMap_Dead(fTimeDelta);
	BossMap_Dead(fTimeDelta);
	FadeIn(fTimeDelta);
	FadeOut(fTimeDelta);
}

void CCody_Space::HP_UI(const _float & fTimeDelta)
{
	if (m_pHp)
		m_pHp->Set_Amount(_float(m_iHp) / m_iMaxHp);

	if (m_iHp <= 0)
		m_ePL_STATE = PL_DEAD;
}

void CCody_Space::Sign_UI(const _float & fTimeDelta)
{
	if (m_tObjInfo.eSceneID != RESOURCE_SF2)
	{
		CMay* pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		CTransform* pMayTrans = (CTransform*)pMay->Get_Component(L"Com_Transform", ID_DYNAMIC);

		if (D3DXVec3Length(&(pMayTrans->Get_Position() - m_pTransformCom->Get_Position())) < 20.f)
		{
			m_pMaySign->Set_Visible(false);
			return;
		}

		m_pMaySign->Set_Visible(true);

		_matrix matProj, matView;

		_vec3 vMayPos = pMayTrans->Get_Position();
		vMayPos.y += 4.f;

		CCodyCamera* pCodyCam = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");
		matView = pCodyCam->Get_View();
		matProj = pCodyCam->Get_Projection();

		D3DXVec3TransformCoord(&vMayPos, &vMayPos, &matView);
		D3DXVec3TransformCoord(&vMayPos, &vMayPos, &matProj);


		_vec3 vPos = { ((vMayPos.x + 0.5f) * WINCX * 0.5f) + WINCX * 0.5f, (vMayPos.y + 1.f) * WINCY * 0.5f - WINCY, vMayPos.z };

		_vec3 FinalPos = vPos + _vec3(-24.f, 0.f, 0.f);


		_bool IsOut = false;
		if (FinalPos.x < 990.f)
		{
			IsOut = true;
			FinalPos.x = 990.f;
		}

		if (FinalPos.x > 1840.f)
		{
			IsOut = true;
			FinalPos.x = 1840.f;
		}

		if (FinalPos.y > -30.f)
		{
			IsOut = true;
			FinalPos.y = -30.f;
		}

		if (FinalPos.y < -1000.f)
		{
			IsOut = true;
			FinalPos.y = -1000.f;
		}

		if (FinalPos.z < 0.f)
		{
			//FinalPos.x *= -1.f;
			FinalPos.y *= -1.f;
			//FinalPos.z *= -1.f;
		}



		m_pMaySign->Set_Pos(FinalPos);
		_vec3 vCenter = { 1440.f,-540.f,0.f };
		FinalPos = FinalPos - vCenter;
		D3DXVec3Normalize(&FinalPos, &FinalPos);
		if (IsOut)
			m_pMaySign->Set_Dir(ROT_Z, atan2f(FinalPos.x, -FinalPos.y) + D3DXToRadian(-90.f));
		else
			m_pMaySign->Set_Dir(ROT_Z, D3DXToRadian(-90.f));
	}
	else if (m_tObjInfo.eSceneID == RESOURCE_SF2)
	{
		if (m_iSize == 0)
		{
			_matrix matProj, matView;

			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

			_vec3 vPos = m_pTransformCom->Get_Position();
			vPos.y += 3.f;

			CCamera* pCodyCam = nullptr;
			pCodyCam = (CCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
			if (nullptr == pCodyCam)
				pCodyCam = (CCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera"); // 새로추가

			matView = pCodyCam->Get_View();
			matProj = pCodyCam->Get_Projection();

			D3DXVec3TransformCoord(&vPos, &vPos, &matView);
			D3DXVec3TransformCoord(&vPos, &vPos, &matProj);

			m_pMaySign->Set_Pos(_vec3(((vPos.x + 1.f) * WINCX * 0.5f), (vPos.y + 1.f) * WINCY * 0.5f - WINCY, vPos.z) + _vec3(-16.f, 0.f, 0.f));
			m_pMaySign->Set_Visible(true);
			m_pMaySign->Set_Color(_vec4(75.f, 255.f, 175.f, 255.f));
		}
		else
			m_pMaySign->Set_Visible(false);
	}
}

void CCody_Space::NormalMap_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadUI || m_tObjInfo.eSceneID == RESOURCE_SB)
		return;

	if (m_eCameraMode == CPlayerValue::Camera_2D || true == m_bSceneWait)
		return;

	if (m_bFade)
	{
		m_fFade += 255.f * fTimeDelta * 2.f;
		if (m_fFade >= 255.f)
		{
			m_bDeadStart = false;
			m_bPhys = true;
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_DEAD;
			m_bRespawnEffect = true;
			m_bPhys = true;
			m_pController->setFootPosition(PxExtendedVec3(m_vSavePoint.x, m_vSavePoint.y, m_vSavePoint.z));
			m_bPlayerDead = false;
			m_fCurDeadTime = 0.0f;
			ZeroMemory(&m_bJump, sizeof(m_bJump));

			m_bJumpDashStart = false;
			m_bJumpDash = false;
			m_fJumpTime = 0.0f;
			m_bStopGravity = false;
			m_fGravityTime = 0.0f;
			m_bStopGravity = false;
			Clear(PL_DEAD);
			Set_ClearJump();

			m_bFade = false;
		}
	}
	else
	{
		m_fFade -= 255.f * fTimeDelta * 3.f;
		if (m_fFade <= 0.f)
		{
			//m_bFade = false;
			m_bDeadUI = false;

		}
	}

	m_pFade->Set_Color(_vec4(0.f, 0.f, 0.f, m_fFade));
}

void CCody_Space::BossMap_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadUI || m_tObjInfo.eSceneID == RESOURCE_SF || m_tObjInfo.eSceneID == RESOURCE_SF2 || m_tObjInfo.eSceneID == RESOURCE_SF3)
		return;

	if (Engine::Key_Pressing(DIK_E) && m_pDeadUI[0]->Get_Amount() < 1.f)
	{
		m_pDeadUI[2]->Change_Color_From_To(0.5f, _vec4(255.f, 255.f, 255.f, 255), _vec4(231.f, 76.f, 60.f, 255.f));
		m_pDeadUI[2]->Change_Size_From_To(0.15f, _vec3(500.f, 500.f, 0.f), _vec3(400.f, 400.f, 0.f));
		m_pDeadUI[3]->Change_Size_From_To(0.15f, _vec3(30.f, 30.f, 0.f), _vec3(60.f, 60.f, 0.f));
		m_pDeadUI[4]->Change_Size_From_To(0.15f, _vec3(20.f, 20.f, 0.f), _vec3(40.f, 40.f, 0.f));

		m_pDeadUI[0]->Set_AddAmount(0.01f);
	}


	if (m_bDeadSign)
	{
		m_fDeadSign += 32.f * fTimeDelta * 5.f;
		if (m_fDeadSign >= 128.f)
			m_bDeadSign = false;
	}
	else
	{
		m_fDeadSign -= 32.f * fTimeDelta * 5.f;
		if (m_fDeadSign <= 96.f)
			m_bDeadSign = true;
	}

	m_pDeadUI[5]->Set_Size(_vec3(m_fDeadSign, m_fDeadSign, 0.f));

	if (m_pDeadUI[0]->Get_Amount() >= 1.f)
	{
		if (m_bBossMapDead)
		{
			Engine::Set_CodyGreyScale(false);

			for (_int i = 0; i < 6; ++i)
				m_pDeadUI[i]->Change_Color_From_To(0.2f, _vec4(255.f, 255.f, 255.f, 255.f), _vec4(255.f, 255.f, 255.f, 0.f));

			for (_int i = 0; i < 3; ++i)
				m_pDeadUI[i]->Change_Size_From_To(0.2f, _vec3(400.f, 400.f, 0.f), _vec3(0.f, 0.f, 0.f));
			m_pDeadUI[3]->Change_Size_From_To(0.2f, _vec3(60.f, 60.f, 0.f), _vec3(0.f, 0.f, 0.f));
			m_pDeadUI[4]->Change_Size_From_To(0.2f, _vec3(40.f, 40.f, 0.f), _vec3(0.f, 0.f, 0.f));
			m_pDeadUI[5]->Change_Size_From_To(0.2f, _vec3(128.f, 128.f, 0.f), _vec3(0.f, 0.f, 0.f));

			//m_fFade = 1.f;
			m_bFade = true;

			m_bBossMapDead = false;
		}

		if (m_bFade)
		{
			m_fFade += 255.f * fTimeDelta * 2.f;
			if (m_fFade >= 255.f)
			{
				m_bDeadStart = false;
				m_bPhys = true;
				m_ePL_STATE = PL_IDLE;
				m_ePre_State = PL_DEAD;
				m_bRespawnEffect = true;
				m_bPhys = true;
				m_pController->setFootPosition(PxExtendedVec3(m_vSavePoint.x, m_vSavePoint.y, m_vSavePoint.z));
				m_bPlayerDead = false;
				m_fCurDeadTime = 0.0f;
				ZeroMemory(&m_bJump, sizeof(m_bJump));

				m_bJumpDashStart = false;
				m_bJumpDash = false;
				m_fJumpTime = 0.0f;
				m_bStopGravity = false;
				m_fGravityTime = 0.0f;
				m_bStopGravity = false;
				Clear(PL_DEAD);
				Set_ClearJump();

				m_bFade = false;
				for (_int i = 0; i < 6; ++i)
					m_pDeadUI[i]->Set_Visible(false);

				m_pProfile->Set_Visible(true);
				m_pHp->Set_Visible(true);
			}
		}
		else
		{
			m_fFade -= 255.f * fTimeDelta * 3.f;
			if (m_fFade <= 0.f)
			{
				m_bDeadUI = false;
			}
		}
	}
	m_pFade->Set_Color(_vec4(0.f, 0.f, 0.f, m_fFade));
}

CCody_Space* CCody_Space::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CCody_Space*	pInstance = new CCody_Space(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCody_Space::Free()
{
	Safe_Release(m_pImageUI);
	Safe_Release(m_pProfile);
	Safe_Release(m_pHp);
	Safe_Release(m_pFade);

	for (_int i = 0; i < 6; ++i)
		Safe_Release(m_pDeadUI[i]);
	Safe_Release(m_pMaySign);

	Engine::CGameObject::Free();
}


void CCody_Space::Set_Small(const _float& fTimeDelta)
{
	CCodyCamera* pCodyCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
	_float fRadius = 0.f;
	_float fHeight = 0.f;
	if (m_bSizeChange || !m_bChangeAble)
		return;
	StopNPlaySound(L"SizeDown.ogg", CSoundMgr::CHANNELID::ChessRook);

	switch (m_iSize)
	{
	case 0:
		return;
		break;
	case 1:
		Effect_SizeDown(fTimeDelta, m_iSize);
		m_fGravity = -0.2f;
		m_fJumpPower = 0.13f;
		m_fMaxSpeed = 2.f;
		fRadius = 0.035f;
		fHeight = 0.122f;
		--m_iSize;
		Set_SizeChange(0.001f);
		if (pCodyCamera&&m_eCameraMode == CPlayerValue::Camera_3D)
			pCodyCamera->Set_Dist(1.2f);
		break;
	case 2:
		Effect_SizeDown(fTimeDelta, m_iSize);
		fRadius = 0.35f;
		fHeight = 1.22f;
		--m_iSize;
		Set_SizeChange(0.01f);
		break;
	default:
		break;
	}
	static_cast<PxCapsuleController*>(m_pController)->setRadius(fRadius);
	static_cast<PxCapsuleController*>(m_pController)->resize(fHeight);
}

void CCody_Space::Set_Big(const _float& fTimeDelta)
{
	CCodyCamera* pCodyCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
	_float fRadius = 0.f;
	_float fHeight = 0.f;
	if (m_bSizeChange || !m_bChangeAble)
		return;
	StopNPlaySound(L"SizeUp.ogg", CSoundMgr::CHANNELID::ChessRook);
	switch (m_iSize)
	{
	case 0:
		Effect_SizeUp(fTimeDelta, m_iSize);
		m_fGravity = -2.0f;
		m_fJumpPower = 1.3f;
		m_fMaxSpeed = 8.0f;

		fRadius = 0.35f;
		fHeight = 1.22f;
		if (m_eCameraMode == CPlayerValue::Camera_3D)
			pCodyCamera->Set_Dist(12.f);
		++m_iSize;
		Set_SizeChange(0.01f);
		break;
	case 1:
		Effect_SizeUp(fTimeDelta, m_iSize);
		fRadius = 1.4f;
		fHeight = 4.88f;
		++m_iSize;
		Set_SizeChange(0.04f);
		break;
	case 2:
		return;
		break;
	default:
		break;
	}
	static_cast<PxCapsuleController*>(m_pController)->setRadius(fRadius);
	static_cast<PxCapsuleController*>(m_pController)->resize(fHeight);
}

HRESULT CCody_Space::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_ObjTag = L"Cody";
	// CDynamicMesh
	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"Cody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(0, 0, 0);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);


	m_fShapeHeight = 1.22f;
	m_fShapeRadius = 0.35f;
	m_fHalfHeight = m_fShapeRadius + (m_fShapeHeight * 0.5f);

	m_vSavePoint = { tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z };

	Create_Actor(Engine::Get_Physics(), m_fShapeHeight, m_fShapeRadius, this, this, { tTemp.vPos.x, tTemp.vPos.y + m_fHalfHeight, tTemp.vPos.z });
	m_tObjInfo.eSceneID = tTemp.eSceneID;

	return S_OK;
}

void CCody_Space::Key_Input(const _float& fTimeDelta)
{
	if (Lock())
		return;

	if (Engine::Key_Down(DIK_0))
	{
		m_bPlayerDead = true;
		//m_bSceneWait = true;
	}

	if (Engine::Key_Pressing(DIK_F3))
	{
		InteractionSetPos(_vec3(172.42f, 40.90f, -227.85f), fTimeDelta);
		//tObj.vPos = { 178.417f ,12.927f, -239.3427f }; // 박스.
		//m_bHandle = true;
	}

	if (Engine::Key_Pressing(DIK_W))
		m_iKeyState |= MOVE_UP;

	if (Engine::Key_Pressing(DIK_S))
		m_iKeyState |= MOVE_DOWN;

	if (Engine::Key_Pressing(DIK_D))
		m_iKeyState |= MOVE_RIGHT;

	if (Engine::Key_Pressing(DIK_A))
		m_iKeyState |= MOVE_LEFT;

	if (Engine::Key_Down(DIK_SPACE))
	{
		if (true == m_bJump[PL_1STJUMP] && m_iSize != 2)
			m_bJump[PL_2NDJUMP] = true;

		m_iKeyState |= JUMP;
	}
	if (Engine::Key_Down(DIM_LB) )
	{
		if(m_ePL_STATE == PL_IDLE || m_ePL_STATE == PL_JUMP || m_ePL_STATE == PL_RUN || m_ePL_STATE == PL_MOVE)
		//크기 작게
		Set_Small(fTimeDelta);
	}
	else if (Engine::Key_Down(DIM_RB))
	{
		if (m_ePL_STATE == PL_IDLE || m_ePL_STATE == PL_JUMP || m_ePL_STATE == PL_RUN || m_ePL_STATE == PL_MOVE)
		//크기 작게
		Set_Big(fTimeDelta);
	}

	if (Engine::Key_Pressing(DIK_LCONTROL))
	{
		if (m_ePL_STATE == PL_JUMP)
			m_bJump[PL_GROUNDPOUND_START] = true;

		if (m_iSize != 2)
			m_iKeyState |= SIT;
	}
	 
	if (Engine::Key_Down(DIK_CAPSLOCK) && m_iSize != 2)
		m_iKeyState |= RUN;

	if (Engine::Key_Down(DIK_E))
	{
		m_iKeyState |= INTERACTION;
		m_bEveentKey = true;
	}
	else 	if (Engine::Key_Down(DIK_Q))
	{
		m_iKeyState |= INTERACTION2;
	}

	if (Engine::Key_Down(DIK_LSHIFT) && m_iSize != 2)
	{
		if (m_ePL_STATE == PL_JUMP)
			m_bJump[PL_JUMPDASH] = true;

		m_iKeyState |= DASH;
	}

	if (Engine::Key_Down(DIK_F10))
	{
		m_bRenderDebug = !m_bRenderDebug;
	}

	//if (Engine::Key_Down(DIK_6))
	//{
	//	m_ePre_State = m_ePL_STATE;
	//	m_ePL_STATE = PL_RIDE;
	//	Set_RideTag("LeftHandBend", L"Com_Mesh", L"Com_Transform", _vec3(-90.0f, 0.0f, 0.0f));
	//	m_pPickUpTarget = Engine::Get_GameObject(L"GameObject", L"VacuumBoss");
	//}
}

HRESULT CCody_Space::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	if (m_bRespawnEffect)
	{
		if (m_fAmount >= 3.14f / 2.f)
		{
			m_bDead = true;
			m_fAmount = 3.14f / 2.f;
		}

		_float fSin = sinf(m_fAmount);
		pEffect->SetFloat("g_fAmount", fSin);
	}
	else
	{
		pEffect->SetFloat("g_fAmount", 0.f);
	}

	if (m_bDeadEffect)
	{
		if (m_fAmount >= 3.14f / 2.f)
		{
			m_bDead = true;
			m_fAmount = 3.14f / 2.f;
		}

		_float fSin = sinf(m_fAmount);
		pEffect->SetFloat("g_fAmount", fSin);
	}

	if (m_bStencil)
		pEffect->SetVector("g_vColor", &_vec4(175.f * 2.5f, 255.f * 2.5f, 225.f * 2.5f, 255.f));
	else
		pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));


	m_pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 0);
	m_pTextureCom->Set_Texture(pEffect, "g_BurnTexture", 1);
	pEffect->SetFloat("g_fMaterialIndex", 0.f);

	pEffect->CommitChanges();
	return S_OK;
}

void CCody_Space::Set_State(const _float & fTimeDelta)
{
	switch (m_ePL_STATE)
	{
	case CCody_Space::PL_IDLE:
		Idle_State(fTimeDelta);
		break;
	case CCody_Space::PL_MOVE:
		Move_State(fTimeDelta);
		break;
	case CCody_Space::PL_JUMP:
		Jump_State(fTimeDelta);
		break;
	case CCody_Space::PL_SIT:
		Sit_State(fTimeDelta);
		break;
	case CCody_Space::PL_RUN:
		Run_State(fTimeDelta);
		break;
	case CCody_Space::PL_DASH:
		Dash_State(fTimeDelta);
		break;
	case CCody_Space::PL_CLIMBING:
		Climbing_State(fTimeDelta);
		break;
	case CCody_Space::PL_HANG:
		Hang_State(fTimeDelta);
		break;
	case CCody_Space::PL_SLIDE:
		Slide_State(fTimeDelta);
		break;
	case CCody_Space::PL_RIDE:
		Ride_State(fTimeDelta);
		break;
	case CCody_Space::PL_PICKUP:
		PickUp_State(fTimeDelta);
		break;
	case CCody_Space::PL_DEAD:
		Dead_State(fTimeDelta);
		break;
	case CCody_Space::PL_LEVER:
		Lever_State(fTimeDelta);
		break;
	case CCody_Space::PL_HANGSWITCH:
		HangSwitch_State(fTimeDelta);
		break;
	case CCody_Space::PL_FLOAT:
		Float_state(fTimeDelta);
		break;
	case CCody_Space::PL_FALL:
		Fall_State(fTimeDelta);
		break;
	case CCody_Space::PL_PUSH:
		Push_State(fTimeDelta);
		break;
	case CCody_Space::PL_HANDLE:
		Handle_State(fTimeDelta);
		break;
	case CCody_Space::PL_FREEGRAVITY:
		Free_Gravity(fTimeDelta);
		break;
	case CCody_Space::PL_MISSILE:
		Missile_Ride(fTimeDelta);
		break;
	case CCody_Space::PL_HIT:
		Hit_State(fTimeDelta);
		break;
	}
}

void CCody_Space::Move(const _float& fTimeDelta, _float3* vResultPos, _float3* vResultDir, _float fRotSpeed)
{
	_float3 vPos, vDir;
	_float fCosTh;
	_int iRotDir;

	vPos = m_pTransformCom->Get_Position();

	_float fAngle = fRotSpeed;

	Set_Dir(&vDir, &iRotDir, &fCosTh);

	fAngle *= iRotDir;

	_float fRatio = 1.0f;
	if (fCosTh > 0.75f)
		fRatio = -1.0f / 0.2f * (fCosTh - 0.8f) + 1.0f;

	if (isnan(fRatio))
		int iA = 0;

	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngle * fTimeDelta * fRatio));

	if (vResultPos)
		*vResultPos = vPos;
	if (vResultDir)
		*vResultDir = vDir;
}

void CCody_Space::Dash(const _float & fTimeDelta, _float fDashRatio)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	_float3 vLook;

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vLook.y = 0.0f;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vDstPos = vLook * m_fMaxSpeed * fTimeDelta * fDashRatio;

	m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
}

void CCody_Space::Dash(const _float & fTimeDelta, _float fTime, _float fDashRatio)
{
	if (false == m_bDashStart)
	{
		m_fDashTime = 0.0f;
		m_bDashStart = true;

		_float3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		vLook.y = 0.0f;
		D3DXVec3Normalize(&vLook, &vLook);
		m_vDashDir = vLook * fDashRatio;
		m_fDashDecelTime = 0.0f;
	}

	m_fDashTime += fTimeDelta;
	_vec3 vDstPos;
	if (m_fDashTime <= fTime)
	{
		vDstPos = m_vDashDir * fTimeDelta * (1.0f / fTime);
		/*vDstPos = m_vDashDir * (m_fDashTime - fTime) - m_vDashDir * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);*/
	}
	else
	{
		m_fDashDecelTime += fTimeDelta * 2.0f;
		if (m_fDashDecelTime >= 1.0f)
			m_fDashDecelTime = 1.0f;

		vDstPos = m_vDashDir * fTimeDelta * (1.0f / fTime) * (1.0f - m_fDashDecelTime);
	}

	m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
}

_bool CCody_Space::Gravity(const _float & fTimeDelta)
{
	if (!m_bSimulate)
		return false;


	PxVec3 vGravity = PxVec3(0.0f, 0.0f, 0.0f);
	if (true == m_bJump[PL_JUMPSTART])
		vGravity.y = m_fHeight;
	else
		vGravity.y = m_fGravity * m_fGravityTime * m_fGravityTime;

	if (vGravity.y <= -1.0f)
		vGravity.y = -1.0f;

	PxU32 flags = 0;
	if (!m_bStopGravity)
	{
		flags = m_pController->move(vGravity, 0, fTimeDelta, m_ControllFilter);
		if (flags == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_bGround = true;
			if (true == m_bJump[PL_JUMPSTART])
			{
				m_bJump[PL_JUMPSTART] = false;
				// 애니메이션
				Set_GroundPoundStop();
			}
			else if (true == m_bSlideEnd)
				m_bRideGround = true;
		}
		else
			m_bGround = false;
	}

	if (true == m_bGround || true == m_bStopGravity || m_ePL_STATE == PL_JUMP)
		m_fGravityTime = 0.01f;
	else
		m_fGravityTime += fTimeDelta;

	return false;
}

void CCody_Space::Event(const _float & fTimeDelta)
{
	if (true == m_bPlayerDead && !(m_ePL_STATE == PL_DEAD))
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_DEAD;
	}

	else if ((m_ePL_STATE == PL_MOVE || m_ePL_STATE == PL_IDLE) && m_bPickUp && (m_iKeyState & INTERACTION))
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_PICKUP;
		switch (m_ePickUpType)
		{
		case CCody_Space::FUSE:
			if (nullptr != m_pPickUpTarget)
				static_cast<CFuseNPC*>(m_pPickUpTarget)->Set_Carried(L"Cody");
			break;
		}
	}
	else if ((m_ePL_STATE == PL_MOVE || m_ePL_STATE == PL_IDLE) && m_bColLever && (m_iKeyState & INTERACTION))
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_LEVER;
	}
	else if (m_bHangSwitch && (m_iKeyState & INTERACTION))
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_HANGSWITCH;
	}
	/*else if (m_ePL_STATE == PL_JUMP && m_bFloat)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FLOAT;

	}*/
}

void CCody_Space::Set_Position(char* tagBoneName)
{
	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName(tagBoneName);
	_float4x4 matBone = pBone->CombinedTranformationMatrix;
	_float4x4 matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	_float3 vPos = m_pTransformCom->Get_Position();
	matBone *= matWorld;
	m_pController->setFootPosition(PxExtendedVec3(matBone._41, matBone._42, matBone._43));
	m_pTransformCom->Set_Pos(_vec3(matBone._41, matBone._42, matBone._43));
	m_pTransformCom->Update_Component(m_fTimeDelta);
}

void CCody_Space::ChangeZ(const _float & fTimeDelta)
{
	if (m_bChangeZ)
	{
		if (false == m_bStartChange)
		{
			m_bStartChange = true;
			m_fChangeZ = m_fNextZ - m_fZ;
		}

		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime >= 1.0f)
		{
			m_fZ = m_fNextZ;
			m_bChangeZ = false;
			m_bStartChange = false;
			m_fChangeTime = 0.0f;
			return;
		}
		m_fZ += m_fChangeZ * fTimeDelta;

	}
}

void CCody_Space::Set_Late_State(const _float & fTimeDelta)
{
	switch (m_ePL_STATE)
	{
	case CPlayerValue::PL_IDLE:
	case CPlayerValue::PL_MOVE:
	case CPlayerValue::PL_SIT:
	case CPlayerValue::PL_RUN:
		/*case CPlayerValue::PL_JUMP:*/
	{
		if (true == m_bRideObj)
		{
			_vec3* vEventDir = ((_vec3*)m_pAttachTarget->On_Attach(this));
			if (!vEventDir)
				return;
			_vec3 vDir = *vEventDir;
			//m_pController->setPosition(m_pController->getPosition() + PxExtendedVec3(vDir.x, vDir.y, vDir.z));
			PxU32 flags = 0;
			flags = m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
			//m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + vDir);

			if (true == m_bRot)
			{
				m_pTransformCom->Set_AddAngleY(m_fAttachAngle);
				m_pTransformCom->Update_Component(fTimeDelta);
			}
		}
		break;
	}
	case CPlayerValue::PL_PUSH:
		Late_Push_State(fTimeDelta);
		break;
	case CPlayerValue::PL_CLIMBING:
	{
		if (true == m_bAttach)
		{
			//m_pController->move(PxVec3(m_vGravitation.x, m_vGravitation.y, m_vGravitation.z), 0, fTimeDelta, m_ControllFilter);
			//Set_SimulationState(false);
			//m_bStopGravity = true;
			//m_bPhys = false;
			_vec3* vEventDir = ((_vec3*)m_pAttachTarget->On_Attach(this));
			if (!vEventDir)
				return;
			_vec3 vDir = *vEventDir;
			//m_pController->setPosition(m_pController->getPosition() + PxExtendedVec3(vDir.x, vDir.y, vDir.z));
			PxU32 flags = 0;
			flags = m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
			//m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + vDir);

			if (flags == PxControllerCollisionFlag::eCOLLISION_DOWN)
			{
				m_bAttach = false;
			}

			if (true == m_bRot)
			{
				m_pTransformCom->Set_AddAngleY(m_fAttachAngle);
				m_pTransformCom->Update_Component(fTimeDelta);
			}
		}
		break;
	}
	case CPlayerValue::PL_MISSILE:
		Late_Missile_Ride(fTimeDelta);
		break;
	}
}

void CCody_Space::Late_Push_State(const _float & fTimeDelta)
{
	switch (m_ePickUpType)
	{
	case CPlayerValue::SPACE_WALL:
		if (m_pPickUpTarget && m_pPickUpTarget->Is<CSpaceWall>()->Get_Move())
			InteractionMove(m_pPickUpTarget->Is<CSpaceWall>()->Get_Dir(), fTimeDelta);
		break;
	}
}

_bool CCody_Space::JumpTo(_float fJumpPower, _float fTime, _vec3 vGoalPos, _float fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3	vGoal = vGoalPos;
	_float fDis = D3DXVec3Length(&_vec3(vGoalPos - vPos));
	vPos.y = 0.0f;
	vGoalPos.y = 0.0f;
	_vec3 vDir = vGoalPos - vPos;
	if (fTime <= m_fCurJumpToTime)
	{
		m_bJumpTo = false;
		_vec3 vPos = m_pTransformCom->Get_Position();
		PxTransform pxTransform = m_pController->getActor()->getGlobalPose();
		pxTransform.p.x = vPos.x;
		pxTransform.p.y = vPos.y;
		pxTransform.p.z = vPos.z;
		m_fGravityTime = 0.0f;
		m_fCurJumpToTime = 0.0f;

		//m_pController->getActor()->setGlobalPose(pxTransform);
		//m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
		//m_pController->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		return true;
	}

	if (false == m_bJumpTo)
	{
		vPos = m_pTransformCom->Get_Position();

		m_vStartPos = vPos;
		m_fJumpPower = fJumpPower;
		m_vJumpToGoalDir = vDir;
		m_fJumpToGoalTime = fTime;
		m_bJumpTo = true;
		m_fTargetHeight = vGoal.y - vPos.y;
		m_fJumpToGravity = -2.0f * (m_fTargetHeight - (fJumpPower * m_fJumpToGoalTime)) / (m_fJumpToGoalTime * m_fJumpToGoalTime);
		m_bGround = false;
	}
	_float fGoalTime = 1.0f / m_fJumpToGoalTime;

	m_fCurJumpToTime += fTimeDelta;
	_float fHeight = fJumpPower * m_fCurJumpToTime - 1.0f / 2.0f * m_fJumpToGravity * (m_fCurJumpToTime * m_fCurJumpToTime);
	fHeight += m_vStartPos.y;

	_vec3 vGolDir = m_vJumpToGoalDir * fTimeDelta * (1.0f / fGoalTime);

	vPos = m_pTransformCom->Get_Position();
	vPos.y = fHeight;

	/*m_pController->move(PxVec3(vGolDir.x, vGolDir.y, vGolDir.z), 0, fTimeDelta, m_ControllFilter);
	PxTransform pxTransform = m_pController->getActor()->getGlobalPose();
	pxTransform.p.y = fHeight;
	m_pController->getActor()->setGlobalPose(pxTransform);*/

	PxExtendedVec3 pxPos;
	pxPos = m_pController->getFootPosition();

	pxPos.x = pxPos.x + vGolDir.x;
	pxPos.y = fHeight;
	pxPos.z = pxPos.z + vGolDir.z;
	//m_pController->setPosition(pxPos);

	/*m_pController->move(vGolDir)*/
	m_pTransformCom->Set_Pos(vPos + vGolDir);

	return false;
}

void CCody_Space::Shoot(const _float& fTimeDelta)
{
	if (!m_bShoot)
		return;

	m_bStopGravity = true;

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &m_vShootDir);
	vDir = vDir*fTimeDelta*m_fShootPower;

	PxControllerCollisionFlags flag;

	flag |= m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
	flag |= m_pController->move(PxVec3(0.f, m_fGravity, 0.f)*m_fShootTime*m_fShootTime, 0, fTimeDelta, m_ControllFilter);
	if (flag)
	{
		//m_bRideGround = true;
		m_bShoot = false;
		m_bStopGravity = false;
	}
	m_fShootTime += fTimeDelta;
}

_bool CCody_Space::MoveTo(const _float& fTimeDelta, _float fGoalTime, _vec3 vGoalPos, _bool bExceptY, _bool bSimilar, _float fDis)
{
	if (false == m_bMoveToStart)
	{
		_vec3 vLook, vPos;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vPos = m_pTransformCom->Get_Position();
		if (true == bExceptY)
			vPos.y = 0.0f;

		m_fMoveToTime = 0.0f;
		m_bMoveToStart = true;
		m_vMoveToDir = vGoalPos - vPos;

		if (true == bSimilar)
		{
			_float fGoalDis = D3DXVec3Length(&m_vMoveToDir);
			if (fGoalDis <= fDis)
			{
				m_bMoveToStart = false;
				return true;
			}
		}
	}

	m_fMoveToTime += fTimeDelta;

	if (m_fMoveToTime >= fGoalTime)
	{
		InteractionSetPos(vGoalPos, fTimeDelta);
		m_bMoveToStart = false;
		return true;
	}
	else
	{
		_float3 vDir = m_vMoveToDir * fTimeDelta * (1.0f / fGoalTime);
		if (m_bPhys)
			m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
		m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + vDir);
	}

	return false;
}

_bool CCody_Space::RotTo(const _float & fTimeDelta, _float fGoalTime,/* _vec3 vTargetLook, */_vec3 vGoalLook, _bool bSimilar/*, _float fRad*/)
{
	if (false == m_bRotToStart)
	{
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		m_fRotToTime = 0.0f;
		m_bRotToStart = true;

		_float fCosTh = D3DXVec3Dot(&vLook, &vGoalLook);
		if (fCosTh >= 1.0f)
			fCosTh = 1.0f;
		m_fRotToAngle = acosf(fCosTh);

		if (bSimilar && fCosTh >= 0.98)
		{
			return true;
			m_bRotToStart = false;
		}

		_vec3 vPerp;
		D3DXVec3Cross(&vPerp, &_vec3(0.0f, 1.0f, 0.0f), &vGoalLook);

		_float fDir = D3DXVec3Dot(&vPerp, &vLook);

		if (fDir > 0.0f)
			m_fRotToAngle *= -1;
	}

	m_fRotToTime += fTimeDelta;

	if (m_fRotToTime >= fGoalTime)
	{
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);

		_float fCosTh = D3DXVec3Dot(&vLook, &vGoalLook);
		if (fCosTh >= 1.0f)
			fCosTh = 1.0f;

		m_fRotToAngle = acosf(fCosTh);

		_vec3 vPerp;
		D3DXVec3Cross(&vPerp, &_vec3(0.0f, 1.0f, 0.0f), &vGoalLook);

		_float fDir = D3DXVec3Dot(&vPerp, &vLook);

		if (fDir > 0.0f)
			m_fRotToAngle *= -1;

		m_pTransformCom->Set_AddAngleY(m_fRotToAngle);

		m_bRotToStart = false;
		return true;
	}
	else
		m_pTransformCom->Set_AddAngleY(m_fRotToAngle * fTimeDelta * (1.0f / fGoalTime));

	return false;
}

_bool CCody_Space::RotTo(_vec3 vGoalLook)
{
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	float fCosTh = D3DXVec3Dot(&vLook, &vGoalLook);
	if (fCosTh >= 1.0f)
		fCosTh = 1.0f;

	m_fRotToAngle = acosf(fCosTh);
	_vec3 vPerp;
	D3DXVec3Cross(&vPerp, &_vec3(0.0f, 1.0f, 0.0f), &vGoalLook);

	_float fDir = D3DXVec3Dot(&vPerp, &vLook);

	if (fDir > 0.0f)
		m_fRotToAngle *= -1;

	m_pTransformCom->Set_AddAngleY(m_fRotToAngle);

	return _bool();
}

void CCody_Space::Set_Pos(char * tagBoneName, const _float& fTimeDelta)
{
	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName(tagBoneName);
	_float4x4 matBoneMatrix = pBone->CombinedTranformationMatrix;
	_float4x4 matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matBoneMatrix *= matWorld;

	PxTransform pxTransform = m_pController->getActor()->getGlobalPose();
	pxTransform.p.x = matBoneMatrix._41;
	pxTransform.p.y = matBoneMatrix._42;
	pxTransform.p.z = matBoneMatrix._43;

	m_pController->setFootPosition(PxExtendedVec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z));
	m_pTransformCom->Set_Pos(_vec3(matBoneMatrix._41, matBoneMatrix._42, matBoneMatrix._43));
	m_pTransformCom->Update_Component(fTimeDelta);
}

_bool CCody_Space::Ground(const _float& fTimeDelta)
{
	m_fGroundTime += fTimeDelta;
	if (false == m_bGround && m_fGroundTime >= 0.4f)
	{
		return true;
	}
	else if (true == m_bGround)
	{
		m_fGroundTime = 0.0f;
	}

	return false;
}

void CCody_Space::BoneIntoPosition(char * tagBoneName, _vec3 vTargetPosition)
{
	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName(tagBoneName);
	_matrix matBone = pBone->CombinedTranformationMatrix;
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld = matBone * matWorld;
	_vec3 vBonePos = _vec3(matWorld._41, matWorld._42, matWorld._43);
	_vec3 vTargetPos = vTargetPosition;

	m_vTargetDir = vTargetPos - vBonePos;

	m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + m_vTargetDir);
	m_pController->setFootPosition(PxExtendedVec3(m_pTransformCom->Get_Position().x, m_pTransformCom->Get_Position().y, m_pTransformCom->Get_Position().z));


}

_bool CCody_Space::BoneIntoPosition(char * tagBoneName, _vec3 vTargetPosition, _float fGoalTime, _float fTimeDelta)
{

	if (false == m_bBoneIntoPosition)
	{
		m_bBoneIntoPosition = true;

		const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName(tagBoneName);
		_matrix matBone = pBone->CombinedTranformationMatrix;
		_matrix matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		matWorld = matBone * matWorld;
		_vec3 vBonePos = _vec3(matWorld._41, matWorld._42, matWorld._43);
		_vec3 vTargetPos = vTargetPosition;

		m_vTargetDir = vTargetPos - vBonePos;

		m_vBIPGoalPos = m_pTransformCom->Get_Position() + m_vTargetDir;
	}

	m_fCurBIPTime += fTimeDelta;
	if (m_fCurBIPTime >= fGoalTime)
	{
		m_bBoneIntoPosition = false;
		m_fCurBIPTime = 0.0f;
		m_pTransformCom->Set_Pos(m_vBIPGoalPos);
		m_pController->setFootPosition(PxExtendedVec3(m_pTransformCom->Get_Position().x, m_pTransformCom->Get_Position().y, m_pTransformCom->Get_Position().z));
		return true;
	}
	m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + (m_vTargetDir * fTimeDelta * (1.0f / fGoalTime)));
	m_pController->setFootPosition(PxExtendedVec3(m_pTransformCom->Get_Position().x, m_pTransformCom->Get_Position().y, m_pTransformCom->Get_Position().z));
	return false;
}

_vec3 CCody_Space::DirBetweenBones(char * tagBoneName1, char * tagBoneName2)
{
	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName(tagBoneName1);
	_matrix matBone1 = pBone->CombinedTranformationMatrix;
	pBone = m_pMeshCom->Get_FrameByName(tagBoneName2);
	_matrix matBone2 = pBone->CombinedTranformationMatrix;

	_vec3 vRoot = _vec3(matBone1._41, matBone1._42, matBone1._43);
	_vec3 vHead = _vec3(matBone2._41, matBone2._42, matBone2._43);

	_vec3 vCurDir = vHead - vRoot;
	return vCurDir;
}

_bool CCody_Space::Lock()
{
	CCodyCamera* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>();
	if (nullptr == pCamera)
		pCamera = Engine::Get_GameObject(L"GameObject", L"MainCamera")->Is<CCodyCamera>();

	_bool bLock = pCamera->Get_CameraFix();
	return pCamera->Get_CameraFix();
}

void CCody_Space::InteractionMove(_vec3 vDir, const _float& fTimeDelta)
{
	_vec3 vPos;
	PxExtendedVec3 pxPos;
	m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);

	pxPos = m_pController->getFootPosition();
	vPos.x = (_float)pxPos.x;
	vPos.y = (_float)pxPos.y;
	vPos.z = (_float)pxPos.z;

	if (m_bPhys)
		m_pTransformCom->Set_Pos(vPos);

	m_pTransformCom->Update_Component(fTimeDelta);
}

void CCody_Space::InteractionSetPos(_vec3 vPos, const _float& fTimeDelta)
{
	if (m_bPhys)
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));

	m_pTransformCom->Set_Pos(vPos);

	m_pTransformCom->Update_Component(fTimeDelta);
}

void CCody_Space::Idle_State(const _float & fTimeDelta)
{
	if (Ground(fTimeDelta))
	{
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_IDLE;
	}
	else if (m_bEventJump && m_bOnPlatform)
	{
		m_bEventJumpStart = true;
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_IDLE;
		m_bEventJump = false;
	}
	else if (m_bRide && (m_iKeyState & INTERACTION))
	{
		m_ePL_STATE = PL_MISSILE;
		m_ePre_State = PL_IDLE;
	}
	else if (true == m_bHandle && m_iKeyState & INTERACTION)
	{
		m_ePL_STATE = PL_HANDLE;
		m_ePre_State = PL_IDLE;
	}
	else if (true == m_bPush && m_iKeyState & INTERACTION)
	{
		m_ePL_STATE = PL_PUSH;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & RUN)
	{
		m_ePL_STATE = PL_RUN;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & SIT)
	{
		m_ePL_STATE = PL_SIT;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & JUMP)
	{
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		m_ePL_STATE = PL_MOVE;
		m_ePre_State = PL_IDLE;
	}
}

void CCody_Space::Move_State(const _float & fTimeDelta)
{
	if (Ground(fTimeDelta))
	{
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_IDLE;
		m_fCurMoveTime = 0.0f;
	}
	else if (m_bRide && (m_iKeyState & INTERACTION))
	{
		m_ePL_STATE = PL_MISSILE;
		m_ePre_State = PL_IDLE;
		m_fCurMoveTime = 0.0f;
	}
	else if (m_bEventJump && m_bOnPlatform)
	{
		m_bEventJumpStart = true;
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_IDLE;
		m_bEventJump = false;
		m_fCurMoveTime = 0.0f;
	}
	else if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_MOVE;
		m_fCurMoveTime = 0.0f;
	}
	else if (m_iKeyState & RUN)
	{
		m_ePL_STATE = PL_RUN;
		m_ePre_State = PL_MOVE;
		m_fCurMoveTime = 0.0f;
	}
	else if (m_iKeyState & SIT)
	{
		m_ePL_STATE = PL_SIT;
		m_ePre_State = PL_MOVE;
		m_fCurMoveTime = 0.0f;
	}
	else if (m_iKeyState & JUMP)
	{
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_MOVE;
		m_fCurMoveTime = 0.0f;
	}
	else if (!(m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT)))
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_MOVE;
		m_fCurMoveTime = 0.0f;
	}
	else
	{
		switch (m_eCameraMode)
		{
		case CPlayerValue::Camera_2D:
			Move2D(fTimeDelta);
			break;
		case CPlayerValue::Camera_3D:
			Move3D(fTimeDelta, true);
			break;
		}

	}
}

void CCody_Space::Jump_State(const _float & fTimeDelta)
{
	// 점프 내려찍기
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		if (false == m_bPlaySound[PLAY_WHOOSH])
		{
			m_bPlaySound[PLAY_WHOOSH] = true;
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_whoosh.wav", CSoundMgr::CHANNELID::Cody_Jump, m_fEffectvolume);
		}

		if (!m_bJump[PL_GROUNDPOUND])
		{
			// 이 애니메이션이 끝나면 다음 if문으로 넘어감

			// 처음 한바퀴 도는 동작에서 중력값을 받지 않기 위해
			m_bStopGravity = true;
		}
		else if (m_bJump[PL_GROUNDPOUND])
		{
			// 처음 애니메이션이 끝나면 다시 중력값을 받는다.
			m_bStopGravity = false;

			// m_fHeight를 이용하여 점프값을 구한걸 중력값으로 바꿔줌
			// 현재는 점프 상태
			m_fHeight = -1.0f;
			if (m_bJump[PL_GROUNDPOUND_STOP] == true)
			{
				Clear(PL_JUMP);
				m_ePL_STATE = PL_IDLE;
				m_ePre_State = PL_JUMP;
			}

		}
		return;
	}

	if (m_bHang)
	{
		m_ePre_State = PL_JUMP;
		m_ePL_STATE = PL_HANG;
		Clear(PL_JUMP);
		m_bHang = false;
		return;
	}

	// 무중력?
	if (m_bFloat && m_fOrgHeight >= m_pTransformCom->Get_Position().y && m_bJump[PL_JUMPSTART])
	{
		Clear(PL_JUMP);
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FLOAT;
		// 애니메이션
		Set_ClearJump();
		return;
	}
	else if (m_bFreeGravity)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FREEGRAVITY;

		Clear(PL_JUMP);
		Set_ClearJump();
		return;
	}
	//else if (true == m_bAttach && true == m_bJump[PL_1STJUMP] && m_fHeight <= 0.0f)
	//{
	//	Clear(PL_JUMP);
	//	m_ePL_STATE = m_ePre_State;
	//	m_ePL_STATE = PL_ATTACH;
	//	Set_ClearJump();

	//	return;
	//}

	// 점프 시작
	if (!m_bJump[PL_1STJUMP] == true && !m_bJump[PL_JUMPSTART])
	{
		m_eCodyAnim = Cody_Space_Jump;
		m_fJumpTime = 0.0f;
		m_bJump[PL_1STJUMP] = true;
		m_bJump[PL_JUMPSTART] = true;
		if (false == m_bPlaySound[PLAY_JUMP])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_Jump.wav", CSoundMgr::CHANNELID::Cody_Jump, m_fEffectvolume);
			m_bPlaySound[PLAY_JUMP] = true;
		}
	}

	// 점프 탈출 조건
	if (false == m_bJump[PL_JUMPSTART])
	{
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			m_ePL_STATE = PL_MOVE;
			m_ePre_State = PL_JUMP;
		}
		else
		{
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_JUMP;
		}

		Clear_Jump();
		/*m_bStopGravity = false;
		ZeroMemory(&m_bJump, sizeof(m_bJump));*/
		return;
	}

	// 점프 대쉬
	// 점프 대쉬가 끝나고 내려올떄 부자연스러움
	if (m_bJump[PL_JUMPDASH] && !m_bJump[PL_JUMPDASHSTOP] && m_ePre_State != PL_FLOAT && false == m_bEventJumpStart)
	{
		// 점프대쉬중에 중력을 안받기 위해 true로
		m_bStopGravity = true;
		m_fJumpTime = 0.75f;
		m_bJumpDash = true;
		Dash(fTimeDelta, 0.05f, 1.0f);
		//Dash(fTimeDelta, 2.3f);

		if (false == m_bPlaySound[PLAY_JUMP_DASH])
		{
			m_bPlaySound[PLAY_JUMP_DASH] = true;
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_Jump_Dash.wav", CSoundMgr::CHANNELID::Cody_Jump, m_fEffectvolume);
		}

		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			_float3 vPos, vDir;
			Move(fTimeDelta, &vPos, &vDir);

			_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta * 0.5f;
			m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
		}

		Set_JumpDash();

		return;
	}

	// 이단 점프
	if (!m_bJump[PL_2NDJMPSTART] && m_bJump[PL_2NDJUMP] && m_ePre_State != PL_FLOAT && false == m_bEventJumpStart)
	{
		m_bJump[PL_2NDJMPSTART] = true;
		m_fJumpTime = 0.0f;
		// 애니메이션
		Set_2ndJump(true);
		if (false == m_bPlaySound[PLAY_DOUBLE_JUMP])
		{
			m_bPlaySound[PLAY_DOUBLE_JUMP] = true;
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_Double_Jump.wav", CSoundMgr::CHANNELID::Cody_Jump, m_fEffectvolume);
		}
	}

	// 점프대쉬가 아닐때만 이동과 점프를
	if (false == m_bJumpDash)
	{
		// 이동 조건
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			_float3 vPos, vDir;
			Move(fTimeDelta, &vPos, &vDir);

			_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta;
			m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
		}
		_float fJumpPower = m_fJumpPower;
		if (m_bEventJumpStart)
		{
			if (m_fJumpTime <= 0.1f && m_eCodyAnim == Cody_Space_Jump_Fall)
				m_eCodyAnim = Cody_Space_Jump;

			switch (m_iSize)
			{
			case 0:
				fJumpPower = 4.f;
				//#저하늘의별
				Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>()->Set_Mode(CAMERA_TRACING, this);
				m_bSceneWait = true;
				break;
			case 1:
				fJumpPower = m_fEventJumpPower;
				break;
			case 2:
				m_ePre_State = m_ePL_STATE;
				m_ePL_STATE = PL_IDLE;

				Clear(PL_JUMP);
				Set_ClearJump();
				return;
				break;
			default:
				break;
			}
			if (m_bSceneWait && m_fJumpTime > 3.f)
			{
				Set_PlayerDead();
				Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>()->Set_Mode(CAMERA_DEFAULT);

			}
		}
		// 점프값

		m_fJumpTime += fTimeDelta * 1.2f;
		m_fHeight = m_fGravity * m_fJumpTime * m_fJumpTime + fJumpPower * m_fJumpTime;
	}
}

void CCody_Space::Sit_State(const _float & fTimeDelta)
{
	if (!(m_iKeyState & SIT))
	{
		m_fHalfHeight = m_fShapeRadius + (m_fShapeHeight * 0.5f);
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			m_ePL_STATE = PL_MOVE;
			m_ePre_State = PL_SIT;
		}
		else
		{
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_SIT;
		}

		PxExtendedVec3 pxPos = m_pController->getFootPosition();
		m_pController->resize(m_fShapeHeight);
		//m_pController->setFootPosition(pxPos);
		return;
	}

	m_fHalfHeight = m_fShapeRadius + (m_fShapeHeight * 0.3f * 0.5f);
	m_pController->resize(m_fShapeHeight * 0.3f);
	_float3 vPos, vDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = _float3(0.0f, 0.0f, 0.0f);

	// 앉았을때 이동유무
	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		// 애니메이션
		Set_CrouchMove(true);
		Move(fTimeDelta, &vPos, &vDir);

		_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta * 0.3f;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
	}
	else
		// 애니메이션
		Set_CrouchMove(false);

}

void CCody_Space::Run_State(const _float & fTimeDelta)
{
	if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_RUN;
	}
	else if (m_iKeyState & SIT)
	{
		m_ePL_STATE = PL_SIT;
		m_ePre_State = PL_RUN;
	}
	else if (!(m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT)))
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_RUN;
	}
	else if (m_iKeyState & JUMP)
	{
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_RUN;
	}
	else
	{
		_float3 vPos, vDir;

		Move(fTimeDelta, &vPos, &vDir);

		_vec3 vDstPos = vDir * m_fMaxSpeed * 2.0f * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
	}
}

void CCody_Space::Dash_State(const _float & fTimeDelta)
{
	if (m_bDash[PL_1STDASH])
	{
		Dash(fTimeDelta, 2.0f);
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
			_float3 vPos, vDir;
			Move(fTimeDelta, &vPos, &vDir, 90.0f);
		}
	}
	else
	{
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			m_ePL_STATE = PL_MOVE;
			m_ePre_State = PL_DASH;
			m_fCurMoveTime = m_fGoalTime;
		}
		else
		{
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_DASH;
		}
	}
}

void CCody_Space::Hang_State(const _float & fTimeDelta)
{
	if (!m_bHangStart)
	{
		m_bHangStart = true;
		RotTo(m_vClimbingDir);
	}

	if (m_bEndRise)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_HANG;
		m_bEndRise = false;
		m_bStopGravity = false;
		m_bHangStart = false;
		Set_Pos("Align", m_fTimeDelta);
		m_pMeshCom->Set_Animationset(Cody_Space_Idle, 1.0f, 0.01f);

		return;
	}
	if (m_iKeyState & JUMP)
	{
		// 애니메이션
		Set_Rise();
	}
	else if (m_iKeyState & INTERACTION2)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FALL;
		m_bEndRise = false;
		m_bStopGravity = false;
		m_bHangStart = false;

		const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("FaceManager");
		_float4x4 matBone = pBone->CombinedTranformationMatrix;
		_float4x4 matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		_float3 vPos = m_pTransformCom->Get_Position();
		matBone *= matWorld;
		m_pController->setFootPosition(PxExtendedVec3(matBone._41, matBone._42, matBone._43));
		m_pTransformCom->Set_Pos(_vec3(matBone._41, matBone._42, matBone._43));
		m_pTransformCom->Update_Component(m_fTimeDelta);
		m_pMeshCom->Set_Animationset(Cody_Space_Fall, 1.0f, 0.01f);
		return;
	}

	m_bStopGravity = true;
	m_fGravityTime = 0.0f;
}

void CCody_Space::Climbing_State(const _float & fTimeDelta)
{
	m_fGravityTime = 0.0f;
	if (false == m_bClimbingStart)
	{
		m_eCodyAnim = Cody_Space_Wall_Idle;
		m_bClimbingStart = true;
		RotTo(m_vClimbingDir);
	}

	if (m_bGround)
	{
		m_bAttach = false;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_CLIMBING;
		m_bStopGravity = false;
		m_bClimbingJump = false;
		m_bClimbingStart = false;
		return;
	}

	if (m_bHang)
	{
		m_bAttach = false;
		m_ePL_STATE = PL_HANG;
		m_ePre_State = PL_CLIMBING;
		m_bStopGravity = false;
		m_bClimbingJump = false;
		m_bClimbingStart = false;
		m_bHang = false;
		//RotTo(m_vClimbingDir);
		return;
	}

	if (m_iKeyState & INTERACTION2)
	{
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_CLIMBING;
		Clear(PL_CLIMBING);
		return;
	}

	// 중력을 끈다.
	m_bStopGravity = true;

	if (m_iKeyState & JUMP && false == m_bClimbingJump)
	{
		if (m_bAttach)
		{
			Set_SimulationState(true);
			m_bPhys = true;
			m_bAttach = false;
			m_pController->setFootPosition(PxExtendedVec3(m_pTransformCom->Get_Position().x, m_pTransformCom->Get_Position().y, m_pTransformCom->Get_Position().z));
		}

		m_bClimbingJump = true;
		// 애니메이션
		Set_ClimbingJump();
		m_fJumpCurTime = m_fJumpCoolTime + 2.f;
	}
	else if (true == m_bClimbingJump)
	{
		m_fJumpTime += fTimeDelta;
		m_fHeight = m_fClimbingGravity * m_fJumpTime * m_fJumpTime + m_fClimbingJumpPower * m_fJumpTime;
		PxVec3 vDir = -PxVec3(m_vClimbingDir.x, m_vClimbingDir.y, m_vClimbingDir.z) * m_fMaxSpeed * fTimeDelta;

		vDir.y += m_fHeight;
		PxU32 flags = m_pController->move(vDir, 0, fTimeDelta, m_ControllFilter);

		if (!m_bRotStart)
		{
			//m_bRotStart = RotTo(fTimeDelta, 0.6f, _vec3(1.0f, 0.0f, 0.0f));
		}
		_float fAngle = 90.0f * fTimeDelta * (1.0f / 0.6f);
		m_pTransformCom->Set_AddAngleY(D3DXToRadian(fAngle));

		if (PxControllerCollisionFlag::eCOLLISION_DOWN & flags)
		{
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_CLIMBING;
			m_bStopGravity = false;
			m_bClimbingJump = false;
			m_bClimbingStart = false;
			return;
		}


		if (true == m_bClimbing)
		{
			/*_float fAngle = 180.0f;
			m_pTransformCom->Set_AddAngleY(D3DXToRadian(fAngle));*/
			RotTo(m_vClimbingDir);

			m_bClimbingJump = false;
			m_bClimbing = false;
			m_fJumpTime = 0.0f;

			// 벽에 닿았을때 애니메이션을 Idle상태로 바꿔준다.
			// 애니메이션
			Set_ClimbingJumpEnd();

			return;
		}
	}

	else if (false == m_bAttach)
	{
		PxU32 flags = m_pController->move(PxVec3(0.0f, -0.05f, 0.0f), 0, fTimeDelta, m_ControllFilter);
		if (flags == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_bGround = true;
		}
	}

	//m_fHeight += -3.0f * fTimeDelta * fTimeDelta;
}

void CCody_Space::Slide_State(const _float& fTimeDelta)
{
	if (m_bRideGround)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = m_ePL_STATE;
		m_bRideGround = false;
		m_bSlideEnd = false;
		return;
	}

	if (true == m_bSlideEnd)
	{
		Shoot(fTimeDelta);
		return;
	}

	if (m_vecSlidePos.empty())
		return;

	_vec3 vPos = m_pTransformCom->Get_Position();

	if (m_bSlideStart)
	{
		//m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		m_iSlideIndex = 0;
		m_iPathCount = 0;
		m_bSlideStart = false;
		m_iInterval = (m_vecSlidePos[m_iPathCount])->size();
		if (m_iInterval / 20 != 0)
			m_iInterval = size_t(m_iInterval / 20.f);
		else
			m_iInterval = 1;
		m_bPhys = false;
	}
	if (m_iPathCount >= m_vecSlidePos.size() - 1)
	{
		if (m_iSlideIndex >= m_vecSlidePos[m_iPathCount]->size())
		{

			//m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
			m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
			_vec3 vTargetPos1 = (*m_vecSlidePos[m_iPathCount])[m_vecSlidePos[m_iPathCount]->size() - 1];
			_vec3 vTargetPos2 = (*m_vecSlidePos[m_iPathCount])[m_vecSlidePos[m_iPathCount]->size() - 1 - m_iInterval];
			D3DXVec3Normalize(&vTargetPos1, &(vTargetPos1 - vTargetPos2));

			Set_Shoot(vTargetPos1, m_fShootPower, m_fShootDuration);
			m_bPhys = true;
			m_bSlide = false;
			//////////////////////////////////////////////////////////////////////////
			m_vecSlidePos.clear();
			m_bSlideEnd = true;
			return;
		}
	}
	else
	{
		if (m_iSlideIndex >= m_vecSlidePos[m_iPathCount]->size())
		{
			++m_iPathCount;
			m_iInterval = m_vecSlidePos[m_iPathCount]->size();
			if (m_iInterval / 20 != 0)
				m_iInterval = size_t(m_iInterval / 20.f);
			else
				m_iInterval = 1;
			m_iSlideIndex = 0;
		}
	}
	if (m_iSlideIndex != 0)
		m_bSlide = true;

	_vec3 vTargetPos = (*m_vecSlidePos[m_iPathCount])[m_iSlideIndex];
	_vec3 vDir = vTargetPos - vPos;

	if (D3DXVec3Length(&(vDir)) > 0.5f*m_fSlideSpeed)
	{
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransformCom->Move_Pos(&vDir, m_fSlideSpeed * 10, fTimeDelta);
	}
	else
	{
		m_pTransformCom->Set_Pos(vTargetPos);
		m_iSlideIndex += (_int)m_iInterval;
		return;
	}

}

void CCody_Space::Ride_State(const _float& fTimeDelta)
{
	// 탈출조건
	if (true == m_bRideJumpMax)
	{
		// 껏던 피직스를 다시 켜준다.
		Set_SimulationState(true);
		// 내려오는 애니메이션 마지막 부분의 위치로 플레이어를 이동
		D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)m_pMeshCom->Get_FrameByName("Align");
		_float4x4 matBone = pBone->CombinedTranformationMatrix;
		const _matrix* matWorld = m_pTransformCom->Get_WorldMatrix();

		_float4x4 matCombined = matBone * *matWorld;
		_vec3 vPos = _vec3(matCombined._41, matCombined._42, matCombined._43);
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		//////////////////////////////////////////////////////////////////////////

		// 이동시킨후 false로 바꿔준 후에 fall상태로 바꿔준다.
		Clear(PL_RIDE);
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_RIDE;
		m_bStopGravity = false;
		m_bGround = false;
		m_bPhys = true;

		return;
	}
	// 탑승시에 e키를 눌렀을경우 탈출조건으로
	if ((m_bRideStart && (m_iKeyState & INTERACTION)) || m_bForceRideOff)
	{
		// 애니메이션
		Set_TakeOff();
		m_bForceRideOff = false;

		return;
	}

	// 맨처음 들어왔을때 여기서부터
	// 머리의 뼈위치 구하는식
	Set_SimulationState(false);

	m_bStopGravity = true;
	m_bPhys = false;
	D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)(m_pPickUpTarget)->Get_Component(m_tagMash, COMPONENTID::ID_STATIC)->Is<CDynamicMesh>()->Get_FrameByName(m_tagBone);
	NULL_CHECK_RETURN(pBone, );

	_float4x4 matBone = pBone->CombinedTranformationMatrix;
	const _matrix* matHead = (m_pPickUpTarget)->Get_Component(m_tagTrans, COMPONENTID::ID_DYNAMIC)->Is<CTransform>()->Get_WorldMatrix();

	_matrix matCombined = matBone * *matHead;
	_vec3 vPos;
	vPos.x = matCombined._41;
	vPos.y = matCombined._42;
	vPos.z = matCombined._43;

	// Ride가 시작?각도와 방향을 한번만 구함
	if (!m_bRideStart)
	{
		m_fCurRot = 0.0f;
		// 회전 각도 구하는식
		_float3 vHeadLook, vLook;
		(m_pPickUpTarget)->Get_Component(m_tagTrans, COMPONENTID::ID_DYNAMIC)->Is<CTransform>()->Get_Info(INFO_LOOK, &vHeadLook);
		vHeadLook = *((_float3*)&(matHead->_31));

		vHeadLook.y = 0.0f;
		D3DXVec3Normalize(&vHeadLook, &vHeadLook);
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		vLook.y = 0.0f;
		D3DXVec3Normalize(&vLook, &vLook);

		_float fCosTh = D3DXVec3Dot(&vLook, &vHeadLook);
		m_fRideRot = acosf(fCosTh);
		m_bRideStart = true;

		// 회전방향 구하는식
		_float3 vPerp;
		D3DXVec3Cross(&vPerp, &_float3(0.0f, 1.0f, 0.0f), &vHeadLook);
		D3DXVec3Normalize(&vPerp, &vPerp);
		_float fDir = D3DXVec3Dot(&vPerp, &vLook);

		if (fDir > 0.0f)
			m_iRideDir = -1;
		else
			m_iRideDir = 1;
	}

	// 누적된 회전각도 목표치에 도달했을경우 회전을 멈추기 위해 누적함
	m_fCurRot += m_fRideRot * fTimeDelta;
	// 회전각도가 목표치에 도달할때까지 회전
	if (m_fCurRot <= m_fRideRot)
		m_pTransformCom->Set_AddAngleY(m_fRideRot * fTimeDelta * m_iRideDir);

	// 점프시에 원하는 위치에 도달하면 true가 되고 true가 된 후엔 매프레임마다 뼈위치로 위치를 이동시켜줌
	if (!m_bRideEnd)
		m_bRideEnd = JumpTo(10, 1.0f, vPos, fTimeDelta);
	// 이동식
	else
	{
		D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)m_pMeshCom->Get_FrameByName("Align");
		_float4x4 matBone = pBone->CombinedTranformationMatrix;

		_matrix matWorld, matRotX, matRotY;
		D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_vRideAngle.x));
		D3DXMatrixRotationZ(&matRotY, D3DXToRadian(m_vRideAngle.y));
		matWorld = matRotX * matRotY * matCombined;

		m_pTransformCom->Set_WorldMatrix(matWorld);
		// 이전것
		//_vec3 HozeEndPos = m_pPickUpTarget->Get_EventPos();
		//PxTransform HozeEnd(HozeEndPos.x, HozeEndPos.y, HozeEndPos.z);

		//HozeEnd.q *= PxQuat(D3DXToRadian(90 * m_iDir), PxVec3(0.f, 1.f, 0.f));
		//m_pTransformCom->Set_WorldMatrix(HozeEnd);

		if (m_iKeyState & MOVE_LEFT)
			(m_pPickUpTarget)->Move_Left();
		if (m_iKeyState & MOVE_RIGHT)
			(m_pPickUpTarget)->Move_Right();
		if (m_iKeyState & MOVE_DOWN)
			(m_pPickUpTarget)->Move_Up();
	}
}

void CCody_Space::Dead_State(const _float & fTimeDelta)
{
	m_bPhys = false;

	if (false == m_bDeadUI)
	{
		D3DXVec3Normalize(&m_vDeadSlide, &m_vDeadSlide);
		//m_pTransformCom->Move_Pos(&m_vDeadSlide, 3.5f, fTimeDelta);
		//m_pTransformCom->Update_Component(fTimeDelta);
		//_vec3 vPos = m_pTransformCom->Get_Position();
		//m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		if (m_eCameraMode != CPlayerValue::Camera_2D)
			Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CCodyCamera>()->Set_RayCheck(false);
	}

	if (false == m_bDeadStart)
	{
		m_bDeadStart = true;
		m_bDeadEffect = true;
		m_bStopGravity = true;
		m_bStencil = false;

		if (m_tObjInfo.eSceneID == RESOURCE_SB)
		{
			Engine::Set_CodyGreyScale(true);
			m_pDeadUI[0]->Set_Amount(0.f);
			m_pFade->Change_Color_From_To(0.7f, _vec4(0.f, 0.f, 0.f, 0.f), _vec4(0.f, 0.f, 0.f, 80.f));
			for (_int i = 0; i < 6; ++i)
			{
				m_pDeadUI[i]->Change_Color_From_To(0.4f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(255.f, 255.f, 255.f, 255.f));
				m_pDeadUI[i]->Set_Visible(true);
			}
			m_pDeadUI[2]->Change_Color_From_To(0.4f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(231.f, 76.f, 60.f, 255.f));
			m_pDeadUI[3]->Change_Color_From_To(0.4f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(236.f, 216.f, 162.f, 255.f));

			for (_int i = 0; i < 3; ++i)
				m_pDeadUI[i]->Change_Size_From_To(0.2f, _vec3(1600.f, 1600.f, 0.f), _vec3(400.f, 400.f, 0.f));
			m_pDeadUI[3]->Change_Size_From_To(0.2f, _vec3(240.f, 240.f, 0.f), _vec3(60.f, 60.f, 0.f));
			m_pDeadUI[4]->Change_Size_From_To(0.2f, _vec3(160.f, 160.f, 0.f), _vec3(40.f, 40.f, 0.f));
			m_pDeadUI[5]->Change_Size_From_To(0.2f, _vec3(500.f, 500.f, 0.f), _vec3(128.f, 128.f, 0.f));

			m_pProfile->Set_Visible(false);
			m_pHp->Set_Visible(false);
		}
	}
	else
	{
		m_fCurDeadTime += fTimeDelta;
		if (m_fCurDeadTime >= m_fGoalDeadTime)
		{

		}
	}
}

void CCody_Space::Lever_State(const _float & fTimeDelta)
{
	if (m_bSlap)
		Lever_Slap(fTimeDelta);
	else
		Lever_Grap(fTimeDelta);
}

void CCody_Space::PickUp_State(const _float & fTimeDelta)
{
	if (m_bInsert)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_PICKUP;
		Clear(PL_PICKUP);
		m_bInteractable = false;
		Insert_Fuse_End();
		return;
	}

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		_float3 vPos, vDir;

		Move(fTimeDelta, &vPos, &vDir);

		m_fCurMoveTime += fTimeDelta;
		if (m_fCurMoveTime < m_fGoalTime)
		{
			m_fSpeed = m_fMaxSpeed * m_fCurMoveTime * (1.0f / m_fGoalTime);
		}
		else
			m_fSpeed = m_fMaxSpeed;

		_vec3 vDstPos = vDir * m_fSpeed * 0.6f * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
		// 애니메이션
		Set_PickUpMove(true);
	}

	else
		// 애니메이션
		Set_PickUpMove(false);

	if (m_bInteractable)
	{
		if (m_iKeyState & INTERACTION)
		{
			// 애니메이션
			Set_Insert_Fuse();
			m_ePickUpType = PICKUPEND;
		}
		return;
	}

	if (m_bPickUpEnd)
	{
		const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("FaceManager");
		_float4x4 matBone = pBone->CombinedTranformationMatrix;
		_float4x4 matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		_float3 vPos = m_pTransformCom->Get_Position();
		matBone *= matWorld;
		m_pController->setFootPosition(PxExtendedVec3(matBone._41, matBone._42, matBone._43));

		Clear(PL_PICKUP);
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_PICKUP;
		if (nullptr != m_pPickUpTarget)
		{
			static_cast<CFuseNPC*>(m_pPickUpTarget)->Set_Drop();
			m_pPickUpTarget = nullptr;
		}
		return;
	}

	// 이벤트에서 상태를 바꿔준 후에 바로 들어오기 때문에
	// m_iKeyState & INTERACTION은 항상 true이기 때문에 맨처음엔 아래 조건을 안타기위해 m_bPickUpStart를 만듬
	if (m_iKeyState & INTERACTION && m_bPickUpStart && !m_bPickUpJump)
	{
		// 애니메이션
		Set_PickUpDrop();
		m_ePickUpType = PICKUPEND;
		return;
	}

	if (!m_bPickUpStart)
		m_bPickUpStart = true;

	if (m_iKeyState & JUMP && !m_bPickUpJump)
	{
		m_bPickUpJump = true;
		m_bStopGravity = true;
	}

	if (m_bPickUpJump)
	{
		m_fHeight = m_fGravity * m_fJumpTime * m_fJumpTime + m_fJumpPower * m_fJumpTime;
		PxVec3 vGravity = PxVec3(0.0f, m_fHeight, 0.0f);
		PxU32 flags = m_pController->move(vGravity, 0, fTimeDelta, m_ControllFilter);
		m_fJumpTime += fTimeDelta;
		if (flags && PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_bPickUpJump = false;
			m_bStopGravity = false;
			m_fJumpTime = 0.0f;
		}
	}
}
void CCody_Space::HangSwitch_State(const _float & fTimeDelta)
{
	if (m_bSuccess)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_IDLE;
		m_bJumpToEnd = false;
		m_bPhys = true;
		m_fGravityTime = 0.0f;
		m_bRotToStart = false;

		m_pPickUpTarget->Set_CodyOn(false);
		m_pController->setFootPosition(PxExtendedVec3(m_vSwitchEndPos.x, m_vSwitchEndPos.y, m_vSwitchEndPos.z));
		m_pTransformCom->Set_Pos(_vec3(m_vSwitchEndPos.x, m_vSwitchEndPos.y, m_vSwitchEndPos.z));
		m_pTransformCom->Update_Component(fTimeDelta);
		return;
	}

	if (m_iKeyState & INTERACTION2)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FALL;
		m_bJumpToEnd = false;
		m_bPhys = true;
		m_fGravityTime = 0.0f;
		m_bRotToStart = false;

		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));

		m_pPickUpTarget->Set_CodyOn(false);
		return;
	}

	if (true == Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>()->Get_SwitchReady() &&
		true == m_bSwitchReady)
	{
		// 애니메이션
		Set_HangSwitchSuccess();
		m_pPickUpTarget->Set_RotStart(true);
		m_pPickUpTarget->Set_CodyOn(true);
		return;
	}

	if (!m_bJumpToEnd)
	{
		m_bPhys = false;
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		vLook.y = 0.0f;
		D3DXVec3Normalize(&vLook, &vLook);

		RotTo(fTimeDelta, 1.0f, _vec3(1.0f, 0.0f, 0.0f));

		if (m_bJumpToEnd = JumpTo(20.f, 1.f, _vec3(50.8f, -0.6f, -3.3f) /* m_vGoalPos */, fTimeDelta))
			m_pTransformCom->Set_Pos(_vec3(50.8f, -0.6f, -3.3f));

		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		return;
	}
}

void CCody_Space::Set_Dir(_float3 * vDir, _int * iRotDir, _float * fCosTh)
{
	_float4x4		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_float3 vPlayerLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	// 이동할 방향 구하기
	_float3 vCamDir = _float3(0.0f, 0.0f, 0.0f);
	if (m_iKeyState & MOVE_UP)
	{
		_float3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_float3));
		vLook.y = 0;
		D3DXVec3Normalize(&vLook, &vLook);
		vCamDir += vLook;
	}
	if (m_iKeyState & MOVE_DOWN)
	{
		_float3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_float3));
		vLook.y = 0;
		D3DXVec3Normalize(&vLook, &vLook);
		vCamDir += -vLook;
	}
	if (m_iKeyState & MOVE_LEFT)
	{
		_float3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_float3));
		vRight.y = 0;
		D3DXVec3Normalize(&vRight, &vRight);
		vCamDir += -vRight;
	}
	if (m_iKeyState & MOVE_RIGHT)
	{
		_float3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_float3));
		vRight.y = 0;
		D3DXVec3Normalize(&vRight, &vRight);
		vCamDir += vRight;
	}

	D3DXVec3Normalize(&vCamDir, &vCamDir);
	if (nullptr != vDir)
		*vDir = vCamDir;

	_float3 vTest = _float3(0.1f, 0.0f, 0.1f);
	D3DXVec3Normalize(&vTest, &vTest);
	// 회전할 방향을 구하자
	_float3 vPerp;
	D3DXVec3Cross(&vPerp, &_float3(0.0f, 1.0f, 0.0f), &vCamDir);
	D3DXVec3Normalize(&vPerp, &vPerp);
	_float fDir = D3DXVec3Dot(&vPerp, &vPlayerLook);

	if (nullptr != iRotDir)
	{
		if (fDir > 0.0f)
			*iRotDir = -1;
		else
			*iRotDir = 1;
	}

	// 코사인TH를 구하자
	_float _fCosTh = D3DXVec3Dot(&vCamDir, &vPlayerLook);
	if (nullptr != fCosTh)
		*fCosTh = _fCosTh;
}

void CCody_Space::Float_state(const _float & fTimeDelta)
{
	m_bStopGravity = true;

	if (m_pPickUpTarget == nullptr || (m_iKeyState & JUMP && m_pPickUpTarget->Is<CFan>()->Get_Up()))
	{
		Clear(PL_FLOAT);
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_FLOAT;
		m_bStopGravity = false;
		m_fOrgHeight = m_pTransformCom->Get_Position().y;
		return;
	}

	_float3 vCodyPos = m_pTransformCom->Get_Position();
	vCodyPos.y = 0.0f;
	_float3 vTargetPos = m_vFanPos;
	_float vHeight = vTargetPos.y + m_fFlyHeight; // 상수 나중에 수정해야?Test
	vTargetPos.y = 0.0f;

	// 이동
	_vec3 vMoveDir = { 0.0f, 0.0f, 0.0f };
	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		_float3 vPos, vDir;
		Move(fTimeDelta, &vPos, &vDir);
		_vec3 vDstDir = vDir * 7.0f * fTimeDelta;
		vMoveDir = vDstDir;
		vMoveDir.y = 0.0f;
	}

	if (false == m_bFloatStart)
	{
		m_fFloatHeight = vHeight;
		m_bFloatStart = true;
	}

	_float fFloatCurDis = D3DXVec3Length(&(vTargetPos - vCodyPos));
	_vec3 vDir = { 0.0f, 0.0f, 0.0f };
	_vec3 vResultPos;
	if (fFloatCurDis >= 0.25f)
	{
		_float3 vFloatDir = vTargetPos - vCodyPos;
		D3DXVec3Normalize(&vFloatDir, &vFloatDir);

		vDir.x = vFloatDir.x * fTimeDelta * 5.0f;
		vDir.y = vFloatDir.y * fTimeDelta * 5.0f;
		vDir.z = vFloatDir.z * fTimeDelta * 5.0f;
	}

	if (m_pPickUpTarget->Is<CFan>()->Get_Up())
	{
		vResultPos = vDir + vMoveDir + vCodyPos;
		if (vResultPos.x >= vTargetPos.x + 2.0f || vResultPos.x <= vTargetPos.x + -2.0f
			|| vResultPos.z >= vTargetPos.z + 2.0f || vResultPos.z <= vTargetPos.z + -2.0f)
			vDir = vDir;
		else
			vDir += vMoveDir;

		_float fFloatHeight = m_pTransformCom->Get_Position().y;
		if (fFloatHeight <= m_fFloatHeight)
		{
			m_fHeight += fTimeDelta;
			if (m_fHeight >= 0.2f)
				m_fHeight = 0.2f;
			else if (m_fHeight <= 0.0f)
				m_fHeight = 0.0f;

			m_pController->move(PxVec3(0.0f, m_fHeight, 0.0f), 0, fTimeDelta, m_ControllFilter);
		}
	}
	else
	{
		m_fHeight = -0.6f;
		m_pController->move(PxVec3(0.0f, m_fHeight, 0.0f), 0, fTimeDelta, m_ControllFilter);
		if (true == m_bColWing)
		{
			m_ePL_STATE = PL_DEAD;
			m_ePre_State = PL_FLOAT;
		}
	}

	m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
}

void CCody_Space::Fall_State(const _float & fTimeDelta)
{
	if (m_bGround)
	{
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			m_ePL_STATE = PL_MOVE;
			m_ePre_State = PL_FALL;
		}
		else
		{
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_FALL;
		}
		Clear(PL_FALL);
		return;
	}
	else if (m_bFreeGravity)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FREEGRAVITY;

		m_bFallStart = false;
		return;
	}

	if (false == m_bFallStart)
	{
		m_bFallStart = true;
	}

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		_float3 vPos, vDir;

		Move(fTimeDelta, &vPos, &vDir);

		_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
	}

}

void CCody_Space::Update_Time(const _float& fTimeDelta)
{
	if (m_ePL_STATE == PL_JUMP || m_bClimbingJump)
		m_fJumpCurTime += fTimeDelta;

	if (m_bOp)
	{
		m_fOpTime += fTimeDelta;
		if (m_fOpTime >= m_fOpMaxTime)
		{
			m_bOp = false;
			m_fOpTime = 0.0f;
		}
	}

	if (m_tObjInfo.eSceneID == RESOURCE_VB && (m_iHp < m_iMaxHp))
	{
		m_fRecoverTime += fTimeDelta;
		if (m_fRecoverTime >= m_fRecoverMaxTime)
		{
			m_fRecoverTime = 0.0f;
			++m_iHp;
		}
	}
}

void CCody_Space::Push_State(const _float & fTimeDelta)
{
	if (false == m_bMoveTo)
	{
		if (false == m_bRotTo)
			m_bRotTo = RotTo(fTimeDelta, 0.1f, m_vPushDIr);

		switch (m_ePickUpType)
		{
		case CPlayerValue::GENERATOR_BATTERY:
			m_vTargetPos = m_pPickUpTarget->Is<CGeneratorBattery>()->Get_Push_StartPos();
			break;
		case CPlayerValue::SPACE_WALL:
			m_vTargetPos = m_pPickUpTarget->Is<CSpaceWall>()->Get_Target1();
			break;
		}

		m_bMoveTo = MoveTo(fTimeDelta, 0.1f, m_vTargetPos);
	}
	else if (m_iKeyState & INTERACTION2)
	{
		m_bMoveTo = false;
		m_bRotTo = false;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_PUSH;
		return;
	}

	_bool bMoveAble = false;
	switch (m_ePickUpType)
	{
	case CPlayerValue::GENERATOR_BATTERY:
		if (m_iKeyState & MOVE_UP)
		{
			if (nullptr == m_pPickUpTarget)
			{
				m_bMoveTo = false;
				m_bRotTo = false;
				m_bPush = false;
				m_ePL_STATE = PL_IDLE;
				m_ePre_State = PL_PUSH;
				return;
			}

			_bool bMove = m_pPickUpTarget->Is<CGeneratorBattery>()->Get_MoveEnd();

			if (!bMove)
			{
				_vec3 vLook;

				m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
				D3DXVec3Normalize(&vLook, &vLook);
				vLook = vLook * 0.13f * fTimeDelta;
				m_pController->move(PxVec3(vLook.x, vLook.y, vLook.z), 0, fTimeDelta, m_ControllFilter);

				m_pPickUpTarget->Is<CGeneratorBattery>()->Set_Move();
			}
		}
		break;
	case CPlayerValue::SPACE_WALL:
		if (m_iKeyState & MOVE_RIGHT)
		{
			m_pPickUpTarget->Is<CSpaceWall>()->Move(fTimeDelta, CSpaceWall::SPACEWALL_CODY);
		}
		break;
	}
}

void CCody_Space::Handle_State(const _float & fTimeDelta)
{
	if (true == m_bTest)
	{
		CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());

		//m_bTest = !BoneIntoPosition("Align", _vec3(94.77f, -2.33f, -171.8f), 0.5f, fTimeDelta); 

		RotTo(fTimeDelta, 0.1f, pObj->Is<CSpaceValve>()->Get_Normal());
		m_bTest = !MoveTo(fTimeDelta, 0.1f, pObj->Is<CSpaceValve>()->Get_TargetPos());
		m_pTransformCom->Update_Component(fTimeDelta);
	}

	//if (Engine::Key_Down(DIK_F7))
	//{
	//	m_bTest = true;
	//}

	if (false == m_bHandleStart)
	{
		m_bPhys = false;
		m_eCodyAnim = Cody_Space_Valve_Idle;
		m_bHandleStart = true;
		m_bTest = true;
	}

	if (m_iKeyState & INTERACTION2)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_HANDLE;
		m_bPhys = true;
		m_bHandleStart = false;
		m_bHandleRot = false;
		m_bTest = false;
		CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());
		pObj->Is<CSpaceValve>()->Release_Valve();

		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		return;
	}

	if (m_iKeyState & (MOVE_LEFT | MOVE_RIGHT))
	{
		m_bHandleRot = true;
		if (false == m_bHandleRotStart)
		{
			_vec3 vCurDir = DirBetweenBones("Align", "RightHandMiddle1");
			D3DXVec3Normalize(&vCurDir, &vCurDir);

			m_vPreBoneDir = vCurDir;
			m_bHandleRotStart = true;
		}

		_vec3 vCurDir = DirBetweenBones("Align", "RightHandMiddle1");
		D3DXVec3Normalize(&vCurDir, &vCurDir);

		_float fCosTh = D3DXVec3Dot(&vCurDir, &m_vPreBoneDir);
		if (fCosTh >= 1.0f)
			fCosTh = 1.0f;

		_float fRad = acosf(fCosTh);

		_float fDir = D3DXVec2CCW(&_vec2(vCurDir.x, vCurDir.y), &_vec2(m_vPreBoneDir.x, m_vPreBoneDir.y));
		if (fDir >= 0.0f && (m_iKeyState & MOVE_LEFT))
		{
			CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());
			pObj->Is<CSpaceValve>()->Rotate(fRad);
			pObj->Is<CSpaceValve>()->Set_AddAngle(fRad);
		}
		else if (fDir <= 0.0f && (m_iKeyState & MOVE_RIGHT))
		{
			CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());
			pObj->Is<CSpaceValve>()->Rotate(-fRad);
			pObj->Is<CSpaceValve>()->Set_AddAngle(-fRad);
			//pObj->Is<CValveHandle>()->Set_AddAngle(-fRad);
			//pObj->Is<CValveHandle>()->Set_Move(true, -1.0f);
		}

		m_vPreBoneDir = vCurDir;
	}
	else
	{
		m_bHandleRotStart = false;
		m_bHandleRot = false;
		CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());
	}

	m_bPhys = false;
}

void CCody_Space::Free_Gravity(const _float & fTimeDelta)
{
	if (false == m_bFreeGravity)
	{
		m_ePL_STATE = PL_FALL;
		m_ePre_State = m_ePL_STATE;
		vAccDir = { 0.f,0.f,0.f };
		m_bStopGravity = false;
		return;
	}

	m_bStopGravity = true;
	if (m_iKeyState & (MOVE_UP | MOVE_RIGHT | MOVE_DOWN | MOVE_LEFT))
	{
		_vec3 vPos, vDir;
		Move(fTimeDelta, &vPos, &vDir);
		vAccDir += vDir*GRAVITYACCSPEED *fTimeDelta;

	}
	if (Engine::Key_Pressing(DIK_LCONTROL))
	{
		//_vec3 vDir = _vec3(0.0f, -1.0f, 0.0f) * fTimeDelta * 10.0f;
		//InteractionMove(vDir, fTimeDelta);
		vAccDir += _vec3{ 0.f,-1.f,0.f } *GRAVITYACCSPEED*fTimeDelta;
	}
	else if (Engine::Key_Pressing(DIK_SPACE))
	{
		//_vec3 vDir = _vec3(0.0f, 1.0f, 0.0f) * fTimeDelta * 10.0f;
		//InteractionMove(vDir, fTimeDelta);
		vAccDir += _vec3{ 0.f,1.f,0.f }  *GRAVITYACCSPEED*fTimeDelta;
	}
	m_pController->move(PxVec3(vAccDir.x, vAccDir.y, vAccDir.z), 0, fTimeDelta, m_ControllFilter);
}

void CCody_Space::Missile_Ride(const _float & fTimeDelta)
{
	if (false == m_bRideStart)
	{
		m_bStopGravity = true;
		m_bRideStart = true;
		m_eCodyAnim = Cody_Space_Jump;
		m_vSawJumpStartPos = m_pTransformCom->Get_Position();
		m_fCurJumpToTime = 0.0f;
		m_fFlyTime = 0.0F;

		_matrix matMissileWorld = m_pPickUpTarget->Is<CBaboonMissile>()->Get_WorldMatrix();
		m_pPickUpTarget->Is<CBaboonMissile>()->Set_Ride(true);
		//m_pPickUpTarget->AddRef();

		memcpy(&m_vTargetPos, (_vec3*)&matMissileWorld._41, sizeof(_vec3));
		memcpy(&m_vPushDIr, (_vec3*)&matMissileWorld._31, sizeof(_vec3));
		D3DXVec3Normalize(&m_vPushDIr, &m_vPushDIr);
		m_vTargetPos += m_vPushDIr * 0.5F;

		PlaySoundOnce(L"SB_CodyOnRocket.ogg", CSoundMgr::CHANNELID::Cody, 0.3f);

	}

	if (false == m_bMoveTo)
	{
		_vec3 vOut;
		RotTo(fTimeDelta, 1.0f, m_vPushDIr);
		m_fFlyTime += fTimeDelta;
		if (m_fFlyTime >= 1.0f)
		{
			m_bRideEnd = true;
			m_bMoveTo = true;
			m_fFlyTime = 1.0f;
			m_pPickUpTarget->Is<CBaboonMissile>()->Set_Att(true);
		}
		D3DXVec3CatmullRom(&vOut, &_vec3(m_vSawJumpStartPos.x, m_vSawJumpStartPos.y - 10.0f, m_vSawJumpStartPos.z), &m_vSawJumpStartPos, &m_vTargetPos, &_vec3(m_vTargetPos.x, m_vTargetPos.y - 10.0f, m_vTargetPos.z), m_fFlyTime);
		InteractionSetPos(vOut, fTimeDelta);
	}
}

void CCody_Space::Hit_State(const _float & fTimeDelta)
{
	if (m_bHitEnd)
	{
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			_float3 vPos, vDir;

			Move(fTimeDelta, &vPos, &vDir);

			_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta;
			m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0.01f, fTimeDelta, m_ControllFilter);

			m_ePre_State = m_ePL_STATE;
			m_ePL_STATE = PL_MOVE;
			m_eCodyAnim = Cody_Space_Walk;
			m_fCurMoveTime = 0.5f;
		}
		else
		{
			m_ePre_State = m_ePL_STATE;
			m_ePL_STATE = PL_IDLE;
			m_eCodyAnim = Cody_Space_Idle;
		}
		m_bHitEnd = false;
		m_bOp = true;
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
	}

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		Move3D(fTimeDelta, false);
}


void CCody_Space::Lever_Slap(const _float & fTimeDelta)
{
	if (false == m_bLeverStart)
	{
		if (true == m_bLeverArriveRot)
		{
			m_bLeverStart = true;

			CSpaceLever::DIR eDir = static_cast<CSpaceLever*>(m_pPickUpTarget)->Get_Dir();
			if (eDir == CSpaceLever::LEFT)
			{
				Set_LeverType(LEFT_TO_RIGHT);
				static_cast<CSpaceLever*>(m_pPickUpTarget)->Set_Dir(CSpaceLever::RIGHT);
			}
			else
			{
				Set_LeverType(RIGHT_TO_LEFT);
				static_cast<CSpaceLever*>(m_pPickUpTarget)->Set_Dir(CSpaceLever::LEFT);
			}
		}
		else
		{
			_float3 vPos, vTargetPos, vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			D3DXVec3Normalize(&vLook, &vLook);

			vPos = m_pTransformCom->Get_Position();
			vPos.y = 0.0f;
			vTargetPos = m_pPickUpTarget->Is<CSpaceLever>()->Get_Point();
			vTargetPos.y = 0.0f;

			_vec3 vGoalLook = vTargetPos - vPos;
			_vec3 vGoalDis = vGoalLook;
			D3DXVec3Normalize(&vGoalLook, &vGoalLook);

			if (false == m_bLeverRot)
			{
				if (D3DXVec3Length(&vGoalDis) <= 0.1f)
					m_bLeverRot = true;
				else
					m_bLeverRot = RotTo(fTimeDelta, 0.2f, vGoalLook);
			}

			if (true == m_bLeverRot)
			{
				if (false == m_bLeverMove)
					m_bLeverMove = MoveTo(fTimeDelta, 0.2f, vTargetPos, true, true);

				if (true == m_bLeverMove)
				{
					_vec3 vGoalDir = vTargetPos = m_pPickUpTarget->Is<CLever>()->Get_LookingAtDir();
					m_bLeverArriveRot = RotTo(fTimeDelta, 0.15f, vGoalDir, true);
				}

			}
		}
	}

	if (m_bLever)
	{
		Clear(PL_LEVER);
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_IDLE;
	}
}

void CCody_Space::Lever_Grap(const _float & fTimeDelta)
{
	if (false == m_bLeverStart)
	{
		m_bPhys = false;
		m_bLeverEnd = false;

		//CLever::DIR eDir = static_cast<CLever*>(m_pPickUpTarget)->Get_Dir();
		//if (eDir == CLever::LEFT)
		//{

		//}

		m_bPlayAnmation = false;
		m_eCodyAnim = Cody_Space_LeverRightToLeft;
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		m_pMeshCom->Play_Animationset(0.01f);

		if (false == m_bRotTo)
			m_bRotTo = RotTo(fTimeDelta, 0.1f, m_pPickUpTarget->Is<CSpaceLever>()->Get_Normal());

		switch (m_ePickUpType)
		{
		case CPlayerValue::LEVER:
			m_vTargetPos = m_pPickUpTarget->Is<CSpaceLever>()->Get_Point();
			break;
		}

		m_bMoveTo = MoveTo(fTimeDelta, 0.1f, m_vTargetPos);

		if (m_bMoveTo)
		{
			m_bLeverStart = true;
			m_bPlayAnmation = true;

			//_float fPeriod = _float(m_pMeshCom->Get_Period());
			//m_pPickUpTarget->Is<CSpaceLever>()->Set_RotTime(fPeriod - 0.3f);

		}
	}

	if (false == m_bLeverTargetStart && m_pMeshCom->Get_CurrentTime() >= 0.27)
	{
		m_bLeverTargetStart = true;
		_float fPeriod = _float(m_pMeshCom->Get_Period());
		m_pPickUpTarget->Is<CSpaceLever>()->Set_RotTime(0.38f);
	}

	if (m_iKeyState & INTERACTION2 || m_bLeverEnd)
	{
		m_bPhys = true;
		m_bPlayAnmation = true;
		m_bLeverStart = false;
		m_bRotTo = false;
		m_bMoveTo = false;
		m_bLeverEnd = false;
		m_bLeverCom = false;
		m_bLeverTargetStart = false;
		m_pPickUpTarget->On_Trigger_Active(nullptr, this);
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_LEVER;
		m_eCodyAnim = Cody_Space_Idle;

		Set_Position("Align");
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		m_pPickUpTarget->Is<CSpaceLever>()->ReSet();

		return;
	}
}

void CCody_Space::Move3D(const _float & fTimeDelta, _bool bAcc)
{
	_vec3 vPos, vDir;

	Move(fTimeDelta, &vPos, &vDir);

	if (true == bAcc)
	{
		m_fCurMoveTime += fTimeDelta;
		if (m_fCurMoveTime < m_fGoalTime)
		{
			m_fSpeed = m_fMaxSpeed * m_fCurMoveTime * (1.0f / m_fGoalTime);
		}
		else
			m_fSpeed = m_fMaxSpeed;
	}
	else
		m_fSpeed = m_fMaxSpeed;

	vDir = vDir * m_fSpeed * fTimeDelta;

	m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
}

void CCody_Space::Move2D(const _float & fTimeDelta)
{
	_vec3 vPos, vDir = { 0.0f, 0.0f, 0.0f };

	if (m_iKeyState & MOVE_LEFT)
	{
		Move(fTimeDelta, &vPos, nullptr);
		vDir = _vec3(-1.0f, 0.0f, 0.0f);
		m_fCurMoveTime += fTimeDelta;
	}
	else if (m_iKeyState & MOVE_RIGHT)
	{
		Move(fTimeDelta, &vPos, nullptr);
		vDir = _vec3(1.0f, 0.0f, 0.0f);
		m_fCurMoveTime += fTimeDelta;
	}

	if (m_fCurMoveTime < m_fGoalTime)
	{
		m_fSpeed = m_fMaxSpeed * m_fCurMoveTime * (1.0f / m_fGoalTime);
	}
	else
		m_fSpeed = m_fMaxSpeed;


	_vec3 vDstPos = vDir * m_fSpeed * fTimeDelta;
	m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);

}

void CCody_Space::Key_Reset()
{
	m_iKeyState = NULL;
	//m_bClimbing = false;
}

void CCody_Space::Set_Damage(_int iDamage)
{
	if (m_ePL_STATE == PL_HIT || m_bOp || m_ePL_STATE == PL_MISSILE)
		return;

	m_iHp += iDamage;
	Set_Hit();

	m_ePre_State = m_ePL_STATE;
	m_ePL_STATE = PL_HIT;
	Clear(PL_DEAD);
}

void CCody_Space::FadeIn(const _float & fTimeDelta)
{
	if (!m_bFadeIn)
		return;

	m_pFade->Change_Color_To(1.f, 0.f, 0.f, 0.f, 0.f);
	m_bFadeIn = false;
}

void CCody_Space::FadeOut(const _float & fTimeDelta)
{
	if (!m_bFadeOut)
		return;

	m_pFade->Change_Color_To(1.f, 0.f, 0.f, 0.f, 255.f);
	m_bFadeOut = false;
}

