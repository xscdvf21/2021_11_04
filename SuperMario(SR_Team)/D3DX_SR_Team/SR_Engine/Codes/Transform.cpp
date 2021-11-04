#include "..\Headers\Transform.h"
USING(Engine)
CTransform::CTransform(_Device pDevice)
    : CComponent(pDevice)
{
    ZeroMemory(&m_TransformDesc, sizeof(TRANSFORM_DESC));
    m_TransformDesc.vScale = { 1.f, 1.f, 1.f };
}

CTransform::CTransform(const CTransform& other)
    : CComponent(other)
    , m_TransformDesc(other.m_TransformDesc)
{
}

HRESULT CTransform::Ready_Component_Prototype()
{
    if (FAILED(CComponent::Ready_Component_Prototype()))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CTransform::Ready_Component(void* pArg)
{
    //여기 Arg는 Transform_desc임.
    if (FAILED(CComponent::Ready_Component(pArg)))
    {
        return E_FAIL;
    }

    if (nullptr != pArg)
    {
        memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORM_DESC));
    }

    return S_OK;
}

void CTransform::Set_Position(_float3 _vPos)
{
    m_TransformDesc.vPos = _vPos;
}

void Engine::CTransform::Set_Scale(_float3 & _vScale)
{
	m_TransformDesc.vScale = _vScale;
}

void CTransform::Set_AxisScale(EAxis _eAxis, _float _fValue)
{
	/* 오브젝트가 바라보는 방향을 기준으로 앞뒤 움직여라 */
	_float3 vDir;

	switch (_eAxis)
	{
	case Engine::EAxis::X:
		memcpy(&vDir, &m_TransformDesc.matWorld.m[0][0], sizeof(_float3));
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= _fValue;
		memcpy(&m_TransformDesc.matWorld.m[0][0], &vDir, sizeof(_float3));
		break;
	case Engine::EAxis::Y:
		memcpy(&vDir, &m_TransformDesc.matWorld.m[1][0], sizeof(_float3));
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= _fValue;
		memcpy(&m_TransformDesc.matWorld.m[1][0], &vDir, sizeof(_float3));
		break;
	case Engine::EAxis::Z:
		memcpy(&vDir, &m_TransformDesc.matWorld.m[2][0], sizeof(_float3));
		D3DXVec3Normalize(&vDir, &vDir);
		vDir *= _fValue;
		memcpy(&m_TransformDesc.matWorld.m[2][0], &vDir, sizeof(_float3));
		break;
	default:
		break;
	}
}

void Engine::CTransform::Set_Rotation(_float3 & _Rot)
{
	m_TransformDesc.vRotate = _float3(D3DXToRadian(_Rot.x), D3DXToRadian(_Rot.y), D3DXToRadian(_Rot.z));
}

void Engine::CTransform::Set_Rotation(EAxis eAxis, float _Rot)
{
	switch (eAxis)
	{
	case EAxis::X:
		m_TransformDesc.vRotate.x = _Rot;
		break;
	case EAxis::Y:
		m_TransformDesc.vRotate.y = _Rot;
		break;
	case EAxis::Z:
		m_TransformDesc.vRotate.z = _Rot;
		break;
	}
}

void Engine::CTransform::Set_WorldMatrix(const _float4x4 & _matWorld)
{
	m_TransformDesc.matWorld = _matWorld;
}

void Engine::CTransform::Set_XScale(bool _bInverce)
{
	_float3 vDir;


	memcpy(&vDir, &m_TransformDesc.matWorld.m[0][0], sizeof(_float3));
}

#pragma region Ready 단계에서 딱 한번만 쓰는 함수.
void Engine::CTransform::Initialize_vScale(const _float3 _vScals)
{
	m_TransformDesc.vScale = _vScals;
}

void Engine::CTransform::Initialize_vRotate(const _float3 _vRotate)
{
	m_TransformDesc.vRotate = _vRotate;
}

void Engine::CTransform::Initialize_vPos(const _float3 _vPos)
{
	m_TransformDesc.vPos = _vPos;
}

void Engine::CTransform::Initialize_matWorld(const _float4x4 _matWorld)
{
	m_TransformDesc.matWorld = _matWorld;
}

void Engine::CTransform::Initialize_fSpeed(const _float _fSpeed)
{
	m_TransformDesc.fSpeed = _fSpeed;
}

void Engine::CTransform::Initialize_fRotate(const _float _fRotate)
{
	m_TransformDesc.fRotate = _fRotate;
}

#pragma endregion

const _float3& CTransform::Get_Position() const
{
    return m_TransformDesc.vPos;
}

const _float CTransform::Get_Rotate() const
{
    return m_TransformDesc.fRotate;
}

const _mat& CTransform::Get_WorldMatrix() const
{
    return m_TransformDesc.matWorld;
}

const _float3 Engine::CTransform::Get_Scale() const
{
	return m_TransformDesc.vScale;
}

