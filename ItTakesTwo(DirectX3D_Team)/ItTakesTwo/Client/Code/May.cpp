#include "stdafx.h"
#include "May.h"
#include "Export_Function.h"
#include "MayCamera.h"
#include "FuseNPC.h"
#include "MiniVacuum.h"
#include "MainCamera.h"
#include "Cody.h"
#include "ImageUI.h"
#include "Lever.h"
#include "Fan.h"
#include "PowerButton.h"
#include "Dumbbell.h"
#include "HangLever.h"
#include "CircleSaw.h"
#include "ValveHandle.h"
#include "FanButton.h"
#include "PowerSwitch.h"
#include "Vacuum.h"

void CMay::Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody)
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
	m_pController->setSlopeLimit(D3DXToRadian(70.f));
	m_pController->setNonWalkableMode(PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING);
}

physx::PxControllerBehaviorFlags CMay::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	PxControllerBehaviorFlags flags;
	if (shape.getSimulationFilterData().word0 & FilterGroup::eRideObj)
	{
		flags = PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	if (shape.getSimulationFilterData().word0 & FilterGroup::eBigFan)
	{
		CGameObject* pGameObjectt = (CGameObject*)actor.getName();
		m_pAttachTarget = pGameObjectt;
		m_bRideObj = true;
	}
	else if (shape.getSimulationFilterData().word0 & FilterGroup::eStone &&
		PL_CLIMBING == m_ePL_STATE)
	{
		CGameObject* pGameObjectt = (CGameObject*)actor.getName();
		if (false == m_bAttach)
		{
			_matrix matWorld;
			_vec3 vPos;
			pGameObjectt->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_WorldMatrix(&matWorld);
			vPos = _vec3(matWorld._41, matWorld._42, matWorld._43);
			m_vAttachDir = m_pTransformCom->Get_Position() -
				vPos;
		}
		m_pAttachTarget = pGameObjectt;
		m_bAttach = true;
	}
	else if (shape.getSimulationFilterData().word0 & FilterGroup::eMonster)
	{
		Set_PlayerDead();
	}

	return flags;
}

physx::PxControllerBehaviorFlags CMay::getBehaviorFlags(const PxController& controller)
{
	OutputDebugString(L"컨트롤러랑 충돌\n");

	PxControllerBehaviorFlags flags;
	return flags;
}

physx::PxControllerBehaviorFlags CMay::getBehaviorFlags(const PxObstacle& obstacle)
{
	OutputDebugString(L"옵스타클이랑 충돌\n");
	PxControllerBehaviorFlags flags;

	return flags;
}

void CMay::onShapeHit(const PxControllerShapeHit& hit)
{
	/*if (hit.shape->getSimulationFilterData().word0 != FilterGroup::eGround)
	OutputDebugString(L"땅이 아닌거랑 충돌\n");*/
	//if (hit.shape->getSimulationFilterData().word0 == FilterGroup::eGround)
	//{
	//	m_bGround = true;
	//	OutputDebugString(L"땅이 아닌거랑 충돌\n");
	//}
	//else
	//	m_bGround = false;

	//OutputDebugString(L"땅이 아닌거랑 충돌\n");
	//if (hit.shape->getSimulationFilterData().word0 & FilterGroup::eFan)
	//{
	//	m_pController->setFootPosition(hit.worldPos);
	//}
	if (m_bJump[PL_GROUNDPOUND] && hit.shape->getSimulationFilterData().word0 & FilterGroup::eButton)
	{
		ToObj<CGameObject>(hit.actor->getName())->On_Active(this);
	}
	if (hit.shape->getSimulationFilterData().word0 & FilterGroup::eGround &&hit.worldNormal.y > 0.99)
	{

		m_pController->setFootPosition(hit.worldPos);
	}
	if ((hit.shape->getSimulationFilterData().word0 & FilterGroup::eGround
		|| hit.shape->getSimulationFilterData().word0 & FilterGroup::eWall
		|| hit.shape->getSimulationFilterData().word0 & FilterGroup::eStone)
		&& (hit.worldNormal.y < 0.09))
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
				vPos.y -= 0.15f;
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
				if (m_pScene->raycast(PxVec3((_float)vPos.x, (_float)vPos.y, (_float)vPos.z),
					-PxVec3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z),
					2.0F,
					buf))
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
	}
	return;
}

void CMay::onControllerHit(const PxControllersHit& hit)
{
	OutputDebugString(L"다른 컨트롤러랑 충돌\n");
	return;
}

void CMay::onObstacleHit(const PxControllerObstacleHit& hit)
{
	OutputDebugString(L"옵스타클이랑 충돌\n");
	return;
}

bool CMay::filter(const PxController& a, const PxController& b)
{
	PxShape* pShape1;
	PxShape* pShape2;
	pShape1 = nullptr;
	pShape2 = nullptr;
	a.getActor()->getShapes(&pShape1, 1);
	b.getActor()->getShapes(&pShape2, 1);
	if ((pShape1->getSimulationFilterData().word0 | pShape2->getSimulationFilterData().word0) & (FilterGroup::eInteractItem | FilterGroup::eMay))
		return false;
	return true;
}

physx::PxQueryHitType::Enum CMay::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{
	return PxQueryHitType::eBLOCK;
}

physx::PxQueryHitType::Enum CMay::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	return PxQueryHitType::eBLOCK;
}



void CMay::Insert_Fuse_End()
{
	ToObj<CFuseNPC>(m_pPickUpTarget)->Set_TriggerOff();
	m_pPickUpTarget->Set_Dead();
	if (!m_pInteractActor)
		return;
	ToTrigger(m_pInteractActor)->Set_Interactable(false);
	ToTrigger(m_pInteractActor)->Activate(this);
	//m_bInsert = true;
}

Engine::_bool CMay::Check_Insert()
{
	m_bInsert = false;
	return m_bInsert;
}

CMay::CMay(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayerValue(pGraphicDev)
{
}

CMay::~CMay()
{
}

void CMay::Check_Wall(const _float & fTimeDelta)
{
	const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

												//CCody* pPlayer = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>();
	PxExtendedVec3 vPos = m_pController->getPosition();
	_vec3 vLook, vDir;
	PxQueryFilterData data{};

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (m_bAttach)
	{
		vDir = vLook;
	}
	else
	{
		vDir = m_vClimbingDir;
	}

	if (m_pScene->raycast(PxVec3((_float)vPos.x, (_float)vPos.y, (_float)vPos.z),
		PxVec3(vDir.x, vDir.y, vDir.z),
		3.0f,
		buf, PxHitFlag::eDEFAULT, data))
	{
		for (PxU32 i = 0; i < buf.nbTouches; ++i)
		{
			if (buf.getTouch(i).shape->getSimulationFilterData().word0 == FilterGroup::eMay)
			{
				continue;
			}
			else
			{
				return;
			}
		}

		if (m_eMayAnim == May_Wall_Idle)
			m_bClimbingFall = true;
	}
}

HRESULT CMay::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));

	ZeroMemory(&m_bJump, sizeof(m_bJump));
	ZeroMemory(&m_bDash, sizeof(m_bDash));
	ZeroMemory(&m_bPlaySound, sizeof(m_bPlaySound));

	m_eMayAnim = May_Idle;
	m_ePreMayAnim = May_Idle;
	m_fAnimRatio = 1.0f;
	m_iMaxHp = 12;
	m_iHp = m_iMaxHp;




	return S_OK;
}

Engine::_int CMay::Update_Object(const _float& fTimeDelta)
{
	FadeIn(fTimeDelta);
	FadeOut(fTimeDelta);
	if (!m_bKeyOff)
	{

		m_fTimeDelta = fTimeDelta;
		m_bEveentKey = false;
		Engine::CGameObject::Update_Object(fTimeDelta);

		Key_Reset();
		Key_Input(fTimeDelta);
		Gravity(fTimeDelta);
		Event(fTimeDelta);
		//if (Engine::Key_Down(DIK_I))
		//{
		//	m_ePL_STATE = PL_DEAD;
		//}
		Set_State(fTimeDelta);
		Update_Time(fTimeDelta);
		if (!m_bPlayerDead)
			PlayAnimation(this, m_pMeshCom, fTimeDelta, m_ePL_STATE, &m_ePre_State);

		Effect(fTimeDelta);
		Update_UI(fTimeDelta);
	}

	CMayCamera* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
	if (pCamera)
	{
		pCamera->Set_TargetUp(ToVec3(m_pController->getUpDirection()));
	}


	if (!m_bDeadUI && !m_bRenderOff)
	{
		//Engine::Add_RenderGroup(Engine::RENDER_VTF_CODY, this);
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		if (m_bStencil)
		{
			Engine::Add_RenderGroup(Engine::RENDER_SHADOW, this);
			Engine::Add_RenderGroup(Engine::RENDER_STENCIL, this);
		}
	}

	return OBJ_NOEVENT;
}

