#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit	CGraphicDev(void);
	virtual		~CGraphicDev(void);

public:
	LPDIRECT3DDEVICE9		GetDevice(void) { return m_pGraphicDev; }
	LPD3DXSPRITE			Get_Sprite() { return m_pSprite; }
public:
	HRESULT		Ready_GraphicDev(HWND hWnd,	WINMODE eMode, const _uint& iSizeX,	const _uint& iSizeY,CGraphicDev** ppGraphicDev);
	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	LPD3DXSPRITE			m_pSprite = nullptr;
public:
	virtual void Free(void);
};

END
#endif // GraphicDev_h__
