//
//  AudioManager.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "AudioManager.h"
#include "ResourceManager.h"
#include "ProjectConfig.h"
#include "Engine.h"

#define MUSIC "music"
#define COMBAT "combat"
#define STEALTH "stealth"

#include "FMOD/lowlevel/fmod.hpp"
#include "FMOD/studio/fmod_studio.hpp"

Implementation *implementation = nullptr;
AudioManager::AudioManager(void) {
	
	m_musicName = "";
	m_stealthName = "";
	m_combatName = "";
	m_audioClipRampQueue[MUSIC] = AudioClipRamp();
	m_audioClipRampQueue[COMBAT] = AudioClipRamp();
	m_audioClipRampQueue[STEALTH] = AudioClipRamp();
}


AudioManager::~AudioManager(void)
{
	Shutdown();
}

void AudioManager::HandleEvent(Event * event)
{
	if (event->GetEventType()==EEventType::OnPause)
	{
		m_masterChannel->setVolume(0.1f);
	}
	else if (event->GetEventType() == EEventType::OnResume || event->GetEventType() == EEventType::Maximized)
	{
		m_masterChannel->setVolume(1.0f);
	}
	else if (event->GetEventType() == EEventType::Minimized)
	{
		m_masterChannel->setVolume(0.0f);
	}
}


