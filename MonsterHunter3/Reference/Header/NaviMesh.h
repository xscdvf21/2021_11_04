#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	const _ulong*		Get_CellIndex(void) { return &m_dwCellIndex; }
	void				Set_CellIndex(const _ulong& dwIndex) { m_dwCellIndex = dwIndex; }

public:
	HRESULT				Ready_NaviMesh(_uint iNaviSize, vector<CCell*> vecCell);
	void				Render_NaviMesh(void);
	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);

private:
	HRESULT				Link_Cell(void);

private:
	vector<CCell*>		m_vecCell;
	_ulong				m_dwCellIndex;

public:
	static CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iNaviSize, vector<CCell*> vecCell);
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

};

END
#endif // NaviMesh_h__