_int CMay::LateUpdate_Object(const _float & fTimeDelta)
{
	if (m_bKeyOff)
		return 0;

	Set_Late_State(fTimeDelta);

	_vec3 vOrgPos = m_pTransformCom->Get_Position();
	m_bRideObj = false;
	m_bShootState = false;
	_vec3 vPos;
	PxTransform Trnasform;
	PxExtendedVec3 pxPos = m_pController->getFootPosition();
	vPos = _vec3((_float)pxPos.x, (_float)pxPos.y, (_float)pxPos.z);

	if (m_ePL_STATE != PL_DEAD)
		m_vDeadSlide = vPos - vOrgPos;

	if (m_bPhys)
		m_pTransformCom->Set_Pos(vPos);

	if (m_bStart)
	{
		if (m_tObjInfo.eSceneID == RESOURCE_VF2)
			StopNPlaySound(L"VF2_MayStart.ogg", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		Create_UI(L"May_UI");
		m_bStart = false;
	}


	//Sign_UI(0.f);

	return _int();
}

void CMay::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	//m_pMeshCom->Render_Meshes(pEffect, 0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	//pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CMay::Render_Shadow(_vec3 * vEye, _vec3 * vAt, const _int & iIndex)
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

	CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CTransform* pCodyTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

	*vAt = (m_pTransformCom->Get_Position() + pCodyTrans->Get_Position()) * 0.5f;
	//*vAt = _vec3(0.f, 0.f, 0.f);
	*vEye = *vAt + SHADOWLIGHT;

	_matrix		LightViewMatrix, LightProjMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, &*vEye, &*vAt, &vUP);
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

void CMay::Render_Stencil(const _int & iIndex)
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

void CMay::Render_VTF(const _int& iIndex /*= 0*/)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	pEffect->BeginPass(0);
	pEffect->CommitChanges();

	m_pMeshCom->Render_VTF(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CMay::Effect(const _float & fTimeDelta)
{
	Effect_Walk(fTimeDelta);
	Effect_Dead(fTimeDelta);
	Effect_Respawn(fTimeDelta);
	Effect_Dust(fTimeDelta);
}

void CMay::Effect_Walk(const _float & fTimeDelta)
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

void CMay::Effect_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadEffect)
		return;

	m_fAmount += fTimeDelta;
	if (m_fAmount >= 1.f)
	{

		if (m_tObjInfo.eSceneID != RESOURCE_VB)
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

void CMay::Effect_Respawn(const _float & fTimedelta)
{
	if (!m_bRespawnEffect)
		return;

	m_fAmount -= fTimedelta;
	if (m_fAmount <= 0.f)
	{

		m_bRespawnEffect = false;
		m_bStencil = true;
		m_fAmount = 0.f;
		Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>()->Set_RayCheck(true);
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

void CMay::Effect_Hit(const _float & fTimeDelta)
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

void CMay::Effect_Dust(const _float & fTimeDelta)
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	_float fRand = 0.f;
	if (CRandoms()(0, 1) == 1)
		fRand = 0.f;
	else
		fRand = 0.5f;
	Engine::Set_AnimState(tParticle, 2, 2, fRand);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fRotateSpeed = 0.f;

	tParticle.fLifeTime = 5.f;

	if (CRandoms()(0, 1) == 1)
		tParticle.fAmount = 0.f;
	else
		tParticle.fAmount = 0.5f;

	_float fSize = CRandoms()(1.f, 2.f);
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-30.f, 30.f), CRandoms()(-30.f, 30.f), CRandoms()(-30.f, 30.f));
	tParticle.vStartColor = { 255.f * 2.5f, 255.f * 2.5f, 255.f * 2.5f, 255.f * 1.5f };
	tParticle.vEndColor = { 255.f * 2.5f, 255.f * 2.5f, 255.f * 2.5f, 55.f * 1.5f };
	_float fStart = CRandoms()(0.3f, 0.5f);
	tParticle.vStartSize = _vec3(fStart, fStart, fStart);
	tParticle.vEndSize = _vec3(fStart, fStart, fStart);
	tParticle.fVelocity = CRandoms()(0.5f, 1.f);
	tParticle.vDir = { CRandoms()(-1.f, 1.f),CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f) };
	Engine::Emit_ImageParticle(tParticle, 51, 32);
}

CMay* CMay::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CMay*	pInstance = new CMay(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMay::Free()
{
	Safe_Release(m_pImageUI);
	Safe_Release(m_pFade);
	Safe_Release(m_pHp);
	Safe_Release(m_pProfile);
	for (_int i = 0; i < 6; ++i)
		Safe_Release(m_pDeadUI[i]);
	Safe_Release(m_pCodySign);
	Safe_Release(m_pFuseSign);
	Engine::CGameObject::Free();
}

HRESULT CMay::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_ObjTag = L"May";

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"May"));
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	m_pTransformCom->Set_Angle(tTemp.vAngle);

	m_tObjInfo.eSceneID = tTemp.eSceneID;

	// 툴박스앞
	m_fShapeHeight = 1.32f;
	m_fShapeRadius = 0.3f;
	m_fHalfHeight = m_fShapeRadius + (m_fShapeHeight * 0.5f);

	m_vSavePoint = { tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z };
	Create_Actor(Engine::Get_Physics(), m_fShapeHeight, m_fShapeRadius, this, this, { tTemp.vPos.x, tTemp.vPos.y + m_fHalfHeight, tTemp.vPos.z });

	//m_vSavePoint = { tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z };

	//tTemp.eSceneID = RESOURCE_STATIC;
	//m_pImageUI = CImageUI::Create(m_pGraphicDev, &tTemp);
	//m_pImageUI->Set_Image(L"Black_Bg");

	// Fan앞
	//Create_Actor(Engine::Get_Physics(), 0.3f, 0.635f, this, this, { 90.f,-0.21f,-31.f });
	//m_vSavePoint = { 91.f,-0.21f,-31.f };

	return S_OK;
}

void CMay::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Key_Down(DIC_MENU))
	{
		m_bPlayerDead = true;
		//m_bSceneWait = true;
	}

	_float fDIC_Y = 0;
	_float fCheck = 0.0f;
	fDIC_Y = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_Y);
	if (fabsf(fDIC_Y) <= 0.2f) {}
	else
	{
		if (fDIC_Y > 0)
			m_iKeyState |= MOVE_UP;
		else if (fDIC_Y < 0)
			m_iKeyState |= MOVE_DOWN;
	}

	_float fDIC_X = 0;
	fDIC_X = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_X);
	if (fabs(fDIC_X) <= 0.2f) {}
	else
	{
		if (fDIC_X > 0)
			m_iKeyState |= MOVE_RIGHT;
		else if (fDIC_X < 0)
			m_iKeyState |= MOVE_LEFT;
	}

	// 패드로 누른 방향
	m_vMoveDir = _float3(fDIC_X, 0.0f, fDIC_Y);
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	// 이동 방향을 구하기위한 각도 구하기
	{
		_float fCosTh = D3DXVec3Dot(&m_vMoveDir, &_float3(0.0f, 0.0f, 1.0f));
		_float fRad = acosf(fCosTh);
		if (fDIC_X <= 0)
			fRad = 3.14f * 2 - fRad;

		m_fDirAngle = D3DXToDegree(fRad);
	}

	if (Engine::Key_Down(DIC_A))
	{
		if (true == m_bJump[PL_1STJUMP])
			m_bJump[PL_2NDJUMP] = true;

		m_iKeyState |= JUMP;
	}

	// X : 대쉬, 꾹누르기 : 대쉬, 앉기 : B
	if (Engine::Key_Pressing(DIC_B))
	{
		if (m_ePL_STATE == PL_JUMP)
			m_bJump[PL_GROUNDPOUND_START] = true;

		m_iKeyState |= (SIT | INTERACTION2);
	}

	if (Engine::Key_Down(DIC_LS))
		m_iKeyState |= RUN;

	if (Engine::Key_Down(DIC_X))
	{
		if (m_ePL_STATE == PL_JUMP)
			m_bJump[PL_JUMPDASH] = true;

		m_iKeyState |= DASH;
	}

	if (Engine::Key_Down(DIC_Y))
	{
		m_iKeyState |= INTERACTION;
		m_bEveentKey = true;
	}
}

