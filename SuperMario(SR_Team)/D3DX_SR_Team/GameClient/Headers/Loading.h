#pragma once
#ifndef __LOADING_H__

#include "Base.h"
#include "Management.h"
#include "Client_Enum.h"

USING(Engine)
class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice, ESceneType eSceneID);
	virtual ~CLoading() = default;

public:
	/* Getter */
	_bool IsFinished();

public:
	HRESULT Ready_Loading();

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice, ESceneType eSceneID);
	virtual void Free() override;

public: /* Thread Main Function */
	static unsigned _stdcall LoadingByThread(void* pParam);

private:
	HRESULT Load_StageResources();
	HRESULT Load_StageTwoResources();

private:
	HRESULT GameObject_Prototype(CManagement * _pManagement, ESceneType _eStage);
	HRESULT Component_Prototype(CManagement * _pManagement, ESceneType _eStage);
	HRESULT Texture_Prototype(CManagement * _pManagement, ESceneType _eStage);


private:
	LPDIRECT3DDEVICE9 m_pDevice;

	HANDLE m_hLoadingThread;
	CRITICAL_SECTION m_CriticalSection;

	ESceneType m_eLoadingScene;
	_bool m_IsFinished = false;
};

#define __LOADING_H__
#endif