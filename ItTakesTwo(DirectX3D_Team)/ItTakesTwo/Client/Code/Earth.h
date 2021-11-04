#ifndef Earth_h__
#define Earth_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CEarth : public Engine::CGameObject
{
private:
	explicit CEarth(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarth(void);

public:
	 HRESULT		Ready_Object(void* pArg = nullptr) override;
	 virtual _int	Update_Object(const _float& fTimeDelta) override;
	 virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	 virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	void			Set_Stop() { m_bStop = true; }

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	_vec3					m_vDir;
	_uint					m_iFlag = 0;
	_bool					m_bStop;
	_float					m_fHeight;

private:
	_float					m_fTimeDelta = 0.f;

public:
	static CEarth*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;

};

#endif // Earth_h__
