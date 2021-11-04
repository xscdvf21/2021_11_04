#ifndef SaveSpace_h__
#define SaveSpace_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;
class CRcTex;
class CTexture;
class CRenderer;
class CShader;
END

class CSaveSpace : public Engine::CGameObject
{
private:
	explicit CSaveSpace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSaveSpace(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	 HRESULT				Ready_Object(_vec3 vSavePoint, _tchar* tTexName = L"",_bool bTex = false, void* pArg = nullptr) ;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;



private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	_vec3					Get_SavePoint() { return m_vSavePoint; }

public:
	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	void Set_CodyRespwanPoint(_vec3 vCody) { m_vCodyRespwanPoint = vCody; }
	void Set_MayRespwanPoint(_vec3 vMay) { m_vMayRespwanPoint = vMay; }
	void Set_MayRespwanUp(_vec3 vMayUp) { m_vMayRespwanUp = vMayUp; }

public:
	_vec3 Get_CodyRespwanPoint() { return m_vCodyRespwanPoint; }
	_vec3 Get_MayRespwanPoint() { return m_vMayRespwanPoint; }
	_vec3 Get_MayRespwanUp() { return m_vMayRespwanUp; }

public:
	static CSaveSpace*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vSavePoint, _tchar* tTexName = L"", _bool bTex = false, void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;

private :			
	_bool					m_bTex;
	_tchar*					m_tTexName;
	_vec3					m_vSavePoint {};

	_vec3	m_vCodyRespwanPoint = {};
	_vec3	m_vMayRespwanPoint = {};
	_vec3	m_vMayRespwanUp = {};

private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
