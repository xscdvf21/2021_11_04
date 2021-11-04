#include "stdafx.h"
#include "CodyCamera.h"
#include "Export_Function.h"
#include "Cody.h"

USING(Engine)

CCodyCamera::CCodyCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fViewAngle(30.f), m_fViewAtHeight(0.f), m_fViewDistance(9.f), m_fViewHeight(0.f), m_bSilde(false), m_bSpetulaScene(false)
	, m_bChange(false), m_fChangeTime(0.f)
{

}

CCodyCamera::~CCodyCamera(void)
{

}

void CCodyCamera::Update_Default()
{
	CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pCodyTransformCom, );

	_vec3 vPlayerPos = pCodyTransformCom->Get_Position();

	vPlayerPos.y += 0.62f*(m_fDist/12.f);
	vPlayerPos.y += m_fOffset*(m_fDist / 12.f);

	m_vTargetAt = vPlayerPos;

	_vec3 MovePos = m_vTargetAt + (m_vEyeDir * -m_fDist);

	m_vTargetEye = MovePos;
}

void CCodyCamera::Update_Stalking()
{
	CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pCodyTransformCom, );


	_vec3 vPlayerPos = pCodyTransformCom->Get_Position();
	vPlayerPos.y += 0.8f*(m_fDist / 12.f);

	m_vTargetAt = vPlayerPos;

	_vec3 MovePos = m_vTargetAt + (m_vEyeDir*-m_fDist);

	m_vTargetEye = MovePos;
}
void CCodyCamera::Update_Fix()
{
	if (m_bFixStart)
	{

		m_vTargetAt = m_vFixAt;
		m_vTargetEye = m_vFixEye;
		m_bFixStart = false;

	}

}
void CCodyCamera::Update_Tracing()
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
HRESULT CCodyCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_vEyeDir = _float3(0.0f, 0.0f, 0.0f) - _float3(0.0, 3.0f, -7.0f);
	D3DXVec3Normalize(&m_vEyeDir, &m_vEyeDir);

	m_fDist = 12.0f;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);

	//Create_HitBox(Engine::Get_Physics());
	return S_OK;
}

Engine::_int CCodyCamera::Update_Object(const _float& TimeDelta)
{

	if (!g_bMenu)
	{
		Key_Input(TimeDelta);
		Shake(TimeDelta);

		if (true == m_bMouseFix)
		{
			Update_CameraPos();
			Fix_Mouse();
			CamView(TimeDelta);
		}
	}

	/*_int iExit = Engine::CCamera::Update_Object(TimeDelta);

	return iExit;*/


	return 0;
}

_int CCodyCamera::LateUpdate_Object(const _float & TimeDelta)
{
	Update_View(TimeDelta);
	CheckRayHit(TimeDelta);
	if (m_bStart)
	{
		CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
		if (pCodyTransformCom)
		{
			_vec3 vPlayerPos = pCodyTransformCom->Get_Position();
			vPlayerPos.y += 0.62f*(m_fDist/12.f);
			m_vAt = vPlayerPos;
			m_vEye = m_vAt + (m_vEyeDir*-m_fDist);
			m_bStart = false;
		}
	}
	_float4x4 matViewMatrix;
	//m_vUp = { 0.f,1.f,0.f };

	D3DXMatrixLookAtLH(&matViewMatrix, &m_vEye, &(m_vAt + m_vShake), &m_vUp);
	Engine::Get_Renderer()->Set_ViewMatrix(1, matViewMatrix);
	m_matView = matViewMatrix;
	return _int();
}

void CCodyCamera::Free(void)
{
	Engine::CCamera::Free();
}






