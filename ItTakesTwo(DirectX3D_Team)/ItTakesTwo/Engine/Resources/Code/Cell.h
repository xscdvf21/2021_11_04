#ifndef Cell_h__
#define Cell_h__

#include "Line.h"

BEGIN(Engine)

class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT { POINT_A , POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR {	NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END	};
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum STATE { STATE_GROUND, STATE_FALL, STATE_END };
	enum COMPAREMOVE { MOVE, STOP };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _vec3*		Get_Point(POINT eType) { return &m_vPoint[eType]; }
	_vec3				Get_PointA() { return m_vPoint[POINT_A]; }
	_vec3				Get_PointB() { return m_vPoint[POINT_B]; }
	_vec3				Get_PointC() { return m_vPoint[POINT_C]; }
	_bool				Get_State();
	_bool				Get_Lock() { return m_bLockArea; }
	_uint				Get_StateTool() { return m_eCellState; }
	CCell*				Get_Neighbor(NEIGHBOR eType) { return m_pNeighbor[eType]; }
	CLine*				Get_Line(LINE eType) { return m_pLine[eType]; }
	const _ulong*		Get_CellIndex(void) { return &m_dwCellIndex; }

public:
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	void				Set_CellState(_uint eState) { m_eCellState = (CCell::STATE)eState; }
	void				Set_Lock(_bool lock) { m_bLockArea = lock; }

public:
	HRESULT				Ready_Cell(const _ulong&	 dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, _uint eState, _bool lock);
	_bool				Compare_Point(const _vec3* pFirst, const _vec3* pSecond, CCell* pCell);
	void				Render_Cell(void);

	COMPAREMOVE			Compare(const _vec3* pEndPos, _ulong* pCellIndex);
	COMPAREMOVE			Compare(const _vec3 * pEndPos, _ulong * pCellIndex, _vec2 * vNormal);

public:
	_bool				Find_Cell(_vec3 pEndPos, _ulong* pCellIndex);

private:
	_vec3				m_vPoint[POINT_END];
	CCell*				m_pNeighbor[NEIGHBOR_END];
	CLine*				m_pLine[LINE_END];
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXLINE			m_pD3DXLine;
	_ulong				m_dwCellIndex;
	STATE				m_eCellState;
	_bool				m_bLockArea;

public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong&	 dwCellIndex,  const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, _uint eState, _bool lock);
	virtual void		Free(void);



};

END
#endif // Cell_h__
