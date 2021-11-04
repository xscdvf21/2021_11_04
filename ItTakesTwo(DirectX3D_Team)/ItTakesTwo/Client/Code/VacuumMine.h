#ifndef __VACUUMMINE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CVacuumMine : public CGameObject
{
private:
	explicit CVacuumMine(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumMine();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, _vec3 vTargetPos);
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
	_float			m_fEffectTime = 0.f;


public:
	static CVacuumMine*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos);
	virtual void Free(void)		override;
};



#define __VACUUMMINE_H__
#endif

