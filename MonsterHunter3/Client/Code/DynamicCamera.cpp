 #include "stdafx.h"
#include "DynamicCamera.h"
#include "Export_Fucntion.h"
#include "SoundMgr.h"
USING(Engine)

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{

	Add_Component();

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY		= fFovY;
	m_fAspect	= fAspect;
	m_fNear		= fNear;
	m_fFar		= fFar;

	//D3DXMatrixIdentity(&matRot);

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	memcpy(&vCameraLook, &matCamWorld.m[2][0], sizeof(_vec3));
	memcpy(&vCameraRight, &matCamWorld.m[0][0], sizeof(_vec3));
	_vec3 vCameraUp;

	memcpy(&vCameraUp, &matCamWorld.m[1][0], sizeof(_vec3));

	//_vec3	vEyePos = m_vEye;

	//m_pTransformCom->m_vInfo[INFO::INFO_POS] = m_vEye;
	//
	FAILED_CHECK_RETURN(Engine::CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CDynamicCamera::Update_Object(const _float& TimeDelta)
{



	Key_Input(TimeDelta);

	if (true == m_bMouseFix)
	{
		Fix_Mouse();
		Mouse_Move(TimeDelta);
	}	

	//Fix_Mouse();
	//Mouse_Move(TimeDelta);
	
	if (Engine::Get_DIKeyState(DIK_F5))
	{

		m_bShake = true;
	}

	if (true == m_bShake)
	{
		Camera_Shaking(TimeDelta);
	}

	_int iExit = Engine::CCamera::Update_Object(TimeDelta);

	if (!m_bShakeTemp)
		m_bShakeTemp = true;
	else if (m_bShakeTemp)
		m_bShakeTemp = false;




	return iExit;
}

void CDynamicCamera::Camera_Shaking(const _float & TimeDelta)
{
	
	m_fShakeTime += TimeDelta;

	if (m_fShakeTime < 0.5f)
	{
	
		_vec3 vLook = m_vEye - m_vAt;
		D3DXVec3Normalize(&vLook, &vLook);
		vLook.y = 0.f;

		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vRight;
		D3DXVec3Cross(&vRight, &vLook, &vUp);

		D3DXVec3Normalize(&vRight, &vRight);

		_vec3 vLeft = vRight * -1.f;

		vRight = vRight * 10.f;
		vLeft = vLeft * 10.f;

		m_fDistanceToTarget++;
		m_vCurrentEye = m_vEye;

		if (!m_bShakeTemp)
		{
			m_fDistanceToTarget = 8 + 40.f * TimeDelta;
			/*m_vEye += vRight * TimeDelta;*/
		}

		else if (m_bShakeTemp)
		{
			m_fDistanceToTarget = 8 - 40.f * TimeDelta;
			/*m_vEye += vLeft * TimeDelta;*/
		}


	}
	else if (m_fShakeTime > 0.5f)
	{
		m_fDistanceToTarget = 8.f;
		m_fShakeTime = 0.f;
		m_bShake = false;
		m_vEye = m_vCurrentEye;
	}


}

void CDynamicCamera::BossCamera_Shaking(const _float & TimeDelta)
{
	m_fShakeTime2 += TimeDelta;

	if (m_fShakeTime2 < 0.5f)
	{

		_vec3 vLook = m_vEye - m_vAt;
		D3DXVec3Normalize(&vLook, &vLook);
		vLook.y = 0.f;

		_vec3 vUp = { 0.f, 1.f, 0.f };
		_vec3 vRight;
		D3DXVec3Cross(&vRight, &vLook, &vUp);

		D3DXVec3Normalize(&vRight, &vRight);

		_vec3 vLeft = vRight * -1.f;

		vRight = vRight * 10.f;
		vLeft = vLeft * 10.f;

		m_fDistanceToTarget++;
		m_vCurrentEye = m_vEye;

		if (!m_bShakeTemp2)
		{
			m_fDistanceToTarget = 8 + 40.f * TimeDelta;
			/*m_vEye += vRight * TimeDelta;*/
		}

		else if (m_bShakeTemp2)
		{
			m_fDistanceToTarget = 8 - 40.f * TimeDelta;
			/*m_vEye += vLeft * TimeDelta;*/
		}


	}
	else if (m_fShakeTime > 0.5f)
	{
		m_fDistanceToTarget = 8.f;
		m_fShakeTime2 = 0.f;
		m_bShake2 = false;
		m_vEye = m_vCurrentEye;
	}
}

void CDynamicCamera::Free(void)
{
	Engine::CCamera::Free();
}


void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	if (Engine::Get_DIKeyState(DIK_LCONTROL) & 0x80)
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

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, );

	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
	pPlayerTransform->Get_Info(INFO_POS, &m_pPlayerPos);

	

	vPlayerDis = m_vEye - m_vAt; //서순 문제였다.
	m_vAt = m_pPlayerPos;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove = 0;
	D3DXVec3Normalize(&vPlayerDis, &vPlayerDis);
	_vec3 vUp = { 0.f,1.f,0.f };
	vPlayerDis = vPlayerDis * m_fDistanceToTarget;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		_matrix	matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 20.f));
		D3DXVec3TransformNormal(&vPlayerDis, &vPlayerDis, &matRot);

	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		_matrix	matRot;
		D3DXMatrixIdentity(&matRot);

		_vec3	Axis;
		_vec3	vPlayerAngle;
		vPlayerAngle = vPlayerDis;
		D3DXVec3Cross(&Axis, &vUp, &vPlayerDis);
	
		D3DXVec3Normalize(&vPlayerAngle, &vPlayerAngle);

		fAngleMax = D3DXToDegree(acosf(D3DXVec3Dot(&vUp, &vPlayerAngle)));
		
		if (fAngleMax >= 60 && fAngleMax <= 90)
		{
			D3DXMatrixRotationAxis(&matRot, &Axis, D3DXToRadian(dwMouseMove / 20.f));
			D3DXVec3TransformNormal(&vPlayerDis, &vPlayerDis, &matRot);
		}
		else if(fAngleMax < 60)
		{
			_float AngleTemp;
			AngleTemp = 60 - fAngleMax;
			D3DXMatrixRotationAxis(&matRot, &Axis, D3DXToRadian(AngleTemp));
			D3DXVec3TransformNormal(&vPlayerDis, &vPlayerDis, &matRot);
		}
		else if (fAngleMax > 90)
		{
			_float AngleTemp;
			AngleTemp = fAngleMax - 90;
			D3DXMatrixRotationAxis(&matRot, &Axis, D3DXToRadian(-AngleTemp));
			D3DXVec3TransformNormal(&vPlayerDis, &vPlayerDis, &matRot);
		}
	}

	m_vEye = m_vAt + vPlayerDis;
}

void CDynamicCamera::Fix_Mouse(void)
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);



}


HRESULT CDynamicCamera::Add_Component()
{

	Engine::CComponent*		pComponent = nullptr;


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Camera_Transform", pComponent);


	return S_OK;
}

