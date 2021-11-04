#ifndef TestWall_h__
#define TestWall_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CDynamicMesh;
class CShader;
class CTexture;
class CPhysXMesh;

END

class CTestWall : public Engine::CGameObject
{
private:
	explicit CTestWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestWall(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

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

public:
	_uint					m_TextureIndex = 0;
	_uint					m_eMesh;
	_uint					m_eRender;
	_float					m_fFog;
	_tchar					m_tagMesh[MAX_PATH];
	_int					m_iPass;

	//
	_bool					m_bCreateShape = false;

public:
	static CTestWall*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
