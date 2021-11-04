#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);

public:
	_float								Get_ViewZ(void) { return m_fViewZ; }
	CComponent*							Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CComponent*							Find_Component(const _tchar* pComponentTag, COMPONENTID eID);
	list<pair<_vec3, _float>>&			Get_Spherelist();
	void								Set_CollCheck(_bool _Coll);
	_bool								Get_Coll();

	_uint								Get_PotionCount() { return m_iCount; }
	_uint								Get_MaxPotionCount() { return m_iPotionMaxCount; }

	INFOBOSS&							Get_INFOBOSS() { return m_tINFOBOSS; }
	INFOPLAYER&							Get_INFOPLAYER() { return m_tINFOPLAYER; }

	void								Set_INFOBOSS(INFOBOSS _INFOBOSS) { m_tINFOBOSS = _INFOBOSS; }
	void								Set_INFOPLAYER(INFOPLAYER _INFOPLAYER) { m_tINFOPLAYER = _INFOPLAYER; }
	void								Set_PotionCount(_uint iCount) { m_iCount = iCount; }
	void								Set_MaxPotionCount(_uint iCount) { m_iPotionMaxCount = iCount; }
public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& TimeDelta);
	virtual void		Render_Object(void);

	void				Compute_ViewZ(const _vec3* pPos);

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	list < pair<_vec3, _float>>			m_listCollision;
	list < pair<_vec3, _float>>			m_listAttackCollision;
	_float								m_fViewZ = 0.f;


	_uint								m_iCount;
	_uint								m_iPotionMaxCount;

	INFOPLAYER							m_tINFOPLAYER;
	INFOBOSS							m_tINFOBOSS;

	_bool								m_bColl = false;

public:
	virtual void	Free(void);

};

END
#endif // GameObject_h__
