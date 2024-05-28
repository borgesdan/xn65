#include "platform-dx/implementations.hpp"

using DxSoundEffect = DirectX::SoundEffect;

namespace xna {
	SoundEffectInstance::SoundEffectInstance() {
		impl = unew<PlatformImplementation>();		
	}

	SoundEffectInstance::~SoundEffectInstance() {
		impl = nullptr;
	}

	SoundEffect::SoundEffect() {
	}

	SoundEffect::SoundEffect(String const& fileName) {
		if (!AudioEngine::impl || !AudioEngine::impl->_dxAudioEngine)
			return;

		const auto file = XnaHToWString(fileName);
		impl->_dxSoundEffect = unew<DxSoundEffect>(AudioEngine::impl->_dxAudioEngine.get(), file.c_str());
	}

	SoundEffect::SoundEffect(
		std::vector<Byte> format,
		std::vector<Byte> data,
		Int loopStart,
		Int loopLength,
		TimeSpan const& duration) {
		if (!AudioEngine::impl || !AudioEngine::impl->_dxAudioEngine)
			return;

		auto wavData = unew<Byte[]>(data.size());
		for (size_t i = 0; i < data.size(); ++i)
			wavData[i] = data[i];

		auto wavFormat = reinterpret_cast<WAVEFORMATEX*>(format.data());
		auto startAudio = wavData.get() + sizeof(WAVEFORMATEX);
		/*auto se = new DxSoundEffect(
			AudioEngine::impl->_dxAudioEngine.get(),
			wavData,
			wavFormat,
			startAudio,
			data.size(),
			loopStart,
			loopLength);*/
		impl->_dxSoundEffect = unew<DxSoundEffect>(
			AudioEngine::impl->_dxAudioEngine.get(),
			wavData,
			wavFormat,
			startAudio,
			data.size(),
			loopStart,
			loopLength);
	}

	void SoundEffect::Play() {
		if (!impl->_dxSoundEffect)
			return;

		impl->_dxSoundEffect->Play();
	}

	void SoundEffect::Play(float volume, float pitch, float pan) {
		if (!impl->_dxSoundEffect)
			return;

		impl->_dxSoundEffect->Play(volume, pitch, pan);
	}

	uptr<SoundEffectInstance> SoundEffect::CreateInstance() {
		if (!impl->_dxSoundEffect)
			return unew<SoundEffectInstance>();

		auto instance = unew<SoundEffectInstance>();
		instance->impl->_dxInstance = impl->_dxSoundEffect->CreateInstance();

		return instance;
	}

	void SoundEffectInstance::Play(bool loop) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Play(loop);
	}

	void SoundEffectInstance::Stop(bool immediate) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Stop(immediate);
	}

	void SoundEffectInstance::Pause() {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Pause();
	}

	void SoundEffectInstance::Resume() {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->Resume();
	}

	void SoundEffectInstance::Volume(float volume) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->SetVolume(volume);
	}

	void SoundEffectInstance::Pitch(float pitch) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->SetPitch(pitch);
	}

	void SoundEffectInstance::Pan(float pan) {
		if (!impl->_dxInstance)
			return;

		impl->_dxInstance->SetPan(pan);
	}

	bool SoundEffectInstance::IsLooped() {
		if (!impl->_dxInstance)
			return false;

		return impl->_dxInstance->IsLooped();
	}
}