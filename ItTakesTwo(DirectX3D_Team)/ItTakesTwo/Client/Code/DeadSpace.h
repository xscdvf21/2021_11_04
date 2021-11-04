#ifndef __DEADSPACE_H__
#define __DEADSPACE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;
class CTexture;
class CRenderer;
class CShader;
class CStaticMesh;

END

class CDeadSpace : public Engine::CGameObject
{
private:
	explicit CDeadSpace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDeadSpace(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	 HRESULT				Ready_Object(_tchar* tTexName = L"",_bool bTex = false, void* pArg = nullptr) ;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;
	 
private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
public:
	void*			On_Active(void* pCaller) override;

public:
	static CDeadSpace*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _tchar* tTexName = L"", _bool bTex = false, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;

private :			
	_bool					m_bTex;
	_tchar*					m_tTexName;
	_float					m_fTimeDelta = 0.f;
	_int					m_iPass = 15;
private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
