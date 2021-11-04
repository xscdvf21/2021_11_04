#ifndef Particle_h__
#define Particle_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CShader;
class CStaticMesh;

class ENGINE_DLL CParticle :	public CGameObject
{
public:
	enum Mode
	{
		IMAGE,
		MESH,
	};
	enum TextureIndex
	{
		SMOKE,
	};

public:
	explicit CParticle(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CParticle();

public:
	virtual HRESULT	Ready_Object(void * pArg = nullptr) override;
	virtual _int	Update_Object(const _float& TimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& TimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	void			SetUp_AddTexture(LPD3DXEFFECT & pEffect);

public:
	void			Set_SpawnData(PARTICLE& tDesc);
	void			Set_Active(const _bool& bCondition) { m_tParticle.bActive = bCondition; }
	void			Change_Pass(_uint PassIndex);
	void			Change_Texture(_uint textureIndex);
	void			Set_Mesh(const _wstr& tagMesh);
	void			Set_AniState(PARTICLE& pParticle, _int x, _int y, _float fSpeed /*= 1.f*/);
	void			Set_ParticlePos(_vec3 vPos);
	void			Set_Amount(_float fAmount) { m_fAmount = fAmount; }

public:
	const _bool&	Get_Active() { return m_tParticle.bActive; }
	PARTICLE&		Get_Particle() { return m_tParticle; }
	CTexture*		Get_Texture() { return m_pTextureCom; }
public:
	static void			Reset_Particle(PARTICLE& pParticle);
	static CParticle*	Create(LPDIRECT3DDEVICE9 pDevice);
public:
	_mat CParticle::Is_Billboarding(const _int& iIndex);
private:
	PARTICLE				m_tParticle;
	_uint					m_iTextureIndex;
	_uint					m_iPassIndex;
	_vec4					m_vRandom;
	_vec4					m_vColor;
	_uint					m_iSceneIndex =0;
	_uint					m_eMode =0;
	_float					m_fAmount = 0.f;
	_float					m_fDissolve = 0.f;
	SHADER					m_tShader;

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr; 

public:
	virtual void Free() override;
};
END

#endif // Particle_h__