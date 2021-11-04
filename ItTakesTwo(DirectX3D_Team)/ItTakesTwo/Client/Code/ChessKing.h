#ifndef __CHESSKING_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessKing : public CEnermy
{
	enum animID {
		King_Dead,                                //0	frame:[151]
		King_Jump,                                //1	frame:[49]
		King_Idle,                                //2	frame:[61]
		King_Summon,                              //3	frame:[97]
		End
	};

	enum SoundID
	{
		JumpEnd,
		Death,
		SoundEnd,
	};


private:
	explicit CChessKing(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessKing();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;


public:
	CGameObject*		Get_Target() { return m_pTargetPlayer; }

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);\

public:
	virtual void			Tick_Damage(const _float& fTimeDelta) override;


	void			Move(const _float& fTimeDelta);
	void			MoveTile(const _float& fTimeDelta);
	void			JumpReady(const _vec3& vEndPos);

	void			MoveJump(const  _float& fTimeDelta , const _vec3& TargetPos);

private:
	// ChessTile이 가지고있는 Hp가 0보다 작거나 같아지게될 경우 호출
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

	_float						m_fTurnSpeed = D3DXToRadian(2); //턴 스피드
	_float						m_fAngle;	 //플레이어와의 각도

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iRandom;
	_uint						m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장

	_bool						m_bAttackStart = false; //어택 스타트를 한번만 하기위해
	_float						m_fAttackTime = 0.f; //어택 딜레이 주기 위해.

	_float						m_fCurTime = 0.0f;
	_float						m_fPreTime = 0.0f;

	_float						m_fJumpTest = 0.f;
		
	_uint						m_iSelect = 0; //패턴.
	_float						m_fPaturnTime = 0.f; //패턴 시간.


	_float						fAngle;
	_vec3						m_vMoveTile;


private :
	_bool						m_bSound[SoundEnd] = { false };
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
	_vec3			m_vSetPos; //스타트할때의 목표점.
	_bool			m_bAnglecheck;
	_float			m_fCameraAngle = 0.f;
private:
	void			Effect_Land();

public:
	static CChessKing*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;

};


#define __CHESSKING_H__
#endif

