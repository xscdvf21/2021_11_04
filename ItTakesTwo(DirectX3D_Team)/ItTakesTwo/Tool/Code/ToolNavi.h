#ifndef ToolNavi_h__
#define ToolNavi_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CNaviMesh;
class CShader;

END

class CToolNavi : public Engine::CGameObject
{
private:
	explicit CToolNavi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolNavi(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_Material(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	Engine::CNaviMesh*			m_pNaviMeshCom = nullptr;
	Engine::CTransform*			m_pTransformCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

public:
	_uint	m_TextureIndex = 0;
	_uint	m_eMesh;

public:
	static CToolNavi*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;



};



#endif // ToolNavi_h__
