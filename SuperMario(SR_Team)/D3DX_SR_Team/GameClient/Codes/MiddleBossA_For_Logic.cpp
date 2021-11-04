#include "framework.h"
#include "..\Headers\MiddleBossA.h"
#include "Management.h"
#include "MiddleBossA_For_Logic.h"

void CMiddleBossA::Update_Sprite(_float fDeltaTime)
{
	if (m_ePreState != m_eState)
	{
		switch (m_eState)
		{
		case STATE::STATE_IDLE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 0;
			m_tFrame.iSpriteNow = 0;//스프라이트의 어떤 종류냐.

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 5.f;
			m_tFrame.fFrameSpeed = 1.f;

			
			m_tFrame.fEventStart = 0.f;
			m_tFrame.fEventNow = m_tFrame.fEventStart;
			m_tFrame.fEventMax = 10.f;
			m_tFrame.fEventSpeed = 1.f;
			

			m_ePreState = m_eState;
			return;

		case STATE::STATE_MOVE_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 7;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 5.f;
			m_tFrame.fFrameSpeed = 10.f;

			m_ePreState = m_eState;
			return;
		case STATE::STATE_ATTACK_FRONT:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 0;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 5.f;
			m_tFrame.fFrameSpeed = 1.f;

			m_tFrame.fEventStart = 0.f;
			m_tFrame.fEventNow = m_tFrame.fEventStart;
			m_tFrame.fEventMax = 3.f;
			m_tFrame.fEventSpeed = 1.f;

			m_ePreState = m_eState;
			return;
		case STATE::STATE_MOVE_BACK:
			m_tFrame.iSpriteStart = 0;
			m_tFrame.iSpriteEnd = 7;
			m_tFrame.iSpriteNow = 0;

			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameMax = 5.f;
			m_tFrame.fFrameSpeed = 10.f;

			m_ePreState = m_eState;
			return;
		}
	}
}

//플레이어 추적 코드.
void CMiddleBossA::Player_Check(CManagement* _pManagement, _float fDeltaTime)
{
	m_pTargetTransform = (CTransform*)_pManagement->Get_Component((_uint)ESceneType::Stage, L"Layer_Player", L"Com_Transform");

	_float3 vTargetPos = m_pTargetTransform->Get_Position();
	_float3 vPos = m_pTransformCom->Get_Position();

	_float fDis = D3DXVec3Length(&(vPos - vTargetPos));
	
	//좌 우 회전 먼저.
	if (1.f < fDis) // 플레이어가 너무 가까이에 있어서 몬스터가 떨리는 것을 방지하는 조건.
	{
		CMiddleBossA::Monster_Sprite_Dir(vTargetPos, vPos, fDeltaTime);
	}
	//플레이어 따라가기.
	if (10.f > fDis && 3.f < fDis)// 플레이어가 10 이라는 범위 안에 있다면.
	{
		_float3 vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->AddPostion(vDir* fDeltaTime);

		m_eState = STATE::STATE_MOVE_FRONT;
		return;
	}
	else
	{
		m_eState = STATE::STATE_IDLE_FRONT;
		return;
	}
	m_eState = STATE::STATE_IDLE_FRONT;
}

bool CMiddleBossA::Monster_Sprite_Dir(_float3 _vTargetPos, _float3 _vPos, _float fDeltaTime)
{
	m_pTransformCom->Set_Rotation(_float3(0.f, m_fRotY, 0.f));

	if (-179.5f > m_fRotY)
	{
		m_fRotY = -180.f;
	}
	else if (179.5f < m_fRotY)
	{
		m_fRotY = 180.f;
	}

	if (_vTargetPos.x > _vPos.x)
	{
		m_fRotY = 0.f;
	}
	else if (_vTargetPos.x < _vPos.x)
	{
		m_fRotY = 180.f;
	}

	return false;
}

