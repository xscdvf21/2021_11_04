#ifndef CPhys_Manager_h__
#define CPhys_Manager_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPhys_Manager : public CBase
{
	DECLARE_SINGLETON(CPhys_Manager)

private:
	explicit	CPhys_Manager(void);
	virtual		~CPhys_Manager(void);

public:
	PxPhysics*					Get_Physics(void) { return m_pPhysics; }
	PxDefaultCpuDispatcher*		Get_Dispatcher(void) { return m_pDispatcher; }
	PxCooking*					Get_Cook(void) { return m_pCook; }
	PxDefaultAllocator*			Get_Allocator() { return &m_Allocator; }
	PxCudaContextManager*		Get_CudaMgr() { return m_pCudaContextManager; }

public:
	HRESULT Ready_Physics(CPhys_Manager** pManager);

private:
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;
	PxFoundation*			m_pFoundation = nullptr;
	PxPhysics*				m_pPhysics = nullptr;
	PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	PxCooking*				m_pCook = nullptr;
	PxPvd*					m_pPVD = nullptr;
	PxPvdTransport*			m_pTransport = nullptr;
	PxCudaContextManager*	m_pCudaContextManager = nullptr;
public:
	virtual void Free(void);
};

END
#endif // CPhys_Manager_h__
