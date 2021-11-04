#include "stdafx.h"
#include "BmpMgr.h"
#include "MyBitmap.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR* _pFilePath, const TCHAR* _pFrameKey)
{
	auto& iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStrCmp(_pFrameKey));

	if (m_mapBit.end() == iter)
	{
		CMyBitmap* pBmp = new CMyBitmap;
		pBmp->Load_Bmp(_pFilePath);

		m_mapBit.emplace(_pFrameKey, pBmp);
	}
}

HDC CBmpMgr::Find_Bmp(const TCHAR* _pFrameKey)
{
	auto& iter = find_if(m_mapBit.begin(), m_mapBit.end(), CStrCmp(_pFrameKey));

	if (m_mapBit.end() == iter)
		return nullptr;

	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for (auto& pair : m_mapBit)
		SAFE_DELETE(pair.second);
	m_mapBit.clear();
}
