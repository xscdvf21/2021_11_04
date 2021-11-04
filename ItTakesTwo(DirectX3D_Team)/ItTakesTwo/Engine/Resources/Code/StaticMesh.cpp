#include "StaticMesh.h"
#include "Phys_Manager.h"
#include "Frustum.h"
#include "ResourcesMgr.h"
USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pAdjacency(nullptr)
	, m_pSubset(nullptr)
	, m_dwSubsetCnt(0)
	, m_pMtrl(nullptr)
	, m_ppTextures(nullptr)
	, m_ppNmTextures(nullptr)
	, m_ppMmTextures(nullptr)
	, m_ppEmTextures(nullptr)
	, m_ppSmTextures(nullptr)
	, m_pMesh(nullptr)
	, m_dwVtxCnt(0)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pMtrl(rhs.m_pMtrl)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_pIndex32(rhs.m_pIndex32)
	, m_pIndex16(rhs.m_pIndex16)
	, m_vecSubSet(rhs.m_vecSubSet)
	, m_pRootNode(rhs.m_pRootNode)
	, m_vecSubPOINT(rhs.m_vecSubPOINT)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppNmTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppMmTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppEmTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppSmTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		if (rhs.m_ppTextures[i] != nullptr)
			m_ppTextures[i]->AddRef();

		m_ppNmTextures[i] = rhs.m_ppNmTextures[i];
		if (rhs.m_ppNmTextures[i] != nullptr)
			m_ppNmTextures[i]->AddRef();

		m_ppMmTextures[i] = rhs.m_ppMmTextures[i];
		if (rhs.m_ppMmTextures[i] != nullptr)
			m_ppMmTextures[i]->AddRef();

		m_ppEmTextures[i] = rhs.m_ppEmTextures[i];
		if (rhs.m_ppEmTextures[i] != nullptr)
			m_ppEmTextures[i]->AddRef();

		m_ppSmTextures[i] = rhs.m_ppSmTextures[i];
		if (rhs.m_ppSmTextures[i] != nullptr)
			m_ppSmTextures[i]->AddRef();
	}

	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}



