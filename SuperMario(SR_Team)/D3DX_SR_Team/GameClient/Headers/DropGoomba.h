#pragma once
//배틀씬용 1 : 그냥 하늘에서 수직낙하하는 굼바
#ifndef __ATTACK_BULLET_DROPGOOMBA_H__
#define __ATTACK_BULLET_DROPGOOMBA_H__

#include "GameObject.h"

USING(Engine);

class CDropGoomba final : public CGameObject
{
private:
	explicit CDropGoomba(_Device pDevice);
	explicit CDropGoomba(const CDropGoomba& other);
	virtual ~CDropGoomba() = default;

public:

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;


	static CDropGoomba* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

public:
	void Set_Drop() { bisTrigger = true; }

private:
	HRESULT Add_Component();

	_bool bisTrigger;			//트리거 발동여부
	float m_fNowTime;			//현재시간
	float m_fDroptime;			//다 떨어질때까지 걸리는 시간
	float m_fDropStart;			//출발 위치
	float m_fDropEnd;			//목적지 (보통 0.0f 1.0f, 0.0f)

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

};
 

#endif // !__ATTACK_BULLET_DROPGOOMBA_H__