#pragma once
#ifndef __COMPONENT_MANAGER_H__

#include "Base.h"


#include "VIBuffer_SlideCubeColor.h"
#include "VIBuffer_CubeColor.h"
#include "VIBuffer_CubeTexture.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_RectTexture.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Alpha_Round.h"
#include "Transform.h"
#include "Texture.h"
#include "Font_Component.h"

#include "HealthCondition.h"


BEGIN(Engine)
class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Ready_Component_Manager(_uint iSceneCount);
	HRESULT Add_Component_Prototype(_uint iSceneIndex, const wstring& PrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iSceneIndex, const wstring& PrototypeTag, void* pArg = nullptr);
	void Clear_ForScene(_uint iSceneIndex);

public:
	virtual void Free() override;

private:
	/* unordered_map: �ؽ� ����� ��, Ž���ӵ��� ���ϰ� ������. ��� Ű�� ������ ����. */
	typedef unordered_map<wstring, CComponent*> COMPONENTS; /* ������Ʈ���� �������� ������ �����̳� */
	COMPONENTS*	m_pComponents = nullptr; /* �����迭 */
	_uint		m_iSceneCount = 0;


};
END


#define __COMPONENT_MANAGER_H__
#endif // !__COMPONENT_MANAGER_H__
