#pragma once

#ifndef __BATTLE_STAGE_A_H__
#define __BATTLE_STAGE_A_H__

#include "Scene.h"

#include "SuperMonster.h"
#include "Monster.h"
#include "BattlePlate.h"
#include "BattleCamera.h"
#include "TestCamera.h"
#include "MonsterA.h"

#include "Stage01.h"

USING(Engine)

class CBattleStageA : public CScene
{
private :
	explicit CBattleStageA(_Device pDevice);
	virtual ~CBattleStageA() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float fDeltaTime) override;
	virtual _uint LateUpdate_Scene(_float fDeltaTime) override;

#pragma region Clone ÇÔ¼ö.

private:
	HRESULT Add_Player_Layer(const wstring& LayerTag);
	HRESULT Add_Camera_Layer(const wstring& LayerTag);
	HRESULT Add_SkyBox_Layer(const wstring& LayerTag);
	HRESULT Add_Monster_Layer(const wstring& LayerTag);
	HRESULT Add_Monster_Object_Layer(const wstring& LayerTag);
	HRESULT Add_BattlePlate_Layer(const wstring& LayerTag);
	HRESULT Add_Battle_UI(const wstring& LayerTag);

#pragma endregion

public:
	static CBattleStageA* Create(_Device pDevice);
	virtual void Free() override;

	class CBattlePlate* pBattlePlate = nullptr;
	class CBattleUI*	pBattleUI = nullptr;
};

#endif