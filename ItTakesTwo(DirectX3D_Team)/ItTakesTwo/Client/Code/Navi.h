#ifndef Navi_h__
#define Navi_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CNaviMesh;
class CShader;

END

class CNavi : public Engine::CGameObject
{
private:
	explicit CNavi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNavi(void);

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_Material(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

public:
	HRESULT			Freeze_Cell();
	void			Set_Freeze(_bool freeze) { m_bFreeze = freeze; }

public:
	Engine::CNaviMesh*	m_pNaviMeshCom = nullptr;
	Engine::CShader*	m_pShaderCom = nullptr;

public:
	_uint	m_TextureIndex = 0;
	_uint	m_eMesh;
	_bool	m_bNaviOn;
	_bool	m_bFreeze;

public:
	static CNavi*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

private:
	virtual void Free(void) override;



};



#endif // Navi_h__
