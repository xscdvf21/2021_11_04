#ifndef Particle_Manager_h__
#define Particle_Manager_h__

#include "Particle.h"
#include "Shader.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CParticle_Manager :public CGameObject
{
	DECLARE_SINGLETON(CParticle_Manager)

public:
	explicit CParticle_Manager();
	virtual ~CParticle_Manager();

public:
	HRESULT		Ready_Particle_Manager(LPDIRECT3DDEVICE9 pDevice, _uint iSize);

public:
	void		Update_Particle(const _float& fDeltaTime);
	void		Update_Particle(PARTICLE& pParticle, const _float& fDeltaTime);
	void		LateUpdate_Particle();


	void		Render_Particle();
	void		Render_Particle(LPD3DXEFFECT & pEffect);


	//////////////////////////////////////////////////////////////////////////
	//Dot Particle
public:
	void		Update_DotParticle(const _float& fDeltaTime);
	void		Add_Particle();
	CParticle*	Emit_DotParticle(PARTICLE& desc, _uint Count, _uint PassIndex =0);
	void		Add_Particle(PARTICLE pParticle);
	void		Remove_DeadParticle();


	//////////////////////////////////////////////////////////////////////////
	//Image Particle
public:
	void		Update_ImageParticle(const _float& fDeltaTime);
	CParticle* Emit_ImageParticle(PARTICLE& desc, _uint TextureIndex, _uint PassIndex);


	//////////////////////////////////////////////////////////////////////////
	//Mesh Particle
public:
	CParticle* Emit_MeshParticle(PARTICLE& desc, const _wstr& tagMesh, _uint PassIndex);
	void		Update_MeshParticle(const _float& fDeltaTime);


	//////////////////////////////////////////////////////////////////////////
	//Setter
public:
	void		Set_Finish() { m_bFinish = true; }
	void		Set_Ready(_float fDelta) { m_bReady = true; m_fDelta = fDelta; } 


	//////////////////////////////////////////////////////////////////////////
	//Getter
public:


	//////////////////////////////////////////////////////////////////////////
	//Function
public:
	void		Set_AniState(PARTICLE& pParticle, _int x, _int y, _float fSpeed = 1.f);
	void		Reset_Particle(PARTICLE&);
	void		Stop_All_Particle();
	void		Reset();
	_bool		Set_Texture(const _wstr& tagTexture);
	CTexture*	Find_Texture(const _wstr& tagTexture);

public:
	virtual void Free() override;

private:
	list<CParticle*>	m_listImageParticle;
	list<CParticle*>	m_listActiveImageParticle;
	list<CParticle*>	m_listMeshParticle;
	list<PARTICLE>		m_listActiveParticle;

	LPDIRECT3DDEVICE9				m_pDevice;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;

	_float m_fSize;
	_wstr m_tagTexture;
	_int  m_MaxParticles;
	DWORD _vbSize;
	DWORD _vbOffset;
	DWORD _vbBatchSize;
	map<_wstr, Engine::CTexture*> m_mapTexture;

private:
	_float m_fDelta =0;
	_bool m_bReady = false;
	_bool m_bFinish = false;
	CShader* m_pShaderCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;

public:
	virtual void Render_Object(const _int& iIndex = 0) override;

private:
	DWORD m_dwTriCnt;
	DWORD m_dwVtxCnt;
	DWORD m_dwVtxSize;
	_ulong m_dwFVF;
};
END

#endif // Particle_Manager_h__