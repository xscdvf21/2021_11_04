#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;

END

class CEffect : public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual HRESULT Ready_Object(EFFECTID _Effectid, _float LifeTime);

	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

	_float			m_fFrame = 0.f;

	_float			m_fLifeTime = 0.f;
	_uint			m_iIndex = 0;
private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, EFFECTID _Effectid, _float LifeTime);
private:
	virtual void Free(void) override;



};



#endif // Effect_h__