void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex)
{
	_tchar	szFullPath[256] = L"";
	m_wSceneIndex = wSceneIndex;
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);
	std::wstring wstr = szFullPath;

	if (FAILED(D3DXLoadMeshFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		&m_pAdjacency,
		&m_pSubset,
		NULL,
		&m_dwSubsetCnt,
		&m_pOriMesh)))
		return E_FAIL;
	m_pOriMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)m_pAdjacency->GetBufferPointer(),
		(DWORD*)m_pAdjacency->GetBufferPointer(), // new adjacency information
		0, 0);
	// 메쉬가 지닌 정점의 속성 정보를 얻어오는 함수
	_ulong	dwFVF = m_pOriMesh->GetFVF();

	D3DVERTEXELEMENT9		Decl2[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl2, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);


	m_pOriMesh->GetDeclaration(Decl2);


	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl2[i].Type == D3DDECLTYPE_UNUSED)
		{
			Decl2[i].Stream = 0;
			Decl2[i].Offset = Decl2[i - 1].Offset + 12;
			Decl2[i].Type = D3DDECLTYPE_FLOAT3;
			Decl2[i].Method = D3DDECLMETHOD_DEFAULT;
			Decl2[i].Usage = D3DDECLUSAGE_BINORMAL;
			Decl2[i].UsageIndex = 0;

			Decl2[i + 1].Stream = 0;
			Decl2[i + 1].Offset = Decl2[i].Offset + 12;
			Decl2[i + 1].Type = D3DDECLTYPE_FLOAT3;
			Decl2[i + 1].Method = D3DDECLMETHOD_DEFAULT;
			Decl2[i + 1].Usage = D3DDECLUSAGE_TANGENT;
			Decl2[i + 1].UsageIndex = 0;

			Decl2[i + 2].Stream = 0xff;
			Decl2[i + 2].Offset = 0;
			Decl2[i + 2].Type = D3DDECLTYPE_UNUSED;
			Decl2[i + 2].Method = 0;
			Decl2[i + 2].Usage = 0;
			Decl2[i + 2].UsageIndex = 0;
			break;
		}
	}

	// 메쉬의 노말 정보가 없는 경우 코드로 삽입
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
		D3DXComputeTangentFrameEx(m_pMesh,
			D3DDECLUSAGE_TEXCOORD, 0,
			D3DDECLUSAGE_BINORMAL, 0,
			D3DDECLUSAGE_TANGENT, 0,
			D3DDECLUSAGE_NORMAL, 0,
			D3DXTANGENT_WRAP_UV | D3DXTANGENT_ORTHOGONALIZE_FROM_V | D3DXTANGENT_CALCULATE_NORMALS | D3DXTANGENT_GENERATE_IN_PLACE,
			(_ulong*)m_pAdjacency->GetBufferPointer(), 0.01f, 0.01f, 0.01f, NULL, NULL);
	}
	// 메쉬 사이즈에 맞는 바운딩박스 생성을 위한 작업
	void*		pVertex = NULL;

	m_dwVtxCnt = m_pMesh->GetNumVertices();	// 메쉬가 지닌 정점의 개수를 반환

	m_pVtxPos = new _vec3[m_dwVtxCnt];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte		byOffSet = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	// 정점이 지닌 fvf정보를 기준으로 정점의 크기를 계산하자
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (i * m_dwStride + byOffSet)));

	}

	m_pMesh->UnlockVertexBuffer();

	// LPD3DXBUFFER가 담고 있는 버퍼 정보 중 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppNmTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppMmTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppEmTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppSmTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar		szFileName[256] = L"";

		_tchar		szNmFileName[256] = L"";
		_tchar		szNmFullPath[256] = L"";

		_tchar		szMmFileName[256] = L"";
		_tchar		szMmFullPath[256] = L"";

		_tchar		szEmFileName[256] = L"";
		_tchar		szEmFullPath[256] = L"";

		_tchar		szSmFileName[256] = L"";
		_tchar		szSmFullPath[256] = L"";

		char szNmName[MAX_PATH] = "";
		strcpy_s(szNmName, m_pMtrl[i].pTextureFilename);
		Find_Normal(szNmName);

		char szMmName[MAX_PATH] = "";
		strcpy_s(szMmName, m_pMtrl[i].pTextureFilename);
		Find_Material(szMmName);

		char szEmName[MAX_PATH] = "";
		strcpy_s(szEmName, m_pMtrl[i].pTextureFilename);
		Find_Emissive(szEmName);

		char szSmName[MAX_PATH] = "";
		strcpy_s(szSmName, m_pMtrl[i].pTextureFilename);
		Find_Specular(szSmName);

		lstrcpy(szFullPath, pFilePath);
		lstrcpy(szNmFullPath, pFilePath);
		lstrcpy(szMmFullPath, pFilePath);
		lstrcpy(szEmFullPath, pFilePath);
		lstrcpy(szSmFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, _uint(strlen(m_pMtrl[i].pTextureFilename)), szFileName, 256);
		MultiByteToWideChar(CP_ACP, 0, szNmName, _uint(strlen(szNmName)), szNmFileName, 256);
		MultiByteToWideChar(CP_ACP, 0, szMmName, _uint(strlen(szMmName)), szMmFileName, 256);
		MultiByteToWideChar(CP_ACP, 0, szEmName, _uint(strlen(szEmName)), szEmFileName, 256);
		MultiByteToWideChar(CP_ACP, 0, szSmName, _uint(strlen(szSmName)), szSmFileName, 256);

		lstrcat(szFullPath, szFileName);
		lstrcat(szNmFullPath, szNmFileName);
		lstrcat(szMmFullPath, szMmFileName);
		lstrcat(szEmFullPath, szEmFileName);
		lstrcat(szSmFullPath, szSmFileName);

		Get_Texture_Clone(m_ppTextures[i], szFileName, szFullPath);

		Get_Texture_Clone(m_ppNmTextures[i], szNmFileName, szNmFullPath);
		Get_Texture_Clone(m_ppMmTextures[i], szMmFileName, szMmFullPath);
		Get_Texture_Clone(m_ppEmTextures[i], szEmFileName, szEmFullPath);
		Get_Texture_Clone(m_ppSmTextures[i], szSmFileName, szSmFullPath);


		//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
		//	return E_FAIL;

		//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szNmFullPath, &m_ppNmTextures[i])))
		//	m_ppNmTextures[i] = nullptr;

		//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szMmFullPath, &m_ppMmTextures[i])))
		//	m_ppMmTextures[i] = nullptr;

		//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szEmFullPath, &m_ppEmTextures[i])))
		//	m_ppEmTextures[i] = nullptr;

		//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szSmFullPath, &m_ppSmTextures[i])))
		//	m_ppSmTextures[i] = nullptr;
	}

	wstr = wstr.substr(0, wstr.length() - 1);

	wstr += L"dat";
	std::wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;

	std::string str = converter.to_bytes(wstr);

	if (Read_BakePx_From_File(str))
		return S_OK;

	LPDIRECT3DINDEXBUFFER9 pbuffer;
	m_pMesh->GetIndexBuffer(&pbuffer);
	D3DINDEXBUFFER_DESC tDesc;
	pbuffer->GetDesc(&tDesc);
	PxTriangleMeshDesc tMesh;

	if (tDesc.Format == D3DFORMAT::D3DFMT_INDEX16)
	{

		m_pIndex16 = new INDEX16[m_pMesh->GetNumFaces()];
		void*		pIndex = NULL;

		pbuffer->Lock(0, 0, &pIndex, 0);
		for (DWORD i = 0; i < m_pMesh->GetNumFaces(); ++i)
		{
			m_pIndex16[i] = *((INDEX16*)(((_ubyte*)pIndex) + (i * sizeof(INDEX16))));
		}
		tMesh.triangles.stride = sizeof(INDEX16);
		tMesh.triangles.data = m_pIndex16;
		tMesh.flags = PxMeshFlag::e16_BIT_INDICES;

	}
	else if (tDesc.Format == D3DFORMAT::D3DFMT_INDEX32)
	{

		m_pIndex32 = new INDEX32[m_pMesh->GetNumFaces()];
		void*		pIndex = NULL;

		pbuffer->Lock(0, 0, &pIndex, 0);
		for (DWORD i = 0; i < m_pMesh->GetNumFaces(); ++i)
		{
			m_pIndex32[i] = *((INDEX32*)(((_ubyte*)pIndex) + (i * sizeof(INDEX32))));
		}
		tMesh.triangles.data = m_pIndex32;
		tMesh.triangles.stride = sizeof(INDEX32);
	}
	pbuffer->Unlock();
	pbuffer->Release();

	tMesh.points.count = m_dwVtxCnt;
	tMesh.points.stride = sizeof(_vec3);
	tMesh.points.data = m_pVtxPos;

	tMesh.triangles.count = m_pMesh->GetNumFaces();

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = CPhys_Manager::GetInstance()->Get_Cook()->cookTriangleMesh(tMesh, writeBuffer, &result);
	if (!status)
		return S_OK;

	PxDefaultFileOutputStream Out(str.c_str());
	if (Out.isValid())
		Out.write(writeBuffer.getData(), writeBuffer.getSize());



	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	m_pPxMesh = CPhys_Manager::GetInstance()->Get_Physics()->createTriangleMesh(readBuffer);

	Build_Info();
	Build_Tree();
	return S_OK;
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT & pEffect, _uint iPassIndex)
{

	_uint bPassChange = false;

	size_t iCount = 0;
	_uint	iPass = iPassIndex;
	if (m_bIsMap)
	{
		sort(m_vecDrawSubset.begin(), m_vecDrawSubset.end());
		m_vecDrawSubset.erase(unique(m_vecDrawSubset.begin(), m_vecDrawSubset.end()), m_vecDrawSubset.end());
		//for (size_t i = 0; i < m_vecDrawSubset.size(); ++i)
		//{
		//	pEffect->SetTexture("g_BaseTexture", m_ppTextures[m_vecDrawSubset[i]]);
		//	pEffect->CommitChanges();

		//	m_pMesh->DrawSubset(m_vecDrawSubset[i]);
		//}

		for (size_t i = 0; i < m_vecDrawSubset.size(); ++i)
		{
			iPass = iPassIndex;

			_bool		bAlpha = false;
			if (bAlpha = Find_Alpha(m_pMtrl[i].pTextureFilename))
				iPass = 14;
			if (bAlpha = Find_Alpha2(m_pMtrl[i].pTextureFilename))
				iPass = 26;

			_bool		bGravity = false;
			if (bGravity = Find_Gravity(m_pMtrl[i].pTextureFilename))
				iPass = 16;

			_bool		bPanel = false;
			if (bPanel = Find_Panel(m_pMtrl[i].pTextureFilename))
				iPass = 17;

			iCount = m_vecDrawSubset[i];
			if (!m_vecCustomDarwCount.empty())
				if (m_vecCustomDarwCount.find(iPass) != m_vecCustomDarwCount.end())
					iCount = m_vecCustomDarwCount[iPass][m_vecDrawSubset[i]];

			pEffect->SetTexture("g_BaseTexture", m_ppTextures[m_vecDrawSubset[i]]);
			pEffect->SetTexture("g_NormalTexture", m_ppNmTextures[i]);
			pEffect->SetTexture("g_MaterialTexture", m_ppMmTextures[i]);
			pEffect->SetTexture("g_EmissiveTexture", m_ppEmTextures[i]);
			pEffect->SetTexture("g_SpecularTexture", m_ppSmTextures[i]);

			if (m_mapPass.find(iPass) != m_mapPass.end())
			{
				auto& passiter = m_mapPass[iPass].find((_int)iCount);
				if (passiter != m_mapPass[iPass].end())
				{
					bPassChange = true;
					pEffect->EndPass();
					pEffect->BeginPass(passiter->second);
				}
			}
			else
			{
				pEffect->BeginPass(iPass);
			}

			if (m_mapTexture.find(iPass) != m_mapTexture.end())
			{
				auto& iterTexture = m_mapTexture[iPass].find((_int)iCount);
				if (iterTexture != m_mapTexture[iPass].end())
				{
					for (auto& tInfo : iterTexture->second)
					{
						if (tInfo.iPass != iPass)
						{
							bPassChange = true;
							pEffect->EndPass();
							pEffect->BeginPass(tInfo.iPass);
						}
						tInfo.pTexture->Set_Texture(pEffect, tInfo.ConstantTag.c_str());
					}
				}
			}
			pEffect->CommitChanges();

			m_pMesh->DrawSubset((_ulong)iCount);

			if (bPassChange)
			{
				pEffect->EndPass();
				pEffect->BeginPass(iPass);
				bPassChange = false;
			}

			pEffect->EndPass();
		}
	}
	else
	{
		for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		{
			iPass = iPassIndex;

			_bool		bAlpha = false;
			if (bAlpha = Find_Alpha(m_pMtrl[i].pTextureFilename))
				iPass = 14;
			if (bAlpha = Find_Alpha2(m_pMtrl[i].pTextureFilename))
				iPass = 26;
			_bool		bGravity = false;
			if (bGravity = Find_Gravity(m_pMtrl[i].pTextureFilename))
				iPass = 16;

			_bool		bPanel = false;
			if (bPanel = Find_Panel(m_pMtrl[i].pTextureFilename))
				iPass = 17;

			iCount = i;
			if (!m_vecCustomDarwCount.empty())
				if (m_vecCustomDarwCount.find(iPass) != m_vecCustomDarwCount.end())
					iCount = m_vecCustomDarwCount[iPass][i];

			pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
			pEffect->SetTexture("g_NormalTexture", m_ppNmTextures[i]);
			pEffect->SetTexture("g_MaterialTexture", m_ppMmTextures[i]);
			pEffect->SetTexture("g_EmissiveTexture", m_ppEmTextures[i]);
			pEffect->SetTexture("g_SpecularTexture", m_ppSmTextures[i]);

			if (m_mapPass.find(iPass) != m_mapPass.end())
			{
				auto& passiter = m_mapPass[iPass].find((_int)iCount);
				if (passiter != m_mapPass[iPass].end())
				{
					bPassChange = true;
					pEffect->EndPass();
					pEffect->BeginPass(passiter->second);
				}
			}
			else
			{
				pEffect->BeginPass(iPass );
			}

			if (m_mapTexture.find(iPass) != m_mapTexture.end())
			{
				auto& iterTexture = m_mapTexture[iPass].find((_int)iCount);
				if (iterTexture != m_mapTexture[iPass].end())
				{
					for (auto& tInfo : iterTexture->second)
					{
						if (tInfo.iPass != iPass)
						{
							bPassChange = true;
							pEffect->EndPass();
							pEffect->BeginPass(tInfo.iPass);
						}
						tInfo.pTexture->Set_Texture(pEffect, tInfo.ConstantTag.c_str());
					}
				}
			}

			pEffect->CommitChanges();

			m_pMesh->DrawSubset((_int)iCount);

			if (bPassChange)
			{
				pEffect->EndPass();
				pEffect->BeginPass(iPass);
				bPassChange = false;
			}

			pEffect->EndPass();
		}
	}
	m_vecDrawSubset.clear();
}

