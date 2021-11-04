#include "stdafx.h"
#include "Astar_Manager.h"
#include "Terrain.h"
#include "Tile.h"
#include "GameObject_Manger.h"

IMPLEMENT_SINGLETON(CAstar_Manager)

CAstar_Manager::CAstar_Manager()
	:m_iStartIndex(0)
	,m_bAStarMonsterCheck(false)
{
}


CAstar_Manager::~CAstar_Manager()
{
}

bool CAstar_Manager::CheckOpenList(const int iIndex)
{
	auto& iterFind = find(m_OpenList.begin(), m_OpenList.end(), iIndex);
	if (iterFind == m_OpenList.end())
		return false;

	return true;
}

bool CAstar_Manager::CheckCloseList(const int iIndex)
{
	auto& iterFind = find(m_CloseList.begin(), m_CloseList.end(), iIndex);
	if (iterFind == m_CloseList.end())
		return false;

	return true;
}

// 이시키가 실질적인 로직이 돌아갈 공간. 재귀함수 돌릴것. 
bool CAstar_Manager::PathFinding(int iStartIndex, int iGoalIndex)
{
	CGameObject* pTerrain = CGameObject_Manger::Get_Instance()->Get_Tile();
	vector<GROUND*>& vecTile = dynamic_cast<CTile*>(pTerrain)->Get_Tile();
	const vector<list<GROUND*>>&  vecGraph = dynamic_cast<CTile*>(pTerrain)->Get_Graph(); // 병건이의 ASMR 수업을 시작할께요.

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIndex);

	for (auto& pTile : vecGraph[iStartIndex])
	{
		if (iGoalIndex == int(pTile->dwIndex))
		{
			pTile->dwParentIndex = iStartIndex;
			return true;
		}
		if (CheckCloseList(pTile->dwIndex) || CheckOpenList(pTile->dwIndex))
			continue;

		pTile->dwParentIndex = iStartIndex;
		m_OpenList.emplace_back(pTile->dwIndex);

	}

	if (m_OpenList.empty())
		return false;

	m_OpenList.sort([&](int iPreIndex, int iNextIndex)
	{
		// 휴리스틱 구할거. 거리 1 + 거리2 = 총 소모 비용. 
		// 1. preIndex - miStartIndex + preIndex - goalIndex =  fDist; 
		_vec3 vPreToStart = vecTile[m_iStartIndex]->vPos - vecTile[iPreIndex]->vPos;
		_vec3 vPreToGoal = vecTile[iGoalIndex]->vPos - vecTile[iPreIndex]->vPos;
		float fDist1 = D3DXVec3Length(&vPreToStart) + D3DXVec3Length(&vPreToGoal);

		_vec3 vNextToStart = vecTile[m_iStartIndex]->vPos - vecTile[iNextIndex]->vPos;
		_vec3 vNextToGoal = vecTile[iGoalIndex]->vPos - vecTile[iNextIndex]->vPos;
		float fDist2 = D3DXVec3Length(&vNextToStart) + D3DXVec3Length(&vNextToGoal);

		return fDist1 < fDist2;
	});
	return PathFinding(m_OpenList.front(), iGoalIndex);
}

void CAstar_Manager::Make_Route(int iStartIndex, int iGoalIndex)
{
	CGameObject* pTerrain = CGameObject_Manger::Get_Instance()->Get_Tile();
	const vector<GROUND*>& vecTile = dynamic_cast<CTile*>(pTerrain)->Get_Tile();

	m_BestList.emplace_back(vecTile[iGoalIndex]);

	int iParentIndex = vecTile[iGoalIndex]->dwParentIndex;

	while (true)
	{
		if (iParentIndex == iStartIndex)
		{	

			//if (m_bAStarMonsterCheck)
			//{
			//	m_BestList.pop_back();
			//	break;
			//}
			//else
				break;
		}

		m_BestList.emplace_front(vecTile[iParentIndex]);
		iParentIndex = vecTile[iParentIndex]->dwParentIndex;
	}
}

int CAstar_Manager::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	CGameObject* pTerrain = CGameObject_Manger::Get_Instance()->Get_Tile();
	const vector<GROUND*>& vecTile = dynamic_cast<CTile*>(pTerrain)->Get_Tile();

	size_t iSize = vecTile.size();
	size_t i = 0;
	for (; i < iSize; ++i)
	{
		if (IsPicking(i, vPos))
			return i;
	}
	return -1;
}

bool CAstar_Manager::IsPicking(const int & _iIndex, const D3DXVECTOR3 & vMousePos)
{
	CGameObject* pTerrain = CGameObject_Manger::Get_Instance()->Get_Tile();
	const vector<GROUND*>& vecTile = dynamic_cast<CTile*>(pTerrain)->Get_Tile();
	

	int		iX = (int)vMousePos.x / TILECX;
	int		iY = (int)vMousePos.y / TILECY;

	int iIndex = iY * TILEX + iX;

	if (iIndex == _iIndex)
		return true;
	else
		return false;
}

void CAstar_Manager::StartAstar(const _vec3 & vStartPos, const _vec3 & vGoalPos)
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();

	m_iStartIndex = GetTileIndex(vStartPos);
	int iGoalIndex = GetTileIndex(vGoalPos);

	if (-1 == m_iStartIndex || -1 == iGoalIndex)
		return;

	if (m_iStartIndex == iGoalIndex)	
		return;
	
	CGameObject* pTerrain = CGameObject_Manger::Get_Instance()->Get_Tile();
	const vector<GROUND*>& vecTile = dynamic_cast<CTile*>(pTerrain)->Get_Tile();

	if (1 == vecTile[iGoalIndex]->byOption)
		return;

	if (PathFinding(m_iStartIndex, iGoalIndex))
		Make_Route(m_iStartIndex, iGoalIndex);
}
