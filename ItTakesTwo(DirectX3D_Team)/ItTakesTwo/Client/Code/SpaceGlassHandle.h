#ifndef SpaceGlassHandle_h__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceGlassHandle : public CGameObject
{
private:
	explicit CSpaceGlassHandle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceGlassHandle();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;


	virtual void*		On_Active(void* pCaller) override;

public:
	_vec3				Get_PushDir() { return m_vPushDir; }
	_vec3				Get_HandlePos() { return m_pTransformCom->Get_Position(); }
	_vec3				Get_Pos() { return m_vTargetPos; }

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
	CTrigger*						m_pTrigger = nullptr;
	_vec3							m_vPushDir = {};
	_vec3							m_vTargetPos = { 162.0621f, 14.1f, -5.8465f };

private:
public:
	static CSpaceGlassHandle*				Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPushDir, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define SpaceGlassHandle_h__
#endif

