#ifndef __PAWNBULLET_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CPawnBullet : public Engine::CGameObject
{
private:
	explicit CPawnBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPawnBullet();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_uint						m_iAniNum = 0;
	_float						m_fLifeTime = 0.f;

private:
	void Effect();

private:
	_float m_fEffecTime = 0.f;
	_bool m_bEffect = false;

public:
	static CPawnBullet*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free(void)		override;


};


#define __PAWNBULLET_H__
#endif


