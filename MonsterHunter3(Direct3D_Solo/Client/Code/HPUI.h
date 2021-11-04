#ifndef __HPUI_H__


#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;

END

class CHPUI : public Engine::CGameObject
{
	
private:
	explicit CHPUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHPUI(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object() override;

private:
	HRESULT					Add_Component(void);

	void					Render_UI_Hp_Frame();
	void					Render_UI_HP_Bar();
	void					Render_UI_Item_Frame();
	void					Render_UI_Item_HP();
	void					Render_UI_Item_SP();
	void					Render_UI_Stemina_Bar();
	void					Rneder_UI_Victory();

public:
	_uint					Get_ItemCount() { return m_iItemCount;}
	UIID					Get_ItemType() { return m_ItemType; }


	void					Set_ItemCount(_uint _iCount) { m_iItemCount = _iCount; }
	void					Set_ItemSelect(_uint _iCount) { m_iSelectItem = _iCount; }

private:
	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

	_bool					m_bVictory = false; //보스가 죽었을 떄, 잠깐 보여주기 위해.
	_float					m_fVictory = 0.f; //보스가 죽었을 때 승리UI보여줄 라이프타임.

	_tchar					m_szFPS[256] = L"";

	_uint					m_iItemCount = 0;
	_uint					m_iSPCount = 0;//SP 포션 갯수 표현

	_int					m_iSelectItem = 0; //현재 아이템이 어떤게 선택되어있는지.

	_bool					m_bVictorySound = false; //빅토리 UI나올떄 소리나오게하기위해.
private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom[UIID_END];
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;

	UIID					m_ItemType;

public:
	static CHPUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;



};

#define __HPUI_H__
#endif // !__HPUI_H__


