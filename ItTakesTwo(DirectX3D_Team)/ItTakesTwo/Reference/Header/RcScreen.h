#ifndef RcScreen_h__
#define RcScreen_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcScreen : public CVIBuffer
{
private:
	explicit CRcScreen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcScreen(const CRcScreen& rhs);
	virtual ~CRcScreen(void);

public:
	virtual HRESULT Ready_Buffer(void) override;
	HRESULT Ready_Buffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY);
	virtual void Render_Buffer(void) override;

public:
	static CRcScreen*		Create(LPDIRECT3DDEVICE9 pGraphicDev,const _bool& bScreen =false ,const _float& fX=0, const _float& fY=0, const _float& fSizeX=1920, const _float& fSizeY=1080);
	virtual CComponent*	Clone(void* pArg = nullptr) override;

	_mat Get_Pos();
	_mat	Get_Scale();
	
	_vec2 Update_Buffer(const _float& fDeltaTime);
private:
	virtual void		Free(void);
	_bool				m_bChange = false;
	_float				m_fChangeSpeed =1.f;

	//fX fY fSizeX fSizeY
	_vec4				m_vTargetSize{ 0.f,0.f,1920.f,1080.f };
	_vec4				m_vCurrentSize{ 0.f,0.f,1920.f,1080.f };
	_vec4				m_vMoveSize;
	_bool				m_bOrth = false;
};


END
#endif // RcScreen_h__
