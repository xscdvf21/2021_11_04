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
	/* 버텍스 배열을 관리하는 컴객체 */
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	_uint m_iVertexSize;	// 버텍스 개당 메모리 크기
	_uint m_iVertexCount;	// 버텍스 개수
	_uint m_iFVF; // Flexible Vertex Format. 유연한 정점 형식.
	_uint m_iTriCount; // 폴리곤 개수


					   /*
					   인덱스 배열을 관리하는 컴객체
					   인덱스: 폴리곤을 표현하기 위한 정점들의 이웃관계 순서를 표현한 단위.
					   */
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	_uint m_iIndexSize; // 인덱스 개당 메모리 크기
	D3DFORMAT m_IndexFmt; // 인덱스 포맷. 16비트 or 32비트

	void* m_pVertexArr;		//버텍스 정보 저장
	void* m_pIndexArr;		//인덱스 정보 저장
	_bool	m_IsClone;
};

