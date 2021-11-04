#ifndef __CHESSKNIGHT_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CChessKnight : public CEnermy
{
	enum animID {
		Enemy_PlayRoom_ChessKnight_AttackMove_var1,//0	frame:[261] //다같은것인가.?
		Enemy_PlayRoom_ChessKnight_AttackMove_var2,//1	frame:[261]
		Enemy_PlayRoom_ChessKnight_AttackMove_var3,//2	frame:[261]
		Enemy_PlayRoom_ChessKnight_AttackMove_var4,//3	frame:[261]
		Enemy_PlayRoom_ChessKnight_Charge_Fwd_Fast,//4	frame:[41]
		Enemy_PlayRoom_ChessKnight_Charge_Fwd_Slow,//5	frame:[41]
		Enemy_PlayRoom_ChessKnight_mh,            //6	frame:[2]
		Enemy_PlayRoom_ChessKnight_Play_Move,     //7	frame:[67]
		Enemy_PlayRoom_ChessKnight_Summon,        //8	frame:[91]
		End
	};

	enum SoundID
	{
		Ready_Cry, Ready_Ground, Attack_Ready,
		SoundID_End
	};
		


private:
	explicit CChessKnight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessKnight();

public:
	virtual HRESULT Ready_Object(_uint iTileIndex = 0);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;



private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
public:
	void			Set_Damage(_int iDamage) override;
private:
	void			Move(const _float& fTimeDelta);
	void			Dead_State(const _float& fTimeDelta);
	void			PhysicsCreate();
	void			Effect_Walk();

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_bool						m_bCodySelect = false;
	_bool						m_bMaySelect = false;

	_float						m_fTurnSpeed = D3DXToRadian(2); //턴 스피드
	_float						m_fAngle;	 //플레이어와의 각도

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장

	_float						m_fMoveStart = 0.f; //공격 스타트 모션과 이동 모션 타이밍 맞추려고.
	_float						m_fDeadPos;

	// 공격이 시작하면 맨 처음만 타게하려고
	_bool						m_bAttStart = false;
	_float						m_fEffecTime = 0.f;

private : 
	_bool						m_bSound[SoundID_End] = { false };
public:
	static CChessKnight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iTileIndex = 0);
	virtual void Free(void)		override;
};

#define __CHESSKNIGHT_H__
#endif


