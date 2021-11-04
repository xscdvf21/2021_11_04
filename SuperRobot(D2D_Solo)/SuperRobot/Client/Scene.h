#pragma once
class CScene abstract
{
public:
	explicit CScene();
	virtual ~CScene();

public:
	virtual HRESULT Ready_Scene()PURE;
	virtual _int	Update_Scene()PURE;
	virtual void	LateUpdate_Scene()PURE;
	virtual void	Render_Scene()PURE;
	virtual void	Release_Scene()PURE;

public :
	void Set_StagePlay(bool _StagePlay) { m_bStagePlayCheck = _StagePlay; }

protected:
	int			iSceneNum;
	bool		m_bStagePlayCheck; //스테이지1에서 지원군 도착 한번만 하기위해사용.
};

