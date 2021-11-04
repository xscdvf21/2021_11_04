#include "stdafx.h"
#include "MyEditor.h"
#include "Line.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"


CMyEditor::CMyEditor()
	: m_bCheck(true), m_iLineRule(1)
{
}


CMyEditor::~CMyEditor()
{
	Release();
}

void CMyEditor::Initialize()
{
	m_DC = GetDC(g_hWnd);
}

void CMyEditor::Update()
{
	Key_Down();
}

void CMyEditor::Late_Update()
{
	CKeyMgr::Get_Instance()->Key_Update();
}

void CMyEditor::Render()
{
	Rectangle(m_DC, 0, 0, WINCX, WINCY);

	for (auto& pLine : m_listLine)
		pLine->Render(m_DC);
}

void CMyEditor::Release()
{
	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();

	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_DC);
}

void CMyEditor::Key_Down()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		Add_Line();

	if (CKeyMgr::Get_Instance()->Key_Down('A'))
		Save_Line();
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		Load_Line();


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
}

void CMyEditor::Add_Line()
{
	LONG iScrollX = (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= iScrollX;

	if (m_bCheck)
	{
		m_tPos[0].fX = (float)pt.x;
		m_tPos[0].fY = (float)pt.y;
		m_bCheck = false;
	}
	else
	{
		m_tPos[1].fX = (float)pt.x;
		m_tPos[1].fY = (float)pt.y;

		m_listLine.emplace_back(new CLine(m_tPos[0], m_tPos[1]));

		if (0 == m_iLineRule)
		{
			ZeroMemory(m_tPos, sizeof(m_tPos));
			m_bCheck = true;
		}
		else if (1 == m_iLineRule)
		{
			m_tPos[0] = m_tPos[1];
		}
	}
}

void CMyEditor::Save_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패!", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pLine : m_listLine)
	{
		WriteFile(hFile, &pLine->Get_Info(), sizeof(LINEINFO), &dwByte, NULL);
	}

	MessageBox(g_hWnd, L"저장 성공", L"성공!", MB_OK);
	CloseHandle(hFile);
}

void CMyEditor::Load_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패!", MB_OK);
		return;
	}

	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();

	DWORD		dwByte = 0;
	LINEINFO	tTemp = {};

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if (0 == dwByte)
			break;

		m_listLine.emplace_back(new CLine(tTemp.tLeftPos, tTemp.tRightPos));
	}


	MessageBox(g_hWnd, L"불러오기 성공", L"성공!", MB_OK);
	CloseHandle(hFile);
}
