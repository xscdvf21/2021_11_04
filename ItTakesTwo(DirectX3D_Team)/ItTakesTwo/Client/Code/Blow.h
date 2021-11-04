
#ifndef Blow_h__
#define Blow_h__

#include "Export_Function.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CShader;
class CTexture;
END

class CBlow :public Engine::CGameObject
{
public:
	explicit CBlow(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBlow();

	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float & TimeDelta) override;
	virtual void Render_Object(const _int& iIndex = 0) override;

	static 	CBlow* Create(LPDIRECT3DDEVICE9 pDevice, void* pArg =nullptr);

	virtual void Free()override;
	void Set_Parent(const Engine::_mat& matParent);

private:
	HRESULT Add_Component(void* pArg);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _int& iIndex);

	void	Play_Sound();

public:
	void	Set_ChannelID(CSoundMgr::CHANNELID eChannelID) { m_eChannelID = eChannelID; }

private:
	Engine::CStaticMesh*	m_pMesh = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTexture*		m_pTextureCom2 = nullptr;
	const Engine::_mat*		m_pParent = nullptr;

private:
	_float			m_fEffectTime = 0.f;

private:
	_float			m_fTimeDelta = 0.f;
	_bool			m_bUp = true;

private:
	_float					m_fVolume = 0.0f;
	class CTransform*		m_pCodyTransform = nullptr;
	class CTransform*		m_pMayTransform = nullptr;

	_int					m_iChannelID = 0;
	CSoundMgr::CHANNELID	m_eChannelID = CSoundMgr::CHANNELID::Vacuum_Blow0;
};
#endif // Blow_h__