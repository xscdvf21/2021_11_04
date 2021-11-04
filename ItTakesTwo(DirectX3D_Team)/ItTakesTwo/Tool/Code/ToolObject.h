#ifndef ToolObject_h__
#define ToolObject_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CStaticMesh;
class CDynamicMesh;
class CShader;

END

class CToolObject : public Engine::CGameObject
{
private:
	explicit CToolObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolObject(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_Material(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	Engine::CStaticMesh*		m_pStaticMeshCom = nullptr;
	Engine::CDynamicMesh*		m_pDynamicMeshCom = nullptr;
	Engine::CTransform*			m_pTransformCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

public:
	_uint	m_TextureIndex = 0;
	_uint	m_eMesh;
	_uint	m_eRender;
	_int	m_iPassIndex;

public:
	static CToolObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;



};



#endif // ToolObject_h__
