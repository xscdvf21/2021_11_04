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
	virtual ~CDynamicMesh();

public:
	const	D3DXFRAME_DERIVED*	Get_FrameByName(const char* pFrameName);
	LPD3DXANIMATIONCONTROLLER	Get_AniCtrl() { return m_pAniCtrl->Get_AniCtrl(); }
	_uint						Get_CurrentTrack() { return m_pAniCtrl->Get_CurrentTrack(); }
	_double						Get_CurrentTime() { return m_pAniCtrl->Get_CurrentTime(); }
	_double						Get_Period() { return m_pAniCtrl->Get_Period(); }
	_float						Get_AccTime() { return m_pAniCtrl->Get_AccTime(); }
	_bool						Is_AnimationSetEnd(_float minusTime, _bool bPosReset = false);
	const	_uint&				Get_AniIndex() { return m_iAniIndex; }
	D3DXFRAME*					Get_RootFrame() { return m_pRootFrame; }
	_ulong						Get_FrameCount(const _int& iIndex);

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex, const _bool& bPhys = false);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect, _uint iPassIndex = 0);
	void		Render_Meshes_VTF(LPD3DXEFFECT& pEffect, _uint iPassIndex = 0);
	void		Render_VTF(LPD3DXEFFECT& pEffect, _uint iPassIndex = 0);
	_bool		Find_Electric(const char * pFileName);

	void		SetUp_VTFMatrices();
	void		Set_Animationset(const _uint& iIndex, _float fSpeed = 1.0f, _float fSyncTime = 0.25f, _float fDelay = 0.0f);
	void		Play_Animationset(const _float& fTimeDelta);

	void		Render_PhysMeshes(LPD3DXEFFECT& pEffect, _uint iPassIndex /*= 0*/, vector<PxRigidDynamic*> vecMat = vector<PxRigidDynamic*>(), const _int& iDir =0);
private:
	//  ��� ���� ��ȸ�ϸ鼭 ������ ������ CombinedTransformationMatrix�� �����ϴ� �Լ�
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	void		Get_FrameMatrix();

private:
	D3DXFRAME*				m_pRootFrame;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;

	_uint					m_iAniIndex = 0; //���絹���ִ� �ִϸ��̼� �ε��� ��ȯ��.

	list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;

public:
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex,const _bool& bPhys);
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex);
	virtual CComponent*		Clone(void* pArg = nullptr);

public:
	virtual void			Free(void);
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