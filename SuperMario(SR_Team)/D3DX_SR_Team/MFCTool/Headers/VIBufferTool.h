#pragma once
#include "ComponentTool.h"

USING(Engine)
class CVIBufferTool :
	public CComponentTool
{
protected:
	explicit CVIBufferTool(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBufferTool(const CVIBufferTool& other);
	virtual ~CVIBufferTool() = default;

public:
	virtual HRESULT Ready_Component_Prototype() = 0;
	virtual HRESULT Ready_Component(void* pArg = nullptr) = 0;
	virtual HRESULT Render_VIBuffer() = 0;
	virtual HRESULT Set_Transform(const _float4x4* pWorldMatrix, const _float4x4* pViewMatrix, const _float4x4* pProjMatrix) { return S_OK; }

public:
	virtual CComponentTool* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	/* ���ؽ� �迭�� �����ϴ� �İ�ü */
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	_uint m_iVertexSize;	// ���ؽ� ���� �޸� ũ��
	_uint m_iVertexCount;	// ���ؽ� ����
	_uint m_iFVF; // Flexible Vertex Format. ������ ���� ����.
	_uint m_iTriCount; // ������ ����


					   /*
					   �ε��� �迭�� �����ϴ� �İ�ü
					   �ε���: �������� ǥ���ϱ� ���� �������� �̿����� ������ ǥ���� ����.
					   */
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	_uint m_iIndexSize; // �ε��� ���� �޸� ũ��
	D3DFORMAT m_IndexFmt; // �ε��� ����. 16��Ʈ or 32��Ʈ

	void* m_pVertexArr;		//���ؽ� ���� ����
	void* m_pIndexArr;		//�ε��� ���� ����
	_bool	m_IsClone;
};

