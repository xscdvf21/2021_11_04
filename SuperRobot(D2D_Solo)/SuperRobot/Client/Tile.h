#pragma once
#include "GameObject.h"

class CTile :
	public CGameObject
{
public:
	explicit CTile();
	virtual ~CTile();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public :
	GROUND* Get_Index(_vec3 _vec);

	GROUND* Get_TileMove(int _TilePos);

	vector<GROUND*>& Get_Tile() { return m_vecGround; }
	vector<list<GROUND*>>& Get_Graph() { return m_vecGraph; }

/* void Set_TileAhpal(int _TileIndex) { TileIndex = _TileIndex; }*/
	vector<GROUND*>		Get_vecGround() { return m_vecGround; }
	

public :
	void CreateGraph();
private :
	vector<GROUND*>		m_vecGround;
	vector<list<GROUND*>> m_vecGraph;
	vector<GROUND*>		temp;
	int iIndex = 0;

	/*int					TileIndex;*/


};

