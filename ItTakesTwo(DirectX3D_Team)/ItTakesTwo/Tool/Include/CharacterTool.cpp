// CharacterTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Export_Function.h"
#include "CharacterTool.h"
#include "afxdialogex.h"

#include "Base.h"
#include "Engine_Define.h"
#include "Layer.h"
#include "ToolObject.h"

#include "FileInfo.h"
#include "ToolDoc.h"
#include "MainFrm.h"
#include "ToolView.h"


// CCharacterTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCharacterTool, CDialog)

CCharacterTool::CCharacterTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CHARACTERTOOL, pParent)
	, m_ScaleX(0)
	, m_ScaleY(0)
	, m_ScaleZ(0)
	, m_PositionX(0)
	, m_PositionY(0)
	, m_PositionZ(0)
	, m_Map(_T(""))
{
}

CCharacterTool::~CCharacterTool()
{

}

void CCharacterTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO11, m_SelectPos[0]);
	DDX_Control(pDX, IDC_RADIO12, m_SelectPos[1]);
	DDX_Control(pDX, IDC_RADIO13, m_SelectPos[2]);
	DDX_Control(pDX, IDC_RADIO14, m_MeshType[0]);
	DDX_Control(pDX, IDC_RADIO15, m_MeshType[1]);
	DDX_Control(pDX, IDC_RADIO17, m_Alpha[0]);
	DDX_Control(pDX, IDC_RADIO18, m_Alpha[1]);
	DDX_Control(pDX, IDC_LIST1, m_ObjectList);
	DDX_Text(pDX, IDC_EDIT2, m_ScaleX);
	DDX_Text(pDX, IDC_EDIT7, m_ScaleY);
	DDX_Text(pDX, IDC_EDIT8, m_ScaleZ);
	DDX_Text(pDX, IDC_EDIT9, m_PositionX);
	DDX_Text(pDX, IDC_EDIT10, m_PositionY);
	DDX_Text(pDX, IDC_EDIT11, m_PositionZ);
	DDX_Text(pDX, IDC_EDIT4, m_Map);
}


BEGIN_MESSAGE_MAP(CCharacterTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CCharacterTool::OnLbnSelchangeObjectList)
	ON_BN_CLICKED(IDC_BUTTON1, &CCharacterTool::OnBnClickedCreateObject)
	ON_BN_CLICKED(IDC_BUTTON2, &CCharacterTool::OnBnClickedObjectListSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CCharacterTool::OnBnClickedObjectListLoad)
	ON_BN_CLICKED(IDC_BUTTON10, &CCharacterTool::OnBnClickedScalePosSet)
	ON_BN_CLICKED(IDC_BUTTON11, &CCharacterTool::OnBnClickedSelectDelete)
	ON_BN_CLICKED(IDC_BUTTON9, &CCharacterTool::OnBnClickedSelectCreate)
	ON_BN_CLICKED(IDC_BUTTON6, &CCharacterTool::OnBnClickedAddMap)
END_MESSAGE_MAP()


// CCharacterTool 메시지 처리기입니다.


void CCharacterTool::OnLbnSelchangeObjectList()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	int iIndex = m_ObjectList.GetCurSel();
	if (LB_ERR == iIndex)
		return;

	CString strFileName;
	m_ObjectList.GetText(iIndex, strFileName);
	lstrcpy(m_Texture, strFileName.GetString());
	m_TextureIndex = iIndex;

	UpdateData(FALSE);
}

