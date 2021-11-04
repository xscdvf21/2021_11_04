#ifndef PosChange_h__
#define PosChange_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;
class CRcTex;
class CTexture;
class CRenderer;
class CShader;
END

class CPosChange : public Engine::CGameObject
{
private:
	explicit CPosChange(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPosChange(void);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	_vec3					Get_SavePoint() { return m_vSavePoint; }
	_float					Get_Z();

public:
	Engine::CShader*		m_pShaderCom = nullptr;
public:

public:
	static CPosChange*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fZ ,void* pArg = nullptr);
	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;

private :			
	_bool					m_bTex;
	_vec3					m_vSavePoint {};
	_float					m_fZ = 0.0f;
private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
