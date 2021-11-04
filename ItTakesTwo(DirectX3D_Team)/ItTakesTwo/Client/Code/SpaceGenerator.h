#ifndef __SPACEGENERATOR_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

class CSpaceGenerator : public CGameObject
{
private:
	explicit CSpaceGenerator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceGenerator();

public:
	virtual HRESULT Ready_Object(void * pArg, _uint Index = 0);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex, const _int& iMeshNum = 100);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

public:
	static CSpaceGenerator*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _uint Index = 0);
	virtual void				Free(void)		override;
};



#define __SPACEGENERATOR_H__
#endif
