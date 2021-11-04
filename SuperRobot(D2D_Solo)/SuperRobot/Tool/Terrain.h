#pragma once
class CToolView; 
class CTerrain final
{
public:
	explicit CTerrain();
			~CTerrain();
public:
	vector<TILE*>& Get_Tile() { return m_vecTile; }
	void Set_Tile(vector<TILE*>& vecTile) { m_vecTile = vecTile; }
public:
	void Tile_Change_Terrain(const D3DXVECTOR3& vMousePos, const BYTE& byDrawID, const BYTE& byOption = 0); 
	int GetTileIndex(const D3DXVECTOR3& vPos);
	bool IsPicking(const int& iIndex, const D3DXVECTOR3& vMousePos); 
	void Set_View(CToolView* pView) { m_pView = pView;  }
public:
	HRESULT Ready_Terrain(); 
	void	Render_Terrain(); 
	void	MiniRender_Terrain();
	void	Release_Terrain(); 
private:
	vector<TILE*> m_vecTile; 
	CToolView* m_pView;
};

