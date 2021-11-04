#ifndef __OGRESTONE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class COgreStone : public CGameObject
{
private:
	explicit COgreStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COgreStone();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr, _vec3 vRot = { 0.f, 0.f, 0.f });
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	CTrigger*					m_pTrigger = nullptr;

private :
	_vec3						m_vRot;

public:
	static COgreStone*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _vec3 vRot = { 0.f, 0.f, 0.f });
	virtual void Free(void)		override;
};



#define __OGRESTONE_H__
#endif

