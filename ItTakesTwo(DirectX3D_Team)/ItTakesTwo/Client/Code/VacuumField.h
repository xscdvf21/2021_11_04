#ifndef VacuumField_h__
#define VacuumField_h__


#include "Scene.h"
#include "Base.h"
#include "Loading.h"
#include "MainCamera.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Navi.h"
#include "Logo.h"
#include "Fodder.h"
#include "Particle.h"
#include <PxSimulationEventCallback.h>

class CVacuumField : public CScene , PxSimulationEventCallback
{
private:
	explicit CVacuumField(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CVacuumField();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene() override;

	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;


	virtual void onWake(PxActor** actors, PxU32 count) override;


	virtual void onSleep(PxActor** actors, PxU32 count) override;


	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;


	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;


	virtual void onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;


private:
	void			Event_Fuse(PxTriggerPair* pairs);
	void			Event_Lever(PxTriggerPair* pairs);
	void			Event_Vacuum_Enter(PxTriggerPair* pairs);

	void			Event_Insert_Fuse(PxTriggerPair* pairs);
	void			Event_PowerSwitch(PxTriggerPair* pairs);
	void			Event_Scene_Change(PxTriggerPair* pairs);
	void			Event_Camera_Change(PxTriggerPair* pairs);
	void			Event_Camera_Change_Fix(PxTriggerPair* pairs);
	void			Event_Dead(PxTriggerPair* pairs);
	void			Event_Save(PxTriggerPair* pairs);
	void			Event_Shoot(PxTriggerPair* pairs);
	void			Event_Fly(PxTriggerPair* pairs);
	void			Event_SawJump(PxTriggerPair* pairs);
	void			Event_Sound(PxTriggerPair * pairs);


private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_Interact_Layer(const _tchar* szLayer);
	HRESULT			Ready_Trigger_Layer(const _tchar* szLayer);
	HRESULT			Ready_LoadMap_Layer(const _tchar*  pLayerTag, const _tchar* pFilePath);
	HRESULT			Ready_LightInfo();

public:
	static CVacuumField* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		 m_vecObject;
	vector<_vec3>		 m_vecChessTile;

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};
#endif // VacuumField_h__
