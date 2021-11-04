#ifndef SpaceField3_h__
#define SpaceField3_h__


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

class CSpaceField3 : public CScene , PxSimulationEventCallback
{
private:
	explicit CSpaceField3(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpaceField3();

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

	void			Event_Spring(PxTriggerPair * pairs);
	void			Event_Dead(PxTriggerPair * pairs);
	void			Event_Scene_Change(PxTriggerPair * pairs);
	void			Event_Tube(PxTriggerPair* pairs);
	void			Event_Generator_Battery(PxTriggerPair* pairs);
	void			Event_Generator_Lever(PxTriggerPair* pairs);
	void			Event_Teleport(PxTriggerPair* pairs);
	void			Event_PushGrap(PxTriggerPair* pairs);
	void			Event_GlassBox(PxTriggerPair* pairs);
	void			Event_Save(PxTriggerPair* pairs);
	void			Clear_SetPos(const _float& fTimeDelta);

private:
	void			SCENE_FADE(const _float& fTimeDelta, SCENEID eID);


private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_Interact_Layer(const _tchar* szLayer);
	void Effect_Warp();
	HRESULT			Ready_LightInfo();

public:
	static CSpaceField3* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		m_vecObject;
	//class CPowerButton*		 m_pButton= nullptr;
	_bool				m_bEnd = false;
	_bool				m_bStart = true;


	_bool				m_bClearField = false;


private :
	_float				m_fLightTime = 0.f;
	_float				m_fLightTime2 = 0.f;
	_float				m_fLightTime3 = 0.f;

private:
	_bool				m_bSceneChange = false;
	_float				m_fSceneChange = 0.f;

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};
#endif // SpaceField3_h__
