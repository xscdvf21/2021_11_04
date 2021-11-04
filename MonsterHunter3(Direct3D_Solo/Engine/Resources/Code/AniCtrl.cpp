#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIdx(999)
	, m_dPeriod(0.0)
{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIdx(rhs.m_iOldAniIdx)
	, m_dPeriod(0.0)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// ���� �� ���� ��ü���� �����Ǵ� �ִϸ��̼� ���� ���� 
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// ���� ������ �ִϸ��̼� ���� �ִ� ����(�밳�� 1���ڿ� ���� ����)
											rhs.m_pAniCtrl->GetMaxNumTracks(),				// ���� ������ Ʈ���� �ִ� ����
											rhs.m_pAniCtrl->GetMaxNumEvents(),				// �޽� �ȿ� ������ִ� ��Ư�� ȿ��, �츮�� ��� ����
											&m_pAniCtrl);
}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AniCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_Animationset(const _uint& iIndex)
{
	if (m_iOldAniIdx == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	m_dPeriod = pAS->GetPeriod();	// ���� �ִϸ��̼� ���� ���� ��ü ��� �ð� ��ȯ
	
	//m_pAniCtrl->GetAnimationSetByName();

	// 0��° Ʈ���� ����ϰ����ϴ� �ִϸ��̼� ���� ������
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// ������� �ʰ� �ִ� �̺�Ʈ ������ ������ ���� ������ �ȵǴ� ��찡 �߻��ؼ� ȣ��
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// ������ Ʈ���� ��� ������ �����ϴ� �Լ�, 3���� : �������� Ʈ���� ��� �Ǵ� ������ ������ ����
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);
	
	// Ʈ���� �����Ǵ� �ð����� ���� �ִϸ��̼� ���� � �ӵ��� �������� ����(�ӵ��� ��� ���� ���� 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// Ʈ���� �����Ǵ� �ð����� ���� �ִϸ��̼� ���� � ����ġ�� ���� �� ��(�ӵ��� ��� ���� ���� 1)
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	
	// Ʈ�� Ȱ��ȭ�� ������ �Ǵ��ϴ� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	// Ʈ���� �����ϴ� �ð����� ���� �ִϸ��̼� ���� � �ӵ��� �������� ����(�ӵ��� ��� ���� ���� 1)
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	// Ʈ���� �����ϴ� �ð����� ���� �ִϸ��̼� ���� � ����ġ�� ���� �� ��(�ӵ��� ��� ���� ���� 1)
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniCtrl->ResetTime(); // �ִϸ��̼��� ����Ǵ� �ð��� �ٽ� ����(advanced�Լ� ȣ�� �� ���������� �����Ǵ� �ð� ����)
	m_fAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;
	
	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animationset(const _float& fTimeDelta)
{
	// ���� �ִϸ��̼��� �����Ű�� �Լ�(fTimeDelta�ʹ� ������ AdvanceTime�Լ� ȣ�� �� ���������� �����ϴ� �ð� ���� ���� ����)
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);// 2���� : �ִϸ��̼� ���ۿ� ���� ����Ʈ�� ���� ó���� ����ϴ� ��ü�� �ּ�(��Դ� ������� �ʰ� ���� ���� ���)

	m_fAccTime += fTimeDelta;

}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*		pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*		pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

Engine::_bool Engine::CAniCtrl::Is_AnimationSetEnd(void)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if (TrackInfo.Position >= m_dPeriod - 0.1)
		return true;

	return false;
}

_bool CAniCtrl::Is_AnimationSetNow(void)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if (TrackInfo.Position >= m_dPeriod - 0.1)
		return true;


	return false;
}

