#include "stdafx.h"
#include "TownBlockMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "TownBlock.h"

CTownBlockMgr*  CTownBlockMgr::m_pInstance = nullptr;

CTownBlockMgr::CTownBlockMgr()
{
}


CTownBlockMgr::~CTownBlockMgr()
{
	Release();
}

void CTownBlockMgr::Initialize()
{
}

void CTownBlockMgr::Update()
{
}

void CTownBlockMgr::Late_Update()
{
}

void CTownBlockMgr::Render(HDC _DC)
{
	for (auto& pTownBlock : m_vecTownBlock)
		pTownBlock->Render(_DC);
}

void CTownBlockMgr::Release()
{
}

void CTownBlockMgr::Town_Block()
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

	m_vecTownBlock.emplace_back(CAbstractFactory<CTownBlock>::Create((float)fBlockX, (float)fBlockY));

}

void CTownBlockMgr::Save_Town_Block()
{
	HANDLE hFile = CreateFile(L"../Data/TownBlock.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pTownBlock : m_vecTownBlock)
	{
		WriteFile(hFile, &pTownBlock->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"타운 블럭 저장 성공", L"성공", MB_OK);
}

void CTownBlockMgr::Load_Town_Block()
{
	HANDLE hFile = CreateFile(L"../Data/TownBlock.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CTownBlock>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::TOWNBLOCK);
		m_vecTownBlock.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CTownBlockMgr::Save_Town_Block2()
{
	HANDLE hFile = CreateFile(L"../Data/TownBlock2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pTownBlock : m_vecTownBlock)
	{
		WriteFile(hFile, &pTownBlock->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"타운 블럭 저장 성공", L"성공", MB_OK);
}

void CTownBlockMgr::Load_Town_Block2()
{
	HANDLE hFile = CreateFile(L"../Data/TownBlock2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CTownBlock>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::TOWNBLOCK);
		m_vecTownBlock.emplace_back(pObj);
	}


	CloseHandle(hFile);

}
