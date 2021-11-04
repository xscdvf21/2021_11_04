#include "stdafx.h"
#include "MayCamera.h"
#include "Export_Function.h"
#include "May.h"

USING(Engine)

CMayCamera::CMayCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
	, m_fViewAngle(30.f), m_fViewAtHeight(0.f), m_fViewDistance(9.f), m_fViewHeight(0.f), m_bSilde(false), m_bSpetulaScene(false)
	, m_bChange(false), m_fChangeTime(0.f)
{
	xAxis = { 1.f,0.f,0.f };
	yAxis = { 0.f,1.f,0.f };
	zAxis = { 0.f,0.f,1.f };
}

CMayCamera::~CMayCamera(void)
{
}

void CMayCamera::LateUpdate_FreeCamera(const _float & TimeDelta)
{
	D3DXMATRIX viewMatrix;
	D3DXMatrixIdentity(&viewMatrix);

	D3DXVec3Cross(&xAxis, &m_vUp, (_vec3*)&m_matView._31);
	D3DXVec3Normalize(&xAxis, &xAxis);

	D3DXVec3Cross(&zAxis, &xAxis, &m_vUp);
	D3DXVec3Normalize(&zAxis, &zAxis);

	m_matView._11 = xAxis.x;
	m_matView._21 = xAxis.y;
	m_matView._31 = xAxis.z;

	m_matView._12 = yAxis.x;
	m_matView._22 = yAxis.y;
	m_matView._32 = yAxis.z;

	m_matView._13 = zAxis.x;
	m_matView._23 = zAxis.y;
	m_matView._33 = zAxis.z;

	m_matView._41 = D3DXVec3Dot(&m_vAt, &xAxis);
	m_matView._42 = D3DXVec3Dot(&m_vAt, &yAxis);
	m_matView._43 = D3DXVec3Dot(&m_vAt, &zAxis);
	Engine::Get_Renderer()->Set_ViewMatrix(0, m_matView);
	Engine::Get_Renderer()->Set_ViewMatrix(2, m_matView);

}

void CMayCamera::Update_FreeCamera(const _float& TimeDelta)
{
	CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pCodyTransformCom, );

	_vec3 vPlayerPos = pCodyTransformCom->Get_Position();
	_vec3 vOffSet = { 0.f,0.62f + m_fOffset,0.f };

	m_vAt = vPlayerPos;// +vOffSet;
	m_fDist = 12.f;
	m_vEye = m_vAt + _vec3(0.0, 3.0f, -7.0f)* m_fDist;


	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_float fPadMoveX = 0.0f;
	fPadMoveX = Engine::Get_DIPadMove(DIC_RS_X);

	if (fabs(fPadMoveX) <= 0.1f) {}
	else
	{
		m_vAngle.y = fPadMoveX * 3.f;


		if (m_vAngle.y < 0.0f)
			m_vAngle.y += 360.0f;

		if (m_vAngle.y > 360.0f)
			m_vAngle.y -= 360.0f;

		Pitch(m_vAngle.y);
	}

	_float fPadMoveY = 0.0f;
	fPadMoveY = Engine::Get_DIPadMove(DIC_RS_Y);

	if (fabs(fPadMoveY) <= 0.1f) {}
	else
	{
		m_vAngle.x = -fPadMoveY * 2.f;

		m_vAngle.x = min(m_vAngle.x, 90.0f);
		m_vAngle.x = max(m_vAngle.x, -90.0f);

		Roll(m_vAngle.x);

	}

}

void CMayCamera::Update_Default()
{
	CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pCodyTransformCom, );

	_vec3 vPlayerPos = pCodyTransformCom->Get_Position();

	vPlayerPos += m_vUp*(0.62f*(m_fDist / 12.f));
	vPlayerPos += m_vUp*(m_fOffset*(m_fDist / 12.f));

	m_vTargetAt = vPlayerPos;

	_vec3 MovePos = m_vTargetAt + (m_vEyeDir * -m_fDist);

	m_vTargetEye = MovePos;
}

void CMayCamera::Update_Stalking()
{
	CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
	NULL_CHECK_RETURN(pCodyTransformCom, );

	_vec3 vPlayerPos = pCodyTransformCom->Get_Position();
	vPlayerPos.y += 0.8f*(m_fDist / 12.f);

	m_vTargetAt = vPlayerPos;

	_vec3 MovePos = m_vTargetAt + (m_vEyeDir*-m_fDist);

	m_vTargetEye = MovePos;
}

