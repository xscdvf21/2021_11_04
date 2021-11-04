#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	const _vec3*			Get_VtxPos(void) const { return m_pVtxPos; }
	_ulong					Get_VtxCnt(void) const { return m_dwVtxCnt; }
	_ulong					Get_Stride(void) const { return m_dwStride; }
	const	D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
public:
	HRESULT			Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void			Render_Meshes(void);

private:
	LPD3DXMESH				m_pMesh;		// �븻 ������ ���������� ���Ե� �޽� �� ��ü

	LPD3DXBUFFER			m_pAdjacency;		// �����ϴ� ������ ������ �����ϱ� ���� �ּ�(ù ��° �ּ� ����)
	LPD3DXBUFFER			m_pSubset;			
	_ulong					m_dwSubsetCnt;		// ������� ���� == �ؽ�ó�� ���� == ������ ����


	LPD3DXMATERIAL			m_pMtrl;			// ���������� �ؽ����� �̸��� �����ϱ� ���� ����ü ������
												
	LPDIRECT3DTEXTURE9*		m_ppTextures;		// �������� �ؽ�ó�� �����ϱ� ���� �迭 ������
	
	D3DXFRAME*				m_pRootFrame;

	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// ������ ũ��(������) ����
	_vec3*					m_pVtxPos;			// ���� ���� �� ������ ���� ������ ���� ������


	// �ٿ�� �ڽ��� ���� ���� �۾�, �븻 ������ ���� �޽��� ������ �븻 ���� �����ϱ� ����
	LPD3DXMESH			m_pOriMesh;	// ���� �ε� �ÿ� ���� �޽� �İ�ü




public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);

};



END
#endif // StaticMesh_h__
