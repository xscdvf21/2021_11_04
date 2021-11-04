#pragma once

#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	enum class eVertexType
	{
		Color,
		Textrue,
		Wireframe
	};


protected:
	explicit CVIBuffer(_Device pDevice);
	explicit CVIBuffer(const CVIBuffer& other);
	virtual ~CVIBuffer() = default;


public:
	virtual HRESULT Ready_Component_Prototype() PURE;
	virtual HRESULT Ready_Component(void* pArg = nullptr) PURE;
	virtual HRESULT Render_VIBuffer() PURE;
	//�� Ʈ������ ���� ���� �ȹھƵ� ��

public:
	eVertexType Get_VertexType();
	void		Set_VertexType(eVertexType eType);
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color,_uint iIndex = 0) PURE;
	void* Get_VertexData();
	void* Get_IndexData();
	HRESULT	Set_VertexData(void* _vertexData);
	//virtual void Set_IndexData(void* _indexData);

public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

protected:
	_Device m_pDevice;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	_uint m_iVertexSize;	//���ؽ� "����" �޸� ũ��
	_uint m_iVertexCount;	//���ؽ� ����
	_uint m_iFVF;			//FVF ���
	_uint m_iTriCount;		//������ ����

	LPDIRECT3DINDEXBUFFER9 m_pIB;
	_uint m_iIndexSize; // �ε��� ���� �޸� ũ��
	D3DFORMAT m_IndexFmt; // �ε��� ����. 16��Ʈ or 32��Ʈ

	void* m_pVertexArr;		//���ؽ� ���� ����
	void* m_pIndexArr;		//�ε��� ���� ����
	_bool		m_IsClone;
	eVertexType m_eVType;	//���ؽ� Ÿ��
};

END

#endif // !__VIBUFFER_H__