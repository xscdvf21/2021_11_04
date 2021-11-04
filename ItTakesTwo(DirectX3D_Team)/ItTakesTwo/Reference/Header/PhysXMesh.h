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
	LPD3DXMESH				m_pMesh;		// �븻 ������ ���������� ���Ե� �޽� �� ��ü

	LPD3DXBUFFER			m_pAdjacency;		// �����ϴ� ������ ������ �����ϱ� ���� �ּ�(ù ��° �ּ� ����)
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;		// ������� ����


	D3DXMATERIAL*			m_pMtrl;			// ���������� �ؽ����� �̸��� �����ϱ� ���� ����ü ������
												// ������� ���� == �ؽ�ó�� ���� == ������ ����
	LPDIRECT3DTEXTURE9*		m_ppTextures;		// �������� �ؽ�ó�� �����ϱ� ���� �迭 ������


	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// ������ ũ��(������) ����
	_vec3*					m_pVtxPos;			// ���� ���� �� ������ ���� ������ ���� ������
	INDEX32*				m_pIndex32 =nullptr;
	INDEX16*				m_pIndex16 = nullptr;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

												// �ٿ�� �ڽ��� ���� ���� �۾�, �븻 ������ ���� �޽��� ������ �븻 ���� �����ϱ� ����
	LPD3DXMESH				m_pOriMesh;	// ���� �ε� �ÿ� ���� �޽� �İ�ü




public:
	static CPhysXMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void* pArg = nullptr) override;
	virtual void			Free(void);
};



END
#endif // PhysXMesh_h__
