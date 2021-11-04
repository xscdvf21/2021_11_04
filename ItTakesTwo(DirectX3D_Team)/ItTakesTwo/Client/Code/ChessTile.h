#ifndef __CHESSTILE_H__

#include "Base.h"
#include "Engine_Function.h"
#include "Client_Define.h"

class CChessTile : public CBase
{
	DECLARE_SINGLETON(CChessTile)

public:
	explicit	CChessTile();
	virtual		~CChessTile();

	void		Ready_ChessTile();
	

	const vector<CHESSTILE>&	Get_ChessTile() const { return m_vecChessTile; }
	const CHESSTILE			Get_TileIndex(_uint iIndex);
	const CHESSTILE			Get_Index(_vec3 _vec);
private :
	vector<CHESSTILE>		m_vecChessTile;
	
	_uint				m_iTileIndex;
	_float				m_fX = -16.f;
	_float				m_fY = -16.f;

public:
	void				Set_Damage(_int iHp);
	void				Set_ChessKnightDir(_float3 vDir) { m_vChessKnightDir = vDir; }

public:
	_uint				Get_BossHp() { return m_iBossHp; }
	_float3				Get_ChessKnightDir() { return m_vChessKnightDir; }

public:
	virtual void Free() override;

private:
	_int				m_iBossHp;
	_float3				m_vChessKnightDir;
};


#define __CHESSTILE_H__
#endif


