#include "stdafx.h"
#include "BlueHouseMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "BlueHouse.h"

CBlueHouseMgr*  CBlueHouseMgr::m_pInstance = nullptr;


CBlueHouseMgr::CBlueHouseMgr()
{
	
}


CBlueHouseMgr::~CBlueHouseMgr()
{
	Release();
}

void CBlueHouseMgr::Initialize()
{
}

void CBlueHouseMgr::Update()
{
}

void CBlueHouseMgr::Late_Update()
{
}

void CBlueHouseMgr::Render(HDC _DC)
{
	for (auto& pBlueHouse : m_vecBlueHouse)
		pBlueHouse->Render(_DC);
}

void CBlueHouseMgr::Release()
{
}

void CBlueHouseMgr::Blue_House()
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

	m_vecBlueHouse.emplace_back(CAbstractFactory<CBlueHouse>::Create((float)fBlockX, (float)fBlockY));
}

void CBlueHouseMgr::Save_Blue_House()
{
	HANDLE hFile = CreateFile(L"../Data/BlueHouse.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pBlueHouse : m_vecBlueHouse)
	{
		WriteFile(hFile, &pBlueHouse->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"파란 집 저장 성공", L"성공", MB_OK);

}

void CBlueHouseMgr::Load_Blue_House()
{
	HANDLE hFile = CreateFile(L"../Data/BlueHouse.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBlueHouse>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::HOUSE);
		m_vecBlueHouse.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CBlueHouseMgr::Save_Blue_House2()
{
	HANDLE hFile = CreateFile(L"../Data/BlueHouse2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pBlueHouse : m_vecBlueHouse)
	{
		WriteFile(hFile, &pBlueHouse->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"파란 집 저장 성공", L"성공", MB_OK);

}

void CBlueHouseMgr::Load_Blue_House2()
{
	HANDLE hFile = CreateFile(L"../Data/BlueHouse2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBlueHouse>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::HOUSE);
		m_vecBlueHouse.emplace_back(pObj);
	}


	CloseHandle(hFile);


}

