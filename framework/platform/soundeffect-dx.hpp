#ifndef XNA_PLATFORM_SOUNDEFFECT_DX_HPP
#define XNA_PLATFORM_SOUNDEFFECT_DX_HPP

#include "../audio/soundeffect.hpp"
#include <Audio.h>

namespace xna {
	struct SoundEffectInstance : public ISoundEffectInstance {
	public:
		SoundEffectInstance() = default;		

		virtual void Play(bool loop = false) override;
		virtual void Stop(bool immediate = true) override;
		virtual void Pause() override;
		virtual void Resume() override;
		virtual void Volume(float volume) override;
		virtual void Pitch(float pitch) override;
		virtual void Pan(float pan) override;
		virtual bool IsLooped() override;

	public:
		uptr<DirectX::SoundEffectInstance> _dxInstance = nullptr;	
	};	

	class SoundEffect : public ISoundEffect {
	public:
		SoundEffect(AudioEngine& audioEngine, String const& fileName);
		virtual void Play() override;
		virtual void Play(float volume, float pitch, float pan) override;
		virtual SoundEffectInstance CreateInstance() override;

	public:
		sptr<DirectX::SoundEffect> _dxSoundEffect = nullptr;
	};
}

#endif