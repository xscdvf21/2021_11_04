#include "framework.h"
#include "SupportCam.h"
#include "Player.h"

CSupportCamera::CSupportCamera(_Device pDevice)
    : CCamera(pDevice)
    , m_eSceneType(ESceneType::Stage)
{
}

CSupportCamera::CSupportCamera(const CSupportCamera& other)
    : CCamera(other)
    , m_eSceneType(ESceneType::TestStage)       //삽입해줄것
{
}

HRESULT CSupportCamera::Ready_GameObject_Prototype()
{
    if (FAILED(CCamera::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSupportCamera::Ready_GameObject(void* pArg)
{
    if (FAILED(CCamera::Ready_GameObject(pArg)))
        return E_FAIL;

    m_fSetCameraRotate = 0.f;
    m_fNowCameraRotate = 0.f;

    return S_OK;
}

_uint CSupportCamera::Update_GameObject(_float fDeltaTime)
{
    CCamera::Update_GameObject(fDeltaTime);

    if (FAILED(KeyInput(fDeltaTime)))
        return 0;

    if (FAILED(Movement(fDeltaTime)))
        return 0;

    return _uint();
}

_uint CSupportCamera::LateUpdate_GameObject(_float fDeltaTime)
{
    CCamera::LateUpdate_GameObject(fDeltaTime);

    return 0;
}

HRESULT CSupportCamera::Render_GameObject()
{
    //안씀.
    if (FAILED(CCamera::Render_GameObject()))
        return E_FAIL;

    return S_OK;
}

void CSupportCamera::Cam_Rotate_LEFT()
{
    m_fSetCameraRotate -= 90.f;
}

void CSupportCamera::Cam_Rotate_RIGHT()
{
    m_fSetCameraRotate += 90.f;
}

HRESULT CSupportCamera::KeyInput(_float fDeltaTime)
{
    CManagement* pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;

    if (pManagement->Key_Up(KEY_A))
    {
        Cam_Rotate_LEFT();
    }

    if (pManagement->Key_Up(KEY_D))
    {
        Cam_Rotate_RIGHT();
    }


    return S_OK;
}

HRESULT CSupportCamera::Movement(_float fDeltaTime)
{
    CManagement* pManagement = CManagement::Get_Instance();

    if (nullptr == pManagement)
    {
        return E_FAIL;
    }

    //플레이어 정보 얻어옴

    CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(
        (_uint)m_eSceneType, L"Layer_Player", L"Com_Transform");
    if (nullptr == pPlayerTransform)
    {
        PRINT_LOG(L"Error", L"Not found Player`s Transform at CSupportCam::Movement");
        return E_FAIL;
    }
    
    m_CameraDesc.vEye.y = pPlayerTransform->Get_Position().y;
    

    //플레이어 정보를 통해 at 지정

    //카메라 각도 회전부
    float fRotate = fDeltaTime * 90.f * 6.f;

    if (abs(m_fSetCameraRotate - m_fNowCameraRotate) >= (fRotate))
    {
        if (m_fSetCameraRotate > m_fNowCameraRotate)
        {
            ;
        }
        else
        {
            fRotate *= -1;
        }
        m_fNowCameraRotate += fRotate;
    }
    else
    {
        fRotate = 0.f;
    }
    //회전부 끝

    //카메라 Eye 위치 지정
    _mat matWorld, matRotY, matParent;
    _vec3 _vPos = m_CameraDesc.vEye;
    _float3 vLook = m_CameraDesc.vEye - m_CameraDesc.vAt;       //목적지 - 출발지
    D3DXVec3Normalize(&vLook, &vLook);
    vLook *= 10.f;

    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixRotationY(&matRotY, D3DXToRadian(fRotate));		//회전 먹이고
    D3DXMatrixTranslation(&matParent, pPlayerTransform->Get_Position().x, pPlayerTransform->Get_Position().y, pPlayerTransform->Get_Position().z);			//부모 위치 먹이고
    matWorld = matRotY * matParent;
    D3DXVec3TransformCoord(&vLook, &vLook, &matWorld);			//좌표 받아서

    m_CameraDesc.vEye = vLook;

    m_CameraDesc.vAt = pPlayerTransform->Get_Position();

    //D3DXVec3Normalize(&vLook, &vLook);

    return S_OK;
}

CSupportCamera* CSupportCamera::Create(_Device pDevice)
{
    CSupportCamera* pInstance = new CSupportCamera(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CSupportCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSupportCamera::Clone(void* pArg)
{
    CSupportCamera* pInstance = new CSupportCamera(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CSupportCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSupportCamera::Free()
{
    CCamera::Free();
}
