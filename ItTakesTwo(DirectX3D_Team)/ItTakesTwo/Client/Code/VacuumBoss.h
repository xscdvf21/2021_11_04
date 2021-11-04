#ifndef VacuumBoss_h__
#define VacuumBoss_h__


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

class CVacuumBoss : public CScene , PxSimulationEventCallback
{
private:
	explicit CVacuumBoss(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CVacuumBoss();

	void Event_Vacuum_Enter(PxTriggerPair * pairs);
	void Event_Ride(PxTriggerPair * pairs);
	void Event_Stone(PxTriggerPair* pairs);
	void Event_Bomb(PxTriggerPair* pairs);
	void Event_Dead(PxTriggerPair* pairs);
	void Event_Ending_Cody(PxTriggerPair* pairs);
	void Event_Ending_May(PxTriggerPair* pairs);
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
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_LoadMap_Layer(const _tchar*  pLayerTag, const _tchar* pFilePath);
	HRESULT			Ready_LightInfo();

public:

	static CVacuumBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Ready_Interact_Layer(const _tchar* szLayer);
	vector<_tchar*>		 m_vecObject;
	vector<_vec3>		 m_vecChessTile;

private:
	virtual void	Free() override;

	void SCENE_FADE(const _float & fTimeDelta, SCENEID eID);
private:
	_bool				m_bSceneChange = false;
	_float				m_fSceneChange = 0.f;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};

#endif // VacuumBoss_h__
