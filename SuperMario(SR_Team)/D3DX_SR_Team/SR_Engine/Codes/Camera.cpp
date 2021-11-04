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
    // TODO: 여기에 return 문을 삽입합니다.
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

    /* 뷰행렬 만들고 장치에게 전달 */
    D3DXMatrixLookAtLH(
        &m_CameraDesc.matView, /* 뷰행렬 반환 */
        &m_CameraDesc.vEye, /* Eye: 카메라(관찰자)의 월드상 위치 */
        &m_CameraDesc.vAt, /* At: 카메라(관찰자)가 바라볼 월드상 위치 */
        &m_CameraDesc.vUp /* Up: 월드상의 절대축. 카메라의 상태행렬 만들기 위해 필요한 재료 */);

    if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_CameraDesc.matView)))
        return E_FAIL;

    ///* 투영행렬 만들고 장치에게 전달 */
    D3DXMatrixPerspectiveFovLH(
        &m_CameraDesc.matProj, /* 원근투영행렬 반환 */
        m_CameraDesc.fFovY, /* FovY: Y축의 Field Of View (Y축 시야각) */
        m_CameraDesc.fAspect, /* Aspect: 종횡비 */
        m_CameraDesc.fZNear, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
        m_CameraDesc.fZFar /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);

    if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matProj)))
        return E_FAIL;


    return _uint();
}

void CCamera::Set_Ortho()
{
    //m_CameraDesc.fZFar = 0.f;
    //m_CameraDesc.fZFar = 1.f;

    D3DXMatrixOrthoLH(
        &m_CameraDesc.matOrtho, /* 원근투영행렬 반환 */
        WINCX, // 가로폭 (WINCX, 하드코딩한거 반드시 바꿀것)
        WINCY, // 세로폭 (WINCY)
        m_CameraDesc.fZNear, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
        m_CameraDesc.fZFar /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);

}

void CCamera::Set_Ortho_For_UI(_float3 vScale, _float3 vPos)
{
    _mat matWorld, matView, matProj;

    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixIdentity(&matView);
    D3DXMatrixIdentity(&matProj);

	D3DXMatrixOrthoLH(
		&m_CameraDesc.matOrtho, /* 직교투영행렬 반환 */
		WINCX, // 가로폭 (WINCX, 하드코딩한거 반드시 바꿀것)
		WINCY, // 세로폭 (WINCY)
		m_CameraDesc.fZNear, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
		m_CameraDesc.fZFar /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);

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
