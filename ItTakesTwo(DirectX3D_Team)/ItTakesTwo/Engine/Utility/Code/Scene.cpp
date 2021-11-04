#include "Scene.h"
#include "include\PxSceneDesc.h"
#include "LightMgr.h"

PxFilterFlags FilterShader
(PxFilterObjectAttributes attributes0,
	PxFilterData filterData0,
	PxFilterObjectAttributes attributes1,
	PxFilterData filterData1,
	PxPairFlags& pairFlags,
	const void* constantBlock,
	PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	//if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eCody| FilterGroup::eMay))
	//	return PxFilterFlag::eKILL;

	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eOgre | FilterGroup::eCody))
		return PxFilterFlag::eKILL;

	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eOgre | FilterGroup::eMonster))
		return PxFilterFlag::eKILL;

	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eOgre | FilterGroup::eMay))
		return PxFilterFlag::eKILL;

	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eMonster | FilterGroup::eCody))
		return PxFilterFlag::eKILL;

	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eMonster | FilterGroup::eMay))
		return PxFilterFlag::eKILL;

	//if ((filterData0.word0 | filterData1.word0) == FilterGroup::ePlayer | FilterGroup::eHoze)
	//{
	//	return PxFilterFlag::eSUPPRESS;
	//}
	// generate contacts for all that were not filtered above

	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eInteractItem | FilterGroup::eHoze))
		return PxFilterFlag::eKILL;
	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eInteractItem |  FilterGroup::eCody))
		return PxFilterFlag::eKILL;
	if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eInteractItem | FilterGroup::eMay))
		return PxFilterFlag::eKILL;

	//if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eHoze | FilterGroup::eMay))
	//	return PxFilterFlag::eSUPPRESS;	
	//if ((filterData0.word0 | filterData1.word0) == (FilterGroup::eHoze | FilterGroup::eCody))
	//	return PxFilterFlag::eSUPPRESS;

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}
USING(Engine)
Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

HRESULT CScene::Add_Layer(const _tchar * pLayerTag, CLayer * pLayer)
{
	if (nullptr == pLayer)
		return E_FAIL;

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

void Engine::CScene::Scene_Start_Event(const _float& fTimeDelta)
{

}

void Engine::CScene::Ready_PhysX(PxSimulationEventCallback* pEventCallback, PxPhysics* pPhys, PxCpuDispatcher* pDispatcher, PxCudaContextManager* pCudaMgr)
{
	PxSceneDesc SceneDesc(pPhys->getTolerancesScale());
	SceneDesc.cpuDispatcher = pDispatcher;
	SceneDesc.simulationEventCallback = pEventCallback;
	SceneDesc.filterShader = FilterShader;
	//SceneDesc.cudaContextManager = pCudaMgr;
	//SceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
	//SceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;

	m_pScene = pPhys->createScene(SceneDesc);
	m_pControllMgr = PxCreateControllerManager(*m_pScene);
}

HRESULT Engine::CScene::Ready_Scene(void)
{


	return S_OK;
}

Engine::_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	int iEvent = 0;
	for (auto& rPair : m_mapLayer)
	{
		iEvent = rPair.second->Update_Layer(fTimeDelta);
		if (m_bChangeFlag || iEvent == SCENE_CHANGE)
			return iEvent;
	}
	return iEvent;
}

Engine::_int Engine::CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer(fTimeDelta);

	return _int();
}

void CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
	if (m_pControllMgr)
		m_pControllMgr->release();
	Safe_Release(m_pGraphicDev);
}


CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CGameObject * CScene::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

CLayer * CScene::Get_Layer(const _tchar * pLayerTag)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;
	return iter->second;
}




void CScene::setupFiltering(PxDefaultAllocator* pAllocator, PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	//Sample

		//filterGroup 
		//ÀÌ ¿¢ÅÍ°¡ ¹«½¼ ±×·ìÀÎ°¡
		//filterMask
		//ÀÌ ¿¢ÅÍ°¡ ¹«½¼ ¾ÖµéÀÌ¶û Ãæµ¹ÇÒ°ÍÀÎ°¡
		//ex) FilterGroup::eGround|FilterGroup::eWall

	if (!actor)
		return;
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)pAllocator->allocate(sizeof(PxShape*)*numShapes, 0, 0, 0);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	pAllocator->deallocate(shapes);


}
void CScene::setupQueryFiltering(PxDefaultAllocator* pAllocator, PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	//Sample

	//filterGroup 
	//ÀÌ ¿¢ÅÍ°¡ ¹«½¼ ±×·ìÀÎ°¡
	//filterMask
	//ÀÌ ¿¢ÅÍ°¡ ¹«½¼ ¾ÖµéÀÌ¶û Ãæµ¹ÇÒ°ÍÀÎ°¡
	//ex) FilterGroup::eGround|FilterGroup::eWall

	if (!actor)
		return;
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)pAllocator->allocate(sizeof(PxShape*)*numShapes, 0, 0, 0);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setQueryFilterData(filterData);
	}
	pAllocator->deallocate(shapes);


}