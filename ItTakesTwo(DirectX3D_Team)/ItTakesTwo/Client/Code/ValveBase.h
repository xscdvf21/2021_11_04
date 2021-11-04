#ifndef ValveBase_h__
#define ValveBase_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CValveBase : public Engine::CGameObject
{
private:
	explicit CValveBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CValveBase(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;
	
public:
	virtual	void*			On_Active(void* pCaller) override;
	virtual void*			On_Attach(void* pCaller) override;
	void					Add_List(wstring wstrObjName) { m_vecCogsList.emplace_back(wstrObjName); }

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	_bool					m_bButton1 = false;
	_bool					m_bButton2 = false;
	_bool					m_bWork = false;
	_float					m_fDuration = 1.0f;
	vector<wstring>			m_vecCogsList;
	_float					m_fMoveAngle = 0.0f;
	_vec3					m_vResult{};

public:
	static CValveBase*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
