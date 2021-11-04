#ifndef __VACUUMSTONE_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CVacuumStone : public CEnermy
{
private:
	explicit CVacuumStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumStone();

public:
	virtual HRESULT Ready_Object(void* pArg, _vec3 vTargetPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta, const _vec3& TargetPos);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_vec3						m_vDir = { 0.f, 0.f, 0.f }; //나아갈 방향벡터.
	_vec3						m_vTargetPos = { 0.f, 0.f, 0.f }; //인자로 받은 타켓 포스.
	_vec3						m_vPos = { 0.f, 0.f, 0.f }; //생성된 자신의 위치

	_float						m_fLifeTime = 0.f;
private:
	_vec3						m_vRot;

	CTrigger*					m_pTrigger = nullptr;

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
	_float			m_fJumpTestTime = 0.f;
	_vec3			m_vSetPos; //스타트할때의 목표점.
	_bool			m_bAnglecheck;


public:
	static CVacuumStone*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg, _vec3 vTargetPos);
	virtual void Free(void)		override;
};


#define __VACUUMSTONE_H__
#endif

