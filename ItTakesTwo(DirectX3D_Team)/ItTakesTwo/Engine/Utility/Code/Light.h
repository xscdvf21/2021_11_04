#ifndef Light_h__
#define Light_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);

public:
	HRESULT		Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void		Render_Light(LPD3DXEFFECT& pEffect);

public:
	void		Set_LightPos(_vec3 vPos) { m_tLightInfo.Position = vPos; }
	void		Set_LightAddPos(_vec3 vPos) { m_tLightInfo.Position.x += vPos.x; m_tLightInfo.Position.y += vPos.y; m_tLightInfo.Position.z += vPos.z;}
	void		Set_LightSize(_float fSize) { m_tLightInfo.Range = fSize; }
	void		Set_LightRender(_bool render) { m_bRender = render; }

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DLIGHT9					m_tLightInfo;
	_uint						m_iIndex;
	_bool						m_bRender = true;
	_bool						m_bStart = true;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
public:
	static CLight*				Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	virtual void Free(void);

};

END
#endif // Light_h__