_bool CStaticMesh::Find_Normal(char * pFileName)
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

	return false;
}

_bool CStaticMesh::Find_Material(char * pFileName)
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

	return false;
}

_bool CStaticMesh::Find_Emissive(char * pFileName)
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

	return false;
}

_bool CStaticMesh::Find_Specular(char * pFileName)
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

	return false;
}

_bool CStaticMesh::Find_Alpha(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 2] == 'A')
			{
				return true;
			}
		}
	}

	return false;
}
_bool CStaticMesh::Find_Alpha2(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 2] == 'B')
			{
				return true;
			}
		}
	}

	return false;
}


_bool CStaticMesh::Find_Gravity(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 2] == 'G')
			{
				return true;
			}
		}
	}

	return false;
}

_bool CStaticMesh::Find_Panel(char * pFileName)
{
	size_t	iLength = strlen(pFileName);

	for (size_t i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 2] == 'P')
			{
				return true;
			}
		}
	}

	return false;
}

void CStaticMesh::Set_Buffer(vector<_vec3> vecStart, vector<_vec3> vecEnd)
{
	m_pMesh->GetVertexBuffer(&m_pVB);

	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		pVertex[i].vPosition = vecStart[i];
		pVertex[i].vTexUV = _vec2(0.f, 0.f);
	}

	m_pVB->Unlock();

}

