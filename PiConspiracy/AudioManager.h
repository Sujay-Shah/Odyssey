//
//  AudioManager.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
//------------------------------------------------
//THIS PART NEEDS TO BE THERE
//IT'S THE FORWARD DECLARATION
//   -ZOHEB
namespace FMOD
{
	class Channel;
	class ChannelGroup;
	class Sound;
	namespace Studio
	{
		class EventInstance;
		class Bank;
		class System;
	}
	class System;
}
enum FMOD_RESULT : int32_t;
typedef uint32_t FMOD_STUDIO_LOAD_BANK_FLAGS;
struct FMOD_VECTOR;
////------------------------------------------------

#include <map>
#include <string>

struct Implementation {
	Implementation();
	~Implementation();

	void Update();

	FMOD::Studio::System *studioSystem;
	FMOD::System* system;
	FMOD_RESULT result;

	int nextChannelID = 0;

	typedef std::map<string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;
	typedef std::map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef std::map<string, FMOD::Studio::Bank*> BankMap;
	typedef std::map<int, float> FrequencyMap;

	BankMap banks;
	EventMap events;
	FrequencyMap channelFrequencies;
	FrequencyMap maxChannelFrequencies;
};

struct AudioClipRamp {

	bool active;
	FMOD::ChannelGroup* fadeOutGroup;
	float startVolumedB;
	float endVolumedB;
	float totalFadeTime;
	float remainingFadeTime;

	AudioClipRamp() {
		active = false;
		fadeOutGroup = nullptr;
	}
};


class AudioManager : public Singleton<AudioManager>
{
private:
	int listener = 0;

public:

	FMOD::ChannelGroup *m_masterChannel;
	FMOD::ChannelGroup *m_allSfx;
	//FMOD::ChannelGroup *all_music;
	FMOD::ChannelGroup *m_backgroundMusic;
	FMOD::ChannelGroup *m_stealthMusic;
	FMOD::ChannelGroup *m_combatMusic;

	//std::vector<AudioClipRamp> audio_clip_ramp_queue;
	std::unordered_map<std::string, AudioClipRamp> m_audioClipRampQueue;

	FMOD::System *m_masterSystem;

	FMOD::Channel* m_musicChannel = nullptr;
	FMOD::Channel* m_stealthChannel = nullptr;
	FMOD::Channel* m_combatChannel = nullptr;

	float m_frequencyFluctuation;
	float m_pitch, m_pitchFluctuation;

	float m_musicfreq, m_stealthfreq, m_combatFreq;

	std::string m_musicName;
	std::string m_stealthName;
	std::string m_combatName;

public:
	AudioManager(void);
	~AudioManager(void);

	void HandleEvent(Event* event);

	//Initialize the AudioManager 
	void Initialize();

	//Update AudioManager
	void Update(float a_dt);

	//Assign a Master Group that contains all SFX and MUSIC channel groups
	void SetMasterGroup(void);

	//Assign a Channel group to channel
	void AssignChannelGroup(FMOD::Channel &a_channelName, FMOD::ChannelGroup &a_group);

	//Set 3D position to a channel
	void SetChannel3dPosition(int a_ChannelID, const glm::vec3 a_pos);

	//Set Volume of a channel
	void SetChannelVolume(int a_channelID, float a_volumedB);

	//Set the Volume of all Audio
	void SetMasterVolume(float a_volumedB);

	//Shut the AudioManager
	static void Shutdown(void);

	//Error Checking function for FMOD calls
	static int ErrorCheck(FMOD_RESULT a_result);

	//Load Bank created in the studio
	void LoadBank(const std::string& a_bankName, FMOD_STUDIO_LOAD_BANK_FLAGS a_flags);

	//Load Events created in studio
	void LoadEvent(const std::string& a_eventName);

	//Load the sound and set properties to 
	//3D or not, Looping or not, Streaming or not
	//void LoadSound(const string &a_audioValue, const string &a_audioName, bool a_isAudio3D = true, bool a_isAudioLooping = false, bool a_isAudioStreaming = false);

	FMOD::System* GetSystem(void);

	//Release all sounds
	//void UnLoadSound(const string &a_audioName);

	//Set Arrtibutes for 3D listener.
	//Upto 8
	void Set3dListenerAndOrientation(const glm::vec3& a_position, const glm::vec3& a_look, const glm::vec3& a_up, const glm::vec3&a_velocity);

	//Set Max and Min distance for the sound's attenuation
	void SetMinMaxSoundDistance(FMOD::Channel &a_channelID, float a_mindDistance, float a_maxDistance);

	//Play selected sound
	//Set True if sound is an SFX
	int PlaySounds(const string &a_audioName, const glm::vec3 a_pos, float a_volume = 0.0f, bool a_isSfx = true, bool a_isStealth = false, bool a_isCombat = false);

	//Pause all audio
	void PauseAudio(void);

	void UnpauseAudio(void);

	//Turn on Slow Motion Audio Effects
	void SetSlowMo(float a_dt);

	//Play an Event from the bank
	void PlayEvent(const string &a_eventName);

	//Stop an Event
	void StopEvent(const string &a_eventName, bool a_immediate = false);

	//Get an Event's parameters
	void GetEventParameter(const string &a_eventName, const string &a_eventParameter, float* a_parameter);

	//Set an event's parameters
	void SetEventParameter(const string &a_eventName, const string &a_parameterName, float a_value);

	void RampAudioGroup(FMOD::ChannelGroup* a_fadeGroup, float a_fadeTime, float a_startVol = 0.0f, float a_endVol = 1.0f);

	//void FadeVolumeOut(FMOD::ChannelGroup fadeOutGroup);

	//void FadeVolumeIn(FMOD::ChannelGroup fadeInGroup);

	//Mute all Audio
	void MuteMasterVolume(bool a_mute = true, bool a_Sfx = false, bool a_music = false);

	//Check if an event is playing
	bool IsEventPlaying(const string &a_eventName) const;

	//Convert entered dB volume to FMOD volume
	float dbToVolume(float a_dB);

	//Conver FMOD volume to dB volume
	float VolumeTodB(float a_volume);

	//Convert glm vector to FMOD vector
	FMOD_VECTOR VectorToFmod(const glm::vec3 a_position);

	bool isMusicPlaying();

	void MuteMusic(bool);
	//test
};

