#include "HierarchyLoader.h"
#include "ResourcesMgr.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ushort& wSceneIndex, const _bool& bPhys)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
	, m_iSceneIndex(wSceneIndex)
	, m_bPhys(bPhys)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{
	for (auto& iter : m_vecFilPath)
		Safe_Delete_Array(iter);
	m_vecFilPath.clear();
	m_vecFilPath.shrink_to_fit();
}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pDerivedFrame->Name, Name);

	pDerivedFrame->CombinedTranformationMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*			pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	_ulong dwNumFaces = pMesh->GetNumFaces();	// 메쉬가 지닌 폴리곤의 개수를 반환

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong	dwFVF = pMesh->GetFVF();

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);


	pMesh->GetDeclaration(Decl);


	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Type == D3DDECLTYPE_UNUSED)
		{
			Decl[i].Stream = 0;
			Decl[i].Offset = Decl[i - 1].Offset + 12;
			Decl[i].Type = D3DDECLTYPE_FLOAT3;
			Decl[i].Method = D3DDECLMETHOD_DEFAULT;
			Decl[i].Usage = D3DDECLUSAGE_BINORMAL;
			Decl[i].UsageIndex = 0;

			Decl[i + 1].Stream = 0;
			Decl[i + 1].Offset = Decl[i].Offset + 12;
			Decl[i + 1].Type = D3DDECLTYPE_FLOAT3;
			Decl[i + 1].Method = D3DDECLMETHOD_DEFAULT;
			Decl[i + 1].Usage = D3DDECLUSAGE_TANGENT;
			Decl[i + 1].UsageIndex = 0;

			//Decl[i + 2].Stream = 0;
			//Decl[i + 2].Offset = Decl[i + 1].Offset + 16;
			//Decl[i + 2].Type = D3DDECLTYPE_FLOAT4;
			//Decl[i + 2].Method = D3DDECLMETHOD_DEFAULT;
			//Decl[i + 2].Usage = D3DDECLUSAGE_TEXCOORD;
			//Decl[i + 2].UsageIndex = 1;

			Decl[i + 2].Stream = 0xff;
			Decl[i + 2].Offset = 0;
			Decl[i + 2].Type = D3DDECLTYPE_UNUSED;
			Decl[i + 2].Method = 0;
			Decl[i + 2].Usage = 0;
			Decl[i + 2].UsageIndex = 0;
			break;
		}
	}

	// 메쉬의 노말 정보가 없는 경우 코드로 삽입
	if (!(dwFVF & D3DFVF_NORMAL))
	{

		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMesh(pMesh->GetOptions(), Decl, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);

		D3DXComputeTangentFrameEx(pDerivedMeshContainer->MeshData.pMesh,
			D3DDECLUSAGE_TEXCOORD, 0,
			D3DDECLUSAGE_BINORMAL, 0,
			D3DDECLUSAGE_TANGENT, 0,
			D3DDECLUSAGE_NORMAL, 0,
			D3DXTANGENT_WRAP_UV | D3DXTANGENT_ORTHOGONALIZE_FROM_V | D3DXTANGENT_CALCULATE_NORMALS | D3DXTANGENT_GENERATE_IN_PLACE,
			pDerivedMeshContainer->pAdjacency, 0.01f, 0.01f, 0.01f, NULL, NULL);

	}
	/*if (!(dwFVF & D3DFVF_TEX1))
	{
		Safe_Release(pDerivedMeshContainer->MeshData.pMesh);

		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_TEX1, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);

		LPVOID pBuf;
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pBuf);
		D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
		ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

		pDerivedMeshContainer->MeshData.pMesh->GetDeclaration(Decl);

		WORD		byOffSet = 0;
		for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
		{
			if (Decl[i].Usage == D3DDECLUSAGE_TEXCOORD && Decl[i].UsageIndex == 1)
			{
				byOffSet = (WORD)Decl[i].Offset;
				break;
			}
		}
		_uint stride = D3DXGetFVFVertexSize(pMesh->GetFVF());
		_ulong iCount = pDerivedMeshContainer->MeshData.pMesh->GetNumVertices();
		for (_ulong i = 0; i < iCount; ++i)
		{

			_vec4 v = _vec4{ _float(i % 256) / 256.f,_float(i / 256) / 256.f,0,0 };
			*((_vec4*)(((_ubyte*)pBuf) + (i * stride + byOffSet))) = _vec4{ _float(i % 256) / 256.f,_float(i / 256) / 256.f,0,0 };
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	}
	else
	{
		LPVOID pBuf;
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pBuf);
		D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
		ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

		pDerivedMeshContainer->MeshData.pMesh->GetDeclaration(Decl);

		WORD		byOffSet = 0;
		for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
		{
			if (Decl[i].Usage == D3DDECLUSAGE_TEXCOORD && Decl[i].UsageIndex == 1)
			{
				byOffSet = (WORD)Decl[i].Offset;
				break;
			}
		}
		_uint stride = D3DXGetFVFVertexSize(pMesh->GetFVF());
		_ulong iCount = pDerivedMeshContainer->MeshData.pMesh->GetNumVertices();
		for (_ulong i = 0; i < iCount; ++i)
		{

			*((_vec4*)(((_ubyte*)pBuf) + (i * stride + byOffSet))) = _vec4{ _float(i % 256) / 256.f,_float(i / 256) / 256.f,0,0 };
			_vec4 v = _vec4{ _float(i % 256) / 256.f,_float(i / 256) / 256.f,0,0 };
		}

		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}*/
	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppNmTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppNmTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppMmTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppMmTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppEmTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppEmTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppSmTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppSmTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);
	if (!m_bPhys)
		m_pGraphicDev->CreateTexture(256, 256, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED,
			&pDerivedMeshContainer->pVTFTexture, nullptr);

	if (0 != NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[256] = L"";
			_tchar		szFileName[256] = L"";
			_tchar		szNormalFullPath[256] = L"";
			_tchar		szNormalFileName[256] = L"";
			_tchar		szMaterialFullPath[256] = L"";
			_tchar		szMaterialFileName[256] = L"";
			_tchar		szEmissiveFullPath[256] = L"";
			_tchar		szEmissiveFileName[256] = L"";
			_tchar		szSpecularFullPath[256] = L"";
			_tchar		szSpecularFileName[256] = L"";

			lstrcpy(szFullPath, m_pPath);
			lstrcpy(szNormalFullPath, m_pPath);
			lstrcpy(szMaterialFullPath, m_pPath);
			lstrcpy(szEmissiveFullPath, m_pPath);
			lstrcpy(szSpecularFullPath, m_pPath);

			char szNMname[MAX_PATH] = "";
			char szMMname[MAX_PATH] = "";
			char szEMname[MAX_PATH] = "";
			char szSMname[MAX_PATH] = "";

			strcpy_s(szNMname, pMaterials[i].pTextureFilename);
			strcpy_s(szMMname, pMaterials[i].pTextureFilename);
			strcpy_s(szEMname, pMaterials[i].pTextureFilename);
			strcpy_s(szSMname, pMaterials[i].pTextureFilename);

			Find_Normal(szNMname);
			Find_Material(szNMname);
			Find_Emissive(szEMname);
			Find_Specular(szSMname);

			MultiByteToWideChar(CP_ACP, 0, szNMname, (_int)strlen(szNMname), szNormalFileName, 256);
			MultiByteToWideChar(CP_ACP, 0, szMMname, (_int)strlen(szMMname), szMaterialFileName, 256);
			MultiByteToWideChar(CP_ACP, 0, szEMname, (_int)strlen(szEMname), szEmissiveFileName, 256);
			MultiByteToWideChar(CP_ACP, 0, szSMname, (_int)strlen(szSMname), szSpecularFileName, 256);
			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename, _int(strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename)), szFileName, 256);

			lstrcat(szNormalFullPath, szNormalFileName);
			lstrcat(szMaterialFullPath, szMaterialFileName);
			lstrcat(szEmissiveFullPath, szEmissiveFileName);
			lstrcat(szSpecularFullPath, szSpecularFileName);
			lstrcat(szFullPath, szFileName);

			char* tag = new char[MAX_PATH];
			strcpy_s(tag, MAX_PATH, pDerivedMeshContainer->pMaterials[i].pTextureFilename);
			m_vecFilPath.emplace_back(tag);

			Get_Texture_Clone(pDerivedMeshContainer->ppTexture[i], szFileName, szFullPath);
			Get_Texture_Clone(pDerivedMeshContainer->ppNmTexture[i], szNormalFileName, szNormalFullPath);
			Get_Texture_Clone(pDerivedMeshContainer->ppMmTexture[i], szMaterialFileName, szMaterialFullPath);
			Get_Texture_Clone(pDerivedMeshContainer->ppEmTexture[i], szEmissiveFileName, szEmissiveFullPath);
			Get_Texture_Clone(pDerivedMeshContainer->ppSmTexture[i], szSpecularFileName, szSpecularFullPath);


			//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppTexture[i])))
			//	return E_FAIL;

			//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szNormalFullPath, &pDerivedMeshContainer->ppNmTexture[i])))
			//	pDerivedMeshContainer->ppNmTexture[i] = nullptr;

			//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szMaterialFullPath, &pDerivedMeshContainer->ppMmTexture[i])))
			//	pDerivedMeshContainer->ppMmTexture[i] = nullptr;

			//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szEmissiveFullPath, &pDerivedMeshContainer->ppEmTexture[i])))
			//	pDerivedMeshContainer->ppEmTexture[i] = nullptr;

			//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szSpecularFullPath, &pDerivedMeshContainer->ppSmTexture[i])))
			//	pDerivedMeshContainer->ppSmTexture[i] = nullptr;
		}
	}
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = nullptr;
		pDerivedMeshContainer->ppNmTexture[0] = nullptr;
		pDerivedMeshContainer->ppMmTexture[0] = nullptr;
		pDerivedMeshContainer->ppEmTexture[0] = nullptr;
		pDerivedMeshContainer->ppSmTexture[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;


	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->SetDeclaration(Decl);
	pDerivedMeshContainer->pSkinInfo->AddRef();


	DWORD maxVertexInfluences = 0;
	DWORD numBoneComboEntries = 0;
	if (m_bPhys)
	{

		pDerivedMeshContainer->MeshData.pMesh->CloneMesh(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), Decl, m_pGraphicDev, &pDerivedMeshContainer->pOriMesh);
	}
	else
	{
		Safe_Release(pDerivedMeshContainer->MeshData.pMesh);


		if (FAILED(pDerivedMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(
			pMeshData->pMesh,
			D3DXMESH_MANAGED, pDerivedMeshContainer->pSkinInfo->GetNumBones(),
			//nullptr,nullptr
			pDerivedMeshContainer->pAdjacency, pDerivedMeshContainer->pAdjacency
			, nullptr, nullptr, &maxVertexInfluences, &numBoneComboEntries
			, &pDerivedMeshContainer->pBoneCombinationBuf, &pDerivedMeshContainer->MeshData.pMesh
		)))
		{
			int i = 0;
		}
		pDerivedMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pDerivedMeshContainer->NumAttributeGroups);
		pDerivedMeshContainer->attributeTable = new D3DXATTRIBUTERANGE[pDerivedMeshContainer->NumAttributeGroups];
		pDerivedMeshContainer->MeshData.pMesh->GetAttributeTable(pDerivedMeshContainer->attributeTable, NULL);
	}



	// 메쉬 컨테이너에 영향을 미치는 뼈의 개수를 반환
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffsetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppFrameCombinedMatrix = new _matrix*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

	*ppNewMeshContainer = pDerivedMeshContainer;



	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{

	D3DXMESHCONTAINER_DERIVED*	pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
	{
		Safe_Release(pDerivedMeshContainer->ppTexture[i]);
		Safe_Release(pDerivedMeshContainer->ppNmTexture[i]);
		Safe_Release(pDerivedMeshContainer->ppMmTexture[i]);
		Safe_Release(pDerivedMeshContainer->ppEmTexture[i]);
		Safe_Release(pDerivedMeshContainer->ppSmTexture[i]);
	}

	Safe_Delete_Array(pDerivedMeshContainer->ppTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppNmTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppMmTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppEmTexture);
	Safe_Delete_Array(pDerivedMeshContainer->ppSmTexture);


	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);
	Safe_Delete_Array(pDerivedMeshContainer->attributeTable);

	Safe_Release(pDerivedMeshContainer->pBoneCombinationBuf);

	Safe_Release(pDerivedMeshContainer->pVTFTexture);
	Safe_Release(pDerivedMeshContainer->pOriMesh);
	Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerivedMeshContainer->pSkinInfo);

	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderingMatrix);

	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = _uint(strlen(pFrameName));

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}

