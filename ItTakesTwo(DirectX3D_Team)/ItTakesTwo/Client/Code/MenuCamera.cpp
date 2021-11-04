#include "stdafx.h"
#include "MenuCamera.h"
#include "Export_Function.h"

USING(Engine)


CMenuCamera::CMenuCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fViewAngle(30.f), m_fViewAtHeight(0.f), m_fViewDistance(9.f), m_fViewHeight(0.f)
	, m_bChange(false), m_fChangeTime(0.f)
{

}

CMenuCamera::~CMenuCamera(void)
{

}

HRESULT CMenuCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CMenuCamera::Update_Object(const _float& TimeDelta)
{
	if (!g_bMenu)
	{

		if (true == m_bMouseFix)
		{
			Fix_Mouse();
			Mouse_Move(TimeDelta);
		}
	}

	if (g_bMenu)
	{
		m_vEye = _vec3(0.f, 2.5f, -5.f);
		m_vAt = _vec3(0.f, 0.f, 0.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);
	}

	_int iExit = Engine::CCamera::Update_Object(TimeDelta);
	_float4x4 matViewMatrix;
	D3DXMatrixLookAtLH(&matViewMatrix, &m_vEye, &m_vAt, &m_vUp);

	Engine::Get_Renderer()->Set_ViewMatrix(2, matViewMatrix);
	m_matView = matViewMatrix;
	return iExit;
}

_int CMenuCamera::LateUpdate_Object(const _float & TimeDelta)
{
	return _int();
}

void CMenuCamera::Free(void)
{
	Engine::CCamera::Free();
}

HRESULT CMenuCamera::Movement(_float fDeltaTime)
{
	CTransform* pPlayerTransform = (CTransform*)Engine::Get_Component(L"GameObject", L"Player", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pPlayerTransform, E_FAIL);

	//카메라 바라보는 위치 설정
	if (!m_bStopCamera)
		m_vAt = pPlayerTransform->Get_Position() + _vec3(0.0f, 3.f, 0.f);

	_matrix matPlayerWorld;
	pPlayerTransform->Get_WorldMatrix(&matPlayerWorld);

	_vec3 vPlayerLook; /* 그림상에서 빨간색 벡터 */
	memcpy(&vPlayerLook, &matPlayerWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	/* 그림상에서 파란색 벡터 */
	_vec3 vInvPlayerLook = vPlayerLook * -m_fViewDistance;

	/* vInvPlayerLook벡터 회전 */
	_vec3 vPlayerRight;
	memcpy(&vPlayerRight, &matPlayerWorld.m[0][0], sizeof(_vec3));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &vPlayerRight, D3DXToRadian(m_fViewAngle));
	D3DXVec3TransformNormal(&vInvPlayerLook, &vInvPlayerLook, &matRot);

	if (!m_bStopCamera && !m_bChange && !m_bMission)
		m_vEye = vInvPlayerLook + pPlayerTransform->Get_Position() + _vec3(0.0f, 3.f, 0.f) + _vec3(0.f, m_fViewHeight, 0.f);

	m_vAt += _vec3{ 0.f, m_fViewAtHeight, 0.f };
	m_vEye -= _vec3{ 0.f, m_fViewAtHeight, 0.f };

	return S_OK;
}

CMenuCamera* CMenuCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CMenuCamera*	pInstance = new CMenuCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMenuCamera::Mouse_Move(const _float& fTimeDelta)
{
	if (m_bStopCamera || m_bMission)
		return;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove = 0;

	CTransform* pPlayerTransform = (CTransform*)Engine::Get_Component(L"GameObject", L"Player", L"Com_Transform", ID_DYNAMIC);
	CGameObject* pPlayer = (CGameObject*)Engine::Get_GameObject(L"GameObject", L"Player");

	NULL_CHECK_RETURN(pPlayerTransform, );


	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;

		_matrix	matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(_float(dwMouseMove) / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
		pPlayerTransform->Rotation(ROT_X, D3DXToRadian((_float(dwMouseMove) / 10.f) * 90.f * fTimeDelta));
		//CTransform* pSpongeBobTransform = (CTransform*)dynamic_cast<CPlayer*>(pPlayer)->Get_SpongeBob()->Get_Component(L"Com_Transform", ID_DYNAMIC);
		//pSpongeBobTransform->Rotation(ROT_X, D3DXToRadian((dwMouseMove / 10) * 90.f * fTimeDelta));
	}


}

void CMenuCamera::Fix_Mouse(void)
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

_matrix CMenuCamera::Get_CameraStateMatrix()
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

_vec3 CMenuCamera::Get_CameraLook()
{
	Get_CameraStateMatrix();
	_vec3 vLook;
	vLook = _vec3(m_matCameraState._31, 0.f, m_matCameraState._33);
	return vLook;
}

_vec3 CMenuCamera::Get_CameraRight()
{
	Get_CameraStateMatrix();
	_vec3 vRight;
	vRight = _vec3(m_matCameraState._11, 0.f, m_matCameraState._13);
	return vRight;
}

_vec3 CMenuCamera::Get_CameraUp()
{
	Get_CameraStateMatrix();
	_vec3 vUp;
	vUp = _vec3(m_matCameraState._21, 0.f, m_matCameraState._23);
	return vUp;
}