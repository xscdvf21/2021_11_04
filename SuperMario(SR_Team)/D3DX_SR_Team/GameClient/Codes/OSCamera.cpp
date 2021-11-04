#include "framework.h"
#include "OSCamera.h"

COSCamera::COSCamera(_Device pDevice)
    : CCamera(pDevice)
{
}

COSCamera::COSCamera(const COSCamera& other)
    : CCamera(other)
{
}

HRESULT COSCamera::Ready_GameObject_Prototype()
{
    if (FAILED(CCamera::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT COSCamera::Ready_GameObject(void* pArg)
{
    if (FAILED(CCamera::Ready_GameObject(pArg)))
        return E_FAIL;


    return S_OK;
}

_uint COSCamera::Update_GameObject(_float fDeltaTime)
{
    CCamera::Update_GameObject(fDeltaTime);

    m_CameraDesc;

    return 0;
}

_uint COSCamera::LateUpdate_GameObject(_float fDeltaTime)
{
    CCamera::LateUpdate_GameObject(fDeltaTime);

    return 0;
}

HRESULT COSCamera::Render_GameObject()
{
    if (FAILED(CCamera::Render_GameObject()))
        return E_FAIL;


    return S_OK;
}

COSCamera* COSCamera::Create(_Device pDevice)
{
    COSCamera* pInstance = new COSCamera(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create COSCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* COSCamera::Clone(void* pArg)
{
    COSCamera* pInstance = new COSCamera(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone COSCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void COSCamera::Free()
{

}
