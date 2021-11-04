#ifndef __SpaceValveSpring_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceValveSpring : public CGameObject
{
private:
	explicit CSpaceValveSpring(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceValveSpring();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;



public:

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CStaticMesh*			m_pMeshSpringCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
	Engine::CTransform*				m_pTransformSpringCom= nullptr;
	CTrigger*						m_pTrigger = nullptr;

private:
public:
	static CSpaceValveSpring*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

private:
	void Move(const _float & fTimeDelta);
};

#define __SpaceValveSpring_H__
#endif