void CMayCamera::Update_Fix()
{
	if (m_bFixStart)
	{
		m_vTargetAt = m_vFixAt;
		m_vTargetEye = m_vFixEye;
		//m_vAt = m_vFixAt;
		//m_vEye = m_vFixEye;
		m_bFixStart = false;

	}
}
void CMayCamera::Update_Tracing()
{
	if (!m_pTarget)
		return;
	_mat matWorld = m_pTarget->Get_EventPos();
	if (matWorld == _mat{})
		Set_Mode(CAMERA_DEFAULT);
	m_vTargetAt = *((_vec3*)(&matWorld._41));
}



void CMayCamera::Set_TargetUp(_vec3 vUp)
{
	Engine::CCamera::Set_TargetUp(vUp);

}

void CMayCamera::Pitch(const _float& fValue)
{
	_mat matRot;
	D3DXMatrixRotationAxis(&matRot, &yAxis, D3DXToRadian(fValue));
	D3DXVec3TransformCoord(&zAxis, &zAxis, &matRot);
	D3DXVec3TransformCoord(&xAxis, &xAxis, &matRot);
}

void CMayCamera::Roll(const _float& fValue)
{
	_mat matRot;
	D3DXMatrixRotationAxis(&matRot, &xAxis, D3DXToRadian(fValue));
	D3DXVec3TransformCoord(&zAxis, &zAxis, &matRot);
	D3DXVec3TransformCoord(&yAxis, &yAxis, &matRot);
}

void CMayCamera::Yaw(const _float& fValue)
{
	_mat matRot;
	D3DXMatrixRotationAxis(&matRot, &zAxis, D3DXToRadian(fValue));
	D3DXVec3TransformCoord(&xAxis, &xAxis, &matRot);
	D3DXVec3TransformCoord(&yAxis, &yAxis, &matRot);
}

HRESULT CMayCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
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

	//_vec3 vUp = { 0.0f, 1.0f, 0.0f };

	//_float fCosTh = D3DXVec3Dot(&m_vEyeDir, &vUp);
	//m_fCorrectAngle = acosf(fCosTh);
	m_fCorrectAngle = 0.0f;

	m_vOffset = { 0.f,0.62f + m_fOffset,0.f };

	m_fDist = 12.0f;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CMayCamera::Update_Object(const _float& TimeDelta)
{
	Update_Timer(TimeDelta);
	if (m_bFreeCamera)
	{
		Update_FreeCamera(TimeDelta);
		return 0;
	}
	if (!g_bMenu)
	{
		Key_Input(TimeDelta);

		Shake(TimeDelta);
		if (true == m_bMouseFix)
		{
			Update_CameraPos();
			Fix_Mouse();
			Mouse_Rev(TimeDelta);
		}
	}

	return 0;
}

_int CMayCamera::LateUpdate_Object(const _float & TimeDelta)
{
	if (m_bFreeCamera)
	{
		LateUpdate_FreeCamera(TimeDelta);
		return 0;
	}
	Update_View(TimeDelta);
	CheckRayHit(TimeDelta);
	if (m_bStart)
	{
		m_bStart = false;
		CTransform* pCodyTransformCom = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
		if (pCodyTransformCom)
		{
			_vec3 vPlayerPos = pCodyTransformCom->Get_Position();
			vPlayerPos.y += 0.62f;
			m_vAt = vPlayerPos;
			m_vEye = m_vAt + (m_vEyeDir*-m_fDist);
		}
	}
	_float4x4 matViewMatrix;
	D3DXMatrixLookAtLH(&matViewMatrix, &m_vEye, &(m_vAt + m_vShake), &m_vUp);

	Engine::Get_Renderer()->Set_ViewMatrix(0, matViewMatrix);
	Engine::Get_Renderer()->Set_ViewMatrix(2, matViewMatrix);
	m_matView = matViewMatrix;

	return _int();
}

void CMayCamera::Set_FreeMode()
{
	m_bFreeCamera = true;
}

void CMayCamera::Set_EyeDir(const _vec3& vUp)
{


}

void CMayCamera::Set_TargetUpRotate(_vec3 param1,_bool bState)
{
	_vec3 vPreUp = m_vTargetUp;
	m_vTargetUp = param1;



	if (D3DXVec3Length(&(m_vTargetUp - vPreUp)) < 0.001f)
		return;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_float4x4 matRev;
	CTransform* pTransform = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
	if (!pTransform)
		return;
	_vec3 vAxis = *(_vec3*)&pTransform->Get_WorldMatrix()->_11;
	D3DXVec3Normalize(&vAxis, &vAxis);
	_float fRad = 0.f;

	if(bState)
		fRad = acosf(D3DXVec3Dot(&m_vTargetUp, &vPreUp));
	else 
		fRad = -acosf(D3DXVec3Dot(&m_vTargetUp, &vPreUp));


	D3DXMatrixRotationAxis(&matRev, &vAxis, fRad);

	D3DXVec3TransformCoord(&m_vEyeDir, &m_vEyeDir, &matRev);
}

