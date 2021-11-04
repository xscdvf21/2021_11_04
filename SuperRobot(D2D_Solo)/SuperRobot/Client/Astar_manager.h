#pragma once
class CScene;
class CAstar_Manager
{
	DECLEARE_SINGLETON(CAstar_Manager)
private:
	CAstar_Manager();
	~CAstar_Manager();
public:
	list<GROUND*>& Get_BestList() { return m_BestList; }
public:
	bool CheckOpenList(const int iIndex);
	bool CheckCloseList(const int iIndex);
public:
	bool PathFinding(int iStartIndex, int iGoalIndex);
	void Make_Route(int iStartIndex, int iGoalIndex);

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
	bool IsPicking(const int& iIndex, const D3DXVECTOR3& vMousePos);
	void StartAstar(const _vec3& vStartPos, const _vec3& vGoalPos);

public :
	void Set_AStarMonsterCheck(bool _bAStarMonsterCheck) {
		m_bAStarMonsterCheck = _bAStarMonsterCheck ;}
private:
	list<int> m_OpenList;
	list<int> m_CloseList;
	list<GROUND*> m_BestList;
	int m_iStartIndex;

	bool	m_bAStarMonsterCheck; //에이스타 거리 구할때 몬스터면 pop_back()를 위해.
};