HRESULT CMay::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	//SetUp_OnRegister(0, L"Target_VTF_Pos_May");
	//SetUp_OnRegister(1, L"Target_VTF_Normal_May");
	//SetUp_OnRegister(2, L"Target_VTF_Tangent_May");
	//SetUp_OnRegister(3, L"Target_VTF_Binormal_May");
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
		pEffect->SetVector("g_vColor", &_vec4(75.f * 3.5f, 155.f * 3.5f, 255.f * 3.5f, 255.f));
	else
		pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	Engine::SetUp_OnShader(pEffect, L"Target_Depth_May", "g_DepthTexture");


	m_pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 0);
	m_pTextureCom->Set_Texture(pEffect, "g_BurnTexture", 1);
	pEffect->SetFloat("g_fMaterialIndex", 0.f);

	pEffect->CommitChanges();

	return S_OK;
}

void CMay::Set_State(const _float & fTimeDelta)
{
	switch (m_ePL_STATE)
	{
	case CMay::PL_IDLE:
		Idle_State(fTimeDelta);
		break;
	case CMay::PL_MOVE:
		Move_State(fTimeDelta);
		break;
	case CMay::PL_JUMP:
		Jump_State(fTimeDelta);
		break;
	case CMay::PL_SIT:
		Sit_State(fTimeDelta);
		break;
	case CMay::PL_RUN:
		Run_State(fTimeDelta);
		break;
	case CMay::PL_DASH:
		Dash_State(fTimeDelta);
		break;
	case CMay::PL_CLIMBING:
		Climbing_State(fTimeDelta);
		break;
	case CMay::PL_HANG:
		Hang_State(fTimeDelta);
		break;
	case CMay::PL_SLIDE:
		Slide_State(fTimeDelta);
		break;
	case CMay::PL_RIDE:
		Ride_State(fTimeDelta);
		break;
	case CMay::PL_PICKUP:
		PickUp_State(fTimeDelta);
		break;
	case CMay::PL_DEAD:
		Dead_State(fTimeDelta);
		break;
	case CMay::PL_LEVER:
		Lever_State(fTimeDelta);
		break;
	case CMay::PL_HANGSWITCH:
		HangSwitch_State(fTimeDelta);
		break;
	case CMay::PL_FLOAT:
		Float_state(fTimeDelta);
		break;
	case CMay::PL_FALL:
		Fall_State(fTimeDelta);
		break;
	case CMay::PL_PUSH:
		Push_State(fTimeDelta);
		break;
	case CMay::PL_HANDLE:
		Handle_State(fTimeDelta);
		break;
	case CCody::PL_BUTTON:
		Button_State(fTimeDelta);
		break;
	case CMay::PL_SAW_SHOOT:
		Event_Saw_Shoot(fTimeDelta);
		break;
	case CMay::PL_SAW_SLIDING:
		Event_Saw_Sliding(fTimeDelta);
		break;
	case CMay::PL_SHOOT:
		Shoot_State(fTimeDelta);
		break;
	case CMay::PL_HIT:
		Hit_State(fTimeDelta);
		break;
	}
}

void CMay::Move(const _float & fTimeDelta, _float3 * vResultPos, _float3 * vResultDir, _float fRotSpeed)
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

	*vResultPos = vPos;
	*vResultDir = vDir;
}

void CMay::Dash(const _float & fTimeDelta, _float fTime, _float fDashRatio)
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

void CMay::Dash(const _float & fTimeDelta, _float fDashRatio)
{
	_float3 vLook;

	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vLook.y = 0.0f;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vDstPos = vLook * m_fMaxSpeed * fTimeDelta * fDashRatio;

	m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
}

_bool CMay::Gravity(const _float & fTimeDelta)
{
	if (!m_bSimulate)
		return false;

	m_fGravity = -2.0f;
	m_fJumpPower = 1.3f;

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

void CMay::Event(const _float & fTimeDelta)
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
		case CMay::FUSE:
			if (nullptr != m_pPickUpTarget)
				static_cast<CFuseNPC*>(m_pPickUpTarget)->Set_Carried(L"May");
			break;
		}
	}
	else if ((m_ePL_STATE == PL_MOVE || m_ePL_STATE == PL_IDLE || m_ePL_STATE == PL_JUMP) && m_bRide && (m_iKeyState & INTERACTION))
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_RIDE;
		ZeroMemory(&m_bJump, sizeof(m_bJump));
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

}

void CMay::Set_Late_State(const _float & fTimeDelta)
{
	switch (m_ePL_STATE)
	{
	case CPlayerValue::PL_IDLE:
	case CPlayerValue::PL_MOVE:
	case CPlayerValue::PL_SIT:
	case CPlayerValue::PL_RUN:
		RideObjet(fTimeDelta);
		break;
	case CPlayerValue::PL_PUSH:
		Late_Push_State(fTimeDelta);
		break;
	case CPlayerValue::PL_CLIMBING:
		Late_Climbing_State(fTimeDelta);
		break;
	case CPlayerValue::PL_SAW_SHOOT:
		Late_Event_Saw_Shoot(fTimeDelta);
		break;
	}
}

void CMay::Late_Push_State(const _float & fTimeDelta)
{
	switch (m_ePickUpType)
	{
	case CPlayerValue::DUMBBELL:
		if (m_pPickUpTarget->Is<CDumbbell>()->Get_Move())
		{
			InteractionMove(m_pPickUpTarget->Is<CDumbbell>()->Get_Dir(), fTimeDelta);
			if (m_bDumbbell)
			{
				Engine::StopNPlaySound(L"VF3_Dumbell_May.ogg", CSoundMgr::CHANNELID::Crusher, 0.3f);
				m_bDumbbell = false;
			}
		}
		break;
	}
}

_bool CMay::JumpTo(_float fJumpPower, _float fTime, _float3 vGoalPos, _float fTimeDelta)
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

	PxExtendedVec3 pxPos;
	pxPos = m_pController->getFootPosition();

	pxPos.x = pxPos.x + vGolDir.x;
	pxPos.y = fHeight;
	pxPos.z = pxPos.z + vGolDir.z;

	m_pTransformCom->Set_Pos(vPos + vGolDir);

	return false;
}

void CMay::Shoot(const _float& fTimeDelta)
{
	if (!m_bShoot)
		return;

	//if (m_fShootTime > m_fShootDuration)
	//{
	//	//m_bRideGround = true;
	//	m_bShoot = false;
	//	m_bStopGravity = false;

	//}
	m_bStopGravity = true;
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &m_vShootDir);
	vDir = vDir*fTimeDelta*m_fShootPower;



	PxControllerCollisionFlags flag;

	flag |= m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
	flag |= m_pController->move(PxVec3(0.f, -0.6f, 0.f)*m_fShootTime*m_fShootTime, 0, fTimeDelta, m_ControllFilter);
	if (flag)
	{
		if (m_fShootTime >= 0.5f && flag & PxControllerCollisionFlag::eCOLLISION_DOWN)
			m_bRideGround = true;

		m_bShoot = false;
		m_bStopGravity = false;
	}

	m_fShootTime += fTimeDelta;

	if (m_iKeyState & (MOVE_DOWN | MOVE_RIGHT | MOVE_UP | MOVE_LEFT))
	{
		_vec3 vMovePos, vMoveDir;
		Move(fTimeDelta, &vMovePos, &vMoveDir);

		_vec3 vDstPos = vMoveDir * m_fMaxSpeed * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
	}
}

_bool CMay::MoveTo(const _float& fTimeDelta, _float fGoalTime, _vec3 vGoalPos, _bool bExceptY, _bool bSimilar, _float fDis)
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
		m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
	}

	return false;
}

_bool CMay::RotTo(const _float & fTimeDelta, _float fGoalTime/*, _vec3 vTargetLook*/, _vec3 vGoalLook, _bool bSimilar/*, _float fRad*/)
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
			m_bRotToStart = false;
			return true;
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

_bool CMay::RotTo(_vec3 vGoalLook)
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

void CMay::Set_Pos(char * tagBoneName, const _float & fTimeDelta)
{
	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName(tagBoneName);
	_float4x4 matBoneMatrix = pBone->CombinedTranformationMatrix;
	_float4x4 matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matBoneMatrix *= matWorld;

	m_pController->setFootPosition(PxExtendedVec3(matBoneMatrix._41, matBoneMatrix._42, matBoneMatrix._43));
	m_pTransformCom->Set_Pos(_vec3(matBoneMatrix._41, matBoneMatrix._42, matBoneMatrix._43));
	m_pTransformCom->Update_Component(fTimeDelta);
}

_bool CMay::Ground(const _float& fTimeDelta)
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

_bool CMay::Lock()
{
	CMayCamera* pCamera = Engine::Get_GameObject(L"GameObject", L"CodyCamera")->Is<CMayCamera>();
	NULL_CHECK_RETURN(pCamera, false);

	return pCamera->Get_CameraFix();
}

void CMay::InteractionMove(_vec3 vDir, const _float & fTimeDelta)
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

void CMay::InteractionSetPos(_vec3 vPos, const _float & fTimeDelta)
{
	if (m_bPhys)
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));

	m_pTransformCom->Set_Pos(vPos);

	m_pTransformCom->Update_Component(fTimeDelta);
}

