#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CShader;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_Material(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	_uint					m_TextureIndex = 0;
	_float					m_fAddUV;
	_uint					m_iTextureIndex = 0;
	_uint					m_iPass = 0;

public:
	static CTerrain*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;
};



#endif // Terrain_h__
