#ifndef __VACUUM_H__

#include "Enermy.h"
#include "ImageUI.h"
#include "VacuumBomb.h"
BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END
// 11 돌멩이 던질때 애니메이션.
// 25 마인 던질때 애니메이션.
// 5 양손으로 내려치기
// 6 한손씩 내려치기
// 19 눈뽑는 애니메이션.
// 20 눈알나옴

//Dead = 17, 13 ,16, 19, 20 , 18

class CVacuum : public CEnermy
{
	enum TriggerID
	{
		Vacuum,
		LeftHand,
		RightHand,
		MayRide,
		CodyRide,
		TriggerID_End,
	};
	enum animID {
		Shed_Awakening_Vacuum_Battle_VacuumBoss,  //0    frame:[2087]
		Shed_Awakening_Vacuum_Meet_VacuumBossHead,//1    frame:[3382]
		Shed_Vacuum_Meet_Sucked_VacuumBossHead,   //2    frame:[2476]
		VacuumBoss_Additive,                      //3    frame:[61]
		VacuumBoss_Attack_Debris,                 //4    frame:[61]
		VacuumBoss_Attack_DoubleSlam,             //5    frame:[61]
		VacuumBoss_Attack_Slam,                   //6    frame:[116]
		VacuumBoss_Bombs_Fire_Left,               //7    frame:[11]
		VacuumBoss_Bombs_Mh,                      //8    frame:[61]
		VacuumBoss_Bombs_Fire_Right,              //9    frame:[11]
		VacuumBoss_Debris_Additive,               //10    frame:[18]
		VacuumBoss_Debris_Fire,                   //11    frame:[31]
		VacuumBoss_End_ArmsOverride,              //12    frame:[31]
		VacuumBoss_End_ArmsOverride_50,           //13    frame:[31]
		VacuumBoss_End_ArmsOverride_50_Left,      //14    frame:[31]
		VacuumBoss_End_ArmsOverride_50_Right,     //15    frame:[31]
		VacuumBoss_End_ArmsOverride_75,           //16    frame:[31]
		VacuumBoss_End_ArmsOverride_Preview,      //17    frame:[31]
		VacuumBoss_End_Death,                     //18    frame:[31]
		VacuumBoss_End_EyePoppedOut,              //19    frame:[28]
		VacuumBoss_End_EyePoppedOut_Enter,        //20    frame:[31]
		VacuumBoss_HitReaction,                   //21    frame:[36]
		VacuumBoss_RefPose,                       //22    frame:[2]
		VacuumBoss_HitReaction_Additive_Max,      //23    frame:[2]
		VacuumBoss_Mh,                            //24    frame:[61]
		VacuumBoss_Minefield,                     //25    frame:[60]
		VacuumBoss_Stunned,                       //26    frame:[16]
		VacuumBoss_Stunned_Enter,                 //27    frame:[151]
		VacuumBossHead_Shed_Vacuum_BossFight_Outro,//28    frame:[251]
		End
	};

	enum SoundID
	{
		Sound_Stone, Sound_Double, Sound_Single, Sound_Mine, Sound_Bomb, Sound_PartUp, Sound_PartDown,
		Sound_End
	};
private:
	explicit CVacuum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuum();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;


	virtual void*	On_Trigger_Active(void* pTrigger,void* pCaller) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);
	void			Stun(const _float& fTimeDelta);
	void			Dead(const _float& fTimeDelta);

public:
	void			May_ride() { m_bMayRide = true; }
	void			Cody_ride() { m_bCodyRide = true; }

public:
	void			All_FrontDoorOpen(_bool bState);
	void			All_BackDoorOpen();

	void			All_FrontDoorCloes();
	void			All_BackDoorCloes();

	void			All_PartUp();
	void			All_PartDown();


private:
	void			PartUp(const _float& fTimeDelta);
	void			PartDown(const _float& fTimeDelta);

public:
	_bool			Get_Bomb() { return m_bBombCheck; }

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	_bool						m_bSound[Sound_End] = { false };
	_uint						m_iSound[Sound_End] = { 0 };