bool CMiddleBossA::Attack_TimingChecker(_float _fDeltaTime)
{
	if (STATE::STATE_IDLE_FRONT == m_eState)
	{
		m_tFrame.fEventNow+= m_tFrame.fEventMax* _fDeltaTime*m_tFrame.fEventSpeed;
		if (m_tFrame.fEventMax <= m_tFrame.fEventNow)
		{
			m_tFrame.fEventNow = m_tFrame.fEventStart;
			m_eState = STATE::STATE_ATTACK_FRONT;
		}
	}
	return true;
}

bool CMiddleBossA::ImAttacking(CManagement* _pManagement, _float fDeltaTime)
{
	m_fAccel += m_fDeccel*(-fDeltaTime*10.f);
	m_fRotY += fDeltaTime*(m_fAccel*10.f);
	m_pTransformCom->Set_Rotation(_float3(0.f, m_fRotY, 0.f)); 

	CGameObject* pObj = nullptr;
	/*
#pragma region 회전샷.
	m_fBulletTime += 1.f * fDeltaTime* (m_fAccel * 0.5f);
	if (1.f <= m_fBulletTime)
	{
		m_fBulletRot += 30.f * fDeltaTime;
		pObj = _pManagement->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_MiddleBossABullet",
			(_uint)ESceneType::Stage,
			L"Layer_MiddleBossABullet");

		_float3 vShotDir;
		memcpy(&vShotDir, &m_pTransformCom->Get_WorldMatrix().m[0][0], sizeof(_float3));

		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_Dir(vShotDir);
		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_BulletPos(m_pTransformCom->Get_Position());
		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_BulletType(BULLETTYPE::TYPE_A);

		m_fBulletTime = 0.f;
	}
#pragma endregion

#pragma region 던지기
	m_fThrowTime += 1.f * fDeltaTime;
	if(1.f<m_fThrowTime)
	{
		pObj = _pManagement->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_MiddleBossABullet",
			(_uint)ESceneType::Stage,
			L"Layer_MiddleBossABullet");

		CTransform* pTarget = (CTransform*)_pManagement->Get_Component((_uint)ESceneType::Stage, L"Layer_Player", L"Com_Transform");

		_float3 vPos = m_pTransformCom->Get_Position();
		_float3 vTarget = pTarget->Get_Position();

		_float3 vDir = (vTarget - vPos);

		float fLength = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_BulletPos(m_pTransformCom->Get_Position());
		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_Dir(vDir);
		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_Accel(fLength);
		dynamic_cast<CMiddleBossABullet*>(pObj)->Set_BulletType(BULLETTYPE::TYPE_B);

		m_fThrowTime = 0.f;
	}
#pragma endregion
*/
	m_fShotGunTime += 10.f * fDeltaTime;
	if (10.f <= m_fShotGunTime)
	{
		m_fShotGunTime = 0.f;
		m_fBulletRot = -30;
		for (int i = 0; i < 3; ++i)
		{
			pObj = _pManagement->Clone_GameObject_For_Load(
				(_uint)ESceneType::Static,
				L"GameObject_MiddleBossABullet",
				(_uint)ESceneType::Stage,
				L"Layer_MiddleBossABullet");

			CTransform* pTarget = (CTransform*)_pManagement->Get_Component((_uint)ESceneType::Stage, L"Layer_Player", L"Com_Transform");

			_float3 fDirection = pTarget->Get_Position()- m_pTransformCom->Get_Position();

			_float4x4 matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fBulletRot));
			D3DXVec3TransformNormal(&fDirection, &fDirection, &matRot);

			dynamic_cast<CMiddleBossABullet*>(pObj)->Set_BulletPos(m_pTransformCom->Get_Position());
			dynamic_cast<CMiddleBossABullet*>(pObj)->Set_Dir(fDirection);
			dynamic_cast<CMiddleBossABullet*>(pObj)->Set_BulletType(BULLETTYPE::TYPE_A);
			m_fBulletRot += 30.f;
		}
	}

	if (0.f >= m_fAccel)
	{
		m_eState = STATE::STATE_IDLE_FRONT;
		m_fAccel = 100.f;
		m_tFrame.fEventNow = m_tFrame.fEventStart;
		m_fBulletRot = 0.f;
		return TRUE;
	}

	return TRUE;
}

