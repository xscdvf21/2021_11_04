#ifndef PhysXMesh_h__
#define PhysXMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CPhysXMesh : public CMesh
{
private:
	explicit CPhysXMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPhysXMesh(const CPhysXMesh& rhs);
	virtual ~CPhysXMesh(void);

public:
	const _vec3*			Get_VtxPos(void) const { return m_pVtxPos; }
	_ulong					Get_VtxCnt(void) const { return m_dwVtxCnt; }
	_ulong					Get_Stride(void) const { return m_dwStride; }

public:
	HRESULT					Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void					Render_Meshes(void);
	void					Render_Meshes(LPD3DXEFFECT& pEffect);

private:
	_bool					Find_Alpha(const char* pFileName);
	void					Set_Buffer(vector<_vec3> vecStart, vector<_vec3> vecEnd);

private:
	LPD3DXMESH				m_pMesh;		// 노말 정보가 최종적으로 삽입된 메쉬 컴 객체

	LPD3DXBUFFER			m_pAdjacency;		// 인접하는 폴리곤 정보를 보관하기 위한 주소(첫 번째 주소 보관)
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;		// 서브셋의 개수


	D3DXMATERIAL*			m_pMtrl;			// 재질정보와 텍스쳐의 이름을 보관하기 위한 구조체 포인터
												// 서브셋의 개수 == 텍스처의 개수 == 재질의 개수
	LPDIRECT3DTEXTURE9*		m_ppTextures;		// 여러장의 텍스처를 보관하기 위한 배열 포인터


	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// 정점의 크기(사이즈) 보관
	_vec3*					m_pVtxPos;			// 정점 정보 중 포지션 값만 얻어오기 위한 포인터
	INDEX32*				m_pIndex32 =nullptr;
	INDEX16*				m_pIndex16 = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

												// 바운딩 박스와 관계 없는 작업, 노말 정보가 없는 메쉬의 정점에 노말 값을 삽입하기 위함
	LPD3DXMESH				m_pOriMesh;	// 최초 로딩 시에 얻어온 메쉬 컴객체




public:
	static CPhysXMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free(void);
};



END
#endif // PhysXMesh_h__
