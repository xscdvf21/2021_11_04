#ifndef SpaceBoss_h__
#define SpaceBoss_h__


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

class CSpaceBoss : public CScene, PxSimulationEventCallback
{
private:
	explicit CSpaceBoss(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpaceBoss();

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

	//////////////////////////////////////////////////////////////////////////
	void			Event_LaserMirror(PxTriggerPair* pairs); //레이저 반사 오브젝트 위로올라오게
	void			Event_Missile_Boss(PxTriggerPair* pairs); //플레이어가 피 닳음.
	void			Event_Missile_Player(PxTriggerPair* pairs); //보스가 피 닳음
	void			Event_Missile_Ride(PxTriggerPair* pairs);

private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_Interact_Layer(const _tchar* szLayer);
	void SCENE_FADE(const _float & fTimeDelta, SCENEID eID);
	HRESULT			Ready_LightInfo();

private:
	void			LightMove(const _float fTimeDelta);

public:
	void			Set_LightMove() { m_bLightMove = true; }

public:
	static CSpaceBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		 m_vecObject;
	//class CPowerButton*		 m_pButton= nullptr;
	_bool				 m_bEnd = false;
	_bool				m_bStart = true;

	CLight*				m_pLight[18];
	_bool				m_bLightMove = false;
	_float				m_fAddPos = 0.f;

private:
	_bool				m_bSceneChange = false;
	_float				m_fSceneChange = 0.f;

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};
#endif // SpaceBoss_h__
