#include "stdafx.h"
#include "RedHouseMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "RedHouse.h"


CRedHouseMgr*  CRedHouseMgr::m_pInstance = nullptr;

CRedHouseMgr::CRedHouseMgr()
{
}


CRedHouseMgr::~CRedHouseMgr()
{
}

void CRedHouseMgr::Initialize()
{
}

void CRedHouseMgr::Update()
{
}

void CRedHouseMgr::Late_Update()
{
}

void CRedHouseMgr::Render(HDC _DC)
{
	for (auto& pRedHouse : m_vecRedHouse)
		pRedHouse->Render(_DC);
}

void CRedHouseMgr::Release()
{
}

void CRedHouseMgr::Red_House()
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

	m_vecRedHouse.emplace_back(CAbstractFactory<CRedHouse>::Create((float)fBlockX, (float)fBlockY));
}

void CRedHouseMgr::Save_Red_House()
{
	HANDLE hFile = CreateFile(L"../Data/RedHouse.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pRedHouse : m_vecRedHouse)
	{
		WriteFile(hFile, &pRedHouse->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"빨간 집 저장 성공", L"성공", MB_OK);
}

void CRedHouseMgr::Load_Red_House()
{
	HANDLE hFile = CreateFile(L"../Data/RedHouse.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CRedHouse>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::HOUSE);
		m_vecRedHouse.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CRedHouseMgr::Save_Red_House2()
{
	HANDLE hFile = CreateFile(L"../Data/RedHouse2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pRedHouse : m_vecRedHouse)
	{
		WriteFile(hFile, &pRedHouse->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"빨간 집 저장 성공", L"성공", MB_OK);
}

void CRedHouseMgr::Load_Red_House2()
{
	HANDLE hFile = CreateFile(L"../Data/RedHouse2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CRedHouse>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::HOUSE);
		m_vecRedHouse.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
