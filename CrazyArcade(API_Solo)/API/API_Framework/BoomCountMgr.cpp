#include "stdafx.h"
#include "BoomCountMgr.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "BoomItem.h"

CBoomCountMgr*  CBoomCountMgr::m_pInstance = nullptr;

CBoomCountMgr::CBoomCountMgr()
{
}


CBoomCountMgr::~CBoomCountMgr()
{
}

void CBoomCountMgr::Initialize()
{
	Release();
}

void CBoomCountMgr::Update()
{
}

void CBoomCountMgr::Late_Update()
{
}

void CBoomCountMgr::Render(HDC _DC)
{
	for (auto& pPadoItem : m_vecPadoItem)
		pPadoItem->Render(_DC);
}

void CBoomCountMgr::Release()
{
}

void CBoomCountMgr::BoomCount_Item()
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

	m_vecPadoItem.emplace_back(CAbstractFactory<CBoomItem>::Create((float)fBlockX, (float)fBlockY));
}

void CBoomCountMgr::Save_BoomCountItem()
{
	HANDLE hFile = CreateFile(L"../Data/BoomCountItem.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pPadoItem : m_vecPadoItem)
	{
		WriteFile(hFile, &pPadoItem->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"폭탄 수 아이템 저장 성공", L"성공", MB_OK);
}

void CBoomCountMgr::Load_BoomCountItem()
{
	HANDLE hFile = CreateFile(L"../Data/BoomCountItem.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBoomItem>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BOOMITEM);
		m_vecPadoItem.emplace_back(pObj);
	}


	CloseHandle(hFile);


}

void CBoomCountMgr::Save_BoomCountItem2()
{
	HANDLE hFile = CreateFile(L"../Data/BoomCountItem2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pPadoItem : m_vecPadoItem)
	{
		WriteFile(hFile, &pPadoItem->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"폭탄 수 아이템 저장 성공", L"성공", MB_OK);
}

void CBoomCountMgr::Load_BoomCountItem2()
{
	HANDLE hFile = CreateFile(L"../Data/BoomCountItem2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBoomItem>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BOOMITEM);
		m_vecPadoItem.emplace_back(pObj);
	}


	CloseHandle(hFile);


}
