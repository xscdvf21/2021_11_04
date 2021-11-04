#include "stdafx.h"
#include "TestMove.h"

#include "Export_Function.h"
#include "BoundingBox.h"

IMPLEMENT_SINGLETON(CTestMove)

CTestMove::CTestMove()
{
}


CTestMove::~CTestMove()
{
}

void CTestMove::Set(wstring tagName, _bool bTrigger)
{
	if (-1 == GetPrivateProfileInt(tagName.c_str(), L"POSITIONX", -1, L"../../Data/Test.ini"))
		WritePrivateProfileString(tagName.c_str(), L"POSITIONX", L"", L"../../Data/Test.ini");
	if (-1 == GetPrivateProfileInt(tagName.c_str(), L"POSITIONY", -1, L"../../Data/Test.ini"))
		WritePrivateProfileString(tagName.c_str(), L"POSITIONY", L"", L"../../Data/Test.ini");
	if (-1 == GetPrivateProfileInt(tagName.c_str(), L"POSITIONZ", -1, L"../../Data/Test.ini"))
		WritePrivateProfileString(tagName.c_str(), L"POSITIONZ", L"", L"../../Data/Test.ini");
	if (-1 == GetPrivateProfileInt(tagName.c_str(), L"ANGLEX", -1, L"../../Data/Test.ini"))
		WritePrivateProfileString(tagName.c_str(), L"ANGLEX", L"", L"../../Data/Test.ini");
	if (-1 == GetPrivateProfileInt(tagName.c_str(), L"ANGLEY", -1, L"../../Data/Test.ini"))
		WritePrivateProfileString(tagName.c_str(), L"ANGLEY", L"", L"../../Data/Test.ini");
	if (-1 == GetPrivateProfileInt(tagName.c_str(), L"ANGLEZ", -1, L"../../Data/Test.ini"))
		WritePrivateProfileString(tagName.c_str(), L"ANGLEZ", L"", L"../../Data/Test.ini");

	if (true == bTrigger)
	{
		if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERPOSX", -1, L"../../Data/Test.ini"))
			WritePrivateProfileString(tagName.c_str(), L"TRIGGERPOSX", L"", L"../../Data/Test.ini");
		if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERPOSY", -1, L"../../Data/Test.ini"))
			WritePrivateProfileString(tagName.c_str(), L"TRIGGERPOSY", L"", L"../../Data/Test.ini");
		if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERPOSZ", -1, L"../../Data/Test.ini"))
			WritePrivateProfileString(tagName.c_str(), L"TRIGGERPOSZ", L"", L"../../Data/Test.ini");
		if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERSCALEX", -1, L"../../Data/Test.ini"))
			WritePrivateProfileString(tagName.c_str(), L"TRIGGERSCALEX", L"", L"../../Data/Test.ini");
		if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERSCALEY", -1, L"../../Data/Test.ini"))
			WritePrivateProfileString(tagName.c_str(), L"TRIGGERSCALEY", L"", L"../../Data/Test.ini");
		//if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERSCALEZ", -1, L"../../Data/Test.ini"))
		//	WritePrivateProfileString(tagName.c_str(), L"TRIGGERSCALEZ", L"", L"../../Data/Test.ini");
		//if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERANGLEX", -1, L"../../Data/Test.ini"))
		//	WritePrivateProfileString(tagName.c_str(), L"TRIGGERANGLEX", L"", L"../../Data/Test.ini");
		//if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERANGLEY", -1, L"../../Data/Test.ini"))
		//	WritePrivateProfileString(tagName.c_str(), L"TRIGGERANGLEY", L"", L"../../Data/Test.ini");
		//if (-1 == GetPrivateProfileInt(tagName.c_str(), L"TRIGGERANGLEZ", -1, L"../../Data/Test.ini"))
		//	WritePrivateProfileString(tagName.c_str(), L"TRIGGERANGLEZ", L"", L"../../Data/Test.ini");
	}
}

