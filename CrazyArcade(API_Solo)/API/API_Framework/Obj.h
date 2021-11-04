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
	void Set_BoomWave() { m_BoomWave = true; }  //파도에 물풍선 닿으면 터짐
	void Set_PlayerDeadTime() { m_PlayerDeadTime = true; }  //플레이어가 죽으면 물풍선에 갇히는 시간을위해 준비

	void Set_PadoAndBlock() { m_PadoAndBlock = true; } // 파도와 블럭이 충돌했을 때 파도를 그만 출력하기 위해,
	void Set_BoomAndPlayer() { m_BoomAndPlayer = true; } // 물폭탄과 플레이어가 충돌 시 물풍선을 못 설치하게 하기위해
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

	int		m_iBoomCount; //물풍선 갯수 판단.
	int		m_iBoomCountMax; //물풍선 최대 갯수.


	float	m_fSpeed;
	float	m_fAngle;

	bool	m_bDead;

	bool	m_bBoomCheck; // 물폭탄이 겹쳐있는지 판단하기 위해
	bool	m_SpeedItem; // 스피드 아이템 먹기판단
	bool	m_PadoItem; // 물폭탄 크기 아이템
	bool	m_BoomItem; // 물폭탄 아이템
	bool	m_BoomWave; //파도에 물풍선 닿으면 터짐

	bool	m_PlayerDeadTime; //플레이어가 죽으면 물풍선에 갇히는 시간을위해 준비
	bool	m_PadoAndBlock; // 파도와 블럭이 충돌했을 때 파도를 그만 출력하기 위해,
	bool	m_BoomAndPlayer; // 물폭탄과 플레이어가 충돌 시 물풍선을 못 설치하게 하기위해
	bool	m_bStart;
	bool	m_bPopo;

	TCHAR*	m_pFramekey;

	RENDERID::ID	m_eGroupID;

	CObj*	m_pTarget;
};


#endif // !__OBJ_H__
