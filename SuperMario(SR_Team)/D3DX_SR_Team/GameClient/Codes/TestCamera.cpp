#include "framework.h"
#include "TestCamera.h"

CTestCamera::CTestCamera(_Device pDevice)
    : CCamera(pDevice)
{
    //m_vAt = _float3(0.f, 0.f, 0.f);
    //m_vRight = _float3(1.f, 0.f, 0.f);
    //m_vLookAt = _float3(0.f, 0.f, 1.f);
}

CTestCamera::CTestCamera(const CTestCamera& other)
    : CCamera(other)
{
    //m_vAt = _float3(0.f, 0.f, 0.f);
    //m_vRight = _float3(1.f, 0.f, 0.f);
    //m_vLookAt = _float3(0.f, 0.f, 1.f);
}

HRESULT CTestCamera::Ready_GameObject_Prototype()
{
    if (FAILED(CCamera::Ready_GameObject_Prototype()))
        return E_FAIL;    

    return S_OK;
}

HRESULT CTestCamera::Ready_GameObject(void* pArg)
{
    if (FAILED(CCamera::Ready_GameObject(pArg)))
        return E_FAIL;

	m_pTransformCom->Initialize_vPos(_float3(0.f, 0.f, -5.f));
	m_pTransformCom->Initialize_fRotate(D3DXToRadian(45.f));

    return S_OK;
}

_uint CTestCamera::Update_GameObject(_float fDeltaTime)
{
    CCamera::Update_GameObject(fDeltaTime);

    if (FAILED(KeyInput(fDeltaTime)))
        return 0;

    if (FAILED(Movement(fDeltaTime)))
        return 0;

    return _uint();
}

_uint CTestCamera::LateUpdate_GameObject(_float fDeltaTime)
{
    CCamera::LateUpdate_GameObject(fDeltaTime);

    return m_pTransformCom->Update_Transform();
}

HRESULT CTestCamera::Render_GameObject()
{
    //�Ⱦ�.
    if (FAILED(CCamera::Render_GameObject()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTestCamera::KeyInput(_float fDeltaTime)
{
    CManagement* pManagement = CManagement::Get_Instance();
    if (nullptr == pManagement)
        return E_FAIL;



    if (pManagement->Key_Pressing(KEY_ADD))
    {
        m_fDistanceToTarget -= 10.f * fDeltaTime;
    }

    if (pManagement->Key_Pressing(KEY_MIN))
    {
        m_fDistanceToTarget += 10.f * fDeltaTime;
    }

    if (pManagement->Key_Pressing(KEY_NUM8))
    {
        m_fCamAngle += 45.f * fDeltaTime;
    }

    if (pManagement->Key_Pressing(KEY_NUM2))
    {
        m_fCamAngle -= 45.f * fDeltaTime;
    }

    //ī�޶� ������
    if (pManagement->Key_Pressing(KEY_W))
    {
        m_pTransformCom->Go_Straight(fDeltaTime);
    }

    if (pManagement->Key_Pressing(KEY_S))
    {
        m_pTransformCom->Go_Straight(-fDeltaTime);
    }

    if (pManagement->Key_Pressing(KEY_A))
    {
        m_pTransformCom->Go_Side(-fDeltaTime);
    }

    if (pManagement->Key_Pressing(KEY_D))
    {
        m_pTransformCom->Go_Side(fDeltaTime);
    }

    //if (pManagement->Key_Pressing(KEY_Q))
    //{
    //	m_pTransformCom->Rotate(EAxis::Y, -fDeltaTime);
    //}
    //
    //if (pManagement->Key_Pressing(KEY_E))
    //{
    //	m_pTransformCom->Rotate(EAxis::Y, fDeltaTime);
    //}

    return S_OK;
}

HRESULT CTestCamera::Movement(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//�÷��̾� Ʈ������ ������.
		CTransform*		pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::Stage, L"Layer_Player", L"Com_Transform");


     if (nullptr == pPlayerTransform)
     {
         pPlayerTransform = (CTransform*)pManagement->Get_Component(
                (_uint)ESceneType::TestStage, L"Layer_Player", L"Com_Transform");
     }


	if (nullptr == pPlayerTransform)
	{
		PRINT_LOG(L"Error", L"Not found Player`s Transform at CTestCamera::Movement");
		return E_FAIL;
	}


	_mat matWorld = m_pTransformCom->Get_WorldMatrix();

	//X,Y,Z �� ���󰡰��ϱ�����. ������ ��ü�� �����;���.
	m_CameraDesc.vAt = pPlayerTransform->Get_Position();

	m_CameraDesc.vAt.y = 0.f;

	_float3 vInLook = { matWorld._31,matWorld._32,matWorld._33 };
	vInLook *= -m_fDistanceToTarget;

	m_CameraDesc.vEye = vInLook + m_CameraDesc.vAt;
	m_CameraDesc.vEye.y = 2.f;


	//�ü����� ���� Z���� �� ����
	//_float3 vLook;
	//memcpy(&vLook, &matWorld.m[2][0], sizeof(_float3));
	//D3DXVec3Normalize(&vLook, &vLook);

	////�׷��� �󸶳� �ڿ� �ִ°�.
	//_float3 vInLook = vLook * -m_fDistanceToTarget;


	//Right���� �޾ƿ���.
	//���� ��� 1�� = Right ����.
	//_float3 vRight;
	//memcpy(&vRight, &matWorld.m[0][0],sizeof(_float3));
	//D3DXVec3Normalize(&vRight, &vRight);


	//_mat matRot;
	//D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fCamAngle));

	////vInLook = _float3(0.f, 0.f, -10.f);
	//D3DXVec3TransformNormal(&vInLook, &vInLook, &matRot);


	//m_CameraDesc.vEye = vInLook + m_CameraDesc.vAt;
	//m_CameraDesc.vEye.y = 5.f;

	return S_OK;



}

CTestCamera* CTestCamera::Create(_Device pDevice)
{
    CTestCamera* pInstance = new CTestCamera(pDevice);
    if (FAILED(pInstance->Ready_GameObject_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CMainCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTestCamera::Clone(void* pArg)
{
    CTestCamera* pInstance = new CTestCamera(*this); /*�������ȣ��*/
    if (FAILED(pInstance->Ready_GameObject(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CMainCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTestCamera::Free()
{
    CCamera::Free();
}
