
#ifndef __BABOONBOMBUPBULLET_H__


#include "Enermy.h"

class CBaboonBombUpBullet : public CEnermy
{
private:
	explicit CBaboonBombUpBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBaboonBombUpBullet();

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
	Engine::CShader*			m_pShaderCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;

private :
	_float						m_fLifeTime = 0.f;
	_float						m_fScale; 

public:
	static CBaboonBombUpBullet*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free(void)		override;
};

#define __BABOONBOMBUPBULLET_H__
#endif

