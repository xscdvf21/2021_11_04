#include "VIBuffer.h"
#include "Camera.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwFVF(0)
	, m_dwIdxSize(0)
	, m_dwTriCnt(0)
{
	
}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
															0, // ���� ���� ��� �� ���� 0(D3DUSAGE_DYNAMIC : ��ƼŬ�� ������ ��)
															m_dwFVF,
															D3DPOOL_MANAGED, // �Ϲ������� ���� ���� ��� �� Managed, �������� ��� �� Default ���
															&m_pVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
														0, 
														m_IdxFmt, 
														D3DPOOL_MANAGED, 
														&m_pIB, NULL), E_FAIL);
	
	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

HRESULT CVIBuffer::Translation_ToLocalSpace(HWND hWnd, _uint iWinCX, _uint iWinCY, CCamera * pCamera, _matrix * pWorldMatrix)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	/* ����Ʈ -> ���������̽� */
	_vec3 vMouse = _vec3(0.f, 0.f, 0.f);
	vMouse.x = ptMouse.x / (iWinCX * 0.5f) - 1.f;
	vMouse.y = 1.f - ptMouse.y / (iWinCY * 0.5f);

	/* ���������̽� -> �佺���̽� */
	_matrix matProj = pCamera->Get_Projection();
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = vMouse - m_vRayPos;

	/* �佺���̽� -> ���彺���̽� */
	_matrix matView = pCamera->Get_View();
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	/* ���彺���̽� -> ���ý����̽� */
	_matrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, pWorldMatrix);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matInvWorld);

	return S_OK;
}

void Engine::CVIBuffer::Free(void)
{
	if (!m_bCheck)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}
	m_bCheck = false;

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CResources::Free();
}

