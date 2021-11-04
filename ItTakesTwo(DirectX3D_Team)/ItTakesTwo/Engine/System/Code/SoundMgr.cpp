#include "SoundMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{

}

CSoundMgr::~CSoundMgr()
{
	Free();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

}

void CSoundMgr::PlaySoundW(TCHAR * pSoundKey, CHANNELID eID, const _float& fVolum)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound[m_iCurScene].begin(), m_mapSound[m_iCurScene].end
	(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound[m_iCurScene].end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{

		FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
		Set_Volum(fVolum, eID);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(TCHAR * pSoundKey, const _float& fVolum)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound[m_iCurScene].begin(), m_mapSound[m_iCurScene].end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound[m_iCurScene].end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[BGM]);
	Set_Volum(fVolum, BGM);
	//FMOD_Channel_SetVolume(m_pChannelArr[BGM], 0.5f);

	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}


void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}


void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::Load_SoundFile(const string tagPath,const RESOURCEID& eResourceID)
{
	if (!m_mapSound[eResourceID].empty())
		return;

	_finddata_t fd;
	string FilePath = tagPath;
	FilePath += "*.*";
	size_t handle = _findfirst(FilePath.c_str(), &fd);

	if (handle == 0)
		return;

	int iResult = 0;

	char szFullPath[MAX_PATH] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, tagPath.c_str());
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = int(strlen(fd.name) + 1);

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound[eResourceID].emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}


void CSoundMgr::Free()
{
	for (_int i = 0; i < RESOURCE_END; ++i)
	{

		for (auto& Mypair : m_mapSound[i])
		{
			delete[] Mypair.first;
			FMOD_Sound_Release(Mypair.second);
		}
		m_mapSound[i].clear();
	}

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
// 0 = silent, 1 = full
void Engine::CSoundMgr::Set_Volum(const _float& fVolum, CHANNELID eID)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolum);
}

_bool Engine::CSoundMgr::Is_Play_End(CHANNELID eID)
{
	FMOD_BOOL bCheck = false;
	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bCheck);
	return (_bool)!bCheck;

}

void Engine::CSoundMgr::Release_Sound(RESOURCEID eResourceID)
{
	StopAll();
	for (auto& Mypair : m_mapSound[eResourceID])
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound[eResourceID].clear();
}

void Engine::CSoundMgr::StopNPlaySound(TCHAR * pSoundKey, CHANNELID eID, const _float& fVolum)
{
	StopSound(eID);
	PlaySoundW(pSoundKey, eID, fVolum);
}
