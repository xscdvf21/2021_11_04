#ifndef ToolTerrain_h__
#define ToolTerrain_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CNormalTerrainTex;
class CTerrainTex;
class CTexture;
class CTransform;

END

class CToolTerrain : public Engine::CGameObject
{
private:
	explicit CToolTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolTerrain(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_Material(void);

public:
	HRESULT			Change_Texture(_uint iIndex);

public:
	Engine::CNormalTerrainTex*	m_pBufferCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTransform*			m_pTransformCom = nullptr;

public:
	_uint	m_TextureIndex = 0;

public:
	static CToolTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;



};



#endif // ToolTerrain_h__
