#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit CRenderTargetMgr(void);
	virtual ~CRenderTargetMgr(void);

public:
	HRESULT		Ready_RenderTarget(const _tchar* pTargetTag, LPDIRECT3DDEVICE9 pGraphpicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);

	HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT		Begin_MRT(const _tchar* pMRTTag);
	HRESULT		End_MRT(const _tchar* pMRTTag);

	HRESULT			Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void			Render_DebugBuffer(const _tchar* pMRTTag);
	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable);
	void			SetUp_OnRegister(_int iRegisterIndex, const _tchar* pTargetTag);

private:
	CRenderTarget*	Find_RenderTarget(const _tchar* pTargetTag);
	list<CRenderTarget*>*	Find_MRT(const _tchar* pMRTTag);

private:
	map<_wstr, CRenderTarget*>		m_mapRenderTarget;

	map<_wstr, list<CRenderTarget*>>	m_mapMRT;

public:
	virtual void Free(void);

};

END
#endif // RenderTargetMgr_h__
