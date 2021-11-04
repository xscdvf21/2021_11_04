#ifndef __MAY_CASTLE_H__
#define __MAY_CASTLE_H__

#include "Player.h"
#include "MayCastle_Enum.h"
#include "ImageUI.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

class CMayCastle : public CPlayer
{
public:
	enum PLAYER_STATE {	PL_IDLE, PL_MOVE, PL_ATT, PL_DASH, PL_HIT, PL_DEAD };
	enum PLAYER_ATT_STATE { ATT_BASIC, ATT_ULTIMATE, ATT_SMASH, ATT_END };

private:
	explicit CMayCastle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMayCastle();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex =0) override;
	virtual void	Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex = 0) override;
	virtual void	Render_Stencil(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	void			Key_Input(const _float& fTimeDelta);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	void			Set_State(const _float& fTimeDelta);

public:
	class CToySword*		Get_Sword() { return m_pSword; }

private:
	void			Move(const _float& fTimeDelta, _float3* vResultPos, _float3* vResultDir, _float fRotSpeed = 720.0f);

private:
	void			Idle_State(const _float& fTimeDelta);
	void			Move_State(const _float& fTimeDelta);
	void			Att_State(const _float& fTimeDelta);
	void			Dash_State(const _float& fTimeDelta);
	void			Hit_State(const _float& fTimeDelta);
	void			Dead_State(const _float& fTimeDelta);

private:
	void			Recover(const _float& fTimeDelta);

private:
	//Att
	void			BasicAtt(const _float& fTimeDelta);
	void			Ultimate(const _float& fTimeDelta);
	void			Smash(const _float& fTimeDelta);

public:
	void			Set_AttEnd() { m_bAttEnd = true; }
	void			Set_DashEnd() { m_bDashEnd = true; }
	void			Set_Hit(HITTYPE eType) override;
	void			Set_SmashAtt() { m_bSmashAtt = true; }
	void			Set_WindAtt() { m_bWindAtt = true; }

private:
	void			Set_Dir(_float3* vDir, _int* iRotDir, _float* fCosTh);
	void			Key_Reset();

private:
	void			Clear(PLAYER_STATE eState);

private:
	void			Clear_Idle();
	void			Clear_Move();
	void			Clear_Att();
	void			Clear_Dash();
	void			Clear_Hit();
	void			Clear_Dead();

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	class CToySword*		m_pSword = nullptr;

private:
	_bool					m_bDashCoolTime = false;
	_bool					m_bSmashCoolTime = false;
	_float					m_fDashCoolTime = 0.f;
	_float					m_fSmashCoolTime = 0.f;

	_float					m_fDashCool = 0.f;
	_float					m_fSmashCool = 0.f;


private:
	// State
	_uint					m_iKeyState = 0;
	_uint					m_iAttState = 0;

	PLAYER_STATE			m_ePL_STATE = PL_IDLE;
	PLAYER_STATE			m_ePre_State = PL_IDLE;

	PLAYER_ATT_STATE		m_eAtt_State = ATT_END;

	// Move
	_float					m_fCurMoveTime = 0.0f;
	_float					m_fGoalTime = 0.5f;
	_float					m_fSpeed = 0.0f;
	_float					m_fMaxSpeed = 40.0f;

	_float3					m_vMoveDir;
	_float					m_fDirAngle = 0.0f;

	// Att
	_bool					m_bAttStart = false;
	_bool					m_bAttEnd = false;
	_float					m_fAttLimitTime = 0.2f;

	_float					m_fAttTime = 0.0f;

	// Dash
	_bool					m_bDashStart = false;
	_bool					m_bDashEnd = false;
	_float					m_fDashRatio = 4.0f;

	// Hit

	_bool					m_bDeadStart = false;

private: // EFFECT FUNCTION
	void		Effect(const _float& fTimeDelta);
	void		Effect_Sword(const _float& fTimeDelta);
	void		Effect_DefaultAttack(const _float& fTimeDelta);
	void		Effect_Dash(const _float& fTimeDelta);
	void		Effect_DefaultSkill(const _float& fTimeDelta);
	void		Effect_UltimateSkill(const _float& fTimeDelta);
	void		Effect_Walk(const _float& fTimeDelta);
	void		Effect_Dead(const _float& fTimeDelta);
	void		Effect_Respawn(const _float& fTimedelta);

public: // EFFECT GET, SET
	void		Set_SmashEffect(_int smash) { m_iSkillEffect = smash; } // 1. start, 2.end
	void		Set_Stencil(_bool bState) { m_bStencil = bState; }
private: // EFFECT VARIABLE
	_bool		m_bDashEffect = false;
	_int		m_iSkillEffect = 0;
	_bool		m_bUltEffect = false;
	_float		m_fUltTime = 0.f;
	_float		m_fUltAngle = 0.f;
	_float		m_fMoveEffectTime = 0.f;
	_bool		m_bDeadEffect = false;
	_bool		m_bDeadParticleEffect = false;
	_float		m_fDeadEffectTime = 0.f;
	_bool		m_bRespawnEffect = false;
	_float		m_fRespawnEffectTime = 0.f;
	_float		m_fAmount = 0.f;
	_bool		m_bAmount = false;
	_bool		m_bStencil = true;

private: // UI FUNCTION
	void		Create_UI(const _tchar* pLayerTag);
	void		Update_UI(const _float& fTimeDelta);
	void		HP_UI(const _float& fTimeDelta);
	void		Skill_UI(const _float& fTimeDelta);
	void		Dead_UI(const _float& fTimeDelta);
	void		KeyEvent_UI(CImageUI* pUI);


private: // UI VARIABLE
	CImageUI*	m_pProfile = nullptr;
	CImageUI*	m_pHp = nullptr;
	CImageUI*	m_pCharge = nullptr;
	CImageUI*	m_pChargeframe = nullptr;
	CImageUI*	m_pSkill[4];
	CImageUI*	m_pSkillBox[8];
	CImageUI*	m_pKey[8];
	CImageUI*	m_pDeadUI[6];
	_float		m_fDeadSign = 128.f;
	_bool		m_bDeadSign = false;
	_bool		m_bDeadUI = false;
	_bool		m_bMapDead = false;

private:
	_float		m_fKeyEventTime = 0;
	_bool		m_bKeyEvent = false;
	_bool		m_bSmashAtt = false;
	_bool		m_bWindAtt = false;
private:
	//CoolTime
	void			CoolTime(const _float& fTimeDelta);


public:
	void			Set_SmashCoolTime();
	void			Set_DashCoolTime();
	
	void			SmashAtt();
	void			WindAtt();

private :
	_float			m_fSmashRadius; //스매쉬 반경
	_float			m_fWindDamageTime; //휠윈드 공격데미지 딜레이

	_int			m_iSmashDamage;
	_int			m_iWindDamage;

public:
	static CMayCastle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void) override;
};

#endif // Player_h__