#include "framework.h"
#include "BattleCamera.h"

CBattleCamera::CBattleCamera(_Device pDevice)
    : CCamera(pDevice)
{
}

CBattleCamera::CBattleCamera(const CBattleCamera& other)
    : CCamera(other)
{
}

HRESULT CBattleCamera::Ready_GameObject_Prototype()
{
    if (FAILED(CCamera::Ready_GameObject_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBattleCamera::Ready_GameObject(void* pArg)
{
    if (FAILED(CCamera::Ready_GameObject(pArg)))
        return E_FAIL;

    m_fSetCameraRotate = 0.f;
    m_fNowCameraRotate = 0.f;

    return S_OK;
}

_uint CBattleCamera::Update_GameObject(_float fDeltaTime)
{
    CCamera::Update_GameObject(fDeltaTime);

    if (FAILED(KeyInput(fDeltaTime)))
        return 0;

    if (FAILED(Movement(fDeltaTime)))
        return 0;

    return _uint();
}

_uint CBattleCamera::LateUpdate_GameObject(_float fDeltaTime)
{
    CCamera::LateUpdate_GameObject(fDeltaTime);

    return 0;
}

HRESULT CBattleCamera::Render_GameObject()
{
    //안씀.
    if (FAILED(CCamera::Render_GameObject()))
        return E_FAIL;

    return S_OK;
}
void CBattleCamera::Cam_Rotate_CW()
{
    m_fSetCameraRotate -= 30.f;
}
void CBattleCamera::Cam_Rotate_CCW()
{
    m_fSetCameraRotate += 30.f;
}
void CBattleCamera::Cam_Move_Zoom()
{
}
void CBattleCamera::Cam_Move_Release()
{
}
//테스트


HRESULT CBattleCamera::KeyInput(_float fDeltaTime)
{
    CManagement* pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;

    if (pManagement->Key_Up(KEY_A))
    {
        m_fSetCameraRotate += 30.f;
    }

    if (pManagement->Key_Up(KEY_D))
    {
        m_fSetCameraRotate -= 30.f;
    }


    return S_OK;
}

HRESULT CBattleCamera::Movement(_float fDeltaTime)
{
    CManagement* pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;

    // float fRotate = fDeltaTime * m_pTransformCom->Get_Rotate();
    float fRotate = fDeltaTime * 30.f * 7.f;

    if (abs(m_fSetCameraRotate-m_fNowCameraRotate) >= (fRotate))
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


    //카메라 Eye 위치 지정
    _mat matWorld, matRotY, matParent;

    _vec3 _vPos = m_CameraDesc.vEye;

    D3DXMatrixRotationY(&matRotY, D3DXToRadian(fRotate));		//회전 먹이고
    D3DXMatrixTranslation(&matParent, 0.f, 0.f, 0.f);			//부모 위치 먹이고
    matWorld = matRotY * matParent;
    D3DXVec3TransformCoord(&_vPos, &_vPos, &matWorld);			//좌표 받아서

    m_CameraDesc.vEye = _vPos;


    //돌아간 카메라에 맞춰 중점 재 세팅


    m_CameraDesc.vAt = _float3(0.f, 0.f, 0.f);      //바라보는 방향은 중점임.

        //시선방향 벡터
    _float3 vLook = -_vPos;
    D3DXVec3Normalize(&vLook, &vLook);

    //그래서 얼마나 뒤에 있는가.
    _float3 vInLook = vLook * -m_fDistanceToTarget;

    //Right벡터 받아오기.
    //월드 행렬 1행 = Right 벡터.
    _float3 vRight = _float3(1.0f, 0.f, 0.f);
    D3DXVec3Normalize(&vRight, &vRight);

    _mat matRot;
    D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(-fRotate));
    D3DXVec3TransformNormal(&vInLook, &vInLook, &matRot);

    //m_CameraDesc.vEye = vInLook + m_CameraDesc.vAt;

    return S_OK;
}

CBattleCamera* CBattleCamera::Create(_Device pDevice)
{
    CBattleCamera* pInstance = new CBattleCamera(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CMainCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBattleCamera::Clone(void* pArg)
{
    CBattleCamera* pInstance = new CBattleCamera(*this); /*복사생성호출*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CMainCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBattleCamera::Free()
{
    Safe_Release(m_pTransformCom);

    CCamera::Free();
}

