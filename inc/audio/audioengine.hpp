#ifndef XNA_SOUND_SOUNDENGINE_HPP
#define XNA_SOUND_SOUNDENGINE_HPP

#include "../default.hpp"

namespace xna {
	class IAudioEngine {
	public:
		virtual ~IAudioEngine(){}	
		virtual bool Reset() = 0;
		virtual bool Resume() = 0;
		virtual bool Suspend() = 0;
		virtual bool Update() = 0;
		virtual void DefaultSampleRate(int value) = 0;
		virtual void MasterVolume(float value) = 0;
		virtual void MasteringLimit(int limit, int loudness) = 0;
		virtual void Reverb(AudioReverb value) = 0;
	};
}

#endif