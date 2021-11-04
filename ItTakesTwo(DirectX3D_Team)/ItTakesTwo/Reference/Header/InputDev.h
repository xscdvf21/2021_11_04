#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"
BEGIN(Engine)
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#define DIK_MAX 0xFF

class ENGINE_DLL CInputDev : public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev(void);
	virtual ~CInputDev(void);

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 
	{ return m_tMouseState.rgbButtons[eMouse]; }
	
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

	_float Get_DIControllerMove(PADMOVESTATE ePadState);

	_bool Key_Pressing(_ubyte _key);
	_bool Key_Down(_ubyte _key);
	_bool Key_Up(_ubyte _key);

	_bool Key_Pressing(MOUSEKEYSTATE eMouse);
	_bool Key_Down(MOUSEKEYSTATE eMouse);
	_bool Key_Up(MOUSEKEYSTATE eMouse);

	_bool Key_Pressing(PADKEYSTATE ePad);
	_bool Key_Down(PADKEYSTATE ePad);
	_bool Key_Up(PADKEYSTATE ePad);

public:
	void	SetUp_Key();
	void	SetUp_Mouse();
	_bool	SetUp_Pad();
	_bool	Check_Connection();
private:

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Set_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	
private:
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_tMouseState;
	_bool					m_bState[DIK_MAX];
	_bool					m_bMouse[DIM_END];
	_bool					m_bController[DIC_END];
	vector<WORD>			m_vecPadID;

	XINPUT_STATE			m_tControllerState;
	XINPUT_GAMEPAD			m_tController;

	_float					leftStickX =0.f;
	_float					leftStickY = 0.f;
	_float					rightStickX = 0.f;
	_float					rightStickY = 0.f;
	_float					leftTrigger = 0.f;
	_float					rightTrigger = 0.f;

	_float					deadzoneX = 0.f;
	_float					deadzoneY = 0.f;
	_int					m_cID = -1;
public:
	virtual void	Free(void);

};
END
#endif // InputDev_h__
