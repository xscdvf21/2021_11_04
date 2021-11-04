#pragma once

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

enum class SOUND_CHANNELID { BGM, PLAYER, ENEMY, BOSS, ENEMYDEAD, EFFECTA, EFFECTB, EFFECTC, UI, MAXCHANNEL };

class CSoundManager : public CBase
{
	DECLARE_SINGLETON(CSoundManager)

private:
	explicit CSoundManager();
	virtual ~CSoundManager() = default;

public:
	void Initialize();

public:
	void PlaySound(TCHAR* pSoundKey, SOUND_CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(SOUND_CHANNELID eID);
	void StopAll();

	virtual void Free() override;

private:
	void LoadSoundFile();


private:
	std::map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[(_uint)SOUND_CHANNELID::MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

};

END
#endif // !__SOUND_MANAGER_H__