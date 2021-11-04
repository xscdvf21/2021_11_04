#pragma once
#ifndef __BATTLEPLATE_SELECT_CUBE_H__
#define __BATTLEPLATE_SELECT_CUBE_H__

#include "BattlePlateEffect.h"

USING(Engine)
//해머 이펙트 판정 범위 (근데 걍 큐브라 부름)
class CBattlePlateSelectCube : public CBattlePlateEffect
{
private:
	explicit CBattlePlateSelectCube(_Device pDevice);
	explicit CBattlePlateSelectCube(const CBattlePlateSelectCube& other);
	virtual ~CBattlePlateSelectCube() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;
	void Set_avIndex(_uint iIndex) { avIndex = iIndex; }


public:
	static CBattlePlateSelectCube* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

};


#endif // !__BATTLEPLATE_SELECT_CUBE_H__