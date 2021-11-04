#include "Export_System.h"
// GraphicDev
// Get
// Set
// General
HRESULT		Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void		Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void		Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}

// TimerMgr
// Get
_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
// Set
void		Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
// General
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

// FrameMgr
// General
_bool				IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT				Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

// FontMgr
// Get
// Set
// General
 HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

 void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	 CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

 inline void Render_Font3D(const _tchar * pFontTag,const _tchar * pString, const _vec2* pPos, const _matrix _PlayerWorld, D3DXCOLOR Color)
 {
	 CFontMgr::GetInstance()->Render_Font3D(pFontTag, pString, pPos, _PlayerWorld, Color);
 }



 // dinput
 // Get
 _byte	Get_DIKeyState(_ubyte byKeyID)
 {
	 return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
 }
 _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
 {
	 return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
 }
 _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
 {
	 return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
 }

_bool Key_Down(_ubyte byKeyID)
 {

	_bool Temp = false;

	Temp = CInputDev::GetInstance()->Key_Down(byKeyID);
	 return Temp;
 }



 // Set
 // General
 HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
 {
	 return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
 }
void	Set_InputDev(void)
{
	CInputDev::GetInstance()->Set_InputDev();
}

inline void Update_KeyMgr()
{
	CInputDev::GetInstance()->Update_KeyMgr();
}



void		Release_System(void) 
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}
