#pragma once
#ifndef __LOGO_H__

#include "Scene.h"

#include "Loading.h"
#include "Stage01.h"
#include "BattleStageA.h"
#include "TestStage.h"

USING(Engine)
class CLogo final : public CScene
{
private :
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo() = default;

public :
	virtual HRESULT Ready_Scene() override;
	virtual _uint Update_Scene(_float fDeltaTime) override;
	virtual _uint LateUpdate_Scene(_float fDeltaTime) override;
	
public :
	HRESULT Add_OpeningBG_Layer(const wstring& LayerTag);

	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;

private :
	class CLoading*		m_pLoading = nullptr;
};

#define __LOGO_H__
#endif // !__LOGO_H__
