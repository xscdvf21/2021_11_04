#ifndef __MAGE_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CMage : public CEnermy
{

	enum animID {
		Idle,                                     //0	frame:[45]
		Attack_Start,                             //1	frame:[45]
		Attack_Idle,                              //2	frame:[34]
		Attack_1,                                 //3	frame:[51]
		Attack_2,                                 //4	frame:[41]
		Attack_3,                                 //5	frame:[51]
		Attack_4,                                 //6	frame:[33]
		Attack_5,                                 //7	frame:[33]
		Dead,                                     //8	frame:[21]
		Teleport_Start,                           //9	frame:[16]
		Teleport_End,                             //10	frame:[26]
		Damage,                                   //11	frame:[16]
		End
	};

public:
	explicit CMage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMage();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr,void* pOBJINFO = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;



private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Attack(const _float& fTimeDelta);
	void			MonsterCreate(const _float& fTimeDelta);

public:
	void			Set_Damage(_int iDamage) override;

private:
	void			Hit_State(const _float& fTimeDelta);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	class CStaff*				m_pStaff = nullptr;

	_float						m_fTurnSpeed = D3DXToRadian(4); //턴 스피드
	_float						m_fAngle;	 //플레이어와의 각도

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장

	_bool						m_bAttackStart = false; //어택 스타트를 한번만 하기위해
	_float						m_fAttackTime = 0.f; //어택 딜레이 주기 위해.

private :
	_bool						m_bCreateOne;
	_bool						m_bCreateTwo;
	_bool						m_bCreateThree;

	_uint						m_iKnightCount;
	_uint						m_iArbalistCount;

	_bool						m_bMoveOne;
	_bool						m_bMoveTwo;
	_bool						m_bMoveThree;
	_bool						m_bMoveFour;

	_bool						m_bMoveStart;
	_bool						m_bAniStop;

	_float						m_bMoveTime;

	_bool						m_bAttack;
	_uint						m_iBulltCount;
	_uint						m_iAniIndex;
	_bool						m_bBulletCreate;


//private :
//	_bool						m_bTel
public:
	static CMage*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual void Free(void)		override;
};

#define __MAGE_H__
#endif


