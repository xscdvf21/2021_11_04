#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)
//enum PADKEYSTATE { DIC_LB, DIC_LT, DIC_RB, DIC_RT, DIC_X, DIC_Y, DIC_A, DIC_B, DIC_START, DIC_MENU, DIC_LS, DIC_RS, DIC_END };

Engine::CInputDev::CInputDev(void)
{
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_LEFT_SHOULDER);
	m_vecPadID.emplace_back(0);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_vecPadID.emplace_back(0);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_X);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_Y);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_A);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_B);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_START);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_BACK);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_LEFT_THUMB);
	m_vecPadID.emplace_back(XINPUT_GAMEPAD_RIGHT_THUMB);
}

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

Engine::_float Engine::CInputDev::Get_DIControllerMove(PADMOVESTATE ePadState)
{
	if (m_cID ==- 1)
		return 0;

	switch (ePadState)
	{
	case Engine::DIC_LS_X:
		if (isnan(leftStickX))
			leftStickX = 0.f;
		return leftStickX;
		break;
	case Engine::DIC_LS_Y:
		if (isnan(leftStickY))
			leftStickY = 0.f;
		return leftStickY;
		break;
	case Engine::DIC_RS_X:
		if (isnan(rightStickX))
			rightStickX = 0.f;
		return rightStickX;
		break;
	case Engine::DIC_RS_Y:
		if (isnan(rightStickY))
			rightStickY = 0.f;
		return rightStickY;
		break;
	case Engine::DICS_END:
		break;
	default:
		break;
	}
	return 0;
}

_bool CInputDev::Key_Pressing(_ubyte _key)
{
	if (m_byKeyState[_key] & 0x80)
		return true;

	return false;
}

Engine::_bool Engine::CInputDev::Key_Pressing(PADKEYSTATE ePad)
{
	switch (ePad)
	{
	case Engine::DIC_LB:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
		break;
	case Engine::DIC_LT:
		return (m_tControllerState.Gamepad.bLeftTrigger ) != 0;
		break;
	case Engine::DIC_RB:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
		break;
	case Engine::DIC_RT:
		return (m_tControllerState.Gamepad.bRightTrigger) != 0;
		break;
	case Engine::DIC_X:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
		break;
	case Engine::DIC_Y:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
		break;
	case Engine::DIC_A:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
		break;
	case Engine::DIC_B:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
		break;
	case Engine::DIC_START:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
		break;
	case Engine::DIC_MENU:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
		break;
	case Engine::DIC_LS:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
		break;
	case Engine::DIC_RS:
		return (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
		break;
	case Engine::DIC_END:
		break;
	default:
		break;
	}
	return false;
}

_bool CInputDev::Key_Down(_ubyte _key)
{
	if (!m_bState[_key] && (m_byKeyState[_key] & 0x80))
	{
		m_bState[_key] = !m_bState[_key];
		return true;
	}
	return false;
}

Engine::_bool Engine::CInputDev::Key_Down(PADKEYSTATE ePad)
{
	switch (ePad)
	{
	case Engine::DIC_LB:
		if (!m_bController[DIC_LB] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			m_bController[DIC_LB] = !m_bController[DIC_LB];
			return true;
		}
		return false;
		break;
	case Engine::DIC_LT:
		if (!m_bController[DIC_LT] && (m_tControllerState.Gamepad.bLeftTrigger != 0))
		{
			m_bController[DIC_LT] = !m_bController[DIC_LT];
			return true;
		}
		return false;
		break;
	case Engine::DIC_RB:
		if (!m_bController[DIC_RB] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			m_bController[DIC_RB] = !m_bController[DIC_RB];
			return true;
		}
		return false;
		break;
	case Engine::DIC_RT:
		if (!m_bController[DIC_RT] && (m_tControllerState.Gamepad.bRightTrigger != 0))
		{
			m_bController[DIC_RT] = !m_bController[DIC_RT];
			return true;
		}
		return false;
		break;
	case Engine::DIC_X:
		if (!m_bController[DIC_X] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X))
		{
			m_bController[DIC_X] = !m_bController[DIC_X];
			return true;
		}
		return false;
		break;
	case Engine::DIC_Y:
		if (!m_bController[DIC_Y] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y))
		{
			m_bController[DIC_Y] = !m_bController[DIC_Y];
			return true;
		}
		return false;
		break;
	case Engine::DIC_A:
		if (!m_bController[DIC_A] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A))
		{
			m_bController[DIC_A] = !m_bController[DIC_A];
			return true;
		}
		return false;
		break;
	case Engine::DIC_B:
		if (!m_bController[DIC_B] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B))
		{
			m_bController[DIC_B] = !m_bController[DIC_B];
			return true;
		}
		return false;
		break;
	case Engine::DIC_START:
		if (!m_bController[DIC_START] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START))
		{
			m_bController[DIC_START] = !m_bController[DIC_START];
			return true;
		}
		return false;
		break;
	case Engine::DIC_MENU:
		if (!m_bController[DIC_MENU] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK))
		{
			m_bController[DIC_MENU] = !m_bController[DIC_MENU];
			return true;
		}
		return false;
		break;
	case Engine::DIC_LS:
		if (!m_bController[DIC_LS] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB))
		{
			m_bController[DIC_LS] = !m_bController[DIC_LS];
			return true;
		}
		return false;
		break;
	case Engine::DIC_RS:
		if (!m_bController[DIC_RS] && (m_tControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			m_bController[DIC_RS] = !m_bController[DIC_RS];
			return true;
		}
		return false;
		break;
	case Engine::DIC_END:
		break;
	default:
		break;
	}
	return false;
}

