#ifndef __CHESSQUEEN_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessQueen : public CEnermy
{
	enum animID {
		Queen_Idle,                               //0	frame:[61]
		Queen_Dead,                               //1	frame:[151]
		Queen_JumpIn,                             //2	frame:[95]
		Queen_Move_Start,                         //3	frame:[25]
		Queen_Move_End,                           //4	frame:[24]
		Queen_Move,                               //5	frame:[31]
		Queen_Summon,                             //6	frame:[96]
		End
	};

private:
	explicit CChessQueen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessQueen();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	virtual		void		Tick_Damage(const _float& fTimeDelta);

	void			Move(const _float& fTimeDelta);
	void			MoveTile(const _float& fTimeDelta);
	void			JumpReady(const _vec3& vEndPos);

	void			MoveStraight(const  _float& fTimeDelta, const _vec3& TargetPos);

private:
	// ChessTile�� �������ִ� Hp�� 0���� �۰ų� �������Ե� ��� ȣ��
	void			Dead_State(const _float& fTimeDelta);
	void			PhysicsCreate();
public:
	void			Set_Damage(_int iHp) override;

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

	_uint						m_iRandom;
	_uint						m_iAniNum = 0; //���� ����ǰ��ִ� �ִϸ��̼� ��ȣ ����

	_bool						m_bAttackStart = false; //���� ��ŸƮ�� �ѹ��� �ϱ�����
	_float						m_fAttackTime = 0.f; //���� ������ �ֱ� ����.

	_float						m_fCurTime = 0.0f;
	_float						m_fPreTime = 0.0f;

	_float						m_fJumpTest = 0.f;

	_uint						m_iSelect = 0; //����.
	_float						m_fPaturnTime = 0.f; //���� �ð�.



	_float						fAngle;
	_vec3						m_vMoveTile;

private:
	_vec3			m_vTramDis;
	_vec3			m_vTramDst;
	_vec3			m_vTramDir;
	_bool			m_bTram;
	_bool			m_bTramStart;
	_float			m_fTramPower;
	_float			m_fTramTime;
	_float			m_fTramHeight;
	_float			m_fDuration;
	_float			m_fFall;
	_float			m_bFall;
	_float			m_fJumpTestTime = 0.f;
	_vec3			m_vSetPos; //��ŸƮ�Ҷ��� ��ǥ��.
	_bool			m_bAnglecheck;

public:
	static CChessQueen*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;

};



#define __CHESSQUEEN_H__
#endif

