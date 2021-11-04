#include "stdafx.h"
#include "YellowHouseMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "YellowHouse.h"


CYellowHouseMgr*  CYellowHouseMgr::m_pInstance = nullptr;

CYellowHouseMgr::CYellowHouseMgr()
{
}


CYellowHouseMgr::~CYellowHouseMgr()
{
	Release();
}

void CYellowHouseMgr::Initialize()
{
}

void CYellowHouseMgr::Update()
{

}

void CYellowHouseMgr::Late_Update()
{
}

void CYellowHouseMgr::Render(HDC _DC)
{
	for (auto& pYellowHouse : m_vecYellowHouse)
		pYellowHouse->Render(_DC);
}

void CYellowHouseMgr::Release()
{
}

void CYellowHouseMgr::Yellow_House()
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

	m_vecYellowHouse.emplace_back(CAbstractFactory<CYellowHouse>::Create((float)fBlockX, (float)fBlockY));
}

void CYellowHouseMgr::Save_Yellow_House()
{
	HANDLE hFile = CreateFile(L"../Data/YellowHouse.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pYellowHouse : m_vecYellowHouse)
	{
		WriteFile(hFile, &pYellowHouse->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"노란 집 저장 성공", L"성공", MB_OK);
}

void CYellowHouseMgr::Load_Yellow_House()
{
	HANDLE hFile = CreateFile(L"../Data/YellowHouse.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CYellowHouse>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::HOUSE);
		m_vecYellowHouse.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
void CYellowHouseMgr::Save_Yellow_House2()
{
	HANDLE hFile = CreateFile(L"../Data/YellowHouse2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pYellowHouse : m_vecYellowHouse)
	{
		WriteFile(hFile, &pYellowHouse->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"노란 집 저장 성공", L"성공", MB_OK);
}

void CYellowHouseMgr::Load_Yellow_House2()
{
	HANDLE hFile = CreateFile(L"../Data/YellowHouse2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CYellowHouse>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::HOUSE);
		m_vecYellowHouse.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
