#ifndef Map_h__
#define Map_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CDynamicMesh;
class CShader;
class CTexture;
class CPhysXMesh;
class CFrustum;

END

class CMap : public Engine::CGameObject
{
private:
	explicit CMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMap(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;
	virtual void			Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex = 0) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_Material(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	

public:
	Engine::CStaticMesh*	m_pStaticMeshCom = nullptr;
	Engine::CDynamicMesh*	m_pDynamicMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CFrustum*		m_pFrustum = nullptr;

public:
	void					Set_PassIndex(_int pass) { m_iPass = pass; }
	void					Set_MaterialIndex(_int materialIndex) { m_fMaterialIndex = _float(materialIndex); }

public:
	_uint					m_TextureIndex = 0;
	_uint					m_eMesh;
	_uint					m_eRender;
	_float					m_fFog;
	_tchar					m_tagMesh[MAX_PATH];
	_int					m_iPass;
	_matrix					m_matProj;
	_bool					m_bShadow = true;
	_float					m_fMaterialIndex = 0.f;
	_mat					m_matWorld;

public:
	static CMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};



#endif // Map_h__