void Engine::CStaticMesh::Get_Texture_Clone(LPDIRECT3DTEXTURE9& ppTexture, _tchar* szFileName, _tchar* szFullPath)
{
	if (FAILED(CResourcesMgr::GetInstance()->Ready_Texture(m_pGraphicDev, m_wSceneIndex, szFileName, TEX_NORMAL, szFullPath)))
	{
		auto* pTexture = CResourcesMgr::GetInstance()->Clone_Resource(m_wSceneIndex, szFileName);
		if (pTexture)
			ppTexture = pTexture->Is<CTexture>()->Get_Texture();
		else
			ppTexture = nullptr;
		Safe_Release(pTexture);
	}
	else
	{
		auto* pTexture = CResourcesMgr::GetInstance()->Clone_Resource(m_wSceneIndex, szFileName);
		if (pTexture)
			ppTexture = pTexture->Is<CTexture>()->Get_Texture();
		else
			ppTexture = nullptr;
		Safe_Release(pTexture);
	}
}

const Engine::_bool Engine::CStaticMesh::Read_BakePx_From_File(const string& tagFile)
{
	PxDefaultFileInputData In(tagFile.c_str());
	if (!In.isValid())
		return false;

	//PxU8* pBuffer = new PxU8[In.getLength()]{};

	//In.read(pBuffer, In.getLength());
	m_pPxMesh = CPhys_Manager::GetInstance()->Get_Physics()->createTriangleMesh(In);

	Build_Info();
	Build_Tree();

	//Safe_Delete_Array(pBuffer);
	return true;
}

