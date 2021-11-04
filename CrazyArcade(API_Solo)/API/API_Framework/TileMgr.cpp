#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"
#include "TileBlock.h"
#include "ScrollMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;
CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)((TILECX >> 1) + (j * TILECX));
			float fY = (float)((TILECY >> 1) + (i * TILECY));

    			m_vecTile.emplace_back(CAbstractFactory<CTile>::Create(fX+0, fY+20));
			
		}
	}
}

void CTileMgr::Update()
{
}

void CTileMgr::Late_Update()
{
}

void CTileMgr::Render(HDC _DC)
{
	int iCullX = abs((int)(CScrollMgr::Get_Instance()->Get_ScrollX()) / TILECX);
	int iCullY = 0;

	int iCullEndX = iCullX + (WINCX / TILECX) -4;
	int iCullEndY = iCullY + (WINCY / TILECY) ;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int iIdx = i * TILEX + j;

			if(0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
				continue;

			m_vecTile[iIdx]->Render(_DC);
			//m_vecTileBlock[iIdx]->Render(_DC);
		}
	}

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();

	//for_each(m_vecTileBlock.begin(), m_vecTileBlock.end(), Safe_Delete<CObj*>);
	//m_vecTileBlock.clear();

}

void CTileMgr::Picking_Tile(int _iDrawID)
{
	POINT	pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	int		iX = pt.x / TILECX;
	int		iY = pt.y / TILECY;
	int iIdx = iY * TILEX + iX; //몇번째 타일인지 구하는것

	m_vecTile[iIdx]->Set_DrawID(_iDrawID);
}


void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pTile : m_vecTile)
	{
		WriteFile(hFile, &pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &pTile->Get_DrawID(), sizeof(int), &dwByte, NULL);
	} 

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Release();

	DWORD	dwByte = 0;
	INFO	Temp = {};
	int		iDrawID = 0;

	while (true)
	{
		ReadFile(hFile, &Temp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObj*		pObj = CAbstractFactory<CTile>::Create(Temp.fX, Temp.fY);
		pObj->Set_DrawID(iDrawID);

		m_vecTile.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

CObj* CTileMgr::Get_Index(float _x , float _y)
{

	int		iX = (int)_x / TILECX;
	int		iY = (int)_y / TILECY;
	int iIdx = iY * TILEX + iX; //몇번째 타일인지 구하는것

	return m_vecTile[iIdx];
}

void CTileMgr::Save_Tile2()
{
	HANDLE hFile = CreateFile(L"../Data/Tile2.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pTile : m_vecTile)
	{
		WriteFile(hFile, &pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &pTile->Get_DrawID(), sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CTileMgr::Load_Tile2()
{
	HANDLE hFile = CreateFile(L"../Data/Tile2.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Release();

	DWORD	dwByte = 0;
	INFO	Temp = {};
	int		iDrawID = 0;

	while (true)
	{
		ReadFile(hFile, &Temp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObj*		pObj = CAbstractFactory<CTile>::Create(Temp.fX, Temp.fY);
		pObj->Set_DrawID(iDrawID);

		m_vecTile.emplace_back(pObj);
	}


	CloseHandle(hFile);

}

