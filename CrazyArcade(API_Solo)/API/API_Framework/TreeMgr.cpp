#include "stdafx.h"
#include "TreeMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "Tree.h"

CTreeMgr*  CTreeMgr::m_pInstance = nullptr;

CTreeMgr::CTreeMgr()
{
}


CTreeMgr::~CTreeMgr()
{
	Release();
}

void CTreeMgr::Initialize()
{
}

void CTreeMgr::Update()
{
}

void CTreeMgr::Late_Update()
{
}

void CTreeMgr::Render(HDC _DC)
{
	for (auto& pTree : m_vecTree)
		pTree->Render(_DC);
}

void CTreeMgr::Release()
{
}

void CTreeMgr::Tree()
{
	POINT	pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	CObj*	pTemp = CTileMgr::Get_Instance()->Get_Index((float)pt.x, (float)pt.y);

	float	fBlockX = pTemp->Get_Info().fX;
	float	fBlockY = pTemp->Get_Info().fY;

	int		iX = pt.x / TILECX;
	int		iY = pt.y / TILECY;
	//int iIdx = iY * TILEX + iX; //몇번째 타일인지 구하는것

	m_vecTree.emplace_back(CAbstractFactory<CTree>::Create((float)fBlockX, (float)fBlockY));
}

void CTreeMgr::Save_Tree()
{
	HANDLE hFile = CreateFile(L"../Data/Tree.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pTree : m_vecTree)
	{
		WriteFile(hFile, &pTree->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"나무 저장 성공", L"성공", MB_OK);
}

void CTreeMgr::Load_Tree()
{
	HANDLE hFile = CreateFile(L"../Data/Tree.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Release();

	DWORD	dwByte = 0;
	INFO	Temp = {};

	while (true)
	{
		ReadFile(hFile, &Temp, sizeof(INFO), &dwByte, NULL);


		if (0 == dwByte)
			break;

		CObj*		pObj = CAbstractFactory<CTree>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::TREE);
		m_vecTree.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CTreeMgr::Save_Tree2()
{
	HANDLE hFile = CreateFile(L"../Data/Tree2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pTree : m_vecTree)
	{
		WriteFile(hFile, &pTree->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"나무 저장 성공", L"성공", MB_OK);
}

void CTreeMgr::Load_Tree2()
{
	HANDLE hFile = CreateFile(L"../Data/Tree2.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Release();

	DWORD	dwByte = 0;
	INFO	Temp = {};

	while (true)
	{
		ReadFile(hFile, &Temp, sizeof(INFO), &dwByte, NULL);


		if (0 == dwByte)
			break;

		CObj*		pObj = CAbstractFactory<CTree>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::TREE);
		m_vecTree.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
