#ifndef __CHESSDOORSWITCHFRAME_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessDoorSwitchFrame : public CGameObject
{
private:
	explicit CChessDoorSwitchFrame(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessDoorSwitchFrame();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

public:
	static CChessDoorSwitchFrame*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void							Free(void)		override;
};

#define __CHESSDOORSWITCHFRAME_H__
#endif

