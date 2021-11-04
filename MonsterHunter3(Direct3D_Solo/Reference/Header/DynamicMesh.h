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
	//  모든 뼈를 순회하면서 뼈들이 소유한 CombinedTransformationMatrix를 갱신하는 함수
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*				m_pRootFrame;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;

	_uint					m_pAniIndex = 0; //애니메이션 인덱스 반환용.

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
//	LPSTR                   Name;		// 메쉬의 뼈 이름을 저장하는 문자열, 아스키코드
//	D3DXMATRIX              TransformationMatrix; // 최초 로드 시점에 해당하는 뼈의 상태 행렬 값을 저장하기 위한 변수
//
//	LPD3DXMESHCONTAINER     pMeshContainer;
//
//	struct _D3DXFRAME       *pFrameSibling;			// 형제 관계에 있는 행렬의 주소 값
//	struct _D3DXFRAME       *pFrameFirstChild;		// 자식 관계에 있는 행렬의 주소 값
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