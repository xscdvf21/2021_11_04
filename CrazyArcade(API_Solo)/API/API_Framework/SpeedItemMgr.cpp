#include "stdafx.h"
#include "SpeedItemMgr.h"
#include "Item.h"
#include "ObjMgr.h"
#include "TileMgr.h"

CSpeedItemMgr*  CSpeedItemMgr::m_pInstance = nullptr;


CSpeedItemMgr::CSpeedItemMgr()
{
}


CSpeedItemMgr::~CSpeedItemMgr()
{
	Release();
}

void CSpeedItemMgr::Initialize()
{
}

void CSpeedItemMgr::Update()
{
}

void CSpeedItemMgr::Late_Update()
{
}

void CSpeedItemMgr::Render(HDC _DC)
{
	for (auto& pSpeedItem : m_vecSpeedItem)
		pSpeedItem->Render(_DC);
}

void CSpeedItemMgr::Release()
{
}

void CSpeedItemMgr::Speed_Item()
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

	m_vecSpeedItem.emplace_back(CAbstractFactory<CItem>::Create((float)fBlockX, (float)fBlockY));
}

void CSpeedItemMgr::Save_SpeedItem()
{
	HANDLE hFile = CreateFile(L"../Data/SpeedItem.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pSpeedItem : m_vecSpeedItem)
	{
		WriteFile(hFile, &pSpeedItem->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"스피드 아이템 저장 성공", L"성공", MB_OK);
}

void CSpeedItemMgr::Load_SpeedItem()
{
	HANDLE hFile = CreateFile(L"../Data/SpeedItem.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CItem>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SPEEDITEM);
		m_vecSpeedItem.emplace_back(pObj);
	}


	CloseHandle(hFile);
	
}

void CSpeedItemMgr::Save_SpeedItem2()
{
	HANDLE hFile = CreateFile(L"../Data/SpeedItem2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pSpeedItem : m_vecSpeedItem)
	{
		WriteFile(hFile, &pSpeedItem->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"스피드 아이템 저장 성공", L"성공", MB_OK);
}

void CSpeedItemMgr::Load_SpeedItem2()
{
	HANDLE hFile = CreateFile(L"../Data/SpeedItem2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CItem>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::SPEEDITEM);
		m_vecSpeedItem.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

