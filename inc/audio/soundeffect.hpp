#ifndef XNA_SOUND_SOUNDEFFECT_HPP
#define XNA_SOUND_SOUNDEFFECT_HPP

#include "../default.hpp"

namespace xna {
	struct SoundEffectInstance {
	public:
		SoundEffectInstance();
		~SoundEffectInstance();

		void Play(bool loop = false);
		void Stop(bool immediate = true);
		void Pause();
		void Resume();
		void Volume(float volume);
		void Pitch(float pitch);
		void Pan(float pan);
		bool IsLooped();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};

	class SoundEffect {
	public:
		SoundEffect(AudioEngine& audioEngine, String const& fileName);
		~SoundEffect();
		void Play();
		void Play(float volume, float pitch, float pan);
		uptr<SoundEffectInstance> CreateInstance();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif