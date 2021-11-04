#pragma once
class CGraphic_Device final 
{
public:
	static CGraphic_Device* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGraphic_Device; 
		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
private:
	CGraphic_Device();
	~CGraphic_Device();
public:
	LPDIRECT3DDEVICE9 Get_Device() { return m_pDevice; }
	LPD3DXSPRITE Get_Sprite() { return m_pSprite; }
	LPD3DXFONT Get_Font() { return m_pFont; }
public:
	HRESULT Ready_Graphic_Device(); 
	void Release_Graphic_Device();

	void Render_Begin();
	void Render_End(HWND hWND = nullptr);
private:
	static CGraphic_Device* m_pInstance; 
	LPD3DXSPRITE m_pSprite; 
	LPDIRECT3D9 m_pSDK; // �׷���ī���� ������ �����ϰ� �����ϴ� ��ü. 
	LPDIRECT3DDEVICE9 m_pDevice; // �ְ� ���������� �׷��� ��ġ�� �����ϴ� ��ü. 
	LPD3DXFONT	m_pFont; 
	// ��ġ�� �����ϴ� ����. 

	//1. ��ġ�� �����ϱ� ���� ��ü�� �����ϴ� ��ü(m_pSdk)�� ����. 
	//2. ��ġ�� ������ �����ؾ��Ѵ�. 
	//3. ���ؿ� �´� ��ġ�� �����ϱ� ���� ��ü ����. 

	/*
	com - component Object Model�� ����. 
	�츮�� ���� �ִ� ���̷�Ʈ������ ��ġ Ȥ�� ��� �����͸� �ٷ�� ��ɵ��� �ϳ��� ��ǰó�� �������ְ� �ִ�. 
	�׸��� ����ڴ� �� ��ǰ���� �̿��Ͽ� ��ġ ���� �����ϵ� ���α׷��� ������ ������ �Ѵ�. 
	*/
};

