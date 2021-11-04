#ifndef __SPACEBABOON_H__

#include "Enermy.h"
#include "BaboonLaser.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

enum BaboonPatturnID
{
	Patturn_Laser,
	Patturn_Missile,
	Patturn_HitReaction,
	Patturn_Bomb,
	Patturn_MoveUp,
	Patturn_Move,
	Patturn_BackMove,
	Patturn_End
};

class CSpaceBaboon : public CEnermy
{
	enum animID {
		FlyingSaucer_Ref,                         //0	frame:[121]
		FlyingSaucer_Ufo_Back,                    //1	frame:[91]
		FlyingSaucer_Ufo_CodyHolding,             //2	frame:[161]
		FlyingSaucer_Ufo_CodyHolding_Enter,       //3	frame:[44]
		FlyingSaucer_Ufo_CodyHolding_Low,         //4	frame:[161]
		FlyingSaucer_Ufo_Controllable_Additive,   //5	frame:[121]
		FlyingSaucer_Ufo_Controllable_Additive_Boost,//6	frame:[121]
		FlyingSaucer_Ufo_Controllable_Additive_Flying,//7	frame:[121]
		FlyingSaucer_Ufo_Controllable_Pose_Bck,   //8	frame:[121]
		FlyingSaucer_Ufo_Controllable_Pose_Fwd,   //9	frame:[121]
		FlyingSaucer_Ufo_Controllable_Pose_Left,  //10	frame:[121]
		FlyingSaucer_Ufo_Controllable_Pose_Right, //11	frame:[121]
		FlyingSaucer_Ufo_FireRocket_Additive_Left,//12	frame:[61]
		FlyingSaucer_Ufo_FireRocket_Additive_Right,//13	frame:[61]
		FlyingSaucer_Ufo_Fwd,                     //14	frame:[91]
		FlyingSaucer_Ufo_GroundPound,             //15	frame:[91]
		FlyingSaucer_Ufo_HitReaction_Bck,         //16	frame:[66]
		FlyingSaucer_Ufo_HitReaction_Fwd,         //17	frame:[56]
		FlyingSaucer_Ufo_HitReaction_Left,        //18	frame:[66]
		FlyingSaucer_Ufo_HitReaction_Right,       //19	frame:[66]
		FlyingSaucer_Ufo_KnockDownMh,             //20	frame:[241]
		FlyingSaucer_Ufo_Laser_HitPod,            //21	frame:[51]
		FlyingSaucer_Ufo_Laser_Mh,                //22	frame:[121]
		FlyingSaucer_Ufo_LaserRippedOff,          //23	frame:[231]
		FlyingSaucer_Ufo_Left,                    //24	frame:[91]
		FlyingSaucer_Ufo_Mh,                      //25	frame:[121]
		FlyingSaucer_Ufo_Right,                   //26	frame:[91]
		FlyingSaucer_Ufo_RocketKnockDown_Mh,      //27	frame:[2]
		PlayRoom_SpaceStation_BossFight_Eject_FlyingSaucer,//28	frame:[1086]
		PlayRoom_SpaceStation_BossFight_EnterUFO_FlyingSaucer,//29	frame:[178]
		PlayRoom_SpaceStation_BossFight_Intro_FlyingSaucer,//30	frame:[2421]
		PlayRoom_SpaceStation_BossFight_LaserRippedOff_FlyingSaucer,//31	frame:[231]
		PlayRoom_SpaceStation_BossFight_Outro_FlyingSaucer,//32	frame:[1889]
		PlayRoom_SpaceStation_BossFight_PowerCoresDestroyed_FlyingSaucer,//33	frame:[143]
		PlayRoom_SpaceStation_BossFight_RocketsPhaseFinished_FlyingSauce,//34	frame:[196]
		End
	};

