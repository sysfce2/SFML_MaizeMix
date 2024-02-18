#pragma once

#include <SFML/Audio.hpp>

namespace Mix {

	class SoundReference;

	class Music : private sf::Music
	{
	 public:
		Music() = default;
		explicit Music(const SoundReference& soundReference);
		~Music();

		void Play();
		void Pause();
		void Stop();

		bool Load(const SoundReference& musicBuffer);

		void SetVolume(float volume);
		void SetPitch(float pitch);
		void SetLoop(bool loop);
		void SetPosition(float x, float y, float depth);
		void SetMinDistance(float distance);
		void SetMaxDistance(float distance);
		void SetPlayback(float seconds);
		void SetRelativeToListener(bool relative);

		const SoundReference* GetReference() const;
		float GetVolume() const;
		bool GetLoop() const;
		sf::Time GetPlayingOffset() const;
		sf::Time GetDuration() const;
        sf::SoundSource::Status GetStatus() const;
		bool IsRelativeToListener() const;

		void ResetReference();

	 private:
		const SoundReference* m_Reference = nullptr;
	};

} // Mix