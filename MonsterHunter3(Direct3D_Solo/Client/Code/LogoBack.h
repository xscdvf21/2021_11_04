#ifndef LogoBack_h__
#define LogoBack_h__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CDynamicMesh;
END

class CLogoBack : public Engine::CGameObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoBack(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
public:
	static CLogoBack*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;



};



#endif // LogoBack_h__
