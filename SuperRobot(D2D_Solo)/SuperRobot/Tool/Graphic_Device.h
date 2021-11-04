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
	LPDIRECT3D9 m_pSDK; // 그래픽카드의 수준을 조사하고 생성하는 객체. 
	LPDIRECT3DDEVICE9 m_pDevice; // 애가 실질적으로 그래픽 장치를 제어하는 객체. 
	LPD3DXFONT	m_pFont; 
	// 장치를 생성하는 과정. 

	//1. 장치를 제어하기 위한 객체를 생성하는 객체(m_pSdk)를 생성. 
	//2. 장치의 수준을 조사해야한다. 
	//3. 수준에 맞는 장치를 제어하기 위한 객체 생성. 

	/*
	com - component Object Model의 약자. 
	우리가 배우고 있는 다이렉트에서는 장치 혹은 어떠한 데이터를 다루는 기능들을 하나의 부품처럼 제공해주고 있다. 
	그리고 사용자는 이 부품들을 이용하여 마치 레고를 조립하듯 프로그램을 구성해 나가야 한다. 
	*/
};

