#ifndef GraphicDevice_h__
#define GraphicDevice_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)

private:
	explicit	CGraphicDevice(void);
	virtual		~CGraphicDevice(void);

public:
	LPDIRECT3DDEVICE9		GetDevice(void) { return m_pGraphicDev; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDevice** ppGraphicDev);
	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End(void);

private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;

public:
	virtual void Free(void);
};

END
#endif // GraphicDevice_h__
