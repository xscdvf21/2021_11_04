#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr(void);
	virtual ~CLightMgr(void);

public:
	CLight*		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void		Render_Light(LPD3DXEFFECT& pEffect);
	_bool		Is_LightExsist() { return !m_LightList.empty(); }
	void		Clear_Light();
private:
	list<CLight*>		m_LightList;

private:
	virtual void Free(void);

};

END
#endif // LightMgr_h__