_bool CInputDev::Key_Up(_ubyte _key)
{
	if (m_bState[_key] && !(m_byKeyState[_key] & 0x80))
	{
		m_bState[_key] = !m_bState[_key];
		return true;
	}
	return false;
}

Engine::_bool Engine::CInputDev::Key_Up(PADKEYSTATE ePad)
{
	switch (ePad)
	{
	case Engine::DIC_LB:
		if (m_bController[ePad] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[ePad] = !m_bController[ePad];
			return true;
		}
		return false;
		break;
	case Engine::DIC_LT:
		if (m_bController[ePad] && !(m_tControllerState.Gamepad.bLeftTrigger != 0))
		{
			m_bController[ePad] = !m_bController[ePad];
			return true;
		}
		return false;
		break;
	case Engine::DIC_RB:
		if (m_bController[DIC_RB] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_RB] = !m_bController[DIC_RB];
			return true;
		}
		return false;
		break;
	case Engine::DIC_RT:
		if (m_bController[DIC_RT] && !(m_tControllerState.Gamepad.bRightTrigger != 0))
		{
			m_bController[DIC_RT] = !m_bController[DIC_RT];
			return true;
		}
		return false;
		break;
	case Engine::DIC_X:
		if (m_bController[DIC_X] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_X] = !m_bController[DIC_X];
			return true;
		}
		return false;
		break;
	case Engine::DIC_Y:
		if (m_bController[DIC_Y] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_Y] = !m_bController[DIC_Y];
			return true;
		}
		return false;
		break;
	case Engine::DIC_A:
		if (m_bController[DIC_A] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_A] = !m_bController[DIC_A];
			return true;
		}
		return false;
		break;
	case Engine::DIC_B:
		if (m_bController[DIC_B] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_B] = !m_bController[DIC_B];
			return true;
		}
		return false;
		break;
	case Engine::DIC_START:
		if (m_bController[DIC_START] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_START] = !m_bController[DIC_START];
			return true;
		}
		return false;
		break;
	case Engine::DIC_MENU:
		if (m_bController[DIC_MENU] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_MENU] = !m_bController[DIC_MENU];
			return true;
		}
		return false;
		break;
	case Engine::DIC_LS:
		if (m_bController[DIC_LS] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_LS] = !m_bController[DIC_LS];
			return true;
		}
		return false;
		break;
	case Engine::DIC_RS:
		if (m_bController[DIC_RS] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[ePad]))
		{
			m_bController[DIC_RS] = !m_bController[DIC_RS];
			return true;
		}
		return false;
		break;
	case Engine::DIC_END:
		break;
	default:
		break;
	}
	return false;
}

