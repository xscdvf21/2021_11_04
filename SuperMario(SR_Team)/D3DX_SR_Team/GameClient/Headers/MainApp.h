#pragma once

#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Base.h"
#include <Management.h>

#include "Logo.h"


#include "TestCamera.h"
#include "MainCamera.h"
#include "BattleCamera.h"
#include "Player.h"
#include "Monster.h"
#include "BattleUI.h"
#include "StageUI.h"

#include "MiddleBossA.h"

#include "BattlePlate.h"
#include "MonsterA.h"
#include "Skybox.h"
#include "Terrain.h"


#include "Effecter.h"
#include "EffectParticle.h"

#include "Teleporter.h"
#include "TriggerBox.h"
#include "FakeTerrain.h"
#include "Gress.h"
#include "Tree.h"


USING(Engine)
//싱글톤 아님
class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Ready_MainApp();
	_uint Update_MainApp();

private:
	HRESULT Setup_DefaultSetting();

private: //최상위 프로토타입 Call
	HRESULT HighLevel_Prototype_Caller();

private://중간 프로토타입 Call
	HRESULT GameObject_Prototype_Caller(CManagement * _pManagement);
	HRESULT Component_Prototype_Caller(CManagement * _pManagement);
	HRESULT Texture_Prototype_Caller(CManagement * _pManagement);
	HRESULT Status_Comp_Prototype(CManagement * _pManagement);
	HRESULT Etc_Prototype_Caller(CManagement * _pManagement);


#pragma region 하위 프로토타입

private://GameObject 관련  Prototype
	HRESULT Player_Prototype(CManagement * _pManagement);
	HRESULT Monster_Prototype(CManagement * _pManagement);
	HRESULT BattlePlate_Prototype(CManagement * _pManagement);
	HRESULT Camera_Prototype(CManagement * _pManagement);
	HRESULT UI_Prototype(CManagement * _pManagement);
	HRESULT BATTLE_UI_Prototype(CManagement * _pManagement);
	HRESULT Terrain_Prototype(CManagement * _pManagement);
	HRESULT SkyBox_Prototype(CManagement * _pManagement);

	HRESULT Effecter_Prototype(CManagement * _pManagerment);
	HRESULT Particle_Prototype(CManagement * _pManagerment);

	HRESULT MiddleBossABullet_Prototype(CManagement* _pManagement);
	HRESULT Item_Prototype(CManagement* _pManagement);

	HRESULT TriggerBox_Prototype(CManagement* _pManagement);
	HRESULT Teleporter_Prototype(CManagement* _pManagement);
	HRESULT Gress_Prototype(CManagement* _pManagement);
	HRESULT Tree_Prototype(CManagement* _pManagement);
	HRESULT FakeTerrain_Prototype(CManagement* _pManagement);


private://Component 관련 Prototype
	HRESULT Transform_Prototype(CManagement * _pManagement);
	HRESULT VIBuffer_Prototype(CManagement * _pManagement);
	HRESULT Font_Prototype(CManagement * _pManagement);

private://Texture 관련 Prototype
	HRESULT Player_Texture_Prototype(CManagement * _pManagement);
	HRESULT Player_2DTexture_Prototype(CManagement* _pManagement);

	HRESULT Monster_Texture_Prototype(CManagement * _pManagement);
	HRESULT BattlePlate_Texture_Prototype(CManagement * _pManagement);
	HRESULT UI_Texture_Prototype(CManagement * _pManagement);
	HRESULT Terrain_Texture_Prototype(CManagement * _pManagement);
	HRESULT SkyBox_Texture_Prototype(CManagement * _pManagement);
	HRESULT Item_Texture_Prototype(CManagement* _pManagement);
	HRESULT BackGround_Texture_Prototype(CManagement* _pManagement);


	//혹시나 싶어서
	HRESULT CLEAR_PROTOTYPE(CManagement* _pManagement);

	HRESULT Gress_Texture_Prototype_Han(CManagement * _pManagement);

#pragma endregion

private : //정달영
	HRESULT Monster_Texture_Prototype_JDY(CManagement * _pManagement);

public:
	static CMainApp* Create();
	virtual void Free() override;

private:
	CManagement*	m_pManagement = nullptr;

	_Device			m_pDevice = nullptr;
	LPD3DXFONT		m_pFont = nullptr;
	LPD3DXSPRITE	m_pSprite = nullptr;


};


#endif // !__MAINAPP_H__