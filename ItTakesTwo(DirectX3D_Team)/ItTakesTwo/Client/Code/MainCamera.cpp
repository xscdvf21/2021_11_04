#include "stdafx.h"
#include "MainCamera.h"
#include "Export_Function.h"

USING(Engine)

CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fViewAngle(30.f), m_fViewAtHeight(0.f), m_fViewDistance(9.f), m_fViewHeight(0.f)

{

}
CMainCamera::~CMainCamera(void)
{

}

HRESULT CMainCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar, const _vec3* vCampos, const _float& fDis)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_vEyeDir = -*vCampos;//_vec3(-8.0f, 15.0f, -5.0f);
	D3DXVec3Normalize(&m_vEyeDir, &m_vEyeDir);

	m_fMinDistance = fDis;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);

	
	return S_OK;
}

Engine::_int CMainCamera::Update_Object(const _float& TimeDelta)
{
	if (GetFocus() != g_hWnd)
		m_bMouseFix = false;
	else
		m_bMouseFix = true;

	Update_Timer(TimeDelta);

	if (!g_bMenu)
	{
		Key_Input(TimeDelta);
	
		if (true == m_bMouseFix)
		{
			Shake(TimeDelta);
			Update_CameraPos(TimeDelta);
			Fix_Mouse();
		}
	}

	if (g_bMenu)
	{
		m_vEye = _vec3(0.f, 0.55f, -0.55f);
		m_vAt = _vec3(-0.03f, 0.2f, 0.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);
	}



	return 0;
}

_int CMainCamera::LateUpdate_Object(const _float & TimeDelta)
{

	Update_View(TimeDelta);

	_float4x4 matViewMatrix;
	D3DXMatrixIdentity(&matViewMatrix);

	D3DXMatrixLookAtLH(&matViewMatrix, &m_vEye, &(m_vAt + m_vShake), &m_vUp);

	Engine::Get_Renderer()->Set_ViewMatrix(2, matViewMatrix);
	m_matView = matViewMatrix;

	return _int();
}

void CMainCamera::Update_View_Proj()
{
	_float4x4 matView, matProj;

	D3DXMatrixLookAtLH(&matView, &m_vEye, &(m_vAt + m_vShake), &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
}

void CMainCamera::Free(void)
{
	Engine::CCamera::Free();
}
void CMainCamera::Update_CameraPos(const _float& fTimeDelta)
{

	switch (m_eMode)
	{
	case Engine::CAMERA_DEFAULT:
		Update_Default(fTimeDelta);
		break;
	case Engine::CAMERA_STALKING:
		Update_Stalking();
		break;
	case Engine::CAMERA_FIX:
		Update_Fix();
		break;
	case Engine::CAMERA_TRACING:
		Update_Tracing();
		break;
	default:
		break;
	}

	//CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	//NULL_CHECK_RETURN(pCodyTransformCom, );

	//CTransform* pMayTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
	//NULL_CHECK_RETURN(pMayTransformCom, );

	//const _float4x4* pCodyMatrix = pCodyTransformCom->Get_WorldMatrix();
	//_float3 vCodyPos;
	//memcpy(&vCodyPos, &pCodyMatrix->m[3][0], sizeof(_float3));

	//const _float4x4* pMayMatrix = pMayTransformCom->Get_WorldMatrix();
	//_float3 vMayPos;
	//memcpy(&vMayPos, &pMayMatrix->m[3][0], sizeof(_float3));

	//// 카메라의 거리 조절함수
	//Update_CameraDis(fTimeDelta, vCodyPos, vMayPos);

	//_float3 vCodyProjPos, vMayProjPos;
	//{
	//	vCodyProjPos = vCodyPos;
	//	D3DXVec3TransformCoord(&vCodyProjPos, &vCodyProjPos, &m_matView);
	//	D3DXVec3TransformCoord(&vCodyProjPos, &vCodyProjPos, &m_matProj);

	//	vMayProjPos = vMayPos;
	//	D3DXVec3TransformCoord(&vMayProjPos, &vMayProjPos, &m_matView);
	//	D3DXVec3TransformCoord(&vMayProjPos, &vMayProjPos, &m_matProj);
	//}

	///*_bool bLock = false;
	//if ((vCodyProjPos.x >= 0.9f || vCodyProjPos.x <= -0.9f) || (vCodyProjPos.y >= 0.9f || vCodyProjPos.y <= -0.9f))
	//	bLock = true;
	//if ((vMayProjPos.x >= 0.9f || vMayProjPos.x <= -0.9f) || (vMayProjPos.y >= 0.9f || vMayProjPos.y <= -0.9f))
	//	bLock = true;

	//if(!bLock)*/
	////m_vAt = vMayPos + m_vShake;// +_float3(0.0f, 0.0f, 0.0f)) / 2.0f;
	//m_vAt = ((vMayPos + vCodyPos) / 2.0f) + m_vShake+m_vOffset;
	//m_vEye = m_vAt + (m_vEyeDir * -m_fDistance)+ m_vOffset;
}

void CMainCamera::Update_CameraDis(const _float & fTimeDelta, _float3 vCodyPos, _float3 vMayPos)
{
	_float3 vDis = vCodyPos - vMayPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 10.0f)
		fDis = 0.0f;
	else
		fDis -= 10.0f;

	_float fRatio = m_fDistance - m_fMinDistance;
	fRatio *= 0.1f;


	m_fDistance = (m_fMinDistance + m_fLength * fDis * 0.04f) - m_fDisAdd ;
	if (m_fDistance <= m_fMinDistance)
		m_fDistance = m_fMinDistance;
	else if (m_fDistance >= m_fMaxDistance)
		m_fDistance = m_fMaxDistance;
}

