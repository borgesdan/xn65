#include "xna/platform/dx.hpp"

namespace xna {
	void AudioEngine::Initialize() {
		impl = unew<PlatformImplementation>();
	}

	bool AudioEngine::Reset() {
		if (!impl || !impl->_dxAudioEngine)
			return false;

		return impl->_dxAudioEngine->Reset();
	}

	bool AudioEngine::Update() {
		if (!impl || !impl->_dxAudioEngine)
			return false;

		return impl->_dxAudioEngine->Update();
	}

	bool AudioEngine::Resume() {
		if (!impl || !impl->_dxAudioEngine)
			return false;

		impl->_dxAudioEngine->Resume();

		return true;
	}

	bool AudioEngine::Suspend() {
		if (!impl || !impl->_dxAudioEngine)
			return false;

		impl->_dxAudioEngine->Resume();

		return true;
	}

	void AudioEngine::DefaultSampleRate(int value) {
		if (!impl || !impl->_dxAudioEngine)
			return;

		impl->_dxAudioEngine->SetDefaultSampleRate(value);
	}

	void AudioEngine::MasteringLimit(int limit, int loudness) {
		if (!impl || !impl->_dxAudioEngine)
			return;

		impl->_dxAudioEngine->SetMasteringLimit(limit, loudness);
	}

	void AudioEngine::MasterVolume(float value) {
		if (!impl || !impl->_dxAudioEngine)
			return;

		impl->_dxAudioEngine->SetMasterVolume(value);
	}

	void AudioEngine::Reverb(AudioReverb value) {
		if (!impl || !impl->_dxAudioEngine)
			return;

		const auto reverb = static_cast<DirectX::AUDIO_ENGINE_REVERB>(value);
		impl->_dxAudioEngine->SetReverb(reverb);
	}
}