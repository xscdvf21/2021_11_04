#pragma once

#ifndef __MYEDITOR_H__
#define __MYEDITOR_H__

class CLine;
class CMyEditor
{
public:
	CMyEditor();
	~CMyEditor();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	void Key_Down();
	void Add_Line();
	void Save_Line();
	void Load_Line();

private:
	list<CLine*>	m_listLine;
	HDC				m_DC;
	LINEPOS			m_tPos[2];
	bool			m_bCheck;
	int				m_iLineRule;
};


#endif // !__MYEDITOR_H__
