#ifndef __ARROW_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CArrow : public CEnermy
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArrow();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, _vec3 vTargetPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);
	void			BoneUpdate(const _float& fTimeDelta);
	void			PhyscisCreate();
private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	

	_vec3						m_vDir = { 0.f, 0.f, 0.f }; //나아갈 방향벡터.
	_vec3						m_vTargetPos = { 0.f, 0.f, 0.f }; //인자로 받은 타켓 포스.
	_vec3						m_vPos = { 0.f, 0.f, 0.f }; //생성된 자신의 위치

	_float						m_fLifeTime = 0.f;

	
private :
	_bool							m_bCreateTrigger;
	CTrigger*						m_pTrigger = nullptr;
public:
	static CArrow*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos);
	virtual void Free(void)		override;


};




#define __ARROW_H__
#endif

