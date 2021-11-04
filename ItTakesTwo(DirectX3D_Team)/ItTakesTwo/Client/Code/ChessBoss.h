#ifndef ChessBoss_h__
#define ChessBoss_h__


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
#include "ImageUI.h"

class CChessBoss : public CScene , PxSimulationEventCallback
{
private:
	explicit CChessBoss(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CChessBoss();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene() override;

	virtual void	onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void	onWake(PxActor** actors, PxU32 count) override;
	virtual void	onSleep(PxActor** actors, PxU32 count) override;
	virtual void	onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void	onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void	onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_LoadMap_Layer(const _tchar*  pLayerTag, const _tchar* pFilePath);
	HRESULT			Ready_LightInfo();

private:
	void			Event_ATTACK(PxTriggerPair * pairs);
	void			Event_Shuttle(PxTriggerPair* pairs);
	void			Event_ShuttleDoor(PxTriggerPair* pairs);

	void			Create_BossUI(const _tchar * pLayerTag);
	void			Check_BossHp();

public:
	static CChessBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		 m_vecObject;
	vector<_vec3>		 m_vecChessTile;

private:
	_int			m_iBossMaxHp = 100;
	CImageUI*		m_pBossName;
	CImageUI*		m_pBossHp;
	CImageUI*		m_pBossHpbg;

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
};

#endif // ChessBoss_h__
