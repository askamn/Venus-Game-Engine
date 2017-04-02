#include "Audio.h"

namespace Syndicate {

Audio::Audio(const std::string& path) :
	m_AudioFilePath(path),
	m_Playing(false),
	m_Audio(nullptr),
	m_AudioHandle(nullptr),
	m_Volume(100)
{
	m_AudioFileFormat = m_AudioFilePath.substr(m_AudioFilePath.size() - 3, 3);

#if !SYNDICATE_USE_BUFFERED_AUDIO
	m_Audio = gau_load_sound_file(m_AudioFilePath.c_str(), m_AudioFileFormat.c_str());
#endif
}

Audio::Audio(const std::string& name, const std::string& path, bool loop) : 
	m_AudioFileName(name),
	m_AudioFilePath(path),
	m_Loop(loop),
	m_Playing(false),
	m_Audio(nullptr),
	m_AudioHandle(nullptr),
	m_Volume(100)
{
	m_AudioFileFormat = m_AudioFilePath.substr(m_AudioFilePath.size() - 3, 3);

#if !SYNDICATE_USE_BUFFERED_AUDIO
	m_Audio = gau_load_sound_file(m_AudioFilePath.c_str(), m_AudioFileFormat.c_str());
#endif
}

ga_Handle* Audio::createHandle(void* in_context, gau_SampleSourceLoop** out_loopSrc)
{
#if SYNDICATE_USE_BUFFERED_AUDIO
	m_AudioHandle = gau_create_handle_buffered_file(AudioManager::i()->getMixer(), AudioManager::i()->getStreamManager(), m_AudioFilePath.c_str(), m_AudioFileFormat.c_str(), &AudioManager::setFlagAndDestroyOnFinish, in_context, out_loopSrc);
#else
	m_AudioHandle = gau_create_handle_sound(AudioManager::i()->getMixer(), m_Audio, &AudioManager::i()->setFlagAndDestroyOnFinish, in_context, out_loopSrc);
#endif

	return m_AudioHandle;
}

void Audio::destroyHandle()
{
	ga_handle_destroy(m_AudioHandle);
}

void Audio::Play()
{
	m_Over = false;
	m_Playing = true;

	// Only create the handle if it does not exist
	if (m_AudioHandle == nullptr)
	{
		// NULL means no looping
		this->createHandle(&m_Over, NULL);
	}

	ga_handle_play(m_AudioHandle);
}

void Audio::Pause()
{
	this->Stop();
}

void Audio::Stop()
{
	if (m_Playing && m_AudioHandle != nullptr)
	{
		ga_handle_stop(m_AudioHandle);
		m_Playing = false;
	}
}

void Audio::Loop()
{
	m_Loop = true;
	m_Over = false;
	m_Playing = true;

	gau_SampleSourceLoop* loopSrc = 0;
	gau_SampleSourceLoop** pLoopSrc = &loopSrc;

	// Only create the handle if it does not exist
	if (m_AudioHandle == nullptr)
	{
		this->createHandle(&m_Over, pLoopSrc);
	}

	ga_handle_play(m_AudioHandle);
}

void Audio::Loop(unsigned int times)
{
	m_Over = false;
	m_Playing = true;
	m_LoopTimes = times - 1;
	m_Loop = true;

	// Only create the handle if it does not exist
	if (m_AudioHandle == nullptr)
	{
		this->createHandle(&m_Over, NULL);
	}

	ga_handle_play(m_AudioHandle);
}

void Audio::Update()
{
	// Don't need to update if it's not playing
	if (!m_Playing)
	{
		return;
	}

	gau_manager_update(AudioManager::i()->getSoundManager());
	gc_thread_sleep(1);

	if (m_Loop)
	{
		int current = ga_handle_tell(m_AudioHandle, GA_TELL_PARAM_CURRENT);
		int end = ga_handle_tell(m_AudioHandle, GA_TELL_PARAM_TOTAL);

		if (current == end)
		{
			m_LoopTimes--;

			this->destroyHandle();
			this->createHandle(&m_Over, NULL);
			ga_handle_play(m_AudioHandle);
		}
	}

	if (m_Over)
	{
		m_Playing = false;
	}
}

void Audio::VolumeUp(int value)
{
	// Invalid value provided
	if (value > SYNDICATE_AUDIO_MAX_VOLUME || value < SYNDICATE_AUDIO_MIN_VOLUME)
	{
		return;
	}

	m_Volume += value;

	// Overflow, reset to max value
	if (m_Volume > SYNDICATE_AUDIO_MAX_VOLUME)
	{
		m_Volume = SYNDICATE_AUDIO_MAX_VOLUME;
	}

	ga_handle_setParamf(m_AudioHandle, GA_HANDLE_PARAM_GAIN, (float)(m_Volume / SYNDICATE_AUDIO_MAX_VOLUME_F));
}

void Audio::VolumeDown(int value)
{
	// Invalid value provided
	if (value > SYNDICATE_AUDIO_MAX_VOLUME || value < SYNDICATE_AUDIO_MIN_VOLUME)
	{
		return;
	}

	m_Volume -= value;

	// Underflow, reset to max value
	if (m_Volume < SYNDICATE_AUDIO_MIN_VOLUME)
	{
		m_Volume = SYNDICATE_AUDIO_MIN_VOLUME;
	}

	ga_handle_setParamf(m_AudioHandle, GA_HANDLE_PARAM_GAIN, (float)(m_Volume / SYNDICATE_AUDIO_MAX_VOLUME_F));
}

Audio::~Audio()
{
#if !SYNDICATE_USE_BUFFERED_AUDIO
	ga_sound_release(m_Audio);
#endif
}

}