_vec3 CMay::DirBetweenBones(char * tagBoneName1, char * tagBoneName2)
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

void CMay::Idle_State(const _float & fTimeDelta)
{
	if (Ground(fTimeDelta))
	{
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_IDLE;
	}
	else if (true == m_bFly && m_iKeyState & INTERACTION)
	{
		m_ePL_STATE = PL_SAW_SHOOT;
		m_ePre_State = PL_IDLE;
	}
	else if (true == m_bShootState)
	{
		m_ePL_STATE = PL_SHOOT;
		m_ePre_State = PL_IDLE;
	}
	else if (true == m_bFanButton && m_iKeyState & INTERACTION)
	{
		m_eMayAnim = May_Push_Button;
		m_ePL_STATE = PL_BUTTON;
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

void CMay::Move_State(const _float & fTimeDelta)
{
	if (Ground(fTimeDelta))
	{
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_IDLE;
		Clear(PL_MOVE);
	}
	else if (true == m_bShootState)
	{
		m_ePL_STATE = PL_SHOOT;
		m_ePre_State = PL_IDLE;
		Clear(PL_MOVE);
	}
	else if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_MOVE;
		Clear(PL_MOVE);
	}
	else if (m_iKeyState & RUN)
	{
		m_ePL_STATE = PL_RUN;
		m_ePre_State = PL_MOVE;
		Clear(PL_MOVE);
	}
	else if (m_iKeyState & SIT)
	{
		m_ePL_STATE = PL_SIT;
		m_ePre_State = PL_MOVE;
		Clear(PL_MOVE);
	}
	else if (m_iKeyState & JUMP)
	{
		m_ePL_STATE = PL_JUMP;
		m_ePre_State = PL_MOVE;
		Clear(PL_MOVE);
	}
	else if (!(m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT)))
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_MOVE;
		Clear(PL_MOVE);
	}
	else
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

		_vec3 vDstPos = vDir * m_fSpeed * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
	}
}

void CMay::Jump_State(const _float & fTimeDelta)
{
	// 점프 내려찍기
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		if (false == m_bPlaySound[PLAY_WHOOSH])
		{
			m_bPlaySound[PLAY_WHOOSH] = true;
			CSoundMgr::GetInstance()->StopNPlaySound(L"May_Whoosh.wav", CSoundMgr::CHANNELID::May_Jump, m_fEffectvolume);
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

	if (m_bFloat && m_fOrgHeight >= m_pTransformCom->Get_Position().y && m_bJump[PL_JUMPSTART])
	{
		Clear(PL_JUMP);
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FLOAT;
		Set_ClearJump();

		return;
	}

	// 점프 시작
	if (!m_bJump[PL_1STJUMP] == true && !m_bJump[PL_JUMPSTART])
	{
		m_eMayAnim = May_Jump;
		m_fJumpTime = 0.0f;
		m_bJump[PL_1STJUMP] = true;
		m_bJump[PL_JUMPSTART] = true;

		if (false == m_bPlaySound[PLAY_JUMP])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"May_Jump.wav", CSoundMgr::CHANNELID::May_Jump, m_fEffectvolume);
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
		return;
	}

	// 점프 대쉬
	// 점프 대쉬가 끝나고 내려올떄 부자연스러움
	if (m_bJump[PL_JUMPDASH] && !m_bJump[PL_JUMPDASHSTOP])
	{
		// 점프대쉬중에 중력을 안받기 위해 true로
		//m_fHeight = -1.0f;
		m_bStopGravity = true;
		m_fJumpTime = 0.75f;
		m_bJumpDash = true;
		Dash(fTimeDelta, 0.05f, 1.0f);
		//Dash(fTimeDelta, 2.3f);

		if (false == m_bPlaySound[PLAY_JUMP_DASH])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"May_Jump_Dash.wav", CSoundMgr::CHANNELID::May_Jump, m_fEffectvolume);
			m_bPlaySound[PLAY_JUMP_DASH] = true;
		}

		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			_float3 vPos, vDir;
			Move(fTimeDelta, &vPos, &vDir);

			_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta * 0.5f;
			m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
		}

		Set_JumpDash();
	}

	// 이단 점프
	if (!m_bJump[PL_2NDJMPSTART] && m_bJump[PL_2NDJUMP] && m_ePre_State != PL_FLOAT)
	{
		m_bJump[PL_2NDJMPSTART] = true;
		m_fJumpTime = 0.0f;
		Set_2ndJump(true);

		if (false == m_bPlaySound[PLAY_DOUBLE_JUMP])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"May_Double_Jump.wav", CSoundMgr::CHANNELID::May_Jump, m_fEffectvolume);
			m_bPlaySound[PLAY_DOUBLE_JUMP] = true;
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

		// 점프값
		m_fJumpTime += fTimeDelta;
		m_fHeight = m_fGravity * m_fJumpTime * m_fJumpTime + m_fJumpPower * m_fJumpTime;
	}
}

void CMay::Sit_State(const _float & fTimeDelta)
{
	if (!(m_iKeyState & SIT))
	{
		if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
		{
			m_ePL_STATE = PL_MOVE;
			m_ePre_State = PL_SIT;
			m_pController->resize(m_fShapeHeight);
		}
		else
		{
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_SIT;
			m_pController->resize(m_fShapeHeight);
		}
	}

	m_fHalfHeight = m_fShapeRadius + (m_fShapeHeight * 0.3f * 0.5f);
	m_pController->resize(m_fShapeHeight * 0.3f);
	_float3 vPos, vDir;
	vPos = m_pTransformCom->Get_Position();
	vDir = _float3(0.0f, 0.0f, 0.0f);



	// 앉았을때 이동유무
	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		Set_CrouchMove(true);
		Move(fTimeDelta, &vPos, &vDir);

		_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta * 0.3f;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
	}
	else
		Set_CrouchMove(false);

	//auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	//_vec3 vDstPos = vDir * m_fSpeed * 0.3f;
}

void CMay::Run_State(const _float & fTimeDelta)
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
		PxTransform pxTrnasform = m_pController->getActor()->getGlobalPose();
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
		//m_pTransformCom->Set_Pos(vDstPos);
	}
}

void CMay::Dash_State(const _float & fTimeDelta)
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

void CMay::Hang_State(const _float & fTimeDelta)
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
		m_pMeshCom->Set_Animationset(Cody_Idle, 1.0f, 0.01f);

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

		const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("FaceManager");
		_float4x4 matBone = pBone->CombinedTranformationMatrix;
		_float4x4 matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		_float3 vPos = m_pTransformCom->Get_Position();
		matBone *= matWorld;
		m_pController->setFootPosition(PxExtendedVec3(matBone._41, matBone._42, matBone._43));
		m_pTransformCom->Set_Pos(_vec3(matBone._41, matBone._42, matBone._43));
		m_pTransformCom->Update_Component(m_fTimeDelta);
		m_pMeshCom->Set_Animationset(May_Fall, 1.0f, 0.01f);
		return;
	}

	m_bStopGravity = true;
	m_fGravityTime = 0.0f;
}

void CMay::Climbing_State(const _float & fTimeDelta)
{
	if (m_iKeyState & INTERACTION2)
	{
		m_bClimbingFall = false;
		m_bAttach = false;
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_CLIMBING;
		m_bStopGravity = false;
		m_bClimbingJump = false;
		m_bClimbingStart = false;
		return;
	}

	m_fGravityTime = 0.0f;
	if (false == m_bClimbingStart)
	{
		m_eMayAnim = May_Wall_Idle;
		m_bClimbingStart = true;
		RotTo(m_vClimbingDir);
	}

	if (m_bClimbingFall)
	{
		m_bClimbingFall = false;
		m_bAttach = false;
		m_ePL_STATE = PL_FALL;
		m_ePre_State = PL_CLIMBING;
		m_bStopGravity = false;
		m_bClimbingJump = false;
		m_bClimbingStart = false;
		return;
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
		m_bClimbingFall = false;
		RotTo(m_vClimbingDir);
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

			_vec3 vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
			vLook.y = 0.0f;
			D3DXVec3Normalize(&vLook, &vLook);
			m_vClimbingDir = _vec3(vLook.x, vLook.y, vLook.z);
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
			RotTo(m_vClimbingDir);

			m_bClimbingJump = false;
			m_bClimbing = false;
			m_fJumpTime = 0.0f;

			// 벽에 닿았을때 애니메이션을 Idle상태로 바꿔준다.
			Set_ClimbingJumpEnd();

			return;
		}
	}


	//m_fHeight += -3.0f * fTimeDelta * fTimeDelta;
}

