#include "Mesh.h"

USING(Engine)

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResources(pGraphicDev)
	, m_pPxMesh(nullptr)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
	:CResources(rhs)
	,m_pPxMesh(rhs.m_pPxMesh)
{

}

Engine::CMesh::~CMesh(void)
{

}

void Engine::CMesh::Free(void)
{
	CResources::Free();
}

void Engine::CMesh::Add_Texture(const _int& iIndex, const _int& iSubsetIndex, const std::string& tagConstanteTable, CTexture* pTexture, const _uint& iPass)
{
	TexInfo tTexinfo;
	tTexinfo.ConstantTag = tagConstanteTable;
	tTexinfo.pTexture = pTexture;
	tTexinfo.iPass = iPass;

	auto& iter = m_mapTexture.find(iIndex);
	if (iter == m_mapTexture.end())
	{
		map<_int, vector<TexInfo>> mapTexture;
		vector<TexInfo> vecTex;
		vecTex.emplace_back(tTexinfo);
		mapTexture.emplace(iSubsetIndex, vecTex);
		m_mapTexture.emplace(iIndex, mapTexture);
	}
	else
	{
		auto& itermap = iter->second.find(iSubsetIndex);
		if (itermap == iter->second.end())
		{
			vector<TexInfo> vecTex;
			vecTex.emplace_back(tTexinfo);

			iter->second.emplace(iSubsetIndex, vecTex);
		}
		else
		{
			itermap->second.emplace_back(tTexinfo);
		}
	}
}

