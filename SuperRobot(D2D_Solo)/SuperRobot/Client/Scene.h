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
	bool		m_bStagePlayCheck; //��������1���� ������ ���� �ѹ��� �ϱ����ػ��.
};

