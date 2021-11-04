#pragma once

#ifndef __BATTLEPLATE_SLIDE_H__
#define __BATTLEPLATE_SLIDE_H__

#include "BattlePlateEffect.h"

USING(Engine);
//슬라이드 이펙트 패널
class CBattlePlateSlide : public CBattlePlateEffect
{
private:
	explicit CBattlePlateSlide(_Device pDevice);
	explicit CBattlePlateSlide(const CBattlePlateSlide& other);
	virtual ~CBattlePlateSlide() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;


public:
	static CBattlePlateSlide* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	//_float3 avPos[7];
	//_float3 avScale[7];



};

#endif