#pragma once

#ifndef __TILEMGR_H__
#define __TILEMGR_H__

class CObj;
class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Picking_Tile(int _iDrawID);

	void Save_Tile();
	void Load_Tile();

	CObj * Get_Index(float _x, float _y);

	void Save_Tile2();

	void Load_Tile2();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CTileMgr*	m_pInstance;
	vector<CObj*>		m_vecTile;
	vector<CObj*>		m_vecTileBlock;
};


#endif // !__TILEMGR_H__
