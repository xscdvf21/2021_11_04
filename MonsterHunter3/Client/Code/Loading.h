#ifndef Loading_h__
#define Loading_h__

#include "define.h"
#include "Base.h"

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const { return m_eLoadingID; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }
	const _tchar*		Get_String(void) const { return m_szString; }

public:
	HRESULT		Ready_Loading(LOADINGID eLoadingID);
	_uint		Loading_ForStage(void);

public:
	static unsigned int __stdcall Thread_Main(void* pArg);

private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eLoadingID;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	_tchar				m_szString[256];

	LPD3DXMESH			m_pMesh;

public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID);
private:
	virtual void Free(void);
};

#endif // Loading_h__
