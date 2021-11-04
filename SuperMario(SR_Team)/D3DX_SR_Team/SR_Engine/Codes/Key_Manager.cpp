#include "..\Headers\Key_Manager.h"

/**
	�⺻ Ű ���� :
	����Ű : �⺻ �̵�
	Z : ���� / Ư�� ��ȣ�ۿ� (���̼� ��)
	X : ��ġ��, ����
	C : ����, �⺻ ��ȣ�ۿ�, ������/���
	A : ���� �κ��丮
	S :
	D : ���� �Ѹ���
	Q : L��ư (������Ʈ Ÿ�� ��ȯ)
	E : R��ư (������Ʈ Ÿ�� ��ȯ)

	R : + �޴� (������, ���� ��)

	���콺 : ���̼� ���� (������ Ŭ��)

	���� ��Ʈ :
	����Ű : �� ������
	Z :
	X : ����(�� ����, ������ ���� ����), Ÿ�̹� ����
	C : ���� (��������)
	A : ��Ʈ - �ʿ��Ѱ�?
	S : ���� - �ʿ��Ѱ�?
	D : �κ��丮
	R : ���� ��� �ð� ä���
	Q / E : ���� / ������ ��ȯ
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

/* ��� ������ �ִ� ��� */
_bool CKey_Manager::Key_Pressing(_uint iKey)
{
	if (m_iCurrentKey & iKey)
		return true;

	return false;
}

/* ���� �����ӿ��� �ѹ��� ���� ��� */
_bool CKey_Manager::Key_Down(_uint iKey)
{
	/* ������ ������ ���� ���� ������ �� true */
	if (!(m_iKeyDown & iKey) && (m_iCurrentKey & iKey))
	{
		m_iKeyDown |= iKey;
		return true;
	}

	/* ���� �����ӿ��� m_iKeyDown ���� */
	if ((m_iKeyDown & iKey) && !(m_iCurrentKey & iKey))
	{
		/*
		m_iKeyDown	= 1111
		iKey		= 0010
		xor�� ���	= 1101
		*/
		m_iKeyDown ^= iKey;

		return false;
	}

	return false;
}

/* ���� �����ӿ��� Ű�� �� ��� */
_bool CKey_Manager::Key_Up(_uint iKey)
{
	/* ������ ���Ⱦ��� ���� ������ ���� ��� */
	if ((m_iKeyUp & iKey) && !(m_iCurrentKey & iKey))
	{
		m_iKeyUp ^= iKey;

		return true;
	}

	/* ������ ������ �ʾҰ� ���� ���� ��� */
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