bool Engine::CStaticMesh::Find_StringTag(const std::string& FileName, const std::string& tag)
{
	return  std::string::npos != FileName.find(tag);
}

void Engine::CStaticMesh::Culling(CFrustum* pFrustum, _float fScale)
{
	Cull_Subset(pFrustum, m_pRootNode, fScale);
	m_bIsMap = true;
}

void Engine::CStaticMesh::Cull_Subset(CFrustum* pFrustum, TREENode* pNode, _float fScale)
{
	if (pFrustum->IsIn_FrustumForObject(&(pNode->vPos*fScale), 10))
	{
		m_vecDrawSubset.emplace_back(pNode->iSubsetCount);
	}
	if (pNode->pLeft)
		Cull_Subset(pFrustum, pNode->pLeft, fScale);
	if (pNode->pRight)
		Cull_Subset(pFrustum, pNode->pRight, fScale);
}


void Engine::CStaticMesh::Free_Tree(TREENode* pNode)
{
	if (!pNode)
		return;
	if (pNode->pLeft)
		Free_Tree(pNode->pLeft);
	if (pNode->pRight)
		Free_Tree(pNode->pRight);
	Safe_Delete(pNode);
}



void Engine::CStaticMesh::Sort_Point(vector<SUBPOINT>& pVecPoint, const _bool bFlag)
{
	if (bFlag)
	{
		sort(pVecPoint.begin(), pVecPoint.end(), [](SUBPOINT& first, SUBPOINT& second)
		{
			return first.vPos.x > second.vPos.x;
		}
		);
	}
	else
	{
		sort(pVecPoint.begin(), pVecPoint.end(), [](SUBPOINT& first, SUBPOINT& second)
		{
			return first.vPos.z > second.vPos.z;
		}
		);
	}
}

