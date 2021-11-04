#ifndef __GENERATORLEVER_H__

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CGeneratorLever : public CGameObject
{
private:
	explicit CGeneratorLever(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGeneratorLever();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr, _vec3 TriggerPos = { 0.f, 0.f,0.f }, _vec3 StartPos = { 0.f,0.f,0.f }, wstring _tag = L"", _uint DirType = 0);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

public :
	_vec3				Get_Push_StartPos() { return m_vPushStartPos; }
	void				Set_Move() { m_bMove = true; }
	
	void				Set_MoveReturn() { m_bMoveReturn = true; }
	void				Set_Field_CutScene() { m_bField_CutScene = true; }


	wstring				Get_Bettry() { return m_tagBattery; }
	_bool				Get_MoveEnd() { return m_bMoveEnd; }
	_vec3				Get_PushDir() { return m_vPushDir; }

public:
	void				Set_LeverDir(_uint iDir) { m_iDir = iDir; }
	_uint				Get_LeverDir() { return m_iDir; }

private:
	void				Move(const _float& fTimeDelta);
	void				MoveRetrun(const _float& fTimeDelta);
	
	void				Field_CutScene(const _float& fTimeDelta);
	void				Field_CutScene2(const _float& fTimeDelta);

private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;
private:
	_vec3							m_vPushStartPos;
	_vec3							m_vTriggerPos;
	_vec3							m_vTriggerScale;
	CTrigger*						m_pTrigger = nullptr;


	_bool							m_bField_CutScene = false;
	_bool							m_bField_CutScene2 = false;
	_bool							m_bMove = false;
	_bool							m_bMoveEnd = false;
	_bool							m_bMoveReturn = false;
	_float							m_fAngle = D3DXToRadian(50.f);


	_bool							m_bSoundStart = false;
	wstring							m_tagBattery = L"";
	_uint							m_iTypeIndex = 0;

	_vec3							m_vPushDir = {};
	_uint							m_iDir = 0;
	_bool							m_bCutSceneStart = true;
public:
	static CGeneratorLever*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, _vec3 TriggerPos = { 0.f, 0.f,0.f }, _vec3 StartPos = { 0.f,0.f,0.f }, wstring _tag = L"", _uint DirType = 0, _vec3 vPushDir = _vec3(0.0f, 0.0f, 1.0f));
	virtual void					Free(void)		override;

};

#define __GENERATORLEVER_H__
#endif

