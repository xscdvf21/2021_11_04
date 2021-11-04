#pragma once
#ifndef __KEY_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

//사용할 키에 따라 변수명 바꿔서 사용
//기본 조작 키
const _uint KEY_UP = 0x00000001;		/* 00000000 00000000 00000000 00000001 */
const _uint KEY_DOWN = 0x00000002;		/* 00000000 00000000 00000000 00000010 */
const _uint KEY_LEFT = 0x00000004;		/* 00000000 00000000 00000000 00000100 */
const _uint KEY_RIGHT = 0x00000008;		/* 00000000 00000000 00000000 00001000 */

const _uint KEY_Z = 0x00000010;		/* 00000000 00000000 00000000 00010000 */
const _uint KEY_X = 0x00000020;		/* 00000000 00000000 00000000 00100000 */
const _uint KEY_C = 0x00000040;		/* 00000000 00000000 00000000 01000000 */
const _uint KEY_A = 0x00000080;		/* 00000000 00000000 00000000 10000000 */
const _uint KEY_S = 0x00000100;		/* 00000000 00000000 00000001 00000000 */
const _uint KEY_D = 0x00000200;		/* 00000000 00000000 00000010 00000000 */
const _uint KEY_Q = 0x00000400;		/* 00000000 00000000 00000100 00000000 */
const _uint KEY_W = 0x00000800;		/* 00000000 00000000 00001000 00000000 */
const _uint KEY_E = 0x00001000;		/* 00000000 00000000 00010000 00000000 */
const _uint KEY_R = 0x00002000;		/* 00000000 00000000 00100000 00000000 */


//마우스 버튼
const _uint KEY_LMBUTTON = 0x00004000;
const _uint KEY_RMBUTTON = 0x00008000;

const _uint KEY_ENTER = 0x00010000;
const _uint KEY_SPACE = 0x00020000;
//디버그 및 테스트용

const _uint KEY_ADD = 0x01000000;
const _uint KEY_MIN = 0x02000000;

const _uint KEY_NUM2 = 0x04000000;
const _uint KEY_NUM8 = 0x08000000;

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)

private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	void Update_KeyManager();
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);


public:
	virtual void Free() override;

private:
	_uint m_iKeyDown = 0;
	_uint m_iKeyUp = 0;
	_uint m_iCurrentKey = 0;
};
END

#define __KEY_MANAGER_H__
#endif