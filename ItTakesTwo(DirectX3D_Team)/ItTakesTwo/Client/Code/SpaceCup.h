#ifndef __SpaceCup_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CShader;

END

class CSpaceCup : public CGameObject
{
private:
	explicit CSpaceCup(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceCup();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;




	virtual void* On_Active(void* pCaller) override;

public:

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:

private:
	Engine::CDynamicMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
	_bool m_bBorken = false;
public:
	static CSpaceCup*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __SpaceCup_H__
#endif

