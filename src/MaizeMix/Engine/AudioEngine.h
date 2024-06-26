#pragma once

#include <SFML/Audio.hpp>

#include <unordered_map>
#include <variant>
#include <memory>
#include <any>
#include <set>

namespace Mix {

	class AudioClip;
	class Music;
	class AudioFinishCallback;

	class AudioEngine
	{
	 public:
		uint8_t PlayAudio(AudioClip& clip, float volume, float pitch, bool loop, const std::any& userData, float x = 0, float y = 0, float depth = 0, float minDistance = 0, float maxDistance = 0);
		void PauseAudio(uint8_t playingID);
		void UnpauseAudio(uint8_t playingID);
		void StopAudio(uint8_t playingID);

		void SetAudioLoopState(uint8_t playingID, bool loop);
		void SetAudioMuteState(uint8_t playingID, bool mute);
		void SetAudioVolume(uint8_t playingID, float volume);
		void SetAudioPitch(uint8_t playingID, float pitch);
		void SetAudioPosition(uint8_t playingID, float x, float y, float depth, float minDistance, float maxDistance);
		void SetSpatializationMode(uint8_t playingID, bool isSpatialization);
        void SetAudioOffsetTime(uint8_t playingID, float time);

		float GetAudioOffsetTime(uint8_t playingID);

		void SetListenerPosition(float x, float y, float depth);
		void SetGlobalVolume(float volume);

		void SetAudioFinishCallback(AudioFinishCallback* callback);

		void Update(float deltaTime);

	 private:
		struct SoundEventData
		{
			const uint8_t playingID = 0;
			const float stopTime = 0;

			SoundEventData() = default;
			SoundEventData(uint8_t playingID, float stopTime)
				: playingID(playingID), stopTime(stopTime)
			{
			}

			bool operator<(const SoundEventData& other) const
			{
				if (stopTime == other.stopTime)
				{
					return playingID < other.playingID;
				}

				return stopTime < other.stopTime;
			}

		};

		struct Audio
		{
            sf::Sound sound;
            std::shared_ptr<Music> music = nullptr;

			const SoundEventData* event = nullptr;
			std::any userData;
			float previousVolume = 0;
            float previousTimeOffset = 0;
            bool isMute = false;

			Audio() = default;
            Audio(sf::Sound&& audio, const SoundEventData* event, const std::any& userData)
                    : sound(audio), event(event), userData(userData)
            {
            }
			Audio(std::shared_ptr<Music>&& audio, const SoundEventData* event, const std::any& userData)
				: music(audio), event(event), userData(userData)
			{
			}

            bool IsSoundValid() const { return sound.getBuffer() != nullptr; }
            bool IsMusicValid() const { return music != nullptr; }
		};

	 private:
		bool HasHitMaxAudioSources() const;
		float GetPlayingOffset(const sf::Sound& sound);
		float GetPlayingOffset(const std::shared_ptr<Music>& music);
		float GetDuration(const sf::Sound& sound);
		float GetDuration(const std::shared_ptr<Music>& music);

		uint8_t PlayAudioClip(AudioClip& clip, float volume, float pitch, bool loop, float x, float y, float depth, float minDistance, float maxDistance, const std::any& userData);
		uint8_t PlayStreamedAudioClip(AudioClip& clip, float volume, float pitch, bool loop, float x, float y, float depth, float minDistance, float maxDistance, const std::any& userData);

		uint8_t GetNextID();
		void ReturnID(uint8_t audioSourceID);

	 private:
		sf::Time m_CurrentTime;

		std::unordered_map<uint8_t, Audio> m_CurrentPlayingAudio;
		std::set<SoundEventData> m_AudioEventQueue;
		std::vector<uint8_t> m_UnusedIDs;

		AudioFinishCallback* m_Callback = nullptr;

		static constexpr uint8_t c_MaxAudioEmitters = 250;
		static constexpr uint8_t c_InvalidClip = 0;
		static constexpr uint8_t c_InvalidAudioSource = 0;
	};

} // Mix