void Engine::CStaticMesh::Create_SubTree(TREENode* pParent, TREENode* pSelf, vector<SUBPOINT> vecPoint, const _bool bFlag)
{
	if (vecPoint.size() <= 0)
	{
		SAFE_DELETE(pSelf);
		return;
	}

	pSelf->pParent = pParent;
	pSelf->pLeft = nullptr;
	pSelf->pRight = nullptr;

	Sort_Point(vecPoint, bFlag);
	if (vecPoint.size() == 1)
	{
		pSelf->vPos = vecPoint.front().vPos;
		pSelf->iSubsetCount = vecPoint.front().iSubsetCount;
		return;
	}

	size_t middleIndex = vecPoint.size() / 2;

	pSelf->vPos = vecPoint[middleIndex].vPos;
	pSelf->iSubsetCount = vecPoint[middleIndex].iSubsetCount;
	vector<SUBPOINT> vecLeft;
	vector<SUBPOINT> vecRight;
	for (size_t i = 0; i < middleIndex; ++i)
	{
		vecLeft.emplace_back(vecPoint[i]);
	}
	if (vecLeft.size() != 0)
	{
		pSelf->pLeft = new TREENode;
		Create_SubTree(pSelf, pSelf->pLeft, vecLeft, !bFlag);
	}

	if (middleIndex + 1 >= vecPoint.size())
		return;


	for (size_t i = middleIndex + 1; i < vecPoint.size(); ++i)
	{
		vecRight.emplace_back(vecPoint[i]);
	}
	if (vecRight.size() != 0)
	{
		pSelf->pRight = new TREENode;
		Create_SubTree(pSelf, pSelf->pRight, vecRight, !bFlag);
	}

}

void Engine::CStaticMesh::Search_Tree(TREENode* pTree, const _vec3 vPos, const _bool bFlag /*=false*/)
{
	if (!pTree->pLeft && !pTree->pRight)
		return;
	if (bFlag)
	{
		if (pTree->vPos.x > vPos.x)
			Search_Tree(pTree->pLeft, vPos, !bFlag);
		else
			Search_Tree(pTree->pRight, vPos, !bFlag);
	}
	else
	{
		if (pTree->vPos.z > vPos.z)
			Search_Tree(pTree->pLeft, vPos, !bFlag);
		else
			Search_Tree(pTree->pRight, vPos, !bFlag);
	}
}

void Engine::CStaticMesh::Build_Tree()
{
	if ((m_vecSubPOINT.size() == 0))
		return;
	m_pRootNode = new TREENode;
	Sort_Point(m_vecSubPOINT, true);
	size_t middleIndex = m_vecSubPOINT.size() / 2;
	m_pRootNode->vPos = m_vecSubPOINT[middleIndex].vPos;
	m_pRootNode->iSubsetCount = m_vecSubPOINT[middleIndex].iSubsetCount;
	vector<SUBPOINT> vecLeft;
	vector<SUBPOINT> vecRight;
	for (size_t i = 0; i < middleIndex - 1; ++i)
	{
		vecLeft.emplace_back(m_vecSubPOINT[i]);
	}

	m_pRootNode->pLeft = new TREENode;
	Create_SubTree(m_pRootNode, m_pRootNode->pLeft, vecLeft, false);
	for (size_t i = middleIndex + 1; i < m_vecSubPOINT.size(); ++i)
	{
		vecRight.emplace_back(m_vecSubPOINT[i]);
	}
	m_pRootNode->pRight = new TREENode;
	Create_SubTree(m_pRootNode, m_pRootNode->pRight, vecRight, false);

}

