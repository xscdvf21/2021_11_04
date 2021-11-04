#pragma once
#include "GameObject.h"
class CTerrain final : public CGameObject
{
public:
	explicit CTerrain();
	virtual ~CTerrain();
public:
	vector<TILE*>& Get_Tile() { return m_vecTile; }
	void Set_Tile(vector<TILE*>& vecTile) { m_vecTile = vecTile; }
public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
	bool IsPicking(const int& iIndex, const D3DXVECTOR3& vMousePos);
public:
	HRESULT LoadTileData_Terrain(const wstring& wstrFilePath); 
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
private:
	vector<TILE*> m_vecTile;

	float m_fSpeed; 
};