void CCodyCamera::Update_CameraPos()
{
	switch (m_eMode)
	{
	case Engine::CAMERA_DEFAULT:
		Update_Default();
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

}

void CCodyCamera::Change2dView(_float3 vGoalDir)
{
	m_b2dCamera = true;
	m_b2DCamRot = true;
	m_fCurYRad = 0.0f;
	m_fCurXRad = 0.0f;

	// 현재 카메라의 방향벡터
	_float3 vEyeDirRev = -m_vEyeDir;
	vEyeDirRev.y = 0.0f;
	D3DXVec3Normalize(&vEyeDirRev, &vEyeDirRev);

	// 목표 방향
	_float3 vGoalDirRev = -vGoalDir;

	// 내적을 통해 얼마만큼 회전할지 구한다.
	_float fCosTh = D3DXVec3Dot(&vEyeDirRev, &vGoalDirRev);
	m_fRotYRad = acosf(fCosTh);

	// 수직인 벡터를 구해 방향을 구한다.
	_float3 vPerp;
	D3DXVec3Cross(&vPerp, &_float3(0.0f, 1.0f, 0.0f), &vGoalDirRev);
	D3DXVec3Normalize(&vPerp, &vPerp);
	_float fDir = D3DXVec3Dot(&vPerp, &vEyeDirRev);
	if (fDir <= 0.0f)
		m_fDirY = -1.0f;
	else
		m_fDirY = 1.0f;

	// 상하 각도
	vEyeDirRev = -m_vEyeDir;
	vEyeDirRev.x = 0.0f;
	D3DXVec3Normalize(&vEyeDirRev, &vEyeDirRev);

	// 
	_float fY, fZ;
	if (vEyeDirRev.y < 0.0f)
		fY = -1.0f;
	else
		fY = 1.0f;

	if (vEyeDirRev.z < 0.0f)
		vEyeDirRev.z *= -1.0f;
	else
		fZ = 1.0f;

	_float3 vZ = _float3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&vZ, &vZ);

	/*vEyeDirRev = _float3(0.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&vEyeDirRev, &vEyeDirRev);

	vZ = _float3(0.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&vZ, &vZ);*/

	fCosTh = D3DXVec3Dot(&vEyeDirRev, &vZ);
	m_fRotXRad = acosf(fCosTh);

	vPerp;
	D3DXVec3Cross(&vPerp, &_float3(1.0f, 0.0f, 0.0f), &vZ);
	D3DXVec3Normalize(&vPerp, &vPerp);
	fDir = D3DXVec3Dot(&vPerp, &vEyeDirRev);

	if (fDir <= 0.0f)
		m_fDirX = -1.0f;
	else
		m_fDirX = 1.0f;
}

void CCodyCamera::Change3dView()
{
	m_b3dCamera = true;
	m_b3dCamRot = true;
	m_b2dCamera = false;
	m_fCurYRad = 0.0f;
	m_fCurXRad = 0.0f;

	_float3 vGoalDirRev, vEyeDirRev;

	CTransform* pTransform = (CTransform*)Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
	pTransform->Get_Info(INFO_LOOK, &vGoalDirRev);
	D3DXVec3Normalize(&vGoalDirRev, &vGoalDirRev);
	vGoalDirRev *= -1;

	vEyeDirRev = -m_vEyeDir;
	vEyeDirRev.y = 0.0f;
	D3DXVec3Normalize(&vEyeDirRev, &vEyeDirRev);

	_float fCosTh = D3DXVec3Dot(&vEyeDirRev, &vGoalDirRev);
	m_fRotYRad = acosf(fCosTh);

	_float3 vPerp;
	D3DXVec3Cross(&vPerp, &_float3(0.0f, 1.0f, 0.0f), &vGoalDirRev);
	D3DXVec3Normalize(&vPerp, &vPerp);
	_float fDir = D3DXVec3Dot(&vPerp, &vEyeDirRev);
	if (fDir <= 0.0f)
		m_fDirY = -1.0f;
	else
		m_fDirY = 1.0f;

	_uint iA = 0;
}

void CCodyCamera::CheckRayHit(const _float & fTimeDelta)
{
	if (!m_pScene || !m_bRay || !m_bForceRay)
		return;
	const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here

	CCody* pPlayer = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>();
	PxQueryFilterData filterData;
	filterData.data = PxFilterData(FilterGroup::eGround,0,0, 0);
	_vec3 vPos = m_vTargetAt;
	_vec3 vDir = m_vEye - vPos;
	D3DXVec3Normalize(&vDir, &vDir);
	if (m_pScene->raycast(PxVec3(vPos.x, vPos.y, vPos.z),
		PxVec3(vDir.x, vDir.y, vDir.z),
		m_fDist,
		buf, PxHitFlag::eMESH_BOTH_SIDES,
		filterData
		))
	{
		for (PxU32 i = 0; i < buf.nbTouches; ++i)
		{
			if (ToObj<CGameObject>(buf.getTouch(i).actor->getName()) != pPlayer)
			{
				_float fHitDist = buf.getTouch(i).distance;

				if (fHitDist < 0.3f)
					continue;
				m_vEye = m_vTargetAt + vDir*fHitDist;
				return;
			}
		}
	}
}

