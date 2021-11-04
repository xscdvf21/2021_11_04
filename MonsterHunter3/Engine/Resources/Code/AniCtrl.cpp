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
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),	// 복제 시 원본 객체에서 제공되는 애니메이션 셋의 개수 
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(),		// 구동 가능한 애니메이션 셋의 최대 개수(대개는 1인자와 값이 같음)
											rhs.m_pAniCtrl->GetMaxNumTracks(),				// 구동 가능한 트랙의 최대 개수
											rhs.m_pAniCtrl->GetMaxNumEvents(),				// 메쉬 안에 적용되있는 독특한 효과, 우리는 사용 못함
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

	m_dPeriod = pAS->GetPeriod();	// 현재 애니메이션 셋이 지닌 전체 재생 시간 반환
	
	//m_pAniCtrl->GetAnimationSetByName();

	// 0번째 트랙에 재생하고자하는 애니메이션 셋을 설정함
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 사용하지 않고 있는 이벤트 정보들 때문에 선형 보간이 안되는 경우가 발생해서 호출
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 지정한 트랙의 사용 유무를 결정하는 함수, 3인자 : 언제부터 트랙을 사용 또는 해제할 것인지 결정
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);
	
	// 트랙이 해제되는 시간동안 현재 애니메이션 셋은 어떤 속도로 움직일지 결정(속도의 상수 값은 각자 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 트랙이 해제되는 시간동안 현재 애니메이션 셋은 어떤 가중치를 갖게 할 지(속도의 상수 값은 각자 1)
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	
	// 트랙 활성화의 유무를 판단하는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	// 트랙이 시작하는 시간동안 현재 애니메이션 셋은 어떤 속도로 움직일지 결정(속도의 상수 값은 각자 1)
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	// 트랙이 시작하는 시간동안 현재 애니메이션 셋은 어떤 가중치를 갖게 할 지(속도의 상수 값은 각자 1)
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniCtrl->ResetTime(); // 애니메이션이 재생되던 시간을 다시 세팅(advanced함수 호출 시 내부적으로 누적되던 시간 리셋)
	m_fAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;
	
	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animationset(const _float& fTimeDelta)
{
	// 실제 애니메이션을 재생시키는 함수(fTimeDelta와는 별개로 AdvanceTime함수 호출 시 내부적으로 증가하는 시간 값이 따로 있음)
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);// 2인자 : 애니메이션 동작에 따른 이펙트에 대한 처리를 담당하는 객체의 주소(대게는 사용하지 않고 직접 만들어서 사용)

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

