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
    //�Ⱦ�.
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
//�׽�Ʈ


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


    //ī�޶� Eye ��ġ ����
    _mat matWorld, matRotY, matParent;

    _vec3 _vPos = m_CameraDesc.vEye;

    D3DXMatrixRotationY(&matRotY, D3DXToRadian(fRotate));		//ȸ�� ���̰�
    D3DXMatrixTranslation(&matParent, 0.f, 0.f, 0.f);			//�θ� ��ġ ���̰�
    matWorld = matRotY * matParent;
    D3DXVec3TransformCoord(&_vPos, &_vPos, &matWorld);			//��ǥ �޾Ƽ�

    m_CameraDesc.vEye = _vPos;


    //���ư� ī�޶� ���� ���� �� ����


    m_CameraDesc.vAt = _float3(0.f, 0.f, 0.f);      //�ٶ󺸴� ������ ������.

        //�ü����� ����
    _float3 vLook = -_vPos;
    D3DXVec3Normalize(&vLook, &vLook);

    //�׷��� �󸶳� �ڿ� �ִ°�.
    _float3 vInLook = vLook * -m_fDistanceToTarget;

    //Right���� �޾ƿ���.
    //���� ��� 1�� = Right ����.
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
    CBattleCamera* pInstance = new CBattleCamera(*this); /*�������ȣ��*/
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

