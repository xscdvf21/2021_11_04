#ifndef __LightObject_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CLightObject : public CGameObject
{
private:
	explicit CLightObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightObject();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	 HRESULT		Ready_Object(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;




	virtual void* On_Active(void* pCaller) override;


	virtual void* On_Trigger_Active(void* pTrigger,void* pCaller) override;


	virtual void* On_Attach(void* pCaller) override;

public:

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	D3DLIGHT9			m_tLightInfo;
	_uint				m_iIndex = 0;
private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
public:
	static CLightObject*				Create(LPDIRECT3DDEVICE9 pGraphicDev,  const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	virtual void					Free(void)		override;

};

#define __LightObject_H__
#endif

