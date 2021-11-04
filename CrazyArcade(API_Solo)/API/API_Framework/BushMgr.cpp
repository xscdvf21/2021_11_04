#include "stdafx.h"
#include "BushMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "Bush.h"

CBushMgr*	CBushMgr::m_pInstance = nullptr;

CBushMgr::CBushMgr()
{
}


CBushMgr::~CBushMgr()
{
	Release();
}

void CBushMgr::Initialize()
{
}

void CBushMgr::Update()
{
}

void CBushMgr::Late_Update()
{
}

void CBushMgr::Render(HDC _DC)
{
	for (auto& pBush : m_vecBush)
		pBush->Render(_DC);
}

void CBushMgr::Release()
{
}

void CBushMgr::Bush()
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

	m_vecBush.emplace_back(CAbstractFactory<CBush>::Create((float)fBlockX, (float)fBlockY));
}

void CBushMgr::Save_Bush()
{
	HANDLE hFile = CreateFile(L"../Data/Bush.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pBush : m_vecBush)
	{
		WriteFile(hFile, &pBush->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"부쉬 저장 성공", L"성공", MB_OK);
}

void CBushMgr::Load_Bush()
{
	HANDLE hFile = CreateFile(L"../Data/Bush.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBush>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BUSH);
		m_vecBush.emplace_back(pObj);
	}


	CloseHandle(hFile);
	
}

void CBushMgr::Save_Bush2()
{
	HANDLE hFile = CreateFile(L"../Data/Bush2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pBush : m_vecBush)
	{
		WriteFile(hFile, &pBush->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"부쉬 저장 성공", L"성공", MB_OK);
}

void CBushMgr::Load_Bush2()
{
	HANDLE hFile = CreateFile(L"../Data/Bush2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBush>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BUSH);
		m_vecBush.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
