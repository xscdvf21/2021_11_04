#pragma once
#ifndef __STAGEUI_H__

#include "GameObject.h"


USING(Engine)

class CStageUI : public CGameObject

{
public:
	explicit CStageUI(_Device _pDevice);
	explicit CStageUI(const CStageUI& other);
	virtual	~CStageUI() =default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Set_Transform();

public:
	static CStageUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
	class CTransform* m_pTransformComp = nullptr;

private:
	class CStageCoin*	m_pCoin_UI = nullptr;
	class CStageHP*		m_pHp_UI = nullptr;
	class CPaperUI*		m_pPaper_UI = nullptr;
};

#define	__STAGEUI_H__
#endif // !__STAGEUI_H__



