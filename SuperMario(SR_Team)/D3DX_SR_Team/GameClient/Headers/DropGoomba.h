#pragma once
//��Ʋ���� 1 : �׳� �ϴÿ��� ���������ϴ� ����
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

	// CGameObject��(��) ���� ��ӵ�
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

	_bool bisTrigger;			//Ʈ���� �ߵ�����
	float m_fNowTime;			//����ð�
	float m_fDroptime;			//�� ������������ �ɸ��� �ð�
	float m_fDropStart;			//��� ��ġ
	float m_fDropEnd;			//������ (���� 0.0f 1.0f, 0.0f)

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

};
 

#endif // !__ATTACK_BULLET_DROPGOOMBA_H__