_bool CHierarchyLoader::Find_Normal(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == '1')
			{
				char szbuf = '2';
				memcpy(&(pFileName[i - 1]), &szbuf, sizeof(char));
				return true;
			}
		}
	}

	return _bool();
}

_bool CHierarchyLoader::Find_Material(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == '1')
			{
				char szbuf = '3';
				memcpy(&(pFileName[i - 1]), &szbuf, sizeof(char));
				return true;
			}
		}
	}

	return _bool();
}

_bool CHierarchyLoader::Find_Emissive(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == '1')
			{
				char szbuf = '4';
				memcpy(&(pFileName[i - 1]), &szbuf, sizeof(char));
				return true;
			}
		}
	}

	return _bool();
}

_bool CHierarchyLoader::Find_Specular(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == '1')
			{
				char szbuf = '5';
				memcpy(&(pFileName[i - 1]), &szbuf, sizeof(char));
				return true;
			}
		}
	}

	return _bool();
}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ushort& wSceneIndex, const _bool& bPhys)
{
	return new CHierarchyLoader(pGraphicDev, pPath, wSceneIndex, bPhys);

}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, const _ushort& wSceneIndex)
{
	return new CHierarchyLoader(pGraphicDev, pPath, wSceneIndex);

}

void Engine::CHierarchyLoader::Get_Texture_Clone(LPDIRECT3DTEXTURE9& ppTexture, _tchar* szFileName, _tchar* szFullPath)
{
	if (FAILED(CResourcesMgr::GetInstance()->Ready_Texture(m_pGraphicDev, m_iSceneIndex, szFileName, TEX_NORMAL, szFullPath)))
	{
		auto* pTexture = CResourcesMgr::GetInstance()->Clone_Resource(m_iSceneIndex, szFileName);
		if (pTexture)
			ppTexture = pTexture->Is<CTexture>()->Get_Texture();
		else
			ppTexture = nullptr;
		Safe_Release(pTexture);
	}
	else
	{
		auto* pTexture = CResourcesMgr::GetInstance()->Clone_Resource(m_iSceneIndex, szFileName);
		if (pTexture)
			ppTexture = pTexture->Is<CTexture>()->Get_Texture();
		else
			ppTexture = nullptr;
		Safe_Release(pTexture);
	}
}



Engine::_ulong Engine::CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

