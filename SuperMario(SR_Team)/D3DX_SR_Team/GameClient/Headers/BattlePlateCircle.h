#pragma once


#ifndef __BATTLEPLATE_CIRCLE_H__
#define __BATTLEPLATE_CIRCLE_H__

//#include "GameObject.h"
#include "BattlePlateEffect.h"

USING(Engine);
//원형으로 도는 이펙트 패널
class CBattlePlateCircle : public CBattlePlateEffect
{
private:
	explicit CBattlePlateCircle(_Device pDevice);
	explicit CBattlePlateCircle(const CBattlePlateCircle& other);
	virtual ~CBattlePlateCircle() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static CBattlePlateCircle* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	_float3 avPos[5];
	_float3 avScale[5];
};


#endif // !__BATTLEPLATE_CIRCLE_H__