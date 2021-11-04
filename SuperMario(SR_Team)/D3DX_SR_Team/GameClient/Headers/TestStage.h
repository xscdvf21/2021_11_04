#pragma once
#ifndef __TESTSTAGE_H__

#include "Scene.h"

#include "Loading.h"
#include "TestCamera.h"
#include "MainCamera.h"
#include "BattlePlate.h"
#include "Terrain.h"
#include "Stage02.h"
#include "BattleStageA.h"

USING(Engine)
class CTestStage final : public CScene
{
private:
	explicit CTestStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTestStage() = default;

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float fDeltaTime) override;
	virtual _uint LateUpdate_Scene(_float fDeltaTime) override;

#pragma region Clone을 위한 함수들
private:
	HRESULT Add_Player_Layer(const wstring& LayerTag);
	HRESULT Add_Camera_Layer(const wstring& LayerTag);
	HRESULT Add_Monster_Layer(const wstring& LayerTag = L"");
	HRESULT Add_MainCamera_Layer(const wstring& LayerTag);

	//HRESULT Add_SupportCamera_Layer(const wstring& LayerTag);

	HRESULT Add_Skybox_Layer(const wstring& LayerTag);
	HRESULT Add_UI_Layer(const wstring& LayerTag);
	HRESULT Add_UIFont_Layer(const wstring& LayerTag);

	HRESULT Add_Terrain_Layer(const wstring& LayerTag);
	HRESULT Add_Effect_Layer(const wstring & LayerTag);


	HRESULT Add_GressTree_Layer(const wstring& LayerTag);

#pragma endregion

public:
	static CTestStage* Create(LPDIRECT3DDEVICE9 pDevice);
	static CTestStage* Swap(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	class CLoading* m_pLoading = nullptr;
};


#define __TESTSTAGE_H__
#endif // !__TESTSTAGE_H__
