#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CRenderer;
class CTransform;
class COptimization;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_Material(void);

private:
	Engine::CTerrainTex*	m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::COptimization*	m_pOptimizationCom = nullptr;

	Engine::INDEX32*		m_pIndex = nullptr;
	_ulong					m_dwTriCnt = 0;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;



};



#endif // Terrain_h__
