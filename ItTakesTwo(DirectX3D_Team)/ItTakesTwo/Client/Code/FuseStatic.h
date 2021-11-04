#ifndef __FuseStatic_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CFuseStatic : public CGameObject
{
private:
	explicit		 CFuseStatic(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CFuseStatic();

public:
	virtual HRESULT Ready_Object(void * pArg, _bool bRun = false);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

public:
	static CFuseStatic*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun = false);

public:
	virtual void Free(void)		override;
};


#define __FuseStatic_H__
#endif
