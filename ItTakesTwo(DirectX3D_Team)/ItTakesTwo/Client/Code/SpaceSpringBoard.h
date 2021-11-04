#ifndef __SpaceSpringBoard_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;

class CShader;

END

class CSpaceSpringBoard : public CGameObject
{
private:
	explicit CSpaceSpringBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceSpringBoard();

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
	void				Move(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
public:
	static CSpaceSpringBoard*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;

};

#define __SpaceSpringBoard_H__
#endif

