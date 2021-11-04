#ifndef __GeneratorBattery_H__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CGeneratorBattery : public CGameObject
{
private:
	explicit CGeneratorBattery(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGeneratorBattery();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr, _vec3 TriggerPos = { 0.f, 0.f,0.f }, _vec3 StartPos = { 0.f,0.f,0.f }, _vec3 MoveDir = { 1.f, 0.f, 0.f });
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

	


	virtual void*		On_Active(void* pCaller) override;

	void				Active();

public:
	_vec3				Get_Push_StartPos() { return m_vPushStartPos; }
	void				Set_Move() { m_bMove = true; }
	_bool				Get_MoveEnd() { return m_bMoveEnd; }

	_vec3				Get_MoveDIr() { return m_vMoveDir; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);


private:
	void				Move(const _float& fTimeDelta);


private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
	_vec3							m_vPushStartPos;
	_vec3							m_vTriggerPos;
	_vec3							m_vTriggerScale;
	CTrigger*						m_pTrigger = nullptr;
	class CBoundingBox*				m_pBox = nullptr;

	_bool							m_bSoundStart= false;
	_bool							m_bMove = false;
	_bool							m_bMoveEnd = false;
	_float							m_fAngle = D3DXToRadian(20.f);
	_vec3							m_vMoveDir;
public:
	static CGeneratorBattery*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _vec3 TriggerPos = { 0.f, 0.f,0.f }, _vec3 StartPos = { 0.f,0.f,0.f }, _vec3 MoveDir = {1.f, 0.f, 0.f});
	virtual void							Free(void)		override;

};

#define __GeneratorBattery_H__
#endif