const _float & Engine::CTransform::Get_vScale(EAxis _eAxis) const
{
	switch (_eAxis)
	{
	case EAxis::X:
		return m_TransformDesc.vRotate.x;
		break;
	case EAxis::Y:
		return m_TransformDesc.vRotate.y;
		break;
	case EAxis::Z:
		return m_TransformDesc.vRotate.z;
		break;
	}
}

HRESULT CTransform::Set_Transform()
{
    return m_pDevice->SetTransform(D3DTS_WORLD, &m_TransformDesc.matWorld);
}

_uint CTransform::Update_Transform()
{

    D3DXMatrixIdentity(&m_TransformDesc.matWorld);
    _float4x4 matScale, matRotateX, matRotateY, matRotateZ, matTrans;

    D3DXMatrixScaling(&matScale, m_TransformDesc.vScale.x, m_TransformDesc.vScale.y, m_TransformDesc.vScale.z);
    D3DXMatrixRotationX(&matRotateX, m_TransformDesc.vRotate.x);
    D3DXMatrixRotationY(&matRotateY, m_TransformDesc.vRotate.y);
    D3DXMatrixRotationZ(&matRotateZ, m_TransformDesc.vRotate.z);
    D3DXMatrixTranslation(&matTrans, m_TransformDesc.vPos.x, m_TransformDesc.vPos.y, m_TransformDesc.vPos.z);

    m_TransformDesc.matWorld = matScale * matRotateX * matRotateY * matRotateZ * matTrans;


    return 0;
}

void CTransform::Go_Straight(_float fDeltaTime)
{
    /* 오브젝트가 바라보는 방향을 기준으로 앞뒤 움직여라 */
    _float3 vLook;

    /* 상태행렬의 3행은 look벡터이다 */
    memcpy(&vLook, &m_TransformDesc.matWorld.m[2][0], sizeof(_float3));
    D3DXVec3Normalize(&vLook, &vLook);

    m_TransformDesc.vPos += vLook * m_TransformDesc.fSpeed * fDeltaTime;
}


void CTransform::Go_AxisStraight(EAxis _eAxis,_float fDeltaTime)
{
	/* 오브젝트가 바라보는 방향을 기준으로 앞뒤 움직여라 */
	_float3 vDir;

	switch (_eAxis)
	{
	case Engine::EAxis::X:
		memcpy(&vDir, &m_TransformDesc.matWorld.m[0][0], sizeof(_float3));
		D3DXVec3Normalize(&vDir, &vDir);
		break;
	case Engine::EAxis::Y:
		memcpy(&vDir, &m_TransformDesc.matWorld.m[1][0], sizeof(_float3));
		D3DXVec3Normalize(&vDir, &vDir);
		break;
	case Engine::EAxis::Z:
		memcpy(&vDir, &m_TransformDesc.matWorld.m[2][0], sizeof(_float3));
		D3DXVec3Normalize(&vDir, &vDir);
		break;
	default:
		break;
	}
	m_TransformDesc.vPos += vDir * m_TransformDesc.fSpeed * fDeltaTime;
}



void CTransform::Go_Side(_float fDeltaTime)
{
    /* 오브젝트가 바라보는 방향을 기준으로 양옆 움직여라 */
    _float3 vRight;

    /* 상태행렬의 1행은 right벡터이다 */
    memcpy(&vRight, &m_TransformDesc.matWorld.m[0][0], sizeof(_float3));
    D3DXVec3Normalize(&vRight, &vRight);

    m_TransformDesc.vPos += vRight * m_TransformDesc.fSpeed * fDeltaTime;
}

void Engine::CTransform::Go_Jump(_float fDeltaTime)
{
	_float3 vUp;

	/*상태행렬의 2행 Up벡터이다.*/
	memcpy(&vUp, &m_TransformDesc.matWorld.m[1][0], sizeof(_float3));
	D3DXVec3Normalize(&vUp, &vUp);
	m_TransformDesc.vPos += vUp*  m_TransformDesc.fSpeed * fDeltaTime;
}

void CTransform::Rotate(EAxis eAxis, _float fDeltaTime)
{
    switch (eAxis)
    {
    case EAxis::X:
        m_TransformDesc.vRotate.x += m_TransformDesc.fRotate * fDeltaTime;
        break;
    case EAxis::Y:
        m_TransformDesc.vRotate.y += m_TransformDesc.fRotate * fDeltaTime;
        break;
    case EAxis::Z:
        m_TransformDesc.vRotate.z += m_TransformDesc.fRotate * fDeltaTime;
        break;
    }
}



void Engine::CTransform::AddPostion(_float3 vPos)
{
	m_TransformDesc.vPos += vPos;
}

CTransform* CTransform::Create(_Device pDevice)
{
    CTransform* pInstance = new CTransform(pDevice);
    if (FAILED(pInstance->Ready_Component_Prototype()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CTransform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
    CTransform* pClone = new CTransform(*this);
    if (FAILED(pClone->Ready_Component(pArg)))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CTransform");
        Safe_Release(pClone);
    }

    return pClone;
}

void CTransform::Free()
{
    CComponent::Free();
}
