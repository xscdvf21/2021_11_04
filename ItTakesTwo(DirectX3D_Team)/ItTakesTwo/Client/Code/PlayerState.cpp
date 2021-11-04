#include "stdafx.h"
#include "PlayerState.h"

#include "Export_Function.h"

CPlayerState::CPlayerState()
{
}

void CPlayerState::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		m_iKeyState |= MOVE_UP;
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		m_iKeyState |= MOVE_DOWN;
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		m_iKeyState |= MOVE_LEFT;
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		m_iKeyState |= MOVE_RIGHT;
}
