#include "GameObject.h"
#include "..\System\Code\SoundMgr.h"

USING(Engine)


Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bDead(false), m_bHit(false), m_fColDis(0.f)

{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{
}

HRESULT Engine::CGameObject::Ready_Object(void* pArg)
{
	return S_OK;
}

Engine::_int Engine::CGameObject::Update_Object(const _float& TimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(TimeDelta);
	Update_Reserve(TimeDelta);
	return 0;
}

Engine::_int Engine::CGameObject::LateUpdate_Object(const _float& TimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component(TimeDelta);

	return 0;
}

void Engine::CGameObject::Render_Object(const _int& iIndex)
{

}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		//for (auto& pcom : m_mapComponent[i])
		//	Safe_Release(pcom.second);
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

CComponent* Engine::CGameObject::Get_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	CComponent*	pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

CComponent* Engine::CGameObject::Find_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void Engine::CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

void CGameObject::Compute_ViewZ()
{
	if (m_pGraphicDev == nullptr)
		return;

	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos, vPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));
	vPos = m_pTransformCom->Get_Position();

	m_fViewZ = D3DXVec3Length(&(vCamPos - vPos));
}

void Engine::CGameObject::PlaySoundOnce(TCHAR* tagFile, const CSoundMgr::CHANNELID& eID, const _float& fVolum)
{
	auto iter = m_mapSound.find(tagFile);
	if (iter == m_mapSound.end())
	{
		m_mapSound.emplace(tagFile, true);
		CSoundMgr::GetInstance()->StopNPlaySound(tagFile, eID, fVolum);
	}
}

void Engine::CGameObject::Set_QueryFilter(const PxFilterData& filterData)
{
	if (m_pRigidBody)
	{
		m_pRigidBody->setSimulationFilterData(filterData);
	}
}

void Engine::CGameObject::Set_SimulationFilter(const PxFilterData& filterData)
{
	if (m_pRigidBody)
	{
		m_pRigidBody->setQueryFilterData(filterData);
	}
}

void* Engine::CGameObject::On_Active(void* pCaller)
{
	return nullptr;
}

void Engine::CGameObject::Set_Controllable(ControlledActorDesc* pDesc)
{
	m_tControllerDesc = *pDesc;
	m_bControllable = true;

}

PxController* Engine::CGameObject::CreateController(PxControllerManager* pManager)
{
	const float radius = m_tControllerDesc.mRadius;
	float height = m_tControllerDesc.mHeight;
	float crouchHeight = m_tControllerDesc.mCrouchHeight;
	PxControllerDesc* tDesc;
	PxCapsuleControllerDesc capsuleDesc;
	PxBoxControllerDesc boxDesc;
	if (m_tControllerDesc.mType == PxControllerShapeType::eBOX)
	{

		height *= 0.5f;
		height += radius;
		crouchHeight *= 0.5f;
		crouchHeight += radius;
		boxDesc.halfHeight = height;
		boxDesc.halfSideExtent = radius;
		boxDesc.halfForwardExtent = radius;
		tDesc = &boxDesc;
	}
	else
	{
		PX_ASSERT(desc.mType == PxControllerShapeType::eCAPSULE);
		capsuleDesc.height = height;
		capsuleDesc.radius = radius;
		capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		tDesc = &capsuleDesc;
	}
	tDesc->density = m_tControllerDesc.mProxyDensity;
	tDesc->scaleCoeff = m_tControllerDesc.mProxyScale;
	tDesc->material = m_pMaterial;
	tDesc->position = m_tControllerDesc.mPosition;
	tDesc->slopeLimit = m_tControllerDesc.mSlopeLimit;
	tDesc->contactOffset = m_tControllerDesc.mContactOffset;
	tDesc->stepOffset = m_tControllerDesc.mStepOffset;
	tDesc->invisibleWallHeight = m_tControllerDesc.mInvisibleWallHeight;
	tDesc->maxJumpHeight = m_tControllerDesc.mMaxJumpHeight;
	//	tDesc->nonWalkableMode		= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	tDesc->reportCallback = m_tControllerDesc.mReportCallback;
	tDesc->behaviorCallback = m_tControllerDesc.mBehaviorCallback;
	tDesc->volumeGrowth = m_tControllerDesc.mVolumeGrowth;

	//m_ControllFilter = PxControllerFilters(FilterGroup::ePlayer, this, this);
	//mType = m_tControllerDesc.mType;
	//mInitialPosition = m_tControllerDesc.mPosition;
	//mStandingSize = height;
	//mCrouchingSize = crouchHeight;
	//mControllerRadius = radius;
	m_pController = pManager->createController(*tDesc);

	PxShape* pShape;
	m_pController->getActor()->getShapes(&pShape, 1);
	m_pController->getActor()->setMass(5);
	auto* pActor = m_pController->getActor();
	m_pRigidActor = pActor;
	m_pRigidBody = pShape;
	Set_ActorSetting(pActor, pShape);
	return m_pController;
}

