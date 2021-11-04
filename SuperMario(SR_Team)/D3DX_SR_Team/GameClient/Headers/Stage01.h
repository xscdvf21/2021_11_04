#pragma once
#ifndef __STAGE01_H__

#include "Scene.h"

#include "Loading.h"
#include "TestCamera.h"
#include "MainCamera.h"
#include "BattlePlate.h"
#include "Terrain.h"
#include "Stage02.h"
#include "BattleStageA.h"
#include "MiddleBossA.h"

#include "Effecter.h"

USING(Engine)
class CStage01 final : public CScene
{
private :
	explicit CStage01(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage01() = default;

public :
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float fDeltaTime) override;
	virtual _uint LateUpdate_Scene(_float fDeltaTime) override;

#pragma region Clone을 위한 함수들
private:
	HRESULT Add_Player_Layer(const wstring& LayerTag);
	HRESULT Add_Camera_Layer(const wstring& LayerTag);
	HRESULT Add_Monster_Layer(const wstring& LayerTag);
	HRESULT Add_MiddleBossA(const wstring& LayerTag);
	HRESULT Add_MainCamera_Layer(const wstring& LayerTag);
	HRESULT Add_Skybox_Layer(const wstring& LayerTag);
	HRESULT Add_UI_Layer(const wstring& LayerTag);
	HRESULT Add_UIFont_Layer(const wstring& LayerTag);
	HRESULT Add_Terrain_Layer(const wstring & LayerTag);

	HRESULT Add_Effect_Layer_Han(const wstring & LayerTag);

	HRESULT ADD_Camera_Layer_Number2(const wstring& LayerTage);

#pragma endregion

public :
	static CStage01* Create(LPDIRECT3DDEVICE9 pDevice);
	static CStage01* Swap(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private:
	class CLoading*		m_pLoading = nullptr;
	NOWINFO             m_tNowInfo;

};


#define __STAGE01_H__
#endif // !__STAGE01_H__