void CMay::Slide_State(const _float& fTimeDelta)
{
	if (true == m_bSlideEnd)
	{
		if (m_bRideGround)
		{
			m_ePre_State = m_ePL_STATE;
			m_ePL_STATE = PL_IDLE;
			m_eMayAnim = May_Idle;
			m_bRideGround = false;
			m_bSlideEnd = false;
			return;
		}
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

			StopSound(Engine::CSoundMgr::CHANNELID::Vacuum_Move);
			m_vecSlidePos.clear();
			m_bSlideEnd = true;
			return;
		}
		else if (m_iSlideIndex + (m_iInterval * 4) >= m_vecSlidePos[m_iPathCount]->size())
		{
			StopSound(Engine::CSoundMgr::CHANNELID::Vacuum_PreOut);
			Engine::PlaySoundW(L"Play_Gameplay_Vehicles_VacuumHose_BeforeExit_Whoosh.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_PreOut);

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

	Engine::PlaySoundW(L"Play_Gameplay_Vehicles_VacuumHose_ObjectInsideTube.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_Move);


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

void CMay::Ride_State(const _float& fTimeDelta)
{
	// 탈출조건
	if (true == m_bRideJumpMax)
	{
		// 껏던 피직스를 다시 켜준다.
		Set_SimulationState(true);
		// 내려오는 애니메이션 마지막 부분의 위치로 플레이어를 이동
		Set_Pos("Align", m_fTimeDelta);
		m_pMeshCom->Set_Animationset(May_Jump_Fall, 1.0f, 0.01f);
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
		if (m_bBossRide)
			return;

		Set_TakeOff();
		m_bForceRideOff = false;

		return;
	}
	Engine::PlaySoundW(L"Play_Gameplay_Vehicles_VacuumHose_Controllable.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_Controll);

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

		if (typeid(CVacuum) == typeid(*m_pPickUpTarget))
		{
			vHeadLook = { 0.0f, 0.0f, 1.0f };
		}

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
	else if (m_tObjInfo.eSceneID == RESOURCE_VB &&
		!m_bBossRide && typeid(CVacuum) == typeid(*m_pPickUpTarget))
	{
		m_pPickUpTarget->Is<CVacuum>()->May_ride();
		m_bBossRide = true;
	}
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

void CMay::Dead_State(const _float& fTimeDelta)
{
	m_bPhys = false;

	if (false == m_bDeadUI)
	{
		D3DXVec3Normalize(&m_vDeadSlide, &m_vDeadSlide);
		m_pTransformCom->Move_Pos(&m_vDeadSlide, 3.5f, fTimeDelta);
		m_pTransformCom->Update_Component(fTimeDelta);
		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
	}

	if (false == m_bDeadStart)
	{
		m_bDeadStart = true;
		m_bDeadEffect = true;
		m_bStopGravity = true;
		m_bStencil = false;

		if (m_tObjInfo.eSceneID == RESOURCE_VB)
		{
			Engine::Set_MayGreyScale(true);
			m_pDeadUI[0]->Set_Amount(0.f);
			m_pFade->Change_Color_From_To(0.7f, _vec4(0.f, 0.f, 0.f, 0.f), _vec4(0.f, 0.f, 0.f, 80.f));
			for (_int i = 0; i < 6; ++i)
			{
				m_pDeadUI[i]->Change_Color_From_To(0.5f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(255.f, 255.f, 255.f, 255.f));
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
			Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>()->Set_RayCheck(false);

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

void CMay::Lever_State(const _float & fTimeDelta)
{
	if (false == m_bLeverStart)
	{
		if (true == m_bLeverArriveRot)
		{
			m_bLeverStart = true;

			CLever::DIR eDir = static_cast<CLever*>(m_pPickUpTarget)->Get_Dir();
			if (eDir == CLever::LEFT)
			{
				Set_LeverType(LEFT_TO_RIGHT);
				static_cast<CLever*>(m_pPickUpTarget)->Set_Dir(CLever::RIGHT);
			}
			else
			{
				Set_LeverType(RIGHT_TO_LEFT);
				static_cast<CLever*>(m_pPickUpTarget)->Set_Dir(CLever::LEFT);
			}
		}
		else
		{
			_float3 vPos, vTargetPos, vOrgTargetPos;

			vPos = m_pTransformCom->Get_Position();
			vPos.y = 0.0f;
			vTargetPos = m_pPickUpTarget->Is<CLever>()->Get_Point();
			vOrgTargetPos = vTargetPos;
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
					m_bLeverMove = MoveTo(fTimeDelta, 0.2f, vOrgTargetPos, false, true, 0.15f);

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

void CMay::PickUp_State(const _float & fTimeDelta)
{
	if (!m_bInitInsertFuse)
	{
		m_bInteractable = false;
		m_bInitInsertFuse = true;
		m_bInsert_Fuse = false;
		return;
	}
	else if (m_bInsert)
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

		_vec3 vDstPos = vDir * m_fSpeed * fTimeDelta;
		PxTransform pxTrnasform = m_pController->getActor()->getGlobalPose();
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0, fTimeDelta, m_ControllFilter);
		Set_PickUpMove(true);
	}

	else
		Set_PickUpMove(false);

	if (m_bInteractable)
	{
		if (m_iKeyState & INTERACTION)
		{
			Set_Insert_Fuse();
			m_ePickUpType = PICKUPEND;
		}
		return;
	}

	if (m_bPickUpEnd)
	{
		Clear(PL_PICKUP);
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_PICKUP;

		return;
	}

	// 이벤트에서 상태를 바꿔준 후에 바로 들어오기 때문에
	// m_iKeyState & INTERACTION은 항상 true이기 때문에 맨처음엔 아래 조건을 안타기위해 m_bPickUpStart를 만듬
	if (m_iKeyState & INTERACTION2 && m_bPickUpStart && !m_bPickUpJump)
	{
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


void CMay::HangSwitch_State(const _float & fTimeDelta)
{
	if (m_bSuccess)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_IDLE;
		m_bJumpToEnd = false;
		m_bPhys = true;
		m_fGravityTime = 0.0f;
		m_bRotToStart = false;
		m_bSuccess = false;
		m_bSwitchReady = false;
		m_bHangSwitchSuccess = false;
		if (m_pPickUpTarget)
			m_pPickUpTarget->Set_MayOn(false);

		m_pController->setFootPosition(PxExtendedVec3(m_vSwitchEndPos.x, m_vSwitchEndPos.y, m_vSwitchEndPos.z));
		m_pTransformCom->Set_Pos(_vec3(m_vSwitchEndPos.x, m_vSwitchEndPos.y, m_vSwitchEndPos.z));
		m_pTransformCom->Update_Component(fTimeDelta);
		m_pMeshCom->Set_Animationset(May_Idle, 1.0f, 0.01f);
		return;
	}

	if (m_iKeyState & INTERACTION2)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_FALL;
		m_bJumpToEnd = false;
		m_bPhys = true;
		m_fGravityTime = 0.1f;
		m_bRotToStart = false;
		m_bHangSwitchSuccess = false;
		m_bSwitchReady = false;

		_float4x4 matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("LeftToeBase");
		_float4x4 matBone = pBone->CombinedTranformationMatrix;
		matBone *= matWorld;

		pBone = m_pMeshCom->Get_FrameByName("RightToeBase");
		_float4x4 matBone2 = pBone->CombinedTranformationMatrix;
		matBone2 *= matWorld;




		_float3 vPos = _vec3((matBone._41 + matBone2._41) * 0.5f, (matBone._42 + matBone2._42) * 0.5f, (matBone._43 + matBone2._43) * 0.5f);


		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		m_pTransformCom->Set_Pos(vPos);
		m_pTransformCom->Update_Component(m_fTimeDelta);
		m_pMeshCom->Set_Animationset(Cody_Jump_Fall, 1.0f, 0.01f);

		m_pPickUpTarget->Set_MayOn(false);
		return;
	}

	_bool bReady = false;
	if (nullptr != m_pPickUpTarget->Is<CPowerSwitch>())
	{
		bReady = m_pPickUpTarget->Is<CPowerSwitch>()->Get_AllReady();
	}
	if (true == Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>()->Get_SwitchReady() &&
		true == m_bSwitchReady && bReady)
	{
		Set_HangSwitchSuccess();
		m_pPickUpTarget->Set_RotStart(true);
		m_pPickUpTarget->Set_MayOn(true);
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
		if (m_bJumpToEnd = JumpTo(20.f, 1.f, _vec3(50.8f, -0.6f, -6.0f) /* m_vGoalPos */, fTimeDelta))
			m_pTransformCom->Set_Pos(_vec3(50.8f, -0.6f, -6.0f));

		_vec3 vPos = m_pTransformCom->Get_Position();
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		return;
	}
}

void CMay::Set_Dir(_float3 * vDir, _int * iRotDir, _float * fCosTh)
{
	_float4x4		matCamWorld;
	matCamWorld = static_cast<CMayCamera*>(Engine::Get_GameObject(L"GameObject", L"MayCamera"))->Get_CamMatrix();
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_float3 vPlayerLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	// 이동할 방향 구하기
	_float3 vCamDir = _float3(0.0f, 0.0f, 0.0f);
	/*if (m_iKeyState & MOVE_UP)
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
		*vDir = vCamDir;*/

		// 전방향 회전을 하기위한 식
	{
		// 카메라의 Look
		memcpy(&vCamDir, &matCamWorld.m[2][0], sizeof(_float3));
		vCamDir.y = 0;
		D3DXVec3Normalize(&vCamDir, &vCamDir);

		// 패드를 기준으로 회전
		_float4x4 matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fDirAngle));
		D3DXVec3TransformCoord(&vCamDir, &vCamDir, &matRotY);

		if (nullptr != vDir)
			*vDir = vCamDir;
	}

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


void CMay::Float_state(const _float & fTimeDelta)
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

void CMay::Fall_State(const _float & fTimeDelta)
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

void CMay::Update_Time(const _float& fTimeDelta)
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

void CMay::Push_State(const _float & fTimeDelta)
{
	if (false == m_bMoveTo)
	{
		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

		if (false == m_bRotTo)
			m_bRotTo = RotTo(fTimeDelta, 0.1f, _vec3(0.0f, 0.0f, 1.0f));

		switch (m_ePickUpType)
		{
		case CPlayerValue::DUMBBELL:
			m_vTargetPos = m_pPickUpTarget->Is<CDumbbell>()->Get_Target2();
			break;
		}
		m_bMoveTo = MoveTo(fTimeDelta, 0.1f, m_vTargetPos, true);
	}
	else if (m_iKeyState & INTERACTION2)
	{
		m_bMoveTo = false;
		m_bRotTo = false;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_PUSH;
		return;
	}
	else if (m_iKeyState & MOVE_UP)
	{
		switch (m_ePickUpType)
		{
		case DUMBBELL:
			m_pPickUpTarget->Is<CDumbbell>()->Move(fTimeDelta, CDumbbell::DUMBBELL_MAY);
			break;
		}
	}
}

void CMay::Handle_State(const _float & fTimeDelta)
{
	if (true == m_bTest)
	{
		CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());

		//#근용 수정할곳
		//m_bTest = !BoneIntoPosition("Align", _vec3(94.77f, -2.33f, -171.8f), 0.5f, fTimeDelta); 

		RotTo(fTimeDelta, 0.1f, pObj->Is<CValveHandle>()->Get_Normal());
		m_bTest = !MoveTo(fTimeDelta, 0.1f, pObj->Is<CValveHandle>()->Get_TargetPos());
		m_pTransformCom->Update_Component(fTimeDelta);
	}

	if (false == m_bHandleStart)
	{
		m_bPhys = false;
		m_eMayAnim = May_Valve_Idle;
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
		pObj->Is<CValveHandle>()->Set_Move(false);
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
			pObj->Is<CValveHandle>()->Set_AddAngle(fRad);
			pObj->Is<CValveHandle>()->Set_Move(true, 1.0f);
		}
		else if (fDir <= 0.0f && (m_iKeyState & MOVE_RIGHT))
		{
			CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());
			pObj->Is<CValveHandle>()->Set_AddAngle(-fRad);
			pObj->Is<CValveHandle>()->Set_Move(true, -1.0f);
		}

		m_vPreBoneDir = vCurDir;
	}
	else
	{
		m_bHandleRotStart = false;
		m_bHandleRot = false;
		CGameObject* pObj = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str());
		pObj->Is<CValveHandle>()->Set_Move(false);
	}
}

void CMay::Button_State(const _float & fTimeDelta)
{
	if (false == m_bTest)
	{
		RotTo(fTimeDelta, 0.05f, _vec3(0.0f, 0.0f, -1.0f));
		m_bTest = !MoveTo(fTimeDelta, 0.05f, _vec3(62.13f, 2.72f, -126.77f));
		m_pTransformCom->Update_Component(fTimeDelta);
	}

	if (true == m_bButtonEnd)
	{
		CGameObject* pCamera = Engine::Get_GameObject(L"GameObject", L"MayCamera");
		if (!pCamera)
			return;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_BUTTON;
		m_bTest = false;
		m_bButtonEnd = false;
		Engine::Get_GameObject(L"GameObject", m_tagButton.c_str())->On_Active(pCamera);
		return;
	}
}

void CMay::Shoot_State(const _float & fTimeDelta)
{
	if (false == m_bShootStart)
	{
		m_bShootStart = true;
		m_eMayAnim = May_Hose_BlowOut;
	}
	if (m_bRideGround)
	{
		m_ePre_State = m_ePL_STATE;
		m_ePL_STATE = PL_IDLE;
		m_eMayAnim = May_Idle;
		m_bRideGround = false;
		return;
	}
	else
	{
		Shoot(fTimeDelta);
	}
}

void CMay::Event_Saw_Shoot(const _float & fTimeDelta)
{
	CCircleSaw* pSaw = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str())->Is<CCircleSaw>();
	if (nullptr == pSaw)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_SAW_SHOOT;
		Clear(PL_SAW_SHOOT);
		pSaw->Set_May(false);
	}

	if (false == m_bFlyStart)
	{
		m_bFlyStart = true;
		m_eMayAnim = May_Grind_Start;
		m_vFlyPos = pSaw->Get_AttachMayPos();
		pSaw->Set_May(true);

		return;
	}
	else if (m_iKeyState & INTERACTION2)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_SAW_SHOOT;
		Clear(PL_SAW_SHOOT);
		pSaw->Set_May(false);
	}

	if (false == m_bMoveTo)
	{
		RotTo(fTimeDelta, 0.2f, _vec3(1.0f, 0.0f, 0.0f));
		m_bMoveTo = MoveTo(fTimeDelta, 0.2f, m_vFlyPos);
	}

}

void CMay::Event_Saw_Sliding(const _float & fTimeDelta)
{
	m_bStopGravity = true;
	if (m_iKeyState & INTERACTION2)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_SAW_SLIDING;
		Clear(PL_SAW_SLIDING);
	}

	if (false == m_bSlidingStart)
	{
		m_bSlidingStart = true;
		m_eMayAnim = May_Slide_Start;
	}
	else if (m_bSawJumpTrigger)
	{
		if (false == m_bSawJumpStart)
		{
			m_vSawJumpStartPos = m_pTransformCom->Get_Position();
			m_bSawJumpStart = true;
			m_fFlyTime = 0.0f;
			m_eMayAnim = May_VacuumLaunch_Idle;
		}

		_vec3 vOut;
		m_fFlyTime += fTimeDelta;
		if (m_fFlyTime >= 1.0f)
		{
			m_fFlyTime = 1.0f;
			m_ePL_STATE = PL_IDLE;
			m_ePre_State = PL_SAW_SLIDING;
			Clear(PL_SAW_SLIDING);
		}

		D3DXVec3CatmullRom(&vOut, &_vec3(m_vSawJumpStartPos.x, m_vSawJumpStartPos.y - 60.0f, m_vSawJumpStartPos.z), &m_vSawJumpStartPos, &_vec3(82.73f, -4.197f, m_vSawJumpStartPos.z), &_vec3(82.73f, -64.197f, m_vSawJumpStartPos.z), m_fFlyTime);
		m_pController->setFootPosition(PxExtendedVec3(vOut.x, vOut.y, vOut.z));
		m_pTransformCom->Set_Pos(vOut);

		return;
	}

	_vec3 vDir = _vec3(56.63f, 9.565f, 27.72f) - _vec3(-9.6f, 47.55f, 27.72f);
	D3DXVec3Normalize(&vDir, &vDir);
	m_vSlidingDir = vDir * 30.0f * fTimeDelta;

	m_pController->move(PxVec3(m_vSlidingDir.x, m_vSlidingDir.y, m_vSlidingDir.z), 0, fTimeDelta, m_ControllFilter);

	_vec3 vRight = { 0.0f, 0.0f, 0.0f };
	_float fSpeed = 2.0f;
	if (m_iKeyState & MOVE_LEFT)
	{
		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
		D3DXVec3Normalize(&vRight, &vRight);
		vRight *= fTimeDelta * -fSpeed;
		m_pController->move(PxVec3(vRight.x, vRight.y, vRight.z), 0, fTimeDelta, m_ControllFilter);
		m_eMayAnim = May_Slide_Left;
	}
	else if (m_iKeyState & MOVE_RIGHT)
	{
		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
		D3DXVec3Normalize(&vRight, &vRight);
		vRight *= fTimeDelta * fSpeed;
		m_pController->move(PxVec3(vRight.x, vRight.y, vRight.z), 0, fTimeDelta, m_ControllFilter);
		m_eMayAnim = May_Slide_Right;
	}
	else
		m_eMayAnim = May_Slide;
}

void CMay::Hit_State(const _float & fTimeDelta)
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
			m_eMayAnim = May_Walk;
			m_fCurMoveTime = 0.5f;
		}
		else
		{
			m_ePre_State = m_ePL_STATE;
			m_ePL_STATE = PL_IDLE;
			m_eMayAnim = May_Idle;
		}

		m_bHitEnd = false;
		m_bOp = true;
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		return;
	}

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		_float3 vPos, vDir;

		Move(fTimeDelta, &vPos, &vDir);

		_vec3 vDstPos = vDir * m_fMaxSpeed * fTimeDelta;
		m_pController->move(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), 0.01f, fTimeDelta, m_ControllFilter);
	}
}

