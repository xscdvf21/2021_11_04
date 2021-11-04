#ifndef SpaceField_h__
#define SpaceField_h__


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
#include "Cody.h"
#include "May.h"

class CSpaceField : public CScene , PxSimulationEventCallback
{

	typedef enum TRIGGER_ID
	{
		Trigger_GravityFly,
		Trigger_GravityFly_BossRoom,
		Trigger_End
	}SpaceTrigger;

private:
	explicit CSpaceField(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpaceField();

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

	void			Event_Gravity_Fly(PxTriggerPair * pairs);
	void			Event_Gravity_FlyBossRoom(PxTriggerPair* pairs);
	void			Event_Generator_Battery(PxTriggerPair* pairs);
	void			Event_Generator_Lever(PxTriggerPair* pairs);
	void			Event_Hang(PxTriggerPair* pairs);
	void			SceneChange(const _float& fTimeDelta);

private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_Interact_Layer(const _tchar* szLayer);
	HRESULT			Ready_LightInfo();

public:
	static CSpaceField* Create(LPDIRECT3DDEVICE9 pGraphicDev,const _vec3& vPos);
	static CSpaceField* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		 m_vecObject;
	//class CPowerButton*		 m_pButton= nullptr;
	_bool				 m_bEnd = false;
	_bool				m_bStart = true;
	_bool				m_bCylinder = false;
	_bool				m_bCylinder2 = false;


	CTrigger*			m_pTrigger[SpaceTrigger::Trigger_End];

private:
	void				Cylinder_Effect();
	void				Cylinder_Effect2();
	void				Effect_Warp();

	
	
private:
	_bool				m_bGenerator = false;
private:
	virtual void	Free() override;

	void Load_Map(const _wstr& tagFile);
	void Create_Map(Engine::CLayer* pLayer, const _wstr& tagFile);
private:
	_matrix			m_matWorld, m_matView, m_matProj;
};
#endif // SpaceField_h__
