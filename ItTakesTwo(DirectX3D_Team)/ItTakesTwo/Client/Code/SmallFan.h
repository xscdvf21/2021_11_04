#ifndef SmallFan_h__
#define SmallFan_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CSmallFan : public Engine::CGameObject
{
private:
	explicit CSmallFan(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSmallFan(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;



private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void					Set_AddAngle(_float fAddAngle) { m_fAddAngle = fAddAngle; }
	void					Set_Move(_bool bMove, _float fDir) { m_bMove = bMove, m_fDir = fDir; }
	void					Set_Swithch();

public:
	void					Add_BodyList(wstring wstrBody) { m_vecBodyList.emplace_back(wstrBody); }

public:
	virtual void*			On_Active(void* pCaller) override;
	virtual void*			On_Attach(void* pCaller) override;
	void					On_Attach(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CStaticMesh*	m_pSylinderMeshCom[4];
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CStaticMesh*	m_pModuleMeshCom[3];
	Engine::CTransform*		m_pModuleTransformCom[3];

	vector<wstring>			m_vecBodyList;

private:
	OBJINFO					m_tObjInfo;
	_bool					m_bSwitchOn = false;
	_float					m_fAddAngle = 0.0f;
	_bool					m_bMove = false;
	_float					m_fRotSpeed = 0.0f;
	_float					m_fDir = 0.0f;
	_float					m_fMoveAngle = 0.0f;
	_vec3					m_vResult{};
	_float					m_fVolume = 0.0f;

	class CTransform*		m_pCodyTransform = nullptr;
	class CTransform*		m_pMayTransform = nullptr;
	_float					m_fVolume2 = 0.0f;

public:
	static CSmallFan*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#endif // __FAN_H__
