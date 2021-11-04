#pragma once
#include "Scene.h"

#include "TestCamera.h"
#include "MainCamera.h"
#include "BattlePlate.h"
#include "Terrain.h"
#include "Loading.h"


#ifndef _STAGE02_H__
#define _STAGE02_H__



USING(Engine)
class CStage02 :
	public CScene
{
private:
	explicit CStage02(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage02() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float fDeltaTime) override;
	virtual _uint LateUpdate_Scene(_float fDeltaTime) override;

private:
	HRESULT Add_Player_Layer(const wstring& LayerTag);
	HRESULT Add_Camera_Layer(const wstring& LayerTag);
	HRESULT Add_MainCamera_Layer(const wstring& LayerTag);
	HRESULT Add_Terrain_Layer(const wstring & LayerTag);

public:
	static CStage02* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;



};

#endif // !_STAGE02_H__