_bool CInputDev::Key_Pressing(MOUSEKEYSTATE eMouse)
{
	if (m_tMouseState.rgbButtons[eMouse] & 0x80)
		return true;

	return false;
}

_bool CInputDev::Key_Down(MOUSEKEYSTATE eMouse)
{
	if (!m_bMouse[eMouse] && (m_tMouseState.rgbButtons[eMouse] & 0x80))
	{
		m_bMouse[eMouse] = !m_bMouse[eMouse];
		return true;
	}
	return false;
}

_bool CInputDev::Key_Up(MOUSEKEYSTATE eMouse)
{
	if (m_bMouse[eMouse] && !(m_tMouseState.rgbButtons[eMouse] & 0x80))
	{
		m_bMouse[eMouse] = !m_bMouse[eMouse];
		return true;
	}
	return false;
}

void CInputDev::SetUp_Key()
{
	for (int i = 0; i < DIK_MAX; ++i)
	{
		if (m_bState[i] && !(m_byKeyState[i] & 0x80))
		{
			m_bState[i] = !m_bState[i];
			m_byKeyState[i] = true;
		}
		if (!m_bState[i] && (m_byKeyState[i] & 0x80))
		{
			m_bState[i] = !m_bState[i];
			m_byKeyState[i] = true;
		}
	}
}

void CInputDev::SetUp_Mouse()
{
	for (int i = 0; i < DIM_END; ++i)
	{
		if (m_bMouse[i] && !(m_tMouseState.rgbButtons[i] & 0x80))
			m_bMouse[i] = !m_bMouse[i];
		if (!m_bMouse[i] && (m_tMouseState.rgbButtons[i] & 0x80))
			m_bMouse[i] = !m_bMouse[i];
	}
}

_bool Engine::CInputDev::Check_Connection()
{
	int controllerId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			controllerId = i;
	}

	m_cID= controllerId;

	return controllerId != -1;
}

_bool Engine::CInputDev::SetUp_Pad()
{
	if (m_cID == -1)
		Check_Connection();

	if (m_cID != -1)
	{
		for (int i = 0; i < DIC_END; ++i)
		{
			if (m_bController[i] && !(m_tControllerState.Gamepad.wButtons & m_vecPadID[i]))
			{
				m_bController[i] = !m_bController[i];
			}
			if (!m_bController[i] && (m_tControllerState.Gamepad.wButtons & m_vecPadID[i]))
			{
				m_bController[i] = !m_bController[i];
			}
		}

		ZeroMemory(&m_tControllerState, sizeof(XINPUT_STATE));
		if (XInputGetState(m_cID, &m_tControllerState) != ERROR_SUCCESS)
		{
			m_cID = -1;
			return false;
		}

		float normLX = fmaxf(-1, (float)m_tControllerState.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)m_tControllerState.Gamepad.sThumbLY / 32767);

		leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
		leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

		if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

		float normRX = fmaxf(-1, (float)m_tControllerState.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)m_tControllerState.Gamepad.sThumbRY / 32767);

		rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
		rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

		if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);

		leftTrigger = (float)m_tControllerState.Gamepad.bLeftTrigger / 255;
		rightTrigger = (float)m_tControllerState.Gamepad.bRightTrigger / 255;

		return true;
	}
	return false;
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInputDev::Set_InputDev(void)
{
	for (int i = 0; i < DIK_MAX; ++i)
	{
		if (m_bState[i] && !(m_byKeyState[i] & 0x80))
			m_bState[i] = !m_bState[i];
		if (!m_bState[i] && (m_byKeyState[i] & 0x80))
			m_bState[i] = !m_bState[i];
	}

	for (int i = 0; i < DIM_END; ++i)
	{
		if (m_bMouse[i] && !(m_tMouseState.rgbButtons[i] & 0x80))
			m_bMouse[i] = !m_bMouse[i];
		if (!m_bMouse[i] && (m_tMouseState.rgbButtons[i] & 0x80))
			m_bMouse[i] = !m_bMouse[i];
	}
	SetUp_Pad();
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CInputDev::Free(void)
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

