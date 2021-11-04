#ifndef Cogs_h__
#define Cogs_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CCogs : public Engine::CGameObject
{
private:
	explicit CCogs(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCogs(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);
	virtual void*			On_Active(void* pCaller) override;

public:
	void					Set_Key(_uint iKey) { m_iKey = iKey; }

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	class CBoundingBox*		m_pBox = nullptr;

private:
	OBJINFO					m_tObjInfo;
	_bool					m_bRot = false;
	_bool					m_bWork = false;
	_bool					m_bPlaySound = false;
	_uint					m_iKey = 0;
	

public:
	static CCogs*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
