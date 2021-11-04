#ifndef Loading_h__
#define Loading_h__


#include "Base.h"

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoading();

public:
	LOADINGID			Get_LoadingID() const { return m_eLoadingID; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() const { return m_bFinish; }
	const _wstr			Get_String() const { return m_szString; }

public:
	HRESULT				Ready_Loading(LOADINGID eLoadingID);
	_uint				Loading_Static();
	_uint				Loading_Menu();
	_uint				Loding_ChapterSelect();
	_uint				Loding_PlayerSelect();

	_uint				Loading_VacuumField();
	_uint				Loading_VacuumField2();
	_uint				Loading_VacuumField3();
	_uint				Loading_VacuumBoss();

	_uint				Loading_ChessField();
	_uint				Loading_ChessBoss();

	_uint				Loading_SpaceField();
	_uint				Loading_SpaceField2();
	_uint				Loading_SpaceField3();
	_uint				Loading_SpaceBoss();

	_uint				Loading_TestField();
	_uint				Loading_Ending();

public:
	static unsigned int __stdcall Thread_Main(void* pArg);

private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eLoadingID;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;
	_tchar				m_szString[256];

public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID);

private:
	virtual void Free();
};

#endif // Loading_h__
