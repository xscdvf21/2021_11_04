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
	vector<vector<CCell*>>	Get_VecCell() { return m_vecCell; }

public:
	HRESULT				Ready_NaviMesh(void);
	void				Render_NaviMesh(void);
	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _uint vecIndex, _bool* State = nullptr, _vec3* vFallPos = nullptr);
	_float3				Jump_OnNaviMesh(const _float3* pPos, const _float* fHeight, _float* fDis, _uint* pCellIndex, _bool* bJump);
	_vec3				Slide_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _uint vecIndex, _bool* State = nullptr);

public:
	void				Add_CellGroup();
	void				Add_Cell(_vec3 first, _vec3 second, _vec3 third, _uint vecIndex, _uint eState, _bool lock);
	void				Delete_CellGroup(_int GroupiIndex);
	void				Delete_Cell(_int GroupIndex, _int CellIndex);
	void				Load_Cell(const _tchar* pFilePath);
	_bool				Find_Cell(_vec3 vPos, _float* Height, _uint* GroupIndex);

private:
	HRESULT				Link_Cell(_uint vecIndex);

private:
	vector<vector<CCell*>>	m_vecCell;
	vector<CLine*>			m_vecOutLine;
	vector<_vec2>			m_vecNormal;
	_ulong					m_dwCellIndex;

public:
	static CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;

};

END
#endif // NaviMesh_h__
