#include "..\Headers\Camera.h"

using namespace Engine;

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
    : CGameObject(pDevice)
{
    ZeroMemory(&m_CameraDesc, sizeof(CAMERA_DESC));
}

CCamera::CCamera(const CCamera& other)
    : CGameObject(other)
    , m_CameraDesc(other.m_CameraDesc)
{
}

const CCamera::CAMERA_DESC& CCamera::Get_CameraDesc() const
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return m_CameraDesc;
}

HRESULT CCamera::Ready_GameObject_Prototype()
{
    if (FAILED(CGameObject::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCamera::Ready_GameObject(void* pArg)
{
    if (FAILED(CGameObject::Ready_GameObject(pArg)))
        return E_FAIL;

    if (pArg != nullptr)
    {
        memcpy(&m_CameraDesc, pArg, sizeof(CAMERA_DESC));
    }

    return S_OK;
}

_uint CCamera::Update_GameObject(_float fDeltaTime)
{
    CGameObject::Update_GameObject(fDeltaTime);

    return _uint();
}

_uint CCamera::LateUpdate_GameObject(_float fDeltaTime)
{
    CGameObject::LateUpdate_GameObject(fDeltaTime);

    /* ����� ����� ��ġ���� ���� */
    D3DXMatrixLookAtLH(
        &m_CameraDesc.matView, /* ����� ��ȯ */
        &m_CameraDesc.vEye, /* Eye: ī�޶�(������)�� ����� ��ġ */
        &m_CameraDesc.vAt, /* At: ī�޶�(������)�� �ٶ� ����� ��ġ */
        &m_CameraDesc.vUp /* Up: ������� ������. ī�޶��� ������� ����� ���� �ʿ��� ��� */);

    if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_CameraDesc.matView)))
        return E_FAIL;

    ///* ������� ����� ��ġ���� ���� */
    D3DXMatrixPerspectiveFovLH(
        &m_CameraDesc.matProj, /* ����������� ��ȯ */
        m_CameraDesc.fFovY, /* FovY: Y���� Field Of View (Y�� �þ߰�) */
        m_CameraDesc.fAspect, /* Aspect: ��Ⱦ�� */
        m_CameraDesc.fZNear, /* Near Z: �����ڿ� ���� ����� ����� Z�Ÿ� */
        m_CameraDesc.fZFar /* Far Z: �����ڿ� ���� �� ����� Z�Ÿ� */);

    if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matProj)))
        return E_FAIL;


    return _uint();
}

void CCamera::Set_Ortho()
{
    //m_CameraDesc.fZFar = 0.f;
    //m_CameraDesc.fZFar = 1.f;

    D3DXMatrixOrthoLH(
        &m_CameraDesc.matOrtho, /* ����������� ��ȯ */
        WINCX, // ������ (WINCX, �ϵ��ڵ��Ѱ� �ݵ�� �ٲܰ�)
        WINCY, // ������ (WINCY)
        m_CameraDesc.fZNear, /* Near Z: �����ڿ� ���� ����� ����� Z�Ÿ� */
        m_CameraDesc.fZFar /* Far Z: �����ڿ� ���� �� ����� Z�Ÿ� */);

}

void CCamera::Set_Ortho_For_UI(_float3 vScale, _float3 vPos)
{
    _mat matWorld, matView, matProj;

    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixIdentity(&matView);
    D3DXMatrixIdentity(&matProj);

	D3DXMatrixOrthoLH(
		&m_CameraDesc.matOrtho, /* ����������� ��ȯ */
		WINCX, // ������ (WINCX, �ϵ��ڵ��Ѱ� �ݵ�� �ٲܰ�)
		WINCY, // ������ (WINCY)
		m_CameraDesc.fZNear, /* Near Z: �����ڿ� ���� ����� ����� Z�Ÿ� */
		m_CameraDesc.fZFar /* Far Z: �����ڿ� ���� �� ����� Z�Ÿ� */);

    matProj = m_CameraDesc.matOrtho;


    matView._11 = vScale.x;
    matView._22 = vScale.y;
    matView._33 = vScale.z;

    matView._41 = vPos.x;
    matView._42 = vPos.y;
    matView._43 = vPos.z;

    m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pDevice->SetTransform(D3DTS_VIEW, &matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

}

_float3 CCamera::Get_AtVector()
{
    _float3 vAns = m_CameraDesc.vAt - m_CameraDesc.vEye;
    D3DXVec3Normalize(&vAns, &vAns);
    return vAns;
}

HRESULT CCamera::Render_GameObject()
{
    return S_OK;
}

void CCamera::Free()
{
    CGameObject::Free();
}
