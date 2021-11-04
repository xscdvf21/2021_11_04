#include "stdafx.h"
#include "..\Headers\VIBufferTool.h"





CVIBufferTool::CVIBufferTool(LPDIRECT3DDEVICE9 pDevice)
	: CComponentTool(pDevice)
	, m_pVB(nullptr)
	, m_iVertexSize(0)
	, m_iVertexCount(0)
	, m_iFVF(0)
	, m_iTriCount(0)
	, m_pIB(nullptr)
	, m_iIndexSize(0)
	, m_IndexFmt(D3DFMT_INDEX16)
	, m_pVertexArr(nullptr)
	, m_pIndexArr(nullptr)
	, m_IsClone(false)
{
}

CVIBufferTool::CVIBufferTool(const CVIBufferTool & other)
	: CComponentTool(other)
	, m_pVB(other.m_pVB)
	, m_iVertexSize(other.m_iVertexSize)
	, m_iVertexCount(other.m_iVertexCount)
	, m_iFVF(other.m_iFVF)
	, m_iTriCount(other.m_iTriCount)
	, m_pIB(other.m_pIB)
	, m_iIndexSize(other.m_iIndexSize)
	, m_IndexFmt(other.m_IndexFmt)
	, m_pVertexArr(other.m_pVertexArr)
	, m_pIndexArr(other.m_pIndexArr)
	, m_IsClone(true)
{
}

HRESULT CVIBufferTool::Ready_Component_Prototype()
{
	/* 버텍스 버퍼 생성 */
	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_iVertexSize * m_iVertexCount,
		0, 
		m_iFVF,
		D3DPOOL_MANAGED, 
		&m_pVB, 
		0)))
	{
		PRINT_LOG(L"Error", L"Failed To CreateVertexBuffer");
		return E_FAIL;
	}

	/* 인덱스 버퍼 생성 */
	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_iIndexSize * m_iTriCount, /* 인덱스 배열의 메모리 총 사이즈 */
		0, /*정적버퍼? 동적버퍼? D3DUSAGE_DYNAMIC이 아닌 경우는 모두 정적버퍼 */
		m_IndexFmt, /* 인덱스 포맷 */
		D3DPOOL_MANAGED, /* 메모리 보관 방식 */
		&m_pIB, /* 생성된 버텍스 버퍼 컴객체의 주소를 반환 */
		0)))
	{
		PRINT_LOG(L"Error", L"Failed To CreateIndexBuffer");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CVIBufferTool::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CVIBufferTool::Render_VIBuffer()
{
	/* 그리기 전 장치에게 버텍스버퍼를 전달 */
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVB, 0, m_iVertexSize)))
	{
		PRINT_LOG(L"Error", L"Failed To SetStreamSource");
		return E_FAIL;
	}

	/* 장치에게 FVF 전달 */
	if (FAILED(m_pDevice->SetFVF(m_iFVF)))
	{
		PRINT_LOG(L"Error", L"Failed To SetFVF");
		return E_FAIL;
	}

	/* 장치에게 인덱스 버퍼 전달 */
	if (FAILED(m_pDevice->SetIndices(m_pIB)))
	{
		PRINT_LOG(L"Error", L"Failed To SetIndices");
		return E_FAIL;
	}

	return S_OK;
}

CComponentTool * CVIBufferTool::Clone(void * pArg)
{
	return nullptr;
}

void CVIBufferTool::Free()
{/* 클론이 아닌 프로토타입에서 해당 배열 삭제 */
	if (!m_IsClone)
	{
		Safe_Delete_Array(m_pIndexArr);
		Safe_Delete_Array(m_pIndexArr);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponentTool::Free();
}