Implementation::Implementation() {
	studioSystem = NULL;

	AudioManager::ErrorCheck(FMOD::Studio::System::create(&studioSystem));
	AudioManager::ErrorCheck(studioSystem->initialize(1024, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
	system = NULL;
	AudioManager::ErrorCheck(studioSystem->getLowLevelSystem(&system));
}

Implementation::~Implementation() {
	AudioManager::ErrorCheck(studioSystem->unloadAll());
	AudioManager::ErrorCheck(studioSystem->release());
}

void Implementation::Update(void) {

	
	std::vector<ChannelMap::iterator> stoppedChannel;
	for (auto it = PiResource->GetChannelMap().begin(), itEnd = PiResource->GetChannelMap().end(); it != itEnd; ++it)
	{
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			stoppedChannel.push_back(it);
		}
	}
	for (auto& it : stoppedChannel)
	{
		PiResource->GetChannelMap().erase(it);
	}
	AudioManager::ErrorCheck(studioSystem->update());
}


void AudioManager::Initialize() {

	implementation = new Implementation();
	SetMasterGroup();
}

void AudioManager::Update(float a_dt) {
	
	
	implementation->Update();

	for (auto &val : m_audioClipRampQueue) {

		auto& clip = val.second;
		if (clip.active) {

			clip.remainingFadeTime -= a_dt;
			if (clip.remainingFadeTime > 0.0f) {

				float t = 1.0f - clip.remainingFadeTime / clip.remainingFadeTime;

				//////////////////////// USE ROHITS LERP //////////////////////////////

			   //	clip.fadeOutGroup->setVolume(util::lerp((clip.start_volume_dB), (clip.end_volume_dB), util::SmoothStep2(t)));

			}
			else {
				clip.active = false;
				//audio_clip_ramp_queue.erase(val.first);
			}
		}
	}
}

void AudioManager::RampAudioGroup(FMOD::ChannelGroup* a_fadeGroup, float a_fadeTime, float a_StartVoldB, float a_endVoldB) {

	void* data;
	a_fadeGroup->getUserData(&data);

	std::string name((char*)data);
	auto &clip = m_audioClipRampQueue[name];
	if (!clip.active) {
		clip.active = true;
		clip.fadeOutGroup = a_fadeGroup;
		clip.totalFadeTime = a_fadeTime;
		clip.remainingFadeTime = a_fadeTime;

		float volume;
		clip.fadeOutGroup->getVolume(&volume);

		clip.startVolumedB = (a_StartVoldB)* volume;
		clip.endVolumedB = (a_endVoldB)* volume;
	}

}

void AudioManager::SetMasterGroup(void) {
	implementation->result = implementation->system->createChannelGroup("ALL_SFX", &m_allSfx);
	implementation->result = implementation->system->createChannelGroup("ALL_BG", &m_backgroundMusic);
	implementation->result = implementation->system->createChannelGroup("ALL_STEALTH", &m_stealthMusic);
	implementation->result = implementation->system->createChannelGroup("ALL_COMBAT", &m_combatMusic);

	implementation->result = implementation->system->getMasterChannelGroup(&m_masterChannel);

	implementation->result = m_masterChannel->addGroup(m_allSfx);
	implementation->result = m_masterChannel->addGroup(m_backgroundMusic);

	implementation->result = m_masterChannel->addGroup(m_stealthMusic);
	implementation->result = m_masterChannel->addGroup(m_combatMusic);

	m_backgroundMusic->setUserData((void*)MUSIC);
	m_stealthMusic->setUserData((void*)STEALTH);
	m_combatMusic->setUserData((void*)COMBAT);

}

void AudioManager::AssignChannelGroup(FMOD::Channel &a_channelName, FMOD::ChannelGroup &a_groupName) {
	implementation->result = a_channelName.setChannelGroup(&a_groupName);
}


int AudioManager::PlaySounds(const string& audioName, const vec3 pos, float volume_dB, bool iSSfx, bool isStealth, bool isCombat) {
	//TODO:remove this after dev
	if (PiResource->GetConfig()->m_isMute)
	{
		return 0;
	}
	float frequency;
	int channel_id = implementation->nextChannelID++;
	FMOD::Sound *sound = PiResource->GetSoundFromMap(audioName);

	if (iSSfx) {

		FMOD::Channel* channel = nullptr;
		AudioManager::ErrorCheck(implementation->system->playSound(sound, nullptr, true, &channel));
		if (channel)
		{
			FMOD_MODE curr_mode;
			sound->getMode(&curr_mode);

			FMOD_VECTOR position = VectorToFmod(pos);
			AudioManager::ErrorCheck(channel->set3DAttributes(&position, nullptr));

			AudioManager::ErrorCheck(channel->setVolume(dbToVolume(volume_dB)));


			AssignChannelGroup(*channel, *m_allSfx);

			PiResource->GetChannelMap()[channel_id] = channel;

			PiResource->GetChannelMap()[channel_id]->getFrequency(&frequency);

			m_frequencyFluctuation = (float)(rand() % -10000) + 10000.0f;

			PiResource->GetChannelMap()[channel_id]->getPitch(&m_pitch);

			m_pitchFluctuation = (float)(rand() % 1000) + 1.0f;
			m_pitchFluctuation /= 9200.0f;

			PiResource->GetChannelMap()[channel_id]->setPitch(m_pitch + m_pitchFluctuation);

			frequency += m_frequencyFluctuation;

			PiResource->GetChannelMap()[channel_id]->setFrequency(frequency);

			AudioManager::SetMinMaxSoundDistance(*channel, 5.0f, 100.0f);

			//LOG("Storing frequency for channel id : %d ->%f", channel_id,f);
			implementation->channelFrequencies[channel_id] = frequency;

			AudioManager::ErrorCheck(channel->setPaused(false));

		}
	}
	else if (isStealth) {

		//if (audioName == stealth_name_) {
		//	return 0;
		//}
		m_stealthName = audioName;
		AudioManager::ErrorCheck(m_stealthMusic->stop());
		AudioManager::ErrorCheck(implementation->system->playSound(sound, nullptr, true, &m_stealthChannel));

		if (m_stealthChannel)
		{
			FMOD_MODE curr_mode;
			sound->getMode(&curr_mode);

			AudioManager::ErrorCheck(m_stealthChannel->setVolume(dbToVolume(volume_dB)));
			AudioManager::ErrorCheck(m_stealthChannel->setPaused(false));

			AssignChannelGroup(*m_stealthChannel, *m_stealthMusic);

			int index;
			m_stealthChannel->getIndex(&index);
			PiResource->GetChannelMap()[index] = m_stealthChannel;

			m_stealthChannel->getFrequency(&frequency);

			//LOG("Storing frequency for channel id : %d ->%f", channel_id,f);
			implementation->channelFrequencies[channel_id] = frequency;
		}
	}
	else if (isCombat) {

		///	if (audioName == combat_name_) {
			//	return 0;
			//}
		m_combatName = audioName;
		AudioManager::ErrorCheck(m_combatMusic->stop());
		AudioManager::ErrorCheck(implementation->system->playSound(sound, nullptr, true, &m_combatChannel));

		if (m_combatChannel)
		{
			FMOD_MODE curr_mode;
			sound->getMode(&curr_mode);

			AudioManager::ErrorCheck(m_combatChannel->setVolume(dbToVolume(volume_dB)));
			AudioManager::ErrorCheck(m_combatChannel->setPaused(false));

			AssignChannelGroup(*m_combatChannel, *m_combatMusic);

			PiResource->GetChannelMap()[channel_id] = m_combatChannel;

			PiResource->GetChannelMap()[channel_id]->getFrequency(&frequency);

			//LOG("Storing frequency for channel id : %d ->%f", channel_id,f);
			implementation->channelFrequencies[channel_id] = frequency;
		}
	}
	else {

		if (audioName == m_musicName) {
			return 0;
		}

		m_musicName = audioName;
		AudioManager::ErrorCheck(m_backgroundMusic->stop());
		AudioManager::ErrorCheck(implementation->system->playSound(sound, m_backgroundMusic, true, &m_musicChannel));

		if (m_musicChannel)
		{
			FMOD_MODE curr_mode;
			sound->getMode(&curr_mode);

			AudioManager::ErrorCheck(m_musicChannel->setVolume(dbToVolume(volume_dB)));
			AudioManager::ErrorCheck(m_musicChannel->setPaused(false));

			AssignChannelGroup(*m_musicChannel, *m_backgroundMusic);

			PiResource->GetChannelMap()[channel_id] = m_musicChannel;

			PiResource->GetChannelMap()[channel_id]->getFrequency(&frequency);

			//LOG("Storing frequency for channel id : %d ->%f", channel_id,f);
			implementation->channelFrequencies[channel_id] = frequency;
		}
	}

	return channel_id;
}


void AudioManager::PauseAudio() {

	m_masterChannel->setPaused(true);
}

void AudioManager::UnpauseAudio() {

	m_masterChannel->setPaused(false);
}

bool AudioManager::isMusicPlaying()
{
	bool res;
	 m_masterChannel->getPaused(&res);
	 return res;
}

void AudioManager::MuteMusic(bool sfx)
{
	if (sfx)
	{
		//all_sfx->getMute(&sfx);
		//m_allSfx->setVolume(0.0f);
		m_backgroundMusic->setVolume(0.0f);
	}
	else
	{
		//m_allSfx->setVolume(1.0f);
		m_backgroundMusic->setVolume(1.0f);
	}
}

void AudioManager::MuteMasterVolume(bool mute, bool sfx, bool music) {
	//master_channel->getMute(&mute);
	//master_channel->setMute(!mute);
	

	if (music)
	{
		//PauseAudio();
		//	stealth_music->getMute(&music);
		//m_stealthMusic->setMute(music);

		////combat_music->getMute(&music);
		//m_combatMusic->setMute(music);


		/////background_music->getMute(&music);
		//m_backgroundMusic->setMute(music);
		m_masterChannel->setVolume(0.0f);
	}
	else
	{
		m_masterChannel->setVolume(1.0f);
	}
}

void AudioManager::SetChannel3dPosition(int a_channelID, const vec3 a_pos) {
	auto found_it = PiResource->GetChannelMap().find(a_channelID);
	if (found_it == PiResource->GetChannelMap().end())
		return;

	FMOD_VECTOR position = VectorToFmod(a_pos);
	AudioManager::ErrorCheck(found_it->second->set3DAttributes(&position, NULL));
}

void AudioManager::SetChannelVolume(int channel_id, float volume_dB)
{
	auto found_it = PiResource->GetChannelMap().find(channel_id);
	if (found_it == PiResource->GetChannelMap().end())
		return;

	AudioManager::ErrorCheck(found_it->second->setVolume(dbToVolume(volume_dB)));
}

void AudioManager::SetMasterVolume(float a_volume_dB) {
	m_masterChannel->setVolume(dbToVolume(a_volume_dB));
}

void AudioManager::SetSlowMo(float a_timescale) {
	int channel_id = 0;
	float frequency;

	/*master_channel->getPitch(&pitch);
	master_channel->setPitch(a_dt*pitch);*/
	for (auto it : PiResource->GetChannelMap())
	{
		frequency = implementation->channelFrequencies[channel_id];
		//UPDATE_LOG(100,"Storing frequency for channel id : %d ->%f", channel_id, frequency);
		if (a_timescale > 1.0f)
			a_timescale = 1.0f;
		AudioManager::ErrorCheck(it.second->setFrequency(frequency * a_timescale));
		channel_id++;
	}
}

void AudioManager::Set3dListenerAndOrientation(const vec3& a_position, const vec3& a_look, const vec3& a_up, const vec3& a_velocity) {
	int listener = 0;
	implementation->system->set3DListenerAttributes(listener, &VectorToFmod(a_position), &VectorToFmod(a_velocity), &VectorToFmod(a_look), &VectorToFmod(a_up));
}


void AudioManager::SetMinMaxSoundDistance(FMOD::Channel &a_channel_id, float a_min_distance, float a_max_distance)
{
	a_channel_id.set3DMinMaxDistance(a_min_distance, a_max_distance);
}


void AudioManager::LoadBank(const std::string& a_bankName, FMOD_STUDIO_LOAD_BANK_FLAGS a_flags) {
	auto found_it = implementation->banks.find(a_bankName);
	if (found_it != implementation->banks.end())
		return;
	FMOD::Studio::Bank* bank;
	AudioManager::ErrorCheck(implementation->studioSystem->loadBankFile(a_bankName.c_str(), a_flags, &bank));
	if (bank)
	{
		implementation->banks[a_bankName] = bank;
	}
}

void AudioManager::LoadEvent(const std::string & a_eventName) {
	auto found_it = implementation->events.find(a_eventName);
	if (found_it != implementation->events.end())
		return;
	FMOD::Studio::EventDescription* eventDescription = NULL;
	AudioManager::ErrorCheck(implementation->studioSystem->getEvent(a_eventName.c_str(), &eventDescription));
	if (eventDescription)
	{
		FMOD::Studio::EventInstance *eventInstance = NULL;
		AudioManager::ErrorCheck(eventDescription->createInstance(&eventInstance));
		if (eventInstance)
		{
			implementation->events[a_eventName] = eventInstance;
		}
	}
}

FMOD::System * AudioManager::GetSystem(void)
{
	return implementation->system;
}

void AudioManager::PlayEvent(const string &a_eventName) {
	auto found_it = implementation->events.find(a_eventName);
	if (found_it == implementation->events.end())
	{
		LoadEvent(a_eventName);
		found_it = implementation->events.find(a_eventName);
		if (found_it == implementation->events.end())
			return;
	}
	found_it->second->start();
}

void AudioManager::StopEvent(const string &a_eventName, bool a_immediate) {
	auto found_it = implementation->events.find(a_eventName);
	if (found_it == implementation->events.end())
		return;

	FMOD_STUDIO_STOP_MODE mode;
	mode = a_immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	AudioManager::ErrorCheck(found_it->second->stop(mode));
}


bool AudioManager::IsEventPlaying(const string &a_eventName) const {
	auto found_it = implementation->events.find(a_eventName);
	if (found_it == implementation->events.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE *state = NULL;
	if (found_it->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING)
	{
		return true;
	}
	return false;
}


void AudioManager::GetEventParameter(const string &a_eventName, const string &a_parameterName, float *a_parameter) {
	auto found_it = implementation->events.find(a_eventName);
	if (found_it == implementation->events.end())
		return;

	FMOD::Studio::ParameterInstance * pParameter = NULL;
	AudioManager::ErrorCheck(found_it->second->getParameter(a_parameterName.c_str(), &pParameter));
	AudioManager::ErrorCheck(pParameter->getValue(a_parameter));
}

void AudioManager::SetEventParameter(const string &a_eventName, const string &a_parameterName, float a_value) {
	auto found_it = implementation->events.find(a_eventName);
	if (found_it == implementation->events.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	AudioManager::ErrorCheck(found_it->second->getParameter(a_parameterName.c_str(), &pParameter));
	AudioManager::ErrorCheck(pParameter->setValue(a_value));
}


FMOD_VECTOR AudioManager::VectorToFmod(const vec3 a_position) {
	FMOD_VECTOR vector;
	vector.x = a_position.x;
	vector.y = a_position.y;
	vector.z = a_position.z;
	return vector;
}



float AudioManager::dbToVolume(float a_dB) {
	return powf(10.0f, 0.05f * a_dB);
}

float AudioManager::VolumeTodB(float a_volume) {
	return 20.0f * log10f(a_volume);
}


int AudioManager::ErrorCheck(FMOD_RESULT a_result) {
	if (a_result != FMOD_OK)
	{
		printf("FMOD ERROR %d", a_result);
		return 1;
	}
	return 0;
}

void AudioManager::Shutdown() {
	delete implementation;
}