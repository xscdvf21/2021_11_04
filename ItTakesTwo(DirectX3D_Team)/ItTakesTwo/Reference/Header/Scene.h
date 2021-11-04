#ifndef Scene_h__
#define Scene_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Layer.h"
BEGIN(Engine)
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit	CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CScene(void);

public:
	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	CLayer*			Get_Layer(const _tchar* pLayerTag);

	void setupFiltering(PxDefaultAllocator* pAllocator, PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
	void setupQueryFiltering(PxDefaultAllocator* pAllocator, PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
public:
	HRESULT			Add_Layer(const _tchar* pLayerTag, CLayer* pLayer);

public:
	void				Ready_PhysX(PxSimulationEventCallback* pEventCallback, PxPhysics* pPhys, PxCpuDispatcher* pDispatcher, PxCudaContextManager* pCudaMgr);
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void) PURE;

public:
	 PxScene*						Get_PxScene() { return m_pScene; }
	 PxControllerManager*			Get_PxControllMgr() { return m_pControllMgr; }
protected:
	virtual void					Scene_Start_Event(const _float& fTimeDelta);
public:

	void							Set_StartPos(const _vec3& vPos) { m_vStartPos = vPos; }
protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;
	class PxScene*					m_pScene;
	PxSimulationEventCallback*		m_pEventCallback = nullptr;
	PxSimulationFilterShader		m_FilterShader;
	PxControllerManager*			m_pControllMgr =nullptr;
	_bool							m_bEnd = false;
	_bool							m_bStart = true;
	_float							m_fSceneTime = 0.0f;
	_uint							m_iSequence = 0;
	_vec3							m_vStartPos = { 0.f,0.f,0.f };
public:
	virtual void	Free(void);



private:
	_bool m_bChangeFlag = false;
};

END
#endif // Scene_h__
