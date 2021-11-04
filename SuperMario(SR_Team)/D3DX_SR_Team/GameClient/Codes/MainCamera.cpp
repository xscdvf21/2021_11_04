#include "framework.h"
#include "..\Headers\MainCamera.h"


CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 pDevice)
	:CCamera(pDevice)
{
}

CMainCamera::CMainCamera(const CMainCamera & other)
	:CCamera(other)
{
}

HRESULT CMainCamera::Ready_GameObject_Prototype()
{
	if (FAILED(CCamera::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainCamera::Ready_GameObject(void * pArg)
{
	if (FAILED(CCamera::Ready_GameObject(pArg)))
		return E_FAIL;

	return S_OK;
}

_uint CMainCamera::Update_GameObject(_float fDeltaTime)
{
	CCamera::Update_GameObject(fDeltaTime);

	return _uint();
}

_uint CMainCamera::LateUpdate_GameObject(_float fDeltaTime)
{
	CCamera::LateUpdate_GameObject(fDeltaTime);
	return _uint();
}

HRESULT CMainCamera::Render_GameObject()
{
	return S_OK;
}

HRESULT CMainCamera::KeyInput(_float fDeltaTime)
{
	//회전 줌, 딱히안하므로, 필요없을듯.
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
	{
		return E_FAIL;	
	}

	return S_OK;
}

HRESULT CMainCamera::Movement(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();

	//로딩 쓰레드에서 원본 생성해줘야댐.

	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	/*여기를 잘 공부해야할듯.*/
	/* 플레이어의 트랜스폼 얻어와야 함 */
	
	CTransform* pPlayerTransform = (CTransform*)pManagement->Get_Component(
		(_uint)ESceneType::Stage, L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
	{
		PRINT_LOG(L"Error", L"Not found Player`s Transform at CMainCamera::Movement");
		return E_FAIL;
	}

	m_CameraDesc.vAt = pPlayerTransform->Get_Position();

	_float4x4 matPlayerWorld = pPlayerTransform->Get_WorldMatrix();

	_float3 vPlayerLook; /* 그림상에서 빨간색 벡터 */
	memcpy(&vPlayerLook, &matPlayerWorld.m[2][0], sizeof(_float3));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	/* 그림상에서 파란색 벡터 */
	_float3 vInvPlayerLook = vPlayerLook * -m_fDistanceToTarget;

	/* vInvPlayerLook벡터 회전 */
	_float3 vPlayerRight;
	memcpy(&vPlayerRight, &matPlayerWorld.m[0][0], sizeof(_float3));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	_float4x4 matRot;
	D3DXMatrixRotationAxis(&matRot, &vPlayerRight, D3DXToRadian(m_fCamAngle));
	D3DXVec3TransformNormal(&vInvPlayerLook, &vInvPlayerLook, &matRot);

	m_CameraDesc.vEye = vInvPlayerLook/* 그림상에서 파란색 벡터 */ + m_CameraDesc.vAt/* 그림상에서 보라색 벡터 */;

	return S_OK;

}

CMainCamera * CMainCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMainCamera* pInstance = new CMainCamera(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CMainCamera (Client - CMainCamera.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMainCamera::Clone(void * pArg)
{
	CMainCamera* pInstance = new CMainCamera(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMainCamera (Client - CMainCamera.cpp)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainCamera::Free()
{
	CCamera::Free();
}

