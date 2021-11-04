#include "stdafx.h"
#include "..\Headers\CubeColor.h"

CCubeColor::CCubeColor(LPDIRECT3DDEVICE9 pDevice)
	: CGameObj(pDevice)
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

}

CCubeColor::CCubeColor(const CCubeColor & other)
	: CGameObj(other)
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

}

HRESULT CCubeColor::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObj::Ready_GameObject_Prototype()))
		return E_FAIL;

	e_Axis = NUN;

	return S_OK;
}

HRESULT CCubeColor::Ready_GameObject(void * pArg)
{
	if (FAILED(CGameObj::Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CCubeColor", L"CCubeColor Ready_GameObj");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
	{
		PRINT_LOG(L"CCubeColor", L"CCubeColor AddComponent");
		return E_FAIL;
	}
	

	return S_OK;
}

_uint CCubeColor::Update_GameObject(_float fDeltaTime)
{

	if (false == m_bSave)
	{
		CCubeColor::Save();
		m_bSave = true;
	}


	if (true == m_bStop)
	{
		return _uint();
	}


	if (true == m_bDead)
	{
		return OBJ_DEAD;
	}

	CGameObj::Update_GameObject(fDeltaTime);


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

		Jumping();
		m_pTransformCom->Set_Scale(m_vScaleStart);
		Set_ChangeColor(0, (int)m_vColorStart.x, (int)m_vColorStart.y, (int)m_vColorStart.z, 0);

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
			m_pTransformCom->Rotate(EAxis::XY, fDeltaTime * m_fRotation);
			break;
		case RotationAxis::XZ:
			m_pTransformCom->Rotate(EAxis::XZ, fDeltaTime * m_fRotation);
			break;
		case RotationAxis::YZ:
			m_pTransformCom->Rotate(EAxis::YZ, fDeltaTime * m_fRotation);
			break;
		case RotationAxis::XYZ:
			m_pTransformCom->Rotate(EAxis::XYZ, fDeltaTime * m_fRotation);
			break;
		case RotationAxis::NUN:
			break;
		default:
			break;
		}

	}

	if (m_bStart)
	{
		Update_Scale();
		Update_Color();
	}

	return _uint();
}

_uint CCubeColor::LateUpdate_GameObject(_float fDeltaTime)
{
	if (true == m_bStop)
	{
		return _uint();
	}

	CGameObj::LateUpdate_GameObject(fDeltaTime);

	//auto pManagement = CManagementTool::Get_Instance();
	//if (nullptr == pManagement)
	//	return 0;

	


	return m_pTransformCom->Update_Transform();
}

HRESULT CCubeColor::Render_GameObject()
{
	if (true == m_bStop)
	{
		return S_OK;
	}

	if (FAILED(m_pTransformCom->Set_Transform()))
		return E_FAIL;

	if (FAILED(CGameObj::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render_VIBuffer()))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CCubeColor::Add_Component()
{
	if (FAILED(CGameObj::Add_Component(
		L"Component_VIBuffer_CubeColor",
		L"Com_VIBuffer",
		(CComponentTool**)&m_pVIBufferCom)))
		return E_FAIL;

	/*For.Com_Transform*/
	CTransformTool::TRANSFORM_DESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransformTool::TRANSFORM_DESC));
	TransformDesc.vScale = { 1.f, 1.f, 1.f };
	TransformDesc.fSpeedPerSecond = 5.f; /* 초당 5만큼 움직여라 */
	TransformDesc.fRotatePerSecond = D3DXToRadian(90.f); /* 초당 90도 회전해라 */

	if (FAILED(CGameObj::Add_Component(
		L"Component_Transform",
		L"Com_Transform",
		(CComponentTool**)&m_pTransformCom,
		&TransformDesc)))
		return E_FAIL;


	return S_OK;
}

