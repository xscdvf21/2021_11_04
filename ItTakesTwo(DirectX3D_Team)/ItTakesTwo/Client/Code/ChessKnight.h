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
		Enemy_PlayRoom_ChessKnight_AttackMove_var1,//0	frame:[261] //�ٰ������ΰ�.?
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

	_float						m_fTurnSpeed = D3DXToRadian(2); //�� ���ǵ�
	_float						m_fAngle;	 //�÷��̾���� ����

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iAniNum = 0; //���� ����ǰ��ִ� �ִϸ��̼� ��ȣ ����

	_float						m_fMoveStart = 0.f; //���� ��ŸƮ ��ǰ� �̵� ��� Ÿ�̹� ���߷���.
	_float						m_fDeadPos;

	// ������ �����ϸ� �� ó���� Ÿ���Ϸ���
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


