#ifndef FireSwordEffect_h__
#define FireSwordEffect_h__

#include "GameObject.h"
#include "Base.h"
#include "Client_Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CShader;

END

class CFireSwordEffect : public Engine::CGameObject
{
private:
	explicit CFireSwordEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireSwordEffect(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT					Add_Component(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CFireSwordEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;



};



#endif // FireSwordEffect_h__
