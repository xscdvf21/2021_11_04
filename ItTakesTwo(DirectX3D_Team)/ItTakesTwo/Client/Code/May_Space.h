#ifndef __May_Space_H__
#define __May_Space_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "May_Space_AnimEnum.h"
#include "ControllerCallback.h"
#include "PlayerValue.h"

using namespace SPACE;

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

class CMay_Space : public CPlayerValue, public CControllerCallback
{
public:
	virtual void Set_ActorSetting(PxRigidDynamic* pActor, PxShape* pBody) override;

	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;

	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;
	virtual bool filter(const PxController& a, const PxController& b) override;
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

private:
	explicit CMay_Space(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMay_Space();

	void	Compute_SortSurface(PxVec3& vDir);
	_float m_fPoundStartY = 0.f;
public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;
	virtual void	Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex = 0) override;
	virtual void	Render_Stencil(const _int& iIndex = 0) override;

private:
	void			Key_Input(const _float& fTimeDelta);
	void			Set_State(const _float& fTimeDelta);
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void			Get_KeyDir(_vec3 vDir);
	_bool			Get_Fade() { return m_bFade; }

	void			ChangeZ(const _float & fTimeDelta);
	void			Respawn(const _float& fTimeDelta);
	void			Late_Dead(const _float& fTimeDelta);
	void			Late_Missile_Ride(const _float& fTimeDelta);

public:
	virtual void	Clear_Jump() override;
	void			SpaceField3_Clear_SetPos(const _float& fTimeDelta);
	void			SpaceField2_Clear_SetPos(const _float& fTimeDelta);

private:
	void			Move(const _float& fTimeDelta, _float3* vResultPos, _float3* vResultDir, _float fRotSpeed = 720.0f);
	void			Dash(const _float& fTimeDelta, _float fDashRatio = 2.0f);
	void			Dash(const _float& fTimeDelta, _float fTime, _float fDashRatio);
	_bool			Gravity(const _float& fTimeDelta);
	_bool			JumpTo(_float fJumpPower, _float fTime, _float3 vGoalPos, _float fTimeDelta);
	void			Shoot(const _float& fTimeDelta);
	_bool			MoveTo(const _float& fTimeDelta, _float fGoalTime, _vec3 vGoalPos, _bool bExceptY = false, _bool bSimilar = false, _float fDis = 0.1f);
	_bool			RotTo(const _float& fTimeDelta, _float fGoalTime/*, _vec3 vTargetLook*/, _vec3 vGoalLook, _bool bSimilar = false/*, _float fRad = 0.1f*/);
	_bool			RotTo(_vec3 vGoalLook);
	void			Set_Pos(char* tagBoneName, const _float& fTimeDelta);
	_bool			Ground(const _float& fTimeDelta);
	void			Free_Gravity(const _float& fTimeDelta);
	_bool			Lock();

	void			DefaultPush(const _float& fTimeDelta);
	void			GrapPush(const _float& fTimeDelta);

	void			InteractionMove(_vec3 vDir, const _float& fTimeDelta);
	void			InteractionSetPos(_vec3 vPos, const _float& fTimeDelta);

	//상호작용 함수
	void			Event(const _float& fTimeDelta);

private:
	void			Set_Late_State(const _float& fTimeDelta);
	void			Late_Push_State(const _float& fTimeDelta);
	void			Late_Idle_State(const _float& fTimeDelta);

private:
	void			Idle_State(const _float& fTimeDelta);
	void			Move_State(const _float& fTimeDelta);
	void			Jump_State(const _float& fTimeDelta);
	void			Sit_State(const _float& fTimeDelta);
	void			Run_State(const _float& fTimeDelta);
	void			Dash_State(const _float& fTimeDelta);
	void			Hang_State(const _float& fTimeDelta);
	void			Climbing_State(const _float& fTimeDelta);
	void			Slide_State(const _float& fTimeDelta);
	void			Ride_State(const _float& fTimeDelta);
	void			Dead_State(const _float& fTimeDelta);
	void			Lever_State(const _float& fTimeDelta);
	void			PickUp_State(const _float& fTimeDelta);
	void			HangSwitch_State(const _float & fTimeDelta);
	void			Float_state(const _float& fTimeDelta);
	void			Fall_State(const _float& fTimeDelta);
	void			Update_Time(const _float& fTimeDelta);
	void			Push_State(const _float& fTimeDelta);
	void			Missile_Ride(const _float& fTimeDelta);
	void			Hit_State(const _float& fTimeDelta);

