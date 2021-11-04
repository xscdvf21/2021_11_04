#include "Sphere.h"
#include "GameObject.h"
USING(Engine)

CSphere::CSphere(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev),
	m_pMesh(nullptr),
	m_pAdjacency(nullptr),
	m_fRadius(0.f),
	m_iSlices(0),
	m_iStack(0)
{

}

CSphere::CSphere(const CSphere & rhs)
	: CResources(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_fRadius(rhs.m_fRadius)
	, m_iSlices(rhs.m_iSlices)
	, m_iStack(rhs.m_iStack)
{


	m_pTextureGreen[0] = rhs.m_pTextureGreen[0];
	m_pTextureGreen[1] = rhs.m_pTextureGreen[1];



	m_pMesh->AddRef();
	m_pAdjacency->AddRef();

	m_pTextureGreen[0]->AddRef();
	m_pTextureGreen[1]->AddRef();
}

CSphere::~CSphere()
{
}


HRESULT CSphere::Ready_Buffer(const _float & _Radius, const _uint & _Slices, const _uint & _Stack, const LPD3DXMESH _pMesh, const LPD3DXBUFFER _pAdjacency)
{

	m_fRadius = _Radius;
	m_iSlices = _Slices;
	m_iStack = _Stack;
	m_pMesh = _pMesh;
	m_pAdjacency = _pAdjacency;

	
	D3DXCreateSphere(m_pGraphicDev, m_fRadius, m_iSlices, m_iStack, &m_pMesh, &m_pAdjacency);

	D3DXCreateTexture(m_pGraphicDev,
		1,
		1,
		1, // miplevel
		0,	// 텍스처의 용도
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTextureGreen[0]);


	D3DLOCKED_RECT LockRect;
	m_pTextureGreen[0]->LockRect(0, &LockRect, NULL, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	
	m_pTextureGreen[0]->UnlockRect(0);


	D3DXCreateTexture(m_pGraphicDev,
		1,
		1,
		1, // miplevel
		0,	// 텍스처의 용도
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTextureGreen[1]);

	LockRect;
	m_pTextureGreen[1]->LockRect(0, &LockRect, NULL, 0);

	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pTextureGreen[1]->UnlockRect(0);


	return S_OK;
}

void CSphere::Render_Buffer(const _matrix* Parent, _vec3 _vScale, _vec3 _vPos, _uint _ii)
{

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	//_vec3  vScale = { (*Parent)._11,(*Parent)._22, (*Parent)._33 };
	_vec3  vPos = { (*Parent)._41,(*Parent)._42,(*Parent)._43 };


	//matWorld._11 = vScale.x;
	//matWorld._22 = vScale.y;
	//matWorld._33 = vScale.z;

	matWorld._11 = _vScale.x;
	matWorld._22 = _vScale.y;
	matWorld._33 = _vScale.z;

	matWorld._41 = vPos.x + _vPos.x;
	matWorld._42 = vPos.y + _vPos.y;
	matWorld._43 = vPos.z + _vPos.z;

	//D3DXMatrixTranslation(&matWorld,vTemp);
	//matWorld = *Parent * matWorld;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	
	
	m_pGraphicDev->SetTexture(0, m_pTextureGreen[_ii]);
	m_pMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CSphere::Render_Buffer(void)
{


	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);


	m_pMesh->DrawSubset(0);
	

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	/*Safe_Release(pTexture);*/

}

CSphere * CSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & _Radius, const _uint & _Slices, const _uint & _Stack,const  LPD3DXMESH  _pMesh, const LPD3DXBUFFER  _pAdjacency)
{

	CSphere*		pInstance = new CSphere(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_Radius, _Slices, _Stack, _pMesh, _pAdjacency)))
		Safe_Release(pInstance);


	return pInstance;
}


void CSphere::Free(void)
{
	//Safe_Release(m_pMesh);
	//Safe_Release(m_pAdjacency);

	Safe_Release(m_pMesh);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pTextureGreen[0]);
	Safe_Release(m_pTextureGreen[1]);
	CResources::Free();
}

CComponent * CSphere::Clone(void)
{
	return new CSphere(*this);
}
