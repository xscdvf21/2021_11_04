#ifndef __INVECTORY_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CRenderer;
class CTransform;

END

class CInventory : public Engine::CGameObject
{
	enum INVEN_STATE
	{
		INVEN_FRAME, INVEN_HP, INVEN_SP,INVEN_CURSOR, INVEN_END
	};



private:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInventory(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual void Render_Object() override;

public :
	vector<CGameObject*>		Get_Inventory() { return m_vecInven; }
	_uint						Get_ITEMCount() { return m_iSelectPotionCount; }
	_uint						Get_SPCount() { return m_iSelectSPCount; }
private:
	HRESULT					Add_Component(void);

	void					Render_KeyInput();

	void					Render_InvenFrame();
	void					Render_InvenHP();
	void					Render_InvenSP();
	void					Render_Cursor();



private:
	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

	_float					m_CursorfX, m_CursorfY;


private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom[INVEN_END];
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	
	vector<CGameObject*>	m_vecInven;

	_bool					m_bInvenSee = false;
	_uint					m_iSelectPotionCount = 0; //������ HP������ 5���� �����°� �Ѱܼ� ǥ�����ֱ�����.
	_uint					m_iSelectSPCount = 0; //������ SP���� 5���� �����°� �Ѱܼ� ǥ��.
	_uint					m_iInvenIndex = 0; //���Ϳ� �����Ҷ� ���° ���ҿ� �����Ұ��� ����.
	
public:
	static CInventory*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void			Free(void) override;

};




#define __INVECTORY_H__
#endif // !__INVECTORY_H__