void CCharacterTool::OnBnClickedCreateObject()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(TRUE, L"x", L"*.x", OFN_OVERWRITEPROMPT, L"Data File(*.x)|*.x||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Resource\\Mesh");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();

		CString strRelativePath = CFileInfo::ConvertRelativePath(strFilePath);
		CString strFileNameAndExtant = PathFindFileName(strFilePath);
		lstrcpy(szBuf, strFileNameAndExtant.GetString());
		PathRemoveExtension(szBuf);
		CString strSave = szBuf;

		CString strInt;
		strInt = strSave[strSave.GetLength() - 1];
		int iIndex = _ttoi(strInt);

		// 오브젝트 추가
		Engine::CGameObject*		pGameObject = nullptr;

		CString tagObj;
		tagObj = strSave.Left(strSave.GetLength() - 1);

		OBJINFO tObj;
		tObj.vPos = _vec3(0.f, 0.f, 0.f);
		tObj.vScale = _vec3(0.01f, 0.01f, 0.01f);
		tObj.vAngle = _vec3(0.f, 0.f, 0.f);
		lstrcpy(tObj.tagMesh, tagObj);
		tObj.eMeshType = m_eMeshType;
		tObj.eRenderType = m_eRenderType;

		// Create Object
		auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
		pGameObject = CToolObject::Create(pView->m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, );

		map<_wstr, CGameObject*> GameObject = pLayer->second->Get_GameObjectMap();

		auto findGameObject = find_if(GameObject.begin(), GameObject.end(), CTag_Finder(szBuf));
		if (findGameObject == GameObject.end())
		{
			pLayer->second->Add_GameObject(strSave, pGameObject);
			m_vecObject.emplace_back(strSave);
			m_ObjectList.AddString(strSave);
		}
		else
		{
			while (true)
			{
				++iIndex;
				CString Temp = strSave;
				Temp = Temp.Left(Temp.GetLength() - 1);

				tObj.vPos = _vec3(0.f, 0.f, 0.f);
				tObj.vScale = _vec3(0.01f, 0.01f, 0.01f);
				tObj.vAngle = _vec3(0.f, 0.f, 0.f);
				lstrcpy(tObj.tagMesh, Temp);
				tObj.eMeshType = m_eMeshType;
				tObj.eRenderType = m_eRenderType;

				wstring tagtemp = std::to_wstring(iIndex);
				strSave = Temp + tagtemp.c_str();

				auto findObj = find_if(GameObject.begin(), GameObject.end(), CTag_Finder(strSave));
				if (findObj == GameObject.end())
				{
					pLayer->second->Add_GameObject(strSave, pGameObject);
					m_vecObject.emplace_back(strSave);
					m_ObjectList.AddString(strSave);
					break;
				}
			}
		}
	}
	UpdateData(FALSE);
}

BOOL CCharacterTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SelectPos[0].SetCheck(TRUE);
	m_MeshType[0].SetCheck(TRUE);
	m_Alpha[0].SetCheck(TRUE);

	return TRUE;
}

void CCharacterTool::HorizontalScroll()
{
	int iCX = 0;

	CString strFilePath = L"";
	CSize tSize{};

	CDC* pDC = m_ObjectList.GetDC();
	for (int i = 0; i < m_ObjectList.GetCount(); ++i)
	{
		m_ObjectList.GetText(i, strFilePath);
		tSize = pDC->GetTextExtent(strFilePath);

		if (iCX < tSize.cx)
			iCX = tSize.cx;
	}

	if (iCX > m_ObjectList.GetHorizontalExtent())
		m_ObjectList.SetHorizontalExtent(iCX);
	ReleaseDC(pDC);
}

void CCharacterTool::Update_ObjectTool()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	// MESH TYPE
	if (m_MeshType[0].GetCheck() == TRUE)
		m_eMeshType = Engine::ID_STATIC;
	else if (m_MeshType[1].GetCheck() == TRUE)
		m_eMeshType = Engine::ID_DYNAMIC;

	// RENDER TYPE
	if (m_Alpha[0].GetCheck() == TRUE)
		m_eRenderType = Engine::RENDER_NONALPHA;
	else if (m_Alpha[1].GetCheck() == TRUE)
		m_eRenderType = Engine::RENDER_ALPHA;

	// POSITION XYZ
	if (m_SelectPos[0].GetCheck() == TRUE)
		m_CheckPos = 0;
	else if (m_SelectPos[1].GetCheck() == TRUE)
		m_CheckPos = 1;
	else if (m_SelectPos[2].GetCheck() == TRUE)
		m_CheckPos = 2;
}


