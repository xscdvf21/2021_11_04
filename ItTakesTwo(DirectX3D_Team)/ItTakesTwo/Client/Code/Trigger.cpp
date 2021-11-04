#include "stdafx.h"
#include "Trigger.h"

vector<CTrigger*> CTrigger::m_vecTrigger = vector<CTrigger*>();


const Engine::_bool CTrigger::Is_In()
{
	return m_bActive;
}

CTrigger::CTrigger(PxScene* pScene, PxGeometry& pGeo, void* GameObjectPointer, const _vec3& vPos, PxU32 FilterGroup, PxU32 FilterMask)
{

	auto* pPhys = Engine::Get_Physics();
	m_vPos = { vPos.x, vPos.y, vPos.z };
	m_qRot = PxQuat(1);
	m_pRigidActor = pPhys->createRigidStatic(PxTransform(m_vPos, m_qRot));
	PxShape* pShape = pPhys->createShape(pGeo, *pPhys->createMaterial(0.5f, 0.5f, 0.f), true, PxShapeFlag::eTRIGGER_SHAPE);
	m_pShape = pShape;

	m_pRigidActor->attachShape(*m_pShape);
	m_pRigidActor->setName((char*)this);
	m_pShape->setName((char*)this);
	m_pConnectClass = GameObjectPointer;
	PxFilterData filterData;
	filterData.word0 = FilterGroup;
	filterData.word1 = FilterMask;
	m_pShape->setSimulationFilterData(filterData);
	pScene->addActor(*m_pRigidActor);
}

CTrigger* CTrigger::CreateBoxTrigger(PxScene* pScene, void* GameObjectPointer, const _vec3& vPos, const _float& fXHalf, const _float& fYHalf, const _float& fZHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup, PxU32 FilterMask)
{
	CTrigger* pInstance = nullptr;
	pInstance = new CTrigger(pScene, PxBoxGeometry(fXHalf, fYHalf, fZHalf), GameObjectPointer, vPos, FilterGroup, FilterGroup);
	pInstance->Set_TriggerType(eTriggerType);
	m_vecTrigger.emplace_back(pInstance);
	return pInstance;
}

CTrigger* CTrigger::CreateBoxTrigger(PxScene* pScene, void* GameObjectPointer, const _vec3& vPos, const _float& fHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup, PxU32 FilterMask)
{
	CTrigger* pInstance = nullptr;
	pInstance = new CTrigger(pScene, PxBoxGeometry(fHalf, fHalf, fHalf), GameObjectPointer, vPos, FilterGroup, FilterGroup);
	pInstance->Set_TriggerType(eTriggerType);
	m_vecTrigger.emplace_back(pInstance);
	return pInstance;

}

CTrigger* CTrigger::CreateSphereTirgger(PxScene* pScene, void* GameObjectPointer, const _vec3& vPos, const _float& fHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup, PxU32 FilterMask)
{
	CTrigger* pInstance = nullptr;
	pInstance = new 	CTrigger(pScene, PxSphereGeometry(fHalf), GameObjectPointer, vPos, FilterGroup, FilterGroup);
	pInstance->Set_TriggerType(eTriggerType);
	m_vecTrigger.emplace_back(pInstance);
	return pInstance;

}

CTrigger* CTrigger::CreateCapsuleTirgger(PxScene* pScene, void* GameObjectPointer, const _vec3& vPos, const _float& fRadius, const _float& fHeightHalf, const TRIGGERTYPE& eTriggerType, PxU32 FilterGroup, PxU32 FilterMask)
{
	CTrigger* pInstance = nullptr;
	pInstance = new CTrigger(pScene, PxCapsuleGeometry(fRadius, fHeightHalf), GameObjectPointer, vPos, FilterGroup, FilterGroup);
	pInstance->Set_TriggerType(eTriggerType);
	m_vecTrigger.emplace_back(pInstance);
	return pInstance;

}

void CTrigger::Clear_Trigger()
{
	for (auto& pTrigger : m_vecTrigger)
		delete pTrigger;
	m_vecTrigger.clear();
}

void CTrigger::Set_Pos(const _vec3& vPos)
{
	m_vPos = { vPos.x ,vPos.y,vPos.z };
	Set_GlobalPos();
}

void CTrigger::Set_Pos(const PxVec3& vPos)
{
	m_vPos = vPos;
	Set_GlobalPos();
}

void CTrigger::Set_Pos(const PxTransform& vPos)
{
	m_vPos = vPos.p;
	Set_GlobalPos();
}

void CTrigger::Set_Pos(const PxExtendedVec3& vPos)
{
	m_vPos = toVec3(vPos);
	Set_GlobalPos();
}

void CTrigger::Set_Rotation(const _vec3& vAngle)
{
	m_qRot = PxQuat(vAngle.x, PxVec3(1.f, 0.f, 0.f))*PxQuat(vAngle.y, PxVec3(0.f, 1.f, 0.f))*PxQuat(vAngle.z, PxVec3(0.f, 0.f, 1.f));
	Set_GlobalPos();
}
void CTrigger::Set_Rotation(const PxQuat& vAngle)
{
	m_qRot = vAngle;
	Set_GlobalPos();
}



void CTrigger::Set_GlobalPos()
{
	m_pRigidActor->setGlobalPose(PxTransform(m_vPos, m_qRot));
}

void CTrigger::Set_Interactable(const _bool& bState)
{
	m_pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, bState);
}



const Engine::_uint CTrigger::Set_FloatValue(const _float& fValue)
{
	m_vecFloatValues.emplace_back(fValue);
	return (_uint)m_vecFloatValues.size() - 1;
}

void CTrigger::Set_Scale(const _float& fScale)
{
	PxShape* pShape;
	m_pRigidActor->getShapes(&pShape, 1);
	PxBoxGeometry boxGeometry = PxBoxGeometry(1.f, 1.f, fScale);
	if (!boxGeometry.isValid())
		return;
	pShape->setGeometry(boxGeometry);


	//PxScaleRigidActor(*m_pRigidActor, fScale);
}
void CTrigger::Set_Scale(const _vec3& fScale)
{
	PxShape* pShape;
	m_pRigidActor->getShapes(&pShape, 1);
	PxBoxGeometry boxGeometry = PxBoxGeometry(fScale.x, fScale.y, fScale.z);

	if (!boxGeometry.isValid())
		return;
	pShape->setGeometry(boxGeometry);



	//PxScaleRigidActor(*m_pRigidActor, fScale);
}

const Engine::_float CTrigger::Get_FloatValue(const _uint& iIndex)
{
	if (m_vecFloatValues.empty())
		return 0.f;
	if (iIndex >= m_vecFloatValues.size())
		return 0.f;

	return m_vecFloatValues[iIndex];
}

void* CTrigger::Activate(void* pCaller)
{
	if (m_pConnectClass)
		return ToObj<CGameObject>(m_pConnectClass)->On_Trigger_Active(this,pCaller);
	return nullptr;
}

void CTrigger::Free(void)
{

}

