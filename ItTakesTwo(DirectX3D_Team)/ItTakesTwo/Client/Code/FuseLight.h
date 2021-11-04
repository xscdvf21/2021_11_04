#ifndef __FuseLight_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CFuseLight : public CGameObject
{
private:
	explicit		 CFuseLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CFuseLight();

public:
	virtual HRESULT Ready_Object(void * pArg, _bool bRun = false);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void			Set_Light(_bool light) { m_bLight = light; }

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private :
	_bool			m_bLight = false;
	_int			m_iPass = 9;

public:
	static CFuseLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun = false);
	virtual void Free(void)		override;
};


#define __FuseLight_H__
#endif
