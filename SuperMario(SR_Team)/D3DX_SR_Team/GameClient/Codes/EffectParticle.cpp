#include "framework.h"
#include "..\Headers\EffectParticle.h"


CEffectParticle::CEffectParticle(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_bStart(false), m_bMultiColor(false)
	, m_fLifeTime(0.f), e_Axis(NUN)
	, m_bJump(false), m_bJumpStart(true), m_fJumpPowerY(0.f), m_fJumpPowerXZ(0.f), m_fJumpTime(0.f)
{
	ZeroMemory(&m_vScaleStart, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vScaleEnd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vScaleAdd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vColorStart, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vColorEnd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vColorAdd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vShotting, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vJumpingPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vJumpStartPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vStartPoint, sizeof(D3DXVECTOR3));
}

CEffectParticle::CEffectParticle(const CEffectParticle & other)
	: CGameObject(other)
	, m_bStart(false)
	, m_fLifeTime(other.m_fLifeTime)
	, m_bJumpStart(true), m_fJumpPowerXZ(0.f), m_fJumpTime(0.f)
	//한승현
	, m_vScaleStart(other.m_vScaleStart)
	, m_vScaleEnd(other.m_vScaleEnd)
	, m_vScaleAdd(other.m_vScaleAdd)
	, m_vColorStart(other.m_vColorStart)
	, m_vColorEnd(other.m_vColorEnd)
	, m_bMultiColor(other.m_bMultiColor)
	, e_Axis(other.e_Axis)
	, m_fRotation(other.m_fRotation)
	, m_bJump(other.m_bJump)
	, m_fJumpPowerY(other.m_fJumpPowerY)
	, m_vShotting(other.m_vShotting)
	, m_fAccel(other.m_fAccel)

	

{
	/*ZeroMemory(&m_vScaleStart, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vScaleEnd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vScaleAdd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vColorStart, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vColorEnd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vColorAdd, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vShotting, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vJumpingPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vJumpStartPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vStartPoint, sizeof(D3DXVECTOR3));*/

	Safe_AddReference(m_pVIBufferCom);




}

HRESULT CEffectParticle::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectParticle::Ready_GameObject(void * pArg /*= nullptr*/)
{
 	if (FAILED(CGameObject::Ready_GameObject(pArg)))
		return E_FAIL;
	
	if (FAILED(Add_Component()))
		return E_FAIL;


	Set_Start();

	return S_OK;
}

_uint CEffectParticle::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (true == m_bStop)
	{
		return _uint();
	}


	if (true == m_bDead)
	{
		m_bStart = false;
		//bIsDead = true;
	}

	CGameObject::Update_GameObject(fDeltaTime);
//	m_pTransformCom->Set_Scale(m_vScaleStart);

//	m_bStart = true;
	if (m_bStart)
	{
		if (m_fLifeTime < 0)
		{
			m_bStop = true;
		}
		else
		{
			m_fLifeTime--;
		}

		Jumping(fDeltaTime);
		m_pTransformCom->Set_Scale(m_vScaleStart);
		Set_ChangeColor(0, (int)m_vColorStart.x, (int)m_vColorStart.y, (int)m_vColorStart.z, 0);


	}
	if (m_bStart)
	{
		Update_Scale(fDeltaTime);
		Update_Trans(fDeltaTime);
		Update_Color(fDeltaTime);
		Update_Rotation(fDeltaTime);
	}

	return _uint();

}

_uint CEffectParticle::LateUpdate_GameObject(_float fDeltaTime)
{
	//여기서 랜더 리스트에 추가하는구나.
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (true == m_bStop)
	{
		return _uint();
	}
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	if (FAILED(pManagement->Add_RenderList(ERenderPriority::NoAlpha, this)))
		return 0;

	return m_pTransformCom->Update_Transform();
}

