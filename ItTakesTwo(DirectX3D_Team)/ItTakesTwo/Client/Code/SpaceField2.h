#ifndef SpaceField2_h__
#define SpaceField2_h__


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
#include "GameObject.h"
#include "Cody.h"
#include "May.h"

class CSpaceField2 : public CScene , PxSimulationEventCallback
{
private:
	explicit CSpaceField2(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpaceField2();

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
	void			Event_Bounce_Switch(PxTriggerPair* pairs);
	void			Event_Float(PxTriggerPair* pairs);

	void			Event_Dead(PxTriggerPair * pairs);
	void			Event_Scene_Change(PxTriggerPair * pairs);
	void			Event_SpaceWall(PxTriggerPair* pairs);
	void			Event_Handle(PxTriggerPair* pairs);
	void			Event_Lever_Cody(PxTriggerPair* pairs);
	void			Event_Lever_May(PxTriggerPair* pairs);
	void			Event_Generator_Battery(PxTriggerPair* pairs);
	void			Event_SpringPlatform(PxTriggerPair* pairs);
	void			Event_Generator_Lever(PxTriggerPair* pairs);
	void			Event_Pos_Change(PxTriggerPair* pairs);
	void			Event_PlatformDead(PxTriggerPair* pairs);
	void			Event_PlatformRespwan(PxTriggerPair* pairs);
	void			Event_Save(PxTriggerPair* pairs);

private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_Interact_Layer(const _tchar* szLayer);
	HRESULT			Ready_Valve_Layer(const _tchar* szLayer);
	HRESULT			Ready_LightInfo();


private :
	void			SceneChange(const _float& fTimeDelta);


public:
	static CSpaceField2* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		 m_vecObject;
	//class CPowerButton*		 m_pButton= nullptr;
	_bool				 m_bEnd = false;
	_bool				m_bStart = true;

private :
	CTrigger*			m_pTrigger[TRIGGER_END];
	_bool				m_bGenerator = false;

private:
	void				Effect_Warp();

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};
#endif // SpaceField2_h__
