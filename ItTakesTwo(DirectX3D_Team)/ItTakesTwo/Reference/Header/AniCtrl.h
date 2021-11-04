#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER	Get_AniCtrl() { return m_pAniCtrl; }
	_uint						Get_CurrentTrack() { return m_iCurrentTrack; }
	_double						Get_Period() { return m_dPeriod; }
	_float						Get_AccTime() { return m_fAccTime; }
	_double						Get_CurrentTime();
	_bool						Is_AnimationSetEnd(_float minusTime, _bool bPosReset = false);

public:
	HRESULT		Ready_AniCtrl(void);
	void		Set_Animationset(const _uint& iIndex, _float fSpeed = 1.0f, _float fSyncTime = 0.25f, _float fDelay = 0.0f);
	void		Play_Animationset(const _float& fTimeDelta);

private:
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl;

	_uint						m_iCurrentTrack;
	_uint						m_iNewTrack;

	_float						m_fAccTime;
	_uint						m_iOldAniIdx;
	_double						m_dPeriod;

	_float						m_iAniDelay;


public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free(void);
};
END
#endif // AniCtrl_h__
