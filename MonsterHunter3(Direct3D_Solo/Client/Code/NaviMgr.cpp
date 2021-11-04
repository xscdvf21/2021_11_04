#include "stdafx.h"
#include "NaviMgr.h"

#include "Export_Fucntion.h"

IMPLEMENT_SINGLETON(CNaviMgr)

CNaviMgr::CNaviMgr()
{
}


CNaviMgr::~CNaviMgr()
{
	Free();
}

HRESULT CNaviMgr::Add_NaviMesh(_int iStageIdx, _uint iType, CNaviMesh * pNaviMesh)
{
	if (nullptr == pNaviMesh || iStageIdx >= RESOURCE_END)
		return E_FAIL;

	auto iter = m_mapNaviList.find(iStageIdx);
	if (m_mapNaviList.end() == iter)
	{
		map<_uint, CNaviMesh*> mapNaviMesh;
		mapNaviMesh.emplace(iType, pNaviMesh);

		m_mapNaviList.emplace(iStageIdx, mapNaviMesh);
	}
	else
	{
		iter->second.emplace(iType, pNaviMesh);
	}

	return S_OK;
}

CNaviMesh * CNaviMgr::Get_NaviMesh(_int iStageIdx, _uint iType)
{
	if (iStageIdx >= RESOURCE_END || iType >= NAVI_END)
		return nullptr;

	auto& iter = m_mapNaviList.find(iStageIdx);
	if (m_mapNaviList.end() == iter)
		return nullptr;

	auto& iter_Navi = iter->second.find(iType);
	if (iter->second.end() == iter_Navi)
		return nullptr;

	return iter_Navi->second;
}

void CNaviMgr::Free()
{
	for (auto& pair_map : m_mapNaviList)
	{
		for (auto& pair_Navi : pair_map.second)
		{
			Safe_Release(pair_Navi.second);
		}
		pair_map.second.clear();
	}
	m_mapNaviList.clear();
}
