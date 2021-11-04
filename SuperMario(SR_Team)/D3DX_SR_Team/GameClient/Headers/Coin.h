#pragma once
#ifndef __COIN_H__

#include "GameObject.h"

USING(Engine)
class CCoin final : public CGameObject
{
private :
	explicit CCoin(LPDIRECT3DDEVICE9 pDevice);
	explicit CCoin(const CCoin& other);
	virtual ~CCoin() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public : //Set
	void Set_Pos(const _float3 _vPos) { m_pTransformCom->Set_Position(_vPos); }
private :
	HRESULT		Add_Component();
	HRESULT		Movement(_float fDeltaTime);

public :
	static CCoin* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free();

private :
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	_float m_fAngle = 0.f;

};



#define __COIN_H__
#endif // !__COIN_H__



