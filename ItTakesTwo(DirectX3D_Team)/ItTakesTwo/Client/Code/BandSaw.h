#ifndef BandSaw_h__
#define BandSaw_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CBandSaw : public Engine::CGameObject
{
private:
	explicit CBandSaw(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBandSaw(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

private:
	OBJINFO					m_tObjInfo;


public:
	static CBandSaw*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