void CTestMove::Move(CTransform * pTransform, const _float& fTimeDelta, wstring tagName)
{
	if (Engine::Key_Down(DIK_NUMPADPERIOD))
		m_bRotation = !m_bRotation;

	if (false == m_bRotation)
	{
		if (Engine::Key_Pressing(DIK_LEFT))
			pTransform->Move_Pos(&_vec3(1.0f, 0.0f, 0.0f), m_fSpeed, fTimeDelta);
		else if (Engine::Key_Pressing(DIK_RIGHT))
			pTransform->Move_Pos(&_vec3(-1.0f, 0.0f, 0.0f), m_fSpeed, fTimeDelta);
		else if (Engine::Key_Pressing(DIK_UP))
			pTransform->Move_Pos(&_vec3(0.0f, 0.0f, 1.0f), m_fSpeed, fTimeDelta);
		else if (Engine::Key_Pressing(DIK_DOWN))
			pTransform->Move_Pos(&_vec3(0.0f, 0.0f, -1.0f), m_fSpeed, fTimeDelta);
		else if (Engine::Key_Pressing(DIK_M))
			pTransform->Move_Pos(&_vec3(0.0f, -1.0f, 0.0f), m_fSpeed, fTimeDelta);
		else if (Engine::Key_Pressing(DIK_N))
			pTransform->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), m_fSpeed, fTimeDelta);
		else if (Engine::Key_Pressing(DIK_V))
			m_fSpeed += 0.5f;
		else if(Engine::Key_Pressing(DIK_B))
			m_fSpeed -= 0.5f;
	}
	else
	{
		if (Engine::Key_Pressing(DIK_LEFT))
			pTransform->Set_AddAngleX(m_fRotaAngle * fTimeDelta);
		else if (Engine::Key_Pressing(DIK_RIGHT))
			pTransform->Set_AddAngleX(-m_fRotaAngle * fTimeDelta);
		else if (Engine::Key_Pressing(DIK_UP))
			pTransform->Set_AddAngleZ(m_fRotaAngle * fTimeDelta);
		else if (Engine::Key_Pressing(DIK_DOWN))
			pTransform->Set_AddAngleZ(-m_fRotaAngle * fTimeDelta);
		else if (Engine::Key_Pressing(DIK_M))
			pTransform->Set_AddAngleY(-m_fRotaAngle * fTimeDelta);
		else if (Engine::Key_Pressing(DIK_N))
			pTransform->Set_AddAngleY(m_fRotaAngle * fTimeDelta);
		else if (Engine::Key_Pressing(DIK_V))
			m_fRotaAngle += 0.5f;
		else if (Engine::Key_Pressing(DIK_B))
			m_fRotaAngle -= 0.5f;
	}


	TCHAR szbuf[MAX_PATH];
	swprintf_s(szbuf, L"%f", pTransform->Get_Angle().x);
	WritePrivateProfileString(tagName.c_str(), L"ANGLEX", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pTransform->Get_Angle().y);
	WritePrivateProfileString(tagName.c_str(), L"ANGLEY", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pTransform->Get_Angle().z);
	WritePrivateProfileString(tagName.c_str(), L"ANGLEZ", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pTransform->Get_Position().x);
	WritePrivateProfileString(tagName.c_str(), L"POSITIONX", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pTransform->Get_Position().y);
	WritePrivateProfileString(tagName.c_str(), L"POSITIONY", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pTransform->Get_Position().z);
	WritePrivateProfileString(tagName.c_str(), L"POSITIONZ", szbuf, L"../../Data/Test.ini");
}

void CTestMove::MoveTrigger(CBoundingBox* pBox, const _float & fTimeDelta, wstring tagName)
{
	if (Engine::Key_Down(DIK_NUMPADPERIOD))
		m_bRotation = !m_bRotation;

	if (false == m_bRotation)
	{
		if (Engine::Key_Pressing(DIK_NUMPAD4))
			pBox->Move_Pos(_vec3(1.0f, 0.0f, 0.0f), fTimeDelta, m_fTriggerSpeed);
		else if (Engine::Key_Pressing(DIK_NUMPAD6))
			pBox->Move_Pos(_vec3(1.0f, 0.0f, 0.0f), fTimeDelta, -m_fTriggerSpeed);
		else if (Engine::Key_Pressing(DIK_NUMPAD8))
			pBox->Move_Pos(_vec3(0.0f, 0.0f, 1.0f), fTimeDelta, m_fTriggerSpeed);
		else if (Engine::Key_Pressing(DIK_NUMPAD5))
			pBox->Move_Pos(_vec3(0.0f, 0.0f, 1.0f), fTimeDelta, -m_fTriggerSpeed);
		else if (Engine::Key_Pressing(DIK_NUMPAD7))
			pBox->Move_Pos(_vec3(0.0f, 1.0f, 0.0f), fTimeDelta, m_fTriggerSpeed);
		else if (Engine::Key_Pressing(DIK_NUMPAD9))
			pBox->Move_Pos(_vec3(0.0f, 1.0f, 0.0f), fTimeDelta, -m_fTriggerSpeed);
		else if (Engine::Key_Pressing(DIK_NUMPADPLUS))
			m_fTriggerSpeed += 0.5f;
		else if (Engine::Key_Pressing(DIK_NUMPADMINUS))
			m_fTriggerSpeed -= 0.5f;
	}
	else
	{
		if (Engine::Key_Pressing(DIK_NUMPAD4))
			pBox->Add_ScaleX(0.1f);
		else if (Engine::Key_Pressing(DIK_NUMPAD6))
			pBox->Add_ScaleX(-0.1f);
		else if (Engine::Key_Pressing(DIK_NUMPAD8))
			pBox->Add_ScaleZ(0.1f);
		else if (Engine::Key_Pressing(DIK_NUMPAD5))
			pBox->Add_ScaleZ(-0.1f);
		else if (Engine::Key_Pressing(DIK_NUMPAD7))
			pBox->Add_ScaleY(0.1f);
		else if (Engine::Key_Pressing(DIK_NUMPAD9))
			pBox->Add_ScaleY(-0.1f);
		//else if (Engine::Key_Pressing(DIK_NUMPADPLUS))
		//	m_fRotaAngle += 0.0f;
		//else if (Engine::Key_Pressing(DIK_NUMPADMINUS))
		//	m_fRotaAngle -= 1.0f;
	}

	TCHAR szbuf[MAX_PATH];
	swprintf_s(szbuf, L"%f", pBox->Get_Scale().x * 0.5f);
	WritePrivateProfileString(tagName.c_str(), L"TRIGGERSCALEX", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pBox->Get_Scale().y * 0.5f);
	WritePrivateProfileString(tagName.c_str(), L"TRIGGERSCALEY", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pBox->Get_Scale().z * 0.5f);
	WritePrivateProfileString(tagName.c_str(), L"TRIGGERSCALEZ", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pBox->Get_Pos().x);
	WritePrivateProfileString(tagName.c_str(), L"TRIGGERPOSX", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pBox->Get_Pos().y);
	WritePrivateProfileString(tagName.c_str(), L"TRIGGERPOSY", szbuf, L"../../Data/Test.ini");
	swprintf_s(szbuf, L"%f", pBox->Get_Pos().z);
	WritePrivateProfileString(tagName.c_str(), L"TRIGGERPOSZ", szbuf, L"../../Data/Test.ini");
}

void CTestMove::Free()
{
}
