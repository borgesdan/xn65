#ifndef XNA_SOUND_SOUNDENGINE_HPP
#define XNA_SOUND_SOUNDENGINE_HPP

#include "../default.hpp"

namespace xna {
	class AudioEngine {
	public:
		AudioEngine();
		~AudioEngine();
		bool Reset();
		bool Resume();
		bool Suspend();
		bool Update();
		void DefaultSampleRate(int value);
		void MasterVolume(float value);
		void MasteringLimit(int limit, int loudness);
		void Reverb(AudioReverb value);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif