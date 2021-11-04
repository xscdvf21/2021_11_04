#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
	, m_pErrMsg(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)	
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
	
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	/*���� m_pEffect ���� �ְ� m_pErrMsg ���� ���� ��� : ���������� ���̴��� ����
	���� m_pEffect ���� �ְ� m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ� ��� ����, �������� ���ϴ� �� �ƴ�
	���� m_pEffect ���� ���� m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ� ������ �־ ������ ���� ��Ȳ*/


	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, 
										pFilePath,
										NULL,	// ���̴����� ��� ������ ��ũ�� ����
										NULL,	// ���̴����� ��� ������ ���� ����
										D3DXSHADER_DEBUG,	//���̴��� �ܺο��� ������ϰڴٴ� �ǹ�(�ܺ� ���̺귯���� �̿��Ͽ� ������� �� ���� �ֱ� ������ ������ ����Ͽ� ����)
										NULL,	// ���� ���ҽ� ������ ��� ������ ���ΰ�(null�ΰ�� �׷��� �޸𸮿� �˾Ƽ� ����)
										&m_pEffect,
										&m_pErrMsg)))	//!!�߿�!! : ������� �Ұ��������� ���� �� ��� ���� ��� �� �޽����� ���ڿ��� ���·� �����ϱ� ���� ������ �޸� ����
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader warning", MB_OK);
	}

	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CShader::Clone(void* pArg)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}

