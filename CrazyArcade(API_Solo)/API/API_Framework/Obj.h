#pragma once

#ifndef __OBJ_H__
#define __OBJ_H__


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_Dead() { m_bDead = true; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; }
	void Set_FrameKey(TCHAR* _pFrameKey) { m_pFramekey = _pFrameKey; }
	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void Set_Popo(bool _ibPopo) { m_bPopo = _ibPopo; }
	/*void Set_BoomCount(int _iBoomCount) { m_iBoomCount = _iBoomCount; }*/

	void Set_BoolSpeed() { m_SpeedItem = true; }
	void Set_BoolPado() { m_PadoItem = true; }
	void Set_BoolBoom() { m_BoomItem = true; }
	void Set_BoomCheck() { m_bBoomCheck = true; }
	void Set_BoomWave() { m_BoomWave = true; }  //�ĵ��� ��ǳ�� ������ ����
	void Set_PlayerDeadTime() { m_PlayerDeadTime = true; }  //�÷��̾ ������ ��ǳ���� ������ �ð������� �غ�

	void Set_PadoAndBlock() { m_PadoAndBlock = true; } // �ĵ��� ���� �浹���� �� �ĵ��� �׸� ����ϱ� ����,
	void Set_BoomAndPlayer() { m_BoomAndPlayer = true; } // ����ź�� �÷��̾ �浹 �� ��ǳ���� �� ��ġ�ϰ� �ϱ�����
	/*bool Get_BoomCheck() { return m_BoomCount; }*/

public:
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_Info() const { return m_tInfo; }
	const int& Get_DrawID() const { return m_iDrawID; }
	const RENDERID::ID& Get_GroupID() const { return m_eGroupID; }

	/*const bool& Get_BoomCount() const { return m_BoomCount; }*/

protected:
	void Update_Rect();
	void Update_Frame();

protected:
	


	INFO	m_tInfo;
	RECT	m_tRect;
	FRAME	m_tFrame;

	int		m_iDrawID;

	int		m_iBoomCount; //��ǳ�� ���� �Ǵ�.
	int		m_iBoomCountMax; //��ǳ�� �ִ� ����.


	float	m_fSpeed;
	float	m_fAngle;

	bool	m_bDead;

	bool	m_bBoomCheck; // ����ź�� �����ִ��� �Ǵ��ϱ� ����
	bool	m_SpeedItem; // ���ǵ� ������ �Ա��Ǵ�
	bool	m_PadoItem; // ����ź ũ�� ������
	bool	m_BoomItem; // ����ź ������
	bool	m_BoomWave; //�ĵ��� ��ǳ�� ������ ����

	bool	m_PlayerDeadTime; //�÷��̾ ������ ��ǳ���� ������ �ð������� �غ�
	bool	m_PadoAndBlock; // �ĵ��� ���� �浹���� �� �ĵ��� �׸� ����ϱ� ����,
	bool	m_BoomAndPlayer; // ����ź�� �÷��̾ �浹 �� ��ǳ���� �� ��ġ�ϰ� �ϱ�����
	bool	m_bStart;
	bool	m_bPopo;

	TCHAR*	m_pFramekey;

	RENDERID::ID	m_eGroupID;

	CObj*	m_pTarget;
};


#endif // !__OBJ_H__
