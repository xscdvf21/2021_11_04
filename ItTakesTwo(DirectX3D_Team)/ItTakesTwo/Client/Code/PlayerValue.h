#pragma once
#ifndef __PLAYER_VALUE_H__

class CPlayerValue:public CGameObject
{
public:
	enum PLAYER_STATE {
		PL_IDLE, PL_MOVE, PL_JUMP, PL_SIT, PL_DASH, PL_RUN, PL_CLIMBING, PL_HANG, PL_RIDE, PL_SLIDE, PL_PICKUP, PL_DEAD, PL_LEVER, PL_HANGSWITCH, PL_FLOAT, PL_FALL, PL_PUSH, PL_HANDLE, PL_BUTTON, PL_SAW_SHOOT, PL_SAW_SLIDING, PL_SHOOT, PL_FREEGRAVITY,
		PL_MISSILE, PL_HIT,
	};
	enum PLAYER_JUMP { PL_JUMPSTART, PL_1STJUMP, PL_2NDJUMP, PL_2NDJMPSTART, PL_JUMPDASH, PL_JUMPDASH_START, PL_JUMPDASHSTOP, PL_GROUNDPOUND_START, PL_GRAVITY_START, PL_GROUNDPOUND, PL_GROUNDPOUND_STOP, PL_JUMPEND, };
	enum PLAYER_DASH { PL_1STDASH, PL_2NDDash, PL_DASH_END, };
	enum PICKUP_TYPE { FUSE, HOSE, LEVER, FAN, DUMBBELL, GENERATOR_BATTERY, GENERATOR_LEVER, SPACE_WALL, PICKUPEND };
	enum Camera_Mode { Camera_2D, Camera_3D, };
	enum HANGTYPE { LEDGE, HANG };
	enum KEYDIR {
		DIR_LEFT, DIR_LEFT_UP, DIR_UP, DIR_RIGHT_UP, DIR_RIGHT, DIR_RIGHT_DOWN, DIR_DOWN, DIR_LEFT_DOWN, DIR_STOP
	};
	enum PLAYER_SOUND {	
		PLAY_MOVE, PLAY_MOVE_END,
		PLAY_SIT, PLAY_SIT_END,
		PLAY_DASH, PLAY_DASH_END,
		PLAY_JUMP ,PLAY_JUMP_DASH, PLAY_DOUBLE_JUMP, PLAY_GROUNDPOUND_IMPACT,PLAY_WHOOSH, PLAY_JUMP_END,
		PLAY_END,};

public:
	CPlayerValue(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayerValue();

protected:
	virtual void			Clear_Jump();
	void					Clear_Ride();
	void					Clear_Sit();
	void					Clear_PickUp();
	void					Clear_Lever();
	void					Clear_Float();
	void					Clear_Dead();
	void					Clear_Move();
	void					Clear_Fall();
	void					Clear_Hang();
	void					Clear_Dash();
	void					Clear_Climbing();
	void					Clear_SawShoot();
	void					Clear_SawSliding();
	void					Clear_Missile();

public:
	void					Clear(PLAYER_STATE eState = PL_DEAD);

public:
	PLAYER_STATE	Get_PlayerState() { return m_ePL_STATE; }
	_bool			Get_PickUpJump() { return m_bPickUpJump; }
	_bool			Get_RideEnd() { return m_bRideEnd; }
	_bool			Click_Left() { return (m_iKeyState & MOVE_LEFT) ? true : false; }
	_bool			Click_Right() { return (m_iKeyState & MOVE_RIGHT) ? true : false; }
	_bool			Click_Down() { return (m_iKeyState & MOVE_DOWN) ? true : false; }
	_bool			Click_Up() { return (m_iKeyState & MOVE_UP) ? true : false; }
	_bool			Get_TakeOff() { return m_bTakeOff; }
	_bool			Is_Carring() { return m_bPickUpStart; }
	PICKUP_TYPE		Get_CarringType() { return m_ePickUpType; }
	_bool			Get_Insert() { return m_bInsert; }
	_bool			Get_SlideEnd() { return m_bSlideEnd; }
	_bool			Get_LeverMove() { return m_bLeverMove; }
	_bool			Get_LeverStart() { return m_bLeverStart; }
	_bool			Get_JumpToEnd() { return m_bJumpToEnd; }
	_bool			Get_SwitchReady() { return m_bSwitchReady; }
	_vec3			Get_AttachDir() { return m_vAttachDir; }
	_bool			Get_RideBox() { return m_bRideBox; }
	_bool			Get_PlayerDead() { return m_bPlayerDead; }
	_bool			Get_SceneWait() { return m_bSceneWait; }
	_bool			Get_Ride() { return m_bRide; }
	_bool			Is_GroundFound() { return m_bJump[PL_GROUNDPOUND]; }

public:
	void			Set_Ride(_bool bState, CGameObject* pObj);
	void			Set_Ride(_bool bState);
	void			Set_EventJump(_float fEventJumpPower);
	void			Set_Dash(PLAYER_DASH eDashState, _bool bState = true) { m_bDash[eDashState] = bState; }
	void			Set_Jump(PLAYER_JUMP eJumpState, _bool bState = true) { m_bJump[eJumpState] = bState; }
	void			Set_JumpDashStop();
	void			Set_JumpEnd() { m_bJump[PL_JUMPSTART] = false; }
	void			Set_Climbing(_bool bClimbing) { m_bClimbing = bClimbing; }
	void			Set_Hang() { m_bHang = true; }
	void			Set_Hang(HANGTYPE eType, _bool bHang, _vec3 vGoalPos, _vec3 vNormal);
	void			Set_EndRise() { m_bEndRise = true; }
	void			Set_PickUp(_bool bPickUp, PICKUP_TYPE ePickUpType, void* pArg);
	void			Set_Slide(_bool bHit, const _float & fPower, const _float & fShootDuration, vector<vector<_vec3>*> vecActor, const _float& fSpeed = 1);
	void			Set_PickUpEnd() { m_bPickUpEnd = true; }
	void			Set_Shoot(const _vec3& vDir, const _float& fPower, const _float& fDuration);
	void			Set_RideJumpMax() { m_bRideJumpMax = true; }
	void			Set_RideEnd(_bool bRideEnd) { m_bRideEnd = bRideEnd; }
	void			Set_PlayerDead() { m_bPlayerDead = true; }
	void			Set_Lever(_bool bLever) { m_bLever = bLever; }
	void			Set_Float(_bool bFloat, _bool bUp, void * pArg, _float3 vPos, _float fHeight = 7.f);
	void			Set_ColWing(_bool bColWing) { m_bColWing = bColWing; }
	void			Set_HangSwitch(const _bool& bState, CGameObject* pObj);
	void			Set_RideTag(const char * tagBoneName, const _tchar* tagMesh, const _tchar* tagTrans, _vec3 vRideAngle);
	void			Force_Ride_Off() { m_bForceRideOff = true; }
	void			Set_Success() { m_bSuccess = true; }
	void			Set_FlytHeight(const _float& fValue) { m_fFlyHeight = fValue; }
	void			Set_SwitchReady() { m_bSwitchReady = true; }
	void			Set_SwitchEndPos(_vec3 vPos) { m_vSwitchEndPos = vPos; }
	void			Set_Insert(_bool bInsert) { m_bInsert = bInsert; }
	void			Set_SavePoint(_vec3 vSavePoint) { m_vSavePoint = vSavePoint; }
	void			Set_SavePointY(_float fY) { m_vSavePoint.y = fY; }
	void			Set_Push(_bool bPush, CGameObject* pObj, _vec3 vTargetPos, PICKUP_TYPE eType, _vec3 vPushDir = { 0.0f, 0.0f, 1.0f }, _bool bGrap = false);
	void			Set_Push(_bool bPush);
	void			Set_Attach() { m_bAttach = true; }
	void			Set_Gravitation(_vec3 vGravitation) { m_vGravitation = vGravitation; }
	void			Set_AttachAngle(_float fAngle) { m_fAttachAngle = fAngle; }
	void			Set_Rot(_bool bRot) { m_bRot = bRot; }
	void			Set_Handle(_bool bHandle, wstring tagHandleName = L"");
	void			Set_AttachDir(_vec3 vAttachDir) { m_vAttachDir = vAttachDir; }
	void			Set_FanButton(_bool bButton, wstring tagButton = L"");
	void			Set_Fly(_bool bFly, wstring tagFlyName);
	void			Set_Fly(_bool bFly) { m_bFly = bFly; }
	void			Set_SlidingDir(_vec3 vSlidingDir) { m_vSlidingDir = vSlidingDir; }
	void			Set_SawJumpTrigger(_bool bTrigger) { m_bSawJumpTrigger = bTrigger; }
	void			Set_CheckGround() { m_bCheckGround = true; }
	void			Set_Slap(_bool bSlap) { m_bSlap = bSlap; }
	void			Set_CameraMode(Camera_Mode eMode) { m_eCameraMode = eMode; }
	void			Set_LeverEnd(_bool bEnd) { m_bLeverEnd = bEnd; }
	void			Set_FreeGravity(const _bool& bState = true);
	void			Set_OnPlatform(const _bool& bState = true);
	void			Fix_ZMove(const _float& fDepth);
	void			Set_RideBox(_bool bRide) { m_bRideBox = bRide; }
	void			Set_EventJumpStart(_bool bState) { m_bEventJumpStart = true, m_fJumpTime = 0.0f; }
	void			Set_NextZ(_float fZ) { m_fNextZ = fZ, m_bChangeZ = true; }
	void			Set_SceneWait(_bool bSceneWati) { m_bSceneWait = bSceneWati; }
	void			Set_SaveUp(_vec3 vSaveUp) { m_vSaveUp = vSaveUp; }
	void			Set_ReadySceneChange();
	//void			Set_Lever()

protected:
	// KeyState
	_uint					m_iKeyState = 0;

	// Player_State
	PLAYER_STATE			m_ePL_STATE = PL_IDLE;
	PLAYER_STATE			m_ePre_State = PL_IDLE;
	_float					m_fAnimRatio = 1.0f;

	// Move
	_float					m_fCurMoveTime = 0.0f;
	_float					m_fGoalTime = 0.5f;
	_float					m_fSpeed = 0.0f;
	_float					m_fMaxSpeed = 8.0f;
	_bool					m_bAcc = false;

	// Run
	_float					m_fRunRatio = 1.5f;

	// Dash
	_bool					m_bDash[PL_DASH_END];
	_bool					m_bDashStart = false;
	_vec3					m_vDashDir{};
	_float					m_fDashTime = 0.0f;
	_float					m_fDashDecelTime = 0.0f;

	// Sit
	_float					m_fSitRatio = 0.2f;

	// Jump
	_float					m_fJumpCoolTime = 1.0f;
	_float					m_fJumpCurTime = 0.f;
	_bool					m_bJump[PL_JUMPEND];
	_bool					m_bJumpDashStart = false;
	_bool					m_bJumpDash = false; // 점프대쉬중엔 방향키 이동을 못하게 막는변수
	_float					m_fGravity = -2.0f;
	_float					m_fJumpTime = 0.0f;
	_float					m_fJumpPower = 1.3f;
	_float					m_fHeight = 0.0f;
	_bool					m_bStopGravity = false;
	_float					m_fGravityTime = 0.0f;
	_float					m_fJumpdashTime = 0.0f;

	// Event
	_bool					m_bEveentKey = false; // 이벤트 키를 눌렀을경우 true로 아닐경우 Update의 제일 앞부분에서 false로 바꿔줌 트리거가 Update이전에 호출되기 떄문
	_bool					m_bEveentKey2 = false;

	// Ride
	_bool					m_bRide = false;
	_bool					m_bRideEnd = false;
	_bool					m_bRideStart = false;
	_float					m_fCurRot = 0.0f;
	_float					m_fRideRot = 0.0f;
	_int					m_iRideDir = 0;
	_bool					m_bTakeOff = false; // Hose에서 내려올때 cody에서 True로 바꿔주고 애니메이션을 바꾼후 바로 false로 바꿔준다.
	_bool					m_bRideJumpMax = false; // HoseEnd 애니메이션이 끝나면 그 위치를 가져오기 위해 사용 위치를 바꾼 후 바로 false로 바꾼다.
	_bool					m_bForceRideOff = false;
	char					m_tagBone[MAX_PATH] = "";
	_tchar					m_tagMash[MAX_PATH] = L"";
	_tchar					m_tagTrans[MAX_PATH] = L"";
	_vec3					m_vRideAngle;

	// PickUp
	_bool					m_bPickUpJump = false;
	_bool					m_bPickUp = false; // 트리거와 충돌했을 경우 true 트리거와 충돌하지 않았을경우 false
	CGameObject*			m_pPickUpTarget = nullptr; // 트리거에서 처음 충돌했을때와 떨어졌을때 호출되기 때문에 이벤트키를 눌렀을 경우에만 PickUp을 하기위해 포인터를 가지고있음
	_bool					m_bPickUpEnd = false; // PickUp상태가 끝났을때 true로 바꿔준다. Drop애니메이션이나 PutInSocket 애니메이션이 끝나면 true로 바꿔준 후에 idle 상태로 바꾼후 false로 다시 바꾼다.
	_bool					m_bPickUpStart = false;
	_bool					m_bInsert = false;
	PICKUP_TYPE				m_ePickUpType = PICKUPEND;
	_bool					m_bInsert_Fuse = false;
	_bool					m_bInitInsertFuse = false;

	// Lever
	_bool					m_bLever = false;
	_bool					m_bColLever = false;
	_bool					m_bLeverStart = false;
	_bool					m_bLeverMove = false;
	_bool					m_bLeverRot = false;
	_bool					m_bLeverArriveRot = false;
	_vec3					m_vLeverDir;
	_bool					m_bSlap = true;
	_bool					m_bLeverEnd = false;
	_bool					m_bLeverCom = false;
	_bool					m_bLeverTargetStart = false;

	// Slide
	vector<vector<_vec3> *> m_vecSlidePos;
	_int					m_iSlideIndex = 0;
	_bool					m_bSlideStart = true;
	_bool					m_bSlideHit = false;
	_bool					m_bSlideTelePort = false;
	_bool					m_bPhys = true;
	_float					m_fSlideTelePortTime = 0.5f;
	size_t					m_iInterval = 0;
	_vec3					m_vShootStartPos;
	_vec3					m_vShootX;

	_bool					m_bShoot = false;
	_vec3					m_vShootDir = { 0.f,0.f,0.f };
	_float					m_fShootDuration = 0.f;
	_float					m_fShootTime = 0.f;
	_bool					m_bRideGround = false;
	_bool					m_bSlideEnd = false;
	_float					m_fShootPower = 0.f;
	_float					m_fSlideSpeed = 1.f;
	_float					m_fShootDist = 0.f;
	_float					m_fShootAngle = 0.f;
	_float					m_fShootSpeed = 0.f;

	// PowerSwitch			
	_bool					m_bJumpToEnd = false;
	_bool					m_bHangSwitch = false;
	_vec3					m_vGoalPos = { 0.f,0.f,0.f };
	_bool					m_bSuccess = false;
	_bool					m_bSwitchReady = false;
	_vec3					m_vSwitchEndPos;

	// Float
	_bool					m_bFloat = false;
	_bool					m_bFloatUp = false;
	_bool					m_bFloatStart = false;
	_vec3					m_vFanPos;
	_float					m_fFloatCurDis = 0.0f;
	_float					m_fFloatHeight = 0.0f;
	_float					m_fOrgHeight = 1000.0f;
	_bool					m_bColWing = false;
	_float					m_fFlyHeight = 9.f;

	// Climbing
	_bool					m_bClimbing = false;
	_bool					m_bClimbingJump = false;
	_vec3					m_vClimbingDir;
	_float					m_fClimbingJumpPower = 2.0f;
	_float					m_fClimbingGravity = -3.0f;
	_bool					m_bClimbingStart = false;
	_bool					m_bClimbingFall = false;
	const PxControllerShapeHit*	m_hitShape{};

	// Dead
	_bool					m_bPlayerDead = false;
	_bool					m_bDeadStart = false;
	_float					m_fCurDeadTime = 0.0f;
	_float					m_fGoalDeadTime = 2.0f;
	_vec3					m_vSavePoint;
	_vec3					m_vDeadSlide = { 0.0f, 0.0f, 0.0f };

	// Hang
	_bool					m_bHang = false;
	_bool					m_bEndRise = false;
	HANGTYPE				m_eHangType = LEDGE;
	_vec3					m_vHangGoalPos = {};
	_vec3					m_vHangNormal = {};
	_bool					m_bHangDoor = false;
	_bool					m_bHangDown = false;
	_float					m_fHangMovoe = 0.0f;
	_bool					m_bHangStart = false;

	// Ground
	_float					m_fGroundTime = 0.0f;
	_bool					m_bGround = false; // 땅이랑 충돌했는지

	// Fall
	_bool					m_bFallStart = false;

	// MoveTo
	_bool					m_bMoveToStart = false;
	_float					m_fMoveToTime = 0.0f;
	_vec3					m_vMoveToDir;

	// RotTo
	_bool					m_bRotToStart = false;
	_float					m_fRotToAngle = 0.0f;
	_float					m_fRotToTime = 0.0f;

	// JumpTo
	_vec3					m_vJumpToGoalDir;
	_vec3					m_vRememberJumpPos;
	_float					m_fJumpToGoalTime = 0.0f;
	_bool					m_bJumpTo = false;
	_float					m_fCurJumpToTime = 0.0f;
	_vec3					m_vStartPos;
	_float					m_fTargetHeight = 0.0f;
	_float					m_fJumpToGravity = 0.0f;

	// Etc
	_bool					m_bFix = false; // ~키를 눌렀을때 플레이어가 안움직이게
	_vec3					m_vTargetPos{};
	_bool					m_bMoveTo = false;
	_bool					m_bRotTo = false;
	_float					m_fTimeDelta = 0.0f;
	_bool					m_bTest = false;

	// Push
	_bool					m_bPush = false;
	_vec3					m_vGravitation{};
	_vec3					m_vPushDIr = { 0.0f, 0.0f, 1.0f };
	_bool					m_bGrap = false;
	_bool					m_bPushStart = false;
	KEYDIR					m_eKEYDIR;
	_bool					m_bRideBox = false;

	// Attach
	_bool					m_bAttach = false;
	CGameObject*			m_pAttachTarget = nullptr;
	_float					m_fAttachAngle = 0.0f;
	_bool					m_bRot = false;
	_vec3					m_vAttachDir{};
	_bool					m_bCheckGround = false;

	// RideObj
	_bool					m_bRideObj = false;

	// Handle
	_bool					m_bHandle = false;
	_bool					m_bHandleStart = false;
	_bool					m_bHandleRot = false;
	_vec3					m_vTargetDir{};
	_vec3					m_vPreBoneDir{};
	_bool					m_bHandleRotStart = false;
	wstring					m_tagHandleName = L"";

	// BoneIntoPos
	_bool					m_bBoneIntoPosition = false;
	_float					m_fCurBIPTime = 0.0f;
	_vec3					m_vBIPGoalPos{};

	// Button
	_bool					m_bButtonEnd = false;
	_bool					m_bFanButton = false;
	wstring					m_tagButton = L"";

	// Shape
	_float					m_fShapeRadius = 0.0f;
	_float					m_fShapeHeight = 0.0f;
	_float					m_fHalfHeight = 0.0f;

	// Fly
	_bool					m_bFly = false;
	_bool					m_bFlyStart = false;
	_vec3					m_vFlyPos{};
	_bool					m_bFlyMove = false;
	_float					m_fFlyTime = 0.0f;
	_bool					m_bLanch = false;

	//Sliding
	_bool					m_bSlidingStart = false;
	_vec3					m_vSlidingDir = { 0.f,0.f,0.f };
	_bool					m_bSawJumpStart = false;
	_bool					m_bSawJumpTrigger = false;
	_vec3					m_vSawJumpStartPos = { 0.0f, 0.0f, 0.0f };
	_float					m_fSawPosZ = 0.0f;

	// Shoot
	_bool					m_bShootStart = false;
	_bool					m_bShootState = false;

	//Tube
	_bool					m_b = false;

	// Animation
	_bool					m_bPlayAnmation = true;

	// Mode
	Camera_Mode				m_eCameraMode = Camera_3D;
	_bool					m_bFreeGravity = false;


	// EventJump
	_bool					m_bEventJump = false;
	_float					m_fEventJumpPower = 0.0f;
	_bool					m_bEventJumpStart = false;
	_bool					m_bOnPlatform = false;

	// FreeGravity
	_uint					m_iFreeGravity = 0;
	_float					m_fZ = 0.0f;
	_bool					m_bChangeZ = false;
	_float					m_fNextZ = 0.0f;
	_bool					m_bStartChange = false;
	_float					m_fChangeZ = 0.0f;
	_float					m_fChangeTime = 0.0f;

	// Respwan
	_vec3					m_vSaveUp = {};
	_bool					m_bSceneWait = false;
	_bool					m_bRespawn = false;
	_bool					m_bKeyOff = false;
	_bool					m_bRenderOff = false;
	_float					m_fFade = 1.f;
	_bool					m_bFade = false;
	_bool					m_bFadeIn = false;
	_bool					m_bFadeOut = false;

	//VB
	_bool					m_bBossRide = false;
	_bool					m_bHitAble = false;
	_bool					m_fHitTime = 0.0f;
	_bool					m_bHitEnd = false;
	_float					m_fOpTime = 0.0f;
	_bool					m_bOp = false;
	_float					m_fRecoverTime = 0.0f;
	_float					m_fOpMaxTime = 3.0f;
	_float					m_fRecoverMaxTime = 10.0f;

	// Sound
	_bool					m_bPlaySound[PLAY_END];
	_float					m_fWalkSoundTime = 0.0f;
	_float					m_fEffectvolume = 0.05f;
	_bool					m_bDumbbell = true;

private:
	_bool m_bFixDepth = false;
	_float m_fDepth = 0.f;
};

#define __PLAYER_VALUE_H__
#endif