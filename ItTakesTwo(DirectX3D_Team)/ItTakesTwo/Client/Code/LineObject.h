#ifndef LineObject_h__
#define LineObject_h__

#include "GameObject.h"

class CLineObject :public CGameObject
{
private:
	CLineObject(LPDIRECT3DDEVICE9 pDevice);
	CLineObject(CLineObject& other);
	~CLineObject() = default;
private:
	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float & TimeDelta) override;
	virtual void Render_Object(const _int& iIndex = 0) override;
	HRESULT Add_Component(void* pArg);
public:
	static CLineObject* Create(LPDIRECT3DDEVICE9 pDevice, void* pArg,const _wstr& tagLine, const _bool& bBack = false);
	virtual void Free(void) override;

	virtual _mat Get_EventPos() override;

public:
	virtual void* On_Active(void* pCaller) override;
	virtual void*			On_Trigger_Active(void* pTrigger, void* pCaller) override;
	virtual void* On_Attach(void* pCaller) override;
public:
	void Start();
private:
	void Load_Line(const _wstr& tagLine, const _bool& bBack);
	void Slide(const _float& TimeDelta);
private:
	vector<_vec3> m_vecPos;
	vector<_float> m_vecPoint;
	vector<vector<_vec3>*> m_vecSlidePos;

	_int m_iSlideIndex;
	_int m_iPathCount = 0;
	size_t m_iInterval = 0;

	_float m_fSlideSpeed = 1.f;

	_bool m_bSlide = false;
	_bool m_bSlideEnd = false;
	_bool m_bSlideStart = true;

};
#endif // LineObject_h__