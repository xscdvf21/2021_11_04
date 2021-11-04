#include "stdafx.h"
#include "BlockMgr.h"
#include "TileBlock.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "YellowBlock.h"
#include "ObjMgr.h"

CBlockMgr*  CBlockMgr::m_pInstance = nullptr;


CBlockMgr::CBlockMgr()
{
	
}


CBlockMgr::~CBlockMgr()
{
	Release();
}

void CBlockMgr::Initialize()
{
}

void CBlockMgr::Update()
{

}

void CBlockMgr::Late_Update()
{
}

void CBlockMgr::Render(HDC _DC)
{

	//int iCullX = abs((int)(CScrollMgr::Get_Instance()->Get_ScrollX()) / TILECX);
	//int iCullY = 0;

	for (auto& pRedBlock : m_vecRedBlock)
		pRedBlock->Render(_DC);


}

void CBlockMgr::Release()
{
	for_each(m_vecRedBlock.begin(), m_vecRedBlock.end(), Safe_Delete<CObj*>);
	m_vecRedBlock.clear();

}

void CBlockMgr::Red_Block()
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

	
	m_vecRedBlock.emplace_back(CAbstractFactory<CTileBlock>::Create((float)fBlockX, (float)fBlockY));
	//m_vecBlock[iIdx]->Set_DrawID(_iDrawID);
}


void CBlockMgr::Save_RedBlock()
{
	HANDLE hFile = CreateFile(L"../Data/RedBlock.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pRedBlock : m_vecRedBlock)
	{
		WriteFile(hFile, &pRedBlock->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"빨간블럭 저장 성공", L"성공", MB_OK);
}


void CBlockMgr::Load_RedBlock()
{
	HANDLE hFile = CreateFile(L"../Data/RedBlock.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CTileBlock>::Create(Temp.fX, Temp.fY);

		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::REDBLOCK);
		m_vecRedBlock.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

void CBlockMgr::Save_RedBlock2()
{
	HANDLE hFile = CreateFile(L"../Data/RedBlock2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pRedBlock : m_vecRedBlock)
	{
		WriteFile(hFile, &pRedBlock->Get_Info(), sizeof(INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"빨간블럭 저장 성공", L"성공", MB_OK);
}


void CBlockMgr::Load_RedBlock2()
{
	HANDLE hFile = CreateFile(L"../Data/RedBlock2.dat", GENERIC_READ
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

		CObj*		pObj = CAbstractFactory<CTileBlock>::Create(Temp.fX, Temp.fY);

		CObjMgr::Get_Instance()->Add_Object(pObj, OBJID::REDBLOCK);
		m_vecRedBlock.emplace_back(pObj);
	}


	CloseHandle(hFile);

}



