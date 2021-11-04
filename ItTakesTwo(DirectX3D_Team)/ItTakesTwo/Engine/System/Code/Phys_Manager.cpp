#include "Phys_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CPhys_Manager)

Engine::CPhys_Manager::CPhys_Manager(void)
	:m_pPhysics(nullptr)
	, m_pFoundation(nullptr)
{

}

Engine::CPhys_Manager::~CPhys_Manager(void)
{
	Free();
}

void Engine::CPhys_Manager::Free(void)
{
	_ulong dwRefCnt = 0;

	m_pDispatcher->release();
	m_pPhysics->release();
	m_pCudaContextManager->release();
	m_pPVD->release();
	m_pCook->release();
	PxCloseExtensions();
	m_pTransport->release();
	m_pFoundation->release();
}



HRESULT Engine::CPhys_Manager::Ready_Physics(CPhys_Manager** pManager)
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_pPVD = PxCreatePvd(*m_pFoundation);
	m_pTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 100);
	m_pPVD->connect(*m_pTransport, PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true,m_pPVD);
	PxInitExtensions(*m_pPhysics, m_pPVD);

	PxCookingParams tParmas(m_pPhysics->getTolerancesScale());
	m_pCook = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, tParmas);

	m_pDispatcher = PxDefaultCpuDispatcherCreate(20);
	PxCudaContextManagerDesc cudaContextManagerDesc;

	m_pCudaContextManager = PxCreateCudaContextManager(*m_pFoundation, cudaContextManagerDesc, PxGetProfilerCallback());


	*pManager = this;
	return S_OK;
}

