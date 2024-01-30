#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <memory>

class AudioClip;

class AudioEngine
{
 public:
	void PlaySound(const AudioClip& clip, float volume, float pitch, bool loop);
	void StopSound(const AudioClip& clip);

	void Update(float currentTime);

 private:
	struct Sound
	{
		sf::Sound sound;
		std::shared_ptr<const sf::SoundBuffer> buffer = nullptr;

		Sound() = default;
		Sound(const sf::Sound& audio, const std::shared_ptr<const sf::SoundBuffer>& buffer)
		: sound(audio), buffer(buffer)
		{
		}
	};

	struct SoundEventData
	{
		int16_t clipID = -1;
		float stopTime = 0;
		//float previousVolume = 0;

		SoundEventData() = default;
		SoundEventData(int16_t clip, float stopTime)
			: stopTime(stopTime), clipID(clip)
		{
		}

		bool operator<(const SoundEventData& other) const
		{
			return stopTime < other.stopTime;
		}
	};

 	uint8_t GetCurrentAudioCount()
	{
		 return 0;
	}

 private:
	std::unordered_map<int16_t, Sound> m_CurrentPlayingSounds;
 	std::set<SoundEventData> m_SoundEventQueue;

	const uint8_t c_MaxAudioEmitters = 250;
};