void CCodyCamera::Update3dView(const _float & fTimeDelta)
{
	_float fRotRad = fTimeDelta;
	if (m_fRotYRad < m_fCurYRad + fRotRad)
	{
		m_b3dCamRot = false;
		fRotRad = m_fRotYRad - m_fCurYRad;
	}

	m_fCurYRad += fRotRad;
	_float4x4 matRotY;
	D3DXMatrixRotationY(&matRotY, fRotRad * -m_fDirY);

	D3DXVec3TransformNormal(&m_vEyeDir, &m_vEyeDir, &matRotY);
}

void CCodyCamera::Update2dView(const _float & fTimeDelta)
{
	if (!m_b2DCamRot)
		return;

	// Y현재 회전시킬 각도
	_float fRotYRad = fTimeDelta;
	if (m_fRotYRad < m_fCurYRad + fRotYRad)
	{
		m_b2DCamRot = false;
		fRotYRad = m_fRotYRad - m_fCurYRad;
	}

	m_fCurYRad += fRotYRad;
	_float4x4 matRotY;
	D3DXMatrixRotationY(&matRotY, fRotYRad * -m_fDirY);

	D3DXVec3TransformNormal(&m_vEyeDir, &m_vEyeDir, &matRotY);


	_float fRotXRad;
	fRotXRad = fRotYRad / m_fRotYRad * m_fRotXRad;

	m_fCurXRad += fRotXRad;

	_float3 vUp = _float3(0.0f, 1.0f, 0.0f);
	_float3 vAxis;
	D3DXVec3Cross(&vAxis, &vUp, &m_vEyeDir);

	_float4x4 matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vAxis, fRotXRad * m_fDirX);

	D3DXVec3TransformNormal(&m_vEyeDir, &m_vEyeDir, &matRotAxis);
}

void CCodyCamera::CamView(const _float & fTimeDelta)
{
	if (m_b2dCamera)
		Update2dView(fTimeDelta);
	else
	{
		if (m_b3dCamRot)
		{
			Update3dView(fTimeDelta);
		}
		else
		{
			Mouse_Rev(fTimeDelta);
		}
	}
}

void CCodyCamera::Mouse_Rev(const _float & fTimeDelta)
{
	if (!m_bRot)
		return;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		m_vAngle.y = dwMouseMove / 10.f;

		_float4x4 matRev;
		D3DXMatrixRotationY(&matRev, D3DXToRadian(m_vAngle.y));

		D3DXVec3TransformNormal(&m_vEyeDir, &m_vEyeDir, &matRev);
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		m_vAngle.x = dwMouseMove / 10.f;

		_float4x4 matRev;
		_float3 vCamRight;
		memcpy(&vCamRight, &matCamWorld.m[0][0], sizeof(_float3));
		D3DXMatrixRotationAxis(&matRev, &vCamRight, D3DXToRadian(m_vAngle.x));

		D3DXVec3TransformNormal(&m_vEyeDir, &m_vEyeDir, &matRev);

		_float3 vUp = _float3(0.0f, 1.0f, 0.0f);
		_float3 vCamDir = m_vEye - m_vAt;
		D3DXVec3Normalize(&vCamDir, &vCamDir);

		_float3 vPreDir = -m_vEyeDir;
		D3DXVec3TransformNormal(&vPreDir, &vPreDir, &matRev);

		_float fCosTh = D3DXVec3Dot(&vUp, &vPreDir);
		//if (fCosTh < 0.9f && fCosTh > -0.6f)
		//	m_vEyeDir = -vPreDir;;
	}
}

void CCodyCamera::Key_Input(const _float& fTimeDelta)
{
	if (m_bSpetulaScene)
		return;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

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

CCodyCamera* CCodyCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CCodyCamera*	pInstance = new CCodyCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCodyCamera::Fix_Mouse(void)
{
	if (GetFocus() != g_hWnd)
		return;
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}
