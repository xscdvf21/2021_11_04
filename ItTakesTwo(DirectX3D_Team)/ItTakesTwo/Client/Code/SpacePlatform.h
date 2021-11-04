#ifndef __SpacePlatform_H__

#include "GameObject.h"
#include "Engine_Define.h"


BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CSpacePlatform : public CGameObject
{
private:
	explicit CSpacePlatform(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpacePlatform();

	void Move1stDoor();
	void Move2ndDoor();
public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;




	virtual void* On_Active(void* pCaller) override;


	virtual void* On_Trigger_Active(void* pTrigger, void* pCaller) override;

public:
	void				Reset_State();
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
public:
	static CSpacePlatform*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void						Free(void)		override;

private:
	void Move(const _float & fTimeDelta);
	_bool m_bMove = false;
	_bool m_bDoorActive[2] = { 0, };
	_float m_fTime = 0.f;
	_float m_fPushTime = 0.f;
	_int m_iCount = 0;
	_vec3 m_vGoalPos = {0.f,0.f,0.f};
	_vec3 m_vStartPos = { 0.f,0.f,0.f };
	_bool m_bArrive = false;

private:
	_float					m_fVolume = 0.0f;
	class CTransform*		m_pCodyTransform = nullptr;
	class CTransform*		m_pMayTransform = nullptr;
};

#define __SpacePlatform_H__
#endif

