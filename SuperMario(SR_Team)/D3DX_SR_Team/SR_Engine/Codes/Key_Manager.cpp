#include "..\Headers\Key_Manager.h"

/**
	기본 키 세팅 :
	십자키 : 기본 이동
	Z : 도움말 / 특수 상호작용 (종이손 등)
	X : 망치질, 선택
	C : 점프, 기본 상호작용, 나가기/취소
	A : 빠른 인벤토리
	S :
	D : 종이 뿌리기
	Q : L버튼 (전투파트 타입 전환)
	E : R버튼 (전투파트 타입 전환)

	R : + 메뉴 (아이템, 지도 등)

	마우스 : 종이손 조작 (선택은 클릭)

	전투 파트 :
	십자키 : 링 돌리기
	Z :
	X : 선택(링 선택, 아이템 선택 포함), 타이밍 어택
	C : 포기 (도망가기)
	A : 힌트 - 필요한가?
	S : 응원 - 필요한가?
	D : 인벤토리
	R : 동전 사용 시간 채우기
	Q / E : 무기 / 아이템 전환
*/

USING(Engine)
IMPLEMENT_SINGLETON(CKey_Manager)

CKey_Manager::CKey_Manager()
{
}

void CKey_Manager::Update_KeyManager()
{
	m_iCurrentKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_iCurrentKey |= KEY_UP;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_iCurrentKey |= KEY_DOWN;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_iCurrentKey |= KEY_LEFT;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_iCurrentKey |= KEY_RIGHT;
	}

	if (GetAsyncKeyState('Z') & 0x8000)
	{
		m_iCurrentKey |= KEY_Z;
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_iCurrentKey |= KEY_X;
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		m_iCurrentKey |= KEY_C;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_iCurrentKey |= KEY_A;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_iCurrentKey |= KEY_S;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_iCurrentKey |= KEY_D;
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_iCurrentKey |= KEY_Q;
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_iCurrentKey |= KEY_W;
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_iCurrentKey |= KEY_E;
	}

	if (GetAsyncKeyState('R') & 0x8000)
	{
		m_iCurrentKey |= KEY_R;
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_iCurrentKey |= KEY_ENTER;
	}

	if (GetAsyncKeyState(VK_ADD) & 0x8000)
	{
		m_iCurrentKey |= KEY_ADD;
	}

	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
	{
		m_iCurrentKey |= KEY_MIN;
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		m_iCurrentKey |= KEY_NUM2;
	}

	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		m_iCurrentKey |= KEY_NUM8;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_iCurrentKey |= KEY_SPACE;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_iCurrentKey |= KEY_LMBUTTON;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_iCurrentKey |= KEY_RMBUTTON;
	}

}

/* 계속 누르고 있는 경우 */
_bool CKey_Manager::Key_Pressing(_uint iKey)
{
	if (m_iCurrentKey & iKey)
		return true;

	return false;
}

/* 현재 프레임에서 한번만 누른 경우 */
_bool CKey_Manager::Key_Down(_uint iKey)
{
	/* 이전에 누른적 없고 현재 눌렀을 때 true */
	if (!(m_iKeyDown & iKey) && (m_iCurrentKey & iKey))
	{
		m_iKeyDown |= iKey;
		return true;
	}

	/* 다음 프레임에서 m_iKeyDown 원복 */
	if ((m_iKeyDown & iKey) && !(m_iCurrentKey & iKey))
	{
		/*
		m_iKeyDown	= 1111
		iKey		= 0010
		xor의 결과	= 1101
		*/
		m_iKeyDown ^= iKey;

		return false;
	}

	return false;
}

/* 현재 프레임에서 키를 뗄 경우 */
_bool CKey_Manager::Key_Up(_uint iKey)
{
	/* 이전에 눌렸었고 현재 누르지 않은 경우 */
	if ((m_iKeyUp & iKey) && !(m_iCurrentKey & iKey))
	{
		m_iKeyUp ^= iKey;

		return true;
	}

	/* 이전에 누르지 않았고 현재 누른 경우 */
	if (!(m_iKeyUp & iKey) && (m_iCurrentKey & iKey))
	{
		m_iKeyUp |= iKey;

		return false;
	}

	return false;
}

void CKey_Manager::Free()
{
	//CBase::Free();
}