void Engine::CGameObject::Set_Controller(PxController* pController)
{
	m_pController = pController;
}

void Engine::CGameObject::Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody)
{

}
void Engine::CGameObject::Create_Actor(
	PxPhysics* pPhys,
	const _float& fHeight,
	const _float& fRadius,
	PxUserControllerHitReport* pHitCallback,
	PxControllerBehaviorCallback* pBehaviorCallback,
	_vec3 vPos,
	_vec3 vDir,
	const PxMaterials& tMatrial)
{
	m_pMaterial = pPhys->createMaterial(tMatrial.StaticFriction, tMatrial.DynamicFriction, tMatrial.Restitution);
	ControlledActorDesc* tDesc = &ControlledActorDesc();
	tDesc->mHeight = fHeight;
	tDesc->mRadius = fRadius;
	tDesc->mPosition = PxExtendedVec3{ vPos.x,vPos.y,vPos.z };
	tDesc->mReportCallback = pHitCallback;
	tDesc->mBehaviorCallback = pBehaviorCallback;
	Set_Controllable(tDesc);
}


void Engine::CGameObject::Set_Interactable(void* pInteracter, const _bool& bState /*= true*/)
{
	if (bState)
	{
		m_pInteractActor = (void*)pInteracter;
		m_bInteractable = true;
	}
	else
	{
		if (m_pInteractActor == pInteracter)
		{
			m_pInteractActor = nullptr;
			m_bInteractable = false;
		}
	}
}

void Engine::CGameObject::Set_Scene(PxScene* pScene)
{
	m_pScene = pScene;
}

void Engine::CGameObject::Set_ControllerFilter()
{

}
//For Dynamic,Static Object
void Engine::CGameObject::Set_Contact(PxShape* pShape, _bool bState /*=true*/)
{
	if (pShape)
		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, bState);
}

//For Dynamic,Static Object
void Engine::CGameObject::Set_Trigger(PxShape* pShape, _bool bState /*= true*/)
{
	if (pShape)
	{

		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !bState);
		pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, bState);
	}
}

void Engine::CGameObject::Set_Pos(const _vec3& vPos)
{
	if (m_pTransformCom)
		m_pTransformCom->Set_Pos(vPos);
	if (m_pController)
	{
		m_pController->setFootPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
	}
	else
	{
		if (m_pRigidActor)
			m_pRigidActor->setGlobalPose(PxTransform(vPos.x, vPos.y, vPos.z));
	}
}


void Engine::CGameObject::Set_InteractObj(void* pInteracter)
{
	m_vecInteractActor.emplace_back(pInteracter);
	m_pInteractActor = pInteracter;
}

void Engine::CGameObject::Set_ReservePos(const _vec3& vPos)
{
	m_bReserve = true;
	m_vMovePos = vPos;
}

void Engine::CGameObject::Update_Reserve(const _float& fTimeDelta)
{
	if (!m_bReserve)
		return;
	Set_Pos(m_vMovePos);
	m_bReserve = false;
}

void* Engine::CGameObject::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	return nullptr;

}

void * CGameObject::On_Attach(void * pCaller)
{
	return nullptr;
}

void Engine::CGameObject::Move_Left()
{

}

void Engine::CGameObject::Move_Right()
{

}

void Engine::CGameObject::Move_Up()
{

}

void Engine::CGameObject::Move_Down()
{

}

_mat Engine::CGameObject::Get_EventPos()
{
	return _mat{};
}

void Engine::CGameObject::Set_SimulationState(const _bool& bState)
{
	m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !bState);
	m_bSimulate = bState;
}