void CCharacterTool::OnBnClickedScalePosSet()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	map<_wstr, CGameObject*> pGameObject = pLayer->second->Get_GameObjectMap();

	auto pObject = find_if(pGameObject.begin(), pGameObject.end(), CTag_Finder(m_Texture));
	if (pObject == pGameObject.end())
		return;

	dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Set_Pos(_vec3(m_PositionX, m_PositionY, m_PositionZ));
	dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Set_Scale(_vec3(m_ScaleX, m_ScaleY, m_ScaleZ));

	UpdateData(FALSE);
}

void CCharacterTool::OnBnClickedSelectDelete()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	map<_wstr, CGameObject*> pGameObject = pLayer->second->Get_GameObjectMap();

	auto pObject = find_if(pGameObject.begin(), pGameObject.end(), CTag_Finder(m_Texture));
	if (pObject == pGameObject.end())
		return;

	dynamic_cast<CToolObject*>(pObject->second)->Set_Dead();
	m_ObjectList.DeleteString(m_TextureIndex);

	UpdateData(FALSE);
}

void CCharacterTool::OnBnClickedSelectCreate()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	Engine::CGameObject*		pGameObject = nullptr;

	auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == pView->m_mapLayer.end())
		return;
	map<_wstr, CGameObject*> GameObjectMap = pLayer->second->Get_GameObjectMap();

	auto pObject = find_if(GameObjectMap.begin(), GameObjectMap.end(), CTag_Finder(m_Texture));
	if (pObject == GameObjectMap.end())
		return;

	CString strTemp = m_Texture;
	CString strInt;
	int iIndex = 0;

	for (_int i = 0; i < strTemp.GetLength(); ++i)
	{
		if (isdigit(strTemp[strTemp.GetLength() - 1]) != 0) // 숫자인 경우
		{
			strInt = strTemp[strTemp.GetLength() - 1];
			strTemp = strTemp.Left(strTemp.GetLength() - 1);

			if (i == 0)
				iIndex += _ttoi(strInt);
			else if (i == 1)
				iIndex += _ttoi(strInt) * 10;
			else if (i == 2)
				iIndex += _ttoi(strInt) * 100;
		}
		else // 숫자가 아닌경우
		{
			break;
		}
	}
	//strInt = strTemp[strTemp.GetLength() - 1];
	//strTemp = strTemp.Left(strTemp.GetLength() - 1);

	while (true)
	{
		++iIndex;

		OBJINFO tObj;
		tObj.vPos = _vec3(0.f, 0.f, 0.f);
		tObj.vScale = _vec3(0.01f, 0.01f, 0.01f);
		tObj.vAngle = _vec3(0.f, 0.f, 0.f);
		lstrcpy(tObj.tagMesh, strTemp);
		tObj.eMeshType = m_eMeshType;
		tObj.eRenderType = m_eRenderType;

		wstring tagtemp = std::to_wstring(iIndex);
		strTemp = strTemp + tagtemp.c_str();

		auto findObj = find_if(GameObjectMap.begin(), GameObjectMap.end(), CTag_Finder(strTemp));
		if (findObj == GameObjectMap.end())
		{
			TCHAR* tagSearch = new TCHAR[strTemp.GetLength() + 1];
			lstrcpy(tagSearch, strTemp);

			// Create Object
			pGameObject = CToolObject::Create(pView->m_pGraphicDev, &tObj);
			NULL_CHECK_RETURN(pGameObject, );
			pLayer->second->Add_GameObject(tagSearch, pGameObject);

			m_vecObject.emplace_back(tagSearch);
			m_ObjectList.AddString(strTemp);
			break;
		}
	}

	UpdateData(FALSE);
}

