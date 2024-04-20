#ifndef XNA_SOUND_SOUNDEFFECT_HPP
#define XNA_SOUND_SOUNDEFFECT_HPP

#include "../default.hpp"

namespace xna {
	struct ISoundEffectInstance {
	public:
		virtual void Play(bool loop = false) = 0;
		virtual void Stop(bool immediate = true) = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Volume(float volume) = 0;
		virtual void Pitch(float pitch) = 0;
		virtual void Pan(float pan) = 0;
		virtual bool IsLooped() = 0;
	};

	class ISoundEffect {
	public:
		virtual ~ISoundEffect(){}
		virtual void Play() = 0;
		virtual void Play(float volume, float pitch, float pan) = 0;
		virtual SoundEffectInstance CreateInstance() = 0;
	};
}

#endif