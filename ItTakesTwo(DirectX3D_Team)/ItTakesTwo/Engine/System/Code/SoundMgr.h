#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr final : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum CHANNELID {
		BGM,
		Menu,


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*STATIC*/
		Cody, May,
		Cody_Dash,May_Dash,
		Cody_Jump,May_Jump,
		Cody_Jump_Dash,May_Jump_Dash,
		Cody_Walk,May_Walk,
		Vacuum_Electric,
		Vacuum_Blow0,
		Vacuum_Blow1,
		Vacuum_Blow2,
		Vacuum_Blow3,
		Vacuum_Blow4,
		Vacuum_Blow5,
		Vacuum_Blow6,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*VF1*/
		Vacuum_Fuse_Insert,
		Vacuum_Fuse_Light_On,
		Vacuum_Fuse_Ready,
		Vacuum_Start,
		Vacuum_GrindStone,
		Vacuum_Cogs,
		Vacuum_Cogs2,
		Vacuum_BandSaw,
		Vacuum_CircleSaw,
		Vacuum_CircleSaw_Work,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*VF2*/
		Vacuum_PreOut,
		Vacuum_Out,
		Vacuum_Controll,
		Vacuum_In,
		Vacuum_PowerButton_Active,
		Vacuum_Hub,
		Vacuum_Move,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*VF3*/
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Vaccum_AirFlowMeter,
		Vaccum_BigFan,
		Vaccum_FerrisWheel,
		Vaccum_Generator,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*VB*/
		Vacuum_Stone, Vacuum_Bomb, Vacuum_Mine, Vacuum_Double, Vacuum_Single, Vacuum_Circle, Vacuum_StoneDead,Vacuum_Part,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*CF*/
		Ogre,OgreChain,OgreRunVoice,OgreStone,
		Crusher, CrusherBridge, CrusherHitPoint,
		Monster,Effect,MonsterEffect,

		ToyKnight,ToyArbalist,ToyMage,

		May_Castle, May_CastleWalk, May_CastleDash,May_CastleSmash,
		Cody_Wizard,Cody_WizardWalk, Cody_WizardDash,Cody_WizardIceOrb,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*CB*/
		ChessKing,ChessQueen,ChessKnight,ChessBishop,ChessPawn,ChessRook,

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*SF1*/
		Space_GravityStart,
		Space_FireDoor,
		Space_Ship_Open,
		Scene_Change,

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*SF2*/
		Space_Laser,
		Space_LaserDoor,
		Space_Valve,
		Space_Bounce,
		Space_Cup,
		Space_Panel,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		Space2_ClearScene,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*SF3*/
		Space_LaunchBoard,
		Space_Tube,
		Space_GroundPoundBox,
		Space_GroundPoundBoxReset,
		Space_Spring,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*SB*/
		Space_Missile_Dead,
		Space_Missile_Move,
		Space_Missile_Start,
		Space_Boss_Mirror_Move_End,
		Space_Boss_Mirror_Move,
		Space_Boss_Mirror_Break,
		Space_Boss_Bomb,
		Space_Boss_Laser,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



		MAXCHANNEL
	};

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

public:
	void Initialize();

public:
	void StopNPlaySound(TCHAR * pSoundKey, CHANNELID eID, const _float& fVolum);
	void PlaySoundW( TCHAR * pSoundKey, CHANNELID eID, const _float& fVolum);

	void PlayBGM(TCHAR * pSoundKey, const _float& fVolum);

	void StopSound(CHANNELID eID);
	void StopAll();

	void Release_Sound(RESOURCEID eResourceID);

	void Load_SoundFile(const string tagPath, const RESOURCEID& eResourceID);

	void Set_Volum(const _float& fVolum, CHANNELID eID);

	void Set_CurScene(const RESOURCEID& eID) { m_iCurScene = eID; }

	_bool Is_Play_End(CHANNELID eID);
private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound[RESOURCE_END];
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

	FMOD_CHANNELGROUP* m_pChannelGroup;

	RESOURCEID m_iCurScene = RESOURCE_STATIC;

public:
	virtual void Free() override;


};

END

#endif // SoundMgr_h__