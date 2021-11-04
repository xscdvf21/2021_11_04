#ifndef TestMonster_h__
#define TestMonster_h__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CTriCol;
class CTransform;

END

class CTestMonster : public Engine::CGameObject
{
private:
	explicit CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestMonster(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

private:
	Engine::CTriCol*			m_pBufferCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;


	_vec3					m_vDir;

public:
	static CTestMonster*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

#endif // TestMonster_h__