void CMainCamera::Update_Default(const _float & fTimeDelta)
{
	CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pCodyTransformCom, );

	CTransform* pMayTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pMayTransformCom, );

	const _float4x4* pCodyMatrix = pCodyTransformCom->Get_WorldMatrix();
	_float3 vCodyPos;
	memcpy(&vCodyPos, &pCodyMatrix->m[3][0], sizeof(_float3));

	const _float4x4* pMayMatrix = pMayTransformCom->Get_WorldMatrix();
	_float3 vMayPos;
	memcpy(&vMayPos, &pMayMatrix->m[3][0], sizeof(_float3));

	// 카메라의 거리 조절함수
	Update_CameraDis(fTimeDelta, vCodyPos, vMayPos);

	_float3 vCodyProjPos, vMayProjPos;
	{
		vCodyProjPos = vCodyPos;
		D3DXVec3TransformCoord(&vCodyProjPos, &vCodyProjPos, &m_matView);
		D3DXVec3TransformCoord(&vCodyProjPos, &vCodyProjPos, &m_matProj);

		vMayProjPos = vMayPos;
		D3DXVec3TransformCoord(&vMayProjPos, &vMayProjPos, &m_matView);
		D3DXVec3TransformCoord(&vMayProjPos, &vMayProjPos, &m_matProj);
	}

	/*_bool bLock = false;
	if ((vCodyProjPos.x >= 0.9f || vCodyProjPos.x <= -0.9f) || (vCodyProjPos.y >= 0.9f || vCodyProjPos.y <= -0.9f))
	bLock = true;
	if ((vMayProjPos.x >= 0.9f || vMayProjPos.x <= -0.9f) || (vMayProjPos.y >= 0.9f || vMayProjPos.y <= -0.9f))
	bLock = true;

	if(!bLock)*/
	//m_vAt = vMayPos + m_vShake;// +_float3(0.0f, 0.0f, 0.0f)) / 2.0f;


	//m_vAt = ((vMayPos + vCodyPos) / 2.0f) + m_vShake + m_vOffset;
	//m_vEye = m_vAt + (m_vEyeDir * -m_fDistance) + m_vOffset;

	m_vTargetAt = ((vMayPos + vCodyPos) / 2.0f) + m_vOffset;
	m_vTargetEye = m_vAt + (m_vEyeDir * -m_fDistance) + m_vOffset;
}

void CMainCamera::Update_Fix()
{
	if (m_bFixStart)
	{
		m_vTargetAt = m_vFixAt;
		m_vTargetEye = m_vFixEye;
		m_bFixStart = false;
	}
}

void CMainCamera::Update_Stalking()
{
	m_vTargetAt = m_vFixAt;
	m_vTargetEye = m_vFixEye;
}

void CMainCamera::Update_Tracing()
{
	if (!m_pTarget)
		return;
	_mat matWorld = m_pTarget->Get_EventPos();
	_mat temp{};
	ZeroMemory(&temp, sizeof(_mat));
	if (matWorld == temp)
		Set_Mode(CAMERA_DEFAULT);
	m_vTargetAt = *((_vec3*)(&matWorld._41));

}


void CMainCamera::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Key_Pressing(DIK_GRAVE))
	{
		if (m_bClick)
			return;

		m_bClick = true;

		if (true == m_bMouseFix)
			m_bMouseFix = false;
		else
			m_bMouseFix = true;
	}
	else
		m_bClick = false;

	if (false == m_bMouseFix)
		return;
}

CMainCamera* CMainCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar, const _vec3* vCampos, const _float& fDis)
{
	CMainCamera*	pInstance = new CMainCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar, vCampos, fDis)))
		Safe_Release(pInstance);

	return pInstance;
}


void CMainCamera::Fix_Mouse(void)
{
	if (GetFocus() != g_hWnd)
	{
		return;
	}

	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

_matrix CMainCamera::Get_CameraStateMatrix()
{
	/* 카메라의 월드상 right, up, look, position을 구해 행렬을 구성한다. */

	_vec3 vRight, vUp, vLook;

	//목적지 - 출발지
	vLook = m_vAt - m_vEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, &m_vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);


	D3DXMatrixIdentity(&m_matCameraState);

	memcpy(&m_matCameraState.m[0][0], vRight, sizeof(_vec3));
	memcpy(&m_matCameraState.m[1][0], vUp, sizeof(_vec3));
	memcpy(&m_matCameraState.m[2][0], vLook, sizeof(_vec3));
	memcpy(&m_matCameraState.m[3][0], m_vEye, sizeof(_vec3));

	return m_matCameraState;
}

_vec3 CMainCamera::Get_CameraLook()
{
	Get_CameraStateMatrix();
	_vec3 vLook;
	vLook = _vec3(m_matCameraState._31, 0.f, m_matCameraState._33);
	return vLook;
}

_vec3 CMainCamera::Get_CameraRight()
{
	Get_CameraStateMatrix();
	_vec3 vRight;
	vRight = _vec3(m_matCameraState._11, 0.f, m_matCameraState._13);
	return vRight;
}

_vec3 CMainCamera::Get_CameraUp()
{
	Get_CameraStateMatrix();
	_vec3 vUp;
	vUp = _vec3(m_matCameraState._21, 0.f, m_matCameraState._23);
	return vUp;
}

_matrix CMainCamera::Get_CamMatrix()
{
	_float4x4 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);

	return matView;
}
