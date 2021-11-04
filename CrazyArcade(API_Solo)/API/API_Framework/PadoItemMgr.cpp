#include "stdafx.h"
#include "PadoItemMgr.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "BoomSizeItem.h"

CPadoItemMgr*  CPadoItemMgr::m_pInstance = nullptr;

CPadoItemMgr::CPadoItemMgr()
{
}


CPadoItemMgr::~CPadoItemMgr()
{
}

void CPadoItemMgr::Initialize()
{
}

void CPadoItemMgr::Update()
{
}

void CPadoItemMgr::Late_Update()
{
}

void CPadoItemMgr::Render(HDC _DC)
{
	for (auto& pPadoItem : m_vecPadoItem)
		pPadoItem->Render(_DC);
}

void CPadoItemMgr::Release()
{
}

void CPadoItemMgr::Pado_Item()
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

	m_vecPadoItem.emplace_back(CAbstractFactory<CBoomSizeItem>::Create((float)fBlockX, (float)fBlockY));
}

void CPadoItemMgr::Save_PadoItem()
{
	HANDLE hFile = CreateFile(L"../Data/BoomSize.dat", GENERIC_WRITE
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
	MessageBox(g_hWnd, L"파도 아이템 저장 성공", L"성공", MB_OK);
}

void CPadoItemMgr::Load_PadoItem()
{
	HANDLE hFile = CreateFile(L"../Data/BoomSize.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBoomSizeItem>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BOOMSIZEITEM);
		m_vecPadoItem.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CPadoItemMgr::Save_PadoItem2()
{
	HANDLE hFile = CreateFile(L"../Data/BoomSize2.dat", GENERIC_WRITE
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
	MessageBox(g_hWnd, L"파도 아이템 저장 성공", L"성공", MB_OK);
}

void CPadoItemMgr::Load_PadoItem2()
{
	HANDLE hFile = CreateFile(L"../Data/BoomSize2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CBoomSizeItem>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::BOOMSIZEITEM);
		m_vecPadoItem.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