	enum SoundID
	{
		Sound_BGM,
		Sound_End
	};



private:
	explicit CSpaceBaboon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceBaboon();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public:
	void			Set_NextPatturn(_bool bState) { m_bNextPatturn = bState; }
	void			Set_NextPatturn2(_bool bState) { m_bNextPatturn2 = bState; }
	void			Set_Patturn(BaboonPatturnID _eType) { m_bPatturn[_eType] = true; }
	void			Set_TargetCody() { m_pTargetCody = nullptr; }
	void			Set_TargetMay() { m_pTargetMay = nullptr; }

	void			TargetLook(const _float& fTimeDelta, CGameObject* pTarget);
private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void			Move(const _float& fTimeDelta);
	void			PhysicsCreate();


	void			LaserTargetLook(const _float& fTimeDelta, CGameObject* pTarget);
	void			PatturnMissile(const _float& fTimeDelta);
	void			PatturnLaser(const _float& fTimeDelta, CGameObject* pTarget);
	void			PatturnBomb(const _float& fTimeDelta);
	void			PatturnMoveUp(const _float& fTimeDelta);
	void			PatturnMove(const _float& fTimeDelta); //미사일 쏘고나서 빙글빙글 돔.
	void			PatturnBackMove(const _float& fTimeDelta); //원래는 코디가 던지면서 이동하는것.
	void			PatturnHitReaction(const _float& fTimeDelta);


private:
	void			CreateLaser(const _tchar* pLayerTag);
	void			Effect_Laser(const _float& fTimeDelta);

public:
	void			Set_Damage(_int iDamage) override;
private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private: //Ray
	_float		m_fDistance = 0.f;
	_float		m_fMaxDistance = 200.f;
	_vec3		m_vUltHitPos{};
	_bool		m_pLaserEffect = false;

	_float		m_fTimeDelta = 0.f;
	_int		m_iCount = 0;
	_float		m_fAmount = 0.f;

	_bool		m_bHit = false;

	_uint		m_iRayCol = 0;

private:
	_bool						m_bSound[Sound_End] = { false };
	_bool						m_bPatturn[Patturn_End] = { false };
private:
	_bool						m_bMove;
	_bool						m_bGroundOne;
	_bool						m_bGroundTwo;
	_bool						m_bGroundThree;
	_bool						m_bLaserStart;
	_bool						m_bMoveUpDir;

	_uint						m_iAniIndex;
	_uint						m_iPatturn;
	_uint						m_iTargetSelect;
	_uint						m_iBombCount;

	_float						m_fAngle;
	_float						m_fTurnSpeed;
	_float						m_fPatturnTime;
	_float						m_fMoveUpDir;

	_vec3						m_vLaserStartPos;
	_vec3						m_vMoveUpStartPos;

	_float						m_fMissileTime;
	_float						m_fCodyMissileTime = 0.f;
	_float						m_fMayMissileTime = 0.f;
	_float						m_fLaserTime;
private: //테스트용
	_bool						m_bUltEffect = false;
	_bool						m_bNextPatturn = false;
	_bool						m_bNextPatturn2 = false;
	_bool						m_bTest = false;

	CGameObject*				m_pTargetMay = nullptr;
	CGameObject*				m_pTargetCody = nullptr;
	CBaboonLaser*				m_pLaser = nullptr;

public:
	static CSpaceBaboon*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void						Free(void)		override;

private:
	void					Effect(const _float& fTimeDelta);
	void					Effect_LaserHit(const _float& fTimeDelta);
	void					Effect_Idle(const _float& fTimeDelta);
	void					Effect_Dead(const _float& fTimeDelta);

private: // UI Funtion
	void					Create_UI(const _tchar* pLayerTag);
	void					Update_UI(const _float& fTimeDelta);
	void					HP_UI(const _float& fTimeDelta);

private: // UI Variable
	CImageUI*				m_pBossName;
	CImageUI*				m_pBossHp;
	CImageUI*				m_pBossHpbg;
	_float					m_fDeadTime = 0.f;
	_float					m_fDeadEffect = 0.f;

};

#define __SPACEBABOON_H__
#endif

