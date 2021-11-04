#pragma once
class CPlayer; 
class CFrame_Manager; 
class CMainApp final 
{
private:
	explicit CMainApp();
	~CMainApp();
public:
	HRESULT Ready_MainApp(); 
	void Update_MainApp(); 
	void Late_UpDate_MainApp(); 
	void Render_MainApp(CFrame_Manager& rFrame); 
	void Release_MainApp(); 
public:
	static CMainApp* Create();
	void Free();
private:

};

