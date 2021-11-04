#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CCamera::~CCamera(void)
{

}

void CCamera::Get_EyeAtUp(_vec3 * _vEye, _vec3 * _vAt, _vec3 * vUp)
{


	*(_vEye) = m_vEye;
	*(_vAt) = m_vAt;
	*(vUp) = m_vUp;
}

void CCamera::Get_View(_matrix * _View)
{
	*(_View) = m_matView;
}

void CCamera::Set_EyeAtUp(_vec3 * _vEye, _vec3 * _vAt, _vec3 * vUp)
{
	m_vEye = *(_vEye);
	m_vAt = *(_vAt);
	m_vUp = *(vUp);


}

HRESULT Engine::CCamera::Ready_Object(void)
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