void CCharacterTool::OnBnClickedObjectListSave()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
		if (pLayer == pView->m_mapLayer.end())
			return;
		map<_wstr, CGameObject*> GameObjectMap = pLayer->second->Get_GameObjectMap();

		for (int i = 0; i < m_ObjectList.GetCount(); ++i)
		{
			CString strFileName;
			m_ObjectList.GetText(i, strFileName);

			auto pObject = find_if(GameObjectMap.begin(), GameObjectMap.end(), CTag_Finder(strFileName));
			if (pObject == GameObjectMap.end())
				return;

			CString strTemp = strFileName.Left(strFileName.GetLength() - 1);

			OBJINFO tObj;
			tObj.vPos = dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Get_Position();
			tObj.vScale = dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Get_Scale();
			tObj.vAngle = dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Get_Angle();
			lstrcpy(tObj.tagMesh, strTemp);
			tObj.eMeshType = dynamic_cast<CToolObject*>(pObject->second)->m_eMesh;
			tObj.eRenderType = dynamic_cast<CToolObject*>(pObject->second)->m_eRender;

			WriteFile(hFile, &tObj, sizeof(OBJINFO), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}

void CCharacterTool::OnBnClickedObjectListLoad()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_OVERWRITEPROMPT, L"Data File(*.dat)|*.dat||", this);

	TCHAR szBuf[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuf);

	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	Engine::CGameObject*		pGameObject = nullptr;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		OBJINFO tObj;
		int iIndex = 0;

		while (true)
		{
			ReadFile(hFile, &tObj, sizeof(OBJINFO), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CString strFileName = tObj.tagMesh;
			wstring wstrtemp = std::to_wstring(iIndex);
			strFileName = strFileName + wstrtemp.c_str();

			auto pLayer = find_if(pView->m_mapLayer.begin(), pView->m_mapLayer.end(), CTag_Finder(L"GameObject"));
			if (pLayer == pView->m_mapLayer.end())
				return;
			map<_wstr, CGameObject*> GameObjectMap = pLayer->second->Get_GameObjectMap();

			auto pObject = find_if(GameObjectMap.begin(), GameObjectMap.end(), CTag_Finder(strFileName));
			if (pObject == GameObjectMap.end())
			{		
				TCHAR* tagSearch = new TCHAR[MAX_PATH];
				lstrcpy(tagSearch, strFileName);

				pGameObject = CToolObject::Create(pView->m_pGraphicDev, &tObj);
				NULL_CHECK_RETURN(pGameObject, );

				pLayer->second->Add_GameObject(tagSearch, pGameObject);
				m_vecObject.emplace_back(tagSearch);
				m_ObjectList.AddString(strFileName);
			}
			else
			{ 
				while (true)
				{
					++iIndex;
					strFileName = strFileName.Left(strFileName.GetLength() - 1);

					wstring tagtemp = std::to_wstring(iIndex);
					strFileName = strFileName + tagtemp.c_str();

					auto findObj = find_if(GameObjectMap.begin(), GameObjectMap.end(), CTag_Finder(strFileName));
					if (findObj == GameObjectMap.end())
					{
						TCHAR* tagSearch = new TCHAR[strFileName.GetLength() + 1];
						lstrcpy(tagSearch, strFileName);

						// Create Object
						pGameObject = CToolObject::Create(pView->m_pGraphicDev, &tObj);
						NULL_CHECK_RETURN(pGameObject, );

						pLayer->second->Add_GameObject(tagSearch, pGameObject);
						m_vecObject.emplace_back(tagSearch);
						m_ObjectList.AddString(strFileName);
						break;
					}
				}
				iIndex = 0;
			}
		}
		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}

void CCharacterTool::OnBnClickedAddMap()
{
	UpdateData(TRUE);
	m_Map = m_Texture;
	UpdateData(FALSE);
}
