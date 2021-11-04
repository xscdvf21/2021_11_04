#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
	: m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
{
	/*ZeroMemory(m_vInfo[INFO_LOOK], sizeof(INFO::INFO_LOOK));*/
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
	{
		ZeroMemory(m_vInfo[i], sizeof(INFO));
	}

}

Engine::CTransform::CTransform(const CTransform& rhs)
	: m_vAngle(rhs.m_vAngle)
	, m_vScale(rhs.m_vScale)
{
	for (_uint i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];



		

	m_matWorld = rhs.m_matWorld;
}

Engine::CTransform::~CTransform(void)
{

}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	
	return S_OK;
}

_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기변환

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	// 회전 변환
	_matrix				matRot[ROT_END];

	for (_uint i = 0; i < ROT_END; ++i)
	{
		D3DXMatrixIdentity(&matRot[i]);
	}

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));


	return 0;
}

CTransform* Engine::CTransform::Create(void)
{
	CTransform*	pInstance = new CTransform();

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}

void Engine::CTransform::Free(void)
{

}

void Engine::CTransform::Get_Info(INFO eType, _vec3* pInfo)
{
	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
}

const _matrix* Engine::CTransform::Get_WorldMatrix(void)
{
	return &m_matWorld;
}

void Engine::CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
}


void CTransform::Move_TargetPos(const _vec3 * pTargetPos, const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3 vDir = *pTargetPos - m_vInfo[INFO_POS];
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
}

void Engine::CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

const _matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
	_vec3		vAxis, vUp;
	_matrix		matRot;

	return D3DXMatrixRotationAxis(&matRot, 
								D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir), 
								acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
									D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void Engine::CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix			matRot = *Compute_LookAtTarget(pTargetPos);

	_matrix			matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

void Engine::CTransform::Get_WorldMatrix(_matrix* pMatrix) const
{
	*pMatrix = m_matWorld;
}


void Engine::CTransform::Get_NRotWorldMatrix(_matrix* pMatrix) const
{
	*pMatrix = m_matNRotWorld;
}

void CTransform::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vInfo[INFO_POS].x = fX;
	m_vInfo[INFO_POS].y = fY;
	m_vInfo[INFO_POS].z = fZ;
}



void CTransform::Set_Pos(const _vec3 & vPos)
{
	m_vInfo[INFO_POS] = vPos;
}

void CTransform::Set_Scale(const _vec3 & vScale)
{
	m_vScale = vScale;
}

void CTransform::Set_Scale(const _float & fScaleX, _float & fScaleY, _float & fScaleZ)
{
	m_vScale.x = fScaleX;
	m_vScale.y = fScaleY;
	m_vScale.z = fScaleZ;
}

void CTransform::Set_Rotation(EAxis eAxis, float fRot)
{
	switch (eAxis)
	{
	case	EAxis::X:
		m_vAngle.x = fRot;
		break;

	case	EAxis::Y:
		m_vAngle.y = fRot;
		break;

	case	EAxis::Z:
		m_vAngle.z = fRot;
		break;
	}
}

void CTransform::Add_Rotation(EAxis eAxis, float fRot , const _float& fDelta)
{
	switch (eAxis)
	{
	case	EAxis::X:
		m_vAngle.x += fRot * fDelta;
		break;

	case	EAxis::Y:
		m_vAngle.y += fRot * fDelta;
		break;

	case	EAxis::Z:
		m_vAngle.z += fRot * fDelta;
		break;
	}
}




void CTransform::Set_ParentMatrix(const _matrix * pParent)
{
	m_matWorld *= *pParent;
}

void Engine::CTransform::Set_WorldMatrix(const _matrix * pMatrix)
{
	m_matWorld = *pMatrix;
}