void CMay::Late_Event_Saw_Shoot(const _float & fTimeDelta)
{
	CCircleSaw* pSaw = Engine::Get_GameObject(L"GameObject", m_tagHandleName.c_str())->Is<CCircleSaw>();
	if (nullptr == pSaw)
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_SAW_SHOOT;
		Clear(PL_SAW_SHOOT);
		return;
	}

	if (true == m_bFlyMove)
	{
		_vec3 vPos = pSaw->Get_AttachMayPos();
		m_fSawPosZ = vPos.z;
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		m_pTransformCom->Set_Pos(vPos);
		m_pTransformCom->Update_Component(fTimeDelta);
	}

	if (false == m_bFlyMove && true == pSaw->Get_Move())
	{
		m_bFlyMove = true;
	}
	else if (true == m_bFlyMove && false == pSaw->Get_Move())
	{
		m_bFlyMove = false;
		if (false == m_bLanch)
		{
			m_bLanch = true;
			m_eMayAnim = May_VacuumLaunch_Idle;
		}
	}

	if (false == m_bFlyMove && true == m_bLanch)
	{
		_vec3 vOut;
		m_fFlyTime += fTimeDelta;
		if (m_fFlyTime >= 1.0f)
		{
			m_fFlyTime = 1.0f;
			m_ePL_STATE = PL_SAW_SLIDING;
			m_ePre_State = PL_SAW_SHOOT;
			Clear(PL_SAW_SHOOT);
			return;
		}

		D3DXVec3CatmullRom(&vOut, &_vec3(-29.73f, -60.5f, m_fSawPosZ), &_vec3(-29.73f, 39.5f, m_fSawPosZ), &_vec3(-7.74f, 46.47f, m_fSawPosZ), &_vec3(-7.74f, -18.923f, m_fSawPosZ), m_fFlyTime);
		m_pController->setFootPosition(PxExtendedVec3(vOut.x, vOut.y, vOut.z));
		m_pTransformCom->Set_Pos(vOut);
	}
}

