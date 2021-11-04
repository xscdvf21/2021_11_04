#ifndef ChessField_h__
#define ChessField_h__


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

class CChessField : public CScene, PxSimulationEventCallback
{
private:
	explicit CChessField(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CChessField();

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene() override;

public:
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;


private:
	void			Event_Scene_Change(PxTriggerPair *pairs);

	void			Event_ATTACK(PxTriggerPair * pairs);
	void			Event_DoorButton(PxTriggerPair* pairs); //문 발판
	void			Event_FireButton(PxTriggerPair* pairs); //불 기둥 발판
	void			Event_BridgeButton(PxTriggerPair* pairs); //다리 발판
	void			Event_Arrow_Dead(PxTriggerPair* pairs); //화살 벽 충돌시 데드.
	void			Event_CrusherButton(PxTriggerPair* pairs); //크러셔 시작 발판.
	void			Event_ArrowHit(PxTriggerPair* pairs); //플레이어와 화살 충돌.
															   //첫번째 맵
	void			Event_KnightMove(PxTriggerPair* pairs); //첫번째 기사들 움직이는 트리거.
	void			Event_Two_KinghtMove(PxTriggerPair* pairs); //두번째 기사들 움직이는 트리거.

																//두번째 맵
	void			Event_Field2_CreateMonster(PxTriggerPair* pairs); // 두번째 맵 몬스터 생성
	void			Event_Field2_MoveArbalist(PxTriggerPair* pairs); //두번째 맵 아처 5마리 무브 트리거.
	void			Event_Field2_MoveKnight(PxTriggerPair* pairs); //두번째 맵 기사 5마리 무브 트리거.

																   //세번째 맵
	void			Event_Crusher_Hit(PxTriggerPair* pairs); //Crusher 다리자물쇠 릴 수 있게.
	void			Event_Field3_CreateMonster(PxTriggerPair* pairs); //세번째 맵 몬스터 생성
	void			Event_Field3_Room1Move(PxTriggerPair* pairs); //1번방 무브 트리거
	void			Event_Field3_Room2Move(PxTriggerPair* pairs); //2번방 무브 트리거
	void			Event_Field3_Room3Move(PxTriggerPair* pairs); //3번방 무브 트리거
	void			Event_Field3_Room4Move(PxTriggerPair* pairs); //4번방 무브 트리거
	void			Event_Field3_Room5Move(PxTriggerPair* pairs); //5번방 무브 트리거
	void			Event_Field3_Room6Move(PxTriggerPair* pairs); //6번방 무브 트리거


	
														  //Ogre 맵
	void			Event_OgreField_DoorStun(PxTriggerPair* pairs); //Ogre 문과 충돌시 스턴.
	void			Event_OgreStone(PxTriggerPair* pairs);
	void			Event_Dead(PxTriggerPair* pairs);
	void			Event_Save(PxTriggerPair* pairs);
private:
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_LoadMap_Layer(const _tchar*  pLayerTag, const _tchar* pFilePath);
	HRESULT			Ready_LightInfo();

public:
	static CChessField* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<_tchar*>		 m_vecObject;
	vector<_vec3>		 m_vecChessTile;

public:
	void			Set_CrusherStart();

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;

private:
	_uint					m_iKnightCount;
	_uint					m_iKnightCountMax;

	_uint					m_iArbalistCount;
	_uint					m_iArbalistCountMax;

	_uint					m_iSheilderCount;
	_uint					m_iSheilderCountMax;

	CTrigger*				m_pTrigger[TRIGGER_END];

private:
	CLight*					m_pLight[2];
};

#endif // ChessField_h__
