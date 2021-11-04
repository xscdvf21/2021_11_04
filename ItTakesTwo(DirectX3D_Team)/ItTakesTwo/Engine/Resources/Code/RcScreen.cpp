#include "RcScreen.h"

USING(Engine)

Engine::CRcScreen::CRcScreen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcScreen::CRcScreen(const CRcScreen& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcScreen::~CRcScreen(void)
{

}

HRESULT Engine::CRcScreen::Ready_Buffer(void)
{


	return S_OK;
}

void Engine::CRcScreen::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}



HRESULT Engine::CRcScreen::Ready_Buffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{

	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXSCREEN);
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwFVF = FVF_SCREEN;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(fX - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4((fX + fSizeX) - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4((fX + fSizeX) - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(fX - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);


	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

Engine::CRcScreen* Engine::CRcScreen::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _bool& bScreen /*=false */, const _float& fX/*=0*/, const _float& fY/*=0*/, const _float& fSizeX/*=1920*/, const _float& fSizeY/*=1080*/)
{
	CRcScreen*		pInstance = new CRcScreen(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(fX, fY, fSizeX, fSizeY)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CRcScreen::Clone(void* pArg)
{
	return new CRcScreen(*this);
}

Engine::_mat Engine::CRcScreen::Get_Scale()
{
	_mat matScale;
	return matScale;
}

Engine::_mat Engine::CRcScreen::Get_Pos()
{
	_mat matTrans;
	return matTrans;
}

void Engine::CRcScreen::Free(void)
{
	CVIBuffer::Free();
}

