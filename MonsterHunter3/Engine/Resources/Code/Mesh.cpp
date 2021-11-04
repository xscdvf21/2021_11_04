#include "Mesh.h"

USING(Engine)

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CResources(pGraphicDev)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
	:CResources(rhs)
{

}

Engine::CMesh::~CMesh(void)
{

}

void Engine::CMesh::Free(void)
{
	CResources::Free();
}

