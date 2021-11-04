#include "Camera.h"
#include "Renderer.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_vShake(0.f, 0.f, 0.f), m_fShakeTime(0.f), m_vTargetUp(0.f, 1.f, 0.f)
{
	m_vUp = { 0.f,1.f,0.f };
	m_vEye = { 0.f,0.f,0.f };
	m_vAt = { 0.f,0.f,0.f };
	m_vTargetEye = { 0.f,0.f,0.f };
	m_vTargetAt = { 0.f,0.f,0.f };
	m_vTargetUp = { 0.f,1.f,0.f };
}

Engine::CCamera::~CCamera(void)
{

}

HRESULT Engine::CCamera::Ready_Object(void* pArg)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

Engine::_int Engine::CCamera::Update_Object(const _float& TimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}
HRESULT CCamera::Shake(_float fDeltaTime)
{
	if (!m_bShake)
		return S_OK;

	//카메라 바라보는 위치 설정
	_vec3 vDir = Get_CameraRight();
	D3DXVec3Normalize(&vDir, &vDir);


	if (m_fShakeTime > 0.24f)
	{
		m_fShakeTime = 0.f;
		m_bShake = false;
		m_vShake = _vec3(0.f, 0.f, 0.f);
	}
	else if (m_fShakeTime > 0.16f)
		m_vShake -= vDir * 0.1f;
	else if (m_fShakeTime > 0.08f)
		m_vShake += vDir * 0.2f;
	else if (m_fShakeTime < 0.08f)
		m_vShake -= vDir * 0.1f;

	m_fShakeTime += fDeltaTime;
	return S_OK;
}
void Engine::CCamera::Update_Timer(const _float& fTimeDelta)
{
	if (!m_bTimer)
		return;
	m_fTime -= fTimeDelta;
	if (m_fTime < 0.f)
	{
		m_bTimer = false;
		if (m_bEndSplit)
			CRenderer::GetInstance()->End_Split();
		else
			CRenderer::GetInstance()->Start_Split();
		Set_Mode(CAMERA_DEFAULT);
	}
}

void Engine::CCamera::Set_ResetTimer(const _float& fTime, const _bool& EndSplit)
{
	m_bTimer = true;
	m_fTime = fTime;
	m_bEndSplit = EndSplit;
}

_matrix CCamera::Get_CameraStateMatrix()
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

_vec3 CCamera::Get_CameraLook()
{
	Get_CameraStateMatrix();
	_vec3 vLook;
	vLook = _vec3(m_matCameraState._31, 0.f, m_matCameraState._33);
	return vLook;
}

_vec3 CCamera::Get_CameraRight()
{
	Get_CameraStateMatrix();
	_vec3 vRight;
	vRight = _vec3(m_matCameraState._11, 0.f, m_matCameraState._13);
	return vRight;
}

_vec3 CCamera::Get_CameraUp()
{
	Get_CameraStateMatrix();
	_vec3 vUp;
	vUp = _vec3(m_matCameraState._21, 0.f, m_matCameraState._23);
	return vUp;
}

_matrix CCamera::Get_CamMatrix()
{
	_float4x4 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);

	return matView;
}



void Engine::CCamera::Set_RayCheck(const _bool& bState /*= true*/)
{
	m_bRay = bState;
}

void Engine::CCamera::CalculateProj()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Engine::CCamera::Set_TargetUp(_vec3 vUp)
{
	//_vec3 prv = m_vTargetUp;
	m_vTargetUp = vUp;
}

void Engine::CCamera::Set_Fix(const _vec3& At, const _vec3& Eye, const _bool& bTelePort /*=false*/)
{
	m_bFixStart = true;
	m_vFixAt = At;
	m_vFixEye = Eye;
	if (bTelePort)
	{
		m_vTargetAt = At;
		m_vAt = At;
		m_vTargetEye = Eye;
		m_vEye = Eye;
	}
}

void Engine::CCamera::Move_To_Target(const _vec3& vTarget, _vec3& vSrc, const _float & TimeDelta)
{
	_vec3 vMove = vTarget - vSrc;
	_float fDist = D3DXVec3Length(&vMove);

	if (fDist < 0.5f)
		m_bMoveEnd = true;
	else
		m_bMoveEnd = false;

	// if (fDist < 1.5f)
	//{
	//	vSrc.x += vMove.x*fDist*TimeDelta;
	//	vSrc.y += vMove.y*fDist*TimeDelta;
	//	vSrc.z += vMove.z*fDist*TimeDelta;
	//}
	//else
	{
		vSrc.x += vMove.x*TimeDelta / m_fReachTime;
		vSrc.y += vMove.y*TimeDelta / m_fReachTime;
		vSrc.z += vMove.z*TimeDelta / m_fReachTime;
	}
}

void Engine::CCamera::Update_View(const _float & TimeDelta)
{

	Move_To_Target(m_vTargetUp, m_vUp, TimeDelta);
	Move_To_Target(m_vTargetAt, m_vAt, TimeDelta);
	Move_To_Target(m_vTargetEye, m_vEye, TimeDelta);
}

void Engine::CCamera::Set_Mode(const CAMERAMODE& eMode)
{
	if (eMode == m_eMode)
		return;

	switch (eMode)
	{
	case CAMERA_DEFAULT:
		m_bRay = true;
		m_fDist = 12.f;
		m_fReachTime = 0.2f;
		break;
	case CAMERA_STALKING:
		m_fDist = 4.f;
		m_bRay = true;
		//m_bRot = false;
		break;
	case CAMERA_FIX:
		m_bRay = false;
		break;
	case CAMERA_TRACING:
		OutputDebugString(L"트레이싱 타겟없음");
		m_bRay = false;
		break;
	default:
		break;
	}
	m_eMode = eMode;
}
void Engine::CCamera::Set_Mode(const CAMERAMODE& eMode, CGameObject* pTarget)
{
	if (eMode == m_eMode)
		return;

	switch (eMode)
	{
	case CAMERA_DEFAULT:
		m_bRay = true;
		m_fDist = 12.f;
		m_fReachTime = 0.2f;

		break;
	case CAMERA_STALKING:
		m_fDist = 4.f;
		m_bRay = true;

		break;
	case CAMERA_TRACING:
		m_pTarget = pTarget;
		m_bRay = false;
		break;
	default:
		break;
	}
	m_eMode = eMode;
}

