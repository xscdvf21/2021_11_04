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
	enum COMPAREMOVE { MOVE, STOP };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _vec3*		Get_Point(POINT eType)		 { return &m_vPoint[eType]; }
	CCell*				Get_Neighbor(NEIGHBOR eType) { return m_pNeighbor[eType]; }
	const _ulong*		Get_CellIndex(void)			 { return &m_dwCellIndex; }
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; } 

	void				Set_Type(_uint iType) { m_iType = iType; }


public:
	HRESULT		Ready_Cell(const _ulong&	 dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	_bool		Compare_Point(const _vec3* pFirst, const _vec3* pSecond, CCell* pCell);
	void		Render_Cell(void);

	COMPAREMOVE	Compare(const _vec3* pEndPos, _ulong* pCellIndex);


private:
	_vec3		m_vPoint[POINT_END];
	CCell*		m_pNeighbor[NEIGHBOR_END];
	CLine*		m_pLine[LINE_END];
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXLINE	m_pD3DXLine;
	_ulong		m_dwCellIndex;

	_uint		m_iType = 0;
public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong&	 dwCellIndex,  const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual void		Free(void);



};

END
#endif // Cell_h__