void CMayCamera::Free(void)
{
	Engine::CCamera::Free();
}



void CMayCamera::CheckRayHit(const _float & fTimeDelta)
{
	if (!m_pScene || !m_bRay||!m_bForceRay)
		return;
	const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here


	CGameObject* pPlayer = Engine::Get_GameObject(L"GameObject", L"May");
	PxQueryFilterData filterData;
	filterData.data = PxFilterData(FilterGroup::eGround, 0, 0, 0);

	_vec3 vPos = m_vTargetAt;
	_vec3 vDir = m_vEye - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	if (m_pScene->raycast(PxVec3(m_vTargetAt.x, m_vTargetAt.y, m_vTargetAt.z),
		PxVec3(vDir.x, vDir.y, vDir.z),
		m_fDist,
		buf, PxHitFlag::eMESH_BOTH_SIDES))
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




void CMayCamera::Update_CameraPos()
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

void CMayCamera::Mouse_Rev(const _float & fTimeDelta)
{
	if (!m_bRot)
		return;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_float fPadMoveX = 0.0f;
	fPadMoveX = Engine::Get_DIPadMove(DIC_RS_X);

	if (fabs(fPadMoveX) <= 0.1f) {}
	else
	{
		m_vAngle.y = fPadMoveX * 3.f;

		_float4x4 matRev;
		_vec3 vAxis = *(_vec3*)&matCamWorld._21;
		D3DXVec3Normalize(&vAxis, &vAxis);
		D3DXMatrixRotationAxis(&matRev, &vAxis, D3DXToRadian(m_vAngle.y));

		D3DXVec3TransformCoord(&m_vEyeDir, &m_vEyeDir, &matRev);

	}

	_float fPadMoveY = 0.0f;
	fPadMoveY = Engine::Get_DIPadMove(DIC_RS_Y);

	if (fabs(fPadMoveY) <= 0.1f) {}
	else
	{
		m_vAngle.x = -fPadMoveY * 2.f;

		_float4x4 matRev;
		_float3 vCamRight;
		memcpy(&vCamRight, (_vec3*)&matCamWorld._11, sizeof(_float3));
		D3DXMatrixRotationAxis(&matRev, &vCamRight, D3DXToRadian(m_vAngle.x));
		D3DXVec3TransformCoord(&m_vEyeDir, &m_vEyeDir, &matRev);

		m_fCorrectAngle += D3DXToRadian(m_vAngle.x);

	}
	/*if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Z))
	{
		if (dwMouseMove > 0)
			m_fDistance -= 0.2f;
		else
			m_fDistance += 0.2f;

		if (m_fDistance >= 6.0f)
			m_fDistance = 6.0f;
		else if (m_fDistance < 1.0f)
			m_fDistance = 1.0f;
	}*/
}

void CMayCamera::Key_Input(const _float& fTimeDelta)
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

CMayCamera* CMayCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CMayCamera*	pInstance = new CMayCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMayCamera::Mouse_Move(const _float& fTimeDelta)
{
	if (m_bSpetulaScene || m_bStopCamera || m_bMission)
		return;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove = 0;

	CTransform* pPlayerTransform = (CTransform*)Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
	CGameObject* pPlayer = (CGameObject*)Engine::Get_GameObject(L"GameObject", L"May");

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

	if (!m_bSilde)
	{
		if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
		{
			_vec3	vLook = m_vAt - m_vEye;

			_matrix	matRot;
			D3DXMatrixRotationAxis(&matRot, &vUP, D3DXToRadian(_float(dwMouseMove) / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);


			m_vAt = m_vEye + vLook;

			pPlayerTransform->Rotation(ROT_Y, D3DXToRadian((_float(dwMouseMove) / 10.f) * 90.f * fTimeDelta));
			//CTransform* pSpongeBobTransform = (CTransform*)dynamic_cast<CPlayer*>(pPlayer)->Get_SpongeBob()->Get_Component(L"Com_Transform", ID_DYNAMIC);
			//pSpongeBobTransform->Rotation(ROT_Y, D3DXToRadian((dwMouseMove / 10) * 90.f * fTimeDelta));
		}
	}
}

void CMayCamera::Fix_Mouse(void)
{
	if (GetFocus() != g_hWnd)
		return;
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}