HRESULT CEffectParticle::Render_GameObject()
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return 0;

	if (true == m_bStop)
	{
		return S_OK;
	}

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectParticle::Add_Component()
{
	if (FAILED(CGameObject::Add_Component(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeColor",
		L"Com_VIBuffer",
		(CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}



CEffectParticle * CEffectParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffectParticle* pInstance = new CEffectParticle(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CEffectParticle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffectParticle::Clone(void * pArg)
{
	CEffectParticle* pInstance = new CEffectParticle(*this); 
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEffectParticle");
		Safe_Release(pInstance);
	}
//	m_bStart = true;
	return pInstance;
}

CGameObject * CEffectParticle::CloneToClone(CEffectParticle* Clone)
{
	CEffectParticle* pInstance = new CEffectParticle(*Clone);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		PRINT_LOG(L"클투클", L"Failed To CloneToClone CEffectParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectParticle::Free()
{
	Safe_Release(m_pVIBufferCom);
	CGameObject::Free();
}

HRESULT CEffectParticle::Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex)
{
	if (FAILED(dynamic_cast<CVIBuffer_CubeColor*>(m_pVIBufferCom)
		->Change_VertexColor(D3DCOLOR_ARGB(_iA, _iR, _iG, _iB), _iIndex)))
	{
		PRINT_LOG(L"CCubeColor", L"뭐였더라");
	}

	return S_OK;
}

void CEffectParticle::Set_Position(D3DXVECTOR3 _vStartPoint)
{
	m_pTransformCom->Set_Position(_vStartPoint);
}

void CEffectParticle::Set_Scale(D3DXVECTOR3 _vScaleStart, D3DXVECTOR3 _vScaleEnd)
{
	m_vScaleStart = _vScaleStart;
	m_vScaleEnd = _vScaleEnd;
	m_vScaleAdd = (m_vScaleEnd - m_vScaleStart) / m_fLifeTime;
}

void CEffectParticle::Set_Color(D3DXVECTOR3 _vColorStart, D3DXVECTOR3 _vColorEnd, bool _bMulti)
{
	m_vColorStart = _vColorStart;
	m_vColorEnd = _vColorEnd;
	m_vColorAdd = (m_vColorEnd - m_vColorStart) / (m_fLifeTime);
	m_bMultiColor = _bMulti;
}

void CEffectParticle::Set_Rotation(int _eAxis, float _Rot)
{
	e_Axis = (RotationAxis)_eAxis;
	m_fRotation = _Rot;
}

void CEffectParticle::Set_Jump(bool _bJump, float _fJumpPower, D3DXVECTOR3 _vShot)
{
	m_bJump = _bJump;
	m_fJumpPowerY = _fJumpPower;
	m_vShotting = _vShot;
}

void CEffectParticle::Update_Color(float _fDeltaTime)
{
	if (m_bMultiColor)
	{
		m_vColorStart += m_vColorAdd * 10.f;

		if (m_vColorStart.x < 0.f || m_vColorStart.x > 255.f)
		{
			int ColorTemp = 0;
			ColorTemp = abs((int)m_vColorStart.x) % 255;
			m_vColorStart.x = (float)ColorTemp;
		}
		if (m_vColorStart.y < 0.f || m_vColorStart.y > 255.f)
		{
			int ColorTemp = 0;
			ColorTemp = abs((int)m_vColorStart.y) % 255;
			m_vColorStart.y = (float)ColorTemp;
		}
		if (m_vColorStart.z < 0.f || m_vColorStart.z > 255.f)
		{
			int ColorTemp = 0;
			ColorTemp = abs((int)m_vColorStart.z) % 255;
			m_vColorStart.z = (float)ColorTemp;
		}

	}
	else
	{
		if (abs(m_vColorStart.x - m_vColorEnd.x) < 1.f)
		{
			m_vColorStart.x = m_vColorEnd.x;
		}
		else
		{
			m_vColorStart.x += m_vColorAdd.x;
		}
		if (abs(m_vColorStart.y - m_vColorEnd.y) < 1.f)
		{
			m_vColorStart.y = m_vColorEnd.y;
		}
		else
		{
			m_vColorStart.y += m_vColorAdd.y;
		}
		if (abs(m_vColorStart.z - m_vColorEnd.z) < 1.f)
		{
			m_vColorStart.z = m_vColorEnd.z;
		}
		else
		{
			m_vColorStart.z += m_vColorAdd.z;
		}
	}

}

void CEffectParticle::Update_Scale(float _fDeltaTime)
{
	if (abs(m_vScaleStart.x - m_vScaleEnd.x) < 0.1f)
	{
		m_vScaleStart = m_vScaleEnd;
	}
	else
	{
		m_vScaleStart += m_vScaleAdd;
	}
}

void CEffectParticle::Update_Rotation(float fDeltaTime)
{
	switch (e_Axis)
	{
	case RotationAxis::X:
		m_pTransformCom->Rotate(EAxis::X, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::Y:
		m_pTransformCom->Rotate(EAxis::Y, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::Z:
		m_pTransformCom->Rotate(EAxis::Z, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::XY:
		m_pTransformCom->Rotate(EAxis::X, fDeltaTime * m_fRotation);
		m_pTransformCom->Rotate(EAxis::Y, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::XZ:
		m_pTransformCom->Rotate(EAxis::X, fDeltaTime * m_fRotation);
		m_pTransformCom->Rotate(EAxis::Z, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::YZ:
		m_pTransformCom->Rotate(EAxis::Y, fDeltaTime * m_fRotation);
		m_pTransformCom->Rotate(EAxis::Z, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::XYZ:
		m_pTransformCom->Rotate(EAxis::X, fDeltaTime * m_fRotation);
		m_pTransformCom->Rotate(EAxis::Y, fDeltaTime * m_fRotation);
		m_pTransformCom->Rotate(EAxis::Z, fDeltaTime * m_fRotation);
		break;
	case RotationAxis::NUN:
		break;
	default:
		break;
	}
}

void CEffectParticle::Update_Trans(float _fDeltaTime)
{
//	m_vStartPoint = {5.f, 5.f, 5.f};
//	m_pTransformCom->Set_Position(m_vStartPoint);
//	m_pTransformCom->Update_Transform();
}

void CEffectParticle::Jumping(float fDeltaTime)
{
	if (m_bJump)
	{
		if (m_bJumpStart)
		{//TransformDesc
			m_vJumpStartPoint = m_pTransformCom->Get_Position();
			m_vJumpingPoint = m_vJumpStartPoint;
			m_fJumpPowerXZ = m_fJumpPowerY * 0.005f * m_pTransformCom->Get_Scale().x;
			m_bJumpStart = false;
		}

		// Y값 이동
		m_vJumpingPoint.y = m_vJumpStartPoint.y + (m_fJumpPowerY * m_fJumpTime - 0.5f * 9.8f * m_fJumpTime * m_fJumpTime);
		// (X, Z)값이동.
		m_vJumpingPoint.x += m_fJumpPowerXZ * m_fJumpTime * m_vShotting.x;
		m_vJumpingPoint.z += m_fJumpPowerXZ * m_fJumpTime * m_vShotting.z;

		m_pTransformCom->Set_Position(m_vJumpingPoint);
		m_fJumpTime += fDeltaTime;
	}
	else
	{
		if (m_bJumpStart)
		{//TransformDesc
			m_fJumpTime = 0.1f;
			m_vJumpStartPoint = m_pTransformCom->Get_Position();
			m_vJumpingPoint = m_vJumpStartPoint;
			m_fJumpPowerXZ = m_fJumpPowerY * 0.005f * m_pTransformCom->Get_Scale().x;
			m_bJumpStart = false;
		}

		m_vJumpingPoint.x += m_fJumpTime * m_vShotting.x;
		m_vJumpingPoint.y += m_fJumpTime * m_vShotting.y;
		m_vJumpingPoint.z += m_fJumpTime * m_vShotting.z;
		m_fJumpTime += fDeltaTime * 0.05;

		m_pTransformCom->Set_Position(m_vJumpingPoint);
	}

}