private:
	CTrigger*					m_pTrigger[TriggerID::TriggerID_End];


	_bool						m_bCreateTrigger;
	_bool						m_bStun;
	_bool						m_bStunFirst;

	_bool						m_bCodyRide;
	_bool						m_bMayRide;
	_bool						m_bAniStop;

	_float						m_fDelta;

	_bool						m_bTriggerStart = false;
private:
	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iStoneCount = 0;
	_float						m_fStoneTime = 0.f;



	_uint						m_iBombCount = 0;
	_float						m_fBombTime = 0.f;
	_uint						m_iBombMaxCount = 0;
	_bool						m_bBombCheck = false;

	_uint						m_iTwoHandCircle = 0;
	_uint						m_iTwoHandCircleMax = 0;
	_float						m_fTwoHandCircleTime = 0.f;


	_uint						m_iOneHandCircle = 0;
	_uint						m_iOneHandCircleMax = 0;
	_float						m_fOneHandCircleTime = 0.f;

	_uint						m_iAniIndex = 0;

	_uint						m_iPatturnSelect = 0;


private:
	_uint						m_iMineSelect = 0;
	_uint						m_iMineZ = 8;
	_uint						m_iMineX = 3;

	_float						m_fMineDis = 3.f;

	_float						m_fMineX;
	_float						m_fMineZ;
	_float						m_fMineX2;
	_float						m_fMineZ2;
	_float						m_fMineX3;
	_float						m_fMineZ3;
	_float						m_fMineX4;
	_float						m_fMineZ4;
	_float						m_fMineX5;
	_float						m_fMineZ5;
	_float						m_fMineX6;
	_float						m_fMineZ6;
	_float						m_fMineX7;
	_float						m_fMineZ7;
	_float						m_fMineX8;
	_float						m_fMineZ8;


	_vec3						m_vNextPos; //다음 프레임 마인 위치 저장할 값.
	_vec3						m_vNextPos2;
	_vec3						m_vNextPos3;
	_vec3						m_vNextPos4;
	_vec3						m_vNextPos5;
	_vec3						m_vNextPos6;



	_uint						m_iMineCount = 0;
	_uint						m_iMineMaxCount = 0;


	_float						m_fMineTime = 0.f;
	_float						m_fMineCreateTime = 0.f;

	_bool						m_bTrigger = false;

private:
	_uint						m_iMinePatturnCount = 0;
	_bool						m_bMineSelect = false;
	_bool						m_bPattrunSelect = false;
	_float						m_fPattrunDelta = 0.f;

private:
	int							data[6], i, sub_i;
	_bool						m_bRandomNum = false;

private: //파츠 업 다운용
	_bool						m_bPartUp = false;
	_bool						m_bPartDown = false;

	_float						m_fPartUpDelta = 0.f;
	_float						m_fPartDownDelta = 0.f;

	_uint						m_iPartUpSelect = 0;
	_uint						m_iPartDownSelect = 0;

	_bool						m_bStartDoorOpne = false;
	_bool						m_bStartPartUp = false;

	_bool						m_bPartPatturn = false;
	_bool						m_bDownOne = false; //파츠 다운 함수 한번만실행하기위해서.

private: // UI Funtion
	void						Create_UI(const _tchar* pLayerTag);
	void						Update_UI(const _float& fTimeDelta);
	void						HP_UI(const _float& fTimeDelta);
	void						Effect_Dead(const _float& fTimeDelta);

private: // UI Variable
	CImageUI*					m_pBossName;
	CImageUI*					m_pBossHp;
	CImageUI*					m_pBossHpbg;
	_float						m_fDeadTime = 0.f;
	_float						m_fDeadEffect = 0.f;
	_bool						m_bDeadEffect = false;


public:
	static CVacuum*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;

	void Create_Bomb(void * pArg);
private:
	list<CVacuumBomb*> m_vecActiveBomb;
	list<CVacuumBomb*> m_vecBomb;
};

#define __VACUUM_H__
#endif


