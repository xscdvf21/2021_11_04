#ifndef __TESTCOL_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CCubeTex;
class CShader;
class CTexture;
class CPhysXMesh;

END

class CTestCol : public Engine::CGameObject
{
private:
	explicit CTestCol(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestCol(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_Material(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
public :
	void					Set_OgreCol(_bool _bCol) { m_bOgreCol = _bCol; }
	_bool					IsCol() { return m_bOgreCol; }

public:
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CPhysXMesh*		m_pPhysMesh = nullptr;
	Engine::CCubeTex*		m_pCubeCom = nullptr;


	_uint					m_TextureIndex = 0;
	_uint					m_eMesh;
	_uint					m_eRender;
	_float					m_fFog;
	_tchar					m_tagMesh[MAX_PATH];
	_int					m_iPass;

private :
	_bool					m_bOgreCol =false;

public:
	static CTestCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};

#define __TESTCOL_H__
#endif

