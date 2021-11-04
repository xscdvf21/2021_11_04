#ifndef SpaceGlassBox_h__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceGlassBox : public CGameObject
{
private:
	explicit CSpaceGlassBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceGlassBox();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;




	virtual void*		On_Active(void* pCaller) override;

public:

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
	class CTrigger*					m_pTrigger = nullptr;
	
private:
	
public:
	static CSpaceGlassBox*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define SpaceGlassBox_h__
#endif

