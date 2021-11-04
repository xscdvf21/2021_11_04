#pragma once
#ifndef __SCENE_MANAGER_H__

#include "Base.h"

BEGIN(Engine)
class CScene_Manager final : public CBase
{
	DECLARE_SINGLETON(CScene_Manager)

private:
	CScene_Manager();
	virtual ~CScene_Manager() = default;

public:
	HRESULT Setup_CurrentScene(_int iSceneIndex, class CScene* pCurrentScene);
	HRESULT SetUp_ScnenSwap(_int iSceneIndex, CScene * pCurrentScene);

public:
	_uint Update_Scene(_float fDeltaTime);
	_uint LateUpdate_Scene(_float fDeltaTime);

public:
	virtual void Free() override;

public://Get
	const _int Get_DuplicateChecker() const { return m_iPreSceneIndex; }

private:
	class CScene*	m_pCurrentScene = nullptr;

	_int m_iCurrentSceneIndex = -1;
	_int m_iPreSceneIndex = -1;
};
END

#define __SCENE_MANAGER_H__
#endif