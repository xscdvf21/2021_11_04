#ifndef __KIGHT_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CKnight : public CEnermy
{
	enum animID {
		ToyKnight_Attack1,                        //0	frame:[18]
		ToyKnight_Attack2,                        //1	frame:[18]
		ToyKnight_Dead,                           //2	frame:[22]
		ToyKnight_Damage,                         //3	frame:[12]
		ToyKnight_Walk,                           //4	frame:[16]
		ToyKnight_Idle,                           //5	frame:[129]
		End
	};

	enum SoundID {
		Attack, Damage, Death, Sound_End
	};


public:
	explicit CKnight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKnight();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public :
	void			Set_Move(_bool bMove) { m_bMove = bMove; }

public:
	void			Set_Damage(_int iDamage) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

private:
	void			Hit_State(const _float& fTimeDelta);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	class CMorningStar*				m_pMorningStar = nullptr;


	_float						m_fTimeDelta;
	// Hit
	_bool						m_bCodySelect = false;
	_bool						m_bMaySelect = false;

	_float						m_fTurnSpeed = D3DXToRadian(10); //턴 스피드
	_float						m_fAngle;	 //플레이어와의 각도

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iRandom;
	_uint						m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장

	_bool						m_bAttack = false;

private :
	_bool						m_bMove;
	_bool						m_bMovePysics = true;
	_bool						m_bSound[Sound_End] = { false };

public:
	static CKnight*				Create(LPDIRECT3DDEVICE9 pGraphicDev,void* pArg = nullptr, void* pOBJINFO = nullptr);
	virtual void Free(void)		override;
};


#define __KIGHT_H__
#endif

