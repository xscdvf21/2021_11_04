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
	/* ���ؽ� ���� ���� */
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

	/* �ε��� ���� ���� */
	if (FAILED(m_pDevice->CreateIndexBuffer(
		m_iIndexSize * m_iTriCount, /* �ε��� �迭�� �޸� �� ������ */
		0, /*��������? ��������? D3DUSAGE_DYNAMIC�� �ƴ� ���� ��� �������� */
		m_IndexFmt, /* �ε��� ���� */
		D3DPOOL_MANAGED, /* �޸� ���� ��� */
		&m_pIB, /* ������ ���ؽ� ���� �İ�ü�� �ּҸ� ��ȯ */
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
	/* �׸��� �� ��ġ���� ���ؽ����۸� ���� */
	if (FAILED(m_pDevice->SetStreamSource(0, m_pVB, 0, m_iVertexSize)))
	{
		PRINT_LOG(L"Error", L"Failed To SetStreamSource");
		return E_FAIL;
	}

	/* ��ġ���� FVF ���� */
	if (FAILED(m_pDevice->SetFVF(m_iFVF)))
	{
		PRINT_LOG(L"Error", L"Failed To SetFVF");
		return E_FAIL;
	}

	/* ��ġ���� �ε��� ���� ���� */
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
{/* Ŭ���� �ƴ� ������Ÿ�Կ��� �ش� �迭 ���� */
	if (!m_IsClone)
	{
		Safe_Delete_Array(m_pIndexArr);
		Safe_Delete_Array(m_pIndexArr);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponentTool::Free();
}