void Engine::CStaticMesh::Build_Info()
{
	D3DXATTRIBUTERANGE* tAttRange = new D3DXATTRIBUTERANGE[m_dwSubsetCnt];
	_ulong dwAttCnt;
	m_pOriMesh->GetAttributeTable(tAttRange, &dwAttCnt);
	_ulong dwOffset = 0, dwCount = 0;
	_vec3 vPos, vTotal;
	if (dwAttCnt == m_dwSubsetCnt)
	{

		for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		{
			dwOffset = tAttRange[i].VertexStart;
			dwCount = tAttRange[i].VertexCount;
			SUBSETINFO tInfo;
			SUBPOINT tPoint[EDGE_END];
			ZeroMemory(&tPoint, sizeof(SUBPOINT) * EDGE_END);
			ZeroMemory(&tInfo, sizeof(SUBSETINFO));
			for (_ulong j = 0; j < dwCount; ++j)
			{
				vPos = m_pVtxPos[dwOffset + j];
				if (tInfo.vEDGE[FLB].x > vPos.x)
					tInfo.vEDGE[FLB].x = vPos.x;

				if (tInfo.vEDGE[FLB].y > vPos.y)
					tInfo.vEDGE[FLB].y = vPos.y;

				if (tInfo.vEDGE[FLB].z > vPos.z)
					tInfo.vEDGE[FLB].z = vPos.z;


				if (tInfo.vEDGE[BRT].x < vPos.x)
					tInfo.vEDGE[BRT].x = vPos.x;

				if (tInfo.vEDGE[BRT].y < vPos.y)
					tInfo.vEDGE[BRT].y = vPos.y;

				if (tInfo.vEDGE[BRT].z < vPos.z)
					tInfo.vEDGE[BRT].z = vPos.z;

				vTotal += vPos;
			}
			tInfo.iSubsetCount = i;
			//6개정보
			//앞면

			tInfo.vEDGE[FRB] = tInfo.vEDGE[FLB];
			tInfo.vEDGE[FRB].x = tInfo.vEDGE[BRT].x;

			tInfo.vEDGE[FLT] = tInfo.vEDGE[FLB];
			tInfo.vEDGE[FLT].y = tInfo.vEDGE[BRT].y;

			tInfo.vEDGE[FRT] = tInfo.vEDGE[BRT];
			tInfo.vEDGE[FRT].z = tInfo.vEDGE[FLB].z;

			tInfo.vEDGE[BLB] = tInfo.vEDGE[FLB];
			tInfo.vEDGE[BLB].z = tInfo.vEDGE[BRT].z;

			tInfo.vEDGE[BRB] = tInfo.vEDGE[BRT];
			tInfo.vEDGE[BRB].y = tInfo.vEDGE[FLB].y;

			tInfo.vEDGE[BLT] = tInfo.vEDGE[BRT];
			tInfo.vEDGE[BLT].x = tInfo.vEDGE[FLB].x;

			//tInfo.vCenterPos = vTotal / (_float)dwCount;

			for (int j = 0; j < EDGE_END; ++j)
			{
				tPoint[j].iSubsetCount = i;
				tPoint[j].vPos = tInfo.vEDGE[j];
				m_vecSubPOINT.emplace_back(tPoint[j]);
			}

			m_vecSubSet.emplace_back(tInfo);
		}
	}
	Safe_Delete_Array(tAttRange);
}
Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName, const _ushort wSceneIndex)
{
	CStaticMesh* pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName, wSceneIndex)))
		Safe_Release(pInstance);

	return pInstance;
}


CComponent* Engine::CStaticMesh::Clone(void* pArg)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pOriMesh);

	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		Safe_Release(m_ppTextures[i]);
		Safe_Release(m_ppNmTextures[i]);
		Safe_Release(m_ppMmTextures[i]);
		Safe_Release(m_ppEmTextures[i]);
		Safe_Release(m_ppSmTextures[i]);
	}

	Safe_Delete_Array(m_ppTextures);
	Safe_Delete_Array(m_ppNmTextures);
	Safe_Delete_Array(m_ppMmTextures);
	Safe_Delete_Array(m_ppEmTextures);
	Safe_Delete_Array(m_ppSmTextures);

	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pVtxPos);
		Safe_Delete_Array(m_pIndex16);
		Safe_Delete_Array(m_pIndex32);
		Free_Tree(m_pRootNode);
	}
	CMesh::Free();
}

