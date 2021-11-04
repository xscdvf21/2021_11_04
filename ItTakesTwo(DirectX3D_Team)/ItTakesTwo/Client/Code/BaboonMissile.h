#ifndef __BABOONMISSILE_H__

#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

//Bool값 이용해서 떨어지기전에는 몬스터 불렛에 담고, True일땐 플레이어 불렛에 담으면 될듯?
class CBaboonMissile : public CPlayer
{
	enum ThrustColor
	{
		Boss,
		Cody,
		May
	};
	enum SoundID
	{
		Sound_Effect,
		Sound_End,
	};

private:
	explicit CBaboonMissile(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaboonMissile();

	_bool m_bChaseStart = true;
public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, CGameObject* pPlayer = nullptr, _bool bTarget = false, _uint iIndex =0);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

	void			Effect_Thrust(const _float & fTimeDelta, const _int& iIndex);
public:
	_bool			Get_Att() { return m_bAtt; }
	_bool			Get_Ride() { return m_bRide; }
	_matrix			Get_WorldMatrix() { return *m_pTransformCom->Get_WorldMatrix(); }
	_vec3			Get_Angle() { return m_pTransformCom->Get_Angle(); }
private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void			Set_Att(_bool bAtt);
	void			Set_Ride(_bool bRide) { m_bRide = bRide; }
	void			Set_BaboonDead();

	_bool			Get_CodyMissile() { return m_bTarget; }
private :
	void			Move(const _float& fTimeDelta);
	void			Chase_Move(const _float& fTimeDelta);
	void			Down_Move(const _float& fTimeDelta);
	void			Att_Move(const _float& fTimeDelta); //플레이어가 조종하는 ?
private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;


	CGameObject*				m_pTarget = nullptr;
private:
	_bool						m_bSound[Sound_End] = { false };
	_float						m_fSpeed;

private:
	_bool						m_bChaseMove;
	_bool						m_bDownMove;
	_bool						m_bChange; //처음에는, 플레이어와 충돌. 나중엔 몬스터와 충돌하기위해.
	_bool						m_bAtt;
	_bool						m_bRide = false;
	_bool						m_bTarget;
	_bool						m_bPlayerAtt;
	_int						m_iEffectOn = 3;

	_float						m_fChangeTime; 
	_float						m_fChaseTime;

	_vec3						m_vTargetPos;

	_float						m_fAngle;
	_float						m_fTurnSpeed;
	_float						m_fLifeTime;

	_uint						m_iIndex;

	_float						m_fDeadTime = 30.f;

	_bool						m_bStartMissile;
	_bool						m_bCheck = false;
	CTrigger*					m_pTrigger = nullptr;
	CTrigger*					m_pTrigger2 = nullptr;

	_bool						m_bBaboonDead = false;
	
public:
	static CBaboonMissile*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, CGameObject* pPlayer = nullptr, _bool bTarget = false, _uint iIndex = 0);
	virtual void						Free(void)			override;


};

#define __BABOONMISSILE_H__
#endif