	void			Lever_Slap(const _float& fTimeDelta);
	void			Lever_Grap(const _float& fTimeDelta);

	void			Set_Position(char* tagBoneName);

public:
	void			Insert_Fuse_End();
	_bool			Check_Insert();

private:
	void			Set_Dir(_float3* vDir, _int* iRotDir, _float* fCosTh);
	void			Key_Reset();

public:
	virtual void	Set_Damage(_int iDamage) override;

private:
	void			Ledge(const _float& fTimeDelta);
	void			Hang(const _float& fTimeDelta);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	class CImageUI*			m_pImageUI = nullptr;

	// RigidBody
	_bool					m_bBody = false;
	_uint					m_iPathCount = 0;
	_bool					m_bSlide = false;
	_vec3					m_vMoveDir{};
	_float					m_fDirAngle = 0.0f;
	_vec3					vAccDir = { 0.0f, 0.0f, 0.0f };



public:
	static CMay_Space*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void) override;


public:
	enum LEVERTYPE { LEFT_TO_RIGHT, RIGHT_TO_LEFT };

public:
	void		PlayAnimation(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, const _float& fTimeDelta, CMay_Space::PLAYER_STATE eCurState, CMay_Space::PLAYER_STATE* ePreState);

private:
	void		ChangeAnimation(CMay_Space* pMay, CMay_Space::PLAYER_STATE eNextState, CMay_Space::PLAYER_STATE* eCurState, Engine::CDynamicMesh* pMesh);

