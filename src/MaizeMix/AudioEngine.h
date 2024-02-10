#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <variant>
#include <memory>
#include <set>

#include "SoundReference.h"

namespace Maize::Mix {

	class AudioClip;
	class Music;

	enum class AudioState
	{
		Pause,
		Unpause,
		Mute,
		Unmute,
		Stop
	};

	class AudioEngine
	{
	 public:
		AudioClip CreateClip(const std::string& audioPath, bool stream);
		void DestroyClip(AudioClip& clip);

		uint8_t PlaySound(AudioClip& clip, float volume, float pitch, bool loop);
		uint8_t PlaySoundAtPosition(AudioClip& clip, float volume, float pitch, bool loop, float x, float y, float depth, float minDistance, float maxDistance);

		void UpdateAudioLoopState(uint8_t audioSourceID, bool loop);
		void UpdateAudioVolume(uint8_t audioSourceID, float volume);
		void UpdateAudioPitch(uint8_t audioSourceID, float pitch);
		void UpdateAudioPosition(uint8_t audioSourceID, float x, float y, float depth, float minDistance, float maxDistance);

		void SetAudioState(uint8_t audioSourceID, AudioState audioState);

		void SetListenerPosition(float x, float y, float depth);
		void SetGlobalVolume(float volume);

		void Update(float deltaTime);

	 private:
		struct SoundEventData
		{
			const uint8_t audioSourceID = 0;
			const float stopTime = 0;

			SoundEventData() = default;
			SoundEventData(int16_t clip, float stopTime)
				: stopTime(stopTime), audioSourceID(clip)
			{
			}

			bool operator<(const SoundEventData& other) const
			{
				if (stopTime == other.stopTime)
				{
					return audioSourceID < other.audioSourceID;
				}

				return stopTime < other.stopTime;
			}

		};

		struct Audio
		{
			std::variant<sf::Sound, std::shared_ptr<Music>> sound;
			const SoundEventData* event = nullptr;

			float previousVolume = 0;

			Audio() = default;
			Audio(const std::shared_ptr<Music>& audio, const SoundEventData* event)
				: sound(audio), event(event)
			{
			}
			Audio(const sf::Sound& audio, const SoundEventData* event)
				: sound(audio), event(event)
			{
			}
		};

	 private:
		float LimitVolume(float volume) const;
		bool HasHitMaxAudioSources() const;
		float GetPlayingOffset(const std::variant<sf::Sound, std::shared_ptr<Music>>& soundVariant);

		void PauseSound(Audio& soundData);
		void UnpauseSound(uint8_t audioSourceID, Audio& soundData);
		void MuteSound(Audio& soundData);
		void UnmuteSound(Audio& soundData);
		void StopSound(uint8_t audioSourceID, Audio& soundData);

		uint8_t PlayAudio(AudioClip& clip, float volume, float pitch, bool loop, float x = 0.0f, float y = 0.0f, float depth = 0.0f, float minDistance = 5.0f, float maxDistance = 10.0f);
		uint8_t PlayStreamedAudio(AudioClip& clip, float volume, float pitch, bool loop, float x = 0.0f, float y = 0.0f, float depth = 0.0f, float minDistance = 5.0f, float maxDistance = 10.0f);

		uint8_t GetNextID();
		void ReturnID(uint8_t audioSourceID);

	 private:
		sf::Time m_CurrentTime;

		std::unordered_map<size_t, sf::SoundBuffer> m_SoundBuffers;
		std::unordered_map<size_t, SoundReference> m_SoundReferences;

		std::unordered_map<uint8_t, Audio> m_CurrentPlayingSounds;
		std::set<SoundEventData> m_AudioEventQueue;

		std::vector<uint8_t> m_UnusedIDs;

		static constexpr const uint8_t c_MaxAudioEmitters = 250;
		static constexpr const uint8_t c_InvalidClip = 0;
		static constexpr const uint8_t c_InvalidAudioSource = 0;
	};

}