#ifndef __CRUSHERBRIDGEBASE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCrusherBridgeBase : public CGameObject
{
private:
	explicit CCrusherBridgeBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrusherBridgeBase();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bMove;


public:
	static CCrusherBridgeBase*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void							Free(void)		override;
};

#define __CRUSHERBRIDGEBASE_H__
#endif