private:
	void		PlayIdle(Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* pPreState);
	void		PlayJump(CMay_Space* pMay, Engine::CDynamicMesh* pMesh);
	void		PlayWalk(Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* pPreState);
	void		PlayDash(CMay_Space* pMay, Engine::CDynamicMesh* pMesh);
	void		PlaySit(Engine::CDynamicMesh* pMesh);
	void		PlayRun(Engine::CDynamicMesh* pMesh);
	void		PlayClimbing(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* pPreState);
	void		PlayHang(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* pPreState);
	void		PlayPickUp(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* pPreState);
	void		PlaySlide(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayRide(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayLever(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayHangSwitch(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayFloat(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayFall(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayPush(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState);
	void		PlayAttach();
	void		PlayGravityFly();
	void		PlayLeverSlap();
	void		PlayLeverGrap();
	void		PlayMissile();
	void		PlayHit();

	void		LedgeAnimation();
	void		HangAnimation();

private:
	//Crouch
	void		Crouch_Idle(Engine::CDynamicMesh* pMesh);
	void		Crouch_Walk_Start(Engine::CDynamicMesh* pMesh);
	void		Crouch_Walk(Engine::CDynamicMesh* pMesh);
	void		Crouch_Start(Engine::CDynamicMesh* pMesh);

	//Idle

	//Jump
	void		Jump(Engine::CDynamicMesh* pMesh);
	void		Jump_Fall(Engine::CDynamicMesh* pMesh);
	void		Jump_Dash(CMay_Space* pMay, Engine::CDynamicMesh* pMesh);
	void		SecondJump(Engine::CDynamicMesh* pMesh);
	void		GroundPound_Start(CMay_Space* pMay, Engine::CDynamicMesh* pMesh);
	void		GroundPound_Fall(CMay_Space* pMay, Engine::CDynamicMesh* pMesh);
	void		GroundPound_End(CMay_Space* pMay, Engine::CDynamicMesh* pMesh);
	void		GroundPound_Land(Engine::CDynamicMesh* pMesh);

public:
	void		Set_2ndJump(_bool b2ndJump) { m_b2ndJump = b2ndJump; }
	void		Set_JumpDash() { m_bJumpDash = true; }
	void		Set_CrouchMove(_bool bMove) { m_bCrouchMove = bMove; }
	void		Set_GroundPoundStop() { m_bGroundPoundStop = true; }

	// 벽을 타고있는 상태에서 점프를 하게되면 Cody에서 true로 바꿔준다.
	void		Set_ClimbingJump() { m_bClimbingJump = true; }

	// 벽점프중 벽에 닿으면 Cody에서 true로 바꿔준 후 벽점프 상태에서 아이들로 바꾼후 false로 바꿈
	void		Set_ClimbingJumpEnd() { m_bClimbingJumpEnd = true; }

	// 매달려있을때 스페이스바를 누르면 올라감
	void		Set_Rise() { m_bRise = true; }

	// PickUp상태일때 플레이어가 움직이면 PickUp상태에서 이값을 true로 바꿔줌
	void		Set_PickUpMove(_bool bPickUpMove) { m_bPickUpMove = bPickUpMove; }

	// PickUp상태일때 내려놓을경우 true로 플레이어에서 바꿔준다.
	void		Set_PickUpDrop() { m_bPickUpDrop = true; }

	void		Set_TakeOff() { m_bTakeOff = true; }
	void		Set_Insert_Fuse();

	void		Set_LeverType(LEVERTYPE eLever) { m_eLever = eLever; }
	void		Set_ClearJump();
	void		Set_HangSwitchSuccess() { m_bHangSwitchSuccess = true; }

public:
	May_Space_AnimID				Get_CurAniState() { return m_eMayAnim; }

private:
	May_Space_AnimID				m_eMayAnim;
	May_Space_AnimID				m_ePreMayAnim;

	// Jump
	_bool					m_b2ndJump = false;
	_bool					m_b2ndJumpStart = false;
	_bool					m_bGroundPoundStop = false;

	// Crouch
	_bool					m_bCrouchMove = false;

	// Climbing
	// 벽타기 애니메이션중에 true로 바뀌면 벽점프 애니메이션 재생용

	// 벽점프 중에 벽에 닿았을경우 true로 바꾸고 벽점프아이들 상태로 변경
	_bool					m_bClimbingJumpEnd = false;

	_bool					m_bAnimationStop = false;
	_bool					m_bWallJumpStart = false;

	// Hang
	_bool					m_bRise = false;

	// PickUp
	_bool					m_bPickUpMove = false; // PickUp상태일때 이동유무 판별
	_bool					m_bPickUpDrop = false; // PickUp상태일때 내려놓을경우 true로 플레이어에서 바꿔준다.

	_bool					m_bInsert_Fuse = false;


	// Ride
	LEVERTYPE					m_eLever = RIGHT_TO_LEFT;

	_bool					m_bHangSwitchSuccess = false;
	_int					m_iHp;
	_int					m_iMaxHp;

	_bool					m_bFireDoor = false;


private: // EFFECT Function
	void		Effect(const _float& fTimeDelta);
	void		Effect_Walk(const _float& fTimeDelta);
	void		Effect_Dead(const _float& fTimeDelta);
	void		Effect_Respawn(const _float& fTimedelta);
	void		Effect_Star(const _float& fTimeDelta);
	void		Effect_Hit(const _float& fTimeDelta);

private:
	void		Set_Hit() { m_bHitEffect = true; }

private: // EFFECT Variable
	_float		m_fMoveEffectTime = 0.f;
	_bool		m_bDeadEffect = false;
	_bool		m_bDeadParticleEffect = false;
	_float		m_fDeadEffectTime = 0.f;
	_bool		m_bRespawnEffect = false;
	_float		m_fRespawnEffectTime = 0.f;
	_float		m_fAmount = 0.f;
	_bool		m_bAmount = false;
	_float		m_fStarEffectTime = 0.f;
	_bool		m_bHitEffect = false;

private:// UI FUNCTION
	void		Create_UI(const _tchar* pLayerTag);
	void		Update_UI(const _float& fTimeDelta);
	void		HP_UI(const _float& fTimeDelta);
	void		Sign_UI(const _float& fTimeDelta);
	void		NormalMap_Dead(const _float& fTimeDelta);
	void		BossMap_Dead(const _float& fTimeDelta);
	void		FadeIn(const _float & fTimeDelta);
	void		FadeOut(const _float & fTimeDelta);

public:
	CImageUI*	Get_FadeUI() { return m_pFade; }

private: // UI VARIABLE
	CImageUI*	m_pFade = nullptr;
	CImageUI*	m_pProfile = nullptr;
	CImageUI*	m_pHp = nullptr;
	CImageUI*	m_pDeadUI[6];
	CImageUI*	m_pCodySign = nullptr;

private:
	_float		m_fDeadSign = 128.f;
	_bool		m_bDeadSign = false;
	_bool		m_bDeadUI = false;
	_bool		m_bBossMapDead = false;
	_bool		m_bStencil = true;

public:
	void Set_Up(const _vec3& vDir);
private:
	void Check_GravityPath(const _float& fTimeDelta);
};

#endif // Player_h__