#include "stdafx.h"
#include "YellowBlockMgr.h"
#include "YellowBlock.h"
#include "TileMgr.h"
#include "ObjMgr.h"

CYellowBlockMgr*  CYellowBlockMgr::m_pInstance = nullptr;

CYellowBlockMgr::CYellowBlockMgr()
{
}


CYellowBlockMgr::~CYellowBlockMgr()
{
	Release();
}

void CYellowBlockMgr::Initialize()
{
}

void CYellowBlockMgr::Update()
{
}

void CYellowBlockMgr::Late_Update()
{
}

void CYellowBlockMgr::Render(HDC _DC)
{
	for (auto& pYellowBlock : m_vecYellowBlock)
		pYellowBlock->Render(_DC);
}

void CYellowBlockMgr::Release()
{
}

void CYellowBlockMgr::Yellow_Block()
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

	m_vecYellowBlock.emplace_back(CAbstractFactory<CYellowBlock>::Create((float)fBlockX, (float)fBlockY));
}

void CYellowBlockMgr::Save_YellowBlock()
{
	HANDLE hFile = CreateFile(L"../Data/YellowBlock.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pYellowBlock : m_vecYellowBlock)
	{
		WriteFile(hFile, &pYellowBlock->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"노란블럭 저장 성공", L"성공", MB_OK);
}

void CYellowBlockMgr::Load_YellowBlock()
{
	HANDLE hFile = CreateFile(L"../Data/YellowBlock.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CYellowBlock>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::YELLOWBLOCK);
		m_vecYellowBlock.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CYellowBlockMgr::Save_YellowBlock2()
{
	HANDLE hFile = CreateFile(L"../Data/YellowBlock2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pYellowBlock : m_vecYellowBlock)
	{
		WriteFile(hFile, &pYellowBlock->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"노란블럭 저장 성공", L"성공", MB_OK);
}

void CYellowBlockMgr::Load_YellowBlock2()
{
	HANDLE hFile = CreateFile(L"../Data/YellowBlock2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CYellowBlock>::Create(Temp.fX, Temp.fY);
		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::YELLOWBLOCK);
		m_vecYellowBlock.emplace_back(pObj);
	}


	CloseHandle(hFile);
	
}

