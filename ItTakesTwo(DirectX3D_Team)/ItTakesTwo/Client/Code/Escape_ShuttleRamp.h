#ifndef __ESCAPE_SHUTTLERAMP_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CShader;

END

class CEscape_ShuttleRamp : public CGameObject
{
private:
	explicit CEscape_ShuttleRamp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEscape_ShuttleRamp();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	

private :
	_bool				m_bMove = false;

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

public:
	static CEscape_ShuttleRamp*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void							Free(void)		override;

};

#define __ESCAPE_SHUTTLERAMP_H__
#endif

