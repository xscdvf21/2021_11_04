#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
	: m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vRevAngle(0.f, 0.f, 0.f)
	, m_vParent(0.0f, 0.0f, 0.0f)
	, m_iMode(MODE_EULER)
{
	D3DXMatrixIdentity(&m_matRot);
}

Engine::CTransform::CTransform(const CTransform& rhs)
	: m_vAngle(rhs.m_vAngle)
	, m_vScale(rhs.m_vScale)
	, m_vRevAngle(rhs.m_vRevAngle)
	, m_vParent(rhs.m_vParent)
	,m_iMode(MODE_EULER)
{
	for (_uint i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
	D3DXMatrixIdentity(&m_matRot);
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
	//if (isnan(m_vInfo[INFO_POS].x))
	//	int iA = 0;
	//if (isnan(m_vInfo[INFO_POS].y))
	//	int iA = 0;
	//if (isnan(m_vInfo[INFO_POS].z))
	//	int iA = 0;
	/*D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	//크기변환

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	//회전 변환
	_matrix				matRot[ROT_END];

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

	return 0;*/

	_matrix matScale, matRotate, matTrans, matRev, matQuat, matRevQuat, matParent;
	_qtnian qQuat;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixIdentity(&matQuat);

	if (m_qQuat)
	{
		D3DXQUATERNION qX, qY, qZ;
		switch (m_iMode)
		{
		case MODE_EULER:
			D3DXQuaternionRotationYawPitchRoll(&m_qQuat, m_vAngle.y, m_vAngle.x, m_vAngle.z);
			D3DXMatrixRotationQuaternion(&matQuat, &(m_qQuat));
			//_mat matAddRot;

			break;
		case MODE_AXIS:
			D3DXQuaternionRotationAxis(&qX, &_vec3{ 1.f,0.f,0.f }, m_vAngle.x);
			D3DXQuaternionRotationAxis(&qY, &_vec3{ 0.f,1.f,0.f }, m_vAngle.y);
			D3DXQuaternionRotationAxis(&qZ, &_vec3{ 0.f,0.f,1.f }, m_vAngle.z);
			D3DXMatrixRotationQuaternion(&matQuat, &(qX*qY*qZ));
			break;
		case MODE_FREEANGLE:

			//D3DXVec3TransformNormal(&m_vUp, &vUP, &m_matRot);
			//D3DXVec3Normalize(&m_vUp, &m_vUp);
			//D3DXVec3TransformNormal(&m_vRight, &vRIGHT, &m_matRot);
			//D3DXVec3Normalize(&m_vRight, &m_vRight);
			//D3DXVec3TransformNormal(&m_vLook, &vLOOK, &m_matRot);
			//D3DXVec3Normalize(&m_vLook, &m_vLook);

			//D3DXQuaternionRotationAxis(&qX, &m_vRight, m_vAngle.x);
			//D3DXQuaternionRotationAxis(&qY, &m_vUp, m_vAngle.y);
			//D3DXQuaternionRotationAxis(&qZ, &m_vLook, m_vAngle.z);

			//D3DXMatrixRotationQuaternion(&matQuat, &(qX*qY*qZ));
			
			D3DXMatrixRotationAxis(&matRotate, (_vec3*)&m_matRot._31, D3DXToRadian(-90.f));

			m_vRight = (_vec3)&m_matRot._11;
			m_vUp = (_vec3)&m_matRot._21;
			m_vLook = (_vec3)&m_matRot._31;

			D3DXQuaternionRotationAxis(&qX, (_vec3*)&m_matRot._11, m_vAngle.x);
			D3DXQuaternionRotationAxis(&qY, (_vec3*)&m_matRot._21, m_vAngle.y);
			D3DXQuaternionRotationAxis(&qZ, (_vec3*)&m_matRot._31, m_vAngle.z);
			D3DXMatrixRotationQuaternion(&matQuat, &(qX*qY*qZ));


			matQuat = m_matRot*matRotate*matQuat;
			break;
		default:
			break;
		}


	}

	if (m_RevqQuat)
	{
		D3DXQuaternionRotationYawPitchRoll(&m_RevqQuat, m_vRevAngle.y, m_vRevAngle.x, m_vRevAngle.z);
		D3DXMatrixRotationQuaternion(&matRevQuat, &m_RevqQuat);
	}
	
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);
	D3DXMatrixTranslation(&matParent, m_vParent.x, m_vParent.y, m_vParent.z);


	m_matWorld = matScale * matQuat * matTrans * matRevQuat * matParent;

	return 0;
}

CTransform* Engine::CTransform::Create(void)
{
	CTransform*	pInstance = new CTransform();

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CTransform::Clone(void* pArg)
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

void CTransform::Get_WorldMatrix(_matrix * pMatrix) const
{
	*pMatrix = m_matWorld;
}

void Engine::CTransform::Set_WorldMatrix(const PxTransform& pMatrix)
{
	PxMat44 pxMat =PxMat44(pMatrix);
	pxMat.scale(PxVec4(0.01f, 0.01f, 0.01f, 1.f));
	memcpy(&m_matWorld, &pxMat, sizeof(_mat));
}

void Engine::CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
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
	vDir.y = 0.f;
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix			matRot = *Compute_LookAtTarget(pTargetPos);

	_matrix			matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

void CTransform::Chase_TargetY(const _vec3 * pTargetPos, const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix			matRot = *Compute_LookAtTarget(pTargetPos);

	_matrix			matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}
