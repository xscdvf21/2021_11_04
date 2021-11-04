#ifndef __CODY_WIZARD_H__
#define __CODY_WIZARD_H__

#include "Player.h"
#include "CodyWizard_Enum.h"
#include "ImageUI.h"
#include "Cone.h"
#include "Orb.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

class CCodyWizard : public CPlayer
{
public:
public:
	enum PLAYER_STATE {	PL_IDLE, PL_MOVE, PL_ATT, PL_DASH, PL_HIT, PL_DEAD };
	enum PLAYER_ATT_STATE { ATT_BASIC, ATT_ULTIMATE, ATT_ORB, ATT_END };

private:
	explicit CCodyWizard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCodyWizard();

	void Check_Ray(const _float& fTimeDelta);
	_float m_fDistance = 0.f;
	_float m_fMaxDistance = 30.f;
public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex =0) override;
	virtual void	Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex = 0) override;
	virtual void	Render_Stencil(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	void			Key_Input(const _float& fTimeDelta);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	void			Set_State(const _float& fTimeDelta);

private: // LIGHT
	void			Ready_Light();
	void			Update_Light(const _float& fTimeDelta);
	void			BasicShoot_Light(_float fSpeed);

public:
	void			Set_OrbAtt(_bool orb) { m_bOrbAttLight = orb; }

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
	void			OrbAtt(const _float& fTimeDelta);

public:
	void			Set_AttEnd() { m_bAttEnd = true; }
	void			Set_DashEnd() { m_bDashEnd = true; }
public:
	_bool			Get_BlinkEnd() { return m_bBlinkAnimation; }

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
	void			Create_Cone(const _tchar* pLayerTag);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

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

	// Att
	_bool					m_bAttStart = false;
	_bool					m_bAttEnd = false;
	_float					m_fAttLimitTime = 0.2f;
	_float					m_fAttTime = 0.0f;

	// Dash
	_bool					m_bDashEnd = false; // Blink 애니메이션이 끝나면 대쉬 종료
	_bool					m_bBlinkAnimation = false; // true가 된 시점에서 Blink 애니메이션 실행
	_bool					m_bBlinkRender = false; // Blink중일때 랜더를 끈다.
	_float					m_fDashRatio = 0.5f; // 대쉬 속도를 조절하는 값
	_float					m_fDashGoalDis = 3.5f; // 대쉬로 이동할 총 길이
	_float					m_fDashDis = 0.0f; // 매프레임마다 이동한 거리를 누적한 길이

	// Physics
	_bool					m_bMovePysics = true;
	_bool					m_bUltimate = false;
	_bool					m_bUltimateStart = true;
	CTrigger*				m_pAttackTrigger = nullptr;
	_float					m_fLastDist = 1.0f; // 매프레임마다 이동한 거리를 누적한 길이
	_bool					m_bDeadStart = false;

	COrb*					m_pLOrb = nullptr;
	COrb*					m_pROrb = nullptr;

	// Light
	CLight*					m_pCodyLight = nullptr;
	CLight*					m_pCodyLight2 = nullptr;
	_bool					m_bAttLight = false;
	_float					m_fAttLight = 0.f;
	_vec3					m_vRightOrbPos;
	_vec3					m_vLeftOrbPos;
	_float					m_fLightSpeed = 30.f;
	_bool					m_bOrbAttLight = false;

private: // EFFECT FUNCTION
	void		Effect(const _float& fTimeDelta);
	void		Effect_Orb(const _float& fTimeDelta);
	void		Effect_DefaultAttack(const _float& fTimeDelta);
	void		Effect_Dash(const _float& fTimeDelta);
	void		Effect_DefaultSkill(const _float& fTimeDelta);
	void		Effect_UltimateSkill(const _float& fTimeDelta);
	void		Effect_UltHitEffect(const _float& fTimeDelta);
	void		Effect_Walk(const _float& fTimeDelta);
	void		Effect_Dead(const _float& fTimeDelta);
	void		Effect_Respawn(const _float& fTimedelta);
	
public: // EFFECT SET, GET
	void		Set_DefaultAttackEffect(_int iAtt) { m_iDefaultAttack = iAtt; }
	void		Set_DashEffect(_int iAtt) { m_iDash = iAtt; }
	void		Set_DashEffect() { m_bDashEffect = true; }
	void		Set_Stencil(_bool bState) { m_bStencil = bState ; }
private: // EFFECT VARIABLE
	_int		m_iDefaultAttack = 0;
	_int		m_iDash = 0;
	_bool		m_bDashEffect = false;
	_bool		m_bUltEffect = false;
	_vec3		m_vUltHitPos{};
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
	CImageUI*	m_pKey[5];
	CImageUI*	m_pDeadUI[6];
	_float		m_fDeadSign = 128.f;
	_bool		m_bDeadSign = false;
	_bool		m_bDeadUI = false;
	_bool		m_bMapDead = false;

private : //CoolTime
	void			CoolTime(const _float& fTimeDelta);

public :
	void			CreateIceOrbLB(_float fLifeDir = 8.f);
	void			CreateIceOrbRB();
	void			Set_IceCoolTime();
	void			Set_DashCoolTime();

private:
	_bool			m_bDashCoolTime = false;
	_bool			m_bIceCoolTime = false;
	_float			m_fDashCoolTime = 0.f;
	_float			m_fIceOrbCoolTime = 0.f;

	_float			m_fDashCool = 0.f;
	_float			m_fIceOrbCool = 0.f;

	_uint			m_iIceOrbCount = 0;
	CCone*			m_pCone = nullptr;
	CCone*			m_pCone2 = nullptr;


public:
	static CCodyWizard*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void) override;
};

#endif // Player_h__