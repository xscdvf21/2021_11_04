#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "Component.h"


BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	void		Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup(void);

private:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Black(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Map(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];

public:
	virtual CComponent*		Clone() { return nullptr; }
private:
	virtual void	Free(void);

};

END
#endif // Renderer_h__
