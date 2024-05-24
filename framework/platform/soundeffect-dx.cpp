#include "platform-dx/soundeffect-dx.hpp"
#include "platform-dx/implementations.hpp"

using DxSoundEffect = DirectX::SoundEffect;

namespace xna {
	SoundEffect::SoundEffect(AudioEngine& audioEngine, String const& fileName) {
		if (!audioEngine.impl->_dxAudioEngine)
			return;

		const auto file = XnaHToWString(fileName);
		_dxSoundEffect = New<DxSoundEffect>(audioEngine.impl->_dxAudioEngine.get(), file.c_str());
	}

	void SoundEffect::Play() {
		if (!_dxSoundEffect)
			return;

		_dxSoundEffect->Play();
	}

	void SoundEffect::Play(float volume, float pitch, float pan) {
		if (!_dxSoundEffect)
			return;

		_dxSoundEffect->Play(volume, pitch, pan);		
	}

	SoundEffectInstance SoundEffect::CreateInstance() {
		if (!_dxSoundEffect)
			return SoundEffectInstance();

		SoundEffectInstance i{};
		i._dxInstance = _dxSoundEffect->CreateInstance();

		return i;
	}

	void SoundEffectInstance::Play(bool loop) {
		if (!_dxInstance)
			return;

		_dxInstance->Play(loop);
	}

	void SoundEffectInstance::Stop(bool immediate) {
		if (!_dxInstance)
			return;

		_dxInstance->Stop(immediate);
	}

	void SoundEffectInstance::Pause() {
		if (!_dxInstance)
			return;

		_dxInstance->Pause();
	}

	void SoundEffectInstance::Resume() {
		if (!_dxInstance)
			return;

		_dxInstance->Resume();
	}

	void SoundEffectInstance::Volume(float volume) {
		if (!_dxInstance)
			return;

		_dxInstance->SetVolume(volume);
	}

	void SoundEffectInstance::Pitch(float pitch) {
		if (!_dxInstance)
			return;

		_dxInstance->SetPitch(pitch);
	}

	void SoundEffectInstance::Pan(float pan) {
		if (!_dxInstance)
			return;

		_dxInstance->SetPan(pan);
	}

	bool SoundEffectInstance::IsLooped() {
		if (!_dxInstance)
			return false;

		return _dxInstance->IsLooped();
	}
}