CCubeColor * CCubeColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeColor* pInstance = new CCubeColor(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		PRINT_LOG(L"CCubeColor", L"CCubeColor Create 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObj * CCubeColor::Clone(void * pArg)
{
	CCubeColor* pInstance = new CCubeColor(*this); /*복사생성호출*/
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		PRINT_LOG(L"CCubeColor", L"CCubeColor Clone 실패");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeColor::Free()
{
	CGameObj::Free();

}

HRESULT CCubeColor::Set_ChangeColor(int _iA, int _iR, int _iG, int _iB, _uint _iIndex)
{
	if (FAILED(dynamic_cast<CVIBuffer_CubeColorTool*>(m_pVIBufferCom)
		->Change_VertexColor(D3DCOLOR_ARGB(_iA, _iR, _iG, _iB), _iIndex)))
	{
		PRINT_LOG(L"CCubeColor", L"색 변경 오류");
	}
	return S_OK;
}

HRESULT CCubeColor::Set_Position(D3DXVECTOR3 _vStartPoint)
{
	m_pTransformCom->Set_Position(_vStartPoint);

	return S_OK;
}

HRESULT CCubeColor::Set_Scale(D3DXVECTOR3 _vScaleStart, D3DXVECTOR3 _vScaleEnd)
{
	m_vScaleStart = _vScaleStart;
	m_vScaleEnd = _vScaleEnd;
	m_vScaleAdd = (m_vScaleEnd - m_vScaleStart) / m_fLifeTime;

	return S_OK;
}

void CCubeColor::Set_Color(D3DXVECTOR3 _vColorStart, D3DXVECTOR3 _vColorEnd, bool _bMulti)
{
	m_vColorStart = _vColorStart;
	m_vColorEnd = _vColorEnd;
	m_vColorAdd = (m_vColorEnd - m_vColorStart) / (m_fLifeTime) ;
	m_bMultiColor = _bMulti;
}

void CCubeColor::Set_Rotation(bool _RotX, bool _RotY, bool _RotZ, float _Rot)
{//현재는 Min값만 투입.
	if (_RotX)
	{
		if (_RotY)
		{
			if (_RotZ)
			{ 
				e_Axis = XYZ;
			}
			else
			{
				e_Axis = XY;
			}
		}
		else
		{
			if (_RotZ)
			{
				e_Axis = XZ;
			}
			else
			{
				e_Axis = X;
			}
		}

	}
	else
	{
		if (_RotY)
		{
			if (_RotZ)
			{
				e_Axis = YZ;
			}
			else
			{
				e_Axis = Y;
			}
		}
		else
		{
			if (_RotZ)
			{
				e_Axis = Z;
			}
			else
			{
				e_Axis = NUN;
			}
		}
	}
	m_fRotation = _Rot;
}

void CCubeColor::Set_Jump(bool _bGrvityOn, float _fJumpPower, D3DXVECTOR3 _vShot)
{
	m_bJump = _bGrvityOn;
	m_fJumpPowerY = _fJumpPower;
	m_vShotting = _vShot;

	int i = 0;
}

void CCubeColor::Update_Color()
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
		if(abs(m_vColorStart.x - m_vColorEnd.x) < 1.f)
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

void CCubeColor::Update_Scale()
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

void CCubeColor::Update_Roation()
{
}

void CCubeColor::Save()
{
	m_vSaveScaleStart = m_vScaleStart;
	m_vSaveScaleEnd = m_vScaleEnd;

	m_vSaveColorStart = m_vColorStart;
	m_vSaveColorEnd = m_vColorEnd;

	e_SaveAxis = e_Axis;
	m_fSaveRotation = m_fRotation;

	m_bSaveJump = m_bJump;

	m_fSaveJumpPowerY = m_fJumpPowerY;
	m_vSaveShotting = m_vShotting;
	m_fSaveLifeTime = m_fLifeTime;

}

void CCubeColor::Jumping()
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
		m_fJumpTime += m_fAccel * 0.01f; //0.02f; // 타이머 0.02f
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
		m_fJumpTime += m_fAccel * 0.005f; // 타이머 0.005

		m_pTransformCom->Set_Position(m_vJumpingPoint);
	}
}
