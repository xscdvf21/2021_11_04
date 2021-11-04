#include "stdafx.h"
#include "..\Headers\TransformTool.h"


CTransformTool::CTransformTool(LPDIRECT3DDEVICE9 pDevice)
	:CComponentTool(pDevice)
{
	ZeroMemory(&m_TransformDesc, sizeof(TRANSFORM_DESC));
	m_TransformDesc.vScale = { 1.f, 1.f, 1.f };
}

CTransformTool::CTransformTool(const CTransformTool & other)
	:CComponentTool(other)
	, m_TransformDesc(other.m_TransformDesc)
{
}

const _float3 & CTransformTool::Get_Position() const
{
	return m_TransformDesc.vPosition;
}

const _float4x4 & CTransformTool::Get_WorldMatrix() const
{
	return m_TransformDesc.matWorld;
}

const _float3 & CTransformTool::Get_Scale() const
{
	return m_TransformDesc.vScale;
}

const _float3 & CTransformTool::Get_Rotation() const
{
	return m_TransformDesc.vRotate;
}

void CTransformTool::Set_Position(const _float3 & vPos)
{
	m_TransformDesc.vPosition = vPos;
}

void CTransformTool::Set_WorldMatrix(const _float4x4 & matWorld)
{
	m_TransformDesc.matWorld = matWorld;
}

void CTransformTool::Set_Scale(const _float3 & vScale)
{
	m_TransformDesc.vScale = vScale;
}

void CTransformTool::Set_RelativeScale(const _float3 & vScale)
{

}


void CTransformTool::Set_Rotation(const _float3 & vRotation)
{
	m_TransformDesc.vRotate = _float3(D3DXToRadian(vRotation.x), D3DXToRadian(vRotation.y), D3DXToRadian(vRotation.z));
}

HRESULT CTransformTool::Ready_Component_Prototype()
{
	if (CComponentTool::Ready_Component_Prototype())
		return E_FAIL;

	return S_OK;
}

HRESULT CTransformTool::Ready_Component(void * pArg)
{
	if (CComponentTool::Ready_Component(pArg))
		return E_FAIL;

	if (nullptr != pArg)
	{
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORM_DESC));
	}

	return S_OK;
}

_uint CTransformTool::Update_Transform()
{
	if (FAILED(CPipelineStaticsTool::Setup_WorldMatrix(
		&m_TransformDesc.matWorld,
		&m_TransformDesc.vScale,
		&m_TransformDesc.vRotate,
		&m_TransformDesc.vPosition)))
		return 0;

	return _uint();
}

HRESULT CTransformTool::Set_Transform()
{
	return m_pDevice->SetTransform(D3DTS_WORLD, &m_TransformDesc.matWorld);
}

void CTransformTool::Go_Straight(_float fDeltaTime)
{
	_float3 vLook;

	/* 상태행렬의 3행은 look벡터이다 */
	memcpy(&vLook, &m_TransformDesc.matWorld.m[2][0], sizeof(_float3));
	D3DXVec3Normalize(&vLook, &vLook);

	m_TransformDesc.vPosition += vLook * m_TransformDesc.fSpeedPerSecond * fDeltaTime;
}

void CTransformTool::Go_Side(_float fDeltaTime)
{
	/* 오브젝트가 바라보는 방향을 기준으로 양옆 움직여라 */
	_float3 vRight;

	/* 상태행렬의 1행은 right벡터이다 */
	memcpy(&vRight, &m_TransformDesc.matWorld.m[0][0], sizeof(_float3));
	D3DXVec3Normalize(&vRight, &vRight);

	m_TransformDesc.vPosition += vRight * m_TransformDesc.fSpeedPerSecond * fDeltaTime;
}

void CTransformTool::Rotate(EAxis eAxis, _float fDeltaTime)
{
	switch (eAxis)
	{
	case EAxis::X:
		m_TransformDesc.vRotate.x += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	case EAxis::Y:
		m_TransformDesc.vRotate.y += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	case EAxis::Z:
		m_TransformDesc.vRotate.z += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	case EAxis::XY:
		m_TransformDesc.vRotate.x += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		m_TransformDesc.vRotate.y += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	case EAxis::XZ:
		m_TransformDesc.vRotate.x += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		m_TransformDesc.vRotate.z += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	case EAxis::YZ:
		m_TransformDesc.vRotate.y += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		m_TransformDesc.vRotate.z += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	case EAxis::XYZ:
		m_TransformDesc.vRotate.x += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		m_TransformDesc.vRotate.y += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		m_TransformDesc.vRotate.z += m_TransformDesc.fRotatePerSecond * fDeltaTime;
		break;
	}
}

void CTransformTool::Rotate_Quick(EAxis eAxis, _float _fAngle)
{
	switch (eAxis)
	{
	case EAxis::X:
		m_TransformDesc.vRotate.x += _fAngle;
		break;
	case EAxis::Y:
		m_TransformDesc.vRotate.y += _fAngle;
		break;
	case EAxis::Z:
		m_TransformDesc.vRotate.z += _fAngle;
		break;
	}
}


void CTransformTool::Move_ToDirection(const _float3 & vMoveDir, _float fDeltaTime)
{
	m_TransformDesc.vPosition += vMoveDir * m_TransformDesc.fSpeedPerSecond * fDeltaTime;
}

CTransformTool * CTransformTool::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTransformTool* pInstance = new CTransformTool(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponentTool * CTransformTool::Clone(void * pArg)
{
	CTransformTool* pClone = new CTransformTool(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTransform");
		Safe_Release(pClone);
	}

	return pClone;
}

void CTransformTool::Free()
{
	CComponentTool::Free();
}

