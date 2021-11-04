#ifndef __CRUSHERBREAKWALL_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCrusherBreakWall : public CEnermy
{
private:
	explicit  CCrusherBreakWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CCrusherBreakWall();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr, _bool bHit = true);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public : 
	void				Set_Hit(_bool bHit) { m_bHit = bHit; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bMove;
	_bool							m_bHit;

public:
	static CCrusherBreakWall*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _bool bHit = true);
	virtual void						Free(void)		override;

};


#define __CRUSHERBREAKWALL_H__
#endif

