#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	const	D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	_bool							Is_AnimationSetEnd(void);
public:
	HRESULT			Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT			Ready_MeshesBoss(const _tchar* pFilePath, const _tchar* pFileName);
	void			Render_Meshes(void);

	void			Set_Animationset(const _uint& iIndex);
	const	_uint&	Get_AniIndex();
	void			Play_AnimationsetBoss(const _float& fTimeDelta);
	void			Play_Animationset(const _float& fTimeDelta);




private:
	//  ��� ���� ��ȸ�ϸ鼭 ������ ������ CombinedTransformationMatrix�� �����ϴ� �Լ�
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*				m_pRootFrame;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;

	_uint					m_pAniIndex = 0; //�ִϸ��̼� �ε��� ��ȯ��.

	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;

public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	static CDynamicMesh*		CreateBoss(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*			Clone(void);
	virtual void				Free(void);

};

END
#endif // DynamicMesh_h__

//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;		// �޽��� �� �̸��� �����ϴ� ���ڿ�, �ƽ�Ű�ڵ�
//	D3DXMATRIX              TransformationMatrix; // ���� �ε� ������ �ش��ϴ� ���� ���� ��� ���� �����ϱ� ���� ����
//
//	LPD3DXMESHCONTAINER     pMeshContainer;
//
//	struct _D3DXFRAME       *pFrameSibling;			// ���� ���迡 �ִ� ����� �ּ� ��
//	struct _D3DXFRAME       *pFrameFirstChild;		// �ڽ� ���迡 �ִ� ����� �ּ� ��
//
//} D3DXFRAME, *LPD3DXFRAME;
//
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;
//
//	D3DXMESHDATA            MeshData;
//
//	LPD3DXMATERIAL          pMaterials;
//	LPD3DXEFFECTINSTANCE    pEffects;
//	DWORD                   NumMaterials;
//	DWORD                  *pAdjacency;
//
//	LPD3DXSKININFO          pSkinInfo;
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;