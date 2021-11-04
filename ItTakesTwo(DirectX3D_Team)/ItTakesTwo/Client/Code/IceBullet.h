#ifndef __ICEBULLET_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

class CIceBullet : public CEnermy
{
private:
	explicit CIceBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIceBullet();

public:
	virtual HRESULT Ready_Object(_vec3 vPos, _vec3 vDir);
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
	Engine::CTexture*			m_pTextureCom = nullptr;

	_float						m_fLifeTime = 0.f;
	_vec3						m_vDir; //인자로 받는 나아갈 벡터 받음
	_float						m_fScale; //어느정도 커지면 나가게 하기위해.
public:
	static CIceBullet*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir);
	virtual void Free(void)		override;


};




#define __ICEBULLET_H__
#endif

