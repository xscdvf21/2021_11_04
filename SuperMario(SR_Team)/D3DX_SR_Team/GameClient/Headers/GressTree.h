#pragma once
#ifndef __GRESSTREE_H__

#include "GameObject.h"
USING(Engine)
class CGressTree final : public CGameObject
{
	enum GressTree
	{
		Gress,
		Tree,
		end
	};
public:
	explicit CGressTree(LPDIRECT3DDEVICE9 pDevice);
	explicit CGressTree(const CGressTree& other);
	virtual ~CGressTree() = default;


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private :
	HRESULT		Add_Component();


public:
	static	CGressTree*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject *	Clone(void * pArg = nullptr) override;
	virtual void			Free()	override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom[GressTree::end];

	_uint		GressTree = 0;

};

#define __GRESSTREE_H__
#endif // !__GRESSTREE_H__


