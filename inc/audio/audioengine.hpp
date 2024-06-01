#ifndef XNA_SOUND_SOUNDENGINE_HPP
#define XNA_SOUND_SOUNDENGINE_HPP

#include "../default.hpp"

namespace xna {
	class AudioEngine {
	public:		
		static bool Reset();
		static bool Resume();
		static bool Suspend();
		static bool Update();
		static void DefaultSampleRate(int value);
		static void MasterVolume(float value);
		static void MasteringLimit(int limit, int loudness);
		static void Reverb(AudioReverb value);

		static void Initialize();

	public:
		struct PlatformImplementation;
		inline static uptr<PlatformImplementation> impl = nullptr;

	private:
		AudioEngine();
		AudioEngine(AudioEngine&);
		AudioEngine(AudioEngine&&);
	};
}

#endif