void CMay::Late_Climbing_State(const _float & fTimeDelta)
{
	if (true == m_bAttach)
	{
		_vec3 vDir = *((_vec3*)m_pAttachTarget->On_Attach(this));
		m_vAttachDir.y -= fTimeDelta * 0.1f;
		Check_Wall(fTimeDelta);
		PxU32 flags = 0;
		flags = m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
		PxExtendedVec3 vPos = m_pController->getFootPosition();
		m_pTransformCom->Set_Pos(_vec3((_float)vPos.x, (_float)vPos.y, (_float)vPos.z));
		m_pTransformCom->Update_Component(fTimeDelta);

		if (true == m_bCheckGround && flags == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_bAttach = false;
		}
		m_bCheckGround = false;

		if (true == m_bRot)
		{
			m_pTransformCom->Set_AddAngleY(m_fAttachAngle);
			m_pTransformCom->Update_Component(fTimeDelta);
		}
	}
	else
	{
		_vec3 vDir = _vec3(0.0f, 1.0f, 0.0f) * fTimeDelta * -0.2f;
		PxU32 flags = m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
		if (flags == PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_bGround = true;
		}
		Check_Wall(fTimeDelta);
	}
}

void CMay::RideObjet(const _float & fTimeDelta)
{
	if (true == m_bRideObj)
	{
		_vec3 vDir = *((_vec3*)m_pAttachTarget->On_Attach(this));
		PxU32 flags = 0;
		flags = m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z), 0, fTimeDelta, m_ControllFilter);
		m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + vDir);
		m_pTransformCom->Update_Component(fTimeDelta);

		if (true == m_bRot)
		{
			m_pTransformCom->Set_AddAngleY(m_fAttachAngle);
			m_pTransformCom->Update_Component(fTimeDelta);
		}
	}
}

void CMay::Key_Reset()
{
	m_iKeyState = NULL;
}

void CMay::Set_Damage(_int iDamage)
{
	if (m_ePL_STATE == PL_HIT || m_bOp)
		return;

	m_iHp += iDamage;
	Set_Hit();

	m_ePre_State = m_ePL_STATE;
	m_ePL_STATE = PL_HIT;
	Clear(PL_DEAD);
}

void CMay::Create_UI(const _tchar * pLayerTag)
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
	m_pFade->Set_Pos(_vec3{ 0.f, 0.f, 0.f });
	m_pFade->Set_RenderID(RENDER_FINALUI);
	m_pFade->Set_PassIndex(2);
	m_pFade->Set_Color(_vec4(0.f, 0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Fade", pGameObject), );
	m_pFade->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pProfile = pGameObject->Is<CImageUI>();
	m_pProfile->Set_Image(L"MC_ImageUI");
	m_pProfile->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pProfile->Set_Pos(_vec3{ 60.f, -880.f, 0.f });
	m_pProfile->Set_RenderID(RENDER_UI);
	m_pProfile->Set_PassIndex(2);
	m_pProfile->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Profile", pGameObject), );
	m_pProfile->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pHp = pGameObject->Is<CImageUI>();
	m_pHp->Set_Image(L"MC_ImageUI");
	m_pHp->Set_Size(_vec3{ 160.f, 160.f, 0.f });
	m_pHp->Set_Pos(_vec3{ 44.f, -864.f, 0.f });
	m_pHp->Set_RenderID(RENDER_UI);
	m_pHp->Set_PassIndex(14);
	m_pHp->Set_TextureIndex(1);
	m_pHp->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Hp", pGameObject), );
	m_pHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[0] = pGameObject->Is<CImageUI>();
	m_pDeadUI[0]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[0]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[0]->Set_Pos(_vec3{ 280.f, -330.f, 0.f });
	m_pDeadUI[0]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[0]->Set_PassIndex(11);
	m_pDeadUI[0]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Dead", pGameObject), );
	m_pDeadUI[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[1] = pGameObject->Is<CImageUI>();
	m_pDeadUI[1]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[1]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[1]->Set_Pos(_vec3{ 280.f, -330.f, 0.f });
	m_pDeadUI[1]->Set_RenderID(RENDER_UI);
	m_pDeadUI[1]->Set_PassIndex(12);
	m_pDeadUI[1]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Dead2", pGameObject), );
	m_pDeadUI[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[2] = pGameObject->Is<CImageUI>();
	m_pDeadUI[2]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[2]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[2]->Set_Pos(_vec3{ 280.f, -330.f, 0.f });
	m_pDeadUI[2]->Set_RenderID(RENDER_UI);
	m_pDeadUI[2]->Set_PassIndex(13);
	m_pDeadUI[2]->Set_TextureIndex(14);
	m_pDeadUI[2]->Set_Color(_vec4(231.f, 76.f, 60.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Dead3", pGameObject), );
	m_pDeadUI[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[3] = pGameObject->Is<CImageUI>();
	m_pDeadUI[3]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pDeadUI[3]->Set_Pos(_vec3{ 450.f, -650.f, 0.f });
	m_pDeadUI[3]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[3]->Set_PassIndex(2);
	m_pDeadUI[3]->Set_TextureIndex(13);
	m_pDeadUI[3]->Set_Color(236.f, 216.f, 162.f, 255.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Dead4", pGameObject), );
	m_pDeadUI[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[4] = pGameObject->Is<CImageUI>();
	m_pDeadUI[4]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[4]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pDeadUI[4]->Set_Pos(_vec3{ 460.f, -660.f, 0.f });
	m_pDeadUI[4]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[4]->Set_PassIndex(2);
	m_pDeadUI[4]->Set_TextureIndex(12);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Dead5", pGameObject), );
	m_pDeadUI[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[5] = pGameObject->Is<CImageUI>();
	m_pDeadUI[5]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[5]->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pDeadUI[5]->Set_Pos(_vec3{ 416.f, -610.f, 0.f });
	m_pDeadUI[5]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[5]->Set_PassIndex(2);
	m_pDeadUI[5]->Set_TextureIndex(15);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_Dead6", pGameObject), );
	m_pDeadUI[5]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pCodySign = pGameObject->Is<CImageUI>();
	m_pCodySign->Set_Image(L"MC_ImageUI");
	m_pCodySign->Set_Size(_vec3{ 48.f, 48.f, 0.f });
	m_pCodySign->Set_Pos(_vec3{ 0.f, -0.f, 0.f });
	m_pCodySign->Set_RenderID(RENDER_FINALUI);
	m_pCodySign->Set_PassIndex(2);
	m_pCodySign->Set_TextureIndex(16);
	m_pCodySign->Set_Color(_vec4(75.f, 255.f, 175.f, 255.f));
	m_pCodySign->Set_RoT(ROT_Z, D3DXToRadian(-90.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_CodySign", pGameObject), );
	m_pCodySign->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pFuseSign = pGameObject->Is<CImageUI>();
	m_pFuseSign->Set_Image(L"MC_ImageUI");
	m_pFuseSign->Set_Size(_vec3{ 64.f, 64.f, 0.f });
	m_pFuseSign->Set_Pos(_vec3{ 1716.f, -864.f, 0.f });
	m_pFuseSign->Set_RenderID(RENDER_FINALUI);
	m_pFuseSign->Set_PassIndex(2);
	m_pFuseSign->Set_TextureIndex(18);
	m_pFuseSign->Set_Visible(false);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"May_FuseSign", pGameObject), );
	m_pFuseSign->AddRef();

	for (_int i = 0; i < 6; ++i)
	{
		m_pDeadUI[i]->Set_Visible(false);
		m_pDeadUI[i]->Change_Size_From_To(0.1f, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f));
	}

	if (m_tObjInfo.eSceneID == RESOURCE_VF || m_tObjInfo.eSceneID == RESOURCE_VF2 || m_tObjInfo.eSceneID == RESOURCE_VF3)
	{
		m_pProfile->Set_Visible(false);
		m_pHp->Set_Visible(false);
	}

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CMay::Update_UI(const _float & fTimeDelta)
{
	HP_UI(fTimeDelta);
	Sign_UI(fTimeDelta);
	NormalMap_Dead(fTimeDelta);
	BossMap_Dead(fTimeDelta);
}

void CMay::HP_UI(const _float & fTimeDelta)
{
	if (m_pHp)
		m_pHp->Set_Amount(_float(m_iHp) / m_iMaxHp);

	if (m_iHp <= 0)
		m_ePL_STATE = PL_DEAD;
}

void CMay::Sign_UI(const _float & fTimeDelta)
{
	if (!g_bChinema)
	{
		CodySign_UI(fTimeDelta);
		FuseSign_UI(fTimeDelta);
	}

}

void CMay::CodySign_UI(const _float & fTimeDelta)
{
	CCody* pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CTransform* pCodyTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

	if (D3DXVec3Length(&(pCodyTrans->Get_Position() - m_pTransformCom->Get_Position())) < 20.f)
	{
		m_pCodySign->Set_Visible(false);
		return;
	}

	m_pCodySign->Set_Visible(true);

	_matrix matProj, matView;

	_vec3 vCodyPos = pCodyTrans->Get_Position();
	vCodyPos.y += 4.f;
	CMayCamera* pMayCam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");

	matView = pMayCam->Get_View();
	matProj = pMayCam->Get_Projection();

	D3DXVec3TransformCoord(&vCodyPos, &vCodyPos, &matView);
	D3DXVec3TransformCoord(&vCodyPos, &vCodyPos, &matProj);

	_vec3 vPos = { ((vCodyPos.x + 0.5f) * WINCX * 0.5f), (vCodyPos.y + 1.f) * WINCY * 0.5f - WINCY, vCodyPos.z };
	_vec3 FinalPos = vPos + _vec3(-24.f, 0.f, 0.f);


	_bool IsOut = false;
	if (FinalPos.x < 30.f)
	{
		IsOut = true;
		FinalPos.x = 30.f;
	}

	if (FinalPos.x >= 880.f)
	{
		IsOut = true;
		FinalPos.x = 880.f;
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

	if (FinalPos.z > 1.f)
		FinalPos *= -1.f;

	m_pCodySign->Set_Pos(FinalPos);
	_vec3 vCenter = { 480.f,-540.f,0.f };
	FinalPos = FinalPos - vCenter;
	D3DXVec3Normalize(&FinalPos, &FinalPos);
	if (IsOut)
		m_pCodySign->Set_Dir(ROT_Z, atan2f(FinalPos.x, -FinalPos.y) + D3DXToRadian(-90.f));
	else
		m_pCodySign->Set_Dir(ROT_Z, D3DXToRadian(-90.f));
}

void CMay::FuseSign_UI(const _float & fTimeDelta)
{
	if (m_tObjInfo.eSceneID != RESOURCE_VF || !m_bFuseRender)
	{
		m_pFuseSign->Set_Visible(false);
		return;
	}

	CFuseNPC* pFuse = (CFuseNPC*)Engine::Get_GameObject(L"GameObject", L"FuseNPC1");
	if (!pFuse)
		return;
	CTransform* pFuseTrans = (CTransform*)pFuse->Get_Component(L"Com_Transform", ID_DYNAMIC);


	_matrix matProj, matView;
	_vec3 vFusePos = pFuseTrans->Get_Position();
	vFusePos.y += 3.f;
	CMayCamera* pCodyCam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");

	matView = pCodyCam->Get_View();
	matProj = pCodyCam->Get_Projection();

	D3DXVec3TransformCoord(&vFusePos, &vFusePos, &matView);
	D3DXVec3TransformCoord(&vFusePos, &vFusePos, &matProj);

	m_pFuseSign->Set_Visible(true);

	_vec3 vPos = { ((vFusePos.x + 0.5f) * WINCX * 0.5f), (vFusePos.y + 1.f) * WINCY * 0.5f - WINCY, vFusePos.z };

	_vec3 FinalPos = vPos + _vec3(-16.f, 0.f, 0.f);

	//if (FinalPos.x < 30.f)
	//	FinalPos.x = 30.f;

	//if (FinalPos.x > 880.f)
	//	FinalPos.x = 880.f;

	//if (FinalPos.y > -30.f)
	//	FinalPos.y = -30.f;

	//if (FinalPos.y < -1000.f)
	//	FinalPos.y = -1000.f;

	if (FinalPos.z > 0.f &&
		FinalPos.x > 30.f && FinalPos.x > -880.f &&
		FinalPos.y > 30.f && FinalPos.y > -1000.f)
		m_pFuseSign->Set_Pos(FinalPos);

	else if (FinalPos.z < 0.f || FinalPos.z > 1.f)
		FinalPos *= -1.f;


	if (m_bFuseSign)
	{
		m_fFuseSign += 32.f * fTimeDelta * 5.f;
		if (m_fFuseSign >= 96.f)
			m_bFuseSign = false;
	}
	else
	{
		m_fFuseSign -= 32.f * fTimeDelta * 5.f;
		if (m_fFuseSign <= 64.f)
			m_bFuseSign = true;
	}

	m_pFuseSign->Set_Size(_vec3(m_fFuseSign, m_fFuseSign, 0.f));
	m_pFuseSign->Set_Pos(FinalPos);
}

void CMay::NormalMap_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadUI || m_tObjInfo.eSceneID == RESOURCE_VB)
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
			m_bFade = false;
			m_bDeadUI = false;
		}
	}

	m_pFade->Set_Color(_vec4(0.f, 0.f, 0.f, m_fFade));
}

void CMay::BossMap_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadUI || m_tObjInfo.eSceneID == RESOURCE_VF || m_tObjInfo.eSceneID == RESOURCE_VF2 || m_tObjInfo.eSceneID == RESOURCE_VF3)
		return;

	if (Engine::Key_Pressing(DIC_Y) && m_pDeadUI[0]->Get_Amount() < 1.f)
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
			Engine::Set_MayGreyScale(false);

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
				m_iHp = m_iMaxHp;
				m_bDeadStart = false;
				m_bPhys = true;
				m_ePL_STATE = PL_IDLE;
				m_ePre_State = PL_DEAD;
				m_bRespawnEffect = true;
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
				m_bFade = false;
				m_bDeadUI = false;
			}
		}
	}
	m_pFade->Set_Color(_vec4(0.f, 0.f, 0.f, m_fFade));

}

void CMay::FadeIn(const _float & fTimeDelta)
{
	if (!m_bFadeIn)
		return;

	m_pFade->Change_Color_To(fTimeDelta, 0.f, 0.f, 0.f, 0.f);
	m_bFadeIn = false;
}

void CMay::FadeOut(const _float & fTimeDelta)
{
	if (!m_bFadeOut)
		return;

	m_pFade->Change_Color_To(fTimeDelta, 0.f, 0.f, 0.f, 255.f);
	m_bFadeOut = false;
}
