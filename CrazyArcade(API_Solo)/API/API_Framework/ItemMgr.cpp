#include "stdafx.h"
#include "ItemMgr.h"
#include "Item.h"
#include "ScrollMgr.h"

CItemMgr*	CItemMgr::m_pInstance = nullptr;

CItemMgr::CItemMgr()
{
}


CItemMgr::~CItemMgr()
{
}

void CItemMgr::Initialize()
{

}

void CItemMgr::Update()
{
}

void CItemMgr::Late_Update()
{
}

void CItemMgr::Render(HDC _DC)
{
}

void CItemMgr::Release()
{
	for_each(m_vecItem.begin(), m_vecItem.end(), Safe_Delete<CObj*>);
	m_vecItem.clear();
}

void CItemMgr::Picking_Item()
{

	POINT		pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	int			iX = pt.x / TILECX;
	int			iY = pt.y / TILECY;


}

void CItemMgr::Save_Item()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;
	for (auto& pItem : m_vecItem)
	{
		WriteFile(hFile, &pItem->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &pItem->Get_DrawID(), sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CItemMgr::Load_Item()
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

		CObj*		pObj = CAbstractFactory<CItem>::Create(Temp.fX, Temp.fY);
		pObj->Set_DrawID(iDrawID);

		m_vecItem.emplace_back(pObj);
	}


	CloseHandle(hFile);
	